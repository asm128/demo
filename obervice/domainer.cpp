#include "domainer.h"
#include "gpk_find.h"
#include "gpk_stdstring.h"


::gpk::error_t									obe::SDomainer::AddSMTPMap	(const ::gpk::view_const_char & textToAdd)		{
	::gpk::view_const_string							copy;
	gpk_necall(Allocator.View(textToAdd.begin(), textToAdd.size(), copy), "%s", "Out of memory?");
	const ::gpk::error_t								arrobaPos					= ::gpk::find('@', copy);
	::gpk::error_t										indexToReturn				= -1;
	if(-1 == arrobaPos) {
		gpk_necall(indexToReturn = SMTPMap.Username	.push_back(copy), "%s", "Out of memory?");
		gpk_necall(indexToReturn = SMTPMap.Domain	.push_back(-1)	, "%s", "Out of memory?");
	}
	else {
		gpk_necall(indexToReturn = SMTPMap.Username	.push_back({copy.begin(), (uint32_t)arrobaPos}), "%s", "Out of memory?");;
		const uint32_t									offsetDomain				= arrobaPos + 1;
		const ::gpk::error_t							iDomain						= Domains.AddText({copy.begin() + offsetDomain, copy.size() - offsetDomain});
		gpk_necall(indexToReturn = SMTPMap.Domain	.push_back(iDomain), "%s", "Out of memory?");;
	}
	return indexToReturn;
}

::gpk::error_t									obe::SDomainer::AddURLMap	(const ::gpk::view_const_char & textToAdd)		{
	::gpk::view_const_string							copy;
	gpk_necall(Allocator.View(textToAdd.begin(), textToAdd.size(), copy), "%s", "Out of memory?");

	::gpk::error_t										indexToReturn				= -1;
	// Read scheme
	const ::gpk::error_t								colonpos					= ::gpk::find(':', copy);
	// Read either authority or path.
	uint32_t											nextOffset					= 0;
	if(-1 == colonpos)
		gpk_necall(indexToReturn = URLMap.Scheme.push_back(::obe::URL_SCHEME_default), "%s", "Out of memory?");
	else {
		nextOffset										= (colonpos + 1);
		::gpk::array_pod<char_t>							scheme						= ::gpk::view_const_char{copy.begin(), (uint32_t)colonpos};
		::gpk::tolower(scheme);
		gpk_necall(indexToReturn = URLMap.Scheme.push_back(::gpk::get_value<::obe::URL_SCHEME>(::gpk::view_const_string{scheme.begin(), scheme.size()})), "%s", "Out of memory?");
	}
	::gpk::view_const_char								remainder					= ::gpk::view_const_char{copy.begin() + nextOffset, copy.size() - nextOffset};
	if(remainder.size() <= 2) {
		gpk_necall(URLMap.Path		.push_back(Paths.AddText(remainder)), "%s", "Out of memory?");
		gpk_necall(URLMap.Authority	.push_back({})						, "%s", "Out of memory?");
		gpk_necall(URLMap.Query		.push_back({})						, "%s", "Out of memory?");
		gpk_necall(URLMap.Fragment	.push_back({})						, "%s", "Out of memory?");
		return indexToReturn;
	}
	if(remainder[0] == '/' && remainder[1] == '/') { // Read optional authority component
		const ::gpk::error_t							offsetPath					= ::gpk::find('/', remainder);
		const ::gpk::error_t							iAuthority					= (-1 == offsetPath)
			? Domains.AddText(remainder)
			: Domains.AddText({remainder.begin(), remainder.size() - offsetPath})
			;
		nextOffset									= offsetPath + 1;
		remainder									= {remainder.begin() + nextOffset, remainder.size() - nextOffset};
		URLMap.Authority	.push_back(iAuthority);
	}
	else {		///-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/
		URLMap.Authority	.push_back({});
	}
	URLMap.Path			.push_back({});
	URLMap.Query		.push_back({});
	URLMap.Fragment		.push_back({});
	return indexToReturn;
}
