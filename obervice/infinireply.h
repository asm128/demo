#include "gpk_array.h"

#include "obe_core.h"

#ifndef INFINIREPLY_H_2983749827432
#define INFINIREPLY_H_2983749827432

namespace obe
{
	struct SComment				{ int64_t Author; int64_t Text; int64_t Parent; };
	struct SQOrA				{ int64_t Author; int64_t Text; };
	struct SQuestion			{ SQOrA Question; ::gpk::array_pod<SQOrA> Answers; };
	struct SComments			{
		::gpk::array_pod<::obe::SReferral>		Referral;
		::gpk::array_pod<::obe::SComment>		Comment;
		::gpk::array_obj<::obe::SQuestion>		Question;
	};
} // namespace

#endif // INFINIREPLY_H_2983749827432
