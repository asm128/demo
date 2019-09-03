#include "gpk_array.h"

#ifndef DEMOGRAPHICS_H_928374982374
#define DEMOGRAPHICS_H_928374982374

namespace obe
{
	struct SCountries			{
		::gpk::array_obj<::gpk::view_const_string>		Name;
		::gpk::array_pod<uint32_t>						City;
		::gpk::array_obj<::gpk::view_array<uint8_t>>	Neighborhoods;
	};

	struct SState				{
		::gpk::array_obj<::gpk::view_const_string>		Name;
		::gpk::array_obj<::gpk::view_array<uint32_t>>	Provinces;
		::gpk::array_pod<uint32_t>						Country;
	};

	struct SProvince			{
		::gpk::array_obj<::gpk::view_const_string>		Name;
		::gpk::array_obj<::gpk::view_array<uint32_t>>	Cities;
		::gpk::array_pod<uint32_t>						Country;
	};

	struct SCities				{
		::gpk::array_obj<::gpk::view_const_string>		Name;
		::gpk::array_pod<uint32_t>						Province;
		::gpk::array_obj<::gpk::view_array<uint32_t>>	Neighborhoods;
		::gpk::array_pod<uint8_t>						MapToState;
	};

	struct SNeighborhoods		{
		::gpk::array_obj<::gpk::view_const_string>		Name;
		::gpk::array_pod<uint32_t>						City;
	};
} // namespace

#endif
