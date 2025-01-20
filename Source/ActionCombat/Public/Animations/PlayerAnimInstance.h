// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

// Variables that will be exposed to Blueprints should be protected or public.
protected:
	// This is an example of direct initialization (as opposed to copy initialization).
	// The distinction between the two is a C++ specific concept.
	UPROPERTY(BlueprintReadWrite, EditAnywhere);
	float CurrentVelocity{ 0.0f };
};
