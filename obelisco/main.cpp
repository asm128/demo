#include "neutralizer.h"

#include "gpk_cgi_app_impl_v2.h"

#include "gpk_find.h"
#include "gpk_process.h"
#include "gpk_base64.h"
#include "gpk_json_expression.h"
#include "gpk_chrono.h"
#include "gpk_parse.h"

GPK_CGI_JSON_APP_IMPL();
// today 1565740800
// tomow 1587363200
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

	::gpk::view_const_string								lang;
	::gpk::view_const_string								module;
	::gpk::view_const_string								width;
	::gpk::view_const_string								height;
	::gpk::find("lang"	, requestReceived.QueryStringKeyVals, lang);
	::gpk::find("module", requestReceived.QueryStringKeyVals, module);
	::gpk::find("width"	, requestReceived.QueryStringKeyVals, width);
	::gpk::find("height", requestReceived.QueryStringKeyVals, height);
	::gpk::view_const_string								title;

	::ntl::SHTMLEndpoint									programState;
	const char												configFileName	[]				= "./neutralizer.json";
	gpk_necall(::gpk::jsonFileRead(programState.Config, configFileName), "Failed to load configuration file: %s.", configFileName);
	{
		::gpk::view_const_string								rootNode;
		const ::gpk::error_t									indexRoot					= ::gpk::jsonExpressionResolve("tuobelisco", programState.Config.Reader, 0, rootNode);
		::ntl::loadConfig(programState, indexRoot);
	}
	::gpk::array_pod<char_t>								fileLogo			;
	::gpk::array_pod<char_t>								fileImageLangEng	;
	::gpk::array_pod<char_t>								fileImageLangEsp	;
	::gpk::array_pod<char_t>								fileImageCopysign	;
	::gpk::array_pod<char_t>								fileStyle			;
	::gpk::array_pod<char_t>								fileScriptHeader	;
	::gpk::array_pod<char_t>								fileScriptMenu		;
	::gpk::array_pod<char_t>								fileProgramContent	;
	::gpk::array_pod<char_t>								fileProgramHeader	;
	::ntl::httpPath(programState.Path.Image		, "logo_home"			, programState.Extension.Image, fileLogo);
	::ntl::httpPath(programState.Path.Image		, "flag_uk"				, programState.Extension.Image, fileImageLangEng);
	::ntl::httpPath(programState.Path.Image		, "flag_ar"				, programState.Extension.Image, fileImageLangEsp);
	::ntl::httpPath(programState.Path.Image		, "icon_small_copyright", programState.Extension.Image, fileImageCopysign);
	::ntl::httpPath(programState.Path.Style		, "blankstyle"			, "css"	, fileStyle			);
	::ntl::httpPath(programState.Path.Script	, "header"				, "js"	, fileScriptHeader	);
	::ntl::httpPath(programState.Path.Script	, "menu"				, "js"	, fileScriptMenu	);

	srand((uint32_t)::gpk::timeCurrentInUs());
	::gpk::view_const_string section;
	switch(rand() % 4) {
	case 0: section = "shops"; break;
	case 1: section = "meals"; break;
	case 2: section = "shows"; break;
	case 3: section = "tours"; break;
	}

	::ntl::httpPath(programState.Path.Program	, "shops"		, programState.Extension.Program, fileProgramContent);
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

	output.append(::gpk::view_const_string{"\n<link rel=\"icon\" type=\"image/png\" href=\"/favicon.png\">"});
	output.append(::gpk::view_const_string{"\n</head>"});
	output.append(::gpk::view_const_string{"\n	<body style=\"background-color:#fc9116;\" onload=\"reframe('dumMainFrame', '"	}); // onresize=\"reframe('dumMainFrame', document.getElementById('frameName').value, document.getElementById('frameProgram').value, document.getElementById('frameLang').value);\"
	output.append(section);
	output.append(::gpk::view_const_string{"', '"});
	output.append(fileProgramContent);
	output.append(::gpk::view_const_string{"', document.getElementById('frameLang').value);\">"
		"\n	<table style=\"width:100%;height:100%;\">"
		"\n		<tr >"
		"\n			<td style=\"text-align:left\" >"
		"\n	<table style=\"width:100%;height:100%;\">"
		"\n		<tr >"
		"\n			<td style=\"text-align:left\" >"
		"\n				<form style=\"width:0px;height:0px;\" id=\"homeState\">"
		"\n					<input style=\"width:0px;height:0px;\" id=\"frameName\"		type=\"hidden\" value=\"0\" />"
		"\n					<input style=\"width:0px;height:0px;\" id=\"frameProgram\"	type=\"hidden\" value=\"0\" />"
		"\n					<input style=\"width:0px;height:0px;\" id=\"frameLang\"		type=\"hidden\" value=\"es\" />"
		//"\n					<input style=\"width:0px;height:0px;\" id=\"frameBarrio\"	type=\"hidden\" value=\"-1\" />"
		"\n				</form>"
		"\n				<a href=\""
		});
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
		"\n<table style=\"width: 100%; height: 100%; text-align: center; color: rgb(0, 0, 0);\" id=\"aWRfaWNvbl8=\" onmouseout=\"cellColor('aWRfaWNvbl8=', null, 0);\" onmouseover=\"cellColor('aWRfaWNvbl8=', '#ec8106', 0);\">"
		"\n<tbody><tr>															"
		"\n<td>																	"
