// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameToken.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MTLPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateScoreDelegate);

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
    int32 CurrentRank;

    /** The GameToken that the player is currently hovering over with their mouse cursor */
    UPROPERTY()
    AGameToken* HoveredOverGameToken;

    /** Holds the "selected tokens" as soon as the player presses the left mouse button while hovering over a GameToken */
    UPROPERTY()
    TArray<AGameToken*> SelectedTokens;

    /** Increases the score by the accumulated score amount of all selected GameTokens */
    void IncreaseScore();

    /** Decrements AmountOfRemainingTurns by 1, and ends the game if the result is 0 */
    void DecrementAmountOfRemainingTurns();

protected:
    virtual void BeginPlay() override;

public:
    /** Reset all members to default/initial values */
    void Init();

    /** Destroys all selected GameTokens, if more than three have been selected; then decrements AmountOfRemainingTurns */
    void EndTurn();

    // Getters, setters and property-related delegates
    /** Event dispatcher to let the UI know about changes regarding the player's score */
    UPROPERTY(BlueprintAssignable, Category = "MTL|UI")
    FUpdateScoreDelegate OnUpdateScoreDelegate;

    UFUNCTION(BlueprintCallable, DisplayName = "SetPlayerName", Category = "MTL|UI")
    void BP_SetPlayerName(const FString& InPlayerName);

    UFUNCTION(BlueprintCallable, Category = "MTL|UI")
    int32 GetAmountOfRemainingTurns() const;

    /** Event dispatcher to let the UI know about changes regarding AmountOfRemainingTurns */
    UPROPERTY(BlueprintAssignable, Category = "MTL|UI")
    FUpdateRemainingTurnsDelegate OnUpdateRemainingTurnsDelegate;

    FORCEINLINE bool IsSelecting() const { return bIsSelecting; }

    FORCEINLINE void SetIsSelecting(const bool bInIsSelecting) { bIsSelecting = bInIsSelecting; }

    UFUNCTION(BlueprintCallable, Category = "MTL|UI")
    int32 GetCurrentRank() const;

    FORCEINLINE void SetCurrentRank(const int32 InRank) { CurrentRank = InRank; }

    FORCEINLINE AGameToken* GetHoveredOverGameToken() const { return HoveredOverGameToken; }

    FORCEINLINE void SetHoveredOverGameToken(AGameToken* InHoveredOverGameToken)
    {
        HoveredOverGameToken = InHoveredOverGameToken;
    }

    FORCEINLINE TArray<AGameToken*> GetSelectedTokens() const { return SelectedTokens; }

    void AddTokenToSelected(AGameToken* Token);
};
