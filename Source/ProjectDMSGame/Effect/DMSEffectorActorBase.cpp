// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffectorActorBase.h"
#include "Effect/DMSEIManagerComponent.h"

// Sets default values
ADMSEffectorActorBase::ADMSEffectorActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	EffectManagerComponent = CreateDefaultSubobject<UDMSEIManagerComponent>("EffectManagerComponent");
}



void ADMSEffectorActorBase::AttachEffectInstance(UDMSEffectInstance* EI)
{
	EffectManagerComponent->AttachEffectInstance(EI);
}

bool ADMSEffectorActorBase::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweaker)
{
	//DMS_LOG_SCREEN(TEXT("%s : OnNotifyReceived"), *this->GetName());
	return EffectManagerComponent->OnNotifyReceived(ResponsedObjects, iChainable, Seq, this);
}

UDMSEffectSet* ADMSEffectorActorBase::GetOwningEffectSet(const FName& iSetName)
{
	return nullptr;
}
