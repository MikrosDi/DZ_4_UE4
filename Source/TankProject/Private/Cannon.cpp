// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>("ProjectileSpawnPoint");
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
}

void ACannon::Shoot(int& AmmoBB)
{
	if(!bReadyToShoot)
		return;
	
	switch (Type)
	{
	case ECannonType::FireProjectile:
		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Red,FString(TEXT("")));
		GetWorld()->SpawnActor<AProjectile>(ProjectileClass,ProjectileSpawnPoint->GetComponentLocation(),ProjectileSpawnPoint->GetComponentRotation());
		AmmoBB--;
		GEngine->AddOnScreenDebugMessage(1234, 999999, FColor::Blue, FString::Printf(TEXT("AmmoBB - %i"), AmmoBB));
		break;
	
	
	case ECannonType::FireTrace:
		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Green,FString(TEXT("")));
		FHitResult Result;
		FCollisionObjectQueryParams Params;
		Params.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
		Params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		Params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		
		auto Start = ProjectileSpawnPoint->GetComponentLocation();
		auto End = Start + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		
		bool HasHit = GetWorld()->LineTraceSingleByObjectType(
		    Result, Start, End, Params);
		
		if(HasHit)
		{
			End = Result.Location;
			if(Result.Actor.IsValid())
			{
				Result.Actor->Destroy();
			}
		}
		DrawDebugLine(GetWorld(),Start, End, FColor::Turquoise,false, 0.1, 0, 10);
		break;
	}
		
	bReadyToShoot = false;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &ACannon::ResetShootState), 1 / FireRate, false);
	
}

void ACannon::ResetShootState()
{
	bReadyToShoot = true;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(1245, -1, FColor::Purple,FString::Printf(TEXT("Recharge - %f"), GetWorld()->GetTimerManager().GetTimerElapsed(TimerHandle)));
};