// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DMSGameInstance.h"
#include "System/DMSSession.h"
#include "GameFramework/PlayerState.h"
#include "OnlineSubsystemNull.h"

//THIRD_PARTY_INCLUDES_START
//#include "ThirdParty/Steamworks/Steamv153/sdk/public/steam/steam_api.h"
//#include "ThirdParty/Steamworks/Steamv153/sdk/public/steam/steam_gameserver.h"
//THIRD_PARTY_INCLUDES_END

UDMSGameInstance::UDMSGameInstance()
{
}


void UDMSGameInstance::Init()
{
    Super::Init();

    //if ( !SteamAPI_Init() )     DMS_LOG_SIMPLE(TEXT("SteamAPI_Init Failed"));

    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (!OnlineSubsystem) return;
    
    SessionInterface = OnlineSubsystem->GetSessionInterface();

    //if (!SessionInterface.IsValid()) return;
    //
    //SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UDMSGameInstance::OnCreateSessionComplete); 
    //SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UDMSGameInstance::OnFindSessionsComplete);
    //SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UDMSGameInstance::OnJoinSessionComplete);

    //DefaultSessionSettings.bUsesPresence=true;
    //DefaultSessionSettings.bAllowJoinInProgress=false;
    //DefaultSessionSettings.NumPublicConnections = 4;
    //DefaultSessionSettings.bIsDedicated=false;
    //DefaultSessionSettings.bIsLANMatch=true;
    //DefaultSessionSettings.bShouldAdvertise=true;
    //DefaultSessionSettings.bUseLobbiesIfAvailable = true;
}

//void UDMSGameInstance::Shutdown()
//{
//    Super::Shutdown();
//    //SessionInterface->DestroySession("MySession");
//}
//
//void UDMSGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
//{
//    if(!Succeeded) {
//        DMS_LOG_SIMPLE(TEXT("OnCreateSession Failed : %s"), *SessionName.ToString());
//        return;
//    }
//    DMS_LOG_SIMPLE(TEXT("OnCreateSession Complete : %s"), *SessionName.ToString());
//
//    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
// // GetWorld()->GetAuthGameMode()->GameSession->RegisterPlayer(PC,PC->PlayerState->GetUniqueId(),false);
//
//    if (SessionInterface->GetNamedSession(SessionName) == nullptr )
//    {
//        DMS_LOG_SIMPLE(TEXT("GetNamedSession Failed : %s"), *SessionName.ToString());
//    }
//}
//
//void UDMSGameInstance::OnFindSessionsComplete(bool Succeeded)
//{
//    if(!Succeeded) { 
//        DMS_LOG_SIMPLE(TEXT("OnFindSession Failed"));
//        return;
//    }
//
//   
//    TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
//
//    if(Results.Num()==0)  { 
//        DMS_LOG_SIMPLE(TEXT("OnFindSession Failed"));
//        return;
//    }
//
//    
//    DMS_LOG_SIMPLE(TEXT("OnFindSession Complete %d / %d"),Results[0].Session.NumOpenPublicConnections,Results[0].Session.SessionSettings.NumPublicConnections);
//
//    SessionInterface->JoinSession(0, "MyGame", Results[0]);
//}
//
//void UDMSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
//{
//    DMS_LOG_SIMPLE(TEXT("OnJoinSession : %s , %d"), *SessionName.ToString(),Result);
//    if (Result == EOnJoinSessionCompleteResult::Type::Success){
//
//        //APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
//        //if(!PC) return;
//        //FString Address="";
//        //SessionInterface->GetResolvedConnectString(SessionName,Address);
//        //if(Address!="") 
//        //    PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
//    }
//}

void UDMSGameInstance::ServerTravel(FString Level)
{
    FString URL=Level;
    URL+= "?listen";
    SessionInterface->StartSession(NAME_GameSession);
    GetWorld()->ServerTravel(URL);
}

//void UDMSGameInstance::CreateDMSSession(FName SessionName)
//{
//    // Call the CreateSession function
//    FOnlineSessionSettings SessionSettings = DefaultSessionSettings;
//    SessionInterface->CreateSession(0, SessionName, SessionSettings);   
//}
//
//void UDMSGameInstance::JoinDMSSession(FName SessionName)
//{
//    SessionSearch = MakeShareable(new FOnlineSessionSearch());
//    SessionSearch->bIsLanQuery = true;    
//    SessionSearch->MaxSearchResults = 100;
//    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
//
//    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
//}
//
//void UDMSGameInstance::QuitDMSSession(FName SessionName)
//{
//    SessionInterface->DestroySession(SessionName);
//}

int32 UDMSGameInstance::GetSessionPlayerNum() const
{
    auto CurrentSession = SessionInterface->GetNamedSession("MyGame");

    if (!CurrentSession) return 0;

    //FOnlineSessionSettings SessionSettings = DefaultSessionSettings;

    //SessionInterface->UpdateSession("MyGame",SessionSettings);

    //ISteamMatchmaking* SteamMatchmakingPtr = SteamMatchmaking();

    //check(SteamMatchmakingPtr);

    //const FString SessionIDString = CurrentSession->GetSessionIdStr();
    //uint64 SteamID = FCString::Atoi64(*SessionIDString);

    //int NumLobby = SteamMatchmakingPtr->GetNumLobbyMembers(SteamID);
    //int MaxLobby = SteamMatchmakingPtr->GetLobbyMemberLimit(SteamID);

    //DMS_LOG_SIMPLE(TEXT("GetSessionPlayerNum [%lld] : %d of %d"),  SteamID, NumLobby, MaxLobby );

    //return CurrentSession->SessionSettings.NumPublicConnections - NumLobby;

    DMS_LOG_SIMPLE(TEXT("GetSessionPlayerNum : %d of %d"), CurrentSession->NumOpenPublicConnections, CurrentSession->SessionSettings.NumPublicConnections );

    return CurrentSession->SessionSettings.NumPublicConnections - CurrentSession->NumOpenPublicConnections;
}


