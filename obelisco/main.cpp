#include "neutralizer.h"

#include "gpk_cgi_app_impl_v2.h"

#include "gpk_find.h"
#include "gpk_process.h"
#include "gpk_base64.h"
#include "gpk_json_expression.h"
#include "gpk_chrono.h"

GPK_CGI_JSON_APP_IMPL();
// today 1565740800
// tomow 1587363200
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

	srand((uint32_t)::gpk::timeCurrentInUs());
	::gpk::view_const_string section;
	switch(rand() % 4) {
	case 0	: section = "shops"		; break;
	case 1	: section = "meals"		; break;
	case 2	: section = "shows"		; break;
	case 3	: section = "tourism"	; break;
	}

	::ntl::httpPath(programState.Path.Program	, section		, programState.Extension.Program, fileProgramContent);
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

	output.append(::gpk::view_const_string{"\n</head>"});
	output.append(::gpk::view_const_string{"\n	<body style=\"background-color:#fc9116;\" onload=\"reframe('dumMainFrame', '"	});
	output.append(section);
	output.append(::gpk::view_const_string{"', '"});
	output.append(fileProgramContent);
	output.append(::gpk::view_const_string{"', document.getElementById('frameLang').value);\">"	});
	const ::gpk::view_const_string							htmlDefaultBegin					=
		"\n	<table style=\"width:100%;height:100%;\">"
		"\n		<tr >"
		"\n			<td style=\"text-align:left\" >"
		"\n	<table style=\"width:100%;height:100%;\">"
		"\n		<tr >"
		"\n			<td style=\"text-align:left\" >"
		"\n				<form style=\"width:0px;height:0px;\" id=\"homeState\">"
		"\n					<input style=\"width:0px;height:0px;\" id=\"frameName\"		type=\"hidden\" value=\"0\" />"
		"\n					<input style=\"width:0px;height:0px;\" id=\"frameLang\"		type=\"hidden\" value=\"es\" />"
		//"\n					<input style=\"width:0px;height:0px;\" id=\"frameCategory\"	type=\"hidden\" value=\"0\" />"
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
		"\n<table style=\"width: 100%; height: 100%; text-align: center; color: rgb(0, 0, 0);\" id=\"aWRfaWNvbl8=\" onmouseout=\"cellColor('aWRfaWNvbl8=', null, 0);\" onmouseover=\"cellColor('aWRfaWNvbl8=', '#ec8106', 0);\">"
		"\n<tbody><tr>															"
		"\n<td>																	"
//"<!-- I got these buttons from simplesharebuttons.com -->"
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
"	<!-- Facebook		--><a href=\"http://www.facebook.com/sharer.php?u=http://tuobelisco.com\" target=\"_blank\">													<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/facebook.png\" alt=\"Facebook\" /></a>"
"	<!-- LinkedIn		--><a href=\"http://www.linkedin.com/shareArticle?mini=true&amp;url=http://tuobelisco.com\" target=\"_blank\">									<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/linkedin.png\" alt=\"LinkedIn\" /></a>"
"	<!-- Reddit			--><a href=\"http://reddit.com/submit?url=http://tuobelisco.com&amp;title=TuObelisco\" target=\"_blank\">										<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/reddit.png\" alt=\"Reddit\" /></a>"
"	<!-- Tumblr			--><a href=\"http://www.tumblr.com/share/link?url=http://tuobelisco.com&amp;title=TuObelisco\" target=\"_blank\">								<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/tumblr.png\" alt=\"Tumblr\" /></a>"
"	<!-- Twitter		--><a href=\"https://twitter.com/share?url=http://tuobelisco.com&amp;text=TuObelisco&amp;hashtags=tuobelisco\" target=\"_blank\"><img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/twitter.png\" alt=\"Twitter\" /></a>"
"	<!-- Pinterest		--><a href=\"javascript:void((function()%7Bvar%20e=document.createElement('script');e.setAttribute('type','text/javascript');e.setAttribute('charset','UTF-8');e.setAttribute('src','http://assets.pinterest.com/js/pinmarklet.js?r='+Math.random()*99999999);document.body.appendChild(e)%7D)());\">"
"<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/pinterest.png\" alt=\"Pinterest\" /></a>"
//"	<!-- Buffer			--><a href=\"https://bufferapp.com/add?url=http://tuobelisco.com&amp;text=TuObelisco\" target=\"_blank\">										<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/buffer.png\" alt=\"Buffer\" /></a>"
//"	<!-- Digg			--><a href=\"http://www.digg.com/submit?url=http://tuobelisco.com\" target=\"_blank\">															<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/diggit.png\" alt=\"Digg\" /></a>"
//"	<!-- Email			--><a href=\"mailto:?Subject=Tu Obelisco&amp;Body=I%20saw%20this%20and%20thought%20of%20you!%20 http://tuobelisco.com\">						<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/email.png\" alt=\"Email\" /></a>"
//"	<!-- Google+		--><a href=\"https://plus.google.com/share?url=http://tuobelisco.com\" target=\"_blank\">														<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/google.png\" alt=\"Google\" /></a>"
//"	<!-- StumbleUpon	--><a href=\"http://www.stumbleupon.com/submit?url=http://tuobelisco.com&amp;title=TuObelisco\" target=\"_blank\">								<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/stumbleupon.png\" alt=\"StumbleUpon\" /></a>"
//"	<!-- VK				--><a href=\"http://vkontakte.ru/share.php?url=http://tuobelisco.com\" target=\"_blank\">														<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/vk.png\" alt=\"VK\" /></a>"
//"	<!-- Yummly			--><a href=\"http://www.yummly.com/urb/verify?url=http://tuobelisco.com&amp;title=TuObelisco\" target=\"_blank\">								<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/yummly.png\" alt=\"Yummly\" /></a>"
//"	<!-- Print			--><a href=\"javascript:;\" onclick=\"window.print()\">																							<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/print.png\" alt=\"Print\" /></a>"
//"</div>"
		"\n</td><td onclick=\"reframe('dumMainFrame', 'copyright', 'copyright.exe', document.getElementById('frameLang').value);\" ><img style=\"\" src=\"/tuobelisco/image/icon_small_copyright.png\">	"
		"\n</td></tr>															"
		"\n<tr>																	"
		"\n<td style=\"ont-size:1.5em;&quot;\">									"
		"\n<p style=\"color:black;font-size:1.5em;\" id=\"copyright\"></p>		"
		"\n</td>																"
		"\n</tr>																"
		"\n</tbody></table>														"
		//"\n				<img src=\"/obelisco/image/blank.png\"/>"
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
		"\n				<img src=\"/obelisco/image/flag_ar.png\" /> Español"
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
		{ {"tours"		, "Turismo y Guías"			}
		, {"shops"		, "Comercio y Servicios"	}
		, {"shows"		, "Shows y Arte"			}
		, {"meals"		, "Comidas y Snacks"		}
		, {"pricing"	, "Publicar"}
//		, {"terms"		, "Términos"}
//		, {"copyright"	, ""}
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
