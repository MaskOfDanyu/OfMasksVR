// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABomb::ABomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(GetRootComponent());

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CapsuleComp);
}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();
	OnExplode.BindDynamic(this, &ABomb::Explode);
}

void ABomb::NotifyActorBeginOverlap(AActor *OtherActor) 
{
	OnExplode.Execute();
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("KABOOM!"));
}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABomb::Explode()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("KABOOM!"));
}
