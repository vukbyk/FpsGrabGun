// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableObject.generated.h"

UCLASS()
class FPSGRABGUN_API APickableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableObject();
	

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;
	UFUNCTION()
	void Pickup(USceneComponent *AParentHolder);
	UFUNCTION()
	void Release();

	class USceneComponent *ParentHolder;

	bool bPicked;

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};