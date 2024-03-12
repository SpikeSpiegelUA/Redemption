
#include "SkillBattleMenu.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "..\UI\HUD\FloatingManaBarWidget.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

bool USkillBattleMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(UseButtonWithNeighbors)) {
		UseButtonWithNeighbors->OnClicked.AddDynamic(this, &USkillBattleMenu::UseButtonWithNeighborsOnClicked);
		UseButtonWithNeighbors->OnHovered.AddDynamic(this, &USkillBattleMenu::UseButtonWithNeighborsOnHovered);
	}
	if (IsValid(BackButtonWithNeighbors)) {
		BackButtonWithNeighbors->OnClicked.AddDynamic(this, &USkillBattleMenu::BackButtonWithNeighborsOnClicked);
		BackButtonWithNeighbors->OnHovered.AddDynamic(this, &USkillBattleMenu::BackButtonWithNeighborsOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void USkillBattleMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (SkillsScrollBox->GetAllChildren().Num() > 0) {
			if (ULearnedSpellEntryWidget* EntryWidget = Cast<ULearnedSpellEntryWidget>(SkillsScrollBox->GetAllChildren()[0]); IsValid(EntryWidget)) {
				UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
		}
		else {
			UIManagerWorldSubsystem->PickedButton = UseButtonWithNeighbors;
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
		}
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void USkillBattleMenu::UseButtonWithNeighborsOnClicked()
{
	if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		UIManagerWorldSubsystem->SpellBattleMenuWidget->UseSpell(false);
}

void USkillBattleMenu::BackButtonWithNeighborsOnClicked()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		this->RemoveFromParent();
		if (!IsOpenedFromDetailedCharacterInfo) {
			UIManagerWorldSubsystem->BattleMenuWidget->AddToViewport();
			UIManagerWorldSubsystem->BattleMenuWidget->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
			UIManagerWorldSubsystem->SpellInfoWidget->RemoveFromParent();
			UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingSkill = false;
			UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingAction = true;
			UIManagerWorldSubsystem->SpellBattleMenuWidget->SetCreatedSpell(nullptr);
			if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
				RedemptionGameModeBase->GetBattleManager()->IsSelectingAllyAsTarget = false;
		}
		else {
			UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->AddToViewport();
			UseButtonWithNeighbors->SetVisibility(ESlateVisibility::Visible);
			UIManagerWorldSubsystem->SpellInfoWidget->RemoveFromParent();
		}
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
		if (IsOpenedFromDetailedCharacterInfo) {
			UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->GetToggleInfoButton();
			IsOpenedFromDetailedCharacterInfo = false;
		}
		else
			UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->BattleMenuWidget->GetAttackButton();
		if (UIManagerWorldSubsystem->PickedButton)
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->SpellInfoWidget->RemoveFromParent();
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void USkillBattleMenu::UseButtonWithNeighborsOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (SelectedSkillButton == UIManagerWorldSubsystem->PickedButton)
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
			else
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
		}
		UIManagerWorldSubsystem->PickedButton = UseButtonWithNeighbors;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void USkillBattleMenu::BackButtonWithNeighborsOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (SelectedSkillButton == UIManagerWorldSubsystem->PickedButton)
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
			else
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
		}
		UIManagerWorldSubsystem->PickedButton = BackButtonWithNeighbors;
		UIManagerWorldSubsystem->PickedButtonIndex = 1;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void USkillBattleMenu::ActivateNotification(const FText& NotificationText)
{
	NotificationTextBlock->SetText(NotificationText);
	PlayAnimation(NotificationShowAndHide);
}

const UScrollBox* USkillBattleMenu::GetSkillsScrollBox() const
{
	return SkillsScrollBox;
}

UButtonWithNeighbors* USkillBattleMenu::GetUseButtonWithNeighbors() const
{
	return UseButtonWithNeighbors;
}

UButtonWithNeighbors* USkillBattleMenu::GetBackButtonWithNeighbors() const
{
	return BackButtonWithNeighbors;
}

void USkillBattleMenu::AddSkillEntryToSkillsScrollBox(const class ASpell* const SpellToAdd)
{
	if(IsValid(LearnedSpellEntryClass))
		LearnedSpellEntryWidget = CreateWidget<ULearnedSpellEntryWidget>(GetWorld(), LearnedSpellEntryClass);
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase) && IsValid(LearnedSpellEntryWidget)){
		LearnedSpellEntryWidget->SetSpellTypeImage(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetSpellTypeImageTexture(SpellToAdd->GetTypeOfSpell()));
		LearnedSpellEntryWidget->SetSpellMainElementImage(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetMainSpellElementImageTexture(ElementsActions::FindSpellsMainElement(SpellToAdd->GetSpellElements())));
		LearnedSpellEntryWidget->SetSpellNameText(SpellToAdd->GetSpellName());
		LearnedSpellEntryWidget->EntrySpell = const_cast<ASpell*>(SpellToAdd);
		SkillsScrollBox->AddChild(LearnedSpellEntryWidget);
	}
}

void USkillBattleMenu::ResetSkillsScrollBox()
{
	SkillsScrollBox->ClearChildren();
	LearnedSpellEntryWidget = nullptr;
}
