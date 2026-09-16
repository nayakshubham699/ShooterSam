// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShooterSamPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "ShooterSam.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "ShooterSamGameMode.h"

void AShooterSamPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogShooterSam, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}

	/*
	HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();

		//HUDWidget->HealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
	*/
}

void AShooterSamPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AShooterSamPlayerController::TogglePause);
	}
}

bool AShooterSamPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AShooterSamPlayerController::TogglePause()
{
	bool bIsPaused = UGameplayStatics::IsGamePaused(GetWorld());
	UE_LOG(LogTemp, Display, TEXT("TogglePause: bIsPaused = %s"), bIsPaused ? TEXT("true") : TEXT("false"));
	if (bIsPaused)
	{
		PauseGame(false);
	}
	else
	{
		PauseGame(true);
	}
}

void AShooterSamPlayerController::PauseGame(bool bPause)
{
	AShooterSamGameMode* ShooterSamGameMode = Cast<AShooterSamGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (ShooterSamGameMode)
	{
		if (bPause)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			ShooterSamGameMode->HUDWidget->SetCountDownText(FString::Printf(TEXT("Paused")));
			ShooterSamGameMode->HUDWidget->ResumeButton->SetVisibility(ESlateVisibility::Visible);

			ShooterSamGameMode->HUDDisplayToggle(true);
			SetInputMode(FInputModeUIOnly());
		}
		else
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);

			ShooterSamGameMode->HUDWidget->ResumeButton->SetVisibility(ESlateVisibility::Hidden);

			ShooterSamGameMode->HUDDisplayToggle(false);
			SetInputMode(FInputModeGameOnly());
		}
	}

	bShowMouseCursor = bPause;
}
