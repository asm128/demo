#include "obelib.h"

#include "gpk_cgi_app_impl_v2.h"

GPK_CGI_JSON_APP_IMPL();

::gpk::error_t										gpk_cgi_generate_output			(::gpk::SCGIRuntimeValues & runtimeValues, ::gpk::array_pod<char_t> & output)	{ return ::obe::cgi_generate_catalog(runtimeValues, output); }
