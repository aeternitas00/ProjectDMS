// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DMSEffect_AttachPersistent.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectInstance.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectorInterface.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_AttachPersistent, "Effect.AttachPersistent");

//FGameplayTagContainer UDMSEffect_AttachPersistent::GetEffectTags_Implementation()
//{
//	return FGameplayTagContainer();
//}

UDMSEffect_AttachPersistent::UDMSEffect_AttachPersistent() {
	EffectTag = TAG_DMS_Effect_AttachPersistent;
}
void UDMSEffect_AttachPersistent::Work_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	auto Target = iEI->GetApplyTargetInterface();

	if (Target == nullptr) {
		OnWorkCompleted.ExecuteIfBound(false);
		return;
	}
	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler(this);
	check(EH);

	for (auto Node : AttachingEffects){
		EH->CreatePersistentActiveEffect(SourceSequence->GetSourceObject(), SourceSequence->GetSourcePlayer(), Target->GetObject(), Node->GetEffectNode());
	}

	OnWorkCompleted.ExecuteIfBound(true);
}
