//

#include "stdafx.h"


#include "lwHeap.h"

LW_BEGIN
	// lwHeap
	LW_STD_IMPLEMENTATION(lwHeap);

	LW_RESULT lwHeap::Clone(lwIHeap** out_heap) {
		lwHeap* o = LW_NEW(lwHeap);

		o->_heap.Copy(&_heap);

		*out_heap = o;

		return LW_RET_OK;
	}

LW_END
