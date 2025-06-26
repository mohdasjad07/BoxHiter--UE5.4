// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoxHiterGameMode.h"
#include "BoxHiterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABoxHiterGameMode::ABoxHiterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
