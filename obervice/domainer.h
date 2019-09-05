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

	struct SURLMapBlock			{
				::gpk::CViewManager<char_t, 1023U*64U>						Allocator					;

				::gpk::array_pod<::obe::URL_SCHEME>							Scheme						;
				::gpk::array_pod<int32_t>									Authority					;
				::gpk::array_pod<int32_t>									Path						;
				::gpk::array_pod<int32_t>									Query						;
				::gpk::array_pod<int32_t>									Fragment					;

				::gpk::error_t												AddURLMap					(const ::gpk::view_const_char & textToAdd);

				inline	::gpk::error_t										GetAuthority				(int32_t index, ::gpk::view_const_char & output)	{ const int32_t viewIndex = Authority	[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
				inline	::gpk::error_t										GetPath						(int32_t index, ::gpk::view_const_char & output)	{ const int32_t viewIndex = Path		[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
				inline	::gpk::error_t										GetQuery					(int32_t index, ::gpk::view_const_char & output)	{ const int32_t viewIndex = Query		[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
				inline	::gpk::error_t										GetFragment					(int32_t index, ::gpk::view_const_char & output)	{ const int32_t viewIndex = Fragment	[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
				::gpk::error_t												GetURL						(int32_t index, ::gpk::array_pod<char_t> & url)		{
			const ::obe::URL_SCHEME													scheme						= Scheme	[index];
			const int32_t															indexViewAuthority			= Authority	[index];
			if(::obe::URL_SCHEME_default != scheme) {
				url.append(::gpk::get_value_label(scheme));
				url.push_back(':');
				if(-1 != indexViewAuthority) {
					url.push_back('/');
					url.push_back('/');
				}
			}
			if(-1 != indexViewAuthority)
				gpk_necall(url.append(Allocator.Views[indexViewAuthority], Allocator.Counts[indexViewAuthority]), "%s", "Out of memory?");
			const int32_t															indexViewPath				= Path		[index];
			if(-1 != indexViewPath) {
				gpk_necall(url.push_back('/'), "%s", "Out of memory?");
				gpk_necall(url.append(Allocator.Views[indexViewPath], Allocator.Counts[indexViewPath]), "%s", "Out of memory?");
			}
			const int32_t															indexViewQuery				= Query		[index];
			if(-1 != indexViewQuery) {
				gpk_necall(url.push_back('?'), "%s", "Out of memory?");
				gpk_necall(url.append(Allocator.Views[indexViewQuery], Allocator.Counts[indexViewQuery]), "%s", "Out of memory?");
			}
			const int32_t															indexViewFragment			= Fragment	[index];
			if(-1 != indexViewFragment) {
				gpk_necall(url.push_back('#'), "%s", "Out of memory?");
				gpk_necall(url.append(Allocator.Views[indexViewFragment], Allocator.Counts[indexViewFragment]), "%s", "Out of memory?");
			}
			return 0;
		}
	};

	struct SSMTPMapBlock			{
				::gpk::CViewManager<char_t, 1024*64>						Allocator					;

				::gpk::array_pod<int32_t>									Domain						;
				::gpk::array_pod<int32_t>									Username					;

				::gpk::error_t												Save						(::gpk::array_pod<byte_t> & output)	{
			::gpk::viewWrite(::gpk::view_const_uint16{Allocator.Counts.begin(), Allocator.Counts.size()}, output);
			for(uint32_t iArray = 0; iArray < Allocator.Counts.size(); ++iArray)
				output.append(Allocator.Views[iArray], Allocator.Counts[iArray]);

			::gpk::viewWrite(::gpk::view_const_int32{Domain.begin(), Domain.size()}, output);
			output.append(::gpk::view_const_byte{(const byte_t*)Username.begin(), Domain.size() * sizeof(int32_t)});
		}

				::gpk::error_t												Load						(const ::gpk::view_const_byte & input)	{
			if(0 == input.size())
				return 0;

			const uint32_t															countArrays					= *(const uint32_t*)input.begin();
			uint32_t																offsetArraySize				= sizeof(uint32_t);
			typedef																	uint16_t					_tViewLen;
			uint32_t																offsetArrayData				= offsetArraySize + sizeof(_tViewLen) * countArrays;
			for(uint32_t iArray = 0; iArray < countArrays; ++iArray) {
				const _tViewLen															currentArraySize			= *(_tViewLen*)&input[offsetArraySize];
				Allocator.View(&input[offsetArrayData], currentArraySize);
				offsetArrayData														+= currentArraySize;
				offsetArraySize														+= sizeof(_tViewLen);
			}
			const uint32_t															countMaps					= *(const uint32_t*)&input[offsetArrayData];
			uint32_t																offsetDataDomain			= offsetArrayData	+ sizeof(uint32_t);
			uint32_t																offsetDataUsername			= offsetDataDomain	+ sizeof(int32_t) * countMaps;
			gpk_necall(Domain	.resize(countMaps), "%s", "Out of memory?");
			gpk_necall(Username	.resize(countMaps), "%s", "Out of memory?");
			for(uint32_t iMap = 0; iMap < countArrays; ++iMap) {
				Domain	[iMap]														= *(int32_t*)&input[offsetDataDomain	];
				Username[iMap]														= *(int32_t*)&input[offsetDataUsername	];
				offsetArrayData														+= sizeof(int32_t);
				offsetArraySize														+= sizeof(int32_t);
			}
			return 0;
		}

				::gpk::error_t												AddSMTPMap					(const ::gpk::view_const_char & textToAdd);

		inline	::gpk::error_t												GetDomain					(int32_t index, ::gpk::view_const_char & output)			{ const int32_t viewIndex = Domain	[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
		inline	::gpk::error_t												GetUsername					(int32_t index, ::gpk::view_const_char & output)			{ const int32_t viewIndex = Username[index]; output = {Allocator.Views[viewIndex], Allocator.Counts[viewIndex]}; return 0; }
				::gpk::error_t												GetEMail					(int32_t index, ::gpk::array_pod<char_t> & email)			{
			const int32_t															indexViewDomain				= Domain	[index];
			const int32_t															indexViewUsername			= Username	[index];
			gpk_necall(email.append(Allocator.Views[indexViewDomain], Allocator.Counts[indexViewDomain]), "%s", "Out of memory?");
			gpk_necall(email.push_back('@'), "%s", "Out of memory?");
			gpk_necall(email.append(Allocator.Views[indexViewUsername], Allocator.Counts[indexViewUsername]), "%s", "Out of memory?");
			return 0;
		}
	};
} // namespace

#endif // DOMAINER_H_2983749823748293
