//
#include "stdafx.h"
#include "lwDDSFile.h"
#include "lwErrorCode.h"
#include "lwGraphicsutil.h"
LW_BEGIN

LW_STD_IMPLEMENTATION(lwDDSFile)

lwDDSFile::lwDDSFile()
: _dev(0), _origin_tex(0), _dds_tex(0)
{
    _cubemap_flag = 0;
    _volume_depth = 0;
    _tex_width = 0;
    _tex_height = 0;
    _mip_level = D3DX_DEFAULT;
}

lwDDSFile::~lwDDSFile()
{
    LW_IF_RELEASE(_origin_tex);
    LW_IF_RELEASE(_dds_tex);
}

LW_RESULT lwDDSFile::Clear()
{
    _cubemap_flag = 0;
    _volume_depth = 0;
    _tex_width = 0;
    _tex_height = 0;
    _mip_level = D3DX_DEFAULT;

    LW_SAFE_RELEASE(_origin_tex);
    LW_SAFE_RELEASE(_dds_tex);

    return LW_RET_OK;
}

LW_RESULT lwDDSFile::LoadOriginTexture(const char* file, DWORD mip_level, D3DFORMAT format, DWORD colorkey)
{
    LW_RESULT ret = LW_RET_FAILED;

    IDirect3DTextureX* tex = 0;

    if(FAILED(D3DXCreateTextureFromFileEx(
        _dev, 
        file,
        D3DX_DEFAULT, 
        D3DX_DEFAULT,
        mip_level,
        0,
        format,
        D3DPOOL_MANAGED,
        D3DX_FILTER_POINT,
        D3DX_FILTER_POINT,
        colorkey,
        NULL,
        NULL,
        &tex)))
    {
        goto __ret;
    }
    
    {
        // Ensure that source image dimensions are power of 2
        D3DSURFACE_DESC sd;
        tex->GetLevelDesc(0, &sd);
        _tex_width = sd.Width;
        _tex_height = sd.Height;

        LONG w = sd.Width;
        LONG h = sd.Height;
        LONG pow_w = 1;
        LONG pow_h = 1;

        while ((w & 1) == 0)
        {
            pow_w++;
            w = w >> 1;
        }

        while ((h & 1) == 0)
        {
            pow_h++;
            h = h >> 1;
        }

        if (w != 1 || h != 1)
            goto __ret;

        _mip_level = D3DX_DEFAULT;

        LW_IF_RELEASE(_origin_tex);
        _origin_tex = tex;
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwDDSFile::Compress(D3DFORMAT new_fmt)
{
    LW_RESULT ret = LW_RET_FAILED;

    D3DFORMAT src_fmt;
    IDirect3DTextureX* pmiptex;
    IDirect3DCubeTextureX* pcubetex;
    IDirect3DVolumeTextureX* pvoltex;
    IDirect3DTextureX*  pmiptexNew;
    IDirect3DCubeTextureX* pcubetexNew;
    IDirect3DVolumeTextureX* pvoltexNew;

    if(IsVolumeMap())
    {
        if(new_fmt == D3DFMT_DXT1 ||
            new_fmt == D3DFMT_DXT2 ||
            new_fmt == D3DFMT_DXT3 ||
            new_fmt == D3DFMT_DXT4 ||
            new_fmt == D3DFMT_DXT5)
        {
            goto __ret;
        }
        pvoltex = (IDirect3DVolumeTextureX*)_origin_tex;
        D3DVOLUME_DESC vd;
        pvoltex->GetLevelDesc(0, &vd);
        src_fmt = vd.Format;
    }
    else if(IsCubeMap())
    {
        pcubetex = (IDirect3DCubeTextureX*)_origin_tex;
        D3DSURFACE_DESC sd;
        pcubetex->GetLevelDesc(0, &sd);
        src_fmt = sd.Format;
    }
    else
    {
        pmiptex = (IDirect3DTextureX* )_origin_tex;
        D3DSURFACE_DESC sd;
        pmiptex->GetLevelDesc(0, &sd);
        src_fmt = sd.Format;
    }

    if(src_fmt == D3DFMT_DXT2 || src_fmt == D3DFMT_DXT4)
    {
        if(new_fmt == D3DFMT_DXT1)
            goto __ret;
        else if(new_fmt != D3DFMT_DXT2 && new_fmt != D3DFMT_DXT4)
            goto __ret;
    }

    if(IsVolumeMap())
    {
        if(FAILED(_dev->CreateVolumeTextureX(
            _tex_width,
            _tex_height,
            _volume_depth,
            _mip_level,
            0,
            new_fmt,
            D3DPOOL_SYSTEMMEM,
            &pvoltexNew,
            NULL)))
        {
            goto __ret;
        }

        LW_IF_RELEASE(_dds_tex);
        _dds_tex = pvoltexNew;

        if(FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, _origin_tex, _dds_tex)))
            goto __ret;
    }
    else if(IsCubeMap())
    {
        if(FAILED(_dev->CreateCubeTextureX(
            _tex_width,
            _mip_level, 
            0,
            new_fmt,
            D3DPOOL_MANAGED,
            &pcubetexNew,
            NULL)))
        {
            goto __ret;
        }

        LW_IF_RELEASE(_dds_tex);
        _dds_tex = pcubetexNew;

        if(FAILED(BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_X, _origin_tex, _dds_tex)))
            goto __ret;
        if(FAILED(BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_X, _origin_tex, _dds_tex)))
            goto __ret;
        if(FAILED(BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_Y, _origin_tex, _dds_tex)))
            goto __ret;
        if(FAILED(BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_Y, _origin_tex, _dds_tex)))
            goto __ret;
        if(FAILED(BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_Z, _origin_tex, _dds_tex)))
            goto __ret;
        if(FAILED(BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_Z, _origin_tex, _dds_tex)))
            goto __ret;
    }
    else
    {
        if(FAILED(_dev->CreateTextureX(
            _tex_width,
            _tex_height,
            _mip_level, 
            0,
            new_fmt,
            D3DPOOL_MANAGED,
            &pmiptexNew,
            NULL)))
        {
            goto __ret;
        }

        LW_IF_RELEASE(_dds_tex);
        _dds_tex = pmiptexNew;

        if(FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, _origin_tex, _dds_tex)))
            goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}
