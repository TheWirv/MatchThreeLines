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
    int32 AmountOfColumns;
    int32 AmountOfRows;

public:
    AMTLGameMode();

    int32 GetMaxAmountOfTurns() const;

    int32 GetAmountOfColumns() const;

    int32 GetAmountOfRows() const;
};

// Getters
FORCEINLINE int32 AMTLGameMode::GetMaxAmountOfTurns() const { return MaxAmountOfTurns; }

FORCEINLINE int32 AMTLGameMode::GetAmountOfColumns() const { return AmountOfColumns; }

FORCEINLINE int32 AMTLGameMode::GetAmountOfRows() const { return AmountOfRows; }
