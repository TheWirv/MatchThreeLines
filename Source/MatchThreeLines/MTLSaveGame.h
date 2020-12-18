// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MTLSaveGame.generated.h"

#define DEFAULT_SAVE_SLOT_NAME "Profile"

/**
 * 
 */
UCLASS()
class MATCHTHREELINES_API UMTLSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UMTLSaveGame();

    UPROPERTY(VisibleAnywhere, Category = "MTL – Saves")
    float HighScore;

    UPROPERTY(VisibleAnywhere, Category = "MTL – Saves")
    FString PlayerName;

    UPROPERTY(VisibleAnywhere, Category = "MTL – Saves")
    FString SaveSlotName;

    UPROPERTY(VisibleAnywhere, Category = "MTL – Saves")
    uint32 UserIndex;
};