HRESULT lwDDSFile::BltAllLevels(D3DCUBEMAP_FACES FaceType, IDirect3DBaseTextureX* ptexSrc, IDirect3DBaseTextureX* ptexDest)
{
    IDirect3DTextureX*  pmiptexSrc{};
    IDirect3DTextureX*  pmiptexDest{};
    IDirect3DCubeTextureX* pcubetexSrc{};
    IDirect3DCubeTextureX* pcubetexDest{};
    IDirect3DVolumeTextureX* pvoltexSrc{};
    IDirect3DVolumeTextureX* pvoltexDest{};
    IDirect3DSurfaceX* psurfSrc{};
    IDirect3DSurfaceX* psurfDest{};
    IDirect3DVolumeX* pvolSrc{};
    IDirect3DVolumeX* pvolDest{};
    DWORD iLevel;

    if(IsVolumeMap())
    {
        pvoltexSrc = (IDirect3DVolumeTextureX*)ptexSrc;
        pvoltexDest = (IDirect3DVolumeTextureX*)ptexDest;
    }
    else if(IsCubeMap())
    {
        pcubetexSrc = (IDirect3DCubeTextureX*)ptexSrc;
        pcubetexDest = (IDirect3DCubeTextureX*)ptexDest;
    }
    else
    {
        pmiptexSrc = (IDirect3DTextureX* )ptexSrc;
        pmiptexDest = (IDirect3DTextureX* )ptexDest;
    }

    for(iLevel = 0; iLevel < _mip_level; iLevel++)
    {
        if(IsVolumeMap())
        {
            pvoltexSrc->GetVolumeLevel(iLevel, &pvolSrc);
            pvoltexDest->GetVolumeLevel(iLevel, &pvolDest);
            D3DXLoadVolumeFromVolume(pvolDest, NULL, NULL, 
                pvolSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
            pvolSrc->Release();
            pvolDest->Release();
        }
        if(IsCubeMap())
        {
            pcubetexSrc->GetCubeMapSurface(FaceType, iLevel, &psurfSrc);
            pcubetexDest->GetCubeMapSurface(FaceType, iLevel, &psurfDest);
            D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
                psurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
            psurfSrc->Release();
            psurfDest->Release();
        }
        else
        {
            pmiptexSrc->GetSurfaceLevel(iLevel, &psurfSrc);
            pmiptexDest->GetSurfaceLevel(iLevel, &psurfDest);

            HRESULT hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
                psurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);

            psurfSrc->Release();
            psurfDest->Release();

        }
    }

    return LW_RET_OK;
}


