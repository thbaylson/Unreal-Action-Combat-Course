#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "LookAtPlayerAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API ULookAtPlayerAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference
	) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;

private:
	void ToggleLookAtPlayerComponent(USkeletalMeshComponent* MeshComp, bool bCanRotate);
	
};
