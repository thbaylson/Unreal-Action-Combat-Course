// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockonComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONCOMBAT_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

	ACharacter* OwnerRef;
	APlayerController* Controller;

	// This is an example of forward declaration, a way to inform the compiler about the existence of a class before fully defining it.
	class UCharacterMovementComponent* MovementComp;

public:	
	// Sets default values for this component's properties
	ULockonComponent();

	AActor* CurrentTargetActor;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartLockon(float Radius= 750.0f);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
