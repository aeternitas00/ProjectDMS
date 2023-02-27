// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Common/DMSCommons.h"
#include "Selector/DMSSelectorQueue.h"
#include "Blueprint/UserWidget.h"
#include "DMSEffectElementSelectorWidget.generated.h"

class UDMSEffectDefinition;

/**																																	
 * ����Ʈ, ������ Ŭ������ ���Ǵϼǿ� ������ �ν��Ͻ���θ� ����. 
 * TODO :: BP�� �߰��� ����.
 * Candidate ��ü�Ҹ��� �̸�?
 */
UCLASS(Blueprintable, Abstract)
class PROJECTDMSGAME_API UDMSEffectElementSelectorWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//Cache
	UDMSSequence* OwnerSeq;

	UPROPERTY()
	UDMSDataObject* CandidatesData;

	UDMSEffectDefinition* SourceEffectDefinition;
public:
	//UDMSEffectElementSelectorWidget(): CandidatesData(nullptr),SourceEffectDefinition(nullptr){}
	// ��ġ? Effect handler or Here
	//UPROPERTY()

	template<typename FuncCompleted, typename FuncCanceled > // ���߿� universal reference �о��
	void InitializeSelector_Internal(FuncCompleted&& iOnCompleted, FuncCanceled&& iOnCanceled, UDMSSequence* iOwnerSeq);

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeSelector(UDMSSequence* iOwnerSeq);

	// Usage : Bind to Confirm Button
	UFUNCTION(BlueprintCallable)
	void CompleteSelect(UDMSDataObjectSet* OutData);

	// Usage : Bind to Cancel Button
	UFUNCTION(BlueprintCallable)
	void CancelSelect();

	// Return "Setting up Candidate was Successful"
	// Usage : 
	UFUNCTION(BlueprintImplementableEvent)
	bool SetupCandidates();
	
	// Usage : Use this in SetupCandidate() when if this selector have dynamic candidates.
	// ex) 
	UFUNCTION(BlueprintCallable)
	bool SetupCandidatesFromED(/*UDMSSequence* iSeq*/);
	
	FORCEINLINE void SetSourceEffectDefinition(UDMSEffectDefinition* iDef) { SourceEffectDefinition = iDef;}
	FORCEINLINE UDMSDataObject* GetCandidatesData() {return CandidatesData; }

	//UDELEGATE(BlueprintAuthorityOnly)
	DECLARE_DELEGATE_OneParam(FOnSelectCompleted, UDMSDataObjectSet*);
	DECLARE_DELEGATE(FOnSelectCanceled);

	FOnSelectCompleted OnSelectCompleted;
	FOnSelectCanceled OnSelectCanceled;
};

UCLASS(Blueprintable, Abstract)
class PROJECTDMSGAME_API UDMSSelectorWidget_Object : public UDMSEffectElementSelectorWidget
{
	GENERATED_BODY()

protected:
	
public:
};


UCLASS(Blueprintable, Abstract)
class PROJECTDMSGAME_API UDMSSelectorWidget_Numeric : public UDMSEffectElementSelectorWidget
{
	GENERATED_BODY()


public:

	
};

template<typename FuncCompleted, typename FuncCanceled>
void UDMSEffectElementSelectorWidget::InitializeSelector_Internal(FuncCompleted&& iOnCompleted, FuncCanceled&& iOnCanceled, UDMSSequence* iOwnerSeq) {
	//CandidatesData=nullptr;
	//SourceEffectDefinition=nullptr;
	OnSelectCompleted.Unbind();
	OnSelectCanceled.Unbind();
	//support for l-value 
	OnSelectCompleted.BindLambda(std::forward<FuncCompleted&&>(iOnCompleted));
	OnSelectCanceled.BindLambda(std::forward<FuncCanceled&&>(iOnCanceled));
	//OnSelectCompleted.BindLambda(Lambda);
	OwnerSeq = iOwnerSeq;

	InitializeSelector(OwnerSeq);
}
