// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTLGameMode.h"
#include "MTLPlayerController.h"
#include "MTLGameState.h"
#include "MTLPlayerState.h"

AMTLGameMode::AMTLGameMode()
{
    // Set up custom PlayerController
    PlayerControllerClass = AMTLPlayerController::StaticClass();

    // Set up custom GameState
    GameStateClass = AMTLGameState::StaticClass();

    // Set up custom PlayerState
    PlayerStateClass = AMTLPlayerState::StaticClass();
}
