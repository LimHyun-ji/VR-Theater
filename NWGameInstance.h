// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h>
#include "NWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VRTHEATER_API UNWGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UNWGameInstance();

	UPROPERTY()
	class AVRTheaterGameModeBase* myGameModeBase;

	UPROPERTY(BlueprintReadWrite, Category="Code")
	class ACharacter* CharacterType;

	UPROPERTY(BlueprintReadWrite, Category = "Code")
	bool isWaitToStart=false;
/*
	UPROPERTY()
	class UUserWidget* waitToStartWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> warningAdminClass;

	UPROPERTY()
		class UUserWidget* warningAdminWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> LoadingWidgetClass;

	UPROPERTY()
	class UUserWidget* loadingWidget;
	*/
	UPROPERTY(BlueprintReadWrite, Category = "Code")
		uint8 currentLevelNumber = 0;

	UPROPERTY(BlueprintReadWrite, Category="Code")
	uint8 UserType=0;

	UFUNCTION(BlueprintCallable, Category = "Code")
		void CreateServer();

	UFUNCTION(BlueprintCallable, Category = "Code")
		void JoinServer();

	UFUNCTION(BlueprintCallable, Category="Code")
		void DestroyServer();

protected:
		virtual void Init() override;
		virtual void OnCreateSessionComplete(FName ServerName, bool isSucceed);
		virtual void OnFindSessionsComplete(bool isSucceed);
		virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
		virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

private:

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	//LEVEL ANIMATION//
public:
	FString JoinAddress = "";

	UPROPERTY(BlueprintReadWrite)
	bool isJoinListenServer=false;

	UPROPERTY(BlueprintReadWrite)
	bool isCreateServer=false;

	UPROPERTY(BlueprintReadWrite)
		bool isCompleteFindSession = false;

};
