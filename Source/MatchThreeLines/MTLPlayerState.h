// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameToken.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MTLPlayerState.generated.h"

/**
 *
 */
UCLASS()
class MATCHTHREELINES_API AMTLPlayerState : public APlayerState
{
    GENERATED_BODY()

    bool bHasSelectionStarted;

    UPROPERTY()
    AGameToken* CurrentlyHoveredGameToken;

    UPROPERTY()
    TArray<const AGameToken*> SelectedTokens;

public:
    bool HasSelectionStarted() const;

    AGameToken* GetCurrentlyHoveredGameToken() const;

    TArray<const AGameToken*> GetSelectedTokens() const;

    void SetSelectionStarted(bool bSelectionStarted);

    void SetCurrentlyHoveredGameToken(AGameToken* HoveredGameToken);

    void AddTokenToSelected(const AGameToken* Token);

    void EndTurn();
};

// Getters
FORCEINLINE bool AMTLPlayerState::HasSelectionStarted() const { return bHasSelectionStarted; }

FORCEINLINE AGameToken* AMTLPlayerState::GetCurrentlyHoveredGameToken() const { return CurrentlyHoveredGameToken; }

FORCEINLINE TArray<const AGameToken*> AMTLPlayerState::GetSelectedTokens() const { return SelectedTokens; }

// Setters
FORCEINLINE void AMTLPlayerState::SetSelectionStarted(const bool bSelectionStarted)
{
    this->bHasSelectionStarted = bSelectionStarted;
}

FORCEINLINE void AMTLPlayerState::SetCurrentlyHoveredGameToken(AGameToken* HoveredGameToken)
{
    CurrentlyHoveredGameToken = HoveredGameToken;
}

FORCEINLINE void AMTLPlayerState::AddTokenToSelected(const AGameToken* Token)
{
    SelectedTokens.AddUnique(Token);
}
