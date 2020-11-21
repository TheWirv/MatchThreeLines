// Fill out your copyright notice in the Description page of Project Settings.


#include "MTLPlayerController.h"
#include "MTLPlayerState.h"
#include "GameToken.h"

// private functions
void AMTLPlayerController::OnMouseDown()
{
    GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Green, TEXT("Pressed left mouse button."));
}

void AMTLPlayerController::OnMouseUp()
{
    GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Green, TEXT("Released left mouse button."));
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

    InputComponent->BindAction("MouseClick", IE_Pressed, this, &AMTLPlayerController::OnMouseDown);
    InputComponent->BindAction("MouseClick", IE_Released, this, &AMTLPlayerController::OnMouseUp);
}

void AMTLPlayerController::Tick(float DeltaSeconds)
{
    AMTLPlayerState* CastedPlayerState = GetPlayerState<AMTLPlayerState>();
    const AGameToken* CurrentlyHoveredGameToken = CastedPlayerState->GetCurrentlyHoveredGameToken();
    const bool bHasSelectionStarted = CastedPlayerState->HasSelectionStarted();

    if (CurrentlyHoveredGameToken == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("No GameToken under Mouse!"));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green,
                                         FString("GameToken under Mouse: ").
                                         Append(CurrentlyHoveredGameToken->GetName()));
    }
    GEngine->AddOnScreenDebugMessage(2, 1.0f, bHasSelectionStarted ? FColor::Green : FColor::Red,
                                     FString("Selection has started: ").
                                     Append(bHasSelectionStarted ? "true" : "false"));
}
