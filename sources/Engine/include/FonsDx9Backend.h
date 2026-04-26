#pragma once

// DX9-бэкенд fontstash.
//
// fontstash управляет CPU-буфером атласа (один байт на пиксель, grayscale) и
// зовёт 5 колбеков через FONSparams. Этот файл реализует их поверх lwITex
// (A8R8G8B8: RGB=white, A=grayscale) и FontVertex/DrawPrimitiveUP — тот же
// pipeline, что уже используется в FontRender.
//
// Владение: Dx9Backend создаёт пользователь (обычно FontManager). FONScontext
// создаётся через fons::CreateContext(backend, w, h) и хранит weak-указатель
// на Dx9Backend. Backend должен жить дольше контекста.

#include "lwHeader.h"

class MPRender;
class CMPEffectFile;

struct FONScontext;
struct FONSparams;

LW_BEGIN
	class lwITex;
LW_END

namespace fons {
	struct Dx9Backend {
		MPRender* Dev = nullptr;
		CMPEffectFile* Effect = nullptr;
		// Индекс HLSL-техники в Effect (alpha-blend SrcAlpha/InvSrcAlpha,
		// modulate texture×diffuse, sampler POINT, Z off). По умолчанию 5 —
		// совпадает с FontRender::_renderIdx.
		int Technique = 5;
		int Width = 0;
		int Height = 0;
		MindPower::lwITex* Atlas = nullptr;
	};

	// Заполнить FONSparams указателями на колбеки + userPtr=backend. width/height —
	// начальный размер атласа. Не создаёт FONScontext — для этого вызывать Fons
	// напрямую (CreateContext) или передавать params в fonsCreateInternal.
	void FillParams(FONSparams& params, Dx9Backend* backend, int width, int height);

	// Convenience-обёртка: заполняет params и вызывает fonsCreateInternal.
	// Возвращает nullptr при ошибке (renderCreate провалился и т.п.).
	FONScontext* CreateContext(Dx9Backend* backend, int width, int height);
} // namespace fons
