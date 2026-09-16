// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ShooterSamPlayerController.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ReplayButton)
	{
		ReplayButton->OnClicked.AddDynamic(this, &UHUDWidget::OnReplayButtonClicked);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UHUDWidget::OnExitButtonClicked);
	}

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UHUDWidget::OnResumeButtonClicked);
	}
}

void UHUDWidget::SetHealthBarPercent(float NewPercent)
{
	if (NewPercent >= 0.0f && NewPercent <= 1.0f)
	{
		HealthBar->SetPercent(NewPercent);
	}
}

void UHUDWidget::SetCountDownText(FString CountDownText)
{
	if (CountDown)
	{
		CountDown->SetText(FText::FromString(CountDownText));
	}
}

void UHUDWidget::SetEnemyCountText(FString EnemyCountText)
{
	if (EnemyCount)
	{
		EnemyCount->SetText(FText::FromString(EnemyCountText));
	}
}

void UHUDWidget::OnReplayButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

	AShooterSamPlayerController* ShooterSamPlayerController = Cast<AShooterSamPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (ShooterSamPlayerController)
	{
		ShooterSamPlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void UHUDWidget::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UHUDWidget::OnResumeButtonClicked()
{
	AShooterSamPlayerController* ShooterSamPlayerController = Cast<AShooterSamPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (ShooterSamPlayerController)
	{
		ShooterSamPlayerController->PauseGame(false);

		UE_LOG(LogTemp, Display, TEXT("Game Resumed Button clicked!"));
	}

}
