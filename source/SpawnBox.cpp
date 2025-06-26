// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBox.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"  
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "BoxHiterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "JsonBoxActor.h"

// Sets default values
ASpawnBox::ASpawnBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpawnBox::BeginPlay()
{
	Super::BeginPlay();
	 TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ASpawnBox::OnResponseReceived);
    Request->SetURL(TEXT("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json"));
    Request->SetVerb(TEXT("GET"));
    Request->ProcessRequest();
	
}

// Called every frame
void ASpawnBox::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid()) return;

    FString JsonStr = Response->GetContentAsString();
    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonStr);

    if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid()) return;

    // Read box types
    TMap<FString, FLinearColor> TypeColors;
    TMap<FString, int32> TypeHealths;
    TMap<FString, int32> TypeScores;

    for (auto& TypeValue : RootObject->GetArrayField(TEXT("types")))
    {
        TSharedPtr<FJsonObject> Type = TypeValue->AsObject();
        FString Name = Type->GetStringField(TEXT("name"));

        auto ColorArr = Type->GetArrayField(TEXT("color"));
        FLinearColor Color(ColorArr[0]->AsNumber() / 255.0f, ColorArr[1]->AsNumber() / 255.0f, ColorArr[2]->AsNumber() / 255.0f);

        TypeColors.Add(Name, Color);
        TypeHealths.Add(Name, Type->GetIntegerField(TEXT("health")));
        TypeScores.Add(Name, Type->GetIntegerField(TEXT("score")));
    }
     int32 BoxCount = 0;

	 for (auto& ObjValue : RootObject->GetArrayField(TEXT("objects")))
    {
        TSharedPtr<FJsonObject> Obj = ObjValue->AsObject();
        FString Type = Obj->GetStringField(TEXT("type"));
        auto Transform = Obj->GetObjectField(TEXT("transform"));

        auto Loc = Transform->GetArrayField(TEXT("location"));
        auto Rot = Transform->GetArrayField(TEXT("rotation"));
        auto Scale = Transform->GetArrayField(TEXT("scale"));

        FVector Location(Loc[0]->AsNumber(), Loc[1]->AsNumber(), Loc[2]->AsNumber());
        FRotator Rotation(Rot[0]->AsNumber(), Rot[1]->AsNumber(), Rot[2]->AsNumber());
        FVector Scale3D(Scale[0]->AsNumber(), Scale[1]->AsNumber(), Scale[2]->AsNumber());

        FActorSpawnParameters Params;
		AJsonBoxActor* Box = GetWorld()->SpawnActor<AJsonBoxActor>(AJsonBoxActor::StaticClass(), Location, Rotation, Params);
        if (Box)
        {
            BoxCount++;
            Box->SetActorScale3D(Scale3D);
             if(ABoxHiterCharacter*Player = Cast<ABoxHiterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0))){
                Box->SetOwner(Player);
            }
            Box->BoxSetup(TypeColors[Type], TypeHealths[Type], TypeScores[Type]);
           
        }
	}
    ABoxHiterCharacter* Player = Cast<ABoxHiterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (Player)
    {
        Player->BoxRemaining = BoxCount;
        UE_LOG(LogTemp, Warning, TEXT("BoxRemaining set to: %d"), BoxCount);
    }
}


