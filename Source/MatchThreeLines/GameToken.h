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

    FIntPoint Index;
    float LocationZ;
    ETokenType TokenType;
    float ScoreValue;
    bool bIsFallingDown;

    /** Creates a dynamic material instance and assigns it to the Token's static mesh */
    void AssignMaterialInstanceToMesh();

    /** Assigns the token a type and sets the score value based on that */
    void AssignTokenType();

    /**
    * Gets the color for this token's material instance.
    * @returns This token's material instance color
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

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnBeginMouseOver(AActor* TouchedActor);

    UFUNCTION()
    void OnEndMouseOver(AActor* TouchedActor);

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

    /** Sets the MaterialInstance's "IsSelected" parameter to mark this token as selected */
    void MarkSelected(const bool bIsSelected);

    // Overrides
    virtual void Tick(const float DeltaTime) override;

    // Getters, setters, and property-related delegates
    FORCEINLINE FIntPoint GetIndex() const { return Index; };

    FORCEINLINE void SetIndex(const FIntPoint InIndex) { Index = InIndex; };

    FORCEINLINE float GetLocationZ() const { return LocationZ; };

    FORCEINLINE void SetLocationZ(const float InLocationZ) { LocationZ = InLocationZ; };

    FORCEINLINE float GetScoreValue() const { return ScoreValue; };

    FORCEINLINE void SetIsFallingDown(const bool bInIsFallingDown) { bIsFallingDown = bInIsFallingDown; };

    // Other inline functions
    FORCEINLINE bool IsOfSameType(const AGameToken* OtherToken) const { return TokenType == OtherToken->TokenType; };

    // Operators
    FORCEINLINE bool operator==(const AGameToken* OtherToken) const { return Index == OtherToken->Index; }
};
