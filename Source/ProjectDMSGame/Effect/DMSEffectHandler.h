// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "UObject/NoExportTypes.h"
#include "DMSEffectHandler.generated.h"

class UDMSEffectInstance;
class UDMSSequence;
class UDMSEffectNode;
class UDMSDataObjectSet;
class IDMSEffectorInterface;
/**
 * 
 */
UCLASS(/*ClassGroup = (Effect)*/)
class PROJECTDMSGAME_API UDMSEffectHandler : public UObject // or ActorComponent to attach GM
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY()
	TArray<UDMSEffectInstance*> EIList;

public:
	// Create from setup
	TArray<UDMSEffectInstance*>/*EIHandle?*/ CreateEffectInstance(UObject* SourceObject, AActor* SourceController, UDMSEffectNode* EffectNode, UDMSDataObjectSet* iSet = nullptr);
	// Create from seq
	TArray<UDMSEffectInstance*>/*EIHandle?*/ CreateEffectInstance(UDMSSequence* Sequence, UDMSEffectNode* EffectNode);


	void Resolve(UDMSSequence* Sequence);

};

//DEPRECATED
//#define GetDMSEffectHandler() ( GetDMSGameMode() ? GetDMSGameMode()->GetEffectHandler() : nullptr )