// Fill out your copyright notice in the Description page of Project Settings.


#include "MTLPlayerController.h"
#include "MTLPlayerState.h"
#include "GameToken.h"

// private functions

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
}

void AMTLPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AMTLPlayerState* MTLPlayerState = GetPlayerState<AMTLPlayerState>();
    if (MTLPlayerState != nullptr)
    {
        const AGameToken* CurrentlyHoveredGameToken = MTLPlayerState->GetCurrentlyHoveredGameToken();
        const bool bHasSelectionStarted = MTLPlayerState->HasSelectionStarted();
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}
