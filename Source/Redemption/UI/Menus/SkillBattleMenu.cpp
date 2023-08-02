// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SkillBattleMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/HorizontalBoxSlot.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\EffectsSpellsAndSkillsManager.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

bool USkillBattleMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(FireElementButton))
		FireElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::FireElementButtonOnClicked);
	if (IsValid(WaterElementButton))
		WaterElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::WaterElementButtonOnClicked);
	if (IsValid(WindElementButton))
		WindElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::WindElementButtonOnClicked);
	if (IsValid(EarthElementButton))
		EarthElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::EarthElementButtonOnClicked);
	if (IsValid(LightningElementButton))
		LightningElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::LightningElementButtonOnClicked);
	if (IsValid(BloodElementButton))
		BloodElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::BloodElementButtonOnClicked);
	if (IsValid(HolyElementButton))
		HolyElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::HolyElementButtonOnClicked);
	if (IsValid(DarkElementButton))
		DarkElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::DarkElementButtonOnClicked);
	if (IsValid(AssaultSpellTypeButton))
		AssaultSpellTypeButton->OnClicked.AddDynamic(this, &USkillBattleMenu::AssaultSpellTypeButtonOnClicked);
	if (IsValid(DebuffSpellTypeButton))
		DebuffSpellTypeButton->OnClicked.AddDynamic(this, &USkillBattleMenu::DebuffSpellTypeButtonOnClicked);
	if (IsValid(RestorationSpellTypeButton))
		RestorationSpellTypeButton->OnClicked.AddDynamic(this, &USkillBattleMenu::RestorationSpellTypeButtonOnClicked);
	if (IsValid(BuffSpellTypeButton))
		BuffSpellTypeButton->OnClicked.AddDynamic(this, &USkillBattleMenu::BuffSpellTypeButtonOnClicked);
	if (IsValid(ShowResultSpellButton))
		ShowResultSpellButton->OnClicked.AddDynamic(this, &USkillBattleMenu::ShowResultSpellButtonOnClicked);
	if (IsValid(BackToSpellCreationButton))
		BackToSpellCreationButton->OnClicked.AddDynamic(this, &USkillBattleMenu::BackToSpellCreationButtonOnClicked);
	if (IsValid(UseButton))
		UseButton->OnClicked.AddDynamic(this, &USkillBattleMenu::UseButtonOnClicked);
	if (IsValid(BackButton))
		BackButton->OnClicked.AddDynamic(this, &USkillBattleMenu::BackButtonOnClicked);
	if (IsValid(ResetButton))
		ResetButton->OnClicked.AddDynamic(this, &USkillBattleMenu::ResetButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void USkillBattleMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillBattleMenu::FireElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/Elements/PNG/FireElementIcon.PNG"), ESpellElements::FIRE);
}

void USkillBattleMenu::WaterElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/Elements/PNG/WaterElementImage.PNG"), ESpellElements::WATER);
}

void USkillBattleMenu::WindElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/Elements/PNG/WindElementImage.PNG"), ESpellElements::WIND);
}

void USkillBattleMenu::EarthElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/Elements/PNG/EarthElementImage.PNG"), ESpellElements::EARTH);
}

void USkillBattleMenu::LightningElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/Elements/PNG/LightningElementImage.PNG"), ESpellElements::LIGHTNING);
}

void USkillBattleMenu::BloodElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/Elements/PNG/BloodElementImage.PNG"), ESpellElements::BLOOD);
}

void USkillBattleMenu::HolyElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/Elements/PNG/HolyElementImage.PNG"), ESpellElements::HOLY);
}

void USkillBattleMenu::DarkElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/Elements/PNG/DarkElementImage.PNG"), ESpellElements::DARK);
}

