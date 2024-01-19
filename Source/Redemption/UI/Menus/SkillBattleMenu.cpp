
#include "SkillBattleMenu.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "..\UI\HUD\FloatingManaBarWidget.h"

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
}

void USkillBattleMenu::UseButtonWithNeighborsOnClicked()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		PlayerCharacter->GetSpellBattleMenuWidget()->UseSpell();
}

void USkillBattleMenu::BackButtonWithNeighborsOnClicked()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		this->RemoveFromParent();
		if (!IsOpenedFromDetailedCharacterInfo) {
			PlayerCharacter->GetBattleMenuWidget()->AddToViewport();
			PlayerCharacter->GetBattleMenuWidget()->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
			PlayerCharacter->GetSpellInfoWidget()->RemoveFromParent();
			PlayerCharacter->GetBattleMenuWidget()->IsChoosingSkill = false;
			PlayerCharacter->GetBattleManager()->IsSelectingAllyAsTarget = false;
			PlayerCharacter->GetBattleMenuWidget()->IsChoosingAction = true;
			PlayerCharacter->GetSpellBattleMenuWidget()->SetCreatedSpell(nullptr);
			HideNotificationAndClearItsTimer();
		}
		else {
			PlayerCharacter->GetDetailedCharacterInfoMenuWidget()->AddToViewport();
			UseButtonWithNeighbors->SetVisibility(ESlateVisibility::Visible);
			PlayerCharacter->GetSpellInfoWidget()->RemoveFromParent();
			IsOpenedFromDetailedCharacterInfo = false;
		}
		if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
			if (IsOpenedFromDetailedCharacterInfo)
				UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetBattleMenuWidget()->GetAttackButton();
			else
				UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetDetailedCharacterInfoMenuWidget()->GetToggleInfoButton();
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
		}
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

void USkillBattleMenu::HideNotificationAndClearItsTimer()
{
	NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
	NotificationTextBlock->SetText(FText::FromString(""));
	GetWorld()->GetTimerManager().ClearTimer(HideNotificationTimerHandle);
}

void USkillBattleMenu::CreateNotification(const FText& NotificationText)
{
	NotificationBorder->SetVisibility(ESlateVisibility::Visible);
	NotificationTextBlock->SetText(NotificationText);
	GetWorld()->GetTimerManager().SetTimer(HideNotificationTimerHandle, this, &USkillBattleMenu::HideNotificationAndClearItsTimer, 3.f, false);
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
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter) && IsValid(LearnedSpellEntryWidget)) {
		LearnedSpellEntryWidget->SetSpellTypeImage(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetSpellTypeImageTexture(SpellToAdd->GetTypeOfSpell()));
		LearnedSpellEntryWidget->SetSpellMainElementImage(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetMainSpellElementImageTexture(ElementsActions::FindSpellsMainElement(SpellToAdd->GetSpellElements())));
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
