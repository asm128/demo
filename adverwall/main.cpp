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

	const ::gpk::view_const_string							txtTitle						= "- Obelisco - Un Patrimonio Incalculable de la República de la Nación Argentina -";

	output.append(::gpk::view_const_string{ "\n<html>"																																										});
	output.append(::gpk::view_const_string{ "\n<head>"																																										});
	::ntl::htmlHeaderTitle(txtTitle, output);
	output.append(::gpk::view_const_string{"\n<script type=\"text/javascript\" src=\"/obelisco/js/menu.js\"  ></script>"});
	output.append(::gpk::view_const_string{"\n<script type=\"text/javascript\" src=\"/obelisco/js/login.js\" ></script>"});
	output.append(::gpk::view_const_string{"\n<link rel=\"stylesheet\" type=\"text/css\" href=\"/obelisco/css/blankstyle.css\" />"});
	output.append(::gpk::view_const_string{"\n</head>"																																										});
	output.append(::gpk::view_const_string{"\n<body style=\"width:100%;height:100%;background-color:#E0E0E0;font-family:Arial;\">"	}); // 202050
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"	});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"																		});
	output.append(::gpk::view_const_string{ "\n<td style=\"font-size:16px; font-weight:bold; vertical-align:top;\">"									});
	//---------------------
	output.append(::gpk::view_const_string{ "\n<table style=\"width:100%;height:100%;text-align:center;\">"	});
	output.append(::gpk::view_const_string{ "\n<tr style=\"\" >"																		});
	output.append(::gpk::view_const_string{ "\n<td style=\"width:100%;font-size:32px; font-weight:bold; vertical-align:top;\">"									});
	output.append(::gpk::view_const_string{ "\n<h1>Términos y condiciones</h1>"	});
	output.append(::gpk::view_const_string{"\n</td>"			});
	output.append(::gpk::view_const_string{"\n</tr>"			});
	output.append(::gpk::view_const_string{ "\n<tr style=\"height:100%;\" >"																		});
	output.append(::gpk::view_const_string{ "\n<td style=\"font-size:32px; font-weight:bold; text-align:left;vertical-align:top;\">"									});
	output.append(::gpk::view_const_string{ "\n<br/><h3 style=\"text-align:left;\" >Punto A.</h2>"	});
	output.append(::gpk::view_const_string{ "\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris ac lacus ut lorem dictum viverra eu maximus est. Nullam auctor nisi nec odio euismod volutpat. Proin porttitor in nulla et aliquet. Nulla nec vulputate tellus, sit amet faucibus dui. Morbi pharetra imperdiet pretium. Mauris ullamcorper dictum mauris sit amet rhoncus. Nulla blandit sem non erat lacinia, in imperdiet elit iaculis. Vestibulum nec dignissim metus. Quisque accumsan urna at sapien convallis, at sollicitudin magna lobortis. Curabitur vulputate aliquet pharetra. Fusce non velit nisl. Mauris ligula nulla, ultricies at nisl sit amet, semper sollicitudin mi.																																								</p>"	});
	output.append(::gpk::view_const_string{ "\n<br/><h3 style=\"text-align:left;\" >Punto B.</h2>"	});
	output.append(::gpk::view_const_string{ "\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Nulla sed tempor nisi, eu interdum velit. Suspendisse egestas sem neque, dictum volutpat nisi molestie aliquet. Morbi eget nunc eget diam mollis luctus vel a turpis. Duis tincidunt dolor vel bibendum tincidunt. Aliquam erat volutpat. Sed sagittis justo et enim venenatis finibus. Proin nisl eros, sodales non vehicula congue, auctor in eros. Maecenas sed ante aliquam, venenatis arcu sed, aliquam ligula. Donec luctus eget justo quis suscipit. Proin dapibus, nibh et facilisis mattis, neque nisi mollis neque, ut tristique mi nibh sed elit. Etiam eget lectus id augue molestie commodo. Mauris nunc nisi, vehicula nec eleifend et, dictum sit amet libero. Donec volutpat ornare diam. Duis gravida sapien bibendum, sodales nisl eu, volutpat magna.													</p>"	});
	output.append(::gpk::view_const_string{ "\n<br/><h3 style=\"text-align:left;\" >Punto C.</h2>"	});
	output.append(::gpk::view_const_string{ "\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Sed aliquet fermentum neque. Duis ullamcorper odio vel porta hendrerit. Sed tincidunt enim vel lacus volutpat mollis. Donec euismod nunc elementum lobortis auctor. Duis faucibus at sem dapibus fermentum. Morbi vulputate ante purus, eu accumsan magna condimentum venenatis. Etiam a diam magna. Fusce vel sollicitudin sapien. Aliquam suscipit ipsum orci, in venenatis lectus consequat ut. Curabitur dictum hendrerit porta. Aliquam erat volutpat. Mauris commodo varius nisi, feugiat volutpat nisl bibendum lacinia. Donec nulla justo, tempor aliquet commodo sit amet, ultricies vitae turpis. Nullam vel lobortis quam. Praesent rhoncus ligula eget enim euismod convallis.																																</p>"	});
	output.append(::gpk::view_const_string{ "\n<br/><h3 style=\"text-align:left;\" >Punto D.</h2>"	});
	output.append(::gpk::view_const_string{ "\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >In sed semper risus, nec porttitor metus. Sed sagittis leo a nunc molestie, pulvinar pulvinar magna consequat. Aliquam tincidunt tincidunt est, vitae auctor erat commodo nec. Proin vehicula purus cursus, volutpat nulla vitae, condimentum ex. Pellentesque sit amet arcu in lectus molestie elementum vel sed nunc. Vivamus luctus tortor sed est semper cursus. Sed faucibus efficitur volutpat. Maecenas nec odio in metus pellentesque efficitur sit amet eu nisl. Morbi erat nisl, volutpat ac pulvinar nec, rutrum at dui.																																																																						</p>"	});
	output.append(::gpk::view_const_string{ "\n<br/><h3 style=\"text-align:left;\" >Punto E.</h2>"	});
	output.append(::gpk::view_const_string{ "\n<br/><p  style=\"text-align:left;text-decoration:none;font-weight:normal;\" >Integer tempor ullamcorper leo, nec lacinia ligula laoreet nec. Quisque id leo orci. Aenean scelerisque quam eget est pretium sagittis. Integer ut dolor rutrum, sollicitudin felis id, ullamcorper nibh. Duis ut diam in nibh porta euismod. Vestibulum faucibus, massa vel tincidunt dictum, dolor nisl commodo elit, vel volutpat nulla mi eu elit. Nam at cursus arcu. Sed a arcu et nibh ultricies pretium ut eu leo. Duis a dapibus eros. Etiam felis nisi, varius sed lectus at, laoreet ultricies felis. Nullam aliquam cursus tellus non gravida.																																																																</p>"	});

	output.append(::gpk::view_const_string{"\n</td>"			});
	output.append(::gpk::view_const_string{"\n</tr>"			});
	output.append(::gpk::view_const_string{"\n</table>"			});

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
