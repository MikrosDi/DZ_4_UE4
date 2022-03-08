// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::OnMoveForward);
	InputComponent->BindAxis("RotationRight", this, &ATankPlayerController::OnRotateRight);
	
	InputComponent->BindAction("Shoot", IE_Pressed, this, &ATankPlayerController::OnShoot);
	InputComponent->BindAction("Shoot", IE_Repeat, this, &ATankPlayerController::OnShoot);
	
	InputComponent->BindAction("Spray",IE_Repeat, this, &ATankPlayerController::OnSpray);

	InputComponent->BindAction("SwapWeapon", IE_Pressed, this, &ATankPlayerController::OnSwapWeapon);
	
	bShowMouseCursor = true;
}

void ATankPlayerController::OnMoveForward(float Value)
{
	if(PlayerTank)
		PlayerTank->MoveForward(Value);
}
void ATankPlayerController::OnRotateRight(float Value)
{
	if(PlayerTank)
		PlayerTank->RotateRight(Value);
}

void ATankPlayerController::OnShoot()
{
	if(PlayerTank)
		PlayerTank->Shoot();
}

void ATankPlayerController::OnSpray()
{
	if(PlayerTank)
		PlayerTank->Spray();
	
}

void ATankPlayerController::OnSwapWeapon()
{
	if(PlayerTank)
		PlayerTank->Swap();
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerTank = CastChecked<ATankPawn>(GetPawn());
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FVector MousePosition, MouseDirection;
	DeprojectMousePositionToWorld(MousePosition,MouseDirection);

	auto Z = FMath::Abs(PlayerTank->GetActorLocation().Z - MousePosition.Z);
	MouseWorldPosition = MousePosition - MouseDirection * Z /MouseDirection.Z;
	
	
}