LW_RESULT lwDDSFile::Convert(const char* file, D3DFORMAT src_fmt, D3DFORMAT dds_fmt, DWORD mip_level, DWORD src_colorkey)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_FAILED(LoadOriginTexture(file, mip_level, src_fmt, src_colorkey)))
        goto __ret;

    if(LW_FAILED(Compress(dds_fmt)))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwDDSFile::Save(const char* file)
{
    LW_RESULT ret = LW_RET_FAILED;

    FILE* fp = 0;

    IDirect3DBaseTextureX* ptex = (_dds_tex == NULL ? _origin_tex : _dds_tex);

    if((fp = fopen(file, "wb")) == 0)
        goto __ret;

    if(LW_FAILED(SaveDDSHeader(ptex, fp)))
        goto __ret;

    if(IsVolumeMap())
    {
        if(FAILED(SaveAllVolumeSurfaces((IDirect3DVolumeTextureX*)ptex, fp)))
            goto __ret;
    }
    else if(IsCubeMap())
    {
        if(FAILED(SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_POSITIVE_X, fp)))
            goto __ret;
        if(FAILED(SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_NEGATIVE_X, fp)))
            goto __ret;
        if(FAILED(SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_POSITIVE_Y, fp)))
            goto __ret;
        if(FAILED(SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_NEGATIVE_Y, fp)))
            goto __ret;
        if(FAILED(SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_POSITIVE_Z, fp)))
            goto __ret;
        if(FAILED(SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_NEGATIVE_Z, fp)))
            goto __ret;
    }
    else // generic texture
    {
        if(FAILED(SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_FORCE_DWORD, fp)))
            goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    if(fp)
    {
        fclose(fp);
    }
    return ret;

}
HRESULT lwDDSFile::SaveDDSHeader(IDirect3DBaseTextureX* tex, FILE* fp)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwDDSHeader ddsh;
    DWORD dwMagic;
    D3DFORMAT fmt;
    DWORD size;
    DWORD dwPitch = 0;

    dwMagic = MAKEFOURCC('D','D','S',' ');

    // Fill in the DDS header structure
    ZeroMemory(&ddsh, sizeof(ddsh));
    ddsh.size = sizeof(ddsh);
    ddsh.header_flag = DDS_HEADER_FLAGS_TEXTURE;
    ddsh.width = _tex_width;
    ddsh.height = _tex_height;
    ddsh.surface_flag = DDS_SURFACE_FLAGS_TEXTURE;
    if(_mip_level > 1)
    {
        ddsh.header_flag |= DDS_HEADER_FLAGS_MIPMAP;
        ddsh.surface_flag |= DDS_SURFACE_FLAGS_MIPMAP;
        ddsh.mipmap_count = _mip_level;
    }
    if(IsVolumeMap())
    {
        ddsh.header_flag |= DDS_HEADER_FLAGS_VOLUME;
        // Note: "cubemap_flag" is mostly forcubemap flags,
        // but in this case is also used to indicate a volume texture
        ddsh.cubemap_flag |= DDS_FLAGS_VOLUME;
        ddsh.volume_depth = _volume_depth;
    }
    
    // In DX7, you could (optionally) use and save a subset of 
    // cubemap faces.  In DX8, you must use and save all faces of a cubemap.
    if(IsCubeMap())
    {
        ddsh.surface_flag |= DDS_SURFACE_FLAGS_CUBEMAP;
        ddsh.cubemap_flag = DDS_CUBEMAP_ALLFACES;
    }

    // Determine dwPitch and size forthe texture
    if(IsVolumeMap())
    {
        D3DVOLUME_DESC vd;
        D3DLOCKED_BOX lb;
        IDirect3DVolumeTextureX* pvoltex = (IDirect3DVolumeTextureX*)tex;
        pvoltex->GetLevelDesc(0, &vd);
        fmt = vd.Format;
        size = lwGetSurfaceSize(vd.Width, vd.Height, vd.Format);
        if(SUCCEEDED(pvoltex->LockBox(0, &lb, NULL, D3DLOCK_READONLY)))
        {
            dwPitch = lb.RowPitch;
            pvoltex->UnlockBox(0);
        }
    }
    else if(IsCubeMap())
    {
        D3DSURFACE_DESC sd;
        D3DLOCKED_RECT lr;
        IDirect3DCubeTextureX* pcubetex = (IDirect3DCubeTextureX*)tex;
        pcubetex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
        size = lwGetSurfaceSize(sd.Width, sd.Height, sd.Format);
        if(SUCCEEDED(pcubetex->LockRect(D3DCUBEMAP_FACE_POSITIVE_X, 0, 
            &lr, NULL, D3DLOCK_READONLY)))
        {
            dwPitch = lr.Pitch;
            pcubetex->UnlockRect(D3DCUBEMAP_FACE_POSITIVE_X, 0);
        }
    }
    else // generic texture
    {
        D3DSURFACE_DESC sd;
        D3DLOCKED_RECT lr;
        IDirect3DTextureX*  pmiptex = (IDirect3DTextureX* )tex;
        pmiptex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
        size = lwGetSurfaceSize(sd.Width, sd.Height, sd.Format);
        if(SUCCEEDED(pmiptex->LockRect(0, &lr, NULL, D3DLOCK_READONLY)))
        {
            dwPitch = lr.Pitch;
            pmiptex->UnlockRect(0);
        }
    }

    // Note that although this code stores the pitch or linear size
    // of the surface in the DDS file (forconsistency with DX7), it
    // is better to use the pitch or linear size of the surface
    // created when loading the DDS file into a texture than to use
    // the file values.  The pitch of a created surface, especially, 
    // can be different from the pitch stored in the DDS file.
    switch (fmt)
    {
    case D3DFMT_DXT1:
        ddsh.ddspf = DDSPF_DXT1;
        ddsh.header_flag |= DDS_HEADER_FLAGS_LINEARSIZE;
        ddsh.pitch_or_linearsize = size;
        break;
    case D3DFMT_DXT2:
        ddsh.ddspf = DDSPF_DXT2;
        ddsh.header_flag |= DDS_HEADER_FLAGS_LINEARSIZE;
        ddsh.pitch_or_linearsize = size;
        break;
    case D3DFMT_DXT3:
        ddsh.ddspf = DDSPF_DXT3;
        ddsh.header_flag |= DDS_HEADER_FLAGS_LINEARSIZE;
        ddsh.pitch_or_linearsize = size;
        break;
    case D3DFMT_DXT4:
        ddsh.ddspf = DDSPF_DXT4;
        ddsh.header_flag |= DDS_HEADER_FLAGS_LINEARSIZE;
        ddsh.pitch_or_linearsize = size;
        break;
    case D3DFMT_DXT5:
        ddsh.ddspf = DDSPF_DXT5;
        ddsh.header_flag |= DDS_HEADER_FLAGS_LINEARSIZE;
        ddsh.pitch_or_linearsize = size;
        break;
    case D3DFMT_A8R8G8B8:
        ddsh.ddspf = DDSPF_A8R8G8B8;
        ddsh.header_flag |= DDS_HEADER_FLAGS_PITCH;
        ddsh.pitch_or_linearsize = dwPitch;
        break;
    case D3DFMT_A1R5G5B5:
        ddsh.ddspf = DDSPF_A1R5G5B5;
        ddsh.header_flag |= DDS_HEADER_FLAGS_PITCH;
        ddsh.pitch_or_linearsize = dwPitch;
        break;
    case D3DFMT_A4R4G4B4:
        ddsh.ddspf = DDSPF_A4R4G4B4;
        ddsh.header_flag |= DDS_HEADER_FLAGS_PITCH;
        ddsh.pitch_or_linearsize = dwPitch;
        break;
    case D3DFMT_R8G8B8:
        ddsh.ddspf = DDSPF_R8G8B8;
        ddsh.header_flag |= DDS_HEADER_FLAGS_PITCH;
        ddsh.pitch_or_linearsize = dwPitch;
        break;
    case D3DFMT_R5G6B5:
        ddsh.ddspf = DDSPF_R5G6B5;
        ddsh.header_flag |= DDS_HEADER_FLAGS_PITCH;
        ddsh.pitch_or_linearsize = dwPitch;
        break;
    default:
        goto __ret;
    }

    fwrite(&dwMagic, sizeof(dwMagic), 1, fp);
    fwrite(&ddsh, sizeof(ddsh), 1, fp);

    ret = LW_RET_OK;
