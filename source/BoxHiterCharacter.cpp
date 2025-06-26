#include "BoxHiterCharacter.h"
#include "BoxHiterProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "JsonBoxActor.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.h"
#include "DrawDebugHelpers.h" 

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ABoxHiterCharacter::ABoxHiterCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void ABoxHiterCharacter::BeginPlay()
{
	Super::BeginPlay();

	

	APlayerController* PController = Cast<APlayerController>(GetController());
	if (PController)
	{
		PController->bShowMouseCursor = false;
    	PController->SetInputMode(FInputModeGameOnly());
		if (ULocalPlayer* LPlayer = PController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (DefaultMappingContext)
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	}
				if (CrosshairWidgetClass)
			{
			UUserWidget* Crosshair = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
			if (Crosshair)
			{
			Crosshair->AddToViewport();
			}
			}


			if (ScoreWidgetClass)
			{
			ScoreWidgetInstance = CreateWidget<UScoreWidget>(GetWorld(), ScoreWidgetClass);
			if (ScoreWidgetInstance)
			{
				ScoreWidgetInstance->AddToViewport();
				ScoreWidgetInstance->UpdateScore(CurrentScore);
	}
}


}

void ABoxHiterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABoxHiterCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABoxHiterCharacter::Look);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ABoxHiterCharacter::Fire);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find Enhanced Input Component."), *GetNameSafe(this));
	}
}

void ABoxHiterCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ABoxHiterCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABoxHiterCharacter::Fire()
{
	FVector Start;
	FRotator Rotation;
	GetController()->GetPlayerViewPoint(Start, Rotation);
	FVector End = Start + Rotation.Vector() * 10000.0f;

	
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.0f);

	FHitResult Hit;
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Param))
	{
		if (AJsonBoxActor* Box = Cast<AJsonBoxActor>(Hit.GetActor()))
		{
			Box->Hit();
			UE_LOG(LogTemp, Warning, TEXT("Hit box: %s"), *Box->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit something else: %s"), *Hit.GetActor()->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing was hit."));
	}
}
void ABoxHiterCharacter:: AddScore(int32 Points){
	CurrentScore += Points;
	ScoreWidgetInstance ->UpdateScore(CurrentScore);
}

void ABoxHiterCharacter::ShowWinScreen(){
	if(WinScreenClass && !WinScreenWidget){
		WinScreenWidget = CreateWidget<UUserWidget>(GetWorld(), WinScreenClass);
		if (WinScreenWidget)
        {
            WinScreenWidget->AddToViewport();
			
			// mouse cursor
			APlayerController* PC = Cast<APlayerController>(GetController());
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }

	}

}
}
	void ABoxHiterCharacter:: BoxDestroyed(){
	BoxRemaining--;
   	if (BoxRemaining <= 0)
    {
        ShowWinScreen();
    }
}

