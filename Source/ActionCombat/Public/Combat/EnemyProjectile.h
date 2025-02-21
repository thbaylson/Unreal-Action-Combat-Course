#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectile.generated.h"

UCLASS()
class ACTIONCOMBAT_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitTemplate;

	UPROPERTY(EditAnywhere)
	float Damage{ 1.0f };
	
public:	
	// Sets default values for this actor's properties
	AEnemyProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void HandleBeginOverlap(AActor* OtherActor);

	UFUNCTION()// Note: UE's timers require the UFUNCTION macro
	void DestroyProjectile();

};
