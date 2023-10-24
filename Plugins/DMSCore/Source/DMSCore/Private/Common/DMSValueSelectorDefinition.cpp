#include "Common/DMSValueSelectorDefinition.h"
#include "Selector/DMSConfirmWidgetBase.h"
#include "Common/DMSTargetGenerator.h"

TSubclassOf<UDMSConfirmWidgetBase> UDMSValueSelectorDefinition::GetWidgetClass() 
{ 
	FClassProperty* Prop = FindFProperty<FClassProperty>(GetClass(),"WidgetClass");
	UClass* Rv = nullptr;
	if (Prop!=nullptr) Rv = Cast<UClass>(Prop->GetPropertyValue_InContainer(this));
	if (Rv==nullptr) return nullptr;
	return Rv->IsChildOf<UDMSConfirmWidgetBase>() ? Rv : nullptr;
}