__ret:
    return ret;

}

HRESULT lwDDSFile::SaveAllMipSurfaces(IDirect3DBaseTextureX* ptex, D3DCUBEMAP_FACES FaceType, FILE* fp)
{
    HRESULT hr;
    IDirect3DSurfaceX* psurf;
    D3DSURFACE_DESC sd;
    UINT iLevel;
    D3DLOCKED_RECT lr;
    IDirect3DTextureX*  pmiptex = NULL;
    IDirect3DCubeTextureX* pcubetex = NULL;

    if(FaceType == D3DCUBEMAP_FACE_FORCE_DWORD)
        pmiptex = (IDirect3DTextureX* )ptex;
    else
        pcubetex = (IDirect3DCubeTextureX*)ptex;

    for(iLevel = 0; iLevel < _mip_level; iLevel++)
    {
        if(pmiptex != NULL)
            hr = pmiptex->GetSurfaceLevel(iLevel, &psurf);
        else
            hr = pcubetex->GetCubeMapSurface(FaceType, iLevel, &psurf);

        if(FAILED(hr))
            return hr;

        psurf->GetDesc(&sd);

        if(pmiptex != NULL)
            hr = pmiptex->LockRect(iLevel, &lr, NULL, 0);
        else
            hr = pcubetex->LockRect(FaceType, iLevel, &lr, NULL, 0);
        if(FAILED(hr))
            return hr;
        if(sd.Format == D3DFMT_DXT1 ||
            sd.Format == D3DFMT_DXT2 ||
            sd.Format == D3DFMT_DXT3 ||
            sd.Format == D3DFMT_DXT4 ||
            sd.Format == D3DFMT_DXT5)
        {
            fwrite(lr.pBits, lwGetSurfaceSize(sd.Width, sd.Height, sd.Format), 1, fp);
        }
        else
        {
            DWORD yp;
            BYTE* pbDest = (BYTE*)lr.pBits;
            LONG dataBytesPerRow = 0;
            if(sd.Format == D3DFMT_A8R8G8B8)
                dataBytesPerRow = 4 * sd.Width;
            else if(sd.Format == D3DFMT_R8G8B8)
                dataBytesPerRow = 3 * sd.Width;
            else
                dataBytesPerRow = 2 * sd.Width;
            for(yp = 0; yp < sd.Height; yp++)
            {
                fwrite(pbDest, dataBytesPerRow, 1, fp);
                pbDest += lr.Pitch;
            }
        }
        if(pmiptex != NULL)
            hr = pmiptex->UnlockRect(iLevel);
        else
            hr = pcubetex->UnlockRect(FaceType, iLevel);

        LW_IF_RELEASE(psurf);
    }

    return S_OK;
}


