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
		const ::gpk::error_t									indexRoot						= ::gpk::jsonExpressionResolve("tuobelisco", programState.Config.Reader, 0, rootNode);
		::ntl::loadConfig(programState, indexRoot);
	}
	::gpk::array_pod<char_t>								fileStyle			;
	::ntl::httpPath(programState.Path.Style, "blankstyle", "css", fileStyle			);

	char													fontSize	[32]					= {};
	::gpk::SCoord2<uint32_t>								sizeScreen							= {};
	::gpk::parseIntegerDecimal(qsArgs.Width	, &sizeScreen.x);
	::gpk::parseIntegerDecimal(qsArgs.Height, &sizeScreen.y);
	const uint32_t											baseSize							= (sizeScreen.x > sizeScreen.y) ? 55 : 44;
	sprintf_s(fontSize, "%u", sizeScreen.x ? sizeScreen.x / baseSize * 2 : 24);

	output.append(::gpk::view_const_string{"\n<html><head>"});
	output.append(::gpk::view_const_string{"\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=0.5\" >"});
	::ntl::htmlHeaderStyleLink({fileStyle.begin(), fileStyle.size()}, output);
	output.append(::gpk::view_const_string{"\n</head>"});
	output.append(::gpk::view_const_string{"\n<body style=\"height:100%;width:100%;background-color:#e0e0e0;font-size:"});
	output.append(::gpk::view_const_string{fontSize});
	output.append(::gpk::view_const_string{"px;\">"});

	::ntl::htmlTag("h1", "tuobelisco.com", "style=\"font-family:Arial;\"", output);

	output.append(::gpk::view_const_string{"\n<table style=\"width:100%;background-color:#c0c0c0;\">"});
	output.append(::gpk::view_const_string{"\n<tr style=\"height:100%;\">"});
	//output.append(::gpk::view_const_string{"\n<td id=\"padding\" style=\"width:100%;background-color:#800080;\" >"});
	//output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n<td style=\"background-color:#F000F0;\" >"});
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});
	output.append(::gpk::view_const_string{"\n<tr>"});
	//output.append(::gpk::view_const_string{"\n<td id=\"padding\" style=\"height:100%;width:100%;background-color:#00F080;\" >"});
	//output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n<td style=\"background-color:#808080;\" >"});

		output.append(::gpk::view_const_string{"\n<form style=\"background-color:#008080;\" method=\"POST\" enctype=\"application/x-www-form-urlencoded\" action=\"session.exe?w="});
		output.append(qsArgs.Width);
		output.append(::gpk::view_const_string{"&h="});
		output.append(qsArgs.Height);
		output.append(::gpk::view_const_string{"\" >"});

		output.append(::gpk::view_const_string{"\n<table style=\"height:100%;\" >"});
		output.append(::gpk::view_const_string{"\n<tr>"});

		output.append(::gpk::view_const_string{"\n<td style=\"background-color:#808000;\" >"});
		output.append(::gpk::view_const_string{"\n<input type=\"submit\" name=\"signup\" value=\""});
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
			output.append(::gpk::view_const_string{"\n<input type=\"text\" name=\"user\" value=\""});
			if(qsArgs.Language == ::gpk::view_const_string{"es"})
				output.append(::gpk::view_const_string{"dirección de correo electrónico"});
			else
				output.append(::gpk::view_const_string{"e-mail"});
			output.append(::gpk::view_const_string{"\" style=\"width:100%;font-size:"});
			output.append(::gpk::view_const_string{fontSize});
			output.append(::gpk::view_const_string{"px;\" />"});

			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});

			output.append(::gpk::view_const_string{"\n<tr>"});
			output.append(::gpk::view_const_string{"\n<td>"});
			output.append(::gpk::view_const_string{"\n<input type=\"password\" name=\"pass\" value=\""});
			if(qsArgs.Language == ::gpk::view_const_string{"es"})
				output.append(::gpk::view_const_string{"contraseña"});
			else
				output.append(::gpk::view_const_string{"password"});
			output.append(::gpk::view_const_string{"\" style=\"width:100%;font-size:"});
			output.append(::gpk::view_const_string{fontSize});
			output.append(::gpk::view_const_string{"px;\" />"});
			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
			output.append(::gpk::view_const_string{"\n</table>"});

		output.append(::gpk::view_const_string{"\n</td>"});
		//output.append(::gpk::view_const_string{"\n</tr>"});
		//output.append(::gpk::view_const_string{"\n<tr>"});
		output.append(::gpk::view_const_string{"\n<td>"});

		output.append(::gpk::view_const_string{"\n<input type=\"submit\" name=\"signin\" value=\""});
		if(qsArgs.Language == ::gpk::view_const_string{"es"})
			output.append(::gpk::view_const_string{"Entrar"});
		else
			output.append(::gpk::view_const_string{"Sign in"});
		output.append(::gpk::view_const_string{"\" style=\"padding:6px;width:100%;height:100%;font-size:"});
		output.append(::gpk::view_const_string{fontSize});
		output.append(::gpk::view_const_string{"px;\" />"});

		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{"\n</tr>"});
		output.append(::gpk::view_const_string{"\n</table>"});
		output.append(::gpk::view_const_string{"\n</form>"});

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
