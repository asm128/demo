#include "neutralizer.h"

#include "gpk_cgi_app_impl_v2.h"

#include "gpk_find.h"
#include "gpk_process.h"
#include "gpk_base64.h"
#include "gpk_json_expression.h"

GPK_CGI_JSON_APP_IMPL();

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
	::ntl::httpPath(programState.Path.Image, "logo_home"	, programState.Extension.Image, fileLogo);
	::ntl::httpPath(programState.Path.Style, "blankstyle"	, "css"	, fileStyle);

	output.append(::gpk::view_const_string{ "\n<html>"});
	output.append(::gpk::view_const_string{ "\n<head>"});
	output.append(::gpk::view_const_string{ "\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />"});
	::ntl::htmlHeaderTitle(programState.Page.Title, output);
	::ntl::htmlHeaderStyleLink({fileStyle.begin(), fileStyle.size()}, output);
	output.append(::gpk::view_const_string{"\n</head>"});

	output.append(::gpk::view_const_string{"\n<body style=\"width:100%;height:100%;background-color:#E0E0E0;font-family:Arial;\">"	}); // 202050
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"	});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"font-size:16px; font-weight:bold; vertical-align:top;\">"});

	static constexpr	const char							contentFileName	[]				= "./tourism.json";
	::gpk::SJSONFile										config							= {};	// json to get copyright notices from pic links.
	gpk_necall(::gpk::jsonFileRead(config, contentFileName), "Failed to load configuration file: %s.", contentFileName);
	const ::gpk::error_t									countItems						= ::gpk::jsonArraySize(*config.Reader[0]);

	//---------------------
	::gpk::view_const_string								lang;
	::gpk::find("lang", requestReceived.QueryStringKeyVals, lang);
	if(lang == ::gpk::view_const_string{"es"}) {
		output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"});
		output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
		output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;font-size:24px; font-weight:bold; vertical-align:top;\">"});
		output.append(::gpk::view_const_string{ "\n<h1>Copyright</h1>"});
		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{"\n</tr>"});
		output.append(::gpk::view_const_string{ "\n<tr style=\"height:100%;\" >"});
		output.append(::gpk::view_const_string{ "\n<td style=\"font-size:32px; font-weight:bold; text-align:left;vertical-align:top;\">"});
		//output.append(::gpk::view_const_string{ "\n<br/><h2 style=\"text-align:left;\" >Copyright</h2>"});
		output.append(::gpk::view_const_string{ "\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Las fotos y el texto informativo de lugares tur�sticos publicados en TuObelisco.com fueron tomados de Wikipedia y reproducidos sin modificaciones, junto con enlaces en cada art�culo o fotograf�a a las p�ginas de Wikipedia y sus respectivas declaraciones de derechos de autor."	});
		output.append(::gpk::view_const_string{ "\n<br/>Este sitio web, sin conexi�n contractual con Wikipedia otra que su licencia de usuario y sus propias declaraciones de derechos de autor, anima expl�citamente a sus usuarios a leer las fuentes originales debido a que la utilizaci�n de las mismas apunta a proveer al usuario con fuentes de informaci�n imparcial y detallada, separada de la publicidad y promoci�n paga provista en otras secciones de este sitio web.</p>"	});
		output.append(::gpk::view_const_string{ "\n<br/><h3 style=\"text-align:left;\" >Atribuciones de im�genes de Wikipedia:</h3>"});
		for(int32_t iPic = 0; iPic < countItems; ++iPic) {
			const ::gpk::error_t									jsonIndexCurrentItem			= ::gpk::jsonArrayValueGet(*config.Reader[0], iPic);
			::gpk::view_const_string								viewWikiImageTitle				= {};
			::gpk::view_const_string								viewWikiImageAlt				= {};
			const ::gpk::error_t									jsonIndexImageTitle				= ::gpk::jsonExpressionResolve("es.image.title"	, config.Reader, jsonIndexCurrentItem, viewWikiImageTitle	);
			const ::gpk::error_t									jsonIndexImageAlt				= ::gpk::jsonExpressionResolve("es.image.alt"	, config.Reader, jsonIndexCurrentItem, viewWikiImageAlt		);
			output.append(::gpk::view_const_string{"\n<p style=\"text-align:left;text-decoration:none;font-weight:bold;font-size:14px;\" >"});
			output.append(viewWikiImageAlt);
			output.append(::gpk::view_const_string{":"});
			::ntl::htmlTag("p", viewWikiImageTitle, "style=\"text-align:left;text-decoration:none;font-weight:normal;font-size:14px;\"", output);
			output.append(::gpk::view_const_string{"</p>"});
		}
		output.append(::gpk::view_const_string{ "\n<br/><h3 style=\"text-align:center;\" >Copyright - TuBelisco.com:</h3>"});
		output.append(::gpk::view_const_string{ "\n<br/><p  style=\"text-align:center;text-decoration:none;font-weight:normal;\" >TuObelisco.com es una creaci�n de Pablo Ariel Zorrilla Cepeda - asm128 - The Dragon <br/> pabloarielz@outlook.com <br/> Todos los derechos reservados</p>"	});
		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{"\n</tr>"});
		output.append(::gpk::view_const_string{"\n</table>"});
	}
	else {
		output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"});
		output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
		output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;font-size:24px; font-weight:bold; vertical-align:top;\">"});
		output.append(::gpk::view_const_string{ "\n<h1>Copyright</h1>"});
		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{"\n</tr>"});
		output.append(::gpk::view_const_string{ "\n<tr style=\"height:100%;\" >"});
		output.append(::gpk::view_const_string{ "\n<td style=\"font-size:32px; font-weight:bold; text-align:left;vertical-align:top;\">"});
		//output.append(::gpk::view_const_string{ "\n<br/><h2 style=\"text-align:left;\" >Copyright</h2>"});
		output.append(::gpk::view_const_string{ "\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Photos and informative text of touristic public places published at TuObelisco.com were taken from Wikipedia and is reproduced unmodified, along with links on each article or photo to the Wikipedia pages and their respective copyright notices. "	});
		output.append(::gpk::view_const_string{ "\n<br/>This website, not connected contractually with Wikipedia through means other than its user license and own copyright notices, explicitly encourages their users to read the original sources as the use of the latter aims to provide the user with impartial and detailed information sources, separate from the advertising and paid promotion provided in other sections of this website.</p>"	});
		output.append(::gpk::view_const_string{ "\n<br/><h3 style=\"text-align:left;\" >Wikipedia images attributions:</h3>"});
		for(int32_t iPic = 0; iPic < countItems; ++iPic) {
			const ::gpk::error_t									jsonIndexCurrentItem			= ::gpk::jsonArrayValueGet(*config.Reader[0], iPic);
			::gpk::view_const_string								viewWikiImageTitle				= {};
			::gpk::view_const_string								viewWikiImageAlt				= {};
			const ::gpk::error_t									jsonIndexImageTitle				= ::gpk::jsonExpressionResolve("en.image.title"	, config.Reader, jsonIndexCurrentItem, viewWikiImageTitle	);
			const ::gpk::error_t									jsonIndexImageAlt				= ::gpk::jsonExpressionResolve("en.image.alt"	, config.Reader, jsonIndexCurrentItem, viewWikiImageAlt		);
			output.append(::gpk::view_const_string{"\n<p style=\"text-align:left;text-decoration:none;font-weight:bold;font-size:14px;\" >"});
			output.append(viewWikiImageAlt);
			output.append(::gpk::view_const_string{":"});
			::ntl::htmlTag("p", viewWikiImageTitle, "style=\"text-align:left;text-decoration:none;font-weight:normal;font-size:14px;\"", output);
			output.append(::gpk::view_const_string{"</p>"});
		}
		output.append(::gpk::view_const_string{ "\n<br/><h3 style=\"text-align:center;\" >Copyright - TuBelisco.com:</h3>"});
		output.append(::gpk::view_const_string{ "\n<br/><p  style=\"text-align:center;text-decoration:none;font-weight:normal;\" >TuObelisco.com is a product by Pablo Ariel Zorrilla Cepeda - asm128 - The Dragon <br/> pabloarielz@outlook.com <br/> All rights reserved</p>"	});
		output.append(::gpk::view_const_string{"\n</td>"});
		output.append(::gpk::view_const_string{"\n</tr>"});
		output.append(::gpk::view_const_string{"\n</table>"});
	}
	//---------------------

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
