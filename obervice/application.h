#include "gpk_udp_server.h"

#include "gpk_framework.h"
#include "gpk_gui.h"

#include <mutex>

#ifndef APPLICATION_H_2078934982734
#define APPLICATION_H_2078934982734

namespace brt // I'm gonna use a different namespace in order to test a few things about the macros.
{
	struct SProcessHandles {
		HANDLE						ChildStd_IN_Read		= INVALID_HANDLE_VALUE;
		HANDLE						ChildStd_IN_Write		= INVALID_HANDLE_VALUE;
		HANDLE						ChildStd_OUT_Read		= INVALID_HANDLE_VALUE;
		HANDLE						ChildStd_OUT_Write		= INVALID_HANDLE_VALUE;
		HANDLE						ChildStd_ERR_Read		= INVALID_HANDLE_VALUE;
		HANDLE						ChildStd_ERR_Write		= INVALID_HANDLE_VALUE;
	};

	struct SProcess {
		PROCESS_INFORMATION			ProcessInfo				= {};
		STARTUPINFOA				StartInfo				= {sizeof(STARTUPINFOA)};
	};

	struct SBackUserTable {
		::gpk::aobj<::gpk::vcs>						Name;
		::gpk::aobj<::gpk::vcs>						Pass;
		::gpk::array_pod<int32_t>										IdPeople;
	};

	struct SBackAddress {
		::gpk::aobj<::gpk::vcs>						AddressLine1;
		::gpk::aobj<::gpk::vcs>						AddressLine2;
	};

	struct SBackNames {
		::gpk::aobj<::gpk::vcs>						First;
		::gpk::aobj<::gpk::vcs>						Last;
	};

	struct SBackPeopleTable {
		::gpk::ai32										NameFirst;
		::gpk::ai32										NameLast;
		::gpk::ai32										AddressLine1;
		::gpk::ai32										AddressLine2;
	};





	struct SApplication {
		::gpk::SFramework					Framework;
		::gpk::pobj<::gpk::rtbgra8d32>		Offscreen							= {};

		::gpk::SUDPServer					Server								= {};
		::gpk::aapobj<::gpk::SUDPMessage>	ReceivedPerClient;
		::gpk::aobj<::gpk::aachar>			ClientResponses;
		::gpk::aobj<::brt::SProcess>		ClientProcesses;

		::gpk::aobj<::brt::SProcessHandles>	ClientIOHandles;
		::gpk::vcs				ProcessFileName						= "";
		::gpk::vcs				ProcessMockPath						= "";
		::gpk::vcs				ProcessParams						= "";
		SECURITY_ATTRIBUTES		DefaultSecurityForPipeHandles		= {sizeof(SECURITY_ATTRIBUTES)};

		::gpk::achar			szCmdlineApp						= "";
		::gpk::achar			szCmdlineFinal						= "";


		::gpk::cid_t			IdExit								= -1;

		::std::mutex			LockGUI;
		::std::mutex			LockRender;

								SApplication		(::gpk::SRuntimeValues& runtimeValues)	: Framework(runtimeValues, "obervice.json")		{}
	};
} // namespace

#endif // APPLICATION_H_2078934982734
