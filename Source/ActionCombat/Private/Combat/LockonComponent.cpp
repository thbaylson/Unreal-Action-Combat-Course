// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockonComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <GameFramework/SpringArmComponent.h>
#include "Interfaces/Enemy.h"

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
	CurrentTargetActor = OutResult.GetActor();

	// Return early if we didn't find a target.
	if (!bHasFoundTarget) { return; }
	
	// Return early if the target is not an enemy. Note that the generic is UEnemy, not IEnemy. This is how we validate interfaces in UE.
	if (!CurrentTargetActor->Implements<UEnemy>()) { return; }

	Controller->SetIgnoreLookInput(true);
	MovementComp->bOrientRotationToMovement = false;
	MovementComp->bUseControllerDesiredRotation = true;

	// This will move the camera higher which will ensure both the player and the target are in frame.
	SpringArmComp->TargetOffset = FVector{ 0.0f, 0.0f, 100.0f };

	IEnemy::Execute_OnSelect(CurrentTargetActor);
}

void ULockonComponent::EndLockon()
{
	if (IsValid(CurrentTargetActor) && CurrentTargetActor->Implements<UEnemy>()) {
		IEnemy::Execute_OnDeselect(CurrentTargetActor);
	}

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

	// Return early if the target is invalid.
	if (!IsValid(CurrentTargetActor)) { return; }

	FVector CurrentLocation{ OwnerRef->GetActorLocation() };
	FVector TargetLocation{ CurrentTargetActor->GetActorLocation() };

	// Return early if the target is too far away.
	double TargetDistance{ FVector::Distance(CurrentLocation, TargetLocation) };
	if (TargetDistance >= BreakDistance) {
		EndLockon();
		return;
	}

	// Adjust the camera's target location. This will tilt the camera downwards.
	TargetLocation.Z -= 125;

	FRotator NewRotation{ UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation) };

	Controller->SetControlRotation(NewRotation);
}

