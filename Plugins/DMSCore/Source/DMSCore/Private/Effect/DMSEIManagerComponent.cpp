// Fill out your copyright notice in the Description page of Project Settings.

#include "Effect/DMSEIManagerComponent.h"

#include "GameModes/DMSGameModeBase.h"

#include "Sequence/Steps/DMSSequenceStep_Decision.h"

#include "Effect/DMSEffectHandler.h"
#include "Effect/DMSEffectDefinition.h"
#include "Effect/DMSEffect_ActivateEffect.h"

#include "Library/DMSCoreFunctionLibrary.h"
// Sets default values for this component's properties
UDMSEIManagerComponent::UDMSEIManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UDMSEIManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	//DMS_LOG_SCREEN(TEXT("%s -> %s"), *GetOuter()->GetName(), *GetOwner()->GetName());
	// ...
	
}


// Called every frame
void UDMSEIManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDMSEIManagerComponent::AttachEffectInstance(UDMSEffectInstance* EI)
{
	//FString Str = (GetOwner()->GetName() + FString(TEXT("%n"), EffectInstances.Num()));
	EffectInstances.Add(EI);
	EI->Rename(nullptr, this);
}

bool UDMSEIManagerComponent::OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable, UDMSSequence* Seq, UObject* SourceTweak)
{
	bool rv=false;
	for (auto OwnEI : OwnEffectInstances)
	{
		if (OwnEI->OnNotifyReceived(ResponsedObjects, iChainable, Seq, SourceTweak)){ 

			if (OwnEI->EffectNode->bIgnoreNotify){ DMS_LOG_SIMPLE(TEXT("%s : Ignore Notify"), *OwnEI->GetName());}
			else {rv = true;}
		}
	}
	return rv;
}

UDMSEffectSet* UDMSEIManagerComponent::GetOwningEffectSet(const FGameplayTag& iSetName)
{
	auto Owner = Cast<IDMSEffectorInterface>(GetOwner());
	return Owner!=nullptr ? Owner->GetOwningEffectSet(iSetName) : nullptr;
}

AActor* UDMSEIManagerComponent::GetOwningPlayer() 
{ 
	return GetOwner()->Implements<UDMSEffectorInterface>() ? Cast<IDMSEffectorInterface>(GetOwner())->GetOwningPlayer() : GetOwner();
}

UDMSEffectNode* UDMSEIManagerComponent::ActivatorNodeGenerator(const FGameplayTag& EffectSetName, const uint8& idx)
{
	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();

	UDMSEffectNode* Node = NewObject<UDMSEffectNode>(this);
	UDMSEffect_ActivateEffect_Static* AEffect = NewObject<UDMSEffect_ActivateEffect_Static>(Node);
	//auto DecisionStep = Cast<UDMSSequenceStep_Decision>(Node->StepRequirements.FindByPredicate([](UDMSSequenceStep* Step){return Step->IsA<UDMSSequenceStep_Decision>();}));
	//
	//FDMSDecisionDefinition NewDecision;

	//auto NewBehavior = NewObject<UDMSSelectorBehaviorDefinition_UpdateData>(DecisionStep);
	//NewBehavior->OutKey = AEffect->EffectTag;
	//NewDecision.Behavior = NewBehavior;

	//DecisionStep->DecisionDefinitions.Add(NewDecision);

	//auto NewGenerator = NewObject<UDMSSelectorRequestGenerator_AE>(DecisionStep);
	//NewGenerator
	//NewDecision.Generator = NewGenerator;
	AEffect->EffectIdx = idx;
	AEffect->UseEffectFromOuter = true;
	AEffect->EffectSetName = EffectSetName;
	// NODE INITIALIZER?
	Node->EffectDefinitions.Add(AEffect);

	return Node;
};



void UDMSEIManagerComponent::SetupOwnEffect(UDMSEffectSet* EffectSet,const FGameplayTag& SetName )
{

	DMS_LOG_SIMPLE(TEXT("%s : Setup own effects [%s]"), *GetOwner()->GetName(), *SetName.ToString());
	if (EffectSet == nullptr) { DMS_LOG_DETAIL(Display, TEXT("%s : No Default Effect"),*GetOwner()->GetName()); return; }
	auto EffectNodes = EffectSet->EffectNodes;
	auto EH = UDMSCoreFunctionLibrary::GetDMSEffectHandler();
	if (!EH) { DMS_LOG_DETAIL(Error, TEXT("No Effect Handler")); 	return; }


	uint8 idx = 0;
	for (auto& EffectWrapper : EffectNodes)
	{
		auto Effect = EffectWrapper->GetEffectNode();

		if (Effect->Conditions == nullptr)
			{DMS_LOG_SIMPLE(TEXT("NO CONDITION"));continue;}

		//UDMSEffectNode* Node = ActivatorNodeGenerator(SetName, idx++);
		//Node->Rename(nullptr, this);
		//Node->Conditions = DuplicateObject(Effect->Conditions, Node);
		//if (Node->Conditions == nullptr)
		//	DMS_LOG_SIMPLE(TEXT("CONDITION DUPLICATION FAILED"));
		//Node->bIsChainableEffect = false;
		//Node->bForced = Effect->bForced;
		//Node->bIgnoreNotify = Effect->bIgnoreNotify;
		//Node->TargetGenerator = NewObject<UDMSTargetGenerator_SourceObject>(Node,"TargetGenerator");
		//AActor* CardOwner = GetOwningPlayer();

		//auto EIs = EH->CreateEffectInstance(GetOwner(), GetOwningPlayer(), GetOwner(), Node);
		auto EIs = EH->CreateEffectInstance(GetOwner(), GetOwningPlayer(), GetOwner(), Effect);
		EIs[0]->ChangeEIState(EDMSEIState::EIS_Persistent);
		OwnEffectInstances.Add(EIs[0]);
	}
}
