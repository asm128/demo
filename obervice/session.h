#include "gpk_array.h"

#ifndef SESSION_H_029387928374
#define SESSION_H_029387928374

namespace obe
{
	struct SSessions		{ };
	struct SUsers			{ };
	struct SIPs				{ };
	struct SHit {
		uint64_t															Session		;
		uint64_t															IP			;
		uint64_t															TimeStamp	;
	};

	struct SHits			{
		::gpk::array_obj<::gpk::array_pod<SHit>>							Phones;
		::gpk::array_obj<::gpk::array_pod<SHit>>							Addresses;
		::gpk::array_obj<::gpk::array_pod<SHit>>							Images;
		::gpk::array_obj<::gpk::array_pod<SHit>>							Websites;
	};
} // namespace

#endif // SESSION_H_029387928374
