// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

UCLASS()
class OFMASKSVR_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComp;
	
	DECLARE_DYNAMIC_DELEGATE(FTestDelegate);
	
	FTestDelegate OnExplode;
	
	UFUNCTION()
	void Explode();
};
