// Fill out your copyright notice in the Description page of Project Settings.


#include "NWGameInstance.h"
#include <../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h>
#include <../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h>
#include <../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/UserWidget.h>
#include "VRTheaterGameModeBase.h"

UNWGameInstance::UNWGameInstance()
{

}

void UNWGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init"));

	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNWGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNWGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNWGameInstance::OnJoinSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNWGameInstance::OnDestroySessionComplete);
		}

	}
}

void UNWGameInstance::OnCreateSessionComplete(FName ServerName, bool isSucceed)
{
	UE_LOG(LogTemp, Warning, TEXT("OncreateSessionComplete, Succeed: %d"), isSucceed);
	if (isSucceed)
	{
		//GetWorld()->ServerTravel("/Game/VRTheater/RedHoodRoom/RedHoodRoomMap?listen");		
		GetWorld()->ServerTravel("/Game/Project1/StoryRoom_RedHoodRoom?listen");
		//UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/VRTheater/Lobby/LobbyMap"));

	}
}

void UNWGameInstance::OnFindSessionsComplete(bool isSucceed)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete, Succeed: %d"), isSucceed);
	if (isSucceed)
	{
		TArray<FOnlineSessionSearchResult>SearchResults = SessionSearch->SearchResults;

		UE_LOG(LogTemp, Warning, TEXT("SearchResult, Server Count: %d"), SearchResults.Num());//생성된 리슨 서버의 수

		if (SearchResults.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("Joining Server"));
			if (UserType == 2) { //빨간 모자
				//빨간모자는 더이상 출입 불가하다는 메세지
				/*if (warningAdminClass)
				{
					loadingWidget->RemoveFromViewport();
					UE_LOG(LogTemp, Warning, TEXT("Widget"));
					warningAdminWidget = CreateWidget<UUserWidget>(GetWorld(), warningAdminClass);
					warningAdminWidget->AddToViewport();

				}*/
				UE_LOG(LogTemp, Warning, TEXT("No more Red Hood"));
			}
			else
				SessionInterface->JoinSession(0, "Theater Session", SearchResults[0]);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UserType: %d"), UserType);
			if (UserType == 2)
				CreateServer();//빨간모자
			
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Wait to start"));
				isWaitToStart=true;
				isCompleteFindSession = true;
				
			}

		}
	}
}



void UNWGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("onJoin Session Complete, SessionName: %s"), *SessionName.ToString());
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
			isJoinListenServer = true;//FadeOutAnimation을 PC에서 받아온다
	}
}

void UNWGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful);

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "LobbyMap", true);
	}

}


void UNWGameInstance::CreateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		SessionSettings.bIsLANMatch = false;
	else
		SessionSettings.bIsLANMatch = true;

	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;

	SessionInterface->CreateSession(0, FName("Theater Session"), SessionSettings);
}

void UNWGameInstance::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Join Server"));


	SessionSearch = MakeShareable(new FOnlineSessionSearch);
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		SessionSearch->bIsLanQuery = false;
	else
		SessionSearch->bIsLanQuery = true;

	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UNWGameInstance::DestroyServer()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->DestroySession("Theater Session");
	}
}