void USkillBattleMenu::AssaultSpellTypeButtonOnClicked()
{
	CreateSelectedSpellTypeWidgetAndAddToBorder(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/SpellsTypes/PNG/AssaultSpellTypeIcon.PNG"), ESpellType::ASSAULT);
	ShowElementsButtonsHideSpellTypesButtons();
}

void USkillBattleMenu::DebuffSpellTypeButtonOnClicked()
{
	CreateSelectedSpellTypeWidgetAndAddToBorder(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/SpellsTypes/PNG/DebuffSpellTypeIcon.PNG"), ESpellType::DEBUFF);
	ShowElementsButtonsHideSpellTypesButtons();
}

void USkillBattleMenu::RestorationSpellTypeButtonOnClicked()
{
	CreateSelectedSpellTypeWidgetAndAddToBorder(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/SpellsTypes/PNG/HealSpellTypeIcon.PNG"), ESpellType::RESTORATION);
	ShowElementsButtonsHideSpellTypesButtons();
}

void USkillBattleMenu::BuffSpellTypeButtonOnClicked()
{
	CreateSelectedSpellTypeWidgetAndAddToBorder(TEXT("D:/UnrealEngineProjects/Redemption/Content/Images/SpellsTypes/PNG/RestorationSpellTypeIcon.PNG"), ESpellType::BUFF);
	ShowElementsButtonsHideSpellTypesButtons();
}

void USkillBattleMenu::ShowResultSpellButtonOnClicked()
{
	if (SelectedSpellType != ESpellType::NONE && SelectedElementsHorizontalBox->GetAllChildren().Num() > 0) {
		CreateSpellAndSetCreatedSpell(SelectedSpellType);
		ShowCreatedSpellInformation(CreatedSpell);
	}
}

class ASpell* USkillBattleMenu::CreateBasicSpell(ESpellType SpellType, const TArray<ESpellElements>& SpellElements)
{
	ASpell* CreatedBasicSpell = NewObject<ASpell>();
	FString NameOfTheSpell{};
	if (SpellElements.Num() == 1)
		NameOfTheSpell.Append("Light ");
	else if (SpellElements.Num() > 1 && SpellElements.Num() < 4)
		NameOfTheSpell.Append("Pushy ");
	else if (SpellElements.Num() == 4)
		NameOfTheSpell.Append("Powerful ");
	else if (SpellElements.Num() == 5)
		NameOfTheSpell.Append("Severe ");
	if (SpellType != ESpellType::RESTORATION)
		NameOfTheSpell.Append(GetSpellElementName(FindSpellsMainElement()).ToString());
	switch (SpellType) {
	case ESpellType::ASSAULT:
		NameOfTheSpell.Append("assault spell");
		break;
	case ESpellType::DEBUFF:
		NameOfTheSpell.Append("resistance debuff spell");
		break;
	case ESpellType::BUFF:
		NameOfTheSpell.Append("resistance buff spell");
		break;
	case ESpellType::RESTORATION:
		NameOfTheSpell.Append("health restoration spell");
		break;
	}
	CreatedBasicSpell->SetSpellName(FText::FromString(NameOfTheSpell));
	CreatedBasicSpell->SetSpellElements(SpellElements);
	//Calculate mana cost;
	int16 BasicManaCost = 0;
	float CostModifier{};
	switch (SpellType) {
	case ESpellType::ASSAULT:
		CostModifier = 1.f;
		break;
	case ESpellType::DEBUFF:
		CostModifier = 2.f;
		break;
	case ESpellType::BUFF:
		CostModifier = 2.f;
		break;
	case ESpellType::RESTORATION:
		CostModifier = 1.5f;
		break;
	}
	for (int8 i = 0; i < SpellElements.Num(); i++)
		if(SpellElements[i] == ESpellElements::BLOOD)
			BasicManaCost += 10 * CostModifier;
		else
			BasicManaCost += 5 * CostModifier;
	CreatedBasicSpell->SetManaCost(BasicManaCost);
	CreatedBasicSpell->SetTypeOfSpell(SpellType);
	FString DescriptionOfTheSpell{};
	switch (SpellType) {
	case ESpellType::ASSAULT:
		DescriptionOfTheSpell.Append("Assault spell. Costs ");
		DescriptionOfTheSpell.AppendInt(BasicManaCost);
		DescriptionOfTheSpell.Append(" mana points.");
		break;
	case ESpellType::DEBUFF:
		DescriptionOfTheSpell.Append("Assault spell. Costs ");
		DescriptionOfTheSpell.AppendInt(BasicManaCost);
		DescriptionOfTheSpell.Append(" mana points.");
		break;
	case ESpellType::RESTORATION:
		DescriptionOfTheSpell.Append("Assault spell. Costs ");
		DescriptionOfTheSpell.AppendInt(BasicManaCost);
		DescriptionOfTheSpell.Append(" mana points. ");
		break;
	}
	CreatedBasicSpell->SetDescription(FText::FromString(DescriptionOfTheSpell));
	return CreatedBasicSpell;
}

class AAssaultSpell* USkillBattleMenu::CreateAssaultSpell(const TArray<ESpellElements>& SpellElements)
{
	AAssaultSpell* CreatedAssaultSpell = NewObject<AAssaultSpell>();
	*CreatedAssaultSpell = *CreateBasicSpell(SelectedSpellType, SelectedSpellElements);
	//Calculate damage
	int16 CreatedAssaultSpellDamage = 0;
	for (int8 i = 0; i < SpellElements.Num(); i++)
		if (SpellElements[i] == ESpellElements::BLOOD)
			CreatedAssaultSpellDamage += 20;
		else
			CreatedAssaultSpellDamage += 10;
	CreatedAssaultSpell->SetAttackValue(CreatedAssaultSpellDamage);
	CreatedAssaultSpell->SetContainedElements(SpellElements);
	CreatedAssaultSpell->SetSpellObjectClass(FindSpellObject(FindSpellsMainElement()));
	FString DescriptionOfTheSpell = CreatedAssaultSpell->GetDescription().ToString();
	DescriptionOfTheSpell.Append("Spell's damage: ");
	DescriptionOfTheSpell.AppendInt(CreatedAssaultSpell->GetAttackValue());
	return CreatedAssaultSpell;
}

class ADebuffSpell* USkillBattleMenu::CreateDebuffSpell(const TArray<ESpellElements>& SpellElements)
{
	return nullptr;
}

class ARestorationSpell* USkillBattleMenu::CreateRestorationSpell(const TArray<ESpellElements>& SpellElements)
{
	return nullptr;
}

class ABuffSpell* USkillBattleMenu::CreateBuffSpell(const TArray<ESpellElements>& SpellElements)
{
	return nullptr;
}

TSubclassOf<ASpellObject> USkillBattleMenu::FindSpellObject(ESpellElements MainSpellElement)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (AEffectsSpellsAndSkillsManager* EffectsSpellsAndSkillsManager = PlayerCharacter->GetEffectsSpellsAndSkillsManager(); IsValid(EffectsSpellsAndSkillsManager)) {
			if (MainSpellElement == ESpellElements::FIRE)
				return EffectsSpellsAndSkillsManager->GetFireMainElementSpellObjectClass();
			else if (MainSpellElement == ESpellElements::BLOOD)
				return EffectsSpellsAndSkillsManager->GetBloodMainElementSpellObjectClass();
			else if (MainSpellElement == ESpellElements::DARK)
				return EffectsSpellsAndSkillsManager->GetDarkMainElementSpellObjectClass();
			else if (MainSpellElement == ESpellElements::EARTH)
				return EffectsSpellsAndSkillsManager->GetEarthMainElementSpellObjectClass();
			else if (MainSpellElement == ESpellElements::HOLY)
				return EffectsSpellsAndSkillsManager->GetHolyMainElementSpellObjectClass();
			else if (MainSpellElement == ESpellElements::LIGHTNING)
				return EffectsSpellsAndSkillsManager->GetLightningMainElementSpellObjectClass();
			else if (MainSpellElement == ESpellElements::WATER)
				return EffectsSpellsAndSkillsManager->GetWaterMainElementSpellObjectClass();
			else if (MainSpellElement == ESpellElements::WIND)
				return EffectsSpellsAndSkillsManager->GetWindMainElementSpellObjectClass();
			else if (MainSpellElement == ESpellElements::MULTIELEMENTAL)
				return EffectsSpellsAndSkillsManager->GetMultielementalMainElementSpellObjectClass();
		}
	return nullptr;
}

ESpellElements USkillBattleMenu::FindSpellsMainElement()
{
	//Count elements
	TArray<int8> Counts{};
	TArray<ESpellElements> CountCorrespondingElement{};
	TArray<ESpellElements> CopyOfSelectedSpellElements = SelectedSpellElements;
	for (int8 j = 0; j < SelectedSpellElements.Num(); j++) {
		ESpellElements CurrentElement{};
		int8 CurrentCount = 0;
		for (int8 i = 0; i < CopyOfSelectedSpellElements.Num(); i++) {
			if (i == 0)
				CurrentElement = SelectedSpellElements[i];
			if (CopyOfSelectedSpellElements[i] == CurrentElement)
				CurrentCount++;
		}
		Counts.Add(CurrentCount);
		CountCorrespondingElement.Add(CurrentElement);
		CopyOfSelectedSpellElements.Remove(CurrentElement);
		if (CopyOfSelectedSpellElements.Num() == 0)
			break;
	}
	//Find the highest element count
	int8 HighestCount = 0;
	int8 HighestCountIndex{};
	for (int8 i = 0; i < Counts.Num(); i++) {
		if (Counts[i] > HighestCount) {
			HighestCount = Counts[i];
			HighestCountIndex = i;
		}
		//If there are elements with the same count, then return Multielemental.
		if (Counts[i] == HighestCount)
			return ESpellElements::MULTIELEMENTAL;
	}
	if (HighestCountIndex < CountCorrespondingElement.Num())
		return CountCorrespondingElement[HighestCountIndex];
	return ESpellElements::NONE;
}

FText USkillBattleMenu::GetSpellElementName(ESpellElements MainSpellElement)
{
	TEnumAsByte<ESpellElements> EnumVar = MainSpellElement;
	FText MyEnumValueText;
	UEnum::GetDisplayValueAsText(EnumVar, MyEnumValueText);
	return MyEnumValueText;
}

void USkillBattleMenu::BackToSpellCreationButtonOnClicked()
{
	FireElementButton->SetVisibility(ESlateVisibility::Visible);
	WindElementButton->SetVisibility(ESlateVisibility::Visible);
	WaterElementButton->SetVisibility(ESlateVisibility::Visible);
	EarthElementButton->SetVisibility(ESlateVisibility::Visible);
	LightningElementButton->SetVisibility(ESlateVisibility::Visible);
	HolyElementButton->SetVisibility(ESlateVisibility::Visible);
	BloodElementButton->SetVisibility(ESlateVisibility::Visible);
	DarkElementButton->SetVisibility(ESlateVisibility::Visible);
	ShowResultSpellButton->SetVisibility(ESlateVisibility::Visible);
	UseButton->SetVisibility(ESlateVisibility::Visible);
	BackButton->SetVisibility(ESlateVisibility::Visible);
	ResetButton->SetVisibility(ESlateVisibility::Visible);
	SelectedElementsBorder->SetVisibility(ESlateVisibility::Visible);
	BackToSpellCreationButton->SetVisibility(ESlateVisibility::Hidden);
	SpellInfoBorder->SetVisibility(ESlateVisibility::Hidden);
}


void USkillBattleMenu::BackButtonOnClicked()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget(); IsValid(BattleMenu)) {
			this->RemoveFromParent();
			BattleMenu->IsChoosingSpell = false;
			BattleMenu->IsAttackingWithSpell = false;
			BattleMenu->IsChoosingAction = true;
			BattleMenu->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
			BattleMenu->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			if (IsValid(PlayerCharacter->GetUIManager())) {
				PlayerCharacter->GetUIManager()->PickedButton = BattleMenu->GetAttackButton();
				PlayerCharacter->GetUIManager()->PickedButtonIndex = 0;
			}
			ResetButtonOnClicked();
			BattleMenu->AddToViewport();
			HideNotificationAndClearItsTimer();
	}
}

