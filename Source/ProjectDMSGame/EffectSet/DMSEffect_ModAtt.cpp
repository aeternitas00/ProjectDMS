// Copyright JeongWoo Lee

#include "EffectSet/DMSEffect_ModAtt.h"
#include "Attribute/DMSAttributeComponent.h"
#include "Attribute/DMSAttributeValue_Numeric.h"
#include "Effect/DMSEffectInstance.h"
#include "Sequence/DMSSequence.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_ModAttribute, "Effect.ModAttribute");
UE_DEFINE_GAMEPLAY_TAG(TAG_DMS_Effect_ModAttribute_Revert, "Effect.ModAttribute.Revert");


// 자손임을 표현하기 위해 파생 키워드들은 + ".~~" 하는 형태? ex) ModifyAttribute.Deal 
// ( 일종의 포함 관계에 속하는 이펙트들의 구분 위함. --> HP가 변화했을 때 > { HP 피해를 입었을 때 , HP 회복을 했을 때 } )

UDMSEffect_ModAtt::UDMSEffect_ModAtt() //:bCreateIfNull(false)
{ 
	EffectTag = TAG_DMS_Effect_ModAttribute;
	
}

bool UDMSEffect_ModAtt::GetTargetAttComp(ADMSActiveEffect* iEI, AActor*& OutTarget, UDMSAttributeComponent*& OutComp)
{
	UObject* Object = iEI->GetApplyTargetInterface()->GetObject();

	if (Object == nullptr)
		return false;
		
	OutTarget = Cast<AActor>(Object);
	if (OutTarget == nullptr)
		return false;

	OutComp = Cast<UDMSAttributeComponent>(OutTarget->GetComponentByClass(UDMSAttributeComponent::StaticClass()));
	return OutComp!=nullptr;
}

void UDMSEffect_ModAtt::Work_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI, const FOnExecuteCompleted& OnWorkCompleted)
{
	//DMS_LOG_SIMPLE(TEXT("%s : ModAtt"), *iEI->GetOuter()->GetName());	

	AActor* Outer=nullptr;
	UDMSAttributeComponent* AttComp=nullptr;

	FDMSAttributeModifier Modifier;
	if (!GenerateModifier(iEI, SourceSequence, Modifier)) {
		OnWorkCompleted.ExecuteIfBound(false); return;
	}

	if (!GetTargetAttComp(iEI, Outer,AttComp))
	{	
		if(!bCreateIfNull)	{
			OnWorkCompleted.ExecuteIfBound(false); return;
		}
		AttComp = Cast<UDMSAttributeComponent>(Outer->AddComponentByClass(UDMSAttributeComponent::StaticClass(),false,FTransform(),false));
	}
	
	if (bCreateIfNull)	AttComp->MakeAttribute(TargetAttributeTags,Modifier.Value->GetClass());
	
	UDMSAttribute* TargetAtt = AttComp->GetAttribute(TargetAttributeTags);

	// 일반적인 로직으로 구현?? // ED_Revertable?
	if (bTemporal){
		TArray<FDMSAttributeModifier> RevertMods;
		TargetAtt->GetDeltasAfterModify(Modifier,iEI,RevertMods);

		iEI->OnDetach.AddLambda([=,CaptureMods = RevertMods]()__declspec(noinline){
			auto LocalMods = CaptureMods;
			for (auto& RevertMod : LocalMods)
				TargetAtt->ApplyModifier(RevertMod);
		});
	}
	TargetAtt->ApplyModifier(Modifier);

	OnWorkCompleted.ExecuteIfBound(true);
}

bool UDMSEffect_ModAtt::Predict_Implementation(ADMSSequence* SourceSequence, ADMSActiveEffect* iEI)
{
	AActor* Outer = nullptr;
	UDMSAttributeComponent* AttComp = nullptr;
	UDMSAttribute* Att = nullptr;
	FDMSAttributeModifier Modifier;

	bool rv=false;
	if (!GenerateModifier(iEI, SourceSequence, Modifier))	return false;

	if (!GetTargetAttComp(iEI, Outer, AttComp))
	{
		if (Outer != nullptr && bCreateIfNull ) 
			AttComp = Cast<UDMSAttributeComponent>(Outer->AddComponentByClass(UDMSAttributeComponent::StaticClass(), false, FTransform(), false));

		else return false;
	}
	Att = AttComp->GetAttribute(TargetAttributeTags);
	if (Att == nullptr) {
		if (bCreateIfNull) Att = AttComp->MakeAttribute(TargetAttributeTags,Modifier.Value->GetClass()); 
		else return false;
	}

	rv = Modifier.ModifierOp->Predict(Att,Modifier.Value);

	return rv;
}

