
#include "SkillBattleMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\SkillEntryWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"

bool USkillBattleMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(UseButton)) {
		UseButton->OnClicked.AddDynamic(this, &USkillBattleMenu::UseButtonOnClicked);
		UseButton->OnHovered.AddDynamic(this, &USkillBattleMenu::UseButtonOnHovered);
	}
	if (IsValid(BackButton)) {
		BackButton->OnClicked.AddDynamic(this, &USkillBattleMenu::BackButtonOnClicked);
		BackButton->OnHovered.AddDynamic(this, &USkillBattleMenu::BackButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void USkillBattleMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillBattleMenu::UseButtonOnClicked()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		PlayerCharacter->GetSpellBattleMenuWidget()->UseUniqueSpellButtonOnClicked();
		this->RemoveFromParent();
	}
}

void USkillBattleMenu::BackButtonOnClicked()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())) {
		this->RemoveFromParent();
		PlayerCharacter->GetBattleMenuWidget()->AddToViewport();
		if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
			UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetBattleMenuWidget()->GetAttackButton();
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
		}
	}
}

void USkillBattleMenu::UseButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (SelectedSkillButton == UIManagerWorldSubsystem->PickedButton)
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
			else
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
		}
		UIManagerWorldSubsystem->PickedButton = UseButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void USkillBattleMenu::BackButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (SelectedSkillButton == UIManagerWorldSubsystem->PickedButton)
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
			else
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
		}
		UIManagerWorldSubsystem->PickedButton = BackButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 1;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void USkillBattleMenu::AddSkillEntryToSkillsScrollBox(const class ASpell* const SpellToAdd)
{
	/*if (IsValid(SkillEntryClass))
		SkillEntryWidget = CreateWidget<USkillEntryWidget>(GetWorld(), SkillEntryClass);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter) && IsValid(SkillEntryWidget)) {
		LearnedSpellEntryWidget->SetSpellTypeImage(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetSpellTypeImageTexture(SpellToAdd->GetTypeOfSpell()));
		LearnedSpellEntryWidget->SetSpellMainElementImage(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetMainSpellElementImageTexture(ElementsActions::FindSpellsMainElement(SpellToAdd->GetSpellElements())));
		LearnedSpellEntryWidget->SetSpellNameText(SpellToAdd->GetSpellName());
		LearnedSpellEntryWidget->EntrySpell = const_cast<ASpell*>(SpellToAdd);
		MainScrollBox->AddChild(LearnedSpellEntryWidget);
	}*/
}
