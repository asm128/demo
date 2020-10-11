#include "ntl_front.h"
#include "ntl_session.h"

#include "gpk_cgi_app_impl_v2.h"

#include "gpk_json_expression.h"
#include "gpk_parse.h"
#include "gpk_stdstring.h"
#include "gpk_find.h"

GPK_CGI_JSON_APP_IMPL();

GDEFINE_ENUM_TYPE(AD_SHOP_CATEGORY, uint8_t);
GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, tours, 0);
GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, meals, 1);
GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, shops, 2);
GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, shows, 3);

static	::gpk::error_t								pageCatalog						(const ::gpk::view_const_char & contentFileName, const ::gpk::view_const_char & pathScript, const ::gpk::SCoord2<uint32_t> sizeScreen, const ::gpk::view_const_char & pathStyles, const AD_SHOP_CATEGORY category, const ::gpk::view_const_char & title, const ::gpk::view_const_char & lang, ::gpk::array_pod<char_t> & output);

::gpk::error_t										gpk_cgi_generate_output			(::gpk::SCGIRuntimeValues & runtimeValues, ::gpk::array_pod<char_t> & output)	{
	::gpk::SHTTPAPIRequest									requestReceived					= {};
	bool													isCGIEnviron					= ::gpk::httpRequestInit(requestReceived, runtimeValues, true);
	::gpk::view_const_string								cookie;
	::gpk::view_const_string								sessionFileName;					;
	::gpk::array_pod<char_t>								sessionFileContents					= {};
	::gpk::array_obj<::gpk::TKeyValConstString>				cookieValues;
	if (isCGIEnviron) {
		gpk_necall(output.append_string("Content-type: text/html\r\nCache-control: no-cache\r\n"), "%s", "Out of memory?");
		gpk_necall(output.append_string("\r\n"), "%s", "Out of memory?");
		::gpk::find(::gpk::vcs{"HTTP_COOKIE"}, runtimeValues.EnvironViews, cookie);
		if(0 == cookie.size()) {
			output.append_string("{ \"status\" : 403, \"description\" :\"forbidden - The server currently doesn't accept requests to this frame without a cookie set.\" }\r\n");
			return 1;
			//::sessionInitialize(requestReceived, cookie, sessionFileName, digested);
			//gpk_necall(output.append_string("\r\nSet-Cookie: "), "%s", "Out of memory?");
			//gpk_necall(output.append(cookie), "%s", "Out of memory?");
		}
		else {
			gpk_necall(output.append_string("\r\n\r\n"), "%s", "Out of memory?");
			gpk_necall(::ntl::sessionFileLoad(cookie, sessionFileContents, cookieValues), "%s", "Failed to load session!");
		}
		//gpk_necall(output.append_string("\r\nSet-Cookie: "), "%s", "Out of memory?");
		//gpk_necall(output.append(cookie), "%s", "Out of memory?");

		::gpk::view_const_char							methodsValid	[]				=
			{ ::gpk::vcs{"GET" }
			, ::gpk::vcs{"POST"}
			};
		if(-1 == ::gpk::keyValVerify(runtimeValues.EnvironViews, "REQUEST_METHOD", methodsValid)) {
			output.append_string("{ \"status\" : 403, \"description\" :\"forbidden\" }\r\n");
			return 1;
		}
	}
	::ntl::SNTLArgs											qsArgs;
	::ntl::loadNTLArgs(qsArgs, requestReceived.QueryStringKeyVals);

	::ntl::SHTMLEndpoint									programState;
	::gpk::view_const_string								configFileName					= "./neutralizer.json";
	gpk_necall(::gpk::jsonFileRead(programState.Config, configFileName), "Failed to load configuration file: %s.", configFileName);
	{
		::gpk::view_const_string								rootNode;
		const ::gpk::error_t									indexRoot						= ::gpk::jsonExpressionResolve(::gpk::vcs{"tuobelisco"}, programState.Config.Reader, 0, rootNode);
		::ntl::frontConfigLoad(programState, indexRoot);
	}
	::gpk::view_const_string								title;

	::AD_SHOP_CATEGORY								category						= ::gpk::get_value<::AD_SHOP_CATEGORY>(qsArgs.Module);
	if(::AD_SHOP_CATEGORY_INVALID == category)
		category = ::AD_SHOP_CATEGORY_tours;
	switch(category) {
	case ::AD_SHOP_CATEGORY_tours: title = (qsArgs.Language == ::gpk::vcs{"es"}) ? (::gpk::vcs{ "Turismo"		}) : (::gpk::vcs{"Tourism"	}); break;
	case ::AD_SHOP_CATEGORY_shops: title = (qsArgs.Language == ::gpk::vcs{"es"}) ? (::gpk::vcs{ "Comercios"		}) : (::gpk::vcs{"Shopping"	}); break;
	case ::AD_SHOP_CATEGORY_meals: title = (qsArgs.Language == ::gpk::vcs{"es"}) ? (::gpk::vcs{ "Comidas"		}) : (::gpk::vcs{"Meals"	}); break;
	case ::AD_SHOP_CATEGORY_shows: title = (qsArgs.Language == ::gpk::vcs{"es"}) ? (::gpk::vcs{ "Espectáculos"	}) : (::gpk::vcs{"Shows"	}); break;
	default: break;
	}

	::gpk::SCoord2<uint32_t>								sizeScreen						= {};
	::gpk::parseIntegerDecimal(qsArgs.Width	, &sizeScreen.x);
	::gpk::parseIntegerDecimal(qsArgs.Height, &sizeScreen.y);

	::pageCatalog(::gpk::vcs{"ads.json"}, programState.Path.Script, sizeScreen, programState.Path.Style, category, title, qsArgs.Language.size() ? qsArgs.Language : ::gpk::view_const_string{"es"}, output);

	if(output.size()) {
		OutputDebugStringA(output.begin());
		OutputDebugStringA("\n");
	}
	return 0;
}


