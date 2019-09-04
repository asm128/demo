#include "gpk_array.h"
#include "gpk_enum.h"
#include "obe_core.h"
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
				::gpk::CViewManager<char_t, 1024*64>						Allocator					;

				::gpk::array_pod<::obe::URL_SCHEME>							Scheme						;
				::gpk::array_pod<int64_t>									Authority					;
				::gpk::array_pod<int64_t>									Path						;
				::gpk::array_pod<int64_t>									Query						;
				::gpk::array_pod<int64_t>									Fragment					;

				::gpk::error_t												AddURLMap	(const ::gpk::view_const_char & textToAdd);
	};

	struct SSMTPMap			{
				::gpk::CViewManager<char_t, 1024*64>						Allocator					;

				::gpk::array_pod<int32_t>									Domain						;
				::gpk::array_obj<int32_t>									Username					;

				::gpk::error_t												AddSMTPMap					(const ::gpk::view_const_char & textToAdd);

		inline	::gpk::error_t												GetDomain					(int32_t index, ::gpk::view_const_char & output)			{ const int32_t viewIndex = Domain	[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex], }; return 0; }
		inline	::gpk::error_t												GetUsername					(int32_t index, ::gpk::view_const_char & output)			{ const int32_t viewIndex = Username[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex], }; return 0; }
				::gpk::error_t												GetEMail					(int32_t index, ::gpk::array_pod<char_t> & email)			{
			const int32_t															indexViewDomain				= Domain	[index];
			const int32_t															indexViewUsername			= Username	[index];
			email																= ::gpk::view_const_char{Allocator.Views[indexViewDomain], Allocator.Counts[indexViewDomain]};
			gpk_necall(email.push_back('@'), "%s", "Out of memory?");
			gpk_necall(email.append(Allocator.Views[indexViewUsername], Allocator.Counts[indexViewUsername]), "%s", "Out of memory?");
			return 0;
		}
	};
} // namespace

#endif // DOMAINER_H_2983749823748293
