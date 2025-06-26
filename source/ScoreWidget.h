// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOXHITER_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	

   public:
	UFUNCTION(BlueprintCallable)
	void UpdateScore(int32 NewScore);

	protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText;
};