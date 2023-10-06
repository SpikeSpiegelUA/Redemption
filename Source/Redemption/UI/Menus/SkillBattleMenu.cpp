
#include "SkillBattleMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\SkillEntryWidget.h"

bool USkillBattleMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(UseButton))
		UseButton->OnClicked.AddDynamic(this, &USkillBattleMenu::UseButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void USkillBattleMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillBattleMenu::UseButtonOnClicked()
{

}