FGameplayTagContainer UDMSEffect_ModAtt::GetEffectTags_Implementation()
{ 
	FGameplayTagContainer Rv(EffectTag);
	Rv.AppendTags(TargetAttributeTags);
	return Rv;
}


bool UDMSEffect_ModAtt_FromAttribute::GenerateModifier_Implementation(ADMSActiveEffect* EI, ADMSSequence* SourceSequence, FDMSAttributeModifier& OutModifier)
{
	//UDMSDataObject* ValueData = nullptr;
	if (!ModifierOp) return false;

	UDMSAttributeComponent* AttComp = nullptr;
	
	switch (ValueAttributeSource) {
		case EDMSAttributeSourceFlag::AE: 
			AttComp = EI->GetComponentByClass<UDMSAttributeComponent>(); break;
		case EDMSAttributeSourceFlag::SourcePlayer: 
			AttComp = SourceSequence->GetSourcePlayer()->GetComponentByClass<UDMSAttributeComponent>(); break;
		case EDMSAttributeSourceFlag::SourceObject: 
			AttComp = SourceSequence->GetSourceObject()->GetComponentByClass<UDMSAttributeComponent>(); break;
		case EDMSAttributeSourceFlag::TG:
			 for(auto& Owner : ValueAttributeOwner->GetTargets(EI,SourceSequence))
			 {
				 AttComp = Owner->IsA<AActor>() ? Cast<AActor>(Owner)->GetComponentByClass<UDMSAttributeComponent>() : nullptr;
				 break;
			 }
			break;
		default: break;
	}
	if(AttComp == nullptr) return false;

	auto Att = AttComp->GetAttribute(ValueAttributeTags);

	if (Att == nullptr) return false;

	UDMSAttributeValue* TempValue = DuplicateObject<UDMSAttributeValue>(Att->AttributeValue,EI);

	for (auto& Prc : ValueProcessers)
		Prc->Process(TempValue);	
	
	OutModifier.ModifierOp = ModifierOp;
	OutModifier.Value = TempValue;
	return true;
}

void UDMSAttributeValueProcesser::Process_Implementation(UObject* iObject)
{
	if (iObject==nullptr || !iObject->IsA<UDMSAttributeValue>()) return;

	auto AttValue = Cast<UDMSAttributeValue>(iObject);

	AttValue->ExecuteModifier(ProcessorModifier);
	
}

//bool UDMSAttributeModifierDefinition::ApplyModifierDefinition(UDMSAttributeValue* TargetAttribute)
//{
//	return false;
//}

bool UDMSAttributeModifierDefinition::GenerateModifier(ADMSActiveEffect* EI, ADMSSequence* SourceSequence, UPARAM(Ref)FDMSAttributeModifier& OutModifier)
{
	if( !GenerateRawModifier(EI,SourceSequence,OutModifier)) return false;

	FDMSAttributeModifier CoefficientModifier;
	if ( !Coefficient->GenerateModifier(EI,SourceSequence,CoefficientModifier) ) return false;
	OutModifier.Value->ExecuteModifier(CoefficientModifier);
	return true;
}

bool UDMSAttributeModifierDefinition_Static::GenerateRawModifier_Implementation(ADMSActiveEffect* EI, ADMSSequence* SourceSequence, FDMSAttributeModifier& OutModifier)
{
	OutModifier.ModifierOp = ModifierOp;
	OutModifier.Value = DuplicateObject(Value,EI);
	return true;
}

bool UDMSAttributeModifierDefinition_Attribute::GenerateRawModifier_Implementation(ADMSActiveEffect* EI, ADMSSequence* SourceSequence, FDMSAttributeModifier& OutModifier)
{

	auto AttOwners = ValueAttributeOwner->GetTargets(EI,SourceSequence);
	if(AttOwners.Num()==0 || !AttOwners[0]->IsA<AActor>()) return false;

	auto SourceAttribute = Cast<AActor>(AttOwners[0])->GetComponentByClass<UDMSAttributeComponent>()->GetAttribute(ValueAttributeTags);

	if(!SourceAttribute) return false;

	OutModifier.ModifierOp = ModifierOp;
	OutModifier.Value = DuplicateObject(SourceAttribute->AttributeValue,EI);

	return true;
}
