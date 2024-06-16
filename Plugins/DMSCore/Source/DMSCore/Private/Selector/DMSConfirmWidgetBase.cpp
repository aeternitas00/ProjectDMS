// Copyright JeongWoo Lee


#include "Selector/DMSConfirmWidgetBase.h"
#include "Sequence/DMSSequence.h"
#include "Effect/DMSEffectInstance.h"
#include "Selector/DMSSelectorManager.h"

uint8 UDMSSelectorBase::GetSelectAmount() const
{
	return OwnerHandle ? OwnerHandle->StoredForm->SelectAmount : 0;
}

void UDMSSelectorBase::CompleteSelect(const TArray<uint8>& SelectedIdx)
{
	OwnerHandle->CompleteHandle(SelectedIdx);
}

void UDMSSelectorBase::CancelSelect()
{
	OwnerHandle->CancelHandle();
}

void UDMSSelectorBase::PopupSelector()
{
	if (IsInViewport()) return;

	AddToViewport();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OnPopupSelector();
}

void UDMSSelectorBase::CloseSelector()
{
	if (!IsInViewport()) return;

	//...
	RemoveFromParent();
	OnCloseSelector();
}
