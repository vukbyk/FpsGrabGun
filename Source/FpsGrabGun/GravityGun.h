// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GravityGun.generated.h"

/**
 * 
 */
UCLASS()
class FPSGRABGUN_API AGravityGun : public AWeapon
{
	GENERATED_BODY()
	
public:
	AGravityGun();

	void Tick(float DeltaTime) override;

	void Fire() override;
	void FireSecondary() override;

	UPROPERTY(EditAnywhere)
	class AActor* HoldingItem;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* pickedObject;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* GrabSound;

	UFUNCTION()
	void Grab();
	UFUNCTION()
	void Release();
};
