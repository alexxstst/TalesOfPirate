#include "stdafx.h"
#include "FonsDx9Backend.h"

#include "EffectFile.h"
#include "MPRender.h"
#include "lwIUtil.h"
#include "lwInterface.h"
#include "logutil.h"

#include <cstring>
#include <vector>

#include "fontstash.h"

using MindPower::lwITex;

namespace fons {

namespace {

// Вертекс под текущий FontVertex/pipeline FontRender'а: XYZRHW + Diffuse + UV1.
struct FontVertex {
    float X, Y, Z, Rhw;
    DWORD Color;
    float U, V;
};

constexpr DWORD kFontFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

lwITex* CreateAtlas(MPRender* dev, int width, int height) {
    if (!dev || width <= 0 || height <= 0) {
        return nullptr;
    }
    lwTexInfo info{};
    info.stage         = 0;
    info.type          = TEX_TYPE_SIZE;
    info.level         = 1;
    info.usage         = 0;
    info.format        = D3DFMT_A8R8G8B8;
    info.pool          = D3DPOOL_MANAGED;
    info.colorkey_type = COLORKEY_TYPE_NONE;
    info.width         = width;
    info.height        = height;

    lwITex* tex = nullptr;
    lwIResourceMgr* resMgr = dev->GetInterfaceMgr()->res_mgr;
    if (LW_RESULT r = lwLoadTex(&tex, resMgr, &info); LW_FAILED(r)) {
        ToLogService("errors", LogLevel::Error,
                     "[{}] lwLoadTex failed: width={}, height={}, ret={}",
                     __FUNCTION__, width, height, static_cast<long long>(r));
        return nullptr;
    }
    return tex;
}

// renderCreate: fontstash вызывает один раз после fonsCreateInternal, чтобы
// backend подготовил GPU-ресурсы под стартовый размер атласа.
int Dx9RenderCreate(void* uptr, int width, int height) {
    auto* backend = static_cast<Dx9Backend*>(uptr);
    if (!backend) {
        return 0;
    }
    if (backend->Atlas) {
        // Повторный вызов возможен через fonsResetAtlas — пересоздаём.
        SAFE_RELEASE(backend->Atlas);
    }
    backend->Atlas  = CreateAtlas(backend->Dev, width, height);
    backend->Width  = width;
    backend->Height = height;
    return backend->Atlas ? 1 : 0;
}

// renderResize: fontstash вызывает при fonsExpandAtlas — новый размер всегда
// ≥ старого. Пересоздаём текстуру; содержимое заливается через renderUpdate
// сразу после.
int Dx9RenderResize(void* uptr, int width, int height) {
    auto* backend = static_cast<Dx9Backend*>(uptr);
    if (!backend) {
        return 0;
    }
    if (backend->Atlas) {
        SAFE_RELEASE(backend->Atlas);
    }
    backend->Atlas  = CreateAtlas(backend->Dev, width, height);
    backend->Width  = width;
    backend->Height = height;
    return backend->Atlas ? 1 : 0;
}

// renderUpdate: data — полный CPU-буфер атласа (width*height байт), rect —
// dirty sub-rect [minx, miny, maxx, maxy). Копируем только sub-rect.
void Dx9RenderUpdate(void* uptr, int* rect, const unsigned char* data) {
    auto* backend = static_cast<Dx9Backend*>(uptr);
    if (!backend || !backend->Atlas || !data || !rect) {
        return;
    }
    const int w  = backend->Width;
    const int x0 = rect[0];
    const int y0 = rect[1];
    const int x1 = rect[2];
    const int y1 = rect[3];
    if (x1 <= x0 || y1 <= y0) {
        return;
    }

    RECT subRect{ x0, y0, x1, y1 };
    D3DLOCKED_RECT lr{};
    // LockRect с subRect — D3D отдаёт pBits уже смещённым на (x0,y0).
    if (HRESULT hr = backend->Atlas->GetTex()->LockRect(0, &lr, &subRect, 0); FAILED(hr)) {
        ToLogService("errors", LogLevel::Warning,
                     "[{}] LockRect failed: rect=({},{}-{},{}), hr=0x{:08X}",
                     __FUNCTION__, x0, y0, x1, y1, static_cast<std::uint32_t>(hr));
        return;
    }

    const int copyW = x1 - x0;
    const int copyH = y1 - y0;
    for (int y = 0; y < copyH; ++y) {
        DWORD* dst = reinterpret_cast<DWORD*>(static_cast<BYTE*>(lr.pBits) + y * lr.Pitch);
        const unsigned char* src = data + (y0 + y) * w + x0;
        for (int x = 0; x < copyW; ++x) {
            const BYTE a = src[x];
            dst[x] = a > 0 ? ((static_cast<DWORD>(a) << 24) | 0x00FFFFFFu) : 0u;
        }
    }
    backend->Atlas->GetTex()->UnlockRect(0);
}

// renderDraw: fontstash даёт плоский массив вершин (verts[x,y], tcoords[s,t],
// colors[rgba]), nverts — общее число. Строим FontVertex и рисуем DX9
// fixed-function pipeline'ом (alpha-blend + modulate texture×diffuse). Effect
// не используется намеренно: после DX10-миграции ранее использовавшаяся
// Technique(5) может быть недоступна / давать пустой рендер; fixed-function
// путь не зависит от шейдеров и даёт предсказуемый результат.
void Dx9RenderDraw(void* uptr, const float* verts, const float* tcoords,
                   const unsigned int* colors, int nverts) {
    auto* backend = static_cast<Dx9Backend*>(uptr);
    if (!backend || !backend->Dev || !backend->Atlas || nverts <= 0) {
        return;
    }
    IDirect3DDevice9* dev = backend->Dev->GetDevice();
    if (!dev) {
        return;
    }

    std::vector<FontVertex> out;
    out.resize(static_cast<size_t>(nverts));
    for (int i = 0; i < nverts; ++i) {
        out[i].X     = verts[i * 2 + 0] - 0.5f; // half-pixel для XYZRHW + LINEAR
        out[i].Y     = verts[i * 2 + 1] - 0.5f;
        out[i].Z     = 0.0f;
        out[i].Rhw   = 1.0f;
        out[i].Color = colors[i];
        out[i].U     = tcoords[i * 2 + 0];
        out[i].V     = tcoords[i * 2 + 1];
    }

    dev->SetVertexShader(nullptr);
    dev->SetPixelShader(nullptr);
    dev->SetFVF(kFontFVF);

    dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    dev->SetRenderState(D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA);
    dev->SetRenderState(D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA);
    dev->SetRenderState(D3DRS_ZENABLE,          FALSE);
    dev->SetRenderState(D3DRS_ZWRITEENABLE,     FALSE);
    dev->SetRenderState(D3DRS_CULLMODE,         D3DCULL_NONE);
    dev->SetRenderState(D3DRS_LIGHTING,         FALSE);
    dev->SetRenderState(D3DRS_FOGENABLE,        FALSE);
    dev->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE);

