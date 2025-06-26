#include "JsonBoxActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "BoxHiterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AJsonBoxActor::AJsonBoxActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	RootComponent = Box;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube"));
	if (Cube.Succeeded())
	{
		Box->SetStaticMesh(Cube.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FoundMat(TEXT("/Game/M_Box.M_Box"));
	if (FoundMat.Succeeded())
	{
		Box->SetMaterial(0, FoundMat.Object);
	}

	// Enable collision for line trace detection
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetCollisionResponseToAllChannels(ECR_Block);
	Box->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AJsonBoxActor::BoxSetup(FLinearColor color, int32 InHealth, int32 InScore)
{
	Health = InHealth;
	Score = InScore;

	UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/M_Box.M_Box"));

	UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(Material, this);

	if (DynamicMat)
	{
		DynamicMat->SetVectorParameterValue("BoxColor", FLinearColor::Green);

		DynamicMat->SetVectorParameterValue("BoxColor", color);
		Box->SetMaterial(0, DynamicMat);

		UE_LOG(LogTemp, Warning, TEXT("Box color set to: %s"), *color.ToString());
	}
}

void AJsonBoxActor::Hit()
{
	Health--;

	if (Health <= 0)
	{
		
		if  (ABoxHiterCharacter* Player = Cast<ABoxHiterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		Player->AddScore(Score); 
		Player->BoxDestroyed();
	}
		
		Destroy();
	}
}
