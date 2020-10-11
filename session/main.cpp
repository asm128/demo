#include "ntl_front.h"
#include "gpk_json_expression.h"
#include "gpk_parse.h"
#include "gpk_find.h"

#include "gpk_cgi_app_impl_v2.h"

GPK_CGI_JSON_APP_IMPL();

static	::gpk::error_t								generate_output_login_form		(const ::ntl::SNTLArgs & qsArgs, const ::gpk::view_const_char fontSize, const ::gpk::view_const_char & pathScript, ::gpk::array_pod<char_t> & output)	{
	output.append_string("\n<form id=\"login\" style=\"background-color:#008080;\" method=\"POST\" enctype=\"application/x-www-form-urlencoded\" action=\"session.exe?w=");
	output.append(qsArgs.Width);
	output.append_string("&h=");
	output.append(qsArgs.Height);
	output.append_string("&l=");
	output.append(qsArgs.Language);
	output.append_string("\" >");

	output.append_string("\n<table style=\"height:100%;\" >");
	output.append_string("\n<tr>");

	output.append_string("\n<td style=\"background-color:#808000;\" >");
	output.append_string("\n<input type=\"button\" name=\"signup\" onclick=\"formOnClickSubmit(true)\" value=\"");
	if(qsArgs.Language == ::gpk::view_const_string{"es"})
		output.append_string("Registrar");
	else
		output.append_string("Sign up");
	output.append_string("\" style=\"padding:6px;width:100%;height:100%;font-size:");
	output.append(fontSize);
	output.append_string("px;\" />");
	output.append_string("</td>");

	output.append_string("\n<td style=\"width:100%;background-color:#00F0F0;\" >");

		output.append_string("\n<table style=\"height:100%;width:100%;\" >");
		output.append_string("\n<tr>");

		output.append_string("\n<td style=\"background-color:#808000;font-size:");
		output.append(fontSize);
		output.append_string("px;\" />");
		if(qsArgs.Language == ::gpk::view_const_string{"es"})
			output.append_string("correo-e");
		else
			output.append_string("e-mail");
		output.append_string("</td>");

		output.append_string("\n<td>");
		output.append_string("\n<input type=\"text\" id=\"user\" onclick=\"if(document.getElementById('user_used').value == '0') { this.select(); document.getElementById('user_used').value = 1; }\" value=\"");
		if(qsArgs.Language == ::gpk::view_const_string{"es"})
			output.append_string("usuario@ej.com");
		else
			output.append_string("user@example.com");
		output.append_string("\" style=\"color:#c0c0c0;width:100%;font-size:");
		output.append(fontSize);
		output.append_string("px;\" />");	// document.getElementById('user_used');

		output.append_string("\n</td>");
		output.append_string("\n</tr>");

		output.append_string("\n<tr>");

		output.append_string("\n<td style=\"background-color:#808000;font-size:");
		output.append(fontSize);
		output.append_string("px;\" />");
		if(qsArgs.Language == ::gpk::view_const_string{"es"})
			output.append_string("contraseña");
		else
			output.append_string("password");
		output.append_string("</td>");

		output.append_string("\n<td>");
		output.append_string("\n<input type=\"password\" id=\"pass\" onclick=\"if(document.getElementById('pass_used').value == '0') { this.select(); document.getElementById('pass_used').value = 1; }\" value=\"");
		if(qsArgs.Language == ::gpk::view_const_string{"es"})
			output.append_string("contraseña");
		else
			output.append_string("password");
		output.append_string("\" style=\"color:#c0c0c0;width:100%;font-size:");
		output.append(fontSize);
		output.append_string("px;\" />");
		output.append_string("\n</td>");
		output.append_string("\n</tr>");

		output.append_string("\n<tr id=\"test_row\" style=\"visibility:collapse;\" >");

		output.append_string("\n<td style=\"background-color:#808000;font-size:");
		output.append(fontSize);
		output.append_string("px;\" />");
		if(qsArgs.Language == ::gpk::view_const_string{"es"})
			output.append_string("confirmar contraseña");
		else
			output.append_string("confirm password");
		output.append_string("</td>");

		output.append_string("\n<td>");
		output.append_string("\n<input type=\"password\" id=\"test\" style=\"height:100%;width:100%;font-size:");
		output.append(fontSize);
		output.append_string("px;\" />");
		output.append_string("\n</td>");
		output.append_string("\n<tr >");
		output.append_string("\n<td id=\"message_box\" style=\"background-color:#808000;font-size:");
		output.append(fontSize);
		output.append_string("px;\" />");
		output.append_string("");
		output.append_string("\n</td>");
		output.append_string("\n</tr>");
		output.append_string("\n</table>");

	output.append_string("\n</td>");
	//output.append_string("\n</tr>");
	//output.append_string("\n<tr>");
	output.append_string("\n<td>");

	output.append_string("\n<input type=\"button\" name=\"signin\" onclick=\"formOnClickSubmit(false)\" value=\"");
	if(qsArgs.Language == ::gpk::view_const_string{"es"})
		output.append_string("Entrar");
	else
		output.append_string("Sign in");
	output.append_string("\" style=\"padding:6px;width:100%;height:100%;font-size:");
	output.append(fontSize);
	output.append_string("px;\" />");
	output.append_string("\n<input type=\"hidden\" id=\"user_used\" value=\"0\" />");
	output.append_string("\n<input type=\"hidden\" id=\"pass_used\" value=\"0\" />");
	output.append_string("\n<input type=\"hidden\" id=\"b64user\" name=\"user\" value=\"\" />");
	output.append_string("\n<input type=\"hidden\" id=\"b64pass\" name=\"pass\" value=\"\" />");

	output.append_string("\n</td>");
	output.append_string("\n</tr>");
	output.append_string("\n</table>");
	output.append_string("\n</form>");
	::gpk::array_pod<char_t>								fileScriptMenu;
	::ntl::httpPath(pathScript, "login", ::gpk::view_const_string{"js"}, fileScriptMenu);
	::ntl::htmlHeaderScriptFile({fileScriptMenu.begin(), fileScriptMenu.size()}, output);
	return 0;
}

