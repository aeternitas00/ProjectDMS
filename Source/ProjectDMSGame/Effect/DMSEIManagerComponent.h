// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Effect/DMSEffectInstance.h"
#include "Attribute/DMSAttributeInterface.h"
#include "Components/ActorComponent.h"
#include "DMSEIManagerComponent.generated.h"

class UDMSEffectSet;
/// EI ���ͷ����� �ʿ�� �ϴ� ������Ʈ�� �����Ͽ� ���.
/// SelfLogging System?

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTDMSGAME_API UDMSEIManagerComponent : public UActorComponent, public IDMSEffectorInterface, public IDMSAttributeInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSEIManagerComponent();

	// Attached Effects
	UPROPERTY()
	TArray<UDMSEffectInstance*> EffectInstances;

	// Owning Effects
	UPROPERTY()
	TArray<UDMSEffectInstance*> OwnEffectInstances;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual UObject* GetObject() { return Cast<UObject>(GetOwner()); }
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual void OnNotifyReceived(UDMSSequence* Seq, UObject* SourceTweak) override;
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override;

	virtual UDMSAttribute* GetAttribute(const FName& AttributeName) override;
	virtual bool TryModAttribute(const FDMSAttributeModifier& Modifier) override;

	void SetupOwnEffect(TArray<UDMSEffectNodeWrapper*> EffectNodes);
};