"<!-- I got these buttons from simplesharebuttons.com -->"
//"\n<style type=\"text/css\">"
//"\n#share-buttons img {"
//"\nwidth: 35px;"
//"\npadding: 5px;"
//"\nborder: 0;"
//"\nbox-shadow: 0;"
//"\ndisplay: inline;"
//"\n}"
//"\n</style>"
//"<div id=\"share-buttons\">"
/*<!-- Facebook		-->*/ "	<a href=\"http://www.facebook.com/sharer.php?u=http://tuobelisco.com\" target=\"_blank\">										<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/facebook.png\" alt=\"Facebook\" /></a>"
/*<!-- LinkedIn		-->*/ "	<a href=\"http://www.linkedin.com/shareArticle?mini=true&amp;url=http://tuobelisco.com\" target=\"_blank\">						<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/linkedin.png\" alt=\"LinkedIn\" /></a>"
/*<!-- Reddit		-->*/ "	<a href=\"http://reddit.com/submit?url=http://tuobelisco.com&amp;title=TuObelisco\" target=\"_blank\">							<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/reddit.png\" alt=\"Reddit\" /></a>"
/*<!-- Tumblr		-->*/ "	<a href=\"http://www.tumblr.com/share/link?url=http://tuobelisco.com&amp;title=TuObelisco\" target=\"_blank\">					<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/tumblr.png\" alt=\"Tumblr\" /></a>"
/*<!-- Twitter		-->*/ "	<a href=\"https://twitter.com/share?url=http://tuobelisco.com&amp;text=TuObelisco&amp;hashtags=tuobelisco\" target=\"_blank\">	<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/twitter.png\" alt=\"Twitter\" /></a>"
/*<!-- Pinterest	-->*/ "	<a href=\"javascript:void((function()%7Bvar%20e=document.createElement('script');e.setAttribute('type','text/javascript');e.setAttribute('charset','UTF-8');e.setAttribute('src','http://assets.pinterest.com/js/pinmarklet.js?r='+Math.random()*99999999);document.body.appendChild(e)%7D)());\">"
"<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/pinterest.png\" alt=\"Pinterest\" /></a>"
/*<!-- Buffer		-->*/ //"	<a href=\"https://bufferapp.com/add?url=http://tuobelisco.com&amp;text=TuObelisco\" target=\"_blank\">						<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/buffer.png\" alt=\"Buffer\" /></a>"
/*<!-- Digg			-->*/ //"	<a href=\"http://www.digg.com/submit?url=http://tuobelisco.com\" target=\"_blank\">											<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/diggit.png\" alt=\"Digg\" /></a>"
/*<!-- Email		-->*/ //"	<a href=\"mailto:?Subject=Tu Obelisco&amp;Body=I%20saw%20this%20and%20thought%20of%20you!%20 http://tuobelisco.com\">		<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/email.png\" alt=\"Email\" /></a>"
/*<!-- Google+		-->*/ //"	<a href=\"https://plus.google.com/share?url=http://tuobelisco.com\" target=\"_blank\">										<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/google.png\" alt=\"Google\" /></a>"
/*<!-- StumbleUpon	-->*/ //"	<a href=\"http://www.stumbleupon.com/submit?url=http://tuobelisco.com&amp;title=TuObelisco\" target=\"_blank\">				<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/stumbleupon.png\" alt=\"StumbleUpon\" /></a>"
/*<!-- VK			-->*/ //"	<a href=\"http://vkontakte.ru/share.php?url=http://tuobelisco.com\" target=\"_blank\">										<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/vk.png\" alt=\"VK\" /></a>"
/*<!-- Yummly		-->*/ //"	<a href=\"http://www.yummly.com/urb/verify?url=http://tuobelisco.com&amp;title=TuObelisco\" target=\"_blank\">				<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/yummly.png\" alt=\"Yummly\" /></a>"
/*<!-- Print		-->*/ //"	<a href=\"javascript:;\" onclick=\"window.print()\">																		<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/print.png\" alt=\"Print\" /></a>"
//"</div>"
		});
	output.append(::gpk::view_const_string{"\n</td><td onclick=\"reframe('dumMainFrame', 'copyright', 'copyright.exe', document.getElementById('frameLang').value);\" ><img style=\"\" src=\""});
	output.append(fileImageCopysign);
	output.append(::gpk::view_const_string{
		"\" >"
		"\n</td></tr>															"
		"\n<tr>																	"
		"\n<td style=\"ont-size:1.5em;&quot;\">									"
		"\n<p style=\"color:black;font-size:1.5em;\" id=\"copyright\"></p>		"
		"\n</td>																"
		"\n</tr>																"
		"\n</tbody></table>														"
		//"\n				<img src=\"/obelisco/image/blank.png\"/>"
		"\n			</td>"
		});