void USkillBattleMenu::LearnedSpellsJournalButtonOnClicked()
{
	this->RemoveFromParent();
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		PlayerCharacter->GetLearnedSpellsJournalMenu()->AddToViewport();
}

void USkillBattleMenu::ResetButtonOnClicked()
{
	for (UWidget* Child : SelectedElementsHorizontalBox->GetAllChildren())
		Child->ConditionalBeginDestroy();
	SelectedElementsHorizontalBox->ClearChildren();
	SelectedSpellElements.Empty();
	for (UWidget* Child : SelectedSpellTypeBorder->GetAllChildren())
		Child->ConditionalBeginDestroy();
	SelectedSpellTypeBorder->ClearChildren();
	SelectedSpellType = ESpellType::NONE;
	ShowSpellTypesButtonsHideElementsButtons();
	CreatedSpell = nullptr;
	HideNotificationAndClearItsTimer();
}

void USkillBattleMenu::CreateSelectedElementWidgetAndAddToHorizontalBox(FString Filepath, ESpellElements Element)
{
	if (SelectedElementsHorizontalBox->GetAllChildren().Num() < 5) {
		if (IsValid(SelectedElementEntryWidgetClass))
			SelectedElementEntryWidget = CreateWidget<USelectedElementEntryWidget>(GetWorld(), SelectedElementEntryWidgetClass);
		if (IsValid(SelectedElementEntryWidget)) {
			SelectedElementEntryWidget->SetElement(Element);
			UTexture2D* MyAsset = UKismetRenderingLibrary::ImportFileAsTexture2D(GetWorld(), Filepath);
			if (IsValid(MyAsset))
				SelectedElementEntryWidget->GetMainImage()->Brush.SetResourceObject(MyAsset);
			SelectedElementEntryWidget->GetMainImage()->Brush.SetImageSize(FVector2D(120, 100));
			SelectedElementEntryWidget->AddToViewport();
			SelectedElementsHorizontalBox->AddChildToHorizontalBox(SelectedElementEntryWidget);
			SelectedElementEntryWidget->SetPadding(FMargin(20.f, 0.f, 0.f, 0.f));
			UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(SelectedElementEntryWidget)->SetSize(ESlateSizeRule::Fill);
			SelectedSpellElements.Add(Element);
		}
	}
}

