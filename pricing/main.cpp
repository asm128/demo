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
		const ::gpk::error_t									indexRoot					= ::gpk::jsonExpressionResolve("obelisco", programState.Config.Reader, 0, rootNode);
		::ntl::loadConfig(programState, indexRoot);
	}
	::gpk::array_pod<char_t>								fileLogo			;
	::gpk::array_pod<char_t>								fileStyle			;
	::ntl::httpPath(programState.Path.Image		, "logo_home"	, programState.Extension.Image, fileLogo);
	::ntl::httpPath(programState.Path.Style		, "blankstyle"	, "css"	, fileStyle			);

	output.append(::gpk::view_const_string{ "\n<html>"});
	output.append(::gpk::view_const_string{ "\n<head>"});
	output.append(::gpk::view_const_string{ "\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />"});
	::ntl::htmlHeaderTitle(programState.Page.Title, output);
	::ntl::htmlHeaderStyleLink	({fileStyle			.begin(), fileStyle			.size()}, output);
	output.append(::gpk::view_const_string{"\n</head>"});

	output.append(::gpk::view_const_string{"\n<body style=\"width:100%;height:100%;background-color:#E0E0E0;font-family:Arial;\">"	}); // 202050
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"	});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"font-size:24px; font-weight:bold; vertical-align:top;\">"});

	//---------------------
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;font-size:24px; font-weight:bold; vertical-align:top;\">"});

	::gpk::view_const_string								lang;
	::gpk::find("lang", requestReceived.QueryStringKeyVals, lang);
	if(lang == ::gpk::view_const_string{"es"})
		output.append(::gpk::view_const_string{ "\n<h1>Publicidad</h1>"});
	else
		output.append(::gpk::view_const_string{ "\n<h1>Advertising</h1>"});
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});
	output.append(::gpk::view_const_string{ "\n<tr style=\"height:100%;\" >"});
	output.append(::gpk::view_const_string{ "\n<td style=\"font-size:32px; font-weight:bold; text-align:center;vertical-align:top;\">"});
	if(lang == ::gpk::view_const_string{"es"})
		output.append(::gpk::view_const_string{ "<a mp-mode=\"dftl\" href=\"https://www.mercadopago.com.ar/checkout/v1/redirect?pref_id=420404344-34d30d46-7d4a-47b8-a961-65a5c8bb638f\" name=\"MP-payButton\" class='blue-ar-l-rn-none'>$1000 (ARS) - Publicación Beta <br /> 1 unidad/mes (imagen + link + domicilio/teléfono)</a>"																																																																	});
	else
		output.append(::gpk::view_const_string{ "<a mp-mode=\"dftl\" href=\"https://www.mercadopago.com.ar/checkout/v1/redirect?pref_id=420404344-34d30d46-7d4a-47b8-a961-65a5c8bb638f\" name=\"MP-payButton\" class='blue-ar-l-rn-none'>$1000 (ARS) - Beta Ad <br /> 1 unit/month (image + url + street address/phone)</a>"																																																																	});

	output.append(::gpk::view_const_string{ "<script type=\"text/javascript\">"																																																																																																																									});
	output.append(::gpk::view_const_string{ "(function(){function $MPC_load(){window.$MPC_loaded !== true && (function(){var s = document.createElement(\"script\");s.type = \"text/javascript\";s.async = true;s.src = document.location.protocol+\"//secure.mlstatic.com/mptools/render.js\";var x = document.getElementsByTagName('script')[0];x.parentNode.insertBefore(s, x);window.$MPC_loaded = true;})();}window.$MPC_loaded !== true ? (window.attachEvent ?window.attachEvent('onload', $MPC_load) : window.addEventListener('load', $MPC_load, false)) : null;})();"	});
	output.append(::gpk::view_const_string{ "</script>"																																																																																																																															});
	output.append(::gpk::view_const_string{"\n</td>"});
	output.append(::gpk::view_const_string{"\n</tr>"});
	output.append(::gpk::view_const_string{"\n</table>"});
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
