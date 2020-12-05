// Copyright Epic Games, Inc. All Rights Reserved.

#include "MTLGameMode.h"
#include "MTLPlayerController.h"
#include "MTLGameState.h"
#include "MTLPlayerState.h"

AMTLGameMode::AMTLGameMode(): MaxAmountOfTurns(5), AmountOfColumns(7), AmountOfRows(11)
{   
    // Set up custom PlayerController
    PlayerControllerClass = AMTLPlayerController::StaticClass();

    // Set up custom GameState
    GameStateClass = AMTLGameState::StaticClass();

    // Set up custom PlayerState
    PlayerStateClass = AMTLPlayerState::StaticClass();
}