struct SItemViews {
	uint32_t											ItemIndex					= 0;
	::gpk::view_const_char								URL							= {};
	::gpk::view_const_char								Name						= {};
	::gpk::view_const_char								Title						= {};
	::gpk::view_const_char								Text						= {};
	::gpk::view_const_char								ImageHRef					= {};
	::gpk::view_const_char								ImageSrc					= {};
	::gpk::view_const_char								ImageTitle					= {};
	::gpk::view_const_char								ImageAlt					= {};
	::gpk::view_const_char								MapURL						= {};
	::gpk::array_pod<uint32_t>							Barrio						= {};
	::gpk::array_obj<::gpk::view_const_char>			Phones						= {};
	::gpk::array_obj<::gpk::view_const_char>			Addresses					= {};
	::gpk::array_obj<::gpk::view_const_char>			WPs							= {};
};

static	::gpk::error_t								getItemViews						(::gpk::SJSONReader & content, const ::AD_SHOP_CATEGORY category, const ::gpk::view_const_char & lang, ::gpk::array_obj<::SItemViews> & indicesToDisplay)	{
	for(int32_t iItem = 0, countItems = ::gpk::jsonArraySize(*content[0]); iItem < countItems; ++iItem) {
		SItemViews												views								= {(uint32_t)iItem};
		const ::gpk::error_t									jsonIndexCurrentItem				= ::gpk::jsonArrayValueGet(*content[0], iItem);
		::gpk::view_const_string								viewWikiCategory					= {};
		const ::gpk::error_t									jsonIndexArrayCategory				= ::gpk::jsonExpressionResolve(::gpk::vcs{"category"}, content, jsonIndexCurrentItem, viewWikiCategory);
		bool													isCategory							= false;
		for(uint32_t iCat = 0, countCat = (uint32_t)::gpk::jsonArraySize(*content[jsonIndexArrayCategory]); iCat < countCat; ++iCat) {
			const ::gpk::error_t									jsonIndexNodeCategory				= ::gpk::jsonArrayValueGet(*content[jsonIndexArrayCategory], iCat);
			const ::gpk::SJSONToken									& jsonToken							= content.Token[jsonIndexNodeCategory];
			if(category == jsonToken.Value) {
				isCategory											= true;
				break;
			}
		}
		if(false == isCategory)
			continue;

		// ---- Root arrays.
		const ::gpk::error_t									jsonIndexArrayPhone					= ::gpk::jsonExpressionResolve(::gpk::vcs{"phone"		}, content, jsonIndexCurrentItem, views.MapURL	);
		const ::gpk::error_t									jsonIndexArrayWP					= ::gpk::jsonExpressionResolve(::gpk::vcs{"whatsapp"	}, content, jsonIndexCurrentItem, views.MapURL	);
		const ::gpk::error_t									jsonIndexArrayAddr					= ::gpk::jsonExpressionResolve(::gpk::vcs{"address"		}, content, jsonIndexCurrentItem, views.MapURL	);
		const ::gpk::error_t									jsonIndexArrayBarrios				= ::gpk::jsonExpressionResolve(::gpk::vcs{"barrio"		}, content, jsonIndexCurrentItem, views.MapURL	);
		// ---- Root properties.																								   ::gpk::vcs{
		const ::gpk::error_t									jsonIndexName						= ::gpk::jsonExpressionResolve(::gpk::vcs{"name"		}, content, jsonIndexCurrentItem, views.Name	);
		const ::gpk::error_t									jsonIndexMap						= ::gpk::jsonExpressionResolve(::gpk::vcs{"location"	}, content, jsonIndexCurrentItem, views.MapURL	);
		// ---- Language-based properties.																						   ::gpk::vcs{
		const ::gpk::error_t									jsonIndexLang						= ::gpk::jsonExpressionResolve(lang						 , content, jsonIndexCurrentItem, views.URL		);
		const ::gpk::error_t									jsonIndexWiki						= ::gpk::jsonExpressionResolve(::gpk::vcs{"wiki"		}, content, jsonIndexLang, views.URL			);
		const ::gpk::error_t									jsonIndexTitle						= ::gpk::jsonExpressionResolve(::gpk::vcs{"title"		}, content, jsonIndexLang, views.Title			);
		const ::gpk::error_t									jsonIndexText						= ::gpk::jsonExpressionResolve(::gpk::vcs{"text"		}, content, jsonIndexLang, views.Text			);
		const ::gpk::error_t									jsonIndexImageHRef					= ::gpk::jsonExpressionResolve(::gpk::vcs{"image.href"	}, content, jsonIndexLang, views.ImageHRef		);
		const ::gpk::error_t									jsonIndexImageSrc					= ::gpk::jsonExpressionResolve(::gpk::vcs{"image.src"	}, content, jsonIndexLang, views.ImageSrc		);
		const ::gpk::error_t									jsonIndexImageTitle					= ::gpk::jsonExpressionResolve(::gpk::vcs{"image.title" }, content, jsonIndexLang, views.ImageTitle		);
		const ::gpk::error_t									jsonIndexImageAlt					= ::gpk::jsonExpressionResolve(::gpk::vcs{"image.alt"	}, content, jsonIndexLang, views.ImageAlt		);

		for(int32_t iPhone	= 0, countAddr		= ::gpk::jsonArraySize(*content[jsonIndexArrayAddr		]); iPhone	< countAddr		; ++iPhone	)	views.Addresses	.push_back(content.View[::gpk::jsonArrayValueGet(*content[jsonIndexArrayAddr	], iPhone	)]);
		for(int32_t iPhone	= 0, countPhones	= ::gpk::jsonArraySize(*content[jsonIndexArrayPhone		]); iPhone	< countPhones	; ++iPhone	)	views.Phones	.push_back(content.View[::gpk::jsonArrayValueGet(*content[jsonIndexArrayPhone	], iPhone	)]);
		for(int32_t iPhone	= 0, countWPs		= ::gpk::jsonArraySize(*content[jsonIndexArrayWP		]); iPhone	< countWPs		; ++iPhone	)	views.WPs		.push_back(content.View[::gpk::jsonArrayValueGet(*content[jsonIndexArrayWP		], iPhone	)]);
		for(int32_t iBarrio	= 0, countBarrios	= ::gpk::jsonArraySize(*content[jsonIndexArrayBarrios	]); iBarrio	< countBarrios	; ++iBarrio	)	views.Barrio	.push_back((uint32_t)content.Token[::gpk::jsonArrayValueGet(*content[jsonIndexArrayBarrios], iBarrio	)].Value);

		gpk_necall(indicesToDisplay.push_back(views), "%s", "Out of memory?");
	}
	return 0;
}

