// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"
#include "Components/TextBlock.h"

void UScoreWidget::UpdateScore(int32 NewScore){
    if (ScoreText)
    {
        ScoreText->SetText(FText::AsNumber(NewScore));
    }
    
}

