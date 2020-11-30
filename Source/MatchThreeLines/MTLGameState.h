// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "GameToken.h"
#include "MTLGameMode.h"
#include "MTLGameState.generated.h"

struct FFallingGameToken
{
    AGameToken* Token; // Pointer to the actual token
    int32 Amount; // How many units is this token about to fall

    FFallingGameToken(AGameToken* InToken, const int32 InitialAmount): Token(InToken), Amount(InitialAmount)
    {
    }

    bool operator==(const FFallingGameToken& OtherFallingToken) const
    {
        return Token->GetIndex() == OtherFallingToken.Token->GetIndex();
    }
};

struct FRespawningGameTokenCounter
{
    int32 ColumnIndex; // The column's index
    int32 Amount; // How many GameTokens have to be respawned

    FRespawningGameTokenCounter(const int32 InColumnIndex, const int32 InitialAmount): ColumnIndex(InColumnIndex),
        Amount(InitialAmount)
    {
    }

    bool operator==(const FRespawningGameTokenCounter& Other) const
    {
        return ColumnIndex == Other.ColumnIndex;
    }
};

/**
 *
 */
UCLASS()
class MATCHTHREELINES_API AMTLGameState : public AGameStateBase
{
    GENERATED_BODY()

    int32 AmountOfRemainingTurns;
    TArray<TArray<AGameToken*>> PlayingField;

    void InitPlayingField(const AMTLGameMode* GameMode);

    FVector CalculateGameTokenLocation(const int32 ColumnIndex, const int32 RowIndex) const;

    bool SpawnGameToken(const int32 ColumnIndex, const int32 RowIndex, const bool SpawnOutsideViewport = false);

    void EndGame();

public:
    int32 GetAmountOfRemainingTurns() const;

    void DecrementAmountOfRemainingTurns();

    bool DestroyTokens(TArray<AGameToken*> SelectedTokens);

protected:
    virtual void BeginPlay() override;
};

// Getters
FORCEINLINE int32 AMTLGameState::GetAmountOfRemainingTurns() const { return AmountOfRemainingTurns; };
