#pragma once

#include "CoreMinimal.h"

namespace ArrayActions {
	template <typename T>
	void ShuffleArray(TArray<T>& Array) {
		if (Array.Num() > 0)
		{
			int32 LastIndex = Array.Num() - 1;
			for (int32 i = 0; i < LastIndex; i++)
			{
				int32 Index = FMath::RandRange(0, LastIndex);
				if (i != Index)
				{
					Array.Swap(i, Index);
				}
			}
		}
	}
}
