// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"
#include "TankPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "CannonSpray.h"
#include "TankProject/TankProject.h"

// Sets default values
ATankPawn::ATankPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	RootComponent = BoxComponent;

	TankBody = CreateDefaultSubobject<USkeletalMeshComponent>("TankBody");
	TankBody->SetupAttachment(RootComponent);

	TankTurret = CreateDefaultSubobject<UStaticMeshComponent>("TankTurret");
	TankTurret->SetupAttachment(TankBody);

	CannonPosition = CreateDefaultSubobject<UArrowComponent>("CannonPosition");
	CannonPosition->SetupAttachment(TankTurret);
	
	ArmComponent = CreateDefaultSubobject<USpringArmComponent>("ArmComponent");
	ArmComponent->SetupAttachment(RootComponent);
	ArmComponent->SetRelativeRotation(FRotator(-90, 0, 0));
	
	CameraComponent =CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(ArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

void ATankPawn::MoveForward(float Scale)
{
	MoveScaleTarget = Scale;
}

void ATankPawn::RotateRight(float Scale)
{
	RotationScaleTarget = Scale;
}

void ATankPawn::Shoot()
{
	if(SwapWep ==0)
	
	{
		if(Cannon && AmmoBB > 0)
		{
			Cannon->Shoot(AmmoBB);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(333, 1.5,FColor::Red,FString(TEXT("NO BULLET")));
		}
	}
	else
	{
		Spray();
	}
}


void ATankPawn::Spray()
{
	if(CannonSpray && AmmoAK > 0)
	{
		CannonSpray->Spray(AmmoAK);
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(333, 1.5,FColor::Red,FString(TEXT("NO BULLET")));
	}
}

void ATankPawn::SetupCannon(const TSubclassOf<ACannon>& CannonClass)
{
	CannonType = CannonClass;
	if(CannonType)
	{
		auto Transform = CannonPosition->GetComponentTransform();
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonType, Transform);
		Cannon->AttachToComponent(CannonPosition,FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

	void ATankPawn::SetupCannonSpray(const TSubclassOf<ACannonSpray>& CannonClass)
	{
		CannonSprayType = CannonClass;
		if(CannonSprayType)
		{
			auto Transform = CannonPosition->GetComponentTransform();
			CannonSpray = GetWorld()->SpawnActor<ACannonSpray>(CannonSprayType, Transform);
			CannonSpray->AttachToComponent(CannonPosition,FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
	}




void ATankPawn::Swap()
{
	if(SwapWep == 0 )
	{
		SwapWep = 1;
	}
	else
	{
		SwapWep = 0;
	}
}


void ATankPawn::UploadAmmo(const int ShellCount)
{
	AmmoBB += ShellCount;
	GEngine->AddOnScreenDebugMessage(1234, 999999, FColor::Blue, FString::Printf(TEXT("AmmoBB - %i"), AmmoBB));
	
	AmmoAK += ShellCount;
	GEngine->AddOnScreenDebugMessage(1235, 999999, FColor::Blue, FString::Printf(TEXT("AmmoAK - %i"), AmmoAK));
}


// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankPlayerController>(GetController());

	SetupCannon(CannonType);

	SetupCannonSpray(CannonSprayType);
	
}

void ATankPawn::Destroyed()
{
	Super::Destroyed();

	if(Cannon)
	{
		Cannon->Destroy();
	}
	if(CannonSpray)
	{
		CannonSpray->Destroy();
	}
}




// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveTank(DeltaTime);
	RotateTank(DeltaTime);
	RotateCannon(DeltaTime);
}

	// Called to bind functionality to input
	void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
	{
		Super::SetupPlayerInputComponent(PlayerInputComponent);

	}

void ATankPawn::MoveTank(float DeltaTime)
{
	MoveScaleCurrent = FMath::Lerp(MoveScaleCurrent, MoveScaleTarget,MovementAcceleration);
	const auto Location = GetActorLocation();
	SetActorLocation(Location + GetActorForwardVector() * MoveScaleCurrent * MovementSpeed * DeltaTime, false);
	GEngine->AddOnScreenDebugMessage(-666, -5, FColor::Turquoise, FString::Printf(TEXT("Speed - %f"), MoveScaleCurrent));
	
}

void ATankPawn::RotateTank(float DeltaTime)
{
	RotationScaleCurrent = FMath::Lerp(RotationScaleCurrent, RotationScaleTarget, RotationAcceleration);
	auto Rotation = GetActorRotation();
	Rotation.Yaw += RotationScaleCurrent * RotationSpeed * DeltaTime;
	SetActorRotation(Rotation);
	UE_LOG(LogTankProject, Warning, TEXT("%s"), *Rotation.ToString())
}

void ATankPawn::RotateCannon(float DeltaTime)
{
	if(!TankController)
		return;
	auto OldRotation = TankTurret->GetComponentRotation();
	FRotator TurretRotation = UKismetMathLibrary::FindLookAtRotation(TankTurret->GetComponentLocation(),TankController->GetMousePosition());
	OldRotation.Yaw = TurretRotation.Yaw;
	TankTurret->SetWorldRotation(FMath::Lerp(TankTurret->GetComponentRotation(),OldRotation,RotationAcceleration));
	
}




