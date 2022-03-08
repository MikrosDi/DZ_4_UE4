
#include "CannonSpray.h"
#include "DrawDebugHelpers.h"

ACannonSpray::ACannonSpray()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>("ProjectileSpawnPoint");
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
}

void ACannonSpray::Spray(int& AmmoAK)
{
	if(!bReadyToShoot)
		return;
	
	switch (Type2)
	{
	case ECannonType2::FireProjectile2:
		GEngine->AddOnScreenDebugMessage(-1, 1.5,FColor::Red,FString(TEXT("")));
		GetWorld()->SpawnActor<AProjectile>(ProjectileClass,ProjectileSpawnPoint->GetComponentLocation(),ProjectileSpawnPoint->GetComponentRotation());
		AmmoAK--;
		GEngine->AddOnScreenDebugMessage(1235, 999999, FColor::Blue, FString::Printf(TEXT("AmmoAK - %i"), AmmoAK));
			
		break;

	case ECannonType2::FireTrace2:
		GEngine->AddOnScreenDebugMessage(-1, 1.5,FColor::Green,FString(TEXT("")));
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
	}
	bReadyToShoot = false;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &ACannonSpray::ResetShootState),  FireRate2,  false);
}

	void ACannonSpray::ResetShootState()
	{
		bReadyToShoot = true;
	}

	// Called when the game starts or when spawned
	void ACannonSpray::BeginPlay()
	{
		Super::BeginPlay();
	}

	// Called every frame
	void ACannonSpray::Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);
		//GEngine->AddOnScreenDebugMessage(12345, -1, FColor::Orange,FString::Printf(TEXT("RechargeSpray - %f"), GetWorld()->GetTimerManager().GetTimerElapsed(TimerHandle)));
	};
