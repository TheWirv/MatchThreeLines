// Fill out your copyright notice in the Description page of Project Settings.

#include "MTLGameState.h"

#include "Kismet/GameplayStatics.h"

// private functions
void AMTLGameState::InitPlayingField(const AMTLGameMode* GameMode)
{
    AmountOfRemainingTurns = GameMode->GetMaxAmountOfTurns();
    PlayingField.SetNum(GameMode->GetAmountOfColumns());

    for (int32 i = 0; i < PlayingField.Num(); i++)
    {
        auto& Column = PlayingField[i];
        Column.SetNum(GameMode->GetAmountOfRows());

        for (int32 j = 0; j < Column.Num(); j++)
        {
            if (GetWorld() != nullptr)
            {
                if (!SpawnGameToken(i, j))
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

FVector AMTLGameState::CalculateGameTokenLocation(const int32 ColumnIndex, const int32 RowIndex) const
{
    const bool bIsEvenCol = ColumnIndex % 2 == 0;
    const float y = HORIZONTAL_OFFSET * ColumnIndex + 500;
    const float z = (VERTICAL_OFFSET * RowIndex + (bIsEvenCol ? 0.f : 43.25f)) + 430;
    return FVector(0.f, y, z);
}

bool AMTLGameState::SpawnGameToken(const int32 ColumnIndex, const int32 RowIndex, const bool bSpawnOutsideViewport)
{
    FVector GameTokenLocation = CalculateGameTokenLocation(ColumnIndex, RowIndex);
    const float GameTokenLocationZ = GameTokenLocation.Z;
    if (bSpawnOutsideViewport)
    {
        GameTokenLocation.Z = 1550.f;
    }
    const FTransform Transform(GameTokenLocation);
    AGameToken* SpawnedGameToken = Cast<AGameToken>(
        UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AGameToken::StaticClass(), Transform));
    if (SpawnedGameToken != nullptr)
    {
        SpawnedGameToken->Init(ColumnIndex, RowIndex, GameTokenLocationZ, bSpawnOutsideViewport);
        UGameplayStatics::FinishSpawningActor(SpawnedGameToken, Transform);
        PlayingField[ColumnIndex][RowIndex] = SpawnedGameToken;
        return true;
    }

    return false;
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
    TArray<FRespawningGameTokenCounter> RespawningGameTokenCounters;

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
                FFallingGameToken CurrentFallingToken(CurrentGameToken, 1);
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

        FRespawningGameTokenCounter RespawningGameTokenCounter(ColumnIndex, 1);
        if (RespawningGameTokenCounters.Num() == 0 || !RespawningGameTokenCounters.Contains(RespawningGameTokenCounter))
        {
            RespawningGameTokenCounters.Add(RespawningGameTokenCounter);
        }
        else
        {
            const int32 Index = RespawningGameTokenCounters.IndexOfByKey(RespawningGameTokenCounter);
            RespawningGameTokenCounters[Index].Amount += 1;
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

    if (RespawningGameTokenCounters.Num() > 0)
    {
        const AMTLGameMode* GameMode = GetDefaultGameMode<AMTLGameMode>();
        if (GameMode != nullptr)
        {
            for (auto& RespawningGameTokenCounter : RespawningGameTokenCounters)
            {
                for (int32 RowIndex = GameMode->GetAmountOfRows() - RespawningGameTokenCounter.Amount;
                     RowIndex < GameMode->GetAmountOfRows();
                     RowIndex++)
                {
                    PlayingField[RespawningGameTokenCounter.ColumnIndex].SetNum(GameMode->GetAmountOfRows());
                    if (GetWorld() != nullptr)
                    {
                        if (!SpawnGameToken(RespawningGameTokenCounter.ColumnIndex, RowIndex, true))
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
        else
        {
            GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get GameMode!"));
        }
    }

    return true;
};

// protected functions
void AMTLGameState::BeginPlay()
{
    Super::BeginPlay();

    const AMTLGameMode* GameMode = GetDefaultGameMode<AMTLGameMode>();
    if (GameMode != nullptr)
    {
        InitPlayingField(GameMode);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get GameMode!"));
    }
}
