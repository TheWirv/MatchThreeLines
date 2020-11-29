// Fill out your copyright notice in the Description page of Project Settings.


#include "MTLPlayerController.h"
#include "MTLPlayerState.h"
#include "GameToken.h"

// private functions
void AMTLPlayerController::OnMouseClicked()
{
    AMTLPlayerState* MTLPlayerState = GetPlayerState<AMTLPlayerState>();    

    if (MTLPlayerState != nullptr)
    {
        AGameToken* CurrentlyHoveredGameToken = MTLPlayerState->GetHoveredOverGameToken();
        if (CurrentlyHoveredGameToken != nullptr)
        {
            MTLPlayerState->SetIsSelecting(true);
            MTLPlayerState->AddTokenToSelected(CurrentlyHoveredGameToken);
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}

void AMTLPlayerController::OnMouseReleased()
{
    AMTLPlayerState* MTLPlayerState = GetPlayerState<AMTLPlayerState>();  

    if (MTLPlayerState != nullptr)
    {
        MTLPlayerState->EndTurn();
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}

// public functions
AMTLPlayerController::AMTLPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

// protected functions
void AMTLPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("MouseClick", IE_Pressed, this, &AMTLPlayerController::OnMouseClicked);
    InputComponent->BindAction("MouseClick", IE_Released, this, &AMTLPlayerController::OnMouseReleased);
}

void AMTLPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AMTLPlayerState* MTLPlayerState = GetPlayerState<AMTLPlayerState>();
    if (MTLPlayerState != nullptr)
    {
        const AGameToken* CurrentlyHoveredGameToken = MTLPlayerState->GetHoveredOverGameToken();
        const bool bHasSelectionStarted = MTLPlayerState->IsSelecting();
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}
