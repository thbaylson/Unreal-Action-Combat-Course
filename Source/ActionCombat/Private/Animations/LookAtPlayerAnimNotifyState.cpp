#include "Animations/LookAtPlayerAnimNotifyState.h"
#include "Characters/LookAtPlayerComponent.h"

void ULookAtPlayerAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ToggleLookAtPlayerComponent(MeshComp, true);
}

void ULookAtPlayerAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ToggleLookAtPlayerComponent(MeshComp, false);
}

void ULookAtPlayerAnimNotifyState::ToggleLookAtPlayerComponent(USkeletalMeshComponent* MeshComp, bool bCanRotate)
{
	AActor* OwnerRef{ MeshComp->GetOwner() };
	if (!IsValid(OwnerRef)) { return; }

	ULookAtPlayerComponent* RotationComp{ OwnerRef->FindComponentByClass<ULookAtPlayerComponent>() };
	if (!IsValid(RotationComp)) { return; }

	RotationComp->bCanRotate = bCanRotate;
}
