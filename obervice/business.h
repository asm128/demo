#include "gpk_array.h"
#include "gpk_coord.h"

#ifndef BUSINESS_H_0239784982374
#define BUSINESS_H_0239784982374

namespace obe
{
	struct SPhones			{
		::gpk::array_obj<::gpk::view_const_string>							Number;
		::gpk::array_pod<uint32_t>											Property;
	};

#pragma pack(push, 1)
	struct SImageProperty	{
		uint8_t																Remote			: 1;
	};
#pragma pack(pop)

	struct SImages			{
		::gpk::array_obj<::gpk::view_const_string>							Name;
		::gpk::array_obj<::gpk::view_const_string>							FileName;
		::gpk::array_obj<::gpk::view_const_string>							URL;
		::gpk::array_pod<SImageProperty>									Property;
	};

	struct SAddressMap		{
		::gpk::array_pod<int32_t>											AddressLine2;
		::gpk::array_pod<int32_t>											AddressLine1;
		::gpk::array_pod<int32_t>											Neighbour;
		::gpk::array_pod<int32_t>											City;
	};

	struct SHours			{
		::gpk::array_obj<::gpk::view_array<::gpk::SRange<uint16_t>>>		Ranges;
		::gpk::array_obj<::gpk::view_array<int8_t>>							WeekDays;
		::gpk::array_pod<uint32_t>											Property;
	};

	struct SAddresses		{
		::gpk::array_obj<::gpk::view_const_string>							AddressLine2;		// Unbound
		::gpk::array_obj<::gpk::view_const_string>							AddressLine1;
		::gpk::array_obj<::gpk::view_const_string>							ZIP;
		::gpk::array_obj<::gpk::view_const_string>							MapURL;
		::gpk::array_pod<::gpk::SCoord2<float>>								MapCoords;
	};

	struct SAddressBook		{
		::gpk::array_pod<int64_t>											Images;
		::gpk::array_pod<int64_t>											Websites;
		::gpk::array_pod<int64_t>											Mails;
		::gpk::array_pod<int64_t>											Phones;
		::gpk::array_pod<int64_t>											Hours;
		::gpk::array_pod<int64_t>											Addresses;
	};

	//////////////////////----------------------------------------------------------------------
	struct SShops			{

	};

	struct SCategories		{
		::gpk::array_obj<::gpk::view_const_string>							Category;
		::gpk::array_obj<::gpk::array_obj<::gpk::view_const_string>>		Subcategories;
		::gpk::array_pod<uint32_t>											Property;
	};

	//////////////////////----------------------------------------------------------------------
	struct SNames			{
		::gpk::array_obj<::gpk::view_const_string>							First;	// Unbound
		::gpk::array_obj<::gpk::view_const_string>							Last;
	};

	struct SNameMap			{
		::gpk::array_obj<::gpk::view_array<uint32_t>>						First;
		::gpk::array_obj<::gpk::view_array<uint32_t>>						Last;
	};

	struct SPeople			{

	};
} // namespace

#endif // BUSINESS_H_0239784982374
