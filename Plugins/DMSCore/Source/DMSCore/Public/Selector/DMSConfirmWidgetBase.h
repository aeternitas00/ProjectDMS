// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	Widget for player decision-making necessary to proceed with the DMS Sequence.
 *
 * =========================================
 */

//#include "DMSCoreIncludes.h"
#include "Common/DMSCommons.h"
#include "Selector/DMSSelectorQueue.h"
#include "Blueprint/UserWidget.h"
#include "DMSConfirmWidgetBase.generated.h"

class UDMSEffectDefinition;
class UDMSEffectInstance;
/**
 *	������ ���� �����߿� �÷��̾� �ǻ縦 �����ؾ� �ϴ� ��Ȳ�� ����� �������� ���̽�.
 *	�뵵�� �°� ����� ������ ����Ʈ �������� �����ͷ� ����� �������� ����� ����.
 * 
 * TODO :: RENAME?
 */
UCLASS()
class DMSCORE_API UDMSConfirmWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:

public:
	UDMSConfirmWidgetBase(const FObjectInitializer& ObjectInitializer) :UUserWidget(ObjectInitializer), CurrentEIIndex(0){}
	
	/**
	 * Owner sequence of widget.
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDMSSequence> CurrentSequence;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UDMSEffectInstance>> EIArray;

	UPROPERTY(BlueprintReadOnly)
	int CurrentEIIndex;

	UPROPERTY(BlueprintReadOnly)
	FDMSValueSelectionForm SelectionForm;

	/**
	 * Storing candidate data before this widget is popped up.
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UDMSDataObject>> CandidatesData;

	UFUNCTION()
	bool InitializeWidget(FDMSValueSelectionForm InitializeData, UDMSSequence* iSequence);

	UFUNCTION(BlueprintImplementableEvent)
	void GenerateCandidaes();

	/**
	 * Popup this widget.
	 */
	UFUNCTION(BlueprintCallable)
	void PopupSelector();

	/**
	 * Executed when widget popped up.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnPopupSelector();
	virtual void OnPopupSelector_Implementation(){}

	/**
	 * Close this widget.
	 */
	UFUNCTION(BlueprintCallable)
	void CloseSelector();

	/**
	 * Executed when widget closed.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnCloseSelector();
	virtual void OnCloseSelector_Implementation(){}

	/**
	 * Complete this selector widget. ( This function have to be bound to widget's ok button or something )
	 */
	UFUNCTION(BlueprintCallable)
	void CompleteSelect();

	/**
	 * Cancel this selector widget. ( This function have to be bound to widget's cancel button or something )
	 */
	UFUNCTION(BlueprintCallable)
	void CancelSelect();

	/**
	 * Setup this selector widget's OnCompleted and OnCanceled delegates.
	 * @param	iOnCompleted				Lambda parameter binded to OnSelectCompleted.
	 * @param	iOnCanceled					Lambda parameter binded to OnSelectCanceled.
	 */
	template<typename FuncCompleted, typename FuncCanceled > // ���߿� universal reference �о��
	void SetupWidgetDelegates(FuncCompleted&& iOnCompleted, FuncCanceled&& iOnCanceled);


	//UDELEGATE(BlueprintAuthorityOnly)
	DECLARE_DELEGATE(FOnSelectCompleted);
	DECLARE_DELEGATE(FOnSelectCanceled);

	FOnSelectCompleted OnSelectCompleted;
	FOnSelectCanceled OnSelectCanceled;

	/**
	 * Setup widget ( with CurrentSequence's data ).
	 * @return	true if Setup was successful
	 */
	UFUNCTION(BlueprintImplementableEvent)
	bool SetupWidget();

	UFUNCTION(BlueprintCallable)
	UDMSDataObject* GetCurrentCandidatesData();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UDMSDataObjectSet* GetTargetDataSet();
	virtual UDMSDataObjectSet* GetTargetDataSet_Implementation();
	//virtual	bool SetupWidget_Implementation(){return true;}
};


template<typename FuncCompleted, typename FuncCanceled>
void UDMSConfirmWidgetBase::SetupWidgetDelegates(FuncCompleted&& iOnCompleted, FuncCanceled&& iOnCanceled/*, UDMSSequence* iOwnerSeq*/) {

	//CurrentSequence = iOwnerSeq;
	//if (!SetupWidget()){return false;};

	OnSelectCompleted.Unbind();
	OnSelectCanceled.Unbind();

	OnSelectCompleted.BindLambda(std::forward<FuncCompleted&&>(iOnCompleted));
	OnSelectCanceled.BindLambda(std::forward<FuncCanceled&&>(iOnCanceled));

	//return true;
}



//==========================================//



UCLASS()
class DMSCORE_API UDMSConfirmWidgetBase_uint8 : public UDMSConfirmWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateData(UDMSDataObjectSet* UpdatingData,uint8 Value){ UpdatingData->SetData(SelectionForm.OutDataKey,Value); }
};