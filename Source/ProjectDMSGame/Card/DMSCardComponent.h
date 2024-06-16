// Copyright JeongWoo Lee

#pragma once

//

#include "ProjectDMS.h"
#include "Components/ActorComponent.h"
#include "DMSCardComponent.generated.h"

class ADMSActiveEffect;
class UDMSCardDefinition;

// ======================================
//			DEPRECATED
// ======================================
UCLASS( ClassGroup=(Card), meta=(BlueprintSpawnableComponent) )
class PROJECTDMSGAME_API UDMSCardComponent : public UActorComponent
{
	GENERATED_BODY()

protected:


public:	
	// Sets default values for this component's properties
	UDMSCardComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//========================================================================================

	const UDMSCardDefinition* GetCardDefinition();
	// Lookup CardDefinition Func
	
	// ���� �̿��� �ߵ��� ����� ���� �̸����� ���
		
};
