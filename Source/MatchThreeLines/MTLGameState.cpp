// Fill out your copyright notice in the Description page of Project Settings.

#include "MTLGameState.h"

#include "Kismet/GameplayStatics.h"

// private functions
void AMTLGameState::InitPlayingField(const AMTLGameMode* GameMode)
{
    AmountOfRemainingTurns = GameMode->GetMaxAmountOfTurns();
    PlayingField.SetNum(GameMode->GetAmountOfColumns());
    PlayingFieldLocations.SetNum(GameMode->GetAmountOfColumns());

    for (int32 i = 0; i < PlayingField.Num(); i++)
    {
        auto& Column = PlayingField[i];
        Column.SetNum(GameMode->GetAmountOfRows());
        PlayingFieldLocations[i].SetNum(GameMode->GetAmountOfRows());

        for (int32 j = 0; j < Column.Num(); j++)
        {
            const bool bIsEvenCol = i % 2 == 0;
            const float y = HORIZONTAL_OFFSET * i + 500;
            const float z = (VERTICAL_OFFSET * j + (bIsEvenCol ? 0.f : 43.25f)) + 430;
            const FVector Location(0.f, y, z);
            PlayingFieldLocations[i][j] = Location;
        }
    }

    SpawnTokens();
}

void AMTLGameState::SpawnTokens()
{
    for (int32 i = 0; i < PlayingField.Num(); i++)
    {
        auto& Column = PlayingField[i];
        for (int32 j = 0; j < Column.Num(); j++)
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
                    Column[j] = SpawnedGameToken;
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
    // FGenericPlatformMisc::RequestExit(false);
}

// public functions
void AMTLGameState::DecrementAmountOfRemainingTurns()
{
    AmountOfRemainingTurns -= 1;
    if (AmountOfRemainingTurns == 0)
    {
        EndGame();
    }
}

bool AMTLGameState::DestroyTokens(TArray<AGameToken*> SelectedTokens)
{
    for (auto& Token : SelectedTokens)
    {
        if (!Token->Destroy())
        {
            // return false if this token could not be destroyed.
            return false;
        }
    }

    return true;
};

// protected functions
void AMTLGameState::BeginPlay()
{
    Super::BeginPlay();

    const AMTLGameMode* GameMode = Cast<AMTLGameMode>(GetDefaultGameMode());
    if (GameMode != nullptr)
    {
        InitPlayingField(GameMode);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get GameMode!"));
    }
}
