// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "CannonSpray.generated.h"

UENUM()
enum class ECannonType2:uint8
{
	FireProjectile2 = 0 UMETA(DisplayName = "Use projectile2"),
	FireTrace2 = 1 UMETA(DisplayName = "Use trace2")
};

UCLASS()
class TANKPROJECT_API ACannonSpray : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACannonSpray();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 1000;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate2 = 0.1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireType2 = 2;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType2 Type2 = ECannonType2::FireProjectile2;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	TSubclassOf<AProjectile> ProjectileClass;
	
	void Spray(int& AmmoAK);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	void ResetShootState();
	
	FTimerHandle TimerHandle;
	
	bool bReadyToShoot = true;
	
};
