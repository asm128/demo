#include "ntl_front.h"
#include "ntl_session.h"

#include "gpk_cgi_app_impl_v2.h"

#include "gpk_json_expression.h"
#include "gpk_parse.h"
#include "gpk_stdstring.h"
#include "gpk_find.h"
#include "gpk_http_client.h"

GPK_CGI_JSON_APP_IMPL();

GDEFINE_ENUM_TYPE(AD_SHOP_CATEGORY, uint8_t);
GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, tours, 0);
GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, meals, 1);
GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, shops, 2);
GDEFINE_ENUM_VALUE(AD_SHOP_CATEGORY, shows, 3);

//static	::gpk::error_t								pathBuild						(::gpk::array_pod<char_t> & pathToCall, bool isPublic, ::gpk::view_const_char path, ::gpk::view_const_char endpoint, ::gpk::view_const_char command)		{
//	gpk_necall(pathToCall.append(path), "%s", "Out of memory?");
//	if(pathToCall.size() && pathToCall[pathToCall.size() - 1] != '/')
//		pathToCall.push_back('/');
//	if(isPublic)
//		gpk_necall(pathToCall.append_string("public/"), "%s", "Out of memory?");
//	gpk_necall(pathToCall.append(endpoint), "%s", "Out of memory?");
//	pathToCall.push_back('/');
//	gpk_necall(pathToCall.append(command), "%s", "Out of memory?");
//	return 0;
//}

//static	::gpk::error_t								apiRequest
//	( ::gpk::SJSONReader		& output
//	, ::gpk::SHTTPResponse		& response
//	, const ::gpk::SIPv4		& address
//	, bool						isPublic
//	, ::gpk::view_const_char	path
//	, ::gpk::view_const_char	endpoint
//	, ::gpk::view_const_char	command
//	) {
//	::gpk::array_pod<char_t>								pathToCall						= {};
//	output												= {};
//	gpk_necall(::pathBuild(pathToCall, isPublic, path, endpoint, command), "%s", "Out of memory?");
//	info_printf("Request to address: '%s'", ::gpk::toString(pathToCall).begin());
//	gpk_necall(::gpk::httpClientRequest(address, ::gpk::HTTP_METHOD_GET, "localhost", {pathToCall.begin(), pathToCall.size()}, "application/json", "", response), "%s", "Request failed.");
//	OutputDebugStringA(::gpk::toString(response.Body).begin());
//	OutputDebugStringA("\n");
// 	gpk_necall(::gpk::jsonParse(output, response.Body), "%s", "Failed to parse JSON response.");
//	return 0;
//}

