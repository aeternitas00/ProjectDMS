// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_GAME_MODULE
 *
 *	DMS Game's Player controller
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include "GameFramework/PlayerController.h"
#include "Effect/DMSEffectorInterface.h"
#include "Attribute/DMSAttributeInterface.h"
#include "Common/DMSCommons.h"

#include "DMSPlayerController.generated.h"


struct FDMSAttributeModifier;
class UDMSCardManagerComponent;
class UDMSCardContainerComponent;
class ADMSCardBase;
class UDMSEIManagerComponent;
class UDMSAttributeComponent;
class UDMSAttribute;

//UENUM(BlueprintType)
//enum class EDMSCardInteractionMode : uint8
//{
//	//PIM_Block UMETA(DisplayName = "Can't interaction with cards"),
//	PIM_Play UMETA(DisplayName = "Interaction with cards for play or activates"),
//	PIM_Skill UMETA(DisplayName = "Interaction with cards for skill test commits"),
//	//...
//};
/**
 * 	========================================
 *
 *	 
 *	
 *	========================================
 */
UCLASS(Blueprintable)
class PROJECTDMSGAME_API ADMSPlayerController : public APlayerController, public IDMSEffectorInterface//, public IDMSAttributeInterface
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(BlueprintReadOnly)
	UObject* SelectingObject;

	UPROPERTY(BlueprintReadOnly)
	UObject* InstigatingObject;

	UPROPERTY(BlueprintReadOnly)
	UDMSCardManagerComponent* CardManagerComponent;

	UPROPERTY(BlueprintReadOnly)
	UDMSEIManagerComponent* EffectManagerComponent;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TMap<FName,TSubclassOf<UDMSCardContainerComponent>> CardContainerTypes;

	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	//TMap<FName, float> DefaultStats;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, float> DefaultStats;

	// TEST
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly,Instanced)
	TMap<FName, UDMSEffectNode*> DefaultBasicActions;

public:
	// Is this player can interact with other game object ( cards, enemy etc... )
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInteractable;

	// Interaction method flag.
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//EDMSCardInteractionMode InteractionMode;

	ADMSPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	// Return Responses
	UFUNCTION(BlueprintCallable)
	void/**/ PopupSelectorWidget(TSubclassOf<UDMSEffectElementSelectorWidget> WidgetClass);

	// 클릭해서 정보 조사 하는 용도로 사용 ( 우클릭 에 할당 ? )
	UFUNCTION(BlueprintCallable)
	void InstigateObject(UObject* Object);
	
	// Sort of focusing 
	UFUNCTION(BlueprintCallable)
	void SelectObject(UObject* Object);

	// BP Implements, Get new one from "SelectingObject"
	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectedObject(UObject* FormerObject);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayCard(ADMSCardBase* Card);


	UFUNCTION(BlueprintCallable)
	UDMSCardContainerComponent* SearchContainer(const FName& ContainerName);

	// INTERFACE FUNCTIONS
	virtual void AttachEffectInstance(UDMSEffectInstance* EI) override;
	virtual bool OnNotifyReceived(TMultiMap<TScriptInterface<IDMSEffectorInterface>, UDMSEffectInstance*>& ResponsedObjects, bool iChainable,UDMSSequence* Seq,UObject* SourceTweak) override;
	virtual UObject* GetObject() override { return this; }
	virtual AActor* GetOwningPlayer() { return this; }
	virtual UDMSEffectSet* GetOwningEffectSet(const FName& iSetName) override { return nullptr; }

// ====================== TEST ====================== //

	UPROPERTY(BlueprintReadOnly)
	UDMSAttributeComponent* AttributeComponent;

	//UFUNCTION(BlueprintCallable)
	//float GetAttributeValue(const FGameplayTag& AttributeName);

// ================================================== //
};