// Set-Cookie: <cookie-name>=<cookie-value>
// Set-Cookie: <cookie-name>=<cookie-value>; Expires=<date>
// Set-Cookie: <cookie-name>=<cookie-value>; Max-Age=<non-zero-digit>	// seconds
// Set-Cookie: <cookie-name>=<cookie-value>; Domain=<domain-value>
// Set-Cookie: <cookie-name>=<cookie-value>; Path=<path-value>
// Set-Cookie: <cookie-name>=<cookie-value>; Secure
// Set-Cookie: <cookie-name>=<cookie-value>; HttpOnly
//
// Set-Cookie: <cookie-name>=<cookie-value>; SameSite=Strict
// Set-Cookie: <cookie-name>=<cookie-value>; SameSite=Lax
// Set-Cookie: <cookie-name>=<cookie-value>; SameSite=None
//
// // Multiple directives are also possible, for example:
// Set-Cookie: <cookie-name>=<cookie-value>; Domain=<domain-value>; Secure; HttpOnly

::gpk::error_t										gpk_cgi_generate_output			(::gpk::SCGIRuntimeValues & runtimeValues, ::gpk::array_pod<char_t> & output)	{
	::gpk::SHTTPAPIRequest									requestReceived					= {};
	bool													isCGIEnviron					= ::gpk::httpRequestInit(requestReceived, runtimeValues, true);
	::gpk::view_const_string								cookie;
	if (isCGIEnviron) {
		gpk_necall(output.append_string("Content-type: text/html\r\nCache-control: no-cache"), "%s", "Out of memory?");
		::gpk::find(::gpk::vcs{"HTTP_COOKIE"}, runtimeValues.EnvironViews, cookie);
		if(0 == cookie.size()) {
			char													temp	[256]					= {};
			sprintf_s(temp, "%u", 5U);
			cookie												= ::gpk::label(temp);
			gpk_necall(output.append_string("\r\nSet-Cookie: tumama="), "%s", "Out of memory?");
			gpk_necall(output.append(cookie), "%s", "Out of memory?");
			gpk_necall(output.append_string("; Secure;"), "%s", "Out of memory?");

		}
		gpk_necall(output.append_string("\r\n\r\n"), "%s", "Out of memory?");
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
	const ::gpk::view_const_string							configFileName					= "./neutralizer.json";
	gpk_necall(::gpk::jsonFileRead(programState.Config, configFileName), "Failed to load configuration file: %s.", configFileName);
	{
		::gpk::view_const_string								rootNode;
		const ::gpk::error_t									indexRoot						= ::gpk::jsonExpressionResolve(::gpk::vcs{"tuobelisco"}, programState.Config.Reader, 0, rootNode);
		::ntl::frontConfigLoad(programState, indexRoot);
	}

	//::gpk::SIPv4							backendAddress				= {};
	//::gpk::view_const_string				udpLanIpString				= {};
	//::gpk::view_const_string				udpLanPortString			= {};
	//::gpk::error_t							indexNodeUdpLanIp			= ::gpk::jsonExpressionResolve(::gpk::vcs{"mineback.udp_lan_address"}, config.Reader, 0, udpLanIpString	);
	//::gpk::error_t							indexNodeUdpLanPort			= ::gpk::jsonExpressionResolve(::gpk::vcs{"mineback.udp_lan_port"	}, config.Reader, 0, udpLanPortString);
	//gwarn_if(errored(indexNodeUdpLanIp		), "Backend address not found in config file: %s.", configFileName);
	//gwarn_if(errored(indexNodeUdpLanPort	), "Backend address not found in config file: %s.", configFileName);
	//::gpk::tcpipInitialize();
	//gpk_necall(::gpk::tcpipAddress(udpLanIpString, udpLanPortString, backendAddress), "%s", "Cannot resolve host.");

	::gpk::array_pod<char_t>								fileStyle			;
	::ntl::httpPath(programState.Path.Style, "blankstyle", ::gpk::view_const_string{"css"}, fileStyle);

	char													fontSize	[32]				= {};
	::gpk::SCoord2<uint32_t>								sizeScreen						= {};
	::gpk::parseIntegerDecimal(qsArgs.Width	, &sizeScreen.x);
	::gpk::parseIntegerDecimal(qsArgs.Height, &sizeScreen.y);
	const uint32_t											baseSize						= (sizeScreen.x > sizeScreen.y) ? 55 : 44;
	sprintf_s(fontSize, "%u", sizeScreen.x ? sizeScreen.x / baseSize * 2 : 24);

	output.append_string("\n<html><head>");
	output.append_string("\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=0.5\" >");
	::ntl::htmlHeaderStyleLink({fileStyle.begin(), fileStyle.size()}, output);
	output.append_string("\n</head>");
	output.append_string("\n<body style=\"font-family:Arial;height:100%;width:100%;background-color:#e0e0e0;font-size:");
	output.append_string(fontSize);
	output.append_string("px;\">");

	output.append_string("\n<table style=\"width:100%;background-color:#c0c0c0;\">");
	output.append_string("\n<tr style=\"height:100%;\">");
	//output.append_string("\n<td id=\"padding\" style=\"width:100%;background-color:#800080;\" >");
	//output.append_string("\n</td>");
	output.append_string("\n<td style=\"background-color:#fc9116;font-size:");
	output.append_string(fontSize);
	output.append_string("px;\">");
	::ntl::htmlTag("h1", ::gpk::view_const_string{"tuobelisco.com"}, ::gpk::view_const_string{"style=\"font-family:Arial;\""}, output);
	output.append_string("\n</td>");
	output.append_string("\n</tr>");
	output.append_string("\n<tr>");
	//output.append_string("\n<td id=\"padding\" style=\"height:100%;width:100%;background-color:#00F080;\" >");
	//output.append_string("\n</td>");
	output.append_string("\n<td style=\"background-color:#808080;\" >");

	::generate_output_login_form(qsArgs, ::gpk::view_const_string{fontSize}, programState.Path.Script, output);

	output.append_string("\n<code>");
	output.append(runtimeValues.Content.Body.begin(), runtimeValues.Content.Length);
	output.append_string( "\n</code>");

	output.append_string("\n</td>");
	output.append_string("\n</tr>");
	output.append_string("\n</table>");

	output.append_string("\n</body>\n</html>");
	if(output.size()) {
		OutputDebugStringA(output.begin());
		OutputDebugStringA("\n");
	}
	return 0;
}
