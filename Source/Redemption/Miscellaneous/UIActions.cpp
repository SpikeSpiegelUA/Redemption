
#include "UIActions.h"
#include "..\Characters\Combat\CombatAllies.h"
#include "..\UI\HUD\FloatingManaBarWidget.h"

void UIActions::SetCrosshairAndManaHealthBarsVisibility(TArray<ACombatNPC*>& TargetsForSelection, int16 Index, FString& BarToShow)
{
	TargetsForSelection[Index]->GetCrosshairWidgetComponent()->SetVisibility(true);
	if (BarToShow == "Health")
		TargetsForSelection[Index]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
	else if (BarToShow == "Mana")
		Cast<ACombatAllies>(TargetsForSelection[Index])->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Visible);
}

void UIActions::SetCrosshairAndManaHealthBarsVisibility(ACombatNPC* const TargetNPC, FString& BarToShow)
{
	TargetNPC->GetCrosshairWidgetComponent()->SetVisibility(true);
	if (BarToShow == "Health")
		TargetNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
	else if (BarToShow == "Mana")
		Cast<ACombatAllies>(TargetNPC)->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Visible);
}
