// Fill out your copyright notice in the Description page of Project Settings.

#include "GameToken.h"
#include "MTLPlayerState.h"

#include "MTLGameMode.h"

// Sets default values
AGameToken::AGameToken()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bIsSelected = false;

    // Set up StaticMesh component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_GameToken"));
    Mesh->SetupAttachment(RootComponent);
    // Get the actual mesh asset
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
    UStaticMesh* PlaneMesh = MeshAsset.Object;
    if (PlaneMesh != nullptr)
    {
        static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(
            TEXT("Material'/Game/Materials/M_GameToken.M_GameToken'"));
        Material = MaterialAsset.Object;
        if (Material != nullptr)
        {
            Mesh->SetStaticMesh(PlaneMesh);
            Mesh->SetMaterial(0, Material);
            const FRotator Rotation(0.f, 90.f, 90.f);
            Mesh->SetRelativeRotation(Rotation);
            this->OnBeginCursorOver.AddDynamic(this, &AGameToken::OnBeginMouseOver);
            this->OnEndCursorOver.AddDynamic(this, &AGameToken::OnEndMouseOver);
            this->OnClicked.AddDynamic(this, &AGameToken::OnMouseClick);
            this->OnReleased.AddDynamic(this, &AGameToken::OnMouseRelease);
            // this->OnReleased
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Couldn't get Material for SM_GameToken!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Couldn't get SM_GameToken!"));
    }

    int32 RandomNumber = FMath::RandRange(Red, White);
    TokenType = static_cast<ETokenTypes>(RandomNumber);
}

void AGameToken::OnBeginMouseOver(AActor* TouchedActor)
{
    AMTLPlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMTLPlayerState>();

    if (PlayerState != nullptr)
    {
        PlayerState->SetCurrentlyHoveredGameToken(this);
        if (PlayerState->HasSelectionStarted())
        {
            const AGameToken* LastToken = Cast<AGameToken>(PlayerState->GetSelectedTokens().Last());
            if (IsNeighbor(LastToken) && LastToken->TokenType == TokenType)
            {
                const AGameToken* SelectedToken = Cast<const AGameToken>(TouchedActor);
                if (SelectedToken != nullptr)
                {
                    PlayerState->GetSelectedTokens().AddUnique(SelectedToken);
                }
                else
                {
                    GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't add Token!"));
                }
            }
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}

void AGameToken::OnEndMouseOver(AActor* TouchedActor)
{
    AMTLPlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMTLPlayerState>();

    if (PlayerState != nullptr)
    {
        PlayerState->SetCurrentlyHoveredGameToken(nullptr);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}

void AGameToken::OnMouseClick(AActor* TouchedActor, FKey ButtonPressed)
{
    bIsSelected = true;
    AMTLPlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMTLPlayerState>();

    if (PlayerState != nullptr)
    {
        PlayerState->SetSelectionStarted(true);
        const AGameToken* SelectedToken = Cast<const AGameToken>(TouchedActor);
        if (SelectedToken != nullptr)
        {
            PlayerState->GetSelectedTokens().AddUnique(SelectedToken);
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't add Token!"));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}

void AGameToken::OnMouseRelease(AActor* TouchedActor, FKey ButtonPressed)
{
    bIsSelected = false;
    AMTLPlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMTLPlayerState>();

    if (PlayerState != nullptr)
    {
        PlayerState->EndTurn();
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}

bool AGameToken::IsNeighbor(const AGameToken* Other) const
{
    // Column index to left/right is the same
    if (Index.Column - 1 == Other->Index.Column || Index.Column + 1 == Other->Index.Column)
    {
        // Same row
        if (Index.Row == Other->Index.Row ||
            // Check the next row for tokens with even column indeces
            ((Index.Column % 2 == 0) && (Index.Row - 1 == Other->Index.Row)) ||
            // Check the previous row for tokens with odd column indeces
            ((Index.Column % 2 == 1) && (Index.Row + 1 == Other->Index.Row)))
        {
            return true;
        }
    }
    else if ((Index.Column == Other->Index.Column) && (Index.Row - 1 == Other->Index.Row || Index.Row + 1 == Other->
        Index.Row))
    {
        // Column index is the same and the row index points either to the next or previous one
        return true;
    }

    return false;
}

void AGameToken::AssignMaterialInstanceToMesh()
{
    if (Material != nullptr)
    {
        MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
        MaterialInstance->SetVectorParameterValue("BaseColor", GetMaterialInstanceColor());
        Mesh->SetMaterial(0, MaterialInstance);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red,
                                         TEXT("Couldn't create MaterialInstance for SM_GameToken!"));
    }
}

void AGameToken::Init(const int32 Row, const int32 Column, const FVector InitialLocation)
{
    Index.Row = Row;
    Index.Column = Column;
    Location = InitialLocation;
}

// Called when the game starts or when spawned
void AGameToken::BeginPlay()
{
    Super::BeginPlay();

    AssignMaterialInstanceToMesh();
}

// Called every frame
void AGameToken::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AMTLPlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMTLPlayerState>();

    if (PlayerState != nullptr)
    {
        bIsSelected = PlayerState->GetSelectedTokens().Contains(this);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red,
                                         TEXT("Couldn't get PlayerState!"));
    }

    MaterialInstance->SetScalarParameterValue("IsSelected", bIsSelected ? 1 : 0);
}
