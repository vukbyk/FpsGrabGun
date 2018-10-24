// Fill out your copyright notice in the Description page of Project Settings.

#include "PickableObject.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "EngineGlobals.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
APickableObject::APickableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetSimulatePhysics(true);
	RootComponent = StaticMeshComponent;

	bPicked = false;

	ParentHolder = NULL;

}

// Called when the game starts or when spawned
void APickableObject::BeginPlay()
{
	Super::BeginPlay();

}

void APickableObject::Pickup(USceneComponent *AParentHolder)
{
	if (!bPicked)
	{
		bPicked = true;
		ParentHolder = AParentHolder;
		StaticMeshComponent->SetEnableGravity(false);
		StaticMeshComponent->AddImpulse(FVector(0,0,1) * 100 * StaticMeshComponent->GetMass());//Small push up 
	}
	//StaticMeshComponent->SetEnableGravity(false);
	////StaticMeshComponent->SetSimulatePhysics(bHolding ? false : true);
	////StaticMeshComponent->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
	//if (HoldingComp && bHolding)
	//{
	//	//StaticMeshComponent->AttachToComponent(HoldingComp, FAttachmentTransformRules::KeepWorldTransform);
	//	//SetActorLocation(HoldingComp->GetComponentLocation());
	//}

	//if (!bHolding)
	//{
	//	//StaticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	//	//StaticMeshComponent->AddForce(PlayerCamera->GetForwardVector() * 100000 * StaticMeshComponent->GetMass());
	//}
}
void APickableObject::Release()
{
	if (bPicked)
	{
		bPicked = false;
		ParentHolder = NULL;
		StaticMeshComponent->SetEnableGravity(true);
	}
}

// Called every frame
void APickableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bPicked == true)
	{
		//StaticMeshComponent->AddImpulse(FVector(0, 0, 1) * 50 * StaticMeshComponent->GetMass());

		//Where to hold location
		FVector loc = ParentHolder->GetComponentLocation() + ParentHolder->GetRightVector() * 120 + ParentHolder->GetUpVector() *100 + ParentHolder->GetForwardVector() * -120;



		FVector forceDirection = FVector(-(GetActorLocation() - loc));
		float distance = forceDirection.Size();
		float force = /*(distance < 10) ? 0: */distance * 200;
		//FMath::Clamp(force, 0.f, 1000.f);
		//FMath::Min(distance/3, 150.f);// (distance < 100) ? GetVelocity().Size()*-5 : distance;
		
		StaticMeshComponent->AddForce( forceDirection.GetSafeNormal() * force * StaticMeshComponent->GetMass());
		FVector dumpForce = GetVelocity() * StaticMeshComponent->GetMass() * -20;
		StaticMeshComponent->AddForce(dumpForce);

		//DrawDebugDirectionalArrow(GetWorld(), ParentHolder->GetComponentLocation(), loc, 120.f, FColor::Cyan, false, -1, 2, 5.f);
		//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetVelocity() + GetActorLocation(), 120.f, FColor::Red, false, -1, 2, 5.f);
		//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), forceDirection.GetSafeNormal() * force + GetActorLocation(), 120.f, FColor::Yellow, false, -1, 2, 5.f);
		//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), forceDirection.GetSafeNormal() * dumpForce + GetActorLocation(), 120.f, FColor::Purple, false, -1, 2, 5.f);
		//GEngine->AddOnScreenDebugMessage(-1, .2f, FColor::Green, FString::Printf(TEXT("%s"), *loc.ToString() ));

	}
}

