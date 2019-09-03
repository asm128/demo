#include "gpk_typeint.h"

#ifndef OBE_CORE_H_2037849283742
#define OBE_CORE_H_2037849283742

namespace obe
{
#pragma pack(push, 1)
	struct SReferral {
		uint64_t						Type	: 8;
		uint64_t						Index	: 56;
	};
#pragma pack(pop)
} // namespace

#endif // OBE_CORE_H_2037849283742
