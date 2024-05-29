// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "Components/ActorComponent.h"
#include "Common/DMSCommons.h"
#include "DMSSelectorManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectorCompletedDynamic, TArray<uint8>, SelectedIndex);
DECLARE_DELEGATE_OneParam(FOnSelectorCompleted, TArray<uint8>);
DECLARE_DELEGATE_TwoParams(FOnSelectCompletedNative, bool, TArray<uint8>);

class UDMSSelectorBase;
class UDMSAttributeValue;

/**
 * Data class used to request selector creation.
 */
UCLASS(BlueprintType,Blueprintable)
class DMSCORE_API UDMSSelectorRequestForm : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * The number of selections the selector to be created will choose.
	 */
	UPROPERTY(BlueprintReadWrite)
	uint8 SelectAmount;

	/**
	 * Class of widget to be generated
	 */
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UDMSSelectorBase> SelectorClass;

	/** 
	 * Delegate to be executed when the generated selector is completed in any way.
	 */
	UPROPERTY(BlueprintReadOnly)
	FOnSelectorCompletedDynamic OnCompleted;
	FOnSelectorCompleted OnCompletedNative;
};


/**
 * 
 */
UCLASS(ClassGroup = (Selector))
class DMSCORE_API UDMSSelectorHandle : public UObject
{
	GENERATED_BODY()

public:
	UDMSSelectorHandle();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSSelectorRequestForm> StoredForm;

	TObjectPtr<UDMSSelectorBase> Widget;

	bool bAutoExecuteForm;
	//TObjectPtr<UDMSWidgetQueue> OwnerQueue;

public:
	UFUNCTION(BlueprintCallable)
	void RunSelector();

	UFUNCTION(BlueprintCallable)
	void HideSelector();

	// For Manual Setup
	UFUNCTION(BlueprintCallable)
	void CreateSelectorWidget(APlayerController* WidgetOwner);

	UFUNCTION(BlueprintCallable)
	bool SetupSelector(APlayerController* WidgetOwner=nullptr);

	UFUNCTION(BlueprintCallable)
	void CloseSelector();

	//Multicast?
	void CompleteHandle( const TArray<uint8>& SelectedIdx );

	//Multicast?
	void CancelHandle();

	/**
	 * Setup this selector widget's OnCompleted and OnCanceled delegates.
	 */
	void SetupDelegate(const FOnSelectCompletedNative& iOnCompleted);

	FOnSelectCompletedNative OnSelectionCompleted;
};

/**
 *	Management class that sends queries to the client side during game event processing on the server side, 
 *	specifically for user-selected elements, and receives responses (via widgets)
 */
UCLASS( ClassGroup=(Selector), meta=(BlueprintSpawnableComponent) )
class DMSCORE_API UDMSSelectorManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDMSSelectorManager();

protected:
	//virtual void BeginPlay() override;

public:
	/**
	 * Create selector handle from param request form
	 * @param	Form			in Request form.
	 * @return	Created handle.
	 */
	UFUNCTION(BlueprintCallable/*, Server*/)
	UDMSSelectorHandle* RequestCreateSelector(UDMSSelectorRequestForm* Form);

	/**
	* Create multiple selectors handle from param request form
	* @param	Form			in Request forms.
	* @return	Created handles.
	*/
	UFUNCTION(BlueprintCallable)
	TArray<UDMSSelectorHandle*> RequestCreateSelectors(TArray<UDMSSelectorRequestForm*>& Form);
};