void USkillBattleMenu::CreateSelectedSpellTypeWidgetAndAddToBorder(FString Filepath, ESpellType SpellType)
{
	if (!SelectedSpellTypeBorder->HasAnyChildren()) {
		if (IsValid(SelectedSpellTypeEntryWidgetClass))
			SelectedSpellTypeEntryWidget = CreateWidget<USelectedSpellTypeEntryWidget>(GetWorld(), SelectedSpellTypeEntryWidgetClass);
		if (IsValid(SelectedSpellTypeEntryWidget)) {
			SelectedSpellTypeEntryWidget->SetSpellType(SpellType);
			UTexture2D* MyAsset = UKismetRenderingLibrary::ImportFileAsTexture2D(GetWorld(), Filepath);
			if (IsValid(MyAsset))
				SelectedSpellTypeEntryWidget->GetMainImage()->Brush.SetResourceObject(MyAsset);
			SelectedSpellTypeEntryWidget->GetMainImage()->Brush.SetImageSize(FVector2D(120, 100));
			SelectedSpellTypeEntryWidget->AddToViewport();
			SelectedSpellTypeBorder->AddChild(SelectedSpellTypeEntryWidget);
			SelectedSpellTypeEntryWidget->SetPadding(FMargin(20.f, 0.f, 0.f, 0.f));
			SelectedSpellType = SpellType;
		}
	}
}

