#include "Characters/AI/BTT_ChargeAttack.h"

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("UBTT_ChargeAttack::ExecuteTask"));
	return EBTNodeResult::Succeeded;
}
