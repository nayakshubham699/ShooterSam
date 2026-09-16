// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERSAM_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* CountDown;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* EnemyCount;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* ReplayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* ExitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* ResumeButton;

	virtual void NativeConstruct() override;

	void SetHealthBarPercent(float NewPercent);

	void SetCountDownText(FString CountDownText);

	void SetEnemyCountText(FString EnemyCountText);

	UFUNCTION()
	void OnReplayButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnResumeButtonClicked();
};
