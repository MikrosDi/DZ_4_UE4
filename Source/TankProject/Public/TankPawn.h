// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoBox.h"
#include "CannonSpray.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "TankPawn.generated.h"

class ACannon;
UCLASS(Blueprintable)
class TANKPROJECT_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	
	ATankPawn();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* TankBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TankTurret;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* ArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Fire parans")
	TSubclassOf<ACannon>CannonType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Fire parans")
	TSubclassOf<ACannonSpray>CannonSprayType;
	
	UPROPERTY()
	ACannon* Cannon;

	UPROPERTY()
	ACannonSpray* CannonSpray;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementAcceleration = 0.06;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed =90;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationAcceleration = 0.06;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
    float TurretAcceleration = 0.1;

	int SwapWep = 0;

	void MoveForward(float Scale);
	void RotateRight(float Scale);
	
	void Shoot();
	void SetupCannon(const TSubclassOf<ACannon>& CannonClass);
	
	void Spray();
	void SetupCannonSpray(const TSubclassOf<ACannonSpray>& CannonClass);

	void Swap();
	
	void UploadAmmo(const int ShellCount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void MoveTank(float DeltaTime);
    void RotateTank(float DeltaTime);
    void RotateCannon(float DeltaTime);
	
	float MoveScaleTarget = 0;
	float MoveScaleCurrent = 0;
	float RotationScaleTarget = 0;
	float RotationScaleCurrent = 0;

	int AmmoBB = 10;
	int AmmoAK = 30;
	

	UPROPERTY()
	class ATankPlayerController* TankController;

};
