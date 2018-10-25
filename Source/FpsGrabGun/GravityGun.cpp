// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityGun.h"

#include "Animation/AnimInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "EngineGlobals.h"
#include "GameFramework/WorldSettings.h"
#include "Components/PrimitiveComponent.h"
#include "Features/IModularFeatures.h"
#include "XRMotionControllerBase.h"

#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/World.h"


AGravityGun::AGravityGun()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<USkeletalMesh> Mesh;
		FConstructorStatics() : Mesh(TEXT("/Game/Weapons/Launcher.Launcher"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;



	//SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	//RootComponent = SkeletalMeshComponent;
	//SkeletalMeshComponent->SetSkeletalMesh(ConstructorStatics.Mesh.Get());	// Set static mesh
	//SkeletalMeshComponent->SetOnlyOwnerSee(false);			// only the owning player will see this mesh
	//SkeletalMeshComponent->SetOwnerNoSee(true);
	//SkeletalMeshComponent->SetVisibility(true, true);
	////SkeletalMeshComponent->bCastDynamicShadow = false;
	////SkeletalMeshComponent->CastShadow = false;
	//SkeletalMeshComponent->SetupAttachment(RootComponent);

	HoldingItem = NULL;
	pickedObject = NULL;
}

void AGravityGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation()+ GetActorForwardVector()*1000, 120.f, FColor::Cyan, false, -1, 2, 5.f);
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Actor %s!!"), *GetActorForwardVector().ToString()));

	FVector Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	float pickRange = 5000.f;
	FVector End = ((ForwardVector * pickRange) + Start);
	if (!pickedObject)
	{
		FComponentQueryParams DefaultComponentQueryParams;
		FCollisionResponseParams DefaultResponseParam;
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam))
		{
			HoldingItem = Hit.GetActor();

			//if (HoldingItem->GetRootComponent()->IsSimulatingPhysics() && HoldingItem->GetRootComponent()->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
			//	GEngine->AddOnScreenDebugMessage(-1, .2f, FColor::Red, FString::Printf(TEXT("Name: %s"), *Hit.GetActor()->GetName()));
		}
		else
			HoldingItem = NULL;
	}
	else
	{
		FVector camera = GetActorLocation();
		FVector catchLocation = camera
			+ GetActorUpVector() * 100
			+ GetActorForwardVector() * 350;
		//+ GetActorRightVector() * 120;


		FVector forceDirection = FVector(-(pickedObject->GetComponentLocation() - catchLocation));
		float distance = forceDirection.Size();
		float force = distance * 200;
		pickedObject->AddForce(forceDirection.GetSafeNormal() * force * pickedObject->GetMass());
		FVector dumpForce = pickedObject->GetComponentVelocity() * pickedObject->GetMass() * -30;
		pickedObject->AddForce(dumpForce);


		//DrawDebugDirectionalArrow(GetWorld(), camera, catchLocation, 120.f, FColor::Cyan, false, -1, 2, 5.f);
		//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetVelocity() + pickedObject->GetComponentLocation(), 120.f, FColor::Red, false, -1, 2, 5.f);
		//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), forceDirection.GetSafeNormal() * force + pickedObject->GetComponentLocation(), 120.f, FColor::Yellow, false, -1, 2, 5.f);
		//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), forceDirection.GetSafeNormal() * dumpForce + GetActorLocation(), 120.f, FColor::Purple, false, -1, 2, 5.f);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Picked %s!!"), *pickedObject->GetName()));
	}
}

void AGravityGun::Fire()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("FIRE1")));
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
	if (pickedObject)
	{
		pickedObject->AddImpulse(GetActorForwardVector() * 5000 * pickedObject->GetMass());
		Release();
	}
	if (HoldingItem && !pickedObject)
	{
		if (HoldingItem->GetRootComponent()->IsSimulatingPhysics() && HoldingItem->GetRootComponent()->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
		{
			pickedObject = (UStaticMeshComponent*)HoldingItem->GetRootComponent();
			pickedObject->AddImpulse(GetActorForwardVector() * 5000 * pickedObject->GetMass());
			pickedObject = NULL;
		}
	}
}

void AGravityGun::FireSecondary()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, FString::Printf(TEXT("FIRE2")));
	if (GrabSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GrabSound, GetActorLocation());
	}
	if (pickedObject)
		Release();
	else if (HoldingItem)
		Grab();
}

void AGravityGun::Release()
{
	if (pickedObject)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Released %s!!"), *pickedObject->GetName()));
		pickedObject->SetEnableGravity(true);
		pickedObject = NULL;
	}
}

void AGravityGun::Grab()
{
	if (HoldingItem->GetRootComponent()->IsSimulatingPhysics() && HoldingItem->GetRootComponent()->GetClass()->IsChildOf(UStaticMeshComponent::StaticClass()))
	{
		pickedObject = (UStaticMeshComponent*)HoldingItem->GetRootComponent();
		pickedObject->SetEnableGravity(false);
		//add small impulse for lifting and better catching
		pickedObject->AddImpulse(FVector(0, 0, 1) * 100 * pickedObject->GetMass());
	}
}