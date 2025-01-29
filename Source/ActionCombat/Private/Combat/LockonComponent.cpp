// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockonComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <GameFramework/SpringArmComponent.h>

// Sets default values for this component's properties
ULockonComponent::ULockonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// NOTE: Code inside constructors runs when the component is edited and when the game is playing.
}


// Called when the game starts
void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = GetOwner<ACharacter>();
	Controller = GetWorld()->GetFirstPlayerController();
	MovementComp = OwnerRef->GetCharacterMovement();
	SpringArmComp = OwnerRef->FindComponentByClass<USpringArmComponent>();
	CurrentTargetActor = nullptr;
}

void ULockonComponent::ToggleLockon(float Radius)
{
	// The target is valid when we're already locked on to something.
	if(IsValid(CurrentTargetActor))
	{
		EndLockon();
	}
	// If the target is null, try to find one
	else 
	{
		StartLockon(Radius);
	}
}

void ULockonComponent::StartLockon(float Radius)
{
	FHitResult OutResult;
	FVector CurrentLocation{ OwnerRef->GetActorLocation() };
	FCollisionShape Sphere{ FCollisionShape::MakeSphere(Radius) };
	FCollisionQueryParams IgnoreParams{
		FName { TEXT("Ignore Collision Params") },
		// This lets us ignore complex collision
		false,
		OwnerRef
	};

	bool bHasFoundTarget{ GetWorld()->SweepSingleByChannel(
		OutResult,
		CurrentLocation,
		CurrentLocation,
		FQuat::Identity,
		// This represents the Fighter channel we set up in the editor. DefaultEngine.ini was searched to find it.
		ECollisionChannel::ECC_GameTraceChannel1,
		Sphere,
		IgnoreParams
	) };

	if (!bHasFoundTarget) { return; }
	
	// string placeholders must be passed by reference
	UE_LOG(LogTemp, Warning, TEXT("Actor Detected: %s"), *OutResult.GetActor()->GetName());

	Controller->SetIgnoreLookInput(true);
	MovementComp->bOrientRotationToMovement = false;
	MovementComp->bUseControllerDesiredRotation = true;

	CurrentTargetActor = OutResult.GetActor();

	// This will move the camera higher which will ensure both the player and the target are in frame.
	SpringArmComp->TargetOffset = FVector{ 0.0f, 0.0f, 100.0f };
}

void ULockonComponent::EndLockon()
{
	CurrentTargetActor = nullptr;

	MovementComp->bOrientRotationToMovement = true;
	MovementComp->bUseControllerDesiredRotation = false;

	SpringArmComp->TargetOffset = FVector::ZeroVector;

	// Consecutive calls to SetIgnoreLookInput stack. This will reset the value to false no matter what.
	Controller->ResetIgnoreLookInput();
}


// Called every frame
void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(CurrentTargetActor)) { return; }

	FVector CurrentLocation{ OwnerRef->GetActorLocation() };
	FVector TargetLocation{ CurrentTargetActor->GetActorLocation() };

	// Adjust the camera's target location. This will tilt the camera downwards.
	TargetLocation.Z -= 125;

	FRotator NewRotation{ UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation) };

	Controller->SetControlRotation(NewRotation);
}

