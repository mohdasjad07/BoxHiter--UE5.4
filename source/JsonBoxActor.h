// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JsonBoxActor.generated.h"

UCLASS()
class BOXHITER_API AJsonBoxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJsonBoxActor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Box;
	void BoxSetup(FLinearColor color,int32 InHealth,int32 InScore );

	void Hit();

	private:
	int32 Health;
	int32 Score;
	

};