    dev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    dev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
    dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    dev->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
    dev->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

    dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    dev->SetSamplerState(0, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP);
    dev->SetSamplerState(0, D3DSAMP_ADDRESSV,  D3DTADDRESS_CLAMP);

    dev->SetTexture(0, backend->Atlas->GetTex());

    dev->DrawPrimitiveUP(
        D3DPT_TRIANGLELIST,
        static_cast<UINT>(nverts / 3),
        out.data(),
        sizeof(FontVertex));
}

void Dx9RenderDelete(void* uptr) {
    auto* backend = static_cast<Dx9Backend*>(uptr);
    if (!backend) {
        return;
    }
    // Намеренно НЕ освобождаем Atlas. renderDelete вызывается только из
    // fonsDeleteInternal, а тот — только в деструкторе FontManager на shutdown.
    // К этому моменту D3D-девайс и lwIResourceMgr могут быть уже уничтожены
    // (порядок статической деструкции недетерминирован) — Release в этих
    // условиях падает access violation'ом. Один leak на exit безвреден,
    // ОС очистит адресное пространство. Для live-сценариев (ресайз атласа)
    // освобождение происходит в Dx9RenderResize/Dx9RenderCreate перед
    // пересозданием — там D3D-девайс ещё валиден.
    backend->Atlas  = nullptr;
    backend->Width  = 0;
    backend->Height = 0;
}

} // namespace

void FillParams(FONSparams& params, Dx9Backend* backend, int width, int height) {
    std::memset(&params, 0, sizeof(params));
    params.width        = width;
    params.height       = height;
    // TOP_LEFT — y растёт вниз, как в D3D screen-space (XYZRHW).
    params.flags        = static_cast<unsigned char>(FONS_ZERO_TOPLEFT);
    params.userPtr      = backend;
    params.renderCreate = &Dx9RenderCreate;
    params.renderResize = &Dx9RenderResize;
    params.renderUpdate = &Dx9RenderUpdate;
    params.renderDraw   = &Dx9RenderDraw;
    params.renderDelete = &Dx9RenderDelete;
}

FONScontext* CreateContext(Dx9Backend* backend, int width, int height) {
    if (!backend) {
        return nullptr;
    }
    FONSparams params{};
    FillParams(params, backend, width, height);
    return fonsCreateInternal(&params);
}

} // namespace fons
