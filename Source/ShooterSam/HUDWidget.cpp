// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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
}

void UHUDWidget::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
