// Copyright JeongWoo Lee

#pragma once

#include "ProjectDMS.h"
#include "Character/DMSCharacterDefinition.h"
#include "DMSEnemyCharacterDefinition.generated.h"



// Inherit Playercharacter asset ???

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSEnemyCharacterDefinition : public UDMSCharacterDefinition
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};