void USkillBattleMenu::CreateSpellAndSetCreatedSpell(ESpellType TypeOfTheSpell)
{
	if (TypeOfTheSpell == ESpellType::ASSAULT)
		CreatedSpell = CreateAssaultSpell(SelectedSpellElements);
	else if (TypeOfTheSpell == ESpellType::DEBUFF)
		CreatedSpell = CreateDebuffSpell(SelectedSpellElements);
	else if (TypeOfTheSpell == ESpellType::RESTORATION)
		CreatedSpell = CreateRestorationSpell(SelectedSpellElements);
	else if (TypeOfTheSpell == ESpellType::BUFF)
		CreatedSpell = CreateBuffSpell(SelectedSpellElements);
}

void USkillBattleMenu::ShowCreatedSpellInformation(ASpell* const& SpellToShow)
{
	if (IsValid(SpellToShow)) {
		FireElementButton->SetVisibility(ESlateVisibility::Hidden);
		WindElementButton->SetVisibility(ESlateVisibility::Hidden);
		WaterElementButton->SetVisibility(ESlateVisibility::Hidden);
		EarthElementButton->SetVisibility(ESlateVisibility::Hidden);
		LightningElementButton->SetVisibility(ESlateVisibility::Hidden);
		HolyElementButton->SetVisibility(ESlateVisibility::Hidden);
		BloodElementButton->SetVisibility(ESlateVisibility::Hidden);
		DarkElementButton->SetVisibility(ESlateVisibility::Hidden);
		ShowResultSpellButton->SetVisibility(ESlateVisibility::Hidden);
		UseButton->SetVisibility(ESlateVisibility::Hidden);
		BackButton->SetVisibility(ESlateVisibility::Hidden);
		ResetButton->SetVisibility(ESlateVisibility::Hidden);
		SelectedElementsBorder->SetVisibility(ESlateVisibility::Hidden);
		BackToSpellCreationButton->SetVisibility(ESlateVisibility::Visible);
		SpellInfoBorder->SetVisibility(ESlateVisibility::Visible);
		FString SpellNameString = FString("Name: ");
		SpellNameString.Append(SpellToShow->GetSpellName().ToString());
		SpellNameTextBlock->SetText(FText::FromString(SpellNameString));
		FString SpellTypeString = FString("Type: ");
		FString SpellEffectValueString{};
		switch (SpellToShow->GetTypeOfSpell()) {
		case ESpellType::ASSAULT:
			if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(SpellToShow); IsValid(AssaultSpell)) {
				SpellTypeString.Append("assault spell");
				SpellEffectValueString = "Damage: ";
				SpellEffectValueString.AppendInt(AssaultSpell->GetAttackValue());
			}
			break;
		case ESpellType::BUFF:
			if (ABuffSpell* BuffSpell = Cast<ABuffSpell>(SpellToShow); IsValid(BuffSpell)) {
				switch (BuffSpell->GetTypeOfBuff()) {
				case EBuffType::ATTACK:
					SpellTypeString.Append("attack boost spell");
					break;
				case EBuffType::ARMOR:
					SpellTypeString.Append("armor boost spell");
					break;
				case EBuffType::AGILITY:
					SpellTypeString.Append("agility boost spell");
					break;
				}
				SpellEffectValueString = "Multiplier: ";
				SpellEffectValueString.AppendInt(BuffSpell->GetBuffValue());
			}
			break;
		case ESpellType::DEBUFF:
			if (ADebuffSpell* DebuffSpell = Cast<ADebuffSpell>(SpellToShow); IsValid(DebuffSpell)) {
				switch (DebuffSpell->GetTypeOfDebuff()) {
				case EDebuffType::ATTACK:
					SpellTypeString.Append("attack boost spell");
					break;
				case EDebuffType::ARMOR:
					SpellTypeString.Append("armor boost spell");
					break;
				case EDebuffType::AGILITY:
					SpellTypeString.Append("agility boost spell");
					break;
				}
				SpellEffectValueString = "Multiplier: ";
				SpellEffectValueString.AppendInt(DebuffSpell->GetDebuffValue());
			}
			break;
		case ESpellType::RESTORATION:
			if (ARestorationSpell* RestorationSpell = Cast<ARestorationSpell>(SpellToShow); IsValid(RestorationSpell)) {
				if (RestorationSpell->GetTypeOfRestoration() == SpellRestorationType::HEALTH) {
					SpellTypeString.Append("healing spell");
					SpellEffectValueString = "HP: ";
				}
				else if (RestorationSpell->GetTypeOfRestoration() == SpellRestorationType::MANA) {
					SpellTypeString.Append("mana restoration spell");
					SpellEffectValueString = "Mana: ";
				}
				SpellEffectValueString.AppendInt(RestorationSpell->GetRestorationValuePercent());
				SpellEffectValueString.AppendChar('%');
			}
			break;

		}
		SpellTypeTextBlock->SetText(FText::FromString(SpellTypeString));
		SpellEffectValueTextBlock->SetText(FText::FromString(SpellEffectValueString));
		FString SpellManaCostString = FString("Mana cost: ");
		SpellManaCostString.AppendInt(SpellToShow->GetManaCost());
		SpellManaCostTextBlock->SetText(FText::FromString(SpellManaCostString));
		SpellDescriptionTextBlock->SetText(SpellToShow->GetDescription());
		HideNotificationAndClearItsTimer();
	}
}