static	::gpk::error_t								getWordList							(const ::SItemViews& item, ::gpk::array_obj<::gpk::array_pod<char_t>> & itemWords) {
	::gpk::array_obj<::gpk::view_const_char>				upper								= {};
	gpk_necall(::gpk::split(item.Text	, ' ', upper), "%s", "Out of memory?");
	gpk_necall(::gpk::split(item.Title	, ' ', upper), "%s", "Out of memory?");
	gpk_necall(::gpk::split(item.URL	, ' ', upper), "%s", "Out of memory?");
	gpk_necall(::gpk::split(item.Text	, ' ', upper), "%s", "Out of memory?");
	for(uint32_t iAddr = 0; iAddr < item.Addresses	.size(); ++iAddr) gpk_necall(::gpk::split(item.Addresses[iAddr], ' ', upper), "%s", "Out of memory?");
	for(uint32_t iAddr = 0; iAddr < item.Phones		.size(); ++iAddr) gpk_necall(::gpk::split(item.Phones	[iAddr], ' ', upper), "%s", "Out of memory?");
	for(uint32_t iAddr = 0; iAddr < item.WPs		.size(); ++iAddr) gpk_necall(::gpk::split(item.WPs		[iAddr], ' ', upper), "%s", "Out of memory?");

	gpk_necall(itemWords.resize(upper.size()), "%s", "Out of memory?");
	const ::gpk::TKeyValConstString							replaceDict	[]						=
	{ ::gpk::TKeyValConstString
		{ ::gpk::vcs{"áéíóúàèìòùäëïöüãõñçºª"}
		, ::gpk::vcs{"aeiouaeiouaeiouaoncoa"}}
	};
	::gpk::array_pod<char_t>								fixedWord;
	for(uint32_t iWord = 0, countWords = upper.size(); iWord < countWords; ++iWord) {
		::gpk::view_const_char									originalWord						= upper[iWord];
		::gpk::trim(originalWord, originalWord);
		::gpk::array_pod<char_t>								& finalWord							= itemWords[iWord];
		finalWord.resize(originalWord.size());
		memcpy(finalWord.begin(), originalWord.begin(), originalWord.size());
		::gpk::tolower(finalWord);
		if( finalWord == ::gpk::view_const_string{"the"		}
		 || finalWord == ::gpk::view_const_string{"and"		}
		 || finalWord == ::gpk::view_const_string{"it"		}
		 || finalWord == ::gpk::view_const_string{"to"		}
		 || finalWord == ::gpk::view_const_string{"an"		}
		 || finalWord == ::gpk::view_const_string{"by"		}
		 || finalWord == ::gpk::view_const_string{"was"		}
		 || finalWord == ::gpk::view_const_string{"is"		}
		 || finalWord == ::gpk::view_const_string{"in"		}
		 || finalWord == ::gpk::view_const_string{"as"		}
		 || finalWord == ::gpk::view_const_string{"for"		}
		 || finalWord == ::gpk::view_const_string{"of"		}
		 || finalWord == ::gpk::view_const_string{"up"		}

		 || finalWord == ::gpk::view_const_string{"a"		}
		 || finalWord == ::gpk::view_const_string{"un"		}
		 || finalWord == ::gpk::view_const_string{"con"		}
		 || finalWord == ::gpk::view_const_string{"del"		}
		 || finalWord == ::gpk::view_const_string{"por"		}
		 || finalWord == ::gpk::view_const_string{"que"		}
		 || finalWord == ::gpk::view_const_string{"es"		}
		 || finalWord == ::gpk::view_const_string{"en"		}
		 || finalWord == ::gpk::view_const_string{"de"		}
		 || finalWord == ::gpk::view_const_string{"la"		}
		 || finalWord == ::gpk::view_const_string{"el"		}
		 || finalWord == ::gpk::view_const_string{"para"	}
		 || finalWord == ::gpk::view_const_string{"los"		}
		 || finalWord == ::gpk::view_const_string{"las"		}
		 )	{
			finalWord.clear();
			continue;
		}
		info_printf("finalWord: %s.", ::gpk::toString(finalWord).begin());
		fixedWord											= finalWord;
		bool													replaced		= false;
		for(uint32_t iChar = 0; iChar < fixedWord.size(); ++iChar) {
			char													& wordChar		= fixedWord[iChar];
			for(uint32_t iTable = 0; iTable < ::gpk::size(replaceDict); ++iTable) {
				const ::gpk::TKeyValConstString &	table = replaceDict[iTable];
				for(uint32_t iItem = 0; iItem < table.Key.size(); ++iItem)
					if(wordChar == table.Key[iItem]) {
						wordChar											= table.Val[iItem];
						replaced											= true;
					}
			}
		}
		if(replaced) {
			itemWords.push_back(fixedWord);
			info_printf("fixedWord: %s.", ::gpk::toString(fixedWord).begin());
		}
	}
	return 0;
}

