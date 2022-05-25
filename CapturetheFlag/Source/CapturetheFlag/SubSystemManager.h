//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SubSystemManager.generated.h"




USTRUCT(BlueprintType)
struct FHostingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HostingData)
	int MatchTime = 1;// In Minutes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HostingData)
	int TotalMatchRound = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HostingData)
	int PlayerNums = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HostingData)
	int BotNums = 0;

	FHostingData(int newMatchTime, int newTotalMatchRound, int newPlayerNums, int newBotNums) :MatchTime(newMatchTime), TotalMatchRound(newTotalMatchRound), PlayerNums(newPlayerNums), BotNums(newBotNums) {}
	FHostingData() {}
};


USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ServerData)
	FString Name = "";
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ServerData)
	int CurrentPlayers = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ServerData)
	int MaxPlayers = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ServerData)
	FString HostUsername = "";

	FServerData(FString newName, int newCurrentPlayers, int newMaxPlayers, FString newHostUsername) :Name(newName), CurrentPlayers(newCurrentPlayers), MaxPlayers(newMaxPlayers), HostUsername(newHostUsername) {}
	FServerData() {}
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServersResult, const TArray<FServerData>&, ServerData);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CAPTURETHEFLAG_API USubSystemManager : public UObject
{
	GENERATED_BODY()
	

//************************** Constructors **************************
public:
	USubSystemManager();


//************************** Public Properties **************************
public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "SubSystem Manager")
	FServersResult OnServersResultReceived;

	FHostingData HostingData;

	//************************** Public Functions **************************
public:
	UFUNCTION(BlueprintCallable)
	void Initialize();

	UFUNCTION(BlueprintCallable)
	void Host(FString ServerName, const FHostingData& InHostingData);

	UFUNCTION(BlueprintCallable)
	void Join(int Index);

	UFUNCTION(BlueprintCallable)
	void CreateSession();

	UFUNCTION(BlueprintCallable)
	void DestroySession();

	UFUNCTION(BlueprintCallable)
	void StartSession();

	UFUNCTION(BlueprintCallable)
	void UpdateServerList();

	//************************** Private Properties **************************
private:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	FString CurrentServerName;

	bool bHostApplied = false;

	//************************** Private Functions **************************
private:
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	FString GetHostingDataOptionString(bool bActiveMatch = false);
};
