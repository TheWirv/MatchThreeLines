// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameToken.generated.h"

struct FIndex
{
    int32 Row;
    int32 Column;
};

UCLASS()
class MATCHTHREELINES_API AGameToken : public AActor
{
    GENERATED_BODY()

    bool bIsSelected;

    FVector Location;
    FIndex Index;

public:
    // Sets default values for this actor's properties
    AGameToken();

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    class UMaterial* Material;

    UPROPERTY(VisibleAnywhere)
    class UMaterialInstanceDynamic* MaterialInstance;

    enum ETokenTypes { Red, Blue, Green, Yellow, White };

    ETokenTypes TokenType;

    void AssignMaterialInstanceToMesh();

    FVector GetMaterialInstanceColorVector() const;

    void Init(const int32 Row, const int32 Column);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnBeginMouseOver(AActor* TouchedActor);

    UFUNCTION()
    void OnEndMouseOver(AActor* TouchedActor);

    UFUNCTION()
    void OnMouseClick(AActor* TouchedActor, FKey ButtonPressed);

    UFUNCTION()
    void OnMouseRelease(AActor* TouchedActor, FKey ButtonPressed);

    bool IsNeighbor(const AGameToken* Other) const;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
