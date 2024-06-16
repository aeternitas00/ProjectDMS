// Copyright JeongWoo Lee

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
class ADMSActiveEffect;

//==========================================//
class UDMSSelectorHandle;

/**
 * Base class of selector widget.
 */
UCLASS()
class DMSCORE_API UDMSSelectorBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Ref of owner handle.
	 */
	UPROPERTY()
	TObjectPtr<UDMSSelectorHandle> OwnerHandle;

	UFUNCTION(BlueprintCallable,BlueprintPure,meta = (CompactNodeTitle = "SelectAmount"))
	uint8 GetSelectAmount() const;


	UFUNCTION(BlueprintImplementableEvent)
	bool InitializeSelector(const UDMSSelectorRequestForm* Candidates);

	UFUNCTION(BlueprintCallable)
	void CompleteSelect(const TArray<uint8>& SelectedIdx);

	/**
	* Cancel this selector widget. ( This function have to be bound to widget's cancel button or something )
	*/
	UFUNCTION ( BlueprintCallable )
	void CancelSelect();

	//UFUNCTION ( BlueprintCallable )
	void PopupSelector();

	/**
	* Executed when widget popped up.
	*/
	UFUNCTION ( BlueprintImplementableEvent )
	void OnPopupSelector();

	/**
	* Close this widget.
	*/
	UFUNCTION ( BlueprintCallable )
	void CloseSelector();

	/**
	* Executed when widget closed.
	*/
	UFUNCTION ( BlueprintImplementableEvent )
	void OnCloseSelector();
};

