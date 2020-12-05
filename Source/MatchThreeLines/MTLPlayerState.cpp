// Fill out your copyright notice in the Description page of Project Settings.


#include "MTLPlayerState.h"
#include "MTLPlayerController.h"
#include "MTLGameState.h"
#include "MTLGameMode.h"

// private fuctions
void AMTLPlayerState::IncreaseScore()
{
    // Since all selected Tokens must be of the same type, the first item suffices for this use case
    const AGameToken* FirstSelectedToken = SelectedTokens[0];
    SetScore(GetScore() + FirstSelectedToken->GetScoreValue() * SelectedTokens.Num());
    // After actually increasing the score, dispatch the event to update the UI
    OnUpdateScoreDelegate.Broadcast();
}

void AMTLPlayerState::DecrementAmountOfRemainingTurns()
{
    // Decrement AmountOfRemainingTurns by 1, then dispatch the event to update the UI
    AmountOfRemainingTurns -= 1;
    OnUpdateRemainingTurnsDelegate.Broadcast();

    if (AmountOfRemainingTurns == 0)
    {
        // If this has been the final turn, get the player controller and end the game
        AMTLPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<AMTLPlayerController>();
        if (PlayerController != nullptr)
        {
            PlayerController->EndGame();
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get GameMode!"));
        }
    }
}

// public functions
void AMTLPlayerState::AddTokenToSelected(AGameToken* Token)
{
    if (SelectedTokens.Num() > 0)
    {
        // If the array already has some Tokens in it, check whether this one has already been added
        if (!SelectedTokens.Contains(Token))
        {
            const AGameToken* LastToken = Cast<AGameToken>(SelectedTokens.Last());
            if (Token->IsNeighbor(LastToken) && Token->IsOfSameType(LastToken))
            {
                // If the Token has not been added yet, and is a neighbor to and of the same type
                // as the last selected token, it can be selected
                Token->MarkSelected(true);
                SelectedTokens.Add(Token);
            }
        }
        else
        {
            // If this token has already been added, get its index and resize the array to index + 1 
            const int32 IndexInSelected = SelectedTokens.IndexOfByKey(Token);
            for (int32 i = IndexInSelected + 1; i < SelectedTokens.Num(); i++)
            {
                SelectedTokens[i]->MarkSelected(false);
            }
            SelectedTokens.SetNum(IndexInSelected + 1);
        }
    }
    else
    {
        // If the array is empty, just add this Token
        Token->MarkSelected(true);
        SelectedTokens.Add(Token);
    }
}

void AMTLPlayerState::Init()
{
    AGameStateBase* GameStateBase = GetWorld()->GetGameState();
    if (GameStateBase != nullptr)
    {
        const AMTLGameMode* GameMode = GameStateBase->GetDefaultGameMode<AMTLGameMode>();
        if (GameMode != nullptr)
        {
            SetScore(0.f);
            AmountOfRemainingTurns = GameMode->GetMaxAmountOfTurns();
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get GameMode!"));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get GameState!"));
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
                IncreaseScore();
                DecrementAmountOfRemainingTurns();
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Red,
                                                 TEXT("Couldn't destroy all GameTokens!"));
            }
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get GameState!"));
        }
    }
    else
    {
        for (auto Token : SelectedTokens)
        {
            Token->MarkSelected(false);
        }
    }
    SelectedTokens.Empty();
}

void AMTLPlayerState::BeginPlay()
{
    Super::BeginPlay();

    Init();
}
