// Fill out your copyright notice in the Description page of Project Settings.


#include "MTLGameState.h"

#include "Kismet/GameplayStatics.h"

void AMTLGameState::SpawnTokens()
{
    for (int32 i = 0; i < PlayingField.Num(); i++)
    {
        auto& Row = PlayingField[i];
        for (int32 j = 0; j < Row.Num(); j++)
        {
            if (GetWorld() != nullptr)
            {
                FRotator Rotation(0.f, 180.f, 0.f);
                FTransform Transform(Rotation, PlayingFieldLocations[i][j]);
                AGameToken* SpawnedGameToken = Cast<AGameToken>(
                    UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AGameToken::StaticClass(), Transform));
                if (SpawnedGameToken != nullptr)
                {
                    SpawnedGameToken->Init(i, j);
                    UGameplayStatics::FinishSpawningActor(SpawnedGameToken, Transform);
                    Row[j] = SpawnedGameToken;
                }
                else
                {
                    GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Could not spawn Token!"));
                }
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("UWorld is not spawned yet!"));
            }
        }
    }
}

AMTLGameState::AMTLGameState()
{
    PlayingField.SetNum(10);
    PlayingFieldLocations.SetNum(10);

    for (int32 i = 0; i < PlayingField.Num(); i++)
    {
        auto& Row = PlayingField[i];
        Row.SetNum(8);
        PlayingFieldLocations[i].SetNum(8);

        for (int32 j = 0; j < Row.Num(); j++)
        {
            const bool bIsEvenCol = j % 2 == 0;
            const float y = 100.f * j - (262.5f + 25.f * j);
            const float z = 87.f * i - (400.f + (bIsEvenCol ? 43.5f : 0.f));
            const FVector Location(0.f, y, z);
            PlayingFieldLocations[i][j] = Location;
        }
    }
}

void AMTLGameState::BeginPlay()
{
    SpawnTokens();
}
