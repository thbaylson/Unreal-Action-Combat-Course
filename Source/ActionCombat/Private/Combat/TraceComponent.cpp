#include "Combat/TraceComponent.h"

// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	SkeletalComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector StartSocketLocation{ SkeletalComp->GetSocketLocation(StartSocketName) };
	FVector EndSocketLocation{ SkeletalComp->GetSocketLocation(EndSocketName) };
	FQuat ShapeRotation{ SkeletalComp->GetSocketQuaternion(SocketRotationName) };
}