void USkillBattleMenu::ShowSpellTypesButtonsHideElementsButtons()
{
	AssaultSpellTypeButton->SetVisibility(ESlateVisibility::Visible);
	DebuffSpellTypeButton->SetVisibility(ESlateVisibility::Visible);
	RestorationSpellTypeButton->SetVisibility(ESlateVisibility::Visible);
	BuffSpellTypeButton->SetVisibility(ESlateVisibility::Visible);
	FireElementButton->SetVisibility(ESlateVisibility::Hidden);
	WaterElementButton->SetVisibility(ESlateVisibility::Hidden);
	WindElementButton->SetVisibility(ESlateVisibility::Hidden);
	EarthElementButton->SetVisibility(ESlateVisibility::Hidden);
	LightningElementButton->SetVisibility(ESlateVisibility::Hidden);
	BloodElementButton->SetVisibility(ESlateVisibility::Hidden);
	HolyElementButton->SetVisibility(ESlateVisibility::Hidden);
	DarkElementButton->SetVisibility(ESlateVisibility::Hidden);
}

void USkillBattleMenu::ShowElementsButtonsHideSpellTypesButtons()
{
	FireElementButton->SetVisibility(ESlateVisibility::Visible);
	WaterElementButton->SetVisibility(ESlateVisibility::Visible);
	WindElementButton->SetVisibility(ESlateVisibility::Visible);
	EarthElementButton->SetVisibility(ESlateVisibility::Visible);
	LightningElementButton->SetVisibility(ESlateVisibility::Visible);
	BloodElementButton->SetVisibility(ESlateVisibility::Visible);
	HolyElementButton->SetVisibility(ESlateVisibility::Visible);
	DarkElementButton->SetVisibility(ESlateVisibility::Visible);
	AssaultSpellTypeButton->SetVisibility(ESlateVisibility::Hidden);
	DebuffSpellTypeButton->SetVisibility(ESlateVisibility::Hidden);
	RestorationSpellTypeButton->SetVisibility(ESlateVisibility::Hidden);
	BuffSpellTypeButton->SetVisibility(ESlateVisibility::Hidden);
}

void USkillBattleMenu::UseButtonOnClicked()
{
	if (SelectedSpellType != ESpellType::NONE && SelectedElementsHorizontalBox->GetAllChildren().Num() > 0) {
		if (!IsValid(CreatedSpell))
			CreateSpellAndSetCreatedSpell(SelectedSpellType);
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		ABattleManager* BattleManager{};
		UBattleMenu* BattleMenu{};
		AUIManager* UIManager{};
		if (IsValid(PlayerCharacter)) {
			BattleManager = PlayerCharacter->GetBattleManager();
			BattleMenu = PlayerCharacter->GetBattleMenuWidget();
			UIManager = PlayerCharacter->GetUIManager();
		}
		if (IsValid(CreatedSpell) && IsValid(BattleManager) && IsValid(BattleMenu) && IsValid(UIManager) && IsValid(PlayerCharacter)) {
			if (PlayerCharacter->CurrentMana >= CreatedSpell->GetManaCost()) {
				switch (CreatedSpell->GetTypeOfSpell()) {
				case ESpellType::ASSAULT:
					if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(CreatedSpell); IsValid(AssaultSpell))
						AssaultSpellUse(AssaultSpell, BattleManager, BattleMenu, UIManager);
					break;
				case ESpellType::RESTORATION:
					if (ARestorationSpell* RestorationSpell = Cast<ARestorationSpell>(CreatedSpell); IsValid(RestorationSpell))
						RestorationSpellUse(RestorationSpell, BattleManager, BattleMenu, UIManager);
					break;
				case ESpellType::BUFF:
					if (ABuffSpell* BuffSpell = Cast<ABuffSpell>(CreatedSpell); IsValid(BuffSpell))
						BuffSpellUse(BuffSpell, BattleManager, BattleMenu, UIManager);
					break;
				case ESpellType::DEBUFF:
					if (ADebuffSpell* DebuffSpell = Cast<ADebuffSpell>(CreatedSpell); IsValid(DebuffSpell))
						DebuffSpellUse(DebuffSpell, BattleManager, BattleMenu, UIManager);
					break;
				}
			}
			else
				CreateNotification(FText::FromString("Not enough mana!!!"));
		}
	}
}

