#include "neutralizer.h"
#include "gpk_json_expression.h"
#include "gpk_parse.h"

#include "gpk_cgi_app_impl_v2.h"

GPK_CGI_JSON_APP_IMPL();

static	::gpk::error_t								generate_output_login_form		(const ::ntl::SNTLArgs & qsArgs, const ::gpk::view_const_string fontSize, const ::gpk::view_const_string & pathScript, ::gpk::array_pod<char_t> & output)	{
	(void)pathScript;
	output.append(::gpk::view_const_string{"\n<form id=\"login\" style=\"background-color:#008080;\" method=\"POST\" enctype=\"application/x-www-form-urlencoded\" action=\"session.exe?w="});
	output.append(qsArgs.Width);
	output.append(::gpk::view_const_string{"&h="});
	output.append(qsArgs.Height);
	output.append(::gpk::view_const_string{"&l="});
	output.append(qsArgs.Language);
	output.append(::gpk::view_const_string{"\" >"});

	output.append(::gpk::view_const_string{"\n<table style=\"height:100%;\" >"});
	output.append(::gpk::view_const_string{"\n<tr>"});

	output.append(::gpk::view_const_string{"\n<td style=\"background-color:#808000;\" >"});
	output.append(::gpk::view_const_string{"\n<input type=\"button\" name=\"signup\" onclick=\"formOnClickSubmit(true)\" value=\""});
	if(qsArgs.Language == ::gpk::view_const_string{"es"})
		output.append(::gpk::view_const_string{"Registrar"});
	else
		output.append(::gpk::view_const_string{"Sign up"});
	output.append(::gpk::view_const_string{"\" style=\"padding:6px;width:100%;height:100%;font-size:"});
	output.append(::gpk::view_const_string{fontSize});
	output.append(::gpk::view_const_string{"px;\" />"});
	output.append(::gpk::view_const_string{"</td>"});

	output.append(::gpk::view_const_string{"\n<td style=\"width:100%;background-color:#00F0F0;\" >"});

		output.append(::gpk::view_const_string{"\n<table style=\"height:100%;width:100%;\" >"});
		output.append(::gpk::view_const_string{"\n<tr>"});
		output.append(::gpk::view_const_string{"\n<td>"});
		output.append(::gpk::view_const_string{"\n<input type=\"text\" id=\"user\" onclick=\"if(document.getElementById('user_used').value == '0') { this.select(); document.getElementById('user_used').value = 1; }\" value=\""});
		if(qsArgs.Language == ::gpk::view_const_string{"es"})
			output.append(::gpk::view_const_string{"dirección de correo electrónico"});
		else
			output.append(::gpk::view_const_string{"e-mail"});
		output.append(::gpk::view_const_string{"\" style=\"width:100%;font-size:"});
		output.append(::gpk::view_const_string{fontSize});
		output.append(::gpk::view_const_string{"px;\" />"});	// document.getElementById('user_used');

		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{"\n</tr>"});

		output.append(::gpk::view_const_string{"\n<tr>"});
		output.append(::gpk::view_const_string{"\n<td>"});
		output.append(::gpk::view_const_string{"\n<input type=\"password\" id=\"pass\" onclick=\"if(document.getElementById('pass_used').value == '0') { this.select(); document.getElementById('pass_used').value = 1; }\" value=\""});
		if(qsArgs.Language == ::gpk::view_const_string{"es"})
			output.append(::gpk::view_const_string{"contraseña"});
		else
			output.append(::gpk::view_const_string{"password"});
		output.append(::gpk::view_const_string{"\" style=\"width:100%;font-size:"});
		output.append(::gpk::view_const_string{fontSize});
		output.append(::gpk::view_const_string{"px;\" />"});
		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{"\n</tr>"});

		output.append(::gpk::view_const_string{"\n<tr>"});
		output.append(::gpk::view_const_string{"\n<td>"});
		output.append(::gpk::view_const_string{"\n<input type=\"password\" id=\"test\" style=\"visibility:collapse;width:100%;font-size:"});
		output.append(::gpk::view_const_string{fontSize});
		output.append(::gpk::view_const_string{"px;\" />"});
		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{"\n</tr>"});
		output.append(::gpk::view_const_string{"\n</table>"});

	output.append(::gpk::view_const_string{"\n</td>"});
	//output.append(::gpk::view_const_string{"\n</tr>"});
	//output.append(::gpk::view_const_string{"\n<tr>"});
	output.append(::gpk::view_const_string{"\n<td>"});

	output.append(::gpk::view_const_string{"\n<input type=\"button\" name=\"signin\" onclick=\"formOnClickSubmit(false)\" value=\""});
	if(qsArgs.Language == ::gpk::view_const_string{"es"})
		output.append(::gpk::view_const_string{"Entrar"});
	else
		output.append(::gpk::view_const_string{"Sign in"});
	output.append(::gpk::view_const_string{"\" style=\"padding:6px;width:100%;height:100%;font-size:"});
	output.append(::gpk::view_const_string{fontSize});
	output.append(::gpk::view_const_string{"px;\" />"});
	output.append(::gpk::view_const_string{ "\n<input type=\"hidden\" id=\"user_used\" value=\"0\" />" });
	output.append(::gpk::view_const_string{ "\n<input type=\"hidden\" id=\"pass_used\" value=\"0\" />" });
	output.append(::gpk::view_const_string{ "\n<input type=\"hidden\" id=\"b64user\" name=\"user\" value=\"\" />" });
	output.append(::gpk::view_const_string{ "\n<input type=\"hidden\" id=\"b64pass\" name=\"pass\" value=\"\" />" });

	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});
	output.append(::gpk::view_const_string{"\n</table>"});
	output.append(::gpk::view_const_string{"\n</form>"});
	::gpk::array_pod<char_t>								fileScriptMenu;
	::ntl::httpPath(pathScript, "login", "js", fileScriptMenu);
	::ntl::htmlHeaderScriptFile({fileScriptMenu.begin(), fileScriptMenu.size()}, output);
	return 0;
}

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
		if(-1 == ::gpk::keyValVerify(runtimeValues.EnvironViews, "REQUEST_METHOD", methodsValid)) {
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
		const ::gpk::error_t									indexRoot						= ::gpk::jsonExpressionResolve("tuobelisco", programState.Config.Reader, 0, rootNode);
		::ntl::loadConfig(programState, indexRoot);
	}
	::gpk::array_pod<char_t>								fileStyle			;
	::ntl::httpPath(programState.Path.Style, "blankstyle", "css", fileStyle			);

	char													fontSize	[32]				= {};
	::gpk::SCoord2<uint32_t>								sizeScreen						= {};
	::gpk::parseIntegerDecimal(qsArgs.Width	, &sizeScreen.x);
	::gpk::parseIntegerDecimal(qsArgs.Height, &sizeScreen.y);
	const uint32_t											baseSize						= (sizeScreen.x > sizeScreen.y) ? 55 : 44;
	sprintf_s(fontSize, "%u", sizeScreen.x ? sizeScreen.x / baseSize * 2 : 24);

	output.append(::gpk::view_const_string{"\n<html><head>"});
	output.append(::gpk::view_const_string{"\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=0.5\" >"});
	::ntl::htmlHeaderStyleLink({fileStyle.begin(), fileStyle.size()}, output);
	output.append(::gpk::view_const_string{"\n</head>"});
	output.append(::gpk::view_const_string{"\n<body style=\"font-family:Arial;height:100%;width:100%;background-color:#e0e0e0;font-size:"});
	output.append(::gpk::view_const_string{fontSize});
	output.append(::gpk::view_const_string{"px;\">"});

	output.append(::gpk::view_const_string{"\n<table style=\"width:100%;background-color:#c0c0c0;\">"});
	output.append(::gpk::view_const_string{"\n<tr style=\"height:100%;\">"});
	//output.append(::gpk::view_const_string{"\n<td id=\"padding\" style=\"width:100%;background-color:#800080;\" >"});
	//output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n<td style=\"background-color:#fc9116;font-size:"});
	output.append(::gpk::view_const_string{fontSize});
	output.append(::gpk::view_const_string{"px;\">"});
	::ntl::htmlTag("h1", "tuobelisco.com", "style=\"font-family:Arial;\"", output);
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});
	output.append(::gpk::view_const_string{"\n<tr>"});
	//output.append(::gpk::view_const_string{"\n<td id=\"padding\" style=\"height:100%;width:100%;background-color:#00F080;\" >"});
	//output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n<td style=\"background-color:#808080;\" >"});

	::generate_output_login_form(qsArgs, fontSize, programState.Path.Script, output);

	output.append(::gpk::view_const_string{ "\n<code>"	});
	output.append(runtimeValues.Content.Body.begin(), runtimeValues.Content.Length);
	output.append(::gpk::view_const_string{ "\n</code>"	});

	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});
	output.append(::gpk::view_const_string{"\n</table>"});

	output.append(::gpk::view_const_string{"\n</body>\n</html>"});
	if(output.size()) {
		OutputDebugStringA(output.begin());
		OutputDebugStringA("\n");
	}
	return 0;
}
