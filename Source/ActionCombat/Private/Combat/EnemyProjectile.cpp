#include "Combat/EnemyProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyProjectile::HandleBeginOverlap(AActor* OtherActor)
{
	APawn* PawnRef{ Cast<APawn>(OtherActor) };
	if (!PawnRef->IsPlayerControlled()) { return; }

	FindComponentByClass<UParticleSystemComponent>()->SetTemplate(HitTemplate);

	FindComponentByClass<UProjectileMovementComponent>()->StopMovementImmediately();

	// This will destroy the projectile after 0.5 seconds. TODO: Could an object pool be used instead?
	FTimerHandle DeathTimerHandle{};
	GetWorldTimerManager().SetTimer(
		DeathTimerHandle,
		this,
		&AEnemyProjectile::DestroyProjectile,
		0.5f
	);

	// TODO: Do we really want to call Find for all of these components?
	FindComponentByClass<USphereComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FDamageEvent ProjectileDamageEvent{};
	PawnRef->TakeDamage(
		Damage,
		ProjectileDamageEvent,
		PawnRef->GetController(),
		this
	);
}

void AEnemyProjectile::DestroyProjectile()
{
	Destroy();
}