static	::gpk::error_t								outputGallery					(::gpk::array_pod<char_t> & output)		{
	::gpk::SJSONReader										vehiclesReader					= {};
	::gpk::SHTTPResponse									vehiclesResponse				= {};
	::gpk::view_const_string								galleryPath						= "/whynot/module/blit.exe";
	::gpk::view_const_string								remoteTable						= "user";
	::gpk::view_const_string								remoteCommand					= "get";
	output.append_string("<table>");
	output.append_string("<tr>");
	output.append_string("<td>");

	const ::gpk::SJSONNode									& jsonNode						= *vehiclesReader.Tree[0];
	for(uint32_t iVehicle = 0; iVehicle < vehiclesReader.Tree[0]->Children.size(); ++iVehicle) {
		output.append(vehiclesReader.View[jsonNode.Children[iVehicle]->ObjectIndex]);
	}
	//output.append(vehiclesResponse.Body);

	output.append_string("</td>");
	output.append_string("</tr>");
	output.append_string("</table>");
	return 0;
}

		::gpk::error_t								gpk_cgi_generate_output			(::gpk::SCGIRuntimeValues & runtimeValues, ::gpk::array_pod<char_t> & output)	{
	::gpk::SHTTPAPIRequest									requestReceived					= {};
	bool													isCGIEnviron					= ::gpk::httpRequestInit(requestReceived, runtimeValues, true);
	::gpk::view_const_string								cookie;
	::gpk::view_const_string								sessionFileName;					;
	::gpk::array_pod<char_t>								sessionFileContents					= {};
	::gpk::array_obj<::gpk::TKeyValConstString>				cookieValues;
	if (isCGIEnviron) {
		gpk_necall(output.append_string("Content-type: text/html\r\nCache-control: no-cache\r\n"), "%s", "Out of memory?");
		gpk_necall(output.append_string("\r\n"), "%s", "Out of memory?");
		::gpk::find("HTTP_COOKIE", runtimeValues.EnvironViews, cookie);
		//if(0 == cookie.size()) {
		//	output.append_string("{ \"status\" : 403, \"description\" :\"forbidden - The server currently doesn't accept requests to this frame without a cookie set.\" }\r\n");
		//	return 1;
		//	//::sessionInitialize(requestReceived, cookie, sessionFileName, digested);
		//	//gpk_necall(output.append_string("\r\nSet-Cookie: "), "%s", "Out of memory?");
		//	//gpk_necall(output.append(cookie), "%s", "Out of memory?");
		//}
		//else {
		//	gpk_necall(output.append_string("\r\n\r\n"), "%s", "Out of memory?");
		//	gpk_necall(::ntl::sessionFileLoad(cookie, sessionFileContents, cookieValues), "%s", "Failed to load session!");
		//}
		////gpk_necall(output.append_string("\r\nSet-Cookie: "), "%s", "Out of memory?");
		////gpk_necall(output.append(cookie), "%s", "Out of memory?");

		::gpk::view_const_string							methodsValid	[]				=
			{ "GET"
			, "POST"
			};
		if(-1 == ::gpk::keyValVerify(runtimeValues.EnvironViews, "REQUEST_METHOD", methodsValid)) {
			output.append_string("{ \"status\" : 403, \"description\" :\"forbidden\" }\r\n");
			return 1;
		}
	}
	::gpk::tcpipInitialize();

	::ntl::SNTLArgs											qsArgs;
	::ntl::loadNTLArgs(qsArgs, requestReceived.QueryStringKeyVals);

	::ntl::SHTMLEndpoint									programState;
	::gpk::view_const_string								configFileName					= "./neutralizer.json";
	gpk_necall(::gpk::jsonFileRead(programState.Config, configFileName), "Failed to load configuration file: %s.", configFileName.begin());
	{
		::gpk::view_const_string								rootNode;
		const ::gpk::error_t									indexRoot						= ::gpk::jsonExpressionResolve("whynot", programState.Config.Reader, 0, rootNode);
		::ntl::frontConfigLoad(programState, indexRoot);
	}
	::gpk::view_const_string								title;

	::AD_SHOP_CATEGORY										category						= AD_SHOP_CATEGORY_tours;//{};//::gpk::get_value<::AD_SHOP_CATEGORY>(qsArgs.Module);
	if(::AD_SHOP_CATEGORY_INVALID == category)
		category = ::AD_SHOP_CATEGORY_tours;
	switch(category) {
	case ::AD_SHOP_CATEGORY_tours: if(qsArgs.Language == ::gpk::view_const_string{"es"}) title = "Turismo"		; else title = "Tourism"	; break;
	case ::AD_SHOP_CATEGORY_shops: if(qsArgs.Language == ::gpk::view_const_string{"es"}) title = "Comercios"	; else title = "Shopping"	; break;
	case ::AD_SHOP_CATEGORY_meals: if(qsArgs.Language == ::gpk::view_const_string{"es"}) title = "Comidas"		; else title = "Meals"		; break;
	case ::AD_SHOP_CATEGORY_shows: if(qsArgs.Language == ::gpk::view_const_string{"es"}) title = "Espectáculos"	; else title = "Shows"		; break;
	default: break;
	}

	::gpk::SCoord2<uint32_t>								sizeScreen						= {};
	::gpk::parseIntegerDecimal(qsArgs.Width	, &sizeScreen.x);
	::gpk::parseIntegerDecimal(qsArgs.Height, &sizeScreen.y);

	::outputGallery(output);
	//::pageCatalog(::gpk::view_const_string{"shirts.json"}, programState.Path.Script, sizeScreen, programState.Path.Style, category, title, qsArgs.Language.size() ? qsArgs.Language : ::gpk::view_const_string{"es"}, output);

	if(output.size()) {
		OutputDebugStringA(output.begin());
		OutputDebugStringA("\n");
	}
	::gpk::tcpipShutdown();
	return 0;
}

