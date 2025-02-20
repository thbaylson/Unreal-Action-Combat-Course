#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LookAtPlayerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONCOMBAT_API ULookAtPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

	ACharacter* OwnerRef;

	UPROPERTY(EditAnywhere)
	float RotationSpeed{ 400.0f };

public:	
	// Sets default values for this component's properties
	ULookAtPlayerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
