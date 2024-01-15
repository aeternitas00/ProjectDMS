// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DMSGameInstance.h"
#include "System/DMSSession.h"
#include "Interfaces/OnlineSessionInterface.h"


UDMSGameInstance::UDMSGameInstance()
{
}


void UDMSGameInstance::Init()
{
    Super::Init();

    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (!OnlineSubsystem) return;
    
    SessionInterface = OnlineSubsystem->GetSessionInterface();

    if (!SessionInterface.IsValid()) return;
    
    SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UDMSGameInstance::OnCresteSessionComplete);

}

void UDMSGameInstance::OnCresteSessionComplete(FName SessionName, bool Succeeded)
{
}

void UDMSGameInstance::CreateDMSSession(FName SessionName)
{ 
    FOnlineSessionSettings SessionSettings;
    // Customize session settings here
    SessionSettings.bUsesPresence=true;
    SessionSettings.bAllowJoinInProgress=true;
    SessionSettings.NumPublicConnections = 4;
    SessionSettings.bIsDedicated=false;
    SessionSettings.bIsLANMatch=true;
    SessionSettings.bShouldAdvertise=true;
    
    // Call the CreateSession function
    SessionInterface->CreateSession(0, SessionName, SessionSettings);
}

void UDMSGameInstance::JoinDMSSession(FName SessionName)
{
    SessionSearch = MakeShareable(new FOnlineSessionSearch());

    SessionSearch->bIsLanQuery = true;
    SessionSearch->MaxSearchResults = 100;

    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}