//-----------------------------------------------------------------

#if defined(ENABLE_MAINFRAME_BARRIO)
	::gpk::SCoord2<uint32_t>								sizeScreen						= {};
	::gpk::parseIntegerDecimal(width	, &sizeScreen.x);
	::gpk::parseIntegerDecimal(height	, &sizeScreen.y);

	char													fontSize	[32]					= {};
	if(sizeScreen.x > sizeScreen.y)
		sprintf_s(fontSize, "%u", sizeScreen.x ? sizeScreen.x / 55 : 24);
	else
		sprintf_s(fontSize, "%u", sizeScreen.y ? sizeScreen.y / 44 : 24);

	output.append(::gpk::view_const_string{"\n<td style=\"font-weight:bold;font-size:"});
	output.append(::gpk::view_const_string{fontSize});
	output.append(::gpk::view_const_string{ "px;\" >"});
	output.append(::gpk::view_const_string{"<h5>Barrio:<h5>"});
	output.append(::gpk::view_const_string{"</td>"});
	output.append(::gpk::view_const_string{
		"\n<td>"
		"<select id=\"testSelect1\" onchange=\"reframe('dumMainFrame', document.getElementById('frameName').value, document.getElementById('frameProgram').value, document.getElementById('frameLang').value);\" style=\"font-size:"
		});
	output.append(::gpk::view_const_string{fontSize});
	output.append(::gpk::view_const_string{ "px;\" >"});

	::gpk::SJSONFile										barrios							;
	::gpk::jsonFileRead(barrios, "barrios.json");
	const ::gpk::error_t									countBarrios					= ::gpk::jsonArraySize(*barrios.Reader[0]);
	char													tempBarrioOption[256]			= {};
	::gpk::view_const_string								barrioName;
	//if(lang == ::gpk::view_const_string{"es"})
		output.append(::gpk::view_const_string{"\n<option id=\"barrioOption\" value=\"-1\" style=\"\">Todos</option>"});
	//else
	//	output.append(::gpk::view_const_string{"\n<option id=\"barrioOption\" value=\"-1\" style=\"\">All</option>"});
	for(int32_t iBarrio = 0; iBarrio < countBarrios; ++iBarrio) {
		int32_t													indexBarrioNode					= ::gpk::jsonArrayValueGet(*barrios.Reader[0], iBarrio);
		int32_t													indexBarrioName					= ::gpk::jsonObjectValueGet(*barrios.Reader[indexBarrioNode], barrios.Reader.View, "name");
		sprintf_s(tempBarrioOption, "\n<option value=\"%u\" style=\"\">", iBarrio);
		output.append(::gpk::view_const_string{tempBarrioOption});
		output.append(barrios.Reader.View[indexBarrioName]);
		output.append(::gpk::view_const_string{"</option>"});
	}

	output.append(::gpk::view_const_string{
		"</select>"
		"\n</td>"
		});
#endif
//-----------------------------------------------------------------
	output.append(::gpk::view_const_string{
		"\n			<td style=\"text-align:center;vertical-align:center;\" onclick=\"reframe('dumMainFrame', document.getElementById('frameName').value, document.getElementById('frameProgram').value, 'en');setLang('en');\" >"
		"\n				<img src=\""
		});
	output.append(fileImageLangEng);
	output.append(::gpk::view_const_string{
		"\" /> English"
		"\n			</td>"
		"\n			<td style=\"text-align:center;vertical-align:center;\" onclick=\"reframe('dumMainFrame', document.getElementById('frameName').value, document.getElementById('frameProgram').value, 'es');setLang('es');\" >"
		"\n				<img src=\""
		});
	output.append(fileImageLangEsp);
	output.append(::gpk::view_const_string{
		"\" /> Español"
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
		{ {"tours"		, "shops"		, "Turismo y Guías"			}
		, {"shops"		, "shops"		, "Comercio y Servicios"	}
		, {"shows"		, "shops"		, "Shows y Arte"			}
		, {"meals"		, "shops"		, "Comidas y Snacks"		}
		, {"pricing"	, "pricing"		, "Publicar"}
		};
	::ntl::htmlControlMenuIconsHorizontal(icons, programState.Path.Image, programState.Extension.Image, output, false);

	output.append(::gpk::view_const_string{
		"\n			</td>"
		"\n		</tr>"
		"\n	</table>"
		"\n</body>"
		"\n</html>"
		});
	if(output.size()) {
		OutputDebugStringA(output.begin());
		OutputDebugStringA("\n");
	}
	return 0;
}
