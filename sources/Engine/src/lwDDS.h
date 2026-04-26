#pragma once
// dds.h
// This header defines constants and structures that are useful when parsing 
// DDS files.  DDS files were originally designed to use several structures
// and constants that are native to DirectDraw and are defined in ddraw.h,
// such as DDSURFACEDESC2 and DDSCAPS2.  This file defines similar 
// (compatible) constants and structures so that one can use DDS files 
// without needing to include ddraw.h.

// this file is copyed from projection dxtex of directx samples
// variable naming was changed to our convention
// by lsh


struct lwDDSPixelFormat {
	DWORD size;
	DWORD flag;
	DWORD four_cc;
	DWORD bit_count;
	DWORD bitmask_r;
	DWORD bitmask_g;
	DWORD bitmask_b;
	DWORD bitmask_a;
};

#define DDS_FOURCC 0x00000004  // DDPF_FOURCC
#define DDS_RGB    0x00000040  // DDPF_RGB
#define DDS_RGBA   0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS

const lwDDSPixelFormat DDSPF_DXT1 =
	{sizeof(lwDDSPixelFormat), DDS_FOURCC, MAKEFOURCC('D', 'X', 'T', '1'), 0, 0, 0, 0, 0};

const lwDDSPixelFormat DDSPF_DXT2 =
	{sizeof(lwDDSPixelFormat), DDS_FOURCC, MAKEFOURCC('D', 'X', 'T', '2'), 0, 0, 0, 0, 0};

const lwDDSPixelFormat DDSPF_DXT3 =
	{sizeof(lwDDSPixelFormat), DDS_FOURCC, MAKEFOURCC('D', 'X', 'T', '3'), 0, 0, 0, 0, 0};

const lwDDSPixelFormat DDSPF_DXT4 =
	{sizeof(lwDDSPixelFormat), DDS_FOURCC, MAKEFOURCC('D', 'X', 'T', '4'), 0, 0, 0, 0, 0};

const lwDDSPixelFormat DDSPF_DXT5 =
	{sizeof(lwDDSPixelFormat), DDS_FOURCC, MAKEFOURCC('D', 'X', 'T', '5'), 0, 0, 0, 0, 0};

const lwDDSPixelFormat DDSPF_A8R8G8B8 =
	{sizeof(lwDDSPixelFormat), DDS_RGBA, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000};

const lwDDSPixelFormat DDSPF_A1R5G5B5 =
	{sizeof(lwDDSPixelFormat), DDS_RGBA, 0, 16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000};

const lwDDSPixelFormat DDSPF_A4R4G4B4 =
	{sizeof(lwDDSPixelFormat), DDS_RGBA, 0, 16, 0x0000f000, 0x000000f0, 0x0000000f, 0x0000f000};

const lwDDSPixelFormat DDSPF_R8G8B8 =
	{sizeof(lwDDSPixelFormat), DDS_RGB, 0, 24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000};

const lwDDSPixelFormat DDSPF_R5G6B5 =
	{sizeof(lwDDSPixelFormat), DDS_RGB, 0, 16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000};

#define DDS_HEADER_FLAGS_TEXTURE    0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT 
#define DDS_HEADER_FLAGS_MIPMAP     0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME     0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH      0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE 0x00080000  // DDSD_LINEARSIZE

#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000 // DDSCAPS_TEXTURE
#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008 // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 // DDSCAPS_COMPLEX

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME


struct lwDDSHeader {
	DWORD size;
	DWORD header_flag;
	DWORD height;
	DWORD width;
	DWORD pitch_or_linearsize;
	DWORD volume_depth; // only if DDS_HEADER_FLAGS_VOLUME is set in header_flag
	DWORD mipmap_count;
	DWORD reserved1[11];
	lwDDSPixelFormat ddspf;
	DWORD surface_flag;
	DWORD cubemap_flag;
	DWORD reserved2[3];
};
