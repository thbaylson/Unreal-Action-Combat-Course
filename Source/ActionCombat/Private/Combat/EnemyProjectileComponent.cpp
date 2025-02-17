#include "Combat/EnemyProjectileComponent.h"

// Sets default values for this component's properties
UEnemyProjectileComponent::UEnemyProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UEnemyProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UEnemyProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyProjectileComponent::SpawnProjectile(FName ComponentName, TSubclassOf<AActor> ProjectileClass)
{
	USceneComponent* SpawnPointComp{ Cast<USceneComponent>(GetOwner()->GetDefaultSubobjectByName(ComponentName)) };

	// GetComponentLocation returns a vector relative to the game world.
	FVector SpawnLocation{ SpawnPointComp->GetComponentLocation() };

	GetWorld()->SpawnActor(ProjectileClass, &SpawnLocation);
}
