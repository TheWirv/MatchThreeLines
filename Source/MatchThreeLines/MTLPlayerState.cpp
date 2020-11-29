// Fill out your copyright notice in the Description page of Project Settings.


#include "MTLPlayerState.h"
#include "MTLGameState.h"

// public functions
void AMTLPlayerState::EndTurn()
{
    SetSelectionStarted(false);
    if (SelectedTokens.Num() >= 3)
    {
        AMTLGameState* GameState = GetWorld()->GetGameState<AMTLGameState>();
        if (GameState != nullptr)
        {
            if (GameState->DestroyTokens(SelectedTokens))
            {
                GameState->DecrementAmountOfRemainingTurns();
                GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, TEXT("You finished your turn!"));
                GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Green,
                                                 FString("You have ").Append(
                                                     FString::FromInt(GameState->GetAmountOfRemainingTurns())).Append(
                                                     " amount of turns left."));
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Red, TEXT("Couldn't destroy all GameTokens!"));
            }
        }
    }
    SelectedTokens.Empty();
}
