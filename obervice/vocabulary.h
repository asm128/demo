#include "gpk_array.h"

#include "obe_core.h"

#ifndef VOCABULARY_H_89023749283742
#define VOCABULARY_H_89023749283742

namespace obe
{
	//-------------------------
	struct SParagraph		{
		::gpk::array_pod<uint16_t>											Language;
		::gpk::array_obj<::gpk::view_const_string>							B64Text;
		::gpk::array_obj<::gpk::array_pod<uint32_t>>						Words;
	};

	struct STags			{
		::gpk::array_obj<::gpk::view_const_string>							Word;
		::gpk::array_obj<::gpk::array_pod<::obe::SReferral>>				Referrals;
	};

	struct SDictionary		{
		::gpk::array_obj<::gpk::view_const_string>							Word;
		::gpk::array_obj<::gpk::array_pod<::obe::SReferral>>				Referrals;
	};

	struct SLanguages		{
		::gpk::array_obj<::gpk::view_const_string>							Name;
		::gpk::array_obj<::gpk::view_const_string>							CodeISO;
		::gpk::array_obj<::gpk::view_const_string>							Code1;
		::gpk::array_pod<uint16_t>											Dictionary;
	};
} // namespace

#endif // VOCABULARY_H_89023749283742
