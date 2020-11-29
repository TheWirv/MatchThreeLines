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

UCLASS()
class MATCHTHREELINES_API AGameToken : public AActor
{
    GENERATED_BODY()

    bool bIsSelected;
    bool bIsFallingDown;

    float LocationZ;
    FIntPoint Index;
    ETokenType TokenType;

    void AssignMaterialInstanceToMesh();

    /**
    * Gets the color for this token's material instance.
    *
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
    // Sets default values for this actor's properties
    AGameToken();

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    class UMaterial* Material;

    UPROPERTY(VisibleAnywhere)
    class UMaterialInstanceDynamic* MaterialInstance;

    void Init(const int32 Column, const int32 Row, const float InitialLocationZ);

    bool IsNeighbor(const AGameToken* Other) const;

    float GetLocationZ() const;

    FIntPoint GetIndex() const;

    void SetIsFallingDown(const bool InIsFallingDown);

    void SetLocationZ(const float NewLocationZ);

    void SetIndex(const FIntPoint NewIndex);

    bool IsOfSameType(const AGameToken* OtherToken) const;

    /**
    * Equality operator.
    *
    * @param OtherToken GameToken to compare.
    * @returns True if GameToken and OtherToken have the same Index. False otherwise.
    */
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

// Setters
FORCEINLINE void AGameToken::SetIsFallingDown(const bool InIsFallingDown) { bIsFallingDown = InIsFallingDown; };

FORCEINLINE void AGameToken::SetLocationZ(const float NewLocationZ) { LocationZ = NewLocationZ; };

FORCEINLINE void AGameToken::SetIndex(const FIntPoint NewIndex) { Index = NewIndex; };

// Other inline functions
FORCEINLINE bool AGameToken::IsOfSameType(const AGameToken* OtherToken) const
{
    return TokenType == OtherToken->TokenType;
};

FORCEINLINE bool AGameToken::operator==(const AGameToken* OtherToken) const { return Index == OtherToken->Index; }
