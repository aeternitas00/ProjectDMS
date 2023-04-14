// Fill out your copyright notice in the Description page of Project Settings.


#include "Card/DMSCardData.h"
#include "Card/DMSCardDefinition.h"
#include "Card/DMSCardBase.h"
#include "Effect/DMSEffectInstance.h"
#include "System/DMSAssetManager.h"


// Sets default values
void FDMSCardData::LoadCardDefinition()
{
	CardDefinition=UDMSAssetManager::Get().ForceLoadCard(CardAssetID);
}

FArchive& operator<<(FArchive& Ar, FDMSCardData& Data)
{
	DMS_LOG_SCREEN(TEXT("FDMSCardData Serialized"));
	Ar << Data.CardAssetID;
	if (Ar.IsSaving())
	{
		int EINum = Data.AttachedEffect.Num();
		Ar << EINum;
	
		for (auto EI : Data.AttachedEffect)
		{
			EI->Serialize(Ar);
		}
	}
	else if (Ar.IsLoading())
	{
		int EINum = 0;
		Ar << EINum;
	
		for (int i=0;i<EINum;i++){
			UDMSEffectInstance* EI = NewObject<UDMSEffectInstance>();
			EI->Serialize(Ar);
	
			Data.AttachedEffect.Add(EI);
		}
	}
	return Ar;
}