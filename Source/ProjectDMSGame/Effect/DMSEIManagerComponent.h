// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	Effect instance management component can be attached to various actors such as cards, enemies, and more.
 *
 * =========================================
 */

#include "ProjectDMS.h"
#include "Effect/DMSEffectInstance.h"
#include "Attribute/DMSAttributeInterface.h"
#include "Components/ActorComponent.h"
#include "DMSEIManagerComponent.generated.h"

class UDMSEffectSet;

/// EI 인터랙션을 필요로 하는 오브젝트에 부착하여 사용.
/// SelfLogging System?
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTDMSGAME_API UDMSEIManagerComponent : public UActorComponent, public IDMSEffectorInterface//, public IDMSAttributeInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSEIManagerComponent();

	// Attached Effects
	UPROPERTY()
	TArray<UDMSEffectInstance*> EffectInstances;

	// Owning Effects
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UDMSEffectInstance*> OwnEffectInstances;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual UObject* GetObject() { return Cast<UObject>(GetOwner()); }
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual void OnNotifyReceived(bool iChainable, UDMSSequence* Seq, UObject* SourceTweak) override;
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override;

	//virtual UDMSAttribute* GetAttribute(const FName& AttributeName) override;
	//virtual bool TryModAttribute(const FDMSAttributeModifier& Modifier) override;
	//virtual void MakeAttribute(const FName& AttributeName, const float& DefValue =0.0f);
	void SetupOwnEffect(UDMSEffectSet* EffectSet);
};