void USkillBattleMenu::AssaultSpellUse(const class AAssaultSpell* const& SpellToUser, class ABattleManager* const& BattleManager,
	class UBattleMenu* const& BattleMenu, class AUIManager* const& UIManager)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if(IsValid(PlayerCharacter)) {
		BattleMenu->AddToViewport();
		//Remove player and menu render and turn on target selection
		BattleMenu->IsPreparingToAttack = true;
		BattleMenu->IsChoosingSpell = false;
		BattleMenu->IsAttackingWithSpell = true;
		//PlayerCharacter->GetMesh()->bHiddenInGame = true;
		BattleManager->SelectedEnemy = PlayerCharacter->GetBattleManager()->BattleEnemies[0];
		BattleManager->SelectedEnemyIndex = 0;
		this->RemoveFromParent();
		BattleMenu->GetCenterMark()->SetVisibility(ESlateVisibility::Visible);
		BattleMenu->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
		BattleMenu->GetAttackMenuBorder()->SetVisibility(ESlateVisibility::Visible);
		BattleMenu->GetLeftRightMenuBorder()->SetVisibility(ESlateVisibility::Visible);
		BattleMenu->GetAttackButton()->SetBackgroundColor(FColor(1, 1, 1, 1));
		BattleManager->SelectedEnemy->GetEnemyHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
		UIManager->PickedButton = BattleMenu->GetAttackActionButton();
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManager->PickedButtonIndex = 0;
		BattleManager->SetCanTurnBehindPlayerCameraToEnemy(true);
		BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
		BattleMenu->GetEnemyNameTextBlock()->SetText(FText::FromName(PlayerCharacter->GetBattleManager()->BattleEnemies[0]->GetCharacterName()));
		PlayerCharacter->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), BattleManager->SelectedEnemy->GetActorLocation()));
	}
}

void USkillBattleMenu::RestorationSpellUse(const class ARestorationSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
	class UBattleMenu* const& BattleMenu, class AUIManager* const& UIManager)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if(IsValid(PlayerCharacter)) {
		bool SpellHasBeenUsed = false;
		if (SpellToUse->GetTypeOfRestoration() == SpellRestorationType::HEALTH && PlayerCharacter->CurrentHP < PlayerCharacter->MaxHP) {
			PlayerCharacter->CurrentHP += (PlayerCharacter->MaxHP * SpellToUse->GetRestorationValuePercent() / 100);
			SpellHasBeenUsed = true;
			if (PlayerCharacter->CurrentHP > PlayerCharacter->MaxHP)
				PlayerCharacter->CurrentHP = PlayerCharacter->MaxHP;
		}
		else if (SpellToUse->GetTypeOfRestoration() == SpellRestorationType::MANA && PlayerCharacter->CurrentMana < PlayerCharacter->MaxMana) {
			PlayerCharacter->CurrentMana += (PlayerCharacter->MaxMana * SpellToUse->GetRestorationValuePercent() / 100);
			SpellHasBeenUsed = true;
			if (PlayerCharacter->CurrentMana > PlayerCharacter->MaxMana)
				PlayerCharacter->CurrentMana = PlayerCharacter->MaxMana;
		}
		else if(SpellToUse->GetTypeOfRestoration() == SpellRestorationType::MANA && PlayerCharacter->CurrentMana >= PlayerCharacter->MaxMana)
			CreateNotification(FText::FromString("Your mana is already full!!!"));
		else if (SpellToUse->GetTypeOfRestoration() == SpellRestorationType::HEALTH && PlayerCharacter->CurrentHP >= PlayerCharacter->MaxHP)
			CreateNotification(FText::FromString("Your health is already full!!!"));
		if (SpellHasBeenUsed) {
			BattleMenu->AddToViewport();
			UIManager->PickedButtonIndex = 0;
			if (IsValid(UIManager->PickedButton))
				UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->UseHealOrBoostSoundCue);
			this->RemoveFromParent();
			GetWorld()->GetTimerManager().SetTimer(UseSpellTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
			PlayerCharacter->CurrentMana -= SpellToUse->GetManaCost();
			BattleMenu->IsChoosingSpell = false;
			if (PlayerCharacter->CurrentMana < 0)
				PlayerCharacter->CurrentMana = 0;
			ResetButtonOnClicked();
		}
	}
}

