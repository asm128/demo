#include "neutralizer.h"

#include "gpk_cgi_app_impl_v2.h"

#include "gpk_find.h"
#include "gpk_process.h"
#include "gpk_base64.h"
#include "gpk_json_expression.h"

GPK_CGI_JSON_APP_IMPL();

static	::gpk::error_t								htmlBoardGenerate				(::gpk::view_const_string lang, ::gpk::array_pod<char_t> & output)	{
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"});

	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;font-size:24px; font-weight:bold; vertical-align:top;\">"});
	if(lang == ::gpk::view_const_string{"es"})
		output.append(::gpk::view_const_string{ "\n<h1>Turismo en Buenos Aires</h1>"});
	else
		output.append(::gpk::view_const_string{ "\n<h1>Tourism in Buenos Aires</h1>"});
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;font-size:24px; font-weight:bold; vertical-align:top;\">"});

	//---------------------
	static constexpr	const char							contentFileName	[]				= "./tourism.json";
	::gpk::SJSONFile										config							= {};
	gpk_necall(::gpk::jsonFileRead(config, contentFileName), "Failed to load configuration file: %s.", contentFileName);
	const ::gpk::error_t									countItems						= ::gpk::jsonArraySize(*config.Reader[0]);
	for(int32_t iItem = 0; iItem < countItems; ++iItem) {
		const ::gpk::error_t									jsonIndexCurrentItem			= ::gpk::jsonArrayValueGet(*config.Reader[0], iItem);
		::gpk::view_const_string								viewWikiURL						= {};
		::gpk::view_const_string								viewWikiTitle					= {};
		::gpk::view_const_string								viewWikiText					= {};
		::gpk::view_const_string								viewWikiImageHRef				= {};
		::gpk::view_const_string								viewWikiImageSrc				= {};
		::gpk::view_const_string								viewWikiImageTitle				= {};
		::gpk::view_const_string								viewWikiImageAlt				= {};
		::gpk::view_const_string								viewMapURL						= {};
		const ::gpk::error_t									jsonIndexMap					= ::gpk::jsonExpressionResolve("location", config.Reader, jsonIndexCurrentItem, viewMapURL);
		if(lang == ::gpk::view_const_string{"es"}) {
			const ::gpk::error_t									jsonIndexWiki					= ::gpk::jsonExpressionResolve("es.wiki"		, config.Reader, jsonIndexCurrentItem, viewWikiURL			);
			const ::gpk::error_t									jsonIndexName					= ::gpk::jsonExpressionResolve("es.title"		, config.Reader, jsonIndexCurrentItem, viewWikiTitle		);
			const ::gpk::error_t									jsonIndexText					= ::gpk::jsonExpressionResolve("es.text"		, config.Reader, jsonIndexCurrentItem, viewWikiText			);
			const ::gpk::error_t									jsonIndexImageHRef				= ::gpk::jsonExpressionResolve("es.image.href"	, config.Reader, jsonIndexCurrentItem, viewWikiImageHRef	);
			const ::gpk::error_t									jsonIndexImageSrc				= ::gpk::jsonExpressionResolve("es.image.src"	, config.Reader, jsonIndexCurrentItem, viewWikiImageSrc		);
			const ::gpk::error_t									jsonIndexImageTitle				= ::gpk::jsonExpressionResolve("es.image.title"	, config.Reader, jsonIndexCurrentItem, viewWikiImageTitle	);
			const ::gpk::error_t									jsonIndexImageAlt				= ::gpk::jsonExpressionResolve("es.image.alt"	, config.Reader, jsonIndexCurrentItem, viewWikiImageAlt		);
		}
		else {
			const ::gpk::error_t									jsonIndexWiki					= ::gpk::jsonExpressionResolve("en.wiki"		, config.Reader, jsonIndexCurrentItem, viewWikiURL			);
			const ::gpk::error_t									jsonIndexName					= ::gpk::jsonExpressionResolve("en.title"		, config.Reader, jsonIndexCurrentItem, viewWikiTitle		);
			const ::gpk::error_t									jsonIndexText					= ::gpk::jsonExpressionResolve("en.text"		, config.Reader, jsonIndexCurrentItem, viewWikiText			);
			const ::gpk::error_t									jsonIndexImageHRef				= ::gpk::jsonExpressionResolve("en.image.href"	, config.Reader, jsonIndexCurrentItem, viewWikiImageHRef	);
			const ::gpk::error_t									jsonIndexImageSrc				= ::gpk::jsonExpressionResolve("en.image.src"	, config.Reader, jsonIndexCurrentItem, viewWikiImageSrc		);
			const ::gpk::error_t									jsonIndexImageTitle				= ::gpk::jsonExpressionResolve("en.image.title"	, config.Reader, jsonIndexCurrentItem, viewWikiImageTitle	);
			const ::gpk::error_t									jsonIndexImageAlt				= ::gpk::jsonExpressionResolve("en.image.alt"	, config.Reader, jsonIndexCurrentItem, viewWikiImageAlt		);
		}
		output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;text-align:center;border-style:solid;border-width:2px;\" >"});
		output.append(::gpk::view_const_string{ "\n<tr>"});
		output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;text-align:left;font-size:32px;vertical-align:top;\">"});
		//
			output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;border-style:solid;border-width:2px;border-radius:16px;\" >"});
			output.append(::gpk::view_const_string{ "\n<tr>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:left;font-size:32px;vertical-align:top;\">"});
			//
			::ntl::htmlHRefLink(viewWikiTitle, viewWikiURL, "style=\"text-align:left;text-decoration:none;font-weight:bold;\"", output);
			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
			output.append(::gpk::view_const_string{ "\n<tr>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:white;height:100%;text-align:left;font-size:32px;vertical-align:top;\">"});
			::ntl::htmlTag("p", viewWikiText, "style=\" font-weight:normal;text-align:left;\"", output);
			//
			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
			output.append(::gpk::view_const_string{"\n</table>"});
		//
		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;height:100%;text-align:left;font-size:32px;vertical-align:top;\">"});

			output.append(::gpk::view_const_string{ "\n<table style=\"background-color:lightgrey;\" >"});
			output.append(::gpk::view_const_string{ "\n<tr>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:left;font-size:32px;vertical-align:top;\">"});
			output.append(::gpk::view_const_string{ "<a target=\"blank\" title=\""});
			output.append(viewWikiImageTitle);
			output.append(::gpk::view_const_string{ "\" href=\""});
			output.append(viewWikiImageHRef);
			output.append(::gpk::view_const_string{ "\" class=\"image\" ><img decoding=\"async\" elementtiming=\"thumbnail\" width=\"245\" height=\"184\" alt=\""});
			output.append(viewWikiImageAlt);
			output.append(::gpk::view_const_string{ "\" src=\""});
			output.append(viewWikiImageSrc);
			output.append(::gpk::view_const_string{ "\"></a>"});
			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
			output.append(::gpk::view_const_string{ "\n<tr>"});
			output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;font-size:32px;vertical-align:top;\">"});

			//
				output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;border-style:solid;border-width:2px;border-radius:16px;\" >"});
				output.append(::gpk::view_const_string{ "\n<tr>"});
				output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:center;font-size:32px;vertical-align:top;\">"});
				output.append(::gpk::view_const_string{"\n<a target=\"blank\" style=\"margin:4px;font-size:24px;\" href=\""});
				output.append(viewMapURL);
				output.append(::gpk::view_const_string{"\">"});
				if(lang == ::gpk::view_const_string{"es"})
					output.append(::gpk::view_const_string{"Localización"});
				else
					output.append(::gpk::view_const_string{"Location"});
				output.append(::gpk::view_const_string{"\n</a>"});
				output.append(::gpk::view_const_string{"\n</td>"});
				output.append(::gpk::view_const_string{"\n</tr>"});
				output.append(::gpk::view_const_string{ "\n<tr style=\"height:100%;\">"});
				output.append(::gpk::view_const_string{ "\n<td style=\"background-color:lightgrey;text-align:center;font-size:24px;vertical-align:top;border-style:solid;border-top-width:1px;\">"});
				output.append(::gpk::view_const_string{"\n<br />"});

				output.append(::gpk::view_const_string{"\n</td>"});
				output.append(::gpk::view_const_string{"\n</tr>"});
				output.append(::gpk::view_const_string{"\n</table>"});




			output.append(::gpk::view_const_string{"\n</td>"});
			output.append(::gpk::view_const_string{"\n</tr>"});
			output.append(::gpk::view_const_string{"\n</table>"});


		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{"\n</tr>"});
		output.append(::gpk::view_const_string{"\n</table>"});
	}
	//---------------------<a href="https://commons.wikimedia.org/wiki/File:ObeliscoBA2017.jpg"><img width="512" alt="ObeliscoBA2017" src="https://upload.wikimedia.org/wikipedia/commons/thumb/0/0f/ObeliscoBA2017.jpg/512px-ObeliscoBA2017.jpg"></a>
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});

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
	::gpk::array_pod<char_t>								fileStyle			;
	::ntl::httpPath(programState.Path.Style		, "blankstyle"	, "css"	, fileStyle			);

	output.append(::gpk::view_const_string{ "\n<html>"});
	output.append(::gpk::view_const_string{ "\n<head>"});
	output.append(::gpk::view_const_string{ "\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />"});
	::ntl::htmlHeaderTitle		(programState.Page.Title, output);
	::ntl::htmlHeaderStyleLink	({fileStyle.begin(), fileStyle.size()}, output);
	output.append(::gpk::view_const_string{"\n</head>"});

	output.append(::gpk::view_const_string{"\n<body style=\"width:100%;height:100%;background-color:#E0E0E0;font-family:Arial;\">"	}); // 202050
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"	});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"font-size:16px; font-weight:bold; vertical-align:top;\">"});
	::gpk::view_const_string								lang;
	::gpk::find("lang", requestReceived.QueryStringKeyVals, lang);
	::htmlBoardGenerate(lang, output);

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
