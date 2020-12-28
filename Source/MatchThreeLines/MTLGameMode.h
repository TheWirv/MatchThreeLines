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
    
    // Getters, setters and property-related delegates
    UFUNCTION(BlueprintCallable, Category = "MTL|UI")
    int32 GetMaxAmountOfTurns() const;

    FORCEINLINE int32 GetAmountOfColumns() const { return AmountOfColumns; }

    FORCEINLINE int32 GetAmountOfRows() const { return AmountOfRows; }
};
