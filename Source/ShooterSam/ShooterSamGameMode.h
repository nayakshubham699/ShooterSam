// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "ShooterSamCharacter.h"
#include "HUDWidget.h"
//#include "ShooterSamPlayerController.h"

#include "ShooterSamGameMode.generated.h"

//class AShooterSamCharacter;

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AShooterSamGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AShooterSamGameMode();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	int32 CountDownDelay = 3;

	int32 CountDownSeconds;

	FTimerHandle CountDownTimerHandle;

	AShooterSamCharacter* Player;

	bool IsVictory = false;

	int32 TotalEnemyCount;

	int32 AliveEnemyCount;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere)
	UHUDWidget* HUDWidget;

	//AShooterSamPlayerController* PlayerController;

	void ActorDied(AActor* DeadActor);

	void OnCountDownTimerTimeout();

	void GameOver(bool Victory);

	void HUDDisplayToggle( bool bHUDVisible);
};



