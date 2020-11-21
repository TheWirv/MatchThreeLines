// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "GameToken.h"

#include "MTLGameState.generated.h"

/**
 *
 */
UCLASS()
class MATCHTHREELINES_API AMTLGameState : public AGameStateBase
{
    GENERATED_BODY()

    TArray<TArray<AGameToken*>> PlayingField;
    TArray<TArray<FVector>> PlayingFieldLocations;

    void SpawnTokens();

public:
    AMTLGameState();

protected:
    virtual void BeginPlay() override;
};
