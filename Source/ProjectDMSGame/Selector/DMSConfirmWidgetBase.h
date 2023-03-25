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

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "Selector/DMSSelectorQueue.h"
#include "Blueprint/UserWidget.h"
#include "DMSConfirmWidgetBase.generated.h"

class UDMSEffectDefinition;

/**
 *	������ ���� �����߿� �÷��̾� �ǻ縦 �����ؾ� �ϴ� ��Ȳ�� ����� �������� ���̽�.
 *	�뵵�� �°� ����� ������ ����Ʈ �������� �����ͷ� ����� �������� ����� ����.
 */
UCLASS()
class PROJECTDMSGAME_API UDMSConfirmWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:

public:
	UDMSConfirmWidgetBase(const FObjectInitializer& ObjectInitializer) :UUserWidget(ObjectInitializer){}
	
	UPROPERTY(BlueprintReadOnly)
	UDMSSequence* OwnerSeq;

	UFUNCTION(BlueprintCallable)
	void PopupSelector();

	UFUNCTION(BlueprintNativeEvent)
	void OnPopupSelector();
	virtual void OnPopupSelector_Implementation(){}

	UFUNCTION(BlueprintCallable)
	void CloseSelector();

	UFUNCTION(BlueprintNativeEvent)
	void OnCloseSelector();
	virtual void OnCloseSelector_Implementation(){}

	// Usage : Bind to Confirm Button
	UFUNCTION(BlueprintCallable)
	void CompleteSelect(UDMSDataObjectSet* OutData);
	// Usage : Bind to Cancel Button
	UFUNCTION(BlueprintCallable)
	void CancelSelect();

	template<typename FuncCompleted, typename FuncCanceled > // ���߿� universal reference �о��
	void SetupWidgetDelegates(FuncCompleted&& iOnCompleted, FuncCanceled&& iOnCanceled, UDMSSequence* iOwnerSeq);

	// Do something with "OwnerSeq"
	// Return "Setup widget was Successful"
	// Usage : 
	UFUNCTION(BlueprintNativeEvent)
	bool SetupWidget();
	virtual	bool SetupWidget_Implementation(){return true;}

	//UDELEGATE(BlueprintAuthorityOnly)
	DECLARE_DELEGATE_OneParam(FOnSelectCompleted, UDMSDataObjectSet*);
	DECLARE_DELEGATE(FOnSelectCanceled);

	FOnSelectCompleted OnSelectCompleted;
	FOnSelectCanceled OnSelectCanceled;

};
