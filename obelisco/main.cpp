#include "ntl_front.h"
#include "ntl_session.h"

#include "gpk_cgi_app_impl_v2.h"

#include "gpk_process.h"
#include "gpk_json_expression.h"
#include "gpk_chrono.h"
#include "gpk_parse.h"
#include "gpk_encoding.h"
#include "gpk_base64.h"
#include "gpk_noise.h"
#include "gpk_aes.h"
#include "gpk_file.h"
#include <ctime>

GPK_CGI_JSON_APP_IMPL();
// today 1565740800
// tomow 1587363200
static	::gpk::error_t								keyGen							(::gpk::vc key)	{
	for(uint32_t iVal = 0; iVal < key.size(); ++iVal)
		while(0 == key[iVal])
			key[iVal]										= (char)rand();
	return 0;
}

static	::gpk::error_t								sessionInitialize
	( const ::gpk::SHTTPAPIRequest	& requestReceived
	, ::gpk::vcs		& cookie
	, ::gpk::vcs		& sessionFileName
	, ::gpk::ac						& digested
	) {
	char													temp		[256]					= {};
	char													keyAES		[256]					= {};
	srand((int)time(0));
	::keyGen(keyAES);
	uint64_t												keyArdell							= ::gpk::noise1DBase(::gpk::timeCurrentInUs());
	for(uint32_t iRound = 0, countRounds = rand(); iRound < countRounds; ++iRound)
		keyArdell											+= ::gpk::noise1DBase(::gpk::timeCurrentInUs());;
	sprintf_s(temp, "%llu", keyArdell);
	digested.append_string(requestReceived.Ip);
	digested.append_string("_");
	::gpk::digest(::gpk::vcs{temp}, digested);

	::gpk::au8									encryptedName;
	::gpk::achar								hexedName;
	::gpk::aesEncode({digested}, ::gpk::vcc{keyAES, 32}, ::gpk::AES_LEVEL_256, encryptedName);
	::gpk::hexEncode(encryptedName, hexedName);
	sessionFileName										= ::gpk::label(hexedName.begin(), ::gpk::min(hexedName.size(), 64U));
	::gpk::achar								sessionFileContents					= {};
	ree_if(0 == ::gpk::fileToMemory(sessionFileName, sessionFileContents), "Invalid session name: '%s'. Already exists!", sessionFileName.begin());

	::gpk::achar								strCookie							= ::gpk::vcs{"tumama="};
	strCookie.append(sessionFileName);
	strCookie.push_back(';');
	//gpk_necall(strCookie.append_string(" Secure;"), "%s", "Out of memory?");

	::gpk::achar								b64Key;
	::gpk::base64Encode(keyAES, b64Key);
	cookie												= ::gpk::label(strCookie.begin(), strCookie.size());
	sessionFileContents.push_back('{');
	sessionFileContents.append_string("\"cookie\":\"");
	gpk_necall(sessionFileContents.append(cookie), "%s", "Out of memory?");
	sessionFileContents.append_string("\",\"key\":\"");
	gpk_necall(sessionFileContents.append(b64Key), "%s", "Out of memory?");
	sessionFileContents.append_string("\",\"ip\":[\"");
	gpk_necall(sessionFileContents.append(requestReceived.Ip), "%s", "Out of memory?");
	sessionFileContents.append_string("\"],\"user\":\"anon\"");
	sessionFileContents.push_back('}');
	gpk_necall(::gpk::fileFromMemory(sessionFileName, sessionFileContents), "Failed to write seesion file: '%s'. Disk full?", sessionFileName.begin());
	return 0;
}

