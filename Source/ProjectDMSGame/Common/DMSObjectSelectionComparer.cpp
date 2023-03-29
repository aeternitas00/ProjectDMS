#include "Common/DMSObjectSelectionComparer.h"
#include "Common/DMSCommons.h"

//UPROPERTY ���� ������ ����?

bool FDMSObjectSelectionRules::CompareObject(UObject* Caller, const TArray<UObject*>& Targets)
{ 
	bool CompareRs = true;
	// TODO :: Use CheckersConbinater. Current code is "All &"
	for (auto Comparer : Comparers)
	{
		for (auto Target : Targets)
		{
			CompareRs = CompareRs && Comparer->Compare(Caller, Target);
		}
	}
	return CompareRs;
}
