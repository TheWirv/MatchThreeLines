// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MTLPlayerController.generated.h"

/**
 *
 */
UCLASS()
class MATCHTHREELINES_API AMTLPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AMTLPlayerController();

protected:
    /**
    * Called to bind functionality to input
    */
    virtual void SetupInputComponent() override;

    virtual void Tick(float DeltaSeconds) override;
};
