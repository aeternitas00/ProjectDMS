// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FindSessionsCallbackProxy.h"
#include "DMSGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FDMSServerInfo
{
	GENERATED_BODY()

public:
	FString ServerName;
	int32 CurrentPlayers;
	int32 MaxPlayers;


};
/**
 * 
 */
UCLASS()
class PROJECTDMSGAME_API UDMSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UDMSGameInstance();

	virtual void Init() override;
	//virtual void Shutdown() override;

protected:
	//FOnlineSession* CurrentSession;
	IOnlineSessionPtr SessionInterface;
	//TSharedPtr<FOnlineSessionSearch> SessionSearch;
	//FOnlineSessionSettings DefaultSessionSettings;

	//virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	//virtual void OnFindSessionsComplete(bool Succeeded);
	//virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

public:

	UFUNCTION(BlueprintCallable)
	void ServerTravel(FString Level);

	//UFUNCTION(BlueprintCallable)
	//void CreateDMSSession(FName SessionName);

	//UFUNCTION(BlueprintCallable)
	//void JoinDMSSession(FName SessionName);

	//UFUNCTION(BlueprintCallable)
	//void QuitDMSSession(FName SessionName);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetSessionPlayerNum() const;
};
