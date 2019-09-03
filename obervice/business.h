#include "gpk_array.h"
#include "gpk_coord.h"

#ifndef BUSINESS_H_0239784982374
#define BUSINESS_H_0239784982374

namespace obe
{

	//////////////////////----------------------------------------------------------------------
	struct SName {
		uint32_t										First		;
		uint32_t										Last		;
	};


	//////////////////////----------------------------------------------------------------------
	struct SPeople			{ };
	struct SIPs				{ };
	struct SArticles		{ };
	struct SArticleText		{ };
	struct SDictionary		{
		::gpk::array_obj<::gpk::view_const_string>							Word;
		::gpk::array_pod<uint32_t>											Property;
	};

	struct STags			{
		::gpk::array_obj<::gpk::view_const_string>							Word;
		::gpk::array_pod<uint32_t>											Property;
	};

	struct SPhones			{ };
	struct SWebsiteMap		{ };
	struct SMailMap			{ };
	struct SAddressMap		{ };
	struct SHours			{
		::gpk::array_obj<::gpk::array_pod<::gpk::SRange<uint16_t>>>			Ranges;
		::gpk::array_obj<::gpk::array_pod<int8_t>>							WeekDays;
		::gpk::array_obj<uint32_t>											Property;
	};

	struct SNameMap			{ };
	struct SCategories		{
		::gpk::array_obj<::gpk::view_const_string>							Category;
		::gpk::array_obj<::gpk::array_obj<::gpk::view_const_string>>		Subcategories;
		::gpk::array_pod<uint32_t>											Property;
	};

	struct SLanguages		{
		::gpk::array_obj<::gpk::view_const_string>							Name;
		::gpk::array_obj<::gpk::view_const_string>							CodeISO;
		::gpk::array_obj<::gpk::view_const_string>							Code1;
		::gpk::array_pod<uint32_t>											Property;
		::gpk::array_pod<uint16_t>											Dictionary;
	};

	struct SPaths			{
		::gpk::array_obj<::gpk::view_const_string>							Text;
		::gpk::array_pod<uint32_t>											Property;
	};

	struct SQueryStrings	{
		::gpk::array_obj<::gpk::view_const_string>							Text;
		::gpk::array_pod<uint32_t>											Property;
	};

	struct SNames			{
		::gpk::array_obj<::gpk::view_const_string>							First;	// Unbound
		::gpk::array_obj<::gpk::view_const_string>							Last;
	};

	struct SDomains			{
		::gpk::array_obj<::gpk::view_const_string>							Name;
		::gpk::array_pod<uint32_t>											Property;
	};

	struct SAddresses		{
		::gpk::array_obj<::gpk::view_const_string>							AddressLine2;		// Unbound
		::gpk::array_obj<::gpk::view_const_string>							AddressLine1;
		::gpk::array_obj<::gpk::view_const_string>							ZIP;
		::gpk::array_obj<::gpk::view_const_string>							MapURL;
		::gpk::array_pod<::gpk::SCoord2<float>>								MapCoords;
	};
} // namespace

#endif // BUSINESS_H_0239784982374
