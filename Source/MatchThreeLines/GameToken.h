// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameToken.generated.h"

#define VERTICAL_OFFSET 86.5f
#define HORIZONTAL_OFFSET 75.f

enum ETokenType
{
    Red,
    Blue,
    Green,
    Yellow,
    White
};

/**
 * GameToken Actor
 * - Main interactable item in the game
 * - The playing field is made up exclusively of these GameTokens
 * - Has a type, which determines its color
 * - Has a score value that is (as of now) uniformly equal among all GameTokens
 * - Can be selected, which gives it a highlighted border
 * - Can be animated to fall down
 */
UCLASS()
class MATCHTHREELINES_API AGameToken : public AActor
{
    GENERATED_BODY()

    bool bIsSelected;
    bool bIsFallingDown;

    float LocationZ;
    
    FIntPoint Index;
    float ScoreValue;
    ETokenType TokenType;

    /** Creates a dynamic material instance and assigns it to the Token's static mesh */
    void AssignMaterialInstanceToMesh();

    /**
    * Gets the color for this token's material instance.
    * @returns This token's material instance color.
    */
    FColor GetMaterialInstanceColor() const
    {
        switch (TokenType)
        {
        case Red:
            return FColor::Red;
        case Blue:
            return FColor::Blue;
        case Green:
            return FColor::Green;
        case Yellow:
            return FColor::Yellow;
        default:
            return FColor::White;
        }
    }

public:
    AGameToken();

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    class UMaterial* Material;

    UPROPERTY(VisibleAnywhere)
    class UMaterialInstanceDynamic* MaterialInstance;

    /** Initializes the GameToken; basically acts kind of as a deferred constructor */
    void Init(const int32 Column, const int32 Row, const float InitialLocationZ, const bool bInIsFallingDown = false);

    /** Checks whether a GameToken is neighbor to another one */
    bool IsNeighbor(const AGameToken* Other) const;

    float GetLocationZ() const;

    FIntPoint GetIndex() const;

    float GetScoreValue() const;

    void SetIsFallingDown(const bool bInIsFallingDown);

    void SetLocationZ(const float NewLocationZ);

    void SetIndex(const FIntPoint NewIndex);

    bool IsOfSameType(const AGameToken* OtherToken) const;

    bool operator==(const AGameToken* OtherToken) const;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnBeginMouseOver(AActor* TouchedActor);

    UFUNCTION()
    void OnEndMouseOver(AActor* TouchedActor);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};

// Getters
FORCEINLINE float AGameToken::GetLocationZ() const { return LocationZ; };

FORCEINLINE FIntPoint AGameToken::GetIndex() const { return Index; };

FORCEINLINE float AGameToken::GetScoreValue() const { return ScoreValue; };

// Setters
FORCEINLINE void AGameToken::SetIsFallingDown(const bool bInIsFallingDown) { bIsFallingDown = bInIsFallingDown; };

FORCEINLINE void AGameToken::SetLocationZ(const float NewLocationZ) { LocationZ = NewLocationZ; };

FORCEINLINE void AGameToken::SetIndex(const FIntPoint NewIndex) { Index = NewIndex; };

// Other inline functions
FORCEINLINE bool AGameToken::IsOfSameType(const AGameToken* OtherToken) const
{
    return TokenType == OtherToken->TokenType;
};

FORCEINLINE bool AGameToken::operator==(const AGameToken* OtherToken) const { return Index == OtherToken->Index; }
