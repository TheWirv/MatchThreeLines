// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "MTLGameMode.generated.h"

/**
 *
 */
UCLASS()
class MATCHTHREELINES_API AMTLGameMode : public AGameModeBase
{
    GENERATED_BODY()

    int32 MaxAmountOfTurns;

public:
    AMTLGameMode();

    int32 GetMaxAmountOfTurns() const;
};

// Getters
FORCEINLINE int32 AMTLGameMode::GetMaxAmountOfTurns() const { return MaxAmountOfTurns; }
