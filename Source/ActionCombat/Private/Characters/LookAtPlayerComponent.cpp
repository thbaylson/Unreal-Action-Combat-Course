#include "Characters/LookAtPlayerComponent.h"
#include "GameFramework/Character.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
ULookAtPlayerComponent::ULookAtPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void ULookAtPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = GetOwner<ACharacter>();	
}

// Called every frame
void ULookAtPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLocation = OwnerRef->GetActorLocation();
	FVector PlayerLocation{ GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() };

	FRotator CurrentRotation{ OwnerRef->GetActorRotation() };
	FRotator DesiredRotation{ UKismetMathLibrary::FindLookAtRotation(CurrentLocation, PlayerLocation) };

	FRotator NewRotation{ UKismetMathLibrary::RInterpTo_Constant(CurrentRotation, DesiredRotation, DeltaTime, RotationSpeed) };
	FRotator NewYawOnlyRotation{ CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll };

	OwnerRef->SetActorRotation(NewYawOnlyRotation);
}
