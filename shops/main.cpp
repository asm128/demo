#include "neutralizer.h"
#include "neutralizer_ad_category.h"
#include "gpk_json_expression.h"
#include "gpk_parse.h"

#include "gpk_cgi_app_impl_v2.h"

GPK_CGI_JSON_APP_IMPL();

::gpk::error_t										gpk_cgi_generate_output			(::gpk::SCGIRuntimeValues & runtimeValues, ::gpk::array_pod<char_t> & output)	{
	::gpk::SHTTPAPIRequest									requestReceived					= {};
	bool													isCGIEnviron					= ::gpk::httpRequestInit(requestReceived, runtimeValues, true);
	if (isCGIEnviron) {
		gpk_necall(output.append(::gpk::view_const_string{"Content-type: text/html\r\nCache-control: no-cache\r\n"}), "%s", "Out of memory?");
		gpk_necall(output.append(::gpk::view_const_string{"\r\n"})								, "%s", "Out of memory?");
		::gpk::view_const_string							methodsValid	[]				=
			{ "GET"
			, "POST"
			};
		if(0 == ::gpk::keyValVerify(runtimeValues.EnvironViews, "REQUEST_METHOD", methodsValid)) {
			output.append(::gpk::view_const_string{"{ \"status\" : 403, \"description\" :\"forbidden\" }\r\n"});
			return 1;
		}
	}

	::ntl::SHTMLEndpoint									programState;
	const char												configFileName	[]				= "./neutralizer.json";
	gpk_necall(::gpk::jsonFileRead(programState.Config, configFileName), "Failed to load configuration file: %s.", configFileName);
	{
		::gpk::view_const_string								rootNode;
		const ::gpk::error_t									indexRoot					= ::gpk::jsonExpressionResolve("obelisco", programState.Config.Reader, 0, rootNode);
		::ntl::loadConfig(programState, indexRoot);
	}
	::gpk::view_const_string								lang;
	::gpk::view_const_string								module;
	::gpk::view_const_string								width;
	::gpk::view_const_string								height;
	::gpk::find("lang"	, requestReceived.QueryStringKeyVals, lang);
	::gpk::find("module", requestReceived.QueryStringKeyVals, module);
	::gpk::find("width", requestReceived.QueryStringKeyVals, width);
	::gpk::find("height", requestReceived.QueryStringKeyVals, height);
	::gpk::view_const_string								title;

	const ::ntl::AD_SHOP_CATEGORY							category			=::gpk::get_value<::ntl::AD_SHOP_CATEGORY>(module);
	switch(category) {
	case ::ntl::AD_SHOP_CATEGORY_tours: title = (lang == ::gpk::view_const_string{"es"}) ? (::gpk::view_const_string{ "Turismo en Buenos Aires"		}) : (::gpk::view_const_string{ "Tourism in Buenos Aires"	}); break;
	case ::ntl::AD_SHOP_CATEGORY_shops: title = (lang == ::gpk::view_const_string{"es"}) ? (::gpk::view_const_string{ "Comercios en Obelisco"		}) : (::gpk::view_const_string{ "Shopping in Obelisco"		}); break;
	case ::ntl::AD_SHOP_CATEGORY_meals: title = (lang == ::gpk::view_const_string{"es"}) ? (::gpk::view_const_string{ "Comidas en Obelisco"			}) : (::gpk::view_const_string{ "Meals in Obelisco"			}); break;
	case ::ntl::AD_SHOP_CATEGORY_shows: title = (lang == ::gpk::view_const_string{"es"}) ? (::gpk::view_const_string{ "Espectáculos en Obelisco"	}) : (::gpk::view_const_string{ "Shows in Obelisco"			}); break;
	default: break;
	}

	::gpk::SCoord2<uint32_t>	sizeScreen = {};
	::gpk::parseIntegerDecimal(width, &sizeScreen.x);
	::gpk::parseIntegerDecimal(height, &sizeScreen.y);

	::ntl::pageCatalog("ads.json", sizeScreen, programState.Path.Style, category, title, lang, output);

	if(output.size()) {
		OutputDebugStringA(output.begin());
		OutputDebugStringA("\n");
	}
	return 0;
}
