#include "neutralizer.h"

#include "gpk_cgi_app_impl_v2.h"

#include "gpk_find.h"
#include "gpk_process.h"
#include "gpk_json_expression.h"

GPK_CGI_JSON_APP_IMPL();

::gpk::error_t										gpk_cgi_generate_output			(::gpk::SCGIRuntimeValues & runtimeValues, ::gpk::array_pod<char_t> & output)	{
	::gpk::SHTTPAPIRequest									requestReceived					= {};
	bool													isCGIEnviron					= ::gpk::httpRequestInit(requestReceived, runtimeValues, true);
	if (isCGIEnviron) {
		gpk_necall(output.append_string("Content-type: text/html\r\nCache-control: no-cache\r\n"), "%s", "Out of memory?");
		gpk_necall(output.append_string("\r\n"), "%s", "Out of memory?");
		::gpk::view_const_string							methodsValid	[]				=
			{ "GET"
			, "POST"
			};
		if(-1 == ::gpk::keyValVerify(runtimeValues.EnvironViews, "REQUEST_METHOD", methodsValid)) {
			output.append_string("{ \"status\" : 403, \"description\" :\"forbidden\" }\r\n");
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
	::ntl::httpPath(programState.Path.Image, "logo_home"	, programState.Extension.Image, fileLogo);
	::gpk::array_pod<char_t>								fileStyle			;
	::ntl::httpPath(programState.Path.Style, "blankstyle"	, "css"	, fileStyle			);

	output.append_string("\n<html>");
	output.append_string("\n<head>");
	output.append_string("\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />");
	::ntl::htmlHeaderTitle(programState.Page.Title, output);
	::ntl::htmlHeaderStyleLink({fileStyle.begin(), fileStyle.size()}, output);
	output.append_string("\n</head>");

	output.append_string("\n<body style=\"width:100%;height:100%;background-color:#E0E0E0;font-family:Arial;\">"); // 202050
	output.append_string("\n<table style=\"width:100%;height:100%;text-align:center;\">");
	output.append_string("\n<tr style=\"\" >");
	output.append_string("\n<td style=\"font-size:24px; font-weight:bold; vertical-align:top;\">");

	//---------------------
	output.append_string("\n<table style=\"width:100%;height:100%;text-align:center;\">");
	output.append_string("\n<tr style=\"\" >");
	output.append_string("\n<td style=\"width:100%;font-size:24px; font-weight:bold; vertical-align:top;\">");

	::gpk::view_const_string								lang;
	::gpk::find("l", requestReceived.QueryStringKeyVals, lang);
	if(lang == ::gpk::view_const_string{"es"})
		output.append_string("\n<h1>Publicidad</h1>");
	else
		output.append_string("\n<h1>Advertising</h1>");
	output.append_string("\n</td>");
	output.append_string("\n</tr>");
	output.append_string("\n<tr style=\"height:100%;\" >");
	output.append_string("\n<td style=\"font-size:32px; font-weight:bold; text-align:center;vertical-align:top;\">");
	//if(lang == ::gpk::view_const_string{"es"})
	//	output.append_string("<a mp-mode=\"dftl\" href=\"https://www.mercadopago.com.ar/checkout/v1/redirect?pref_id=420404344-34d30d46-7d4a-47b8-a961-65a5c8bb638f\" name=\"MP-payButton\" class='blue-ar-l-rn-none'>$1000 (ARS) - Publicación Beta <br /> 1 unidad/mes (imagen + link + domicilio/teléfono)</a>");
	//else
	//	output.append_string("<a mp-mode=\"dftl\" href=\"https://www.mercadopago.com.ar/checkout/v1/redirect?pref_id=420404344-34d30d46-7d4a-47b8-a961-65a5c8bb638f\" name=\"MP-payButton\" class='blue-ar-l-rn-none'>$1000 (ARS) - Beta Ad <br /> 1 unit/month (image + url + street address/phone)</a>");
	//
	//output.append_string("<script type=\"text/javascript\">");
	//output.append_string("(function(){function $MPC_load(){window.$MPC_loaded !== true && (function(){var s = document.createElement(\"script\");s.type = \"text/javascript\";s.async = true;s.src = document.location.protocol+\"//secure.mlstatic.com/mptools/render.js\";var x = document.getElementsByTagName('script')[0];x.parentNode.insertBefore(s, x);window.$MPC_loaded = true;})();}window.$MPC_loaded !== true ? (window.attachEvent ?window.attachEvent('onload', $MPC_load) : window.addEventListener('load', $MPC_load, false)) : null;})();");
	//output.append_string("</script>");
	output.append_string("\n</td>");
	output.append_string("\n</tr>");
	output.append_string("\n</table>");
	//---------------------
	output.append_string("\n</td>");
	output.append_string("\n</tr>");

	//---------------------
	output.append_string("\n<tr style=\"height:100%;\" >");
	output.append_string("\n<td style=\"font-size:32px; font-weight:bold; text-align:center;vertical-align:top;\">");
	output.append_string("\n<form method=\"POST\" action=\"pricing.exe\" enctype=\"multipart/form-data\" ><input type=\"file\" id=\"avatar\" name=\"avatar\" accept=\"image/png\" /><input type=\"submit\" /></form>");
		output.append_string("\n<table style=\"width:100%;height:100%;text-align:center;\">");
		output.append_string("\n<tr style=\"\" >");
		output.append_string("\n<td style=\"width:100%;font-size:24px; font-weight:bold; vertical-align:top;\">");
	if(lang == ::gpk::view_const_string{"es"}) {
		output.append_string("\n<h1>Términos y condiciones</h1>");
		output.append_string("\n</td>");
		output.append_string("\n</tr>");
		output.append_string("\n<tr style=\"height:100%;\" >");
		output.append_string("\n<td style=\"font-size:32px; font-weight:bold; text-align:left;vertical-align:top;\">");

		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Autorización</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >La utilización de este sitio web es opcional y se considera que, durante su acceso al mismo, el usuario declara estar de acuerdo con los términos y condiciones declarados a cotinuación.</p>");
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Descripción del Servicio</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com es un sitio web que ofrece publicidad e información de interés turístico y comercial.</p>");
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Hipervínculos y Publicaciones</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com provee de hipervínculos a sitios de terceros, los cuales son verificados al momento de su publicación con el objetivo de evitar publicaciones de negocios que operen fuera del marco de la ley vigente. Sin embargo, TuObelisco.com no puede tomar responsabilidad por destinos web modificados luego de la publicación en TuObelisco.com, sea una modificación intencional, accidental o malintencionada debido a intrusiones informáticas no autorizadas."
			"De esta manera, TuObelisco.com no participa en, no representa ni aprueba la precisión, legalidad, legitimación, validez y confianza de los productos y servicios, sugerencias, y recomendaciones de los comercios de terceros."
			"TuObelisco.com tampoco garantiza, ni se hace responsable, de la precisión de la información presentada incluyendo la moneda, la calidad, el cumplimiento de la propiedad intelectual o la legalidad de la misma.</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Propiedad Intelectual</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com se reserva todos los derechos de Propiedad intelectual y demás derechos sobre el servicio así como sobre los contenidos publicados, informaciones, imágenes, videos y bancos de datos. Por ello, está expresamente prohibida cualquier modificación, reproducción, publicación, transmisión a terceros y/o demás uso o explotación de la propiedad protegida sin el consentimiento previo por escrito de TuObelisco.com"
			"TuObelisco.com respeta la propiedad intelectual de otros. Si usted sabe o sospecha que cualquier uso del Servicio constituye un incumplimiento de la propiedad intelectual, por favor notifique al departamento de soporte de TuObelisco.com vía e-mail a pabloarielz@outlook.com"
			"</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Privacidad</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Al continuar utilizando los servicios de TuObelisco.com, el usuario da consentimiento a recolectar y utilizar información conforme a lo establecido en nuestra Política de Privacidad.</p>");

		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Usos no permitidos</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >No está permitida ninguna acción o uso de dispositivo, software, u otro medio tendiente a interferir tanto en las actividades y operatoria de TuObelisco.com como en las bases de datos de TuObelisco.com. Cualquier intromisión, tentativa o actividad violatoria o contraria a las leyes sobre derecho de propiedad intelectual y/o a las prohibiciones estipuladas en este contrato harán pasible a su responsable de las acciones legales pertinentes, y a las sanciones previstas por este acuerdo, así como lo hará responsable de indemnizar los daños ocasionados."
			"No está permitido desencriptar o hacer ingeniería inversa del Sitio Web, o romper, alterar o interferir con el Servicio. Todo Usuario será legalmente responsable de las siguientes conductas: a) Uso incorrecto del sitio o de la información que contenga; b) Uso con fines o efectos ilícitos del sitio, sus servicios o de la información que contenga; c) Causar inutilización, demora, deterioro o daño al sitio, sus servicios o a la información que contenga; d) Reproducción total o parcial, comercialización, distribución, publicación o modificación no autorizada de información.</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Exclusión de responsabilidad</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com no es responsable por daños y/o perjuicios directos o indirectos causados por cualquier adición o alteración efectuada al momento de tomar el servicio ofrecido por el comercio seleccionado."
			"Es obligación de los usuarios contar con antivirus adecuados, antispyware y en general con todos los mecanismos necesarios para proteger los datos e información computacional. TuObelisco.com no será responsable por la seguridad técnica, calidad y funcionamiento de los antivirus/antispyware y fiabilidad de las comunicaciones efectuadas a través de aparatos, redes, terminales o equipos."
			"TuObelisco.com procura que la información contenida en su sitio web sea completa, exacta y actualizada. Sin embargo, no está en condiciones de garantizar dicha integridad, exactitud o vigencia de toda la información ofrecida ya que es con fines no lucrativos para el Usuario."
			"Por ello, no garantiza un servicio libre de errores e ininterrumpido y no será responsable bajo ninguna circunstancia por eventuales daños y/o perjuicios directos o indirectos que puedan derivarse de su uso, por errores o inexactitudes de la información contenida en él, ni respecto de interrupciones, demoras, virus o fallas del sistema."
			"</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Ley Aplicable</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Estos términos y condiciones se otorgan en la Ciudad Autónoma de Buenos Aires y se gobiernan por las leyes de la República Argentina."
			"Cualquier controversia derivada del presente acuerdo, su existencia, validez, interpretación, alcance o cumplimiento, será sometida a los Tribunales Ordinarios en lo Comercial con asiento en la Ciudad Autónoma de Buenos Aires.</p>"
			);
	}
	else {
		output.append_string("\n<h1>Terms and conditions</h1>");
		output.append_string("\n</td>");
		output.append_string("\n</tr>");
		output.append_string("\n<tr style=\"height:100%;\" >");
		output.append_string("\n<td style=\"font-size:32px; font-weight:bold; text-align:left;vertical-align:top;\">");

		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Autorización</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >La utilización de este sitio web es opcional y se considera que, durante su acceso al mismo, el usuario declara estar de acuerdo con los términos y condiciones declarados a cotinuación.</p>");
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Descripción del Servicio</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com es un sitio web que ofrece publicidad e información de interés turístico y comercial.</p>");
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Hipervínculos y Publicaciones</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com provee de hipervínculos a sitios de terceros, los cuales son verificados al momento de su publicación con el objetivo de evitar publicaciones de negocios que operen fuera del marco de la ley vigente. Sin embargo, TuObelisco.com no puede tomar responsabilidad por destinos web modificados luego de la publicación en TuObelisco.com, sea una modificación intencional, accidental o malintencionada debido a intrusiones informáticas no autorizadas."
			"De esta manera, TuObelisco.com no participa en, no representa ni aprueba la precisión, legalidad, legitimación, validez y confianza de los productos y servicios, sugerencias, y recomendaciones de los comercios de terceros."
			"TuObelisco.com tampoco garantiza, ni se hace responsable, de la precisión de la información presentada incluyendo la moneda, la calidad, el cumplimiento de la propiedad intelectual o la legalidad de la misma.</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Propiedad Intelectual</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com se reserva todos los derechos de Propiedad intelectual y demás derechos sobre el servicio así como sobre los contenidos publicados, informaciones, imágenes, videos y bancos de datos. Por ello, está expresamente prohibida cualquier modificación, reproducción, publicación, transmisión a terceros y/o demás uso o explotación de la propiedad protegida sin el consentimiento previo por escrito de TuObelisco.com"
			"TuObelisco.com respeta la propiedad intelectual de otros. Si usted sabe o sospecha que cualquier uso del Servicio constituye un incumplimiento de la propiedad intelectual, por favor notifique al departamento de soporte de TuObelisco.com vía e-mail a pabloarielz@outlook.com"
			"</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Privacidad</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Al continuar utilizando los servicios de TuObelisco.com, el usuario da consentimiento a recolectar y utilizar información conforme a lo establecido en nuestra Política de Privacidad.</p>");
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Usos no permitidos</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >No está permitida ninguna acción o uso de dispositivo, software, u otro medio tendiente a interferir tanto en las actividades y operatoria de TuObelisco.com como en las bases de datos de TuObelisco.com. Cualquier intromisión, tentativa o actividad violatoria o contraria a las leyes sobre derecho de propiedad intelectual y/o a las prohibiciones estipuladas en este contrato harán pasible a su responsable de las acciones legales pertinentes, y a las sanciones previstas por este acuerdo, así como lo hará responsable de indemnizar los daños ocasionados."
			"No está permitido desencriptar o hacer ingeniería inversa del Sitio Web, o romper, alterar o interferir con el Servicio. Todo Usuario será legalmente responsable de las siguientes conductas: a) Uso incorrecto del sitio o de la información que contenga; b) Uso con fines o efectos ilícitos del sitio, sus servicios o de la información que contenga; c) Causar inutilización, demora, deterioro o daño al sitio, sus servicios o a la información que contenga; d) Reproducción total o parcial, comercialización, distribución, publicación o modificación no autorizada de información.</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Exclusión de responsabilidad</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com no es responsable por daños y/o perjuicios directos o indirectos causados por cualquier adición o alteración efectuada al momento de tomar el servicio ofrecido por el comercio seleccionado."
			"Es obligación de los usuarios contar con antivirus adecuados, antispyware y en general con todos los mecanismos necesarios para proteger los datos e información computacional. TuObelisco.com no será responsable por la seguridad técnica, calidad y funcionamiento de los antivirus/antispyware y fiabilidad de las comunicaciones efectuadas a través de aparatos, redes, terminales o equipos."
			"TuObelisco.com procura que la información contenida en su sitio web sea completa, exacta y actualizada. Sin embargo, no está en condiciones de garantizar dicha integridad, exactitud o vigencia de toda la información ofrecida ya que es con fines no lucrativos para el Usuario."
			"Por ello, no garantiza un servicio libre de errores e ininterrumpido y no será responsable bajo ninguna circunstancia por eventuales daños y/o perjuicios directos o indirectos que puedan derivarse de su uso, por errores o inexactitudes de la información contenida en él, ni respecto de interrupciones, demoras, virus o fallas del sistema."
			"</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Ley Aplicable</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Estos términos y condiciones se otorgan en la Ciudad Autónoma de Buenos Aires y se gobiernan por las leyes de la República Argentina."
			"Cualquier controversia derivada del presente acuerdo, su existencia, validez, interpretación, alcance o cumplimiento, será sometida a los Tribunales Ordinarios en lo Comercial con asiento en la Ciudad Autónoma de Buenos Aires.</p>"
			);
	}
		output.append_string("\n</td>");
		output.append_string("\n</tr>");
		output.append_string("\n</table>");

	//---------------------

	output.append_string("\n</td>");
	output.append_string("\n</tr>");

	output.append_string("\n</table>");
	output.append_string("\n<code >");
	output.append(runtimeValues.Content.Body.begin(), runtimeValues.Content.Length);
	output.append_string("\n</code >");
	output.append_string("\n</body>");
	output.append_string("\n</html>");

	if(output.size()) {
		OutputDebugStringA(output.begin());
		OutputDebugStringA("\n");
	}
	return 0;
}
