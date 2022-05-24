//// Fill out your copyright notice in the Description page of Project Settings.
//
//
#include "SubSystemManager.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

USubSystemManager::USubSystemManager() {}

void USubSystemManager::Initialize() {
	UE_LOG(LogTemp, Warning, TEXT("subsystem Intialized"));

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (Subsystem != nullptr) {

		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());

		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USubSystemManager::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &USubSystemManager::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USubSystemManager::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USubSystemManager::OnJoinSessionComplete);
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Session Interface Started"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}

}

void USubSystemManager::Host(FString ServerName, const FHostingData& InHostingData)
{

	if (!SessionInterface.IsValid()) return;

	HostingData = InHostingData;

	CurrentServerName = ServerName;

	auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession != nullptr)
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}
	else {
		CreateSession();
	}

}

void USubSystemManager::CreateSession() {

	if (!SessionInterface.IsValid()) return;

	FOnlineSessionSettings SessionSettings;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		SessionSettings.bIsLANMatch = true;
	}
	else
	{
		SessionSettings.bIsLANMatch = false;
	}

	SessionSettings.NumPublicConnections = HostingData.PlayerNums;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	//SessionSettings.bUseLobbiesIfAvailable = true;

	SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, CurrentServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);

}

void USubSystemManager::OnCreateSessionComplete(FName SessionName, bool Success) {

	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("created session"));

	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;

	FString InURL = "/Game/Maps/Lobby";

	World->ServerTravel(InURL);
}


void USubSystemManager::DestroySession() {

	if (!SessionInterface.IsValid()) return;

	auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession != nullptr)
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}
}

void USubSystemManager::OnDestroySessionComplete(FName SessionName, bool Success) {

	if (Success) {
		UE_LOG(LogTemp, Warning, TEXT("Session Destroyed Successfuly!"));
		CreateSession();
	}

}


void USubSystemManager::UpdateServerList() {
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;

		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void USubSystemManager::OnFindSessionsComplete(bool Success) {

	if (Success && SessionSearch.IsValid()) {

		UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));

		TArray<FServerData> ServerData;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults) {
			UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());

			FServerData Data;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				UE_LOG(LogTemp, Warning, TEXT("Found Server name: %s"), *ServerName);
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = "Could not find name.";
			}
			ServerData.Add(Data);
		}

		OnServersResultReceived.Broadcast(ServerData);
	}
}

void USubSystemManager::Join(int Index) {

	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void USubSystemManager::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {

	UE_LOG(LogTemp, Warning, TEXT("Join SessionCompleted."));

	if (!SessionInterface.IsValid()) return;

	FString Address;

	if (!SessionInterface->GetResolvedConnectString(SESSION_NAME, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}

	//UEngine* Engine = GetEngine();
	//Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

FString USubSystemManager::GetHostingDataOptionString(bool bActiveMatch)
{
	FString HostingDataString = "?HostingData=";
	HostingDataString.Append("/MatchTime_");
	HostingDataString.AppendInt(HostingData.MatchTime);
	HostingDataString.Append("/TotalMatchRound_");
	HostingDataString.AppendInt(HostingData.TotalMatchRound);
	HostingDataString.Append("/PlayerNums_");
	HostingDataString.AppendInt(HostingData.PlayerNums);
	HostingDataString.Append("/BotNums_");
	HostingDataString.AppendInt(HostingData.BotNums);
	HostingDataString.Append("/ActiveMatch_");
	HostingDataString.Append(bActiveMatch ? TEXT("true") : TEXT("false"));

	return HostingDataString;
}

void USubSystemManager::StartSession() {

	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}

}
