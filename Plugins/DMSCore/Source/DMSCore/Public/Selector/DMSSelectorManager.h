// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DMSCoreIncludes.h"
#include "Components/ActorComponent.h"
#include "Common/DMSCommons.h"
#include "DMSSelectorManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectorCompletedDynamic, TArray<uint8>, SelectedIndex);
DECLARE_DELEGATE_OneParam(FOnSelectorCompleted, TArray<uint8>);

class UDMSSelectorBase;
class UDMSWidgetQueue;

USTRUCT(BlueprintType)
struct FDMSSelectorRequestForm
{
	GENERATED_BODY()

	FDMSSelectorRequestForm():SelectAmount(0){}

	UPROPERTY(BlueprintReadWrite)
	TArray<UDMSDataObject*> Candidates;

	UPROPERTY(BlueprintReadWrite)
	uint8 SelectAmount;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UDMSSelectorBase> SelectorClass;

	UPROPERTY(BlueprintReadOnly)
	FOnSelectorCompletedDynamic OnCompleted;

	FOnSelectorCompleted OnCompletedNative;
};


UCLASS(ClassGroup = (Selector))
class DMSCORE_API UDMSSelectorHandle : public UObject
{
	GENERATED_BODY()

public:
	UDMSSelectorHandle();

	UPROPERTY(BlueprintReadOnly)
	FDMSSelectorRequestForm StoredForm;

	UDMSSelectorBase* Widget;

	UDMSWidgetQueue* OwnerQueue;

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
	 * @param	iOnCompleted				Lambda parameter binded to OnSelectCompleted.
	 * @param	iOnCanceled					Lambda parameter binded to OnSelectCanceled.
	 */
	template<typename FuncCompleted , typename FuncCanceled > // 나중에 universal reference 읽어보기
	void SetupDelegates ( FuncCompleted&& iOnCompleted , FuncCanceled&& iOnCanceled );


	//UDELEGATE(BlueprintAuthorityOnly)

	DECLARE_MULTICAST_DELEGATE (FOnSelectorHandleCompleted);
	DECLARE_MULTICAST_DELEGATE (FOnSelectorHandleCanceled);

	FOnSelectorHandleCompleted OnSelectCompleted;
	FOnSelectorHandleCanceled OnSelectCanceled;
};

template<typename FuncCompleted , typename FuncCanceled>
void UDMSSelectorHandle::SetupDelegates( FuncCompleted&& iOnCompleted , FuncCanceled&& iOnCanceled )
{
	OnSelectCompleted.Clear();
	OnSelectCanceled.Clear();

	OnSelectCompleted.AddLambda( std::forward<FuncCompleted&&> ( iOnCompleted ) );
	OnSelectCanceled.AddLambda( std::forward<FuncCanceled&&> ( iOnCanceled ) );
}

UCLASS( ClassGroup=(Selector), meta=(BlueprintSpawnableComponent) )
class DMSCORE_API UDMSSelectorManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDMSSelectorManager();

protected:
	// Called when the game starts
	//virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	UDMSSelectorHandle* RequestCreateSelector(FDMSSelectorRequestForm& Form);

	UFUNCTION(BlueprintCallable)
	TArray<UDMSSelectorHandle*> RequestCreateSelectors(TArray<FDMSSelectorRequestForm>& Form);
};