// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterVR.generated.h"

UCLASS()
class OFMASKSVR_API ACharacterVR : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterVR();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void UpdateDestinationMarker();
	void MoveForward(float Value);
	void MoveRight (float Value);
	void BeginTeleport();
	void FinishTeleport();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private: 	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DestinationMarker;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed = 100.f;

	UPROPERTY(EditAnywhere)
	float MaxTeleportDistance = 1000;

	UPROPERTY(EditAnywhere)
	float TeleportFadeTime = 0.5f;

	UPROPERTY(EditAnywhere)
	FVector TeleportProjectionExtent = FVector(100,100,100);
};
