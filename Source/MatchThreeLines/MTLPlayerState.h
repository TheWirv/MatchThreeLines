// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameToken.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MTLPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateRemainingTurnsDelegate);

/**
 * Custom PlayerState - Manages the following things:
 * - The amount of remaining turns
 * - Whether the player is currently selecting any GameTokens or not
 * - Which GameToken the player is currently hovering over
 * - The actual GameTokens they have selected already (if they are selecting at all)
 */
UCLASS()
class MATCHTHREELINES_API AMTLPlayerState : public APlayerState
{
    GENERATED_BODY()

    int32 AmountOfRemainingTurns;
    bool bIsSelecting;

    /** The GameToken that the player is currently hovering over with their mouse cursor */
    UPROPERTY()
    AGameToken* HoveredOverGameToken;

    /** Holds the "selected tokens" as soon as the player presses the left mouse button while hovering over a GameToken */
    UPROPERTY()
    TArray<AGameToken*> SelectedTokens;

public:
    UFUNCTION(BlueprintCallable, Category = "MTL – UI")
    int32 GetAmountOfRemainingTurns() const;

    /** Decrements AmountOfRemainingTurns by 1, and ends the game if the result is 0 */
    void DecrementAmountOfRemainingTurns();

    /** Event dispatcher to let the UI know about changes regarding AmountOfRemainingTurns */
    UPROPERTY(BlueprintAssignable, Category = "MTL – UI")
    FUpdateRemainingTurnsDelegate OnUpdateRemainingTurnsDelegate;

    bool IsSelecting() const;

    AGameToken* GetHoveredOverGameToken() const;

    TArray<AGameToken*> GetSelectedTokens() const;

    void SetIsSelecting(const bool bInIsSelecting);

    void SetHoveredOverGameToken(AGameToken* InHoveredOverGameToken);

    void AddTokenToSelected(AGameToken* Token);

    /** Destroys all selected GameTokens, if more than three have been selected; then decrements AmountOfRemainingTurns */
    void EndTurn();

protected:
    virtual void BeginPlay() override;
};

// Getters
FORCEINLINE int32 AMTLPlayerState::GetAmountOfRemainingTurns() const { return AmountOfRemainingTurns; };

FORCEINLINE bool AMTLPlayerState::IsSelecting() const { return bIsSelecting; }

FORCEINLINE AGameToken* AMTLPlayerState::GetHoveredOverGameToken() const { return HoveredOverGameToken; }

FORCEINLINE TArray<AGameToken*> AMTLPlayerState::GetSelectedTokens() const { return SelectedTokens; }

// Setters
FORCEINLINE void AMTLPlayerState::SetIsSelecting(const bool bInIsSelecting) { bIsSelecting = bInIsSelecting; }

FORCEINLINE void AMTLPlayerState::SetHoveredOverGameToken(AGameToken* InHoveredOverGameToken)
{
    HoveredOverGameToken = InHoveredOverGameToken;
}
