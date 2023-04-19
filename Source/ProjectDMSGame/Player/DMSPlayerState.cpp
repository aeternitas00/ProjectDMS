// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DMSPlayerState.h"
#include "Library/DMSCoreFunctionLibrary.h"

void ADMSPlayerState::SetCardDatas(const TArray<FDMSCardData>& InDatas)
{
	OriginalCardDatas= InDatas;

	for (auto& Data : OriginalCardDatas) 
	{
		Data.LoadCardDefinition(); 
		UDMSCoreFunctionLibrary::SetDataOwner(Data,this);
	}
}