struct SWordIndices {
	::gpk::array_pod<char_t>							Text;
	::gpk::array_pod<uint32_t>							Indices;
};

static	::gpk::error_t								getWordDict							(const ::SItemViews& item, uint32_t iItem, ::gpk::array_obj<::SWordIndices> & wordSet, ::gpk::array_obj<::gpk::array_pod<char_t>> & cacheItemWords) {
	::getWordList(item, cacheItemWords);
	for(uint32_t iItemWord = 0; iItemWord < cacheItemWords.size(); ++iItemWord) {
		::gpk::view_const_string								itemWord						= {cacheItemWords[iItemWord].begin(), cacheItemWords[iItemWord].size()};
		info_printf("itemWord: %s.", ::gpk::toString(itemWord).begin());
		if(1 >= itemWord.size())
			continue;
		bool													bFound							= false;
		if(',' == itemWord[itemWord.size() - 1] || '.' == itemWord[itemWord.size() - 1]) {
			itemWord											= {itemWord.begin(), itemWord.size() - 1};
			if(0 >= itemWord.size())
				continue;
		}
		for(uint32_t iWordInSet = 0; iWordInSet < wordSet.size(); ++iWordInSet) {
			SWordIndices											& dictWord						= wordSet[iWordInSet];
			if(itemWord != dictWord.Text)
				continue;
			bFound												= true;
			if(0 > ::gpk::find(iItem, {dictWord.Indices.begin(), dictWord.Indices.size()}))
				dictWord.Indices.push_back(iItem);
		}
		if(false == bFound)
			wordSet[wordSet.push_back({itemWord})].Indices.push_back(iItem);
	}
	return 0;
}

struct SBarrio {
			::gpk::view_const_char						Name						= {};
			::gpk::view_const_char						Area						= {};
			::gpk::view_const_char						Population					= {};
			::gpk::view_const_char						Commune						= {};
};

static	::gpk::error_t								outputSearchScript					(const ::gpk::view_array<const ::SItemViews> & indicesToDisplay, ::gpk::array_pod<char_t> & output) {
	::gpk::array_obj<::SWordIndices>						wordSet;
	::gpk::array_obj<::gpk::array_pod<char_t>>				cacheItemWords;

	for(uint32_t iItem = 0, countItems = indicesToDisplay.size(); iItem < countItems; ++iItem)
		::getWordDict(indicesToDisplay[iItem], iItem, wordSet, cacheItemWords);

	output.append_string("\nvar names=[");
	for(uint32_t iWord = 0, countWords = wordSet.size(); iWord < countWords; ++iWord) {
		const SWordIndices										& element							= wordSet[iWord];
		info_printf("Word: %s.", ::gpk::toString(element.Text).begin());
		if(0 <= ::gpk::find('\'', ::gpk::view_array{element.Text.begin(), element.Text.size()})) {
			if(0 <= ::gpk::find('"', ::gpk::view_array{element.Text.begin(), element.Text.size()}))
				continue;
			output.push_back('"');
			output.append(element.Text);
			output.push_back('"');
		}
		else {
			output.push_back('\'');
			output.append(element.Text);
			output.push_back('\'');
		}
		if(iWord < (wordSet.size() - 1))
			output.push_back(',');
	}
	output.append_string("];");

	output.append_string("\nvar indices=[");
	char													tempIntStr[256]				= {};
	for(uint32_t iWord = 0; iWord < wordSet.size(); ++iWord) {
		const SWordIndices		& element = wordSet[iWord];
		output.push_back('[');
		for(uint32_t iArticle = 0; iArticle < element.Indices.size(); ++iArticle) {
			sprintf_s(tempIntStr, "%u", element.Indices[iArticle]);
			output.append_string(tempIntStr);
			if(iArticle < (element.Indices.size() - 1))
				output.push_back(',');
		}
		output.push_back(']');
		if(iWord < (wordSet.size() - 1))
			output.push_back(',');
	}
	output.append_string("];");

	output.append_string("\nvar idList=[");
	for(uint32_t iArticle = 0; iArticle < indicesToDisplay.size(); ++iArticle) {
		sprintf_s(tempIntStr, "%u", iArticle);
		output.append_string(tempIntStr);
		if(iArticle < (indicesToDisplay.size() - 1))
			output.push_back(',');
	}
	output.append_string("];");
	output.append_string("\nvar barrioIdMap=[");
	for(uint32_t iArticle = 0; iArticle < indicesToDisplay.size(); ++iArticle) {
		const ::SItemViews										& ad								= indicesToDisplay[iArticle];
		output.push_back('[');
		for(uint32_t iBarrio = 0; iBarrio < ad.Barrio.size(); ++iBarrio) {
			sprintf_s(tempIntStr, "%u", ad.Barrio[iBarrio]);
			output.append_string(tempIntStr);
			if(iBarrio < (ad.Barrio.size() - 1))
				output.push_back(',');
		}
		output.push_back(']');
		if(iArticle < (indicesToDisplay.size() - 1))
			output.push_back(',');
	}
	output.append_string("];");
	return 0;
}

