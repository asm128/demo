#include "gpk_array.h"
#include "gpk_enum.h"
#include "gpk_view_manager.h"

#ifndef DOMAINER_H_2983749823748293
#define DOMAINER_H_2983749823748293

namespace obe
{
	GDEFINE_ENUM_TYPE(URL_SCHEME, uint8_t)
	GDEFINE_ENUM_VALUE(URL_SCHEME, default	, 0);
	GDEFINE_ENUM_VALUE(URL_SCHEME, http		, 1);
	GDEFINE_ENUM_VALUE(URL_SCHEME, https	, 2);
	GDEFINE_ENUM_VALUE(URL_SCHEME, ftp		, 3);
	GDEFINE_ENUM_VALUE(URL_SCHEME, mailto	, 4);
	GDEFINE_ENUM_VALUE(URL_SCHEME, file		, 5);
	GDEFINE_ENUM_VALUE(URL_SCHEME, data		, 6);
	GDEFINE_ENUM_VALUE(URL_SCHEME, irc		, 7);

	struct SURLMap			{
		::gpk::array_pod<::obe::URL_SCHEME>									Scheme;
		::gpk::array_pod<int64_t>											Authority;
		::gpk::array_pod<int64_t>											Path;
		::gpk::array_pod<int64_t>											Query;
		::gpk::array_pod<int64_t>											Fragment;
	};

	struct SSMTPMap			{
		::gpk::array_pod<int64_t>											Domain;
		::gpk::array_obj<::gpk::view_const_char>							Username;
	};

	struct STextContainer {
		::gpk::array_obj<::gpk::view_const_char>							Text;
		::gpk::array_pod<uint32_t>											Property;

		::gpk::error_t														AddText		(const ::gpk::view_const_char & textToAdd, uint32_t textProperty)	{ const ::gpk::error_t newIndex = Text.push_back(textToAdd); gpk_necall(newIndex, "%s", "Out of memory?"); gpk_necall(Property.push_back(textProperty)	, "%s", "Out of memory?"); return newIndex; }
		::gpk::error_t														AddText		(const ::gpk::view_const_char & textToAdd)							{ const ::gpk::error_t newIndex = Text.push_back(textToAdd); gpk_necall(newIndex, "%s", "Out of memory?"); gpk_necall(Property.push_back(0)				, "%s", "Out of memory?"); return newIndex; }
	};

	struct SDomainer {
		::obe::STextContainer												QueryStrings;
		::obe::STextContainer												Paths		;
		::obe::STextContainer												Domains		;
		//---------------------
		::obe::SSMTPMap														SMTPMap		;
		::obe::SURLMap														URLMap		;

		::gpk::CViewManager<char_t, 1024*64>								Allocator	;

		::gpk::error_t														AddSMTPMap	(const ::gpk::view_const_char & textToAdd);
		::gpk::error_t														AddURLMap	(const ::gpk::view_const_char & textToAdd);
	};

} // namespace

#endif // DOMAINER_H_2983749823748293
