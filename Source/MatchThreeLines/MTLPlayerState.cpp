// Fill out your copyright notice in the Description page of Project Settings.


#include "MTLPlayerState.h"
#include "MTLGameState.h"

// public functions
void AMTLPlayerState::AddTokenToSelected(AGameToken* Token)
{
    if (SelectedTokens.Num() > 0)
    {
        // if the array already has some tokens in it, check whether this one has already been added
        if (!SelectedTokens.Contains(Token))
        {
            const AGameToken* LastToken = Cast<AGameToken>(SelectedTokens.Last());
            if (Token->IsNeighbor(LastToken) && Token->IsOfSameType(LastToken))
            {
                // if it has not been added yet, and is a neighbor to and of the same type
                // as the last selected token, add it to the array
                SelectedTokens.Add(Token);
            }
        }
        else
        {
            // if this token has already been added, get its index and resize the array to index + 1 
            const int32 IndexInSelected = SelectedTokens.IndexOfByKey(Token);
            SelectedTokens.SetNum(IndexInSelected + 1);
        }
    }
    else
    {
        // if the array is empty, just add this one
        SelectedTokens.Add(Token);
    }
}

void AMTLPlayerState::EndTurn()
{
    SetIsSelecting(false);
    if (SelectedTokens.Num() >= 3)
    {
        AMTLGameState* GameState = GetWorld()->GetGameState<AMTLGameState>();
        if (GameState != nullptr)
        {
            if (GameState->DestroyTokens(SelectedTokens))
            {
                GameState->DecrementAmountOfRemainingTurns();
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Red,
                                                 TEXT("Couldn't destroy all GameTokens!"));
            }
        }
    }
    SelectedTokens.Empty();
}
