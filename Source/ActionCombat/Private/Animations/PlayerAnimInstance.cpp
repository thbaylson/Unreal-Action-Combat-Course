// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerAnimInstance.h"

void UPlayerAnimInstance::UpdateSpeed()
{
	APawn* PawnRef{ TryGetPawnOwner() };

	// If the PawnRef is not valid, return early.
	if (!IsValid(PawnRef)) { return; }
	
	FVector Velocity{ PawnRef->GetVelocity() };

	// We have to use assignment here because CurrentSpeed has already been initialized.
	// static_cast isn't necessary here, but it's good practice to be explicit about type conversions.
	CurrentSpeed = static_cast<float>(Velocity.Length());
}

void UPlayerAnimInstance::HandleUpdatedTarget(AActor* TargetActorRef)
{
	bIsInCombat = IsValid(TargetActorRef);
}

void UPlayerAnimInstance::UpdateDirection()
{
	APawn* PawnRef{ TryGetPawnOwner() };
	if (!IsValid(PawnRef)) { return; }

	if (!bIsInCombat) { return; }

	// Note that CalculateDirection returns a value between -180 and 180. That's why the 
	// Horizontal Axis in the Blend Space details was set to that same range.
	CurrentDirection = CalculateDirection(
		PawnRef->GetVelocity(),
		PawnRef->GetActorRotation()
	);
}
