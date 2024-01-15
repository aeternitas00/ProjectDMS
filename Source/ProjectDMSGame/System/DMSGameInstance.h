// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "DMSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UDMSGameInstance();

protected:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void OnCresteSessionComplete(FName SessionName, bool Succeeded);

	virtual void Init() override;
public:
	UFUNCTION(BlueprintCallable)
	void CreateDMSSession(FName SessionName);

	UFUNCTION(BlueprintCallable)
	void JoinDMSSession(FName SessionName);
};
