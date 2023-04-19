// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "GameFramework/Actor.h"
#include "Effect/DMSEffectorInterface.h"
#include "DMSEffectorActorBase.generated.h"

UCLASS(Abstract)
class PROJECTDMSGAME_API ADMSEffectorActorBase : public AActor, public IDMSEffectorInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADMSEffectorActorBase();

protected:
	/**
	 * Card's effector component.
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UDMSEIManagerComponent* EffectManagerComponent;

public:	
	virtual UObject* GetObject() override { return this; }
	virtual AActor* GetOwningPlayer() { return GetOwner(); }
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweak) override;
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override;

};
