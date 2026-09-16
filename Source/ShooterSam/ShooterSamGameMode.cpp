// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterSamGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "ShooterAI.h"
#include "ShooterSamPlayerController.h"

AShooterSamGameMode::AShooterSamGameMode()
{
	// stub
}

void AShooterSamGameMode::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AShooterSamCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	TArray<AActor*> ShooterAIActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterAI::StaticClass(), ShooterAIActors);

	for (int32 LoopIndex = 0; LoopIndex < ShooterAIActors.Num(); LoopIndex++)
	{
		AActor* ShooterAIActor = ShooterAIActors[LoopIndex];
		AShooterAI* ShooterAI = Cast<AShooterAI>(ShooterAIActor);

		if (ShooterAI)
		{
			ShooterAI->StartBehaviorTree(Player);
		}
	}
	// Alive Enemy count
	
	TotalEnemyCount = ShooterAIActors.Num();
	AliveEnemyCount = TotalEnemyCount;
	
	//CountDownCode
	CountDownSeconds = CountDownDelay;

	GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &AShooterSamGameMode::OnCountDownTimerTimeout, 1.0f, true);

	AShooterSamPlayerController*PlayerController = Cast<AShooterSamPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerController)
	{
		HUDWidget = CreateWidget<UHUDWidget>(PlayerController, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->SetCountDownText(FString::Printf(TEXT("Get Ready")));
			HUDWidget->SetEnemyCountText(FString::Printf(TEXT("%d"), AliveEnemyCount));
			HUDWidget->ReplayButton->SetVisibility(ESlateVisibility::Hidden);
			HUDWidget->ExitButton->SetVisibility(ESlateVisibility::Hidden);
			HUDWidget->ResumeButton->SetVisibility(ESlateVisibility::Hidden);
			HUDWidget->AddToViewport();
		}
	}
}

void AShooterSamGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;
	if (DeadActor == Player)
	{
		IsGameOver = true;
		IsVictory = false;
	}
	else
	{
		AliveEnemyCount--;

		HUDWidget->SetEnemyCountText(FString::Printf(TEXT("%d"), AliveEnemyCount));
		if(AliveEnemyCount == 0)
		{
			IsVictory = true;
			IsGameOver = true;

			
		}
	}

	if (IsGameOver)
	{
		AShooterSamPlayerController* PlayerController = Cast<AShooterSamPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = true;

			GameOver(IsVictory);
		}
	}
}

void AShooterSamGameMode::OnCountDownTimerTimeout()
{
	CountDownSeconds--;
	if(CountDownSeconds > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Countdown: %d"), CountDownSeconds);
		if(HUDWidget)
		{
			HUDWidget->SetCountDownText(FString::Printf(TEXT("%d"), CountDownSeconds));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUDWidget is null!"));
		}
	}
	else if(CountDownSeconds == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Countdown finished!"));
		if (HUDWidget)
		{
			HUDWidget->SetCountDownText(FString::Printf(TEXT("Go!")));
		}
		Player->SetPlayerEnabled(true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountDownTimerHandle);
		if (HUDWidget)
		{
			HUDWidget->CountDown->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AShooterSamGameMode::GameOver(bool Victory)
{
	if (Victory)
	{
		HUDWidget->CountDown->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
		
		HUDWidget->SetCountDownText(FString::Printf(TEXT("Mission Successful!")));

		HUDDisplayToggle(true);
	}
	else
	{
		HUDWidget->CountDown->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		
		HUDWidget->SetCountDownText(FString::Printf(TEXT("You Died!")));

		HUDDisplayToggle(true);
	}

	
}

void AShooterSamGameMode::HUDDisplayToggle(bool bHUDVisible)
{
	if (bHUDVisible)
	{
		HUDWidget->CountDown->SetVisibility(ESlateVisibility::Visible);

		HUDWidget->ReplayButton->SetVisibility(ESlateVisibility::Visible);

		HUDWidget->ExitButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HUDWidget->CountDown->SetVisibility(ESlateVisibility::Hidden);

		HUDWidget->ReplayButton->SetVisibility(ESlateVisibility::Hidden);

		HUDWidget->ExitButton->SetVisibility(ESlateVisibility::Hidden);
	}
	
}
