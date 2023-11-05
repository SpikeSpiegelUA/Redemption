// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementsActions.h"

ESpellElements ElementsActions::FindSpellsMainElement(const TArray<ESpellElements>& SpellElements)
{
	//Count elements
	TArray<int8> Counts{};
	TArray<ESpellElements> CountCorrespondingElement{};
	TArray<ESpellElements> CopyOfSpellElements = SpellElements;
	for (int8 j = 0; j < SpellElements.Num(); j++) {
		ESpellElements CurrentElement{};
		int8 CurrentCount = 0;
		for (int8 i = 0; i < CopyOfSpellElements.Num(); i++) {
			if (i == 0)
				CurrentElement = SpellElements[i];
			if (CopyOfSpellElements[i] == CurrentElement)
				CurrentCount++;
		}
		Counts.Add(CurrentCount);
		CountCorrespondingElement.Add(CurrentElement);
		CopyOfSpellElements.Remove(CurrentElement);
		if (CopyOfSpellElements.Num() == 0)
			break;
	}
	//If there are elements with the same count, then return Multielemental.
	for (int8 i = 0; i < Counts.Num(); i++)
		for (int8 j = i + 1; j < Counts.Num(); j++)
			if (Counts[i] == Counts[j])
				return ESpellElements::MULTIELEMENTAL;
	//Find the highest element count
	int8 HighestCount = 0;
	int8 HighestCountIndex{};
	for (int8 i = 0; i < Counts.Num(); i++) {
		if (Counts[i] > HighestCount) {
			HighestCount = Counts[i];
			HighestCountIndex = i;
		}
	}
	if (HighestCountIndex < CountCorrespondingElement.Num())
		return CountCorrespondingElement[HighestCountIndex];
	return ESpellElements::NONE;
}