void USkillBattleMenu::BuffSpellUse(const class ABuffSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
	class UBattleMenu* const& BattleMenu, class AUIManager* const& UIManager)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		bool SpellHasBeenUsed = false;
		AEffect* NewEffect = NewObject<AEffect>(this, SpellToUse->GetEffectClass());
		PlayerCharacter->Effects.Add(NewEffect);
		BattleMenu->AddToViewport();
		UIManager->PickedButtonIndex = 0;
		if (IsValid(UIManager->PickedButton))
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->UseHealOrBoostSoundCue);
		this->RemoveFromParent();
		GetWorld()->GetTimerManager().SetTimer(UseSpellTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
		PlayerCharacter->CurrentMana -= SpellToUse->GetManaCost();
		BattleMenu->IsChoosingSpell = false;
		if (PlayerCharacter->CurrentMana < 0)
			PlayerCharacter->CurrentMana = 0;
		ResetButtonOnClicked();
	}
}

void USkillBattleMenu::DebuffSpellUse(const class ADebuffSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
	class UBattleMenu* const& BattleMenu, class AUIManager* const& UIManager)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (IsValid(PlayerCharacter)) {
		BattleMenu->AddToViewport();
		//Remove player and menu render and turn on target selection
		BattleMenu->IsPreparingToAttack = true;
		BattleMenu->IsChoosingSpell = false;
		BattleMenu->IsAttackingWithSpell = true;
		//PlayerCharacter->GetMesh()->bHiddenInGame = true;
		BattleManager->SelectedEnemy = PlayerCharacter->GetBattleManager()->BattleEnemies[0];
		BattleManager->SelectedEnemyIndex = 0;
		this->RemoveFromParent();
		BattleMenu->GetCenterMark()->SetVisibility(ESlateVisibility::Visible);
		BattleMenu->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
		BattleMenu->GetAttackMenuBorder()->SetVisibility(ESlateVisibility::Visible);
		BattleMenu->GetLeftRightMenuBorder()->SetVisibility(ESlateVisibility::Visible);
		BattleMenu->GetAttackButton()->SetBackgroundColor(FColor(1, 1, 1, 1));
		BattleManager->SelectedEnemy->GetEnemyHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
		UIManager->PickedButton = BattleMenu->GetAttackActionButton();
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManager->PickedButtonIndex = 0;
		BattleManager->SetCanTurnBehindPlayerCameraToEnemy(true);
		BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
		BattleMenu->GetEnemyNameTextBlock()->SetText(FText::FromName(PlayerCharacter->GetBattleManager()->BattleEnemies[0]->GetCharacterName()));
		PlayerCharacter->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), BattleManager->SelectedEnemy->GetActorLocation()));
	}
}

void USkillBattleMenu::CreateNotification(const FText& NotificationText)
{
	NotificationBorder->SetVisibility(ESlateVisibility::Visible);
	NotificationTextBlock->SetText(NotificationText);
	GetWorld()->GetTimerManager().SetTimer(HideNotificationTimerHandle, this, &USkillBattleMenu::HideNotificationAndClearItsTimer, 3.f, false);
}

void USkillBattleMenu::HideNotificationAndClearItsTimer()
{
	NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
	NotificationTextBlock->SetText(FText::FromString(""));
	GetWorld()->GetTimerManager().ClearTimer(HideNotificationTimerHandle);
}

void USkillBattleMenu::SelectedSpellElementsRemoveSingleItem(ESpellElements ElementToRemove)
{
	SelectedSpellElements.RemoveSingle(ElementToRemove);
}

void USkillBattleMenu::SetSelectedSpellType(ESpellType NewSpellType)
{
	SelectedSpellType = NewSpellType;
}

void USkillBattleMenu::SetCreatedSpell(ASpell* NewSpell)
{
	CreatedSpell = NewSpell;
}

UHorizontalBox* USkillBattleMenu::GetSelectedElementsHorizontalBox() const
{
	return SelectedElementsHorizontalBox;
}

UBorder* USkillBattleMenu::GetSelectedSpellTypeBorder() const
{
	return SelectedSpellTypeBorder;
}

TArray<ESpellElements> USkillBattleMenu::GetSelectedSpellElements() const
{
	return SelectedSpellElements;
}

ASpell* USkillBattleMenu::GetCreatedSpell() const
{
	return CreatedSpell;
}
