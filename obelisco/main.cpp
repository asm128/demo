#include "neutralizer.h"

#include "gpk_cgi_app_impl_v2.h"

#include "gpk_find.h"
#include "gpk_process.h"
#include "gpk_base64.h"
#include "gpk_json_expression.h"

GPK_CGI_JSON_APP_IMPL();
// today 1565740800
// tomow 1587363200
::gpk::error_t										gpk_cgi_generate_output			(::gpk::SCGIRuntimeValues & runtimeValues, ::gpk::array_pod<char_t> & output)	{
	::gpk::SHTTPAPIRequest									requestReceived					= {};
	bool													isCGIEnviron					= ::gpk::httpRequestInit(requestReceived, runtimeValues, true);
	::gpk::array_obj<::gpk::TKeyValConstString>				environViews;
	gpk_necall(::gpk::environmentBlockViews(runtimeValues.EntryPointArgs.EnvironmentBlock, environViews), "%s", "If this breaks, we better know ASAP.");
	if (isCGIEnviron) {
		gpk_necall(output.append(::gpk::view_const_string{"Content-type: text/html\r\nCache-control: no-cache\r\n"}), "%s", "Out of memory?");
		gpk_necall(output.append(::gpk::view_const_string{"\r\n"})								, "%s", "Out of memory?");
		::gpk::view_const_string							methodsValid	[]				=
			{ "GET"
			, "POST"
			};
		if(0 == ::gpk::keyValVerify(environViews, "REQUEST_METHOD", methodsValid) || (time(0) > 1587363200U)) {
			output.append(::gpk::view_const_string{"{ \"status\" : 403, \"description\" :\"forbidden\" }\r\n"});
			return 1;
		}
	}

	::ntl::SHTMLEndpoint									programState;
	const char												configFileName	[]				= "./neutralizer.json";
	gpk_necall(::gpk::jsonFileRead(programState.Config, configFileName), "Failed to load configuration file: %s.", configFileName);
	{
		::gpk::view_const_string								rootNode;
		const ::gpk::error_t									indexRoot					= ::gpk::jsonExpressionResolve("tuobelisco", programState.Config.Reader, 0, rootNode);
		::ntl::loadConfig(programState, indexRoot);
	}
	::gpk::array_pod<char_t>								fileLogo			;
	::gpk::array_pod<char_t>								fileStyle			;
	::gpk::array_pod<char_t>								fileScriptHeader	;
	::gpk::array_pod<char_t>								fileScriptMenu		;
	::gpk::array_pod<char_t>								fileProgramContent	;
	::gpk::array_pod<char_t>								fileProgramHeader	;
	::ntl::httpPath(programState.Path.Image		, "logo_home"	, programState.Extension.Image, fileLogo);
	::ntl::httpPath(programState.Path.Style		, "blankstyle"	, "css"	, fileStyle			);
	::ntl::httpPath(programState.Path.Script	, "header"		, "js"	, fileScriptHeader	);
	::ntl::httpPath(programState.Path.Script	, "menu"		, "js"	, fileScriptMenu	);
	::ntl::httpPath(programState.Path.Program	, "shopping"	, programState.Extension.Program, fileProgramContent);
	::ntl::httpPath(programState.Path.Program	, "obelisco"	, programState.Extension.Program, fileProgramHeader);

	const ::gpk::view_const_string							htmlDefaultAnte						=
		"\n<html>"
		"\n<head>"
		"\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />"
		"\n<meta name=\"copyright\"		content=\"tuobelisco.com is a registered trademark of Pablo Ariel Zorrilla Cepeda\" />"
		"\n<meta name=\"description\"	content=\"Lugares\" />"
		"\n<meta name=\"keywords\"		content=\"obelisco, microcentro, tuobelisco, tu obelisco, tu-obelisco, tuobelisco.com, buenos aires, turismo, publicidad, publicidad online\" />"
		"\n<meta name=\"robots\"		content=\"index,follow\" />"
		"\n<meta name=\"DC.title\"		content=\"Publicidad Online\" />"
		"\n<meta name=\"viewport\"		content=\"width=device-width, initial-scale=0.5\" >"
		;
	output.append(htmlDefaultAnte);
	::ntl::htmlHeaderTitle(programState.Page.Title, output);
	::ntl::htmlHeaderScriptFile({fileScriptHeader	.begin(), fileScriptHeader	.size()}, output);
	::ntl::htmlHeaderScriptFile({fileScriptMenu		.begin(), fileScriptMenu	.size()}, output);
	::ntl::htmlHeaderStyleLink({fileStyle			.begin(), fileStyle			.size()}, output);

	const ::gpk::view_const_string							htmlDefaultAlmost					=
		"\n	<script type=\"text/javascript\">"
		"\n		function getFrameURLNoQS	()						{ return '"
		;
	output.append(htmlDefaultAlmost);
	output.append(fileProgramContent);

	const ::gpk::view_const_string							htmlDefaultBegin					=
		"'; }"
		"\n	</script>"
		"\n	</head><!--background-color:#E680C5;-->"
		"\n	<body style=\"background-color:#fc9116;\" onload=\"reframe('dumMainFrame', 'shopping', getFrameURLNoQS(), document.getElementById('frameLang').value);\">"	//202050
		"\n	<table style=\"width:100%;height:100%;\">"
		"\n		<tr >"
		"\n			<td style=\"text-align:left\" >"
		"\n	<table style=\"width:100%;height:100%;\">"
		"\n		<tr >"
		"\n			<td style=\"text-align:left\" >"
		"\n				<form style=\"width:0px;height:0px;\" id=\"homeState\">"
		"\n					<input style=\"width:0px;height:0px;\" id=\"frameName\" type=\"hidden\" value=\"0\" />"
		"\n					<input style=\"width:0px;height:0px;\" id=\"frameLang\" type=\"hidden\" value=\"es\" />"
		"\n				</form>"
		"\n				<a href=\""
		;
	output.append(htmlDefaultBegin);
	output.append(fileProgramHeader);
	output.append(::gpk::view_const_string{"\"><img style=\"height:72px\" src=\""});
	output.append(fileLogo);
	output.append(::gpk::view_const_string{
		"\" /></a>"
		"\n			</td>"
		"\n			<td style=\"width:100%;font-family:Arial;text-align:right;\" >"
		"\n	<table style=\"height:100%;width:100%;font-weight:bold;\">"
		"\n		<tr >"
		"\n			<td style=\"width:60%;height:100%;\" >"
		"\n				<img src=\"/obelisco/image/blank.png\"/>"
		"\n			</td>"
		"\n			<td style=\"text-align:center;vertical-align:center;\" onclick=\"reframe('dumMainFrame', document.getElementById('frameName').value, '"
		});
	output.append(programState.Path.Program);
	output.append(::gpk::view_const_string{"/' + document.getElementById('frameName').value + '.exe', 'en');setLang('en');\" >"});
	output.append(::gpk::view_const_string{
		"\n				<img src=\"/obelisco/image/flag_uk.png\"/> English"
		"\n			</td>"
		"\n			<td style=\"text-align:center;vertical-align:center;\" onclick=\"reframe('dumMainFrame', document.getElementById('frameName').value, '"
		});
	output.append(programState.Path.Program);
	output.append(::gpk::view_const_string{"/' + document.getElementById('frameName').value + '.exe', 'es');setLang('es');\" >"});
	output.append(::gpk::view_const_string{
		"\n				<img src=\"/obelisco/image/flag_ar.png\" /> Espa�ol"
		"\n			</td>"
		"\n		</tr>"
		"\n	</table>"
		"\n			</td>"
		"\n		</tr>"
		"\n	</table>"
		"\n			</td>"
		"\n		</tr>"
		"\n		<tr style=\"height:90%;\" >"
		"\n			<td style=\"width:100%;font-family:Arial;\" >"
		"\n				<iframe id=\"dumMainFrame\" style=\"width:100%;height:100%;\" src=\"\"></iframe>"
		"\n			</td>"
		"\n		</tr>"
		"\n		<tr >"
		"\n			<td style=\"width:100%;height:72px;font-family:Arial;\" >"
		});

	const ::ntl::SHTMLIcon									icons[]							=
		{ {"tourism"	, "Turismo"}
		, {"shopping"	, "Comercios"}
		, {"pricing"	, "Publicidad"}
		, {"terms"		, "Condiciones"}
		};
	::ntl::htmlControlMenuIconsHorizontal(icons, programState.Path.Image, programState.Extension.Image, output, false);

	const ::gpk::view_const_string							htmlDefaultPost				=
		"\n			</td>"
		"\n		</tr>"
		"\n	</table>"
		"\n</body>"
		"\n</html>"
		;
	output.append(htmlDefaultPost);
	if(output.size()) {
		OutputDebugStringA(output.begin());
		OutputDebugStringA("\n");
	}
	return 0;
}