static ::gpk::error_t								htmlBoardGenerate					(const ::gpk::view_array<const ::SItemViews> indicesToDisplay, const ::gpk::view_const_char & fontSize, const ::gpk::view_const_char & title, const ::gpk::view_const_char & lang, ::gpk::array_pod<char_t> & output)	{
	output.append_string("\n<table style=\"width:100%;height:100%;text-align:center;font-size:");
	output.append(fontSize);
	output.append_string("px;\" >");
	output.append_string("\n<tr style=\"\" >");
	output.append_string("\n<td style=\"width:100%;font-weight:bold; vertical-align:top;\">");
	::ntl::htmlTag("h2", title, {}, output);
	output.append_string("\n</td>");
	output.append_string("\n</tr>");
	output.append_string("\n<tr style=\"\" >");
	output.append_string("\n<td style=\"width:100%;font-weight:bold; vertical-align:top;\">");

		output.append_string("\n<table style=\"width:100%;text-align:center;border-style:solid;border-width:2px;font-size:");
		output.append(fontSize);
		output.append_string("px;\" >");
		::gpk::array_pod<char_t>								base64Id;
		char													tempIntStr[256]				= {};
		for(int32_t iItem = 0, countItems = indicesToDisplay.size(); iItem < countItems; ++iItem) {
			const SItemViews										& views						= indicesToDisplay[iItem];
			base64Id.clear();
			sprintf_s(tempIntStr, "%u", iItem);
			output.append_string("\n<tr id=\"");
			output.append_string(tempIntStr);
			output.append_string("\">");
			output.append_string("\n<td style=\"width:100%;text-align:left;vertical-align:top;\">");
			//
				output.append_string("\n<table style=\"background-color:white;width:100%;height:100%;min-height:100%;text-align:center;border-style:solid;border-width:2px;border-radius:8px;font-size:");
				output.append(fontSize);
				output.append_string("px;\" >");
				output.append_string("\n<tr>");
				output.append_string("\n<td style=\"background-color:lightgrey;border-radius:8px;text-align:left;vertical-align:top;\">");
				//
				::gpk::array_pod<char_t> pTitle;
				::ntl::htmlTag("h3", views.Title, ::gpk::view_const_string{"style=\"text-align:left;\""}, pTitle);
				::ntl::htmlHRefLink({pTitle.begin(), pTitle.size()}, views.URL, ::gpk::view_const_string{"style=\"text-decoration:none;font-weight:bold;\""}, output);
				output.append_string("\n</td>");
				output.append_string("\n</tr>");
				output.append_string("\n<tr>");
				output.append_string("\n<td style=\"vertical-align:top;height:100%;text-align:left;font-size:");
				output.append(fontSize);
				output.append_string("px;\">");
				::ntl::htmlTag("h3", views.Text, ::gpk::view_const_string{"style=\" font-weight:normal;text-align:left;\""}, output);
				//
				output.append_string("\n</td>");
				output.append_string("\n</tr>");
				output.append_string("\n</table>");
			//
			output.append_string("\n</td>");
			output.append_string("\n<td style=\"height:100%;text-align:left;vertical-align:top;\">");

				output.append_string("\n<table style=\"\" >");
				output.append_string("\n<tr>");
				output.append_string("\n<td style=\"text-align:left;vertical-align:top;width:100%;\">");
				output.append_string("<a target=\"blank\" title=\"");
				output.append(views.ImageTitle);
				output.append_string("\" href=\"");
				output.append(views.ImageHRef);
				output.append_string("\" class=\"image\" ><img style=\"width:320px;\" decoding=\"async\" elementtiming=\"thumbnail\" alt=\"");
				output.append(views.ImageAlt);
				output.append_string("\" src=\"");
				output.append(views.ImageSrc);
				output.append_string("\" ></a>");
				output.append_string("\n</td>");
				output.append_string("\n</tr><tr>");
				output.append_string("\n<td style=\"vertical-align:top;\">");

				// ----- Image info table
					output.append_string("\n<table style=\"background-color:white;width:100%;height:100%;text-align:center;border-style:solid;border-width:2px;border-radius:8px;\" font-size:");
					output.append(fontSize);
					output.append_string("px;\">");
					if(views.MapURL.size()) {
						output.append_string("\n<tr>");
						output.append_string("\n<td style=\"text-align:center;vertical-align:top;font-size:");
						output.append(fontSize);
						output.append_string("px;\">");
						output.append_string("\n<a target=\"blank\" style=\"margin:4px;\" href=\"");
						output.append(views.MapURL);
						output.append_string("\" >");
						if(lang == ::gpk::view_const_string{"es"})
							output.append_string("Localización");
						else
							output.append_string("Location");
						output.append_string("\n</a>");
						output.append_string("\n</td>");
						output.append_string("\n</tr>");
					}
					// ----- Addresses
					if(views.Addresses.size()) {
						output.append_string("\n<tr style=\"height:100%;\">");
						output.append_string("\n<td style=\"text-align:center;vertical-align:top;border-style:solid;border-top-width:1px;font-size:");
						output.append(fontSize);
						output.append_string("px;\" >");
						for(uint32_t iPhone = 0, countAddr = views.Addresses.size(); iPhone < countAddr; ++iPhone) {
							output.append(views.Addresses[iPhone]);
							output.append_string("\n<br />");
						}
						output.append_string("\n</td>");
						output.append_string("\n</tr>");
					}

					// ----- Phones
					if(views.Phones.size() || views.WPs.size()) {
						output.append_string("\n<tr style=\"height:100%;\">");
						output.append_string("\n<td style=\"text-align:center;vertical-align:top;border-style:solid;border-top-width:1px;font-size:");
						output.append(fontSize);
						output.append_string("px;\" >");
						for(uint32_t iPhone = 0, countPhones = views.Phones.size(); iPhone < countPhones; ++iPhone) {
							output.append(views.Phones[iPhone]);
							output.append_string("\n<br />");
						}
						for(uint32_t iPhone = 0, countWPs	 = views.WPs	.size(); iPhone < countWPs		; ++iPhone) {
							::gpk::view_const_char									textPhone						= views.WPs[iPhone];
							output.append_string("\n<a target=\"blank\" href=\"https://wa.me/");
							output.append(textPhone);
							output.append_string("\" >");
							output.append(textPhone);
							output.append_string("</a>");
							output.append_string("<br />");
						}

						//
						output.append_string("\n</td>");
						output.append_string("\n</tr>");
					}
					output.append_string("\n</table>");

				output.append_string("\n</td>");
				output.append_string("\n</tr>");
				output.append_string("\n</table>");


			output.append_string("\n</td>");
			output.append_string("\n</tr>");
		}
		output.append_string("\n</table>");

	output.append_string("\n</td>");
	output.append_string("\n</tr>");
	output.append_string("\n</table>");

	return 0;
}

