#include "Characters/AI/BTT_ChargeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animations/BossAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_ChargeAttack::UBTT_ChargeAttack()
{
	// Note: This must be set in order for the TickTask to ever be called.
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ControllerRef = OwnerComp.GetAIOwner();
	CharacterRef = ControllerRef->GetCharacter();
	BossAnim = Cast<UBossAnimInstance>(CharacterRef->GetMesh()->GetAnimInstance());

	BossAnim->bIsCharging = true;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsReadyToCharge"), false);

	return EBTNodeResult::InProgress;
}

void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool bIsReadyToCharge{ OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsReadyToCharge")) };

	if (bIsReadyToCharge)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsReadyToCharge"), false);
		ChargeAtPlayer();
	}
}

void UBTT_ChargeAttack::ChargeAtPlayer()
{
	APawn* PlayerRef{ GetWorld()->GetFirstPlayerController()->GetPawn() };
	FVector PlayerLocation{ PlayerRef->GetActorLocation() };

	// Initialize and configure the move request.
	FAIMoveRequest MoveRequest{ PlayerLocation };
	// Note: We must add the object "Actors > Volumes > NavMeshBoundsVolume" to the level in order for the AI to pathfind.
		// Note: By default, the "p" key toggles the visibility of the NavMeshBoundsVolume.
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

	// Initiate the move request.
	ControllerRef->MoveTo(MoveRequest);

	// Make sure the AI is looking at the player.
	ControllerRef->SetFocus(PlayerRef);
}
