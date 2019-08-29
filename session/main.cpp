#include "neutralizer.h"
#include "gpk_json_expression.h"
#include "gpk_parse.h"

#include "gpk_cgi_app_impl_v2.h"

GPK_CGI_JSON_APP_IMPL();

::gpk::error_t										gpk_cgi_generate_output			(::gpk::SCGIRuntimeValues & runtimeValues, ::gpk::array_pod<char_t> & output)	{
	::gpk::SHTTPAPIRequest									requestReceived					= {};
	bool													isCGIEnviron					= ::gpk::httpRequestInit(requestReceived, runtimeValues, true);
	if (isCGIEnviron) {
		gpk_necall(output.append(::gpk::view_const_string{"Content-type: text/html\r\nCache-control: no-cache\r\n"}), "%s", "Out of memory?");
		gpk_necall(output.append(::gpk::view_const_string{"\r\n"}), "%s", "Out of memory?");
		::gpk::view_const_string							methodsValid	[]				=
			{ "GET"
			, "POST"
			};
		if(0 == ::gpk::keyValVerify(runtimeValues.EnvironViews, "REQUEST_METHOD", methodsValid)) {
			output.append(::gpk::view_const_string{"{ \"status\" : 403, \"description\" :\"forbidden\" }\r\n"});
			return 1;
		}
	}
	::ntl::SNTLArgs											qsArgs;
	::ntl::loadNTLArgs(qsArgs, requestReceived.QueryStringKeyVals);

	::ntl::SHTMLEndpoint									programState;
	const char												configFileName	[]				= "./neutralizer.json";
	gpk_necall(::gpk::jsonFileRead(programState.Config, configFileName), "Failed to load configuration file: %s.", configFileName);
	{
		::gpk::view_const_string								rootNode;
		const ::gpk::error_t									indexRoot					= ::gpk::jsonExpressionResolve("tuobelisco", programState.Config.Reader, 0, rootNode);
		::ntl::loadConfig(programState, indexRoot);
	}

	if(output.size()) {
		OutputDebugStringA(output.begin());
		OutputDebugStringA("\n");
	}
	return 0;
}
