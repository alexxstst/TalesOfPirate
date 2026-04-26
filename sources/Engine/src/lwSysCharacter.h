//
#include "lwHeader.h"
#include "lwSlotMap.h"

LW_BEGIN
	typedef lwSlotMapVoidPtr1024 lwObjectPoolSkeleton;
	typedef lwSlotMapVoidPtr1024 lwObjectPoolSkin;


	class lwSysCharacter {
	private:
		lwObjectPoolSkeleton* _pool_skeleton;
		lwObjectPoolSkin* _pool_skinmesh;

	public:
		lwSysCharacter();
		~lwSysCharacter();

		LW_RESULT QuerySkeleton(DWORD* ret_id, std::string_view file);
	};

LW_END
