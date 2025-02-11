#include "Animations/ToggleTraceNotifyState.h"
#include "Combat/TraceComponent.h"

void UToggleTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ToggleTraceIsAttacking(MeshComp, true);
}

void UToggleTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ToggleTraceIsAttacking(MeshComp, false);
}

void UToggleTraceNotifyState::ToggleTraceIsAttacking(USkeletalMeshComponent* MeshComp, bool bIsAttacking)
{
	UTraceComponent* TraceComp{
		MeshComp->GetOwner()->FindComponentByClass<UTraceComponent>()
	};

	if (!IsValid(TraceComp)) { return; }

	TraceComp->bIsAttacking = bIsAttacking;
}
