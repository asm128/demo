#include "neutralizer.h"

#include "gpk_cgi_app_impl_v2.h"

#include "gpk_find.h"
#include "gpk_process.h"
#include "gpk_base64.h"
#include "gpk_json_expression.h"

GPK_CGI_JSON_APP_IMPL();

static	::gpk::error_t								htmlBoardGenerate				(::gpk::array_pod<char_t> & output)	{
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"});

	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;font-size:24px; font-weight:bold; vertical-align:top;\">"});
	output.append(::gpk::view_const_string{ "\n<h1>Tourism in Buenos Aires</h1>"});
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});

	//---------------------
	const char												contentFileName	[]				= "./tourism.json";
	::gpk::SJSONFile										config							= {};
	gpk_necall(::gpk::jsonFileRead(config, contentFileName), "Failed to load configuration file: %s.", contentFileName);
	const ::gpk::error_t									countItems						= ::gpk::jsonArraySize(*config.Reader[0]);
	for(int32_t iItem = 0; iItem < countItems; ++iItem) {
		const ::gpk::error_t									jsonIndexCurrentItem			= ::gpk::jsonArrayValueGet(*config.Reader[0], iItem);
		const ::gpk::error_t									jsonIndexWiki					= ::gpk::jsonObjectValueGet(*config.Reader[jsonIndexCurrentItem], config.Reader.View, "wiki");
		const ::gpk::error_t									jsonIndexName					= ::gpk::jsonObjectValueGet(*config.Reader[jsonIndexCurrentItem], config.Reader.View, "title");
		output.append(::gpk::view_const_string{ "\n<tr>"});
		output.append(::gpk::view_const_string{ "\n<td style=\"text-align:left;font-size:32px;vertical-align:top;\">"});
		output.append(::gpk::view_const_string{ "\n<br/>"	});
		::ntl::htmlHRefLink(config.Reader.View[jsonIndexName], config.Reader.View[jsonIndexWiki], "style=\"text-align:left;text-decoration:none;font-weight:bold;\"", output);
		//::ntl::htmlTag("h4", config.Reader.View[jsonIndexName], "style=\"text-align:left;\"", output);
		output.append(::gpk::view_const_string{ "\n<br/>"	});
		//::ntl::htmlHRefLink(config.Reader.View[jsonIndexWiki], config.Reader.View[jsonIndexWiki], "style=\"text-align:left;text-decoration:none;font-weight:normal;\"", output);
		::ntl::htmlTag("p", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris ac lacus ut lorem dictum viverra eu maximus est. Nullam auctor nisi nec odio euismod volutpat. Proin porttitor in nulla et aliquet. Nulla nec vulputate tellus, sit amet faucibus dui. Morbi pharetra imperdiet pretium. Mauris ullamcorper dictum mauris sit amet rhoncus. Nulla blandit sem non erat lacinia, in imperdiet elit iaculis. Vestibulum nec dignissim metus. Quisque accumsan urna at sapien convallis, at sollicitudin magna lobortis. Curabitur vulputate aliquet pharetra. Fusce non velit nisl. Mauris ligula nulla, ultricies at nisl sit amet, semper sollicitudin mi.", "style=\" font-weight:normal;text-align:left;\"", output);
		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{"\n</tr>"});
	}
	//---------------------

	output.append(::gpk::view_const_string{"\n</table>"});
	return 0;
}

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
		if(0 == ::gpk::keyValVerify(environViews, "REQUEST_METHOD", methodsValid)) {
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
	::ntl::httpPath(programState.Path.Program	, "adverwall"	, programState.Extension.Program, fileProgramContent);
	::ntl::httpPath(programState.Path.Program	, "obelisco"	, programState.Extension.Program, fileProgramHeader);
	const ::gpk::view_const_string							txtTitle						= "- Obelisco - Un Patrimonio Incalculable de la República de la Nación Argentina -";

	output.append(::gpk::view_const_string{ "\n<html>"});
	output.append(::gpk::view_const_string{ "\n<head>"});
	::ntl::htmlHeaderTitle		(txtTitle, output);
	::ntl::htmlHeaderScriptFile	({fileScriptHeader	.begin(), fileScriptHeader	.size()}, output);
	::ntl::htmlHeaderScriptFile	({fileScriptMenu	.begin(), fileScriptMenu	.size()}, output);
	::ntl::htmlHeaderStyleLink	({fileStyle			.begin(), fileStyle			.size()}, output);
	output.append(::gpk::view_const_string{"\n</head>"});

	output.append(::gpk::view_const_string{"\n<body style=\"width:100%;height:100%;background-color:#E0E0E0;font-family:Arial;\">"	}); // 202050
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"	});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"font-size:16px; font-weight:bold; vertical-align:top;\">"});
	::htmlBoardGenerate(output);

	output.append(::gpk::view_const_string{"\n</td>"			});
	output.append(::gpk::view_const_string{"\n</tr>"			});
	output.append(::gpk::view_const_string{"\n</table>"			});
	output.append(::gpk::view_const_string{ "\n<code >"	});
	output.append(runtimeValues.Content.Body.begin(), runtimeValues.Content.Length);
	output.append(::gpk::view_const_string{ "\n</code >"	});
	output.append(::gpk::view_const_string{"\n</body>"			});
	output.append(::gpk::view_const_string{"\n</html>"			});

	if(output.size()) {
		OutputDebugStringA(output.begin());
		OutputDebugStringA("\n");
	}
	return 0;
}
