//
#include "lwHeader.h"
#include "lwPrimitive.h"
#include "lwClassDecl.h"
#include "lwITypes.h"
#include "lwInterface.h"


LW_BEGIN
LW_RESULT lwLoadPrimitiveLineList(lwINodePrimitive* obj, const char* name, DWORD vert_num, const lwVector3* vert_buf, const DWORD* color_buf, const lwSubsetInfo* subset_seq, DWORD subset_num);
LW_RESULT lwLoadPrimitiveLineList( lwIPrimitive* obj, const char* name, DWORD vert_num, const lwVector3* vert_buf, const DWORD* color_buf );
LW_RESULT lwLoadPrimitiveLineList( lwIPrimitive* obj, const char* name, DWORD vert_num, const lwVector3* vert_buf, const DWORD* color_buf, const lwSubsetInfo* subset_seq, DWORD subset_num );
LW_RESULT lwLoadPrimitiveGrid( lwIPrimitive* obj, const char* name, float width, float height, int row, int col, DWORD color );
LW_RESULT lwLoadPrimitiveAxis( lwIPrimitive* obj, const char* name, float length );
LW_RESULT lwLoadPrimitiveLineSphere( lwIPrimitive* obj, const char* name, DWORD color, float radius, int long_seg, int lat_seg );
LW_RESULT lwLoadPrimitiveLineCube(lwIPrimitive* obj, const char* name, DWORD color, const lwVector3* size);
LW_RESULT lwLoadPrimitivePlane(lwIPrimitive* obj, const char* name, DWORD color, float width, float height, int seg_width, int seg_height, BOOL two_side, BOOL wire_frame);
LW_END 