static	::gpk::error_t								pageCatalog					(const ::gpk::view_const_char & contentFileName, const ::gpk::view_const_char & pathScript, const ::gpk::SCoord2<uint32_t> screenSize, const ::gpk::view_const_char & pathStyles, const AD_SHOP_CATEGORY category, const ::gpk::view_const_char & title, const ::gpk::view_const_char & lang, ::gpk::array_pod<char_t> & output) {

	output.append_string("\n<html>");
	// --- Head
	output.append_string("\n<head>");
	output.append_string("\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=0.5\" />");
	output.append_string("\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />");
	::gpk::array_pod<char_t>								fileStyle			;
	::ntl::httpPath(pathStyles, ::gpk::view_const_string{"blankstyle"}, ::gpk::view_const_string{"css"}, fileStyle);
	::ntl::htmlHeaderTitle		(title, output);
	::ntl::htmlHeaderStyleLink	({fileStyle.begin(), fileStyle.size()}, output);
	output.append_string("\n</head>");

	// --- Body
	output.append_string(
		"\n<body style=\"width:100%;height:100%;font-family:Arial;\">"
		"\n<table style=\"width:100%;height:100%;text-align:center;\">"
		"\n<tr style=\"\" >"
		"\n<td style=\"vertical-align:top;\">"
		);

	//---------------------
	::gpk::SJSONFile										config								= {};
	gpk_necall(::gpk::jsonFileRead(config, contentFileName), "Failed to load configuration file: %s.", contentFileName);
	::gpk::array_obj<::SItemViews>							indicesToDisplay					= {};
	::getItemViews(config.Reader, category, lang, indicesToDisplay);

	char													fontSize	[32]					= {};
	if(screenSize.x > screenSize.y)
		sprintf_s(fontSize, "%u", screenSize.x ? screenSize.x / 55 : 24);
	else
		sprintf_s(fontSize, "%u", screenSize.y ? screenSize.y / 44 : 24);

	output.append_string(
		"\n<div style=\"background-color:#ffffff;position:sticky;left:0;top:0;\">"
		"\n<table style=\"width:100%;height:100%;text-align:center;\">"
		"\n<tr style=\"\" >"
		"\n<td style=\"font-weight:bold;vertical-align:center;font-size:"
		);
	output.append_string(fontSize);
	output.append_string("px;\" >");

	const ::gpk::view_const_string							textSearch							= (lang == ::gpk::view_const_string{"es"}) ? "Buscar" : "Search";
	::ntl::htmlTag("h5", textSearch, {}, output);

	output.append_string(
		"\n</td>"
		"\n<td style=\"position:sticky;left:0px;top:0px;sticky;font-size:24px;font-weight:bold;width:20px;text-align:left;vertical-align:top;\">"
	//"\n<img src=\"/obelisco/image/blank.png\"/>"
		"\n</td>"
		"\n<td style=\"position:sticky;left:0px;top:0px;width:100%;text-align:left;vertical-align:top;\">"
		"<input id=\"searchBar\" oninput=\"if(this.value.length > 0) obeSearch(this.value); else clearSearch();\" style=\"position:sticky;height:100%;font-size:24px;border-width:1px;width:50%;text-align:left;border-style:solid;border-radius:8px;\" type=\"text\"></input>"
		"\n</td>"
		);

#define ENABLE_CATALOG_BARRIO
#if defined(ENABLE_CATALOG_BARRIO)
	output.append_string("\n<td style=\"font-weight:bold;font-size:");
	output.append_string(fontSize);
	output.append_string("px;\" >");
	output.append_string("<h5>Barrio:<h5>");
	output.append_string("</td>");
	output.append_string(
		"\n<td>"
		"<select id=\"selectBarrio\" onchange=\"var searchBar = document.getElementById('searchBar'); if(searchBar.value.length > 0) obeSearch(searchBar.value); else clearSearch();\" style=\"font-size:"
		);
	output.append_string(fontSize);
	output.append_string("px;\" >");

	::gpk::SJSONFile										jsonBarrios;
	::gpk::jsonFileRead(jsonBarrios, ::gpk::view_const_string{"barrios.json"});
	const ::gpk::error_t									countBarrios					= ::gpk::jsonArraySize(*jsonBarrios.Reader[0]);
	char													tempBarrioOption[256]			= {};
	::gpk::view_const_string								barrioName;
	if(lang == ::gpk::view_const_string{"es"})
		output.append_string("\n<option value=\"-1\" style=\"\">Todos</option>");
	else
		output.append_string("\n<option value=\"-1\" style=\"\">All</option>");

	;;gpk::array_obj<::SBarrio>								viewsBarrio;
	for(int32_t iBarrio = 0; iBarrio < countBarrios; ++iBarrio) {
		int32_t													indexBarrioNode					= ::gpk::jsonArrayValueGet(*jsonBarrios.Reader[0], iBarrio);
		int32_t													indexBarrioName					= ::gpk::jsonObjectValueGet(*jsonBarrios.Reader[indexBarrioNode], jsonBarrios.Reader.View, "name"		);
		int32_t													indexBarrioArea					= ::gpk::jsonObjectValueGet(*jsonBarrios.Reader[indexBarrioNode], jsonBarrios.Reader.View, "area"		);
		int32_t													indexBarrioPopulation			= ::gpk::jsonObjectValueGet(*jsonBarrios.Reader[indexBarrioNode], jsonBarrios.Reader.View, "population"	);
		int32_t													indexBarrioCommune				= ::gpk::jsonObjectValueGet(*jsonBarrios.Reader[indexBarrioNode], jsonBarrios.Reader.View, "commune"	);
		::SBarrio												barrio;
		barrio.Name											= jsonBarrios.Reader.View[indexBarrioName		];
		barrio.Area											= jsonBarrios.Reader.View[indexBarrioArea		];
		barrio.Population									= jsonBarrios.Reader.View[indexBarrioPopulation	];
		barrio.Commune										= jsonBarrios.Reader.View[indexBarrioCommune	];
		viewsBarrio.push_back(barrio);
	}

	for(uint32_t iBarrio = 0; iBarrio < viewsBarrio.size(); ++iBarrio) {
		sprintf_s(tempBarrioOption, "\n<option value=\"%u\" style=\"\">", iBarrio);
		output.append_string(tempBarrioOption);
		output.append(viewsBarrio[iBarrio].Name);
		output.append_string("</option>");
	}

	output.append_string(
		"</select>"
		"\n</td>"
		);
#endif

	output.append_string(
		"\n</tr>"
		"\n</table>"
		"\n</div>"
		);

	::htmlBoardGenerate(indicesToDisplay, ::gpk::view_const_string{fontSize}, title, lang, output);

	output.append_string("\n<script charset=\"iso-8859-1\">");
	::outputSearchScript(indicesToDisplay, output);
	output.append_string("\n</script>");

	::gpk::array_pod<char_t>								fileScriptMenu;
	::ntl::httpPath(pathScript, ::gpk::view_const_string{"filter"}, ::gpk::view_const_string{"js"}, fileScriptMenu);
	::ntl::htmlHeaderScriptFile({fileScriptMenu.begin(), fileScriptMenu.size()}, output);

	output.append_string(
		"\n</td>"
		"\n</tr>"
		"\n</table>"
		"\n</body>"
		);

	output.append_string("\n</html>");
	return 0;
}
