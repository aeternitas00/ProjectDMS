// Copyright JeongWoo Lee

#pragma once

#include "ProjectDMS.h"
#include "GameModes/DMSGameStateBase.h"
#include "DMSGameState.generated.h"

//class UDMSEIManagerComponent;
class ADMSLocationBase;
class ADMSPlayerState;
class UDMSContainerManagerComponent;
/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API ADMSGameState : public ADMSGameStateBase
{
	GENERATED_BODY()
public:
	ADMSGameState(const FObjectInitializer& Initializer);

protected:
	UPROPERTY(BlueprintReadOnly,VisibleInstanceOnly,Replicated)
	TObjectPtr<UDMSContainerManagerComponent> ContainerManagerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Replicated)
	TObjectPtr<UDMSAttributeComponent> AttributeComponent;
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int TestPlayerNum = 0;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<ADMSLocationBase*> Locations;

	UFUNCTION(BlueprintCallable)
	TArray<ADMSPlayerState*> GetDMSPlayers();

	UFUNCTION(BlueprintCallable)
	TArray<ADMSPlayerController*> GetDMSPlayerControllers();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetupDefaults();
	virtual void SetupDefaults_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
	ADMSSequence* CreateChaosTokenDrawSeq(ADMSSequence* ParentSeq);

	//Temp
	void SetPlayersFocusTarget(AActor* Target);

	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

};
