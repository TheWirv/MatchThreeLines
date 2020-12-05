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

    bool bIsSelecting;

    /** The GameToken that the player is currently hovering over with their mouse cursor */
    UPROPERTY()
    AGameToken* HoveredOverGameToken;

    /** Holds the "selected tokens" as soon as the player presses the left mouse button while hovering over a GameToken */
    UPROPERTY()
    TArray<AGameToken*> SelectedTokens;

public:
    bool IsSelecting() const;

    AGameToken* GetHoveredOverGameToken() const;

    TArray<AGameToken*> GetSelectedTokens() const;

    void SetIsSelecting(const bool bInIsSelecting);

    void SetHoveredOverGameToken(AGameToken* InHoveredOverGameToken);

    void AddTokenToSelected(AGameToken* Token);

    /** Destroys all selected GameTokens, if more than three have been selected; then decrements AmountOfRemainingTurns */
    void EndTurn();
};

// Getters
FORCEINLINE bool AMTLPlayerState::IsSelecting() const { return bIsSelecting; }

FORCEINLINE AGameToken* AMTLPlayerState::GetHoveredOverGameToken() const { return HoveredOverGameToken; }

FORCEINLINE TArray<AGameToken*> AMTLPlayerState::GetSelectedTokens() const { return SelectedTokens; }

// Setters
FORCEINLINE void AMTLPlayerState::SetIsSelecting(const bool bInIsSelecting) { bIsSelecting = bInIsSelecting; }

FORCEINLINE void AMTLPlayerState::SetHoveredOverGameToken(AGameToken* InHoveredOverGameToken)
{
    HoveredOverGameToken = InHoveredOverGameToken;
}
