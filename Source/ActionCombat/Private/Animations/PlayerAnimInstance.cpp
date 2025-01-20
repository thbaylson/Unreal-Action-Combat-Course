// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerAnimInstance.h"

void UPlayerAnimInstance::UpdateVelocity()
{
	APawn* PawnRef{ TryGetPawnOwner() };

	// If the PawnRef is not valid, return early.
	if (!IsValid(PawnRef)) { return; }
	
	FVector Velocity{ PawnRef->GetVelocity() };

	// We have to use assignment here because CurrentVelocity has already been initialized.
	// static_cast isn't necessary here, but it's good practice to be explicit about type conversions.
	CurrentVelocity = static_cast<float>(Velocity.Length());
}
