// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "GameFramework/PlayerController.h"
#include "Effect/DMSEffectorInterface.h"
#include "Attribute/DMSAttributeInterface.h"
#include "Common/DMSCommons.h"

#include "DMSPlayerController.generated.h"


/**
 * 
 * 
 * 
 * ====================================
 * 
 * 
 * 
 * 
 * 
 * 
 */

struct FDMSAttributeModifier;
class UDMSCardManagerComponent;
class UDMSCardContainerComponent;
class ADMSCardBase;
class UDMSEIManagerComponent;
class UDMSAttributeComponent;
class UDMSAttribute;

UCLASS(Blueprintable)
class PROJECTDMSGAME_API ADMSPlayerController : public APlayerController, public IDMSEffectorInterface, public IDMSAttributeInterface
{
	GENERATED_BODY()
	

protected:
	UPROPERTY()
	UObject* SelectingObject;

	UPROPERTY()
	UObject* InstigatingObject;

	UPROPERTY(BlueprintReadOnly)
	UDMSCardManagerComponent* CardManagerComponent;

	UPROPERTY(BlueprintReadOnly)
	UDMSEIManagerComponent* EffectManagerComponent;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	TMap<FName,TSubclassOf<UDMSCardContainerComponent>> CardContainerTypes;

public:
	ADMSPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	// GetDMSPS();
	
	// Return Responses
	UFUNCTION(BlueprintCallable)
	void/**/ PopupSelectorWidget(TSubclassOf<UDMSEffectElementSelectorWidget> WidgetClass);

	// Ŭ���ؼ� ���� ���� �ϴ� �뵵�� ��� ( ��Ŭ�� �� �Ҵ� ? )
	UFUNCTION(BlueprintCallable)
	void InstigateObject(UObject* Object);
	
	// Ŭ���ؼ� ���� ���� �ϴ� �뵵�� ��� ( ��Ŭ�� �� �Ҵ� ? )
	UFUNCTION(BlueprintCallable)
	void SelectObject(UObject* Object);

	// Deprecated
	UFUNCTION(BlueprintCallable)
	void PlayCardDep(ADMSCardBase* Card);

	// INTERFACE FUNCTIONS
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual void OnNotifyReceived(UDMSSequence* Seq,UObject* SourceTweak) override;
	virtual UObject* GetObject() override { return this; }
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override { return nullptr; }
	virtual UDMSAttribute* GetAttribute(const FName& AttributeName) override;
	virtual bool TryModAttribute(const FDMSAttributeModifier& Modifier) override;
// ====================== TEST ====================== //

	UPROPERTY()
	UDMSAttributeComponent* AttributeComponent;

	UFUNCTION(BlueprintCallable)
	float GetAttributeValue(const FName& AttributeName);

// ================================================== //
};
