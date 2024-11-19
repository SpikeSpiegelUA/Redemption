// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckItemInInventory.h"
#include "Redemption/GameInstance/RedemptionGameInstance.h"

UBTDecorator_CheckItemInInventory::UBTDecorator_CheckItemInInventory(const FObjectInitializer& ObjectInitializer) {
	bCreateNodeInstance = true;
	NodeName = "Check if item is in inventory";
}

bool UBTDecorator_CheckItemInInventory::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {

	URedemptionGameInstance* RedemptionGameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>();
	if (!IsValid(RedemptionGameInstance))
		return false;

	for (const FItemClassAndAmount ItemClassAndAmount : RedemptionGameInstance->InstanceItemsInTheInventory)
		if (ItemClassAndAmount.ItemClass == ItemToFindClass)
			return true;

	return false;
}