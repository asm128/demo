#include "gpk_array.h"
#include "gpk_tcpip.h"

#ifndef SESSION_H_029387928374
#define SESSION_H_029387928374

namespace obe
{
	struct SSessions		{
		::gpk::array_pod<int64_t>							User		= {};
		::gpk::array_pod<int64_t>							IP			= {};
		::gpk::array_obj<::gpk::view_const_string>			Cookie		= {};
		::gpk::array_obj<::gpk::view_const_string>			KeyR		= {};
		::gpk::array_obj<::gpk::view_const_string>			KeyL		= {};
		::gpk::array_obj<::gpk::view_const_string>			KeyS		= {};
	};

#pragma pack(push, 1)
	struct SUserStatus		{
		uint8_t												Deleted		: 1;
		uint8_t												Disabled	: 1;
		uint8_t												Verified	: 1;
	};
#pragma pack(pop)

	struct SUsers			{
		::gpk::array_obj<int64_t>							Mail		= {};
		::gpk::array_obj<int64_t>							People		= {};
		::gpk::array_obj<::gpk::array_pod<int32_t>>			Categories	= {};
		::gpk::array_obj<::gpk::array_pod<int32_t>>			Groups		= {};
		::gpk::array_obj<::gpk::view_const_string>			Pass		= {};
		::gpk::array_obj<::obe::SUserStatus>				Status		= {};
		::gpk::array_obj<int64_t>							Contacts	= {};
	};

	struct SIPs				{
		::gpk::array_pod<::gpk::SIPv4				>		IPv4		= {};
		::gpk::array_obj<::gpk::view_const_string	>		IPv6		= {};
		::gpk::array_pod<uint8_t					>		Isv6		= {};
	};

	struct SHit				{
		uint64_t											Session		= 0;
		uint64_t											IP			= 0;
		uint64_t											TimeStamp	= 0;
	};

	struct SHits			{
		::gpk::array_obj<::gpk::array_pod<::obe::SHit>>		Phones		= {};
		::gpk::array_obj<::gpk::array_pod<::obe::SHit>>		Addresses	= {};
		::gpk::array_obj<::gpk::array_pod<::obe::SHit>>		Images		= {};
		::gpk::array_obj<::gpk::array_pod<::obe::SHit>>		Websites	= {};
		::gpk::array_obj<::gpk::array_pod<::obe::SHit>>		Mails		= {};
		::gpk::array_obj<::gpk::array_pod<::obe::SHit>>		Hours		= {};
	};
} // namespace

#endif // SESSION_H_029387928374