HRESULT lwDDSFile::SaveAllVolumeSurfaces(IDirect3DVolumeTextureX* pvoltex, FILE* fp)
{
    HRESULT hr;
    D3DVOLUME_DESC vd;
    D3DBOX box;
    D3DLOCKED_BOX lb;
    UINT iLevel;
    UINT numBytesPerRow;
    BYTE* pbSlice;
    BYTE* pbRow;
    UINT zp;
    UINT yp;

    for(iLevel = 0; iLevel < _mip_level; iLevel++)
    {
        pvoltex->GetLevelDesc(iLevel, &vd);
        box.Left = 0;
        box.Right = vd.Width;
        box.Top = 0;
        box.Bottom = vd.Height;
        box.Front = 0;
        box.Back = vd.Depth;
        hr = pvoltex->LockBox(iLevel, &lb, &box, 0);
        if(FAILED(hr))
            return hr;
        switch(vd.Format)
        {
        case D3DFMT_A8R8G8B8:
            numBytesPerRow = 4 * vd.Width;
            break;
        case D3DFMT_R8G8B8:
            numBytesPerRow = 3 * vd.Width;
            break;
        case D3DFMT_A1R5G5B5:
        case D3DFMT_A4R4G4B4:
        case D3DFMT_R5G6B5:
            numBytesPerRow = 2 * vd.Width;
            break;
        default:
            return E_FAIL;
        }
        pbSlice = (BYTE*)lb.pBits;
        for(zp = 0; zp < vd.Depth; zp++)
        {
            pbRow = pbSlice;
            for(yp = 0; yp < vd.Height; yp++)
            {
                fwrite(pbRow, numBytesPerRow, 1, fp);
                pbRow += lb.RowPitch;
            }
            pbSlice += lb.SlicePitch;
        }

        pvoltex->UnlockBox(iLevel);
    }

    return S_OK;
}



LW_END
