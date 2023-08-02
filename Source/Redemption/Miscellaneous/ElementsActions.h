// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Miscellaneous\ElementAndItsPercentage.h"

/**
 * 
 */
namespace ElementsActions {
	//Find elements that are contained in the spell and their percent.
	template <typename T>
	TArray<FElementAndItsPercentageStruct> FindContainedElements(TArray<T> ElementsArray)
	{
		TArray<FElementAndItsPercentageStruct> ArrayToReturn{};
		if (ElementsArray.Num() != 0) {
			//Count elements
			TArray<int8> Counts{};
			TArray<T> CountCorrespondingElement{};
			TArray<T> CopyOfElementsArray = ElementsArray;
			for (int8 j = 0; j < ElementsArray.Num(); j++) {
				T CurrentElement{};
				int8 CurrentCount = 0;
				for (int8 i = 0; i < CopyOfElementsArray.Num(); i++) {
					if (i == 0)
						CurrentElement = ElementsArray[i];
					if (CopyOfElementsArray[i] == CurrentElement)
						CurrentCount++;
				}
				Counts.Add(CurrentCount);
				CountCorrespondingElement.Add(CurrentElement);
				CopyOfElementsArray.Remove(CurrentElement);
				if (CopyOfElementsArray.Num() == 0)
					break;
			}

			int8 PercentOfOneElement = 100 / ElementsArray.Num();
			for (int8 i = 0; i < CountCorrespondingElement.Num(); i++) {
				FElementAndItsPercentageStruct NewElementAndItsPercent;
				NewElementAndItsPercent.SetElement(CountCorrespondingElement[i]);
				NewElementAndItsPercent.SetPercent(Counts[i] * PercentOfOneElement);
				ArrayToReturn.Add(NewElementAndItsPercent);
			}
		}
		return ArrayToReturn;
	}
}
