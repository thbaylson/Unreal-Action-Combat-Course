#include "Combat/TraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/Fighter.h"

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

	double WeaponLength{ FVector::Distance(StartSocketLocation, EndSocketLocation) };
	FVector BoxHalfExtent{
		BoxCollisionLength, BoxCollisionLength, WeaponLength
	};
	BoxHalfExtent /= 2;

	TArray<FHitResult> OutResults;
	FCollisionShape CollisionBox{ FCollisionShape::MakeBox(BoxHalfExtent) };
	FCollisionQueryParams IgnoreParams{
		FName { TEXT("Ignore Params") },
		false,
		GetOwner()
	};

	bool bHasFoundTargets = GetWorld()->SweepMultiByChannel(
		OutResults,
		StartSocketLocation,
		EndSocketLocation,
		ShapeRotation,
		ECollisionChannel::ECC_GameTraceChannel1,
		CollisionBox,
		IgnoreParams
	);

	// We could probably use the debug preprocessor directive here instead.
	if (bDebugMode)
	{
		FVector CenterPoint{
			UKismetMathLibrary::VLerp(
				StartSocketLocation,
				EndSocketLocation,
				0.5f
			)
		};

		UKismetSystemLibrary::DrawDebugBox(
			GetWorld(),
			CenterPoint,
			CollisionBox.GetExtent(),
			bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
			// Need to convert FQuat to FRotator.
			ShapeRotation.Rotator(),
			// This is the duration of the debug line in seconds.
			0.75f,
			// This is the thickness of the debug line.
			2.0f
		);
	}

	// Return early if there are no results from the sweep.
	if (OutResults.Num() == 0) { return; }

	float CharacterDamage{ 0.0f };

	IFighter* FighterRef{ Cast<IFighter>(GetOwner()) };
	if (FighterRef)
	{
		CharacterDamage = FighterRef->GetDamage();
	}

	FDamageEvent TargetAttackedEvent;

	// We use const here to tell the compiler that we won't be modifying the Hit variable.
	// We use & to pass the Hit variable by reference. This is more efficient than passing by value, which
	// would create a whole new copy of the Hit variable.
	for (const FHitResult& Hit : OutResults)
	{
		AActor* TargetActor{ Hit.GetActor() };

		// UE built-in for sending a damage event to the target actor.
		TargetActor->TakeDamage(
			CharacterDamage,
			TargetAttackedEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner()
		);
	}
}
