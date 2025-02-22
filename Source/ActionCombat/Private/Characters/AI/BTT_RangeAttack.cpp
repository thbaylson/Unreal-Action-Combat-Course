#include "Characters/AI/BTT_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{ OwnerComp.GetAIOwner()->GetPawn<ACharacter>() };
	if (!IsValid(CharacterRef))
	{
		return EBTNodeResult::Failed;
	}

	CharacterRef->PlayAnimMontage(AnimMontage);

	double RandomValue{ UKismetMathLibrary::RandomFloat() };
	if (RandomValue > Threshold)
	{
		Threshold = 0.9;
		
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Charge);
	}
	else
	{
		Threshold -= 0.1;
	}

	return EBTNodeResult::Succeeded;
}
