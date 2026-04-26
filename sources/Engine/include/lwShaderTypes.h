//
#pragma once

#include "lwHeader.h"
#include "lwDirectX.h"
#include "lwStdInc.h"

LW_BEGIN
	enum lwVertexShaderRegisterInput {
		VSREG_V_POSITION = 0,
		VSREG_V_BLENDWEIGHT = 1,
		VSREG_V_BLENDINDICES = 2,
		VSREG_V_NORMAL = 3,
		VSREG_V_PSIZE = 4,
		VSREG_V_DIFFUSE = 5,
		VSREG_V_SPECULAR = 6,
		VSREG_V_TEXCOORD0 = 7,
		VSREG_V_TEXCOORD1 = 8,
		VSREG_V_TEXCOORD2 = 9,
		VSREG_V_TEXCOORD3 = 10,
		VSREG_V_TEXCOORD4 = 11,
		VSREG_V_TEXCOORD5 = 12,
		VSREG_V_TEXCOORD6 = 13,
		VSREG_V_TEXCOORD7 = 14,
		VSREG_V_POSITION2 = 15,
		VSREG_V_NORMAL2 = 16
	};

	enum lwVertexDeclarationTypesEnum {
		// vertex declaration
		__begin_decl_type = 0,
		VDT_PD = 0,
		VDT_PDT0,
		VDT_PDT1,
		VDT_PDT2,
		VDT_PDT3,
		VDT_PNT0,
		VDT_PND,
		VDT_PNDT0,
		VDT_PNDT1,
		VDT_PNDT2,
		VDT_PNDT3,

		VDT_PU4NT0,
		VDT_PB1U4NT0,
		VDT_PB2U4NT0,
		VDT_PB3U4NT0,

		VDT_EFF_134,
		VDT_EFF_2,
		VDT_EFF_SHADE,
		VDT_EFF_MINIMAP,

		VDT_PB4U4NT0_DX9,

		VDT_USER_DEFINE = 0xff,

		VDT_INVALID = LW_INVALID_INDEX
	};

	enum lwVertexShaderTypesEnum {
		// vertex shader 
		__begin_vs_type = 0,

		VST_PU4NT0_LD,
		VST_PB1U4NT0_LD,
		VST_PB2U4NT0_LD,
		VST_PB3U4NT0_LD,

		VST_PNT0_LD,
		VST_PNT0_LD_TT0,
		VST_PNT0_TT0,

		VST_PNDT0,
		VST_PNDT0_TT0,
		VST_PNDT0_LD,
		VST_PNDT0_LD_TT0,

		VST_PB4U4NT0_LD_DX9,


		VST_USER_DEFINE = 0xff,

		VST_BUFFER_SIZE = 1024,

		VST_INVALID = LW_INVALID_INDEX,
	};

	enum lwVertexShaderLightTypeEnum {
		VSLT_POINT = 1,
		VSLT_SPOT = 2,
		VSLT_DIRECTIONAL = 3,
		VSLT_INVALID = LW_INVALID_INDEX,
	};

	enum lwVertexShaderAnimTypeEnum {
		VSAT_NULL = 0x0000,

		VSAT_VERTEXMATRIX = VSAT_NULL, // donot need vs support

		VSAT_VERTEXBLEND0 = 0x0001,
		VSAT_VERTEXBLEND1 = 0x0002,
		VSAT_VERTEXBLEND2 = 0x0003,
		VSAT_VERTEXBLEND3 = 0x0004,

		VSAT_TEXTURETRANSFORM0 = 0x0010,
		VSAT_TEXTURETRANSFORM1 = 0x0020,
		VSAT_TEXTURETRANSFORM2 = 0x0030,
		VSAT_TEXTURETRANSFORM3 = 0x0040,

		VSAT_INVALID = LW_INVALID_INDEX
	};

	struct lwShaderDeclQueryInfo {
		DWORD decl_type;
		DWORD light_type;
		DWORD anim_type;
	};

	struct lwShaderDeclCreateInfo {
		DWORD shader_id;
		DWORD decl_type;
		DWORD light_type;
		DWORD anim_type;
		char file[LW_MAX_NAME];
	};

	inline lwShaderDeclCreateInfo SDCI_VALUE(DWORD shader, DWORD decl, DWORD light, DWORD anim, std::string_view file) {
		lwShaderDeclCreateInfo i;
		i.shader_id = shader;
		i.decl_type = decl;
		i.light_type = light;
		i.anim_type = anim;
		const std::size_t n = std::min<std::size_t>(file.size(), sizeof(i.file) - 1);
		std::memcpy(i.file, file.data(), n);
		i.file[n] = '\0';

		return i;
	}

LW_END
