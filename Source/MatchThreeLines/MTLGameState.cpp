// Fill out your copyright notice in the Description page of Project Settings.

#include "MTLGameState.h"
#include "MTLGameMode.h"

#include "Kismet/GameplayStatics.h"

// private functions
void AMTLGameState::SpawnTokens()
{
    for (int32 i = 0; i < PlayingField.Num(); i++)
    {
        auto& Row = PlayingField[i];
        for (int32 j = 0; j < Row.Num(); j++)
        {
            if (GetWorld() != nullptr)
            {
                FTransform Transform(PlayingFieldLocations[i][j]);
                AGameToken* SpawnedGameToken = Cast<AGameToken>(
                    UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AGameToken::StaticClass(), Transform));
                if (SpawnedGameToken != nullptr)
                {
                    SpawnedGameToken->Init(i, j, PlayingFieldLocations[i][j]);
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

void AMTLGameState::EndGame()
{
    GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Green, TEXT("The game is over!"));
}

// public functions
AMTLGameState::AMTLGameState()
{
    AmountOfRemainingTurns = 0;
    PlayingField.SetNum(11);
    PlayingFieldLocations.SetNum(11);

    for (int32 i = 0; i < PlayingField.Num(); i++)
    {
        auto& Row = PlayingField[i];
        Row.SetNum(7);
        PlayingFieldLocations[i].SetNum(7);

        for (int32 j = 0; j < Row.Num(); j++)
        {
            const bool bIsEvenCol = j % 2 == 0;
            const float y = 75.f * j + 500;
            const float z = (86.5f * i + (bIsEvenCol ? 0.f : 43.25f)) + 430;
            const FVector Location(0.f, y, z);
            PlayingFieldLocations[i][j] = Location;
        }
    }
}

void AMTLGameState::DecrementAmountOfRemainingTurns()
{
    AmountOfRemainingTurns -= 1;
    if (AmountOfRemainingTurns == 0)
    {
        EndGame();
    }
};

// protected functions
void AMTLGameState::BeginPlay()
{
    Super::BeginPlay();

    SpawnTokens();
    const AMTLGameMode* GameMode = Cast<AMTLGameMode>(GetDefaultGameMode());
    if (GameMode != nullptr)
    {
        AmountOfRemainingTurns = GameMode->GetMaxAmountOfTurns();
    }
}
