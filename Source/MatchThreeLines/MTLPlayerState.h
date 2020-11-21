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

    void SetSelectionStarted(bool bHasSelectionStarted);

    void SetCurrentlyHoveredGameToken(AGameToken* HoveredGameToken);

    void AddSelectedToken(const AGameToken* GameToken);

    void ResetSelectedTokens();
};

FORCEINLINE bool AMTLPlayerState::HasSelectionStarted() const { return bHasSelectionStarted; };


FORCEINLINE AGameToken* AMTLPlayerState::GetCurrentlyHoveredGameToken() const { return CurrentlyHoveredGameToken; }

FORCEINLINE TArray<const AGameToken*> AMTLPlayerState::GetSelectedTokens() const { return SelectedTokens; };

FORCEINLINE void AMTLPlayerState::SetSelectionStarted(const bool _bHasSelectionStarted)
{
    bHasSelectionStarted = _bHasSelectionStarted;
}

FORCEINLINE void AMTLPlayerState::SetCurrentlyHoveredGameToken(AGameToken* HoveredGameToken)
{
    CurrentlyHoveredGameToken = HoveredGameToken;
}

FORCEINLINE void AMTLPlayerState::AddSelectedToken(const AGameToken* GameToken)
{
    SelectedTokens.AddUnique(GameToken);
};

FORCEINLINE void AMTLPlayerState::ResetSelectedTokens() { SelectedTokens.Empty(); };
