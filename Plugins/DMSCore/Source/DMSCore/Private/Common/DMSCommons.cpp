#include "Common/DMSCommons.h"

void UDMSDataObjectSet::Inherit(UDMSDataObjectSet* Parent)
{
	if (Parent==nullptr)return;
	for (auto r : Parent->DataMap)
	{
		if (r.Value==nullptr) continue;
		if (r.Value->IsInheriting()){
			UDMSDataObject* tDS = NewObject<UDMSDataObject>(GetOuter());
			tDS->CopyValue(r.Value);
			tDS->SetInheriting(true);
			DataMap.Add(r.Key, tDS);
		}
	}
}

void UDMSDataObjectSet::Merge(UDMSDataObjectSet* iSet)
{
	if (iSet == nullptr)return;
	for (auto& r : iSet->DataMap)
	{
		if (r.Value == nullptr) continue;
		// Handling confliction?
		//UDMSDataObject* tDS = DuplicateObject<UDMSDataObject>(r.Value, GetOuter());
		DataMap.Add(r);
	}
}

UDMSDataObject* FDMSValueSelectionForm::Get(UDMSDataObjectSet* DataSet)
{
	if(DataSet == nullptr) return nullptr;
	UDMSDataObject* rv = DataSet->ContainData(DataKey) ? DataSet->GetData(DataKey) : Get(DataSet->ParentDataSet);

	for (auto& Prc:ValueProcessers)
		Prc->Process(rv);
	
	return rv;
}
