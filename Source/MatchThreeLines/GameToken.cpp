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
    Index.SetNum(2);

    // Set up StaticMesh component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_GameToken"));
    Mesh->SetupAttachment(RootComponent);
    // Get the actual mesh asset
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("StaticMesh'/Game/Meshes/SM_GameToken.SM_GameToken'"));
    UStaticMesh* GameTokenMesh = MeshAsset.Object;
    if (GameTokenMesh != nullptr)
    {
        static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(
            TEXT("Material'/Game/Materials/M_GameToken.M_GameToken'"));
        Material = MaterialAsset.Object;
        if (Material != nullptr)
        {
            Mesh->SetStaticMesh(GameTokenMesh);
            Mesh->SetMaterial(0, Material);
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
                    PlayerState->AddSelectedToken(SelectedToken);
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
            PlayerState->AddSelectedToken(SelectedToken);
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
        PlayerState->SetSelectionStarted(false);
        PlayerState->ResetSelectedTokens();
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Couldn't get PlayerState!"));
    }
}

void AGameToken::AssignMaterialInstanceToMesh()
{
    if (Material != nullptr)
    {
        MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
        MaterialInstance->SetVectorParameterValue("BaseColor", GetMaterialInstanceColorVector());
        Mesh->SetMaterial(0, MaterialInstance);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red,
                                         TEXT("Couldn't create MaterialInstance for SM_GameToken!"));
    }
}

FVector AGameToken::GetMaterialInstanceColorVector() const
{
    FColor MaterialInstanceColor;
    switch (TokenType)
    {
    case Red:
        MaterialInstanceColor = FColor::Red;
        break;
    case Blue:
        MaterialInstanceColor = FColor::Blue;
        break;
    case Green:
        MaterialInstanceColor = FColor::Green;
        break;
    case Yellow:
        MaterialInstanceColor = FColor::Yellow;
        break;
    default:
        MaterialInstanceColor = FColor::White;
    }


    return FVector(MaterialInstanceColor.R, MaterialInstanceColor.G, MaterialInstanceColor.B);
}

void AGameToken::Init(const TArray<int32> InIndex)
{
    Index = InIndex;
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

    if (bIsSelected)
    {
        MaterialInstance->SetScalarParameterValue("IsSelected", 1);
    }
    else
    {
        MaterialInstance->SetScalarParameterValue("IsSelected", 0);
    }
}
