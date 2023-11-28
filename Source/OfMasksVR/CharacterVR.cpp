// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterVR.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NavigationSystem.h"

// Sets default values
ACharacterVR::ACharacterVR()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(FName("VRRoot"));
	VRRoot->SetupAttachment(GetRootComponent());	

	CameraComp = CreateDefaultSubobject<UCameraComponent>(FName("CameraComp"));
	CameraComp->SetupAttachment(VRRoot);

	DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(FName("DestinationMarker"));
	DestinationMarker->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void ACharacterVR::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterVR::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewCameraOffset = CameraComp->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);	
	UpdateDestinationMarker();
}

// Called to bind functionality to input
void ACharacterVR::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Teleport"), EInputEvent::IE_Released, this, &ACharacterVR::BeginTeleport);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ACharacterVR::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ACharacterVR::MoveRight);
}

void ACharacterVR::MoveForward(float Value)
{	
	AddMovementInput(CameraComp->GetForwardVector() * MovementSpeed * GetWorld()->GetDeltaSeconds(), Value);
}

void ACharacterVR::MoveRight(float Value)
{
	AddMovementInput(CameraComp->GetRightVector()* MovementSpeed * GetWorld()->GetDeltaSeconds(), Value);
}

void ACharacterVR::BeginTeleport()
{
	const APlayerController* PC = Cast<APlayerController>(GetController());	
	if(!PC) return;
	PC->PlayerCameraManager->StartCameraFade(0, 1, TeleportFadeTime, FLinearColor::Black);
	
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ACharacterVR::FinishTeleport, TeleportFadeTime);
}

void ACharacterVR::FinishTeleport()
{
	const APlayerController* PC = Cast<APlayerController>(GetController());
	if(!PC) return;	
	PC->PlayerCameraManager->StartCameraFade(1, 0, TeleportFadeTime, FLinearColor::Black);	
	const FVector CapsuleOffset = FVector(0,0,GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	SetActorLocation(DestinationMarker->GetComponentLocation() /*+ CapsuleOffset*/); 		
}


void ACharacterVR::UpdateDestinationMarker()
{	
	const FVector Start = CameraComp->GetComponentLocation();
	const FVector End = Start + CameraComp->GetForwardVector() * MaxTeleportDistance;	
	FHitResult Hit;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation NavLocation;
	//bool bOnNavMesh = NavSys->ProjectPointToNavigation(Hit.Location, NavLocation, TeleportProjectionExtent);
	const bool bOnNavMesh = UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(Hit.Location, NavLocation, TeleportProjectionExtent);
	
	if(bHit)
	{
		DestinationMarker->SetVisibility(true);
		DestinationMarker->SetWorldLocation(Hit.Location);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Hit Something!")));
	}
	else
	{	
		DestinationMarker->SetVisibility(false);
	}
}