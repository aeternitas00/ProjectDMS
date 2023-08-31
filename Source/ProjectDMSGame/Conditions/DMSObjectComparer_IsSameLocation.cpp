#include "Conditions/DMSObjectComparer_IsSameLocation.h"
#include "Location/DMSLocatableInterface.h"

bool UDMSObjectComparer_IsSameLocation::Compare_Implementation(UObject* SourceObject, UDMSSequence* iSeq, UObject* TargetObject, bool NullIsTrue) {
	DMS_LOG_SIMPLE(TEXT("%s and %s is Same location?"), *SourceObject->GetName(), *TargetObject->GetName());
	
	//DMS_LOG_SCREEN(TEXT("%s and %s is Same location?"), *SourceObject->GetName(), *TargetObject->GetName());
	if (!SourceObject->Implements<UDMSLocatableInterface>() || !TargetObject->Implements<UDMSLocatableInterface>())
		return NullIsTrue;

	return IDMSLocatableInterface::Execute_GetCurrentLocation(SourceObject) == IDMSLocatableInterface::Execute_GetCurrentLocation(TargetObject);
}
