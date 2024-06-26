// Copyright JeongWoo Lee


#include "Card/DMSCardContainerComponent.h"
#include "Library/DMSCoreFunctionLibrary.h"
#include "Misc/DateTime.h"

TArray<ADMSCardBase*> UDMSCardContainerComponent::GetCards()
{
	TArray<ADMSCardBase*> rv;

	for ( auto Card : SpawnableList.Actors )
	{
		if(Card->IsA<ADMSCardBase>())
			rv.Add(Cast<ADMSCardBase>(Card));
	}

	return rv;
}
