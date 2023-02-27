#include "Common/DMSCommons.h"

void UDMSDataObjectSet::Inherit(UDMSDataObjectSet* Parent,const bool& InheritAgain)
{
	if (Parent==nullptr)return;
	for (auto r : Parent->DataMap)
	{
		if (r.Value==nullptr) continue;
		if (r.Value->IsInheriting()){
			UDMSDataObject* tDS = DuplicateObject<UDMSDataObject>(r.Value,GetOuter());
			tDS->SetInheriting(InheritAgain);
			DataMap.Add(r);
		}
	}
}
