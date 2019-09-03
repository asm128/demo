#include "gpk_array.h"
#include "gpk_enum.h"

#ifndef DOMAINER_H_2983749823748293
#define DOMAINER_H_2983749823748293

namespace obe
{
	GDEFINE_ENUM_TYPE(BROWSER_PROTOCOL, uint8_t)
	GDEFINE_ENUM_VALUE(BROWSER_PROTOCOL, none		, 0);
	GDEFINE_ENUM_VALUE(BROWSER_PROTOCOL, http		, 1);
	GDEFINE_ENUM_VALUE(BROWSER_PROTOCOL, https		, 2);
	GDEFINE_ENUM_VALUE(BROWSER_PROTOCOL, ftp		, 3);

	struct SHTTPMap			{
		::gpk::array_pod<int64_t>											Path;
		::gpk::array_pod<int64_t>											QueryString;
		::gpk::array_pod<int64_t>											Domain;
		::gpk::array_pod<BROWSER_PROTOCOL>									Protocol;
	};

	struct SSMTPMap			{
		::gpk::array_pod<int64_t>											Domain;
		::gpk::array_obj<::gpk::view_const_string>							Username;
	};

	struct SDomains			{
		::gpk::array_obj<::gpk::view_const_string>							Name;
		::gpk::array_pod<uint32_t>											Property;
	};

	struct SPaths			{
		::gpk::array_obj<::gpk::view_const_string>							Text;
		::gpk::array_pod<uint32_t>											Property;
	};

	struct SQueryStrings	{
		::gpk::array_obj<::gpk::view_const_string>							Text;
		::gpk::array_pod<uint32_t>											Property;
	};
} // namespace

#endif // DOMAINER_H_2983749823748293
