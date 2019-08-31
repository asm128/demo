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
	//	output.append_string("<a mp-mode=\"dftl\" href=\"https://www.mercadopago.com.ar/checkout/v1/redirect?pref_id=420404344-34d30d46-7d4a-47b8-a961-65a5c8bb638f\" name=\"MP-payButton\" class='blue-ar-l-rn-none'>$1000 (ARS) - Publicaci�n Beta <br /> 1 unidad/mes (imagen + link + domicilio/tel�fono)</a>");
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
		output.append_string("\n<h1>T�rminos y condiciones</h1>");
		output.append_string("\n</td>");
		output.append_string("\n</tr>");
		output.append_string("\n<tr style=\"height:100%;\" >");
		output.append_string("\n<td style=\"font-size:32px; font-weight:bold; text-align:left;vertical-align:top;\">");

		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Autorizaci�n</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >La utilizaci�n de este sitio web es opcional y se considera que, durante su acceso al mismo, el usuario declara estar de acuerdo con los t�rminos y condiciones declarados a cotinuaci�n.</p>");
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Descripci�n del Servicio</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com es un sitio web que ofrece publicidad e informaci�n de inter�s tur�stico y comercial.</p>");
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Hiperv�nculos y Publicaciones</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com provee de hiperv�nculos a sitios de terceros, los cuales son verificados al momento de su publicaci�n con el objetivo de evitar publicaciones de negocios que operen fuera del marco de la ley vigente. Sin embargo, TuObelisco.com no puede tomar responsabilidad por destinos web modificados luego de la publicaci�n en TuObelisco.com, sea una modificaci�n intencional, accidental o malintencionada debido a intrusiones inform�ticas no autorizadas."
			"De esta manera, TuObelisco.com no participa en, no representa ni aprueba la precisi�n, legalidad, legitimaci�n, validez y confianza de los productos y servicios, sugerencias, y recomendaciones de los comercios de terceros."
			"TuObelisco.com tampoco garantiza, ni se hace responsable, de la precisi�n de la informaci�n presentada incluyendo la moneda, la calidad, el cumplimiento de la propiedad intelectual o la legalidad de la misma.</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Propiedad Intelectual</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com se reserva todos los derechos de Propiedad intelectual y dem�s derechos sobre el servicio as� como sobre los contenidos publicados, informaciones, im�genes, videos y bancos de datos. Por ello, est� expresamente prohibida cualquier modificaci�n, reproducci�n, publicaci�n, transmisi�n a terceros y/o dem�s uso o explotaci�n de la propiedad protegida sin el consentimiento previo por escrito de TuObelisco.com"
			"TuObelisco.com respeta la propiedad intelectual de otros. Si usted sabe o sospecha que cualquier uso del Servicio constituye un incumplimiento de la propiedad intelectual, por favor notifique al departamento de soporte de TuObelisco.com v�a e-mail a pabloarielz@outlook.com"
			"</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Privacidad</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Al continuar utilizando los servicios de TuObelisco.com, el usuario da consentimiento a recolectar y utilizar informaci�n conforme a lo establecido en nuestra Pol�tica de Privacidad.</p>");

		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Usos no permitidos</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >No est� permitida ninguna acci�n o uso de dispositivo, software, u otro medio tendiente a interferir tanto en las actividades y operatoria de TuObelisco.com como en las bases de datos de TuObelisco.com. Cualquier intromisi�n, tentativa o actividad violatoria o contraria a las leyes sobre derecho de propiedad intelectual y/o a las prohibiciones estipuladas en este contrato har�n pasible a su responsable de las acciones legales pertinentes, y a las sanciones previstas por este acuerdo, as� como lo har� responsable de indemnizar los da�os ocasionados."
			"No est� permitido desencriptar o hacer ingenier�a inversa del Sitio Web, o romper, alterar o interferir con el Servicio. Todo Usuario ser� legalmente responsable de las siguientes conductas: a) Uso incorrecto del sitio o de la informaci�n que contenga; b) Uso con fines o efectos il�citos del sitio, sus servicios o de la informaci�n que contenga; c) Causar inutilizaci�n, demora, deterioro o da�o al sitio, sus servicios o a la informaci�n que contenga; d) Reproducci�n total o parcial, comercializaci�n, distribuci�n, publicaci�n o modificaci�n no autorizada de informaci�n.</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Exclusi�n de responsabilidad</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com no es responsable por da�os y/o perjuicios directos o indirectos causados por cualquier adici�n o alteraci�n efectuada al momento de tomar el servicio ofrecido por el comercio seleccionado."
			"Es obligaci�n de los usuarios contar con antivirus adecuados, antispyware y en general con todos los mecanismos necesarios para proteger los datos e informaci�n computacional. TuObelisco.com no ser� responsable por la seguridad t�cnica, calidad y funcionamiento de los antivirus/antispyware y fiabilidad de las comunicaciones efectuadas a trav�s de aparatos, redes, terminales o equipos."
			"TuObelisco.com procura que la informaci�n contenida en su sitio web sea completa, exacta y actualizada. Sin embargo, no est� en condiciones de garantizar dicha integridad, exactitud o vigencia de toda la informaci�n ofrecida ya que es con fines no lucrativos para el Usuario."
			"Por ello, no garantiza un servicio libre de errores e ininterrumpido y no ser� responsable bajo ninguna circunstancia por eventuales da�os y/o perjuicios directos o indirectos que puedan derivarse de su uso, por errores o inexactitudes de la informaci�n contenida en �l, ni respecto de interrupciones, demoras, virus o fallas del sistema."
			"</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Ley Aplicable</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Estos t�rminos y condiciones se otorgan en la Ciudad Aut�noma de Buenos Aires y se gobiernan por las leyes de la Rep�blica Argentina."
			"Cualquier controversia derivada del presente acuerdo, su existencia, validez, interpretaci�n, alcance o cumplimiento, ser� sometida a los Tribunales Ordinarios en lo Comercial con asiento en la Ciudad Aut�noma de Buenos Aires.</p>"
			);
	}
	else {
		output.append_string("\n<h1>Terms and conditions</h1>");
		output.append_string("\n</td>");
		output.append_string("\n</tr>");
		output.append_string("\n<tr style=\"height:100%;\" >");
		output.append_string("\n<td style=\"font-size:32px; font-weight:bold; text-align:left;vertical-align:top;\">");

		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Autorizaci�n</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >La utilizaci�n de este sitio web es opcional y se considera que, durante su acceso al mismo, el usuario declara estar de acuerdo con los t�rminos y condiciones declarados a cotinuaci�n.</p>");
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Descripci�n del Servicio</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com es un sitio web que ofrece publicidad e informaci�n de inter�s tur�stico y comercial.</p>");
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Hiperv�nculos y Publicaciones</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com provee de hiperv�nculos a sitios de terceros, los cuales son verificados al momento de su publicaci�n con el objetivo de evitar publicaciones de negocios que operen fuera del marco de la ley vigente. Sin embargo, TuObelisco.com no puede tomar responsabilidad por destinos web modificados luego de la publicaci�n en TuObelisco.com, sea una modificaci�n intencional, accidental o malintencionada debido a intrusiones inform�ticas no autorizadas."
			"De esta manera, TuObelisco.com no participa en, no representa ni aprueba la precisi�n, legalidad, legitimaci�n, validez y confianza de los productos y servicios, sugerencias, y recomendaciones de los comercios de terceros."
			"TuObelisco.com tampoco garantiza, ni se hace responsable, de la precisi�n de la informaci�n presentada incluyendo la moneda, la calidad, el cumplimiento de la propiedad intelectual o la legalidad de la misma.</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Propiedad Intelectual</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com se reserva todos los derechos de Propiedad intelectual y dem�s derechos sobre el servicio as� como sobre los contenidos publicados, informaciones, im�genes, videos y bancos de datos. Por ello, est� expresamente prohibida cualquier modificaci�n, reproducci�n, publicaci�n, transmisi�n a terceros y/o dem�s uso o explotaci�n de la propiedad protegida sin el consentimiento previo por escrito de TuObelisco.com"
			"TuObelisco.com respeta la propiedad intelectual de otros. Si usted sabe o sospecha que cualquier uso del Servicio constituye un incumplimiento de la propiedad intelectual, por favor notifique al departamento de soporte de TuObelisco.com v�a e-mail a pabloarielz@outlook.com"
			"</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Privacidad</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Al continuar utilizando los servicios de TuObelisco.com, el usuario da consentimiento a recolectar y utilizar informaci�n conforme a lo establecido en nuestra Pol�tica de Privacidad.</p>");
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Usos no permitidos</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >No est� permitida ninguna acci�n o uso de dispositivo, software, u otro medio tendiente a interferir tanto en las actividades y operatoria de TuObelisco.com como en las bases de datos de TuObelisco.com. Cualquier intromisi�n, tentativa o actividad violatoria o contraria a las leyes sobre derecho de propiedad intelectual y/o a las prohibiciones estipuladas en este contrato har�n pasible a su responsable de las acciones legales pertinentes, y a las sanciones previstas por este acuerdo, as� como lo har� responsable de indemnizar los da�os ocasionados."
			"No est� permitido desencriptar o hacer ingenier�a inversa del Sitio Web, o romper, alterar o interferir con el Servicio. Todo Usuario ser� legalmente responsable de las siguientes conductas: a) Uso incorrecto del sitio o de la informaci�n que contenga; b) Uso con fines o efectos il�citos del sitio, sus servicios o de la informaci�n que contenga; c) Causar inutilizaci�n, demora, deterioro o da�o al sitio, sus servicios o a la informaci�n que contenga; d) Reproducci�n total o parcial, comercializaci�n, distribuci�n, publicaci�n o modificaci�n no autorizada de informaci�n.</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Exclusi�n de responsabilidad</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >TuObelisco.com no es responsable por da�os y/o perjuicios directos o indirectos causados por cualquier adici�n o alteraci�n efectuada al momento de tomar el servicio ofrecido por el comercio seleccionado."
			"Es obligaci�n de los usuarios contar con antivirus adecuados, antispyware y en general con todos los mecanismos necesarios para proteger los datos e informaci�n computacional. TuObelisco.com no ser� responsable por la seguridad t�cnica, calidad y funcionamiento de los antivirus/antispyware y fiabilidad de las comunicaciones efectuadas a trav�s de aparatos, redes, terminales o equipos."
			"TuObelisco.com procura que la informaci�n contenida en su sitio web sea completa, exacta y actualizada. Sin embargo, no est� en condiciones de garantizar dicha integridad, exactitud o vigencia de toda la informaci�n ofrecida ya que es con fines no lucrativos para el Usuario."
			"Por ello, no garantiza un servicio libre de errores e ininterrumpido y no ser� responsable bajo ninguna circunstancia por eventuales da�os y/o perjuicios directos o indirectos que puedan derivarse de su uso, por errores o inexactitudes de la informaci�n contenida en �l, ni respecto de interrupciones, demoras, virus o fallas del sistema."
			"</p>"
			);
		output.append_string("\n<br/><h3 style=\"text-align:left;\" >Ley Aplicable</h3>");
		output.append_string("\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Estos t�rminos y condiciones se otorgan en la Ciudad Aut�noma de Buenos Aires y se gobiernan por las leyes de la Rep�blica Argentina."
			"Cualquier controversia derivada del presente acuerdo, su existencia, validez, interpretaci�n, alcance o cumplimiento, ser� sometida a los Tribunales Ordinarios en lo Comercial con asiento en la Ciudad Aut�noma de Buenos Aires.</p>"
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
