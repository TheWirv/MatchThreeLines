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
            PlayingFieldLocations[i][j] = FVector(0.f, y, z);
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
                    SpawnedGameToken->Init(i, j, PlayingFieldLocations[i][j].Z);
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
    TArray<FFallingGameToken> FallingGameTokens;

    for (auto& SelectedToken : SelectedTokens)
    {
        
        const int32 ColumnIndex = SelectedToken->GetIndex().X;
        const int32 RowIndex = SelectedToken->GetIndex().Y;
        const auto& CurrentColumn = PlayingField[ColumnIndex];
        for (int32 i = RowIndex + 1; i < CurrentColumn.Num(); i++)
        {
            // go through all the tokens in this columns, starting at the one above the current selected token
            AGameToken* CurrentGameToken = CurrentColumn[i];
            if (!SelectedTokens.Contains(CurrentGameToken))
            {
                // obviously only do something, if this token is not one of the selected tokens
                // create an falling token object out of this one, initialized with an amount of 1
                FFallingGameToken CurrentFallingToken = FFallingGameToken(CurrentGameToken, 1);
                if (FallingGameTokens.Num() == 0 || !FallingGameTokens.Contains(CurrentFallingToken))
                {
                    // if the falling tokens array is empty or does not contain the current token, then add it 
                    FallingGameTokens.Add(CurrentFallingToken);
                }
                else
                {
                    // if the array contains the current token, get its index and increment its amount
                    const int32 Index = FallingGameTokens.IndexOfByKey(CurrentFallingToken);
                    FallingGameTokens[Index].Amount += 1;
                }
            }
        }

        // after we have taken note of all the falling tokens, destroy this one
        if (!SelectedToken->Destroy())
        {
            // return false if this token could not be destroyed.
            return false;
        }
    }

    // this is only necessary until respanwning is iplemented:
    for (auto& SelectedToken : SelectedTokens)
    {
        PlayingField[SelectedToken->GetIndex().X].RemoveSingle(SelectedToken);
    }

    if (FallingGameTokens.Num() > 0)
    {
        // if we have tokens that are supposed to fall, update their respective indeces and locations
        for (auto& FallingToken : FallingGameTokens)
        {
            const float NewLocationZ = FallingToken.Token->GetLocationZ();
            FIntPoint NewIndex = FallingToken.Token->GetIndex();
            NewIndex.Y = NewIndex.Y - FallingToken.Amount;
            FallingToken.Token->SetLocationZ(NewLocationZ - VERTICAL_OFFSET * FallingToken.Amount);
            FallingToken.Token->SetIndex(NewIndex);
            FallingToken.Token->SetIsFallingDown(true);
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