::gpk::error_t			gpk_cgi_generate_output				(::gpk::SCGIRuntimeValues & runtimeValues, ::gpk::achar & output)	{
	::gpk::SHTTPAPIRequest		requestReceived						= {};
	bool						isCGIEnviron						= ::gpk::httpRequestInit(requestReceived, runtimeValues, true);
	::gpk::vcs					cookie;
	::gpk::vcs					sessionFileName;					;
	::gpk::achar				sessionFileContents					= {};
	::gpk::ac					digested;
	::gpk::aobj<::gpk::TKeyValConstString>				cookieValues;
	if (isCGIEnviron) {
		gpk_necall(output.append_string("Content-type: text/html\r\nCache-control: no-cache"), "%s", "Out of memory?");
		::gpk::find(::gpk::vcs{"HTTP_COOKIE"}, runtimeValues.EnvironViews, cookie);
		if(0 == cookie.size()) {
			::sessionInitialize(requestReceived, cookie, sessionFileName, digested);
			gpk_necall(output.append_string("\r\nSet-Cookie: "), "%s", "Out of memory?");
			gpk_necall(output.append(cookie), "%s", "Out of memory?");
		}
		else {
		}
		gpk_necall(output.append_string("\r\n\r\n"), "%s", "Out of memory?");
		gpk_necall(::ntl::sessionFileLoad(cookie, sessionFileContents, cookieValues), "%s", "Failed to load session!");
		::gpk::vcc							methodsValid	[]				=
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
	if(0 == qsArgs.Language.size())
		qsArgs.Language										= ::gpk::vcs{"es"};

	::gpk::SJSONFile										sessionFile;
	::gpk::vcs								section;
	const ::gpk::vcs							folderSession					= {};
	const ::gpk::vcs							folderUser						= {};
	if(0 == qsArgs.Session.size()) {
		const uint64_t											timeInUs						= ::gpk::timeCurrentInUs();
		char													strTempSession	[2048]			= {};
		sprintf_s(strTempSession, "%s_%llu", requestReceived.Ip.begin(), timeInUs);
		::gpk::au8								encrypted;
		srand((uint32_t)::gpk::noise1DBase(::gpk::timeCurrentInUs()));
		::gpk::ardellEncode(::gpk::vcs{strTempSession}, (uint32_t)rand(), true, encrypted);
		::gpk::achar								encoded;
		::gpk::base64EncodeFS(encrypted, encoded);
		qsArgs.Session										= ::gpk::label(encoded.begin(), encoded.size());
		FILE													* fp							= 0;
		fopen_s(&fp, qsArgs.Session.begin(), "rb");
		ree_if(fp, "%s", "Some invalid access caused a redundant anon session.");
		switch(rand() % 4) {
		case 0: section = "shops"; break;
		case 1: section = "meals"; break;
		case 2: section = "shows"; break;
		case 3: section = "tours"; break;
		}
	}
	else {
		::gpk::achar								finalPath						= ::gpk::vcs{"session/"};
		gpk_necall(finalPath.append(qsArgs.Session), "Failed to append path. Invalid session id?");
		::gpk::jsonFileRead(sessionFile, {finalPath.begin(), finalPath.size()});
		section												= "tours";
	}

	::gpk::vcs								title;

	::ntl::SHTMLEndpoint									programState;
	const char												configFileName	[]				= "./neutralizer.json";
	gpk_necall(::gpk::jsonFileRead(programState.Config, configFileName), "Failed to load configuration file: %s.", configFileName);
	{
		::gpk::vcs								rootNode;
		const ::gpk::error_t									indexRoot						= ::gpk::jsonExpressionResolve(::gpk::vcs{"tuobelisco"}, programState.Config.Reader, 0, rootNode);
		::ntl::frontConfigLoad(programState, indexRoot);
	}
	::gpk::achar								fileLogo			;
	::gpk::achar								fileImageLangEng	;
	::gpk::achar								fileImageLangEsp	;
	::gpk::achar								fileImageCopysign	;
	::gpk::achar								fileImageSignedOut	;
	::gpk::achar								fileStyle			;
	::gpk::achar								fileScriptHeader	;
	::gpk::achar								fileProgramContent	;
	::gpk::achar								fileProgramHeader	;
	::gpk::achar								fileProgramSession	;
	::ntl::httpPath(programState.Path.Image		, ::gpk::vcs{"logo_home"			}, programState.Extension.Image, fileLogo			);
	::ntl::httpPath(programState.Path.Image		, ::gpk::vcs{"flag_uk"				}, programState.Extension.Image, fileImageLangEng	);
	::ntl::httpPath(programState.Path.Image		, ::gpk::vcs{"flag_ar"				}, programState.Extension.Image, fileImageLangEsp	);
	::ntl::httpPath(programState.Path.Image		, ::gpk::vcs{"icon_small_signed_out"}, programState.Extension.Image, fileImageSignedOut	);
	::ntl::httpPath(programState.Path.Image		, ::gpk::vcs{"icon_small_copyright"	}, programState.Extension.Image, fileImageCopysign	);
	::ntl::httpPath(programState.Path.Style		, ::gpk::vcs{"blankstyle"			}, ::gpk::vcs{"css"	}, fileStyle		);
	::ntl::httpPath(programState.Path.Script	, ::gpk::vcs{"header"				}, ::gpk::vcs{"js"	}, fileScriptHeader	);

	::ntl::httpPath(programState.Path.Program	, ::gpk::vcs{"shops"	}, programState.Extension.Program, fileProgramContent);
	::ntl::httpPath(programState.Path.Program	, ::gpk::vcs{"obelisco"	}, programState.Extension.Program, fileProgramHeader);
	::ntl::httpPath(programState.Path.Program	, ::gpk::vcs{"session"	}, programState.Extension.Program, fileProgramSession);

	const ::gpk::vcs							htmlDefaultAnte					=
		"\n<html>"
		"\n<head>"
		"\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />"
		"\n<meta name=\"copyright\"		content=\"tuobelisco.com is a registered trademark of Pablo Ariel Zorrilla Cepeda\" />"
		"\n<meta name=\"description\"	content=\"Lugares\" />"
		"\n<meta name=\"keywords\"		content=\"obelisco, microcentro, tuobelisco, tu obelisco, tu-obelisco, tuobelisco.com, buenos aires, turismo, publicidad, publicidad online\" />"
		"\n<meta name=\"robots\"		content=\"index,follow\" />"
		"\n<meta name=\"DC.title\"		content=\"Publicidad Online\" />"
		"\n<meta name=\"viewport\"		content=\"width=device-width, initial-scale=0.5\" />"
		;
	output.append(htmlDefaultAnte);
	::ntl::htmlHeaderTitle(programState.Page.Title, output);
	::ntl::htmlHeaderScriptFile	({fileScriptHeader	.begin(), fileScriptHeader	.size()}, output);
	::ntl::htmlHeaderStyleLink	({fileStyle			.begin(), fileStyle			.size()}, output);

	output.append_string("\n<link rel=\"icon\" type=\"image/png\" href=\"/favicon.png\">");
	output.append_string("\n</head>");
	output.append_string("\n	<body style=\"background-color:#fc9116;\" onload=\"reframe('tuMainFrame', '"); // onresize=\"reframe('tuMainFrame', document.getElementById('m').value, document.getElementById('p').value, document.getElementById('l').value, document.getElementById('s').value);\"
	output.append(section);
	output.append_string("', '");
	output.append(fileProgramContent);
	output.append_string("', document.getElementById('l').value, document.getElementById('s').value);\">"
		"\n	<table style=\"width:100%;height:100%;\">"
		"\n		<tr >"
		"\n			<td style=\"text-align:left\" >"
		"\n	<table style=\"width:100%;height:100%;\">"
		"\n		<tr >"
		"\n			<td style=\"text-align:left\" >"
		"\n				<form style=\"width:0px;height:0px;\" id=\"homeState\">"
		"\n					<input style=\"width:0px;height:0px;\" id=\"m\"	type=\"hidden\" value=\"0\" />"
		"\n					<input style=\"width:0px;height:0px;\" id=\"p\"	type=\"hidden\" value=\"0\" />"
		"\n					<input style=\"width:0px;height:0px;\" id=\"l\"	type=\"hidden\" value=\"es\" />"
		"\n					<input style=\"width:0px;height:0px;\" id=\"s\"	type=\"hidden\" value=\""
		);
	output.append(qsArgs.Session);
	output.append_string(
		"\" />"
		"\n				</form>"
		"\n				<a href=\""
		);
	output.append(fileProgramHeader);
	output.append_string("\"><img style=\"height:72px\" src=\"");
	output.append(fileLogo);
	output.append_string(
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
///*<!-- Pinterest	-->*/ "	<a href=\"javascript:void((function()%7Bvar%20e=document.createElement('script');e.setAttribute('type','text/javascript');e.setAttribute('charset','UTF-8');e.setAttribute('src','http://assets.pinterest.com/js/pinmarklet.js?r='+Math.random()*99999999);document.body.appendChild(e)%7D)());\">"
//"<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/pinterest.png\" alt=\"Pinterest\" /></a>"
///*<!-- Buffer		-->*/ "	<a href=\"https://bufferapp.com/add?url=http://tuobelisco.com&amp;text=TuObelisco\" target=\"_blank\">						<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/buffer.png\" alt=\"Buffer\" /></a>"
///*<!-- Digg			-->*/ "	<a href=\"http://www.digg.com/submit?url=http://tuobelisco.com\" target=\"_blank\">											<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/diggit.png\" alt=\"Digg\" /></a>"
///*<!-- Email		-->*/ "	<a href=\"mailto:?Subject=Tu Obelisco&amp;Body=I%20saw%20this%20and%20thought%20of%20you!%20 http://tuobelisco.com\">		<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/email.png\" alt=\"Email\" /></a>"
///*<!-- Google+		-->*/ "	<a href=\"https://plus.google.com/share?url=http://tuobelisco.com\" target=\"_blank\">										<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/google.png\" alt=\"Google\" /></a>"
///*<!-- StumbleUpon	-->*/ "	<a href=\"http://www.stumbleupon.com/submit?url=http://tuobelisco.com&amp;title=TuObelisco\" target=\"_blank\">				<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/stumbleupon.png\" alt=\"StumbleUpon\" /></a>"
///*<!-- VK			-->*/ "	<a href=\"http://vkontakte.ru/share.php?url=http://tuobelisco.com\" target=\"_blank\">										<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/vk.png\" alt=\"VK\" /></a>"
///*<!-- Yummly		-->*/ "	<a href=\"http://www.yummly.com/urb/verify?url=http://tuobelisco.com&amp;title=TuObelisco\" target=\"_blank\">				<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/yummly.png\" alt=\"Yummly\" /></a>"
///*<!-- Print		-->*/ "	<a href=\"javascript:;\" onclick=\"window.print()\">																		<img style=\"width:32px;\" src=\"https://simplesharebuttons.com/images/somacro/print.png\" alt=\"Print\" /></a>"
//"</div>"
		);
	output.append_string("\n</td><td onclick=\"reframe('tuMainFrame', 'copyright', 'copyright.exe', document.getElementById('l').value, document.getElementById('s').value);\" ><img style=\"\" src=\"");
	output.append(fileImageCopysign);
	output.append_string(
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
		);
//-----------------------------------------------------------------

#if defined(ENABLE_MAINFRAME_BARRIO)
	::gpk::
		n2u32								sizeScreen						= {};
	::gpk::parseIntegerDecimal(width	, sizeScreen.x);
	::gpk::parseIntegerDecimal(height	, sizeScreen.y);

	char													fontSize	[32]					= {};
	if(sizeScreen.x > sizeScreen.y)
		sprintf_s(fontSize, "%u", sizeScreen.x ? sizeScreen.x / 55 : 24);
	else
		sprintf_s(fontSize, "%u", sizeScreen.y ? sizeScreen.y / 44 : 24);

	output.append_string("\n<td style=\"font-weight:bold;font-size:");
	output.append_string(fontSize);
	output.append_string("px;\" >");
	output.append_string("<h5>Barrio:<h5>");
	output.append_string("</td>");
	output.append_string(
		"\n<td>"
		"<select id=\"testSelect1\" onchange=\"reframe('tuMainFrame', document.getElementById('m').value, document.getElementById('p').value, document.getElementById('l').value, document.getElementById('s').value);\" style=\"font-size:"
		);
	output.append_string(fontSize);
	output.append_string("px;\" >");

	::gpk::SJSONFile										barrios							;
	::gpk::jsonFileRead(barrios, "barrios.json");
	const ::gpk::error_t									countBarrios					= ::gpk::jsonArraySize(*barrios.Reader[0]);
	char													tempBarrioOption[256]			= {};
	::gpk::vcs								barrioName;
	//if(lang == ::gpk::vcs{"es"})
		output.append_string("\n<option id=\"barrioOption\" value=\"-1\" style=\"\">Todos</option>");
	//else
	//	output.append_string("\n<option id=\"barrioOption\" value=\"-1\" style=\"\">All</option>");
	for(int32_t iBarrio = 0; iBarrio < countBarrios; ++iBarrio) {
		int32_t													indexBarrioNode					= ::gpk::jsonArrayValueGet(*barrios.Reader[0], iBarrio);
		int32_t													indexBarrioName					= ::gpk::jsonObjectValueGet(*barrios.Reader[indexBarrioNode], barrios.Reader.View, "name");
		sprintf_s(tempBarrioOption, "\n<option value=\"%u\" style=\"\">", iBarrio);
		output.append_string(tempBarrioOption);
		output.append(barrios.Reader.View[indexBarrioName]);
		output.append_string("</option>");
	}

	output.append_string(
		"</select>"
		"\n</td>"
		);
#endif
//-----------------------------------------------------------------
	output.append_string(
		"\n			<td style=\"text-align:center;vertical-align:center;\" onclick=\"reframe('tuMainFrame', document.getElementById('m').value, document.getElementById('p').value, 'en', document.getElementById('s').value);setLang('en');\" >"
		"\n				<img src=\""
		);
	output.append(fileImageLangEng);
	output.append_string(
		"\" /> English"
		"\n			</td>"
		"\n			<td style=\"text-align:center;vertical-align:center;\" onclick=\"reframe('tuMainFrame', document.getElementById('m').value, document.getElementById('p').value, 'es', document.getElementById('s').value);setLang('es');\" >"
		"\n				<img src=\""
		);
	output.append(fileImageLangEsp);
	output.append_string(
		"\" /> Espa�ol"
		"\n			</td>"
		"\n			<td style=\"text-align:center;vertical-align:center;\" >"	// onclick=\"reframe('tuMainFrame', document.getElementById('m').value, document.getElementById('p').value, document.getElementById('l').value, document.getElementById('s').value);\"
		);
	output.append_string(
		"\n				<a onclick=\"var frameRect = document.getElementById('tuMainFrame').getBoundingClientRect(); window.location = 'session.exe?w=' + frameRect.width + '&h=' + frameRect.height + '&l="
		);
	output.append(qsArgs.Language);
	output.append_string("';\" >");
	output.append_string(
		"\n<img src=\""
		);
	output.append(fileImageSignedOut);
	output.append_string(
		"\" /></a>"
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
		"\n				<iframe id=\"tuMainFrame\" style=\"width:100%;height:100%;\" src=\"\"></iframe>"
		"\n			</td>"
		"\n		</tr>"
		"\n		<tr >"
		"\n			<td style=\"width:100%;height:72px;font-family:Arial;\" >"
		);
	const ::ntl::SHTMLIcon									icons[]							=
		{ {::gpk::vcs{"tours"		}, ::gpk::vcs{"shops"		}, ::gpk::vcs{"Turismo y<br/> Gu�as"		}}
		, {::gpk::vcs{"shops"		}, ::gpk::vcs{"shops"		}, ::gpk::vcs{"Comercios y<br/> Servicios"}}
		, {::gpk::vcs{"shows"		}, ::gpk::vcs{"shops"		}, ::gpk::vcs{"Shows y<br/> Arte"			}}
		, {::gpk::vcs{"meals"		}, ::gpk::vcs{"shops"		}, ::gpk::vcs{"Comidas y<br/> Snacks"		}}
		//, {::gpk::vcs{"pricing"	}, ::gpk::vcs{"pricing"	}, ::gpk::vcs{"Publicar"					}}
		};
	::ntl::htmlControlMenuIconsHorizontal(icons, programState.Path.Image, programState.Extension.Image, output, false);

	output.append_string(
		"\n			</td>"
		"\n		</tr>"
		"\n	</table>"
		);
	//output.append_string("\n<code>");
	//output.append(sessionFileName);
	//output.append_string( "\n</code>");
	//output.append_string("\n<code>");
	//for(uint32_t iPair = 0; iPair < cookieValues.size(); ++iPair) {
	//	output.append(cookieValues[iPair].Key);
	//	output.push_back('*');
	//	output.append(cookieValues[iPair].Val);
	//	output.push_back('\n');
	//}
	//output.append_string( "\n</code>");
	//output.append_string("\n<code>");
	//for(uint32_t iPair = 0; iPair < cookiePairs.size(); ++iPair) {
	//	output.append(cookiePairs[iPair]);
	//	output.push_back('*');
	//	output.push_back('\n');
	//}
	//output.append_string( "\n</code>");
	//
	//output.append_string("\n<code>");
	//output.append(digested);
	//output.append_string( "\n</code>");
	//output.append_string("\n<code>");
	//output.append(cookie);
	//output.append_string( "\n</code>");
	//output.append_string("\n<code>");
	//output.append(sessionFileContents);
	//output.append_string( "\n</code>");

	output.append_string(
		"\n</body>"
		"\n</html>"
		);
	if(output.size()) {
		OutputDebugStringA(output.begin());
		OutputDebugStringA("\n");
	}
	return 0;
}
