// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "..\UI\Screens\LoadingScreen.h"
#include "..\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "..\UI\Miscellaneous\CharacterPerks.h"
#include "..\UI\Menus\PerksLevelingUpMenu.h"
#include "..\UI\Menus\CharacterCreationMenu.h"
#include "..\UI\Menus\ContainerInventoryMenu.h"
#include "..\UI\Menus\MainMenu.h"
#include "..\Dynamics\World\LootInTheWorld.h"
#include "..\Dynamics\Logic\Interfaces\DialogueActionsInterface.h"
#include "..\Characters\NonCombat\TownNPC.h"
#include "..\Dynamics\World\Items\Equipment\WeaponItem.h"
#include "..\Dynamics\World\Items\Equipment\ArmorItem.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\EffectWithPlainModifier.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "..\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "..\Characters\Combat\CombatPlayerCharacter.h"
#include "..\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "GameFramework/TouchInterface.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Components/StackBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "..\Miscellaneous\InventoryActions.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "EngineUtils.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Redemption/UI/Menus/JournalMenu.h"
#include "Redemption/Characters/NonCombat/NonCombatEnemyNPC.h"
#include "Redemption/UI/Screens/StartFinishGameScreen.h"

APlayerCharacter::APlayerCharacter() {
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 91.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...    
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character    
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	PrimaryActorTick.bTickEvenWhenPaused = true;

	InitializeSkills();
	InitializeStats();
	InitializeSkillsProgress();
}

void APlayerCharacter::BeginPlay() 
{
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController();
	if(IsValid(PlayerController))
		PlayerSkeletalMesh = PlayerController->GetPawn()->FindComponentByClass<USkeletalMeshComponent>();
	if (IsValid(PlayerSkeletalMesh))
		PlayerAnimInstance = Cast<UPlayerCharacterAnimInstance>(PlayerSkeletalMesh->GetAnimInstance());
	UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	//Creating UI widgets...
	if(IsValid(PlayerController) && IsValid(UIManagerWorldSubsystem))
		if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
			PlayerController->SetTickableWhenPaused(true);
			if (IsValid(RedemptionGameModeBase->GetForwardRayInfoClass()))
				UIManagerWorldSubsystem->ForwardRayInfoWidget = CreateWidget<UForwardRayInfo>(PlayerController, RedemptionGameModeBase->GetForwardRayInfoClass());
			if (IsValid(RedemptionGameModeBase->GetInventoryScrollBoxEntryClass()))
				UIManagerWorldSubsystem->InventoryScrollBoxEntryWidget = CreateWidget<UInventoryScrollBoxEntryWidget>(PlayerController, RedemptionGameModeBase->GetInventoryScrollBoxEntryClass());
			if (IsValid(RedemptionGameModeBase->GetInventoryMenuClass()))
				UIManagerWorldSubsystem->InventoryMenuWidget = CreateWidget<UInventoryMenu>(PlayerController, RedemptionGameModeBase->GetInventoryMenuClass());
			if (IsValid(RedemptionGameModeBase->GetAlliesInfoBarsClass()))
				UIManagerWorldSubsystem->AlliesInfoBarsWidget = CreateWidget<UAlliesInfoBars>(PlayerController, RedemptionGameModeBase->GetAlliesInfoBarsClass());
			if (IsValid(RedemptionGameModeBase->GetNotificationClass()))
				UIManagerWorldSubsystem->NotificationWidget = CreateWidget<UNotification>(PlayerController, RedemptionGameModeBase->GetNotificationClass());
			if (IsValid(RedemptionGameModeBase->GetLearnedSpellsJournalMenuClass()))
				UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget = CreateWidget<ULearnedSpellsJournalMenu>(PlayerController, RedemptionGameModeBase->GetLearnedSpellsJournalMenuClass());
			if (IsValid(RedemptionGameModeBase->GetSpellInfoClass()))
				UIManagerWorldSubsystem->SpellInfoWidget = CreateWidget<USpellInfo>(PlayerController, RedemptionGameModeBase->GetSpellInfoClass());
			if (IsValid(RedemptionGameModeBase->GetCombatCharacterInfoMenuClass()))
				UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget = CreateWidget<UCombatCharacterInfoMenu>(PlayerController, RedemptionGameModeBase->GetCombatCharacterInfoMenuClass());
			if (IsValid(RedemptionGameModeBase->GetSaveLoadGameMenuClass()))
				UIManagerWorldSubsystem->SaveLoadGameMenuWidget = CreateWidget<USaveLoadGameMenu>(PlayerController, RedemptionGameModeBase->GetSaveLoadGameMenuClass());
			if (IsValid(RedemptionGameModeBase->GetJournalMenuClass()))
				UIManagerWorldSubsystem->JournalMenuWidget = CreateWidget<UJournalMenu>(PlayerController, RedemptionGameModeBase->GetJournalMenuClass());
		}
	UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->PercentDelegate.Clear();
	UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->PercentDelegate.Clear();
	UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesInfoVerticalBoxes()[0]->SetVisibility(ESlateVisibility::Visible);
	UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->PercentDelegate.BindUFunction(this, "GetHealthPercentage");
	UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->PercentDelegate.BindUFunction(this, "GetManaPercentage");
	UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesNameTextBlockes()[0]->SetText(FText::FromString(CharacterName));
	//Level change logic
	if (auto* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance)) {
		Execute_LoadObjectFromGameInstance(this, RedemptionGameInstance);
		RedemptionGameInstance->IsChangingLevel = true;
	}
	//Main Menu Turn On Logic
	FString MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (MapName == "MainMenu") {
		if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
			if (IsValid(RedemptionGameModeBase->GetMainMenuClass()))
				UIManagerWorldSubsystem->MainMenuWidget = CreateWidget<UMainMenu>(PlayerController, RedemptionGameModeBase->GetMainMenuClass());
		UIManagerWorldSubsystem->MainMenuWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		//PlayerController->ActivateTouchInterface(PlayerCharacter->GetEmptyTouchInterface());
		UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->MainMenuWidget->GetNewGameButton();
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		CanOpenOtherMenus = false;
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UIManagerWorldSubsystem->MainMenuWidget, EMouseLockMode::DoNotLock);
	}
	else if (MapName == "Dungeon") {
		UIManagerWorldSubsystem->AlliesInfoBarsWidget->AddToViewport();
		UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->SynchronizeProperties();
		UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->SynchronizeProperties();
	}
	verify(PlayerController);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!IsInDialogue)
		CheckForwardRayHitResult();
}
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (CanOpenOtherMenus) {
		const FVector2D MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);
		//Forward/Backward direction
		if (MoveValue.Y != 0.f) {
			//Get forward vector
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(Direction, MoveValue.Y);
		}
		//Right/Left direction
		if (MoveValue.X != 0.f) {
			//Get right vector
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(Direction, MoveValue.X);
		}
	}
}
void APlayerCharacter::Look(const FInputActionValue& Value) {
	if (CanOpenOtherMenus) {
		if (IsValid(Controller)) {
			const FVector2D LookValue = Value.Get<FVector2D>();
			if (LookValue.X != 0.f) {
				AddControllerYawInput(LookValue.X);
			}
			if (LookValue.Y != 0.f) {
				AddControllerPitchInput(LookValue.Y);
			}
		}
	}
}
void APlayerCharacter::InputJump()
{
	if (CanOpenOtherMenus)
		Jump();
}

void APlayerCharacter::InputSpellUseSaveLoad()
{
	if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport())
		UIManagerWorldSubsystem->SpellBattleMenuWidget->UseButtonOnClicked();
	else if (IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget) && UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport())
		UIManagerWorldSubsystem->SkillBattleMenuWidget->UseButtonWithNeighborsOnClicked();
	else if (IsValid(UIManagerWorldSubsystem->SaveLoadGameMenuWidget) && UIManagerWorldSubsystem->SaveLoadGameMenuWidget->IsInViewport())
		UIManagerWorldSubsystem->SaveLoadGameMenuWidget->SaveLoadButtonWithNeighborsOnClicked();
}

void APlayerCharacter::InputUniqueSpellUseSpellInfoToggle()
{

	if(IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport() &&
		UIManagerWorldSubsystem->SpellBattleMenuWidget->GetToggleSpellInfoButton()->GetVisibility() == ESlateVisibility::Visible)
			UIManagerWorldSubsystem->SpellBattleMenuWidget->ToggleSpellInfoButtonOnClicked();
	else if(IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport())
		UIManagerWorldSubsystem->SpellBattleMenuWidget->UseUniqueSpellButtonOnClicked();
}

void APlayerCharacter::InputSpellReset()
{
	if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport()) {
		UIManagerWorldSubsystem->SpellBattleMenuWidget->ResetButtonOnClicked();
		UIManagerWorldSubsystem->SpellBattleMenuWidget->ResetUIKeyboardControlLogic();
	}
}

void APlayerCharacter::InputOpenLearnedSpells()
{
	if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport())
		UIManagerWorldSubsystem->SpellBattleMenuWidget->LearnedSpellsJournalButtonOnClicked();
}

void APlayerCharacter::InputOpenSpellInfo()
{
	if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport())
		UIManagerWorldSubsystem->SpellBattleMenuWidget->ShowResultSpellButtonOnClicked();
}

void APlayerCharacter::InputOpenPlayerMenu()
{
	if(!IsValid(UIManagerWorldSubsystem->PlayerMenuWidget))
		if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
			if (IsValid(RedemptionGameModeBase->GetPlayerMenuClass()))
				UIManagerWorldSubsystem->PlayerMenuWidget = CreateWidget<UPlayerMenu>(PlayerController, RedemptionGameModeBase->GetPlayerMenuClass());
	if (IsValid(UIManagerWorldSubsystem->PlayerMenuWidget)) {
		if (!UIManagerWorldSubsystem->PlayerMenuWidget->IsInViewport() && CanOpenOtherMenus) {
			PlayerController->SetPause(true);
			UIManagerWorldSubsystem->PlayerMenuWidget->AddToViewport();
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UIManagerWorldSubsystem->PlayerMenuWidget, EMouseLockMode::DoNotLock);
			//PlayerController->ActivateTouchInterface(EmptyTouchInterface);
			UIManagerWorldSubsystem->PlayerMenuWidget->GetInventoryButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PlayerMenuWidget->GetInventoryButton();
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			CanOpenOtherMenus = false;
		}
		else if (UIManagerWorldSubsystem->PlayerMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->PlayerMenuWidget->RemoveFromParent();
			UIManagerWorldSubsystem->PlayerMenuWidget->ConditionalBeginDestroy();
			UIManagerWorldSubsystem->PlayerMenuWidget = nullptr;
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
			PlayerController->SetPause(false);
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
			//PlayerController->ActivateTouchInterface(EmptyTouchInterface);
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			UIManagerWorldSubsystem->PickedButton = nullptr;
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			CanOpenOtherMenus = true;
		}
	}
}

void APlayerCharacter::InputOpenPauseMenu()
{
	FString MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if(!IsValid(UIManagerWorldSubsystem->PauseMenuWidget))
		if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
			if (IsValid(RedemptionGameModeBase->GetPauseMenuClass()))
				UIManagerWorldSubsystem->PauseMenuWidget = CreateWidget<UPauseMenu>(PlayerController, RedemptionGameModeBase->GetPauseMenuClass());
	if (IsValid(UIManagerWorldSubsystem->PauseMenuWidget)) {
		if (!UIManagerWorldSubsystem->PauseMenuWidget->IsInViewport() && CanOpenOtherMenus) {
			PlayerController->SetPause(true);
			UIManagerWorldSubsystem->PauseMenuWidget->AddToViewport();
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UIManagerWorldSubsystem->PauseMenuWidget, EMouseLockMode::DoNotLock);
			//PlayerController->ActivateTouchInterface(EmptyTouchInterface);
			UIManagerWorldSubsystem->PauseMenuWidget->GetResumeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PauseMenuWidget->GetResumeButton();
			CanOpenOtherMenus = false;
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
		}
		else if (UIManagerWorldSubsystem->PauseMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->PauseMenuWidget->RemoveFromParent();
			UIManagerWorldSubsystem->PauseMenuWidget->ConditionalBeginDestroy();
			UIManagerWorldSubsystem->PauseMenuWidget = nullptr;
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
			PlayerController->SetPause(false);
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
			//PlayerController->ActivateTouchInterface(EmptyTouchInterface);
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			UIManagerWorldSubsystem->PickedButton = nullptr;
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			CanOpenOtherMenus = true;
		}
	}
	if (IsValid(UIManagerWorldSubsystem->SettingsMenuWidget)) {
		if (UIManagerWorldSubsystem->SettingsMenuWidget->IsInViewport() && MapName != "MainMenu") {
			UIManagerWorldSubsystem->SettingsMenuWidget->RemoveFromParent();
			UIManagerWorldSubsystem->SettingsMenuWidget->ConditionalBeginDestroy();
			UIManagerWorldSubsystem->SettingsMenuWidget = nullptr;
			if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
				if (IsValid(RedemptionGameModeBase->GetPauseMenuClass()))
					UIManagerWorldSubsystem->PauseMenuWidget = CreateWidget<UPauseMenu>(PlayerController, RedemptionGameModeBase->GetPauseMenuClass());
			if (IsValid(UIManagerWorldSubsystem->PauseMenuWidget)) {
				UIManagerWorldSubsystem->PauseMenuWidget->AddToViewport();
				UIManagerWorldSubsystem->PauseMenuWidget->GetResumeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PauseMenuWidget->GetResumeButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
		}
		else if (UIManagerWorldSubsystem->SettingsMenuWidget->IsInViewport() && MapName == "MainMenu") {
			UIManagerWorldSubsystem->SettingsMenuWidget->RemoveFromParent();
			UIManagerWorldSubsystem->SettingsMenuWidget->ConditionalBeginDestroy();
			UIManagerWorldSubsystem->SettingsMenuWidget = nullptr;
			if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
				if (IsValid(RedemptionGameModeBase->GetMainMenuClass()))
					UIManagerWorldSubsystem->MainMenuWidget = CreateWidget<UMainMenu>(PlayerController, RedemptionGameModeBase->GetMainMenuClass());
			if (IsValid(UIManagerWorldSubsystem->MainMenuWidget)) {
				UIManagerWorldSubsystem->MainMenuWidget->AddToViewport();
				UIManagerWorldSubsystem->MainMenuWidget->GetNewGameButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->MainMenuWidget->GetNewGameButton();
			}
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
		}
	}
}

void APlayerCharacter::InputScrollLeft()
{
	if (IsValid(Controller) && IsValid(UIManagerWorldSubsystem->BattleMenuWidget) && UIManagerWorldSubsystem->BattleMenuWidget->IsInViewport())
		if (UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToAttack || UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToTalk 
			|| UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToViewInfo) {
				if (auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
					TArray<ACombatNPC*> TargetsForSelection{};
					//Add BattleEnemies in a loop, cause we need to convert them to the ACombatNPC.
					if (UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToViewInfo) {
						for (ACombatNPC* CombatNPC : RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer)
								TargetsForSelection.Add(CombatNPC);
						for (ACombatNPC* CombatNPC : RedemptionGameModeBase->GetBattleManager()->BattleEnemies)
								TargetsForSelection.Add(CombatNPC);
					}
					else if (RedemptionGameModeBase->GetBattleManager()->IsSelectingAllyAsTarget) {
						for (ACombatNPC* CombatNPC : RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer)
								TargetsForSelection.Add(CombatNPC);
					}
					else if (!RedemptionGameModeBase->GetBattleManager()->IsSelectingAllyAsTarget)
						for (ACombatNPC* CombatNPC : RedemptionGameModeBase->GetBattleManager()->BattleEnemies)
								TargetsForSelection.Add(CombatNPC);
					//Choose Target with scroll
					if (TargetsForSelection.Num() > 1) {
						int8 CurrentIndex = RedemptionGameModeBase->GetBattleManager()->SelectedCombatNPCIndex;
						int8 StartIndex = RedemptionGameModeBase->GetBattleManager()->SelectedCombatNPCIndex;
						while (true) {
							if (CurrentIndex + 1 < TargetsForSelection.Num()) {
								if (TargetsForSelection[CurrentIndex + 1]->GetCurrentHP() > 0) {
									RedemptionGameModeBase->GetBattleManager()->SelectNewTargetCrosshairLogic(TargetsForSelection, CurrentIndex + 1, CurrentIndex, "Left");
									RedemptionGameModeBase->GetBattleManager()->SelectNewTarget(TargetsForSelection[CurrentIndex + 1], CurrentIndex + 1);
									break;
								}
								else {
									RedemptionGameModeBase->GetBattleManager()->HideTargetCrosshairAndWidgets(TargetsForSelection, CurrentIndex, "Health", false);
									CurrentIndex++;
									if (CurrentIndex == StartIndex) 
										break;
								}
							}
							else {
								if (TargetsForSelection[0]->GetCurrentHP() > 0) {
									RedemptionGameModeBase->GetBattleManager()->SelectNewTargetCrosshairLogic(TargetsForSelection, 0, CurrentIndex, "Left");
									RedemptionGameModeBase->GetBattleManager()->SelectNewTarget(TargetsForSelection[0], 0);
									break;
								}
								else {
									RedemptionGameModeBase->GetBattleManager()->HideTargetCrosshairAndWidgets(TargetsForSelection, CurrentIndex, "Health", false);
									CurrentIndex = 0;
									if (CurrentIndex == StartIndex)
										break;
								}
							}
						}
					}
				}
		}
	if ((IsValid(UIManagerWorldSubsystem->PartyMenuWidget) && UIManagerWorldSubsystem->PartyMenuWidget->IsInViewport()) 
		&& UIManagerWorldSubsystem->PartyMenuWidget->IsSelectingCharacter) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
			for (uint8 Index = 0; Index < UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetAllChildren().Num(); Index++)
				if (const auto* const PartyMenuGeneralCharacterInfo = Cast<UPartyMenuGeneralCharacterInfo>(
					UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetChildAt(Index)); IsValid(PartyMenuGeneralCharacterInfo)) {
					if (PartyMenuGeneralCharacterInfo->GetPerksLevelingUpButton() == UIManagerWorldSubsystem->PickedButton ||
						PartyMenuGeneralCharacterInfo->GetCharacterNameButton() == UIManagerWorldSubsystem->PickedButton) {
						UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex = Index;
						break;
					}
				}
			if (UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex == 0) {
				if (auto* GeneralCharacterInfoWidget = Cast<UPartyMenuGeneralCharacterInfo>(UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->
					GetChildAt(UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetAllChildren().Num() - 1));
					IsValid(GeneralCharacterInfoWidget)) {
					UIManagerWorldSubsystem->PickedButton = GeneralCharacterInfoWidget->GetCharacterNameButton();
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex = UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetAllChildren().Num() - 1;
				}
			}
			else {
				if (auto* GeneralCharacterInfoWidget = Cast<UPartyMenuGeneralCharacterInfo>(UIManagerWorldSubsystem->PartyMenuWidget->
					GetCharactersHorizontalBox()->GetChildAt(UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex - 1));
					IsValid(GeneralCharacterInfoWidget)) {
						UIManagerWorldSubsystem->PickedButton = GeneralCharacterInfoWidget->GetCharacterNameButton();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
						UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex = UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex - 1;
					}
			}
	}
	else if (IsValid(UIManagerWorldSubsystem->PerksLevelingUpMenuWidget) && UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->IsInViewport()
		&& UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetStatsLevelingUpScaleBox()->GetVisibility() != ESlateVisibility::Visible) {
		if (UIManagerWorldSubsystem->PickedButton != UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton() &&
			UIManagerWorldSubsystem->PickedButton != UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetUnlockButton()) {
			TArray<UWidget*> Buttons{};
			bool IsCategory{};
			for (UWidget* Widget : UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren())
				if (UIManagerWorldSubsystem->PickedButton == Widget) {
					Buttons = UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren();
					break;
				}
			if (Buttons.Num() == 0) {
				for (UWidget* Widget : UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren())
					if (UIManagerWorldSubsystem->PickedButton == Widget) {
						IsCategory = true;
						Buttons = UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren();
						break;
					}
			}
			if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
				if (IsCategory) {
					if (UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedCategoryIndex == UIManagerWorldSubsystem->PickedButtonIndex)
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					else
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				}
				else {
					if (auto* const PickedButtonWithActorClass = Cast<UButtonWithActorClass>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithActorClass)) {
						if (PickedButtonWithActorClass->ActorClass == UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass)
							PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
						else {
							bool IsActivated = false;
							PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
							if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner)) {
								for (TSubclassOf<APerk> ActivatedPerkClass : UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->ActivatedPerks) {
									if (ActivatedPerkClass == PickedButtonWithActorClass->ActorClass) {
										PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
										IsActivated = true;
										break;
									}
								}
							}
							else if (const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
								for (TSubclassOf<APerk> ActivatedPerkClass : PlayerCharacter->ActivatedPerks) {
									if (ActivatedPerkClass == PickedButtonWithActorClass->ActorClass) {
										PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
										IsActivated = true;
										break;
									}
								}
							}
							if (!IsActivated)
								PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
						}
					}
				}
			}
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(Buttons[UIManagerWorldSubsystem->PickedButtonIndex]);
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else {
			if (UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetUnlockButton()->GetVisibility() == ESlateVisibility::Visible) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				if (UIManagerWorldSubsystem->PickedButton == UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetUnlockButton())
					UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton();
				else if (UIManagerWorldSubsystem->PickedButton == UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton())
					UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetUnlockButton();
				else
					UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu) && UIManagerWorldSubsystem->ContainerInventoryMenu->IsInViewport()) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget) &&
				UIManagerWorldSubsystem->PickedButton == UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget->GetMainButton())
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
			else if (IsValid(Cast<UInventoryScrollBoxEntryWidget>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter())))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
			else
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		if (UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox == UIManagerWorldSubsystem->ContainerInventoryMenu->GetItemsVerticalBox()) {
			UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox = const_cast<UVerticalBox*>(UIManagerWorldSubsystem->ContainerInventoryMenu->GetButtonsVerticalBox());
			UIManagerWorldSubsystem->ContainerInventoryMenu->SetPickedButtonToTakeButton();
		}
		else if (UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox == UIManagerWorldSubsystem->ContainerInventoryMenu->GetButtonsVerticalBox()) {
			UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox = const_cast<UVerticalBox*>(UIManagerWorldSubsystem->ContainerInventoryMenu->GetItemsVerticalBox());
			UIManagerWorldSubsystem->ContainerInventoryMenu->SetPickedButtonToFirstItemEntry();
		}
	}
	if ((IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport()) || 
		(IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget) && UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport())
		|| (IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget) && UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport())
		|| (IsValid(UIManagerWorldSubsystem->CharacterCreationMenu) && UIManagerWorldSubsystem->CharacterCreationMenu->IsInViewport())) {
			if (UButtonWithNeighbors* PickedButtonWithNeighbors = Cast<UButtonWithNeighbors>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithNeighbors))
				for (FSideAndItsButton SideAndItsButton : PickedButtonWithNeighbors->SidesAndTheirButtons)
					if (SideAndItsButton.Side == ESides::LEFT) {
						if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
							if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport())
								UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
							else if ((IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget) && UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport())
								|| (IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget) && UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport()))
									UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
							else if (IsValid(UIManagerWorldSubsystem->CharacterCreationMenu) && UIManagerWorldSubsystem->CharacterCreationMenu->IsInViewport())
								UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
						}
						UIManagerWorldSubsystem->PickedButton = SideAndItsButton.NeighborButton;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
					}
	}
}

void APlayerCharacter::InputScrollRight()
{
	if (IsValid(Controller) && IsValid(UIManagerWorldSubsystem->BattleMenuWidget) && UIManagerWorldSubsystem->BattleMenuWidget->IsInViewport())
		if (UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToAttack || UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToTalk || UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToViewInfo) {
			if (auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
				TArray<ACombatNPC*> TargetsForSelection{};
				//Add BattleEnemies in a loop, cause we need to convert them to the ACombatNPC.
				if (UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToViewInfo) {
					for (ACombatNPC* CombatNPC : RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer)
							TargetsForSelection.Add(CombatNPC);
					for (ACombatNPC* CombatNPC : RedemptionGameModeBase->GetBattleManager()->BattleEnemies)
							TargetsForSelection.Add(CombatNPC);
				}
				else if (RedemptionGameModeBase->GetBattleManager()->IsSelectingAllyAsTarget) {
					for (ACombatNPC* CombatNPC : RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer)
							TargetsForSelection.Add(CombatNPC);
				}
				else if (!RedemptionGameModeBase->GetBattleManager()->IsSelectingAllyAsTarget)
					for (ACombatNPC* CombatNPC : RedemptionGameModeBase->GetBattleManager()->BattleEnemies)
							TargetsForSelection.Add(CombatNPC);
				if (TargetsForSelection.Num() > 1) {
					//Choose target with scroll
					int8 CurrentIndex = RedemptionGameModeBase->GetBattleManager()->SelectedCombatNPCIndex;
					int8 StartIndex = RedemptionGameModeBase->GetBattleManager()->SelectedCombatNPCIndex;
					//Need this for a sv suffix.
					while (true) {
						if (CurrentIndex - 1 >= 0) {
							if (TargetsForSelection[CurrentIndex - 1]->GetCurrentHP() > 0) {
								RedemptionGameModeBase->GetBattleManager()->SelectNewTargetCrosshairLogic(TargetsForSelection, CurrentIndex - 1, CurrentIndex, "Right");
								RedemptionGameModeBase->GetBattleManager()->SelectNewTarget(TargetsForSelection[CurrentIndex - 1], CurrentIndex - 1);
								break;
							}
							else {
								RedemptionGameModeBase->GetBattleManager()->HideTargetCrosshairAndWidgets(TargetsForSelection, CurrentIndex, "Health", false);
								CurrentIndex--;
								if (CurrentIndex == StartIndex)
									break;
							}
						}
						else {
							if (TargetsForSelection[TargetsForSelection.Num() - 1]->GetCurrentHP() > 0) {
								RedemptionGameModeBase->GetBattleManager()->SelectNewTargetCrosshairLogic(TargetsForSelection, TargetsForSelection.Num() - 1, CurrentIndex, "Right");
								RedemptionGameModeBase->GetBattleManager()->SelectNewTarget(TargetsForSelection[TargetsForSelection.Num() - 1], TargetsForSelection.Num() - 1);
								break;
							}
							else {
								RedemptionGameModeBase->GetBattleManager()->HideTargetCrosshairAndWidgets(TargetsForSelection, CurrentIndex, "Health", false);
								CurrentIndex = TargetsForSelection.Num() - 1;
								if (CurrentIndex == StartIndex)
									break;
							}
						}
					}
				}
			}
		}
	if ((IsValid(UIManagerWorldSubsystem->PartyMenuWidget) && UIManagerWorldSubsystem->PartyMenuWidget->IsInViewport()) 
		&& UIManagerWorldSubsystem->PartyMenuWidget->IsSelectingCharacter) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
			for(uint8 Index = 0; Index < UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetAllChildren().Num(); Index++)
				if (const auto* const PartyMenuGeneralCharacterInfo = Cast<UPartyMenuGeneralCharacterInfo>(
					UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetChildAt(Index)); IsValid(PartyMenuGeneralCharacterInfo)) {
					if (PartyMenuGeneralCharacterInfo->GetPerksLevelingUpButton() == UIManagerWorldSubsystem->PickedButton ||
						PartyMenuGeneralCharacterInfo->GetCharacterNameButton() == UIManagerWorldSubsystem->PickedButton) {
						UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex = Index;
						break;
					}
								
				}
			if (UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex == UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetAllChildren().Num() - 1) {
				if (auto* GeneralCharacterInfoWidget = Cast<UPartyMenuGeneralCharacterInfo>(UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetChildAt(0));
					IsValid(GeneralCharacterInfoWidget)) {
					UIManagerWorldSubsystem->PickedButton = GeneralCharacterInfoWidget->GetCharacterNameButton();
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex = 0;
				}
			}
			else {
				if (auto* GeneralCharacterInfoWidget = Cast<UPartyMenuGeneralCharacterInfo>(UIManagerWorldSubsystem->PartyMenuWidget->
					GetCharactersHorizontalBox()->GetChildAt(UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex + 1));
					IsValid(GeneralCharacterInfoWidget)) {
						UIManagerWorldSubsystem->PickedButton = GeneralCharacterInfoWidget->GetCharacterNameButton();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
						UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex = UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex + 1;
				}
			}
	}
	else if (IsValid(UIManagerWorldSubsystem->PerksLevelingUpMenuWidget) && UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->IsInViewport()
		&& UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetStatsLevelingUpScaleBox()->GetVisibility() != ESlateVisibility::Visible) {
		if (UIManagerWorldSubsystem->PickedButton != UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton() &&
			UIManagerWorldSubsystem->PickedButton != UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetUnlockButton()) {
			TArray<UWidget*> Buttons{};
			bool IsCategory{};
			for (UWidget* Widget : UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren())
				if (UIManagerWorldSubsystem->PickedButton == Widget) {
					Buttons = UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren();
					break;
				}
			if (Buttons.Num() == 0) {
				for (UWidget* Widget : UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren())
					if (UIManagerWorldSubsystem->PickedButton == Widget) {
						IsCategory = true;
						Buttons = UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren();
						break;
					}
			}
			if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
				if (IsCategory) {
					if(UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedCategoryIndex == UIManagerWorldSubsystem->PickedButtonIndex)
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					else
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				}
				else {
					if (auto* const PickedButtonWithActorClass = Cast<UButtonWithActorClass>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithActorClass)) {
						if (PickedButtonWithActorClass->ActorClass == UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass)
							PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
						else {
							bool IsActivated = false;
							PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
							if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner)) {
								for (TSubclassOf<APerk> ActivatedPerkClass : UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->ActivatedPerks) {
									if (ActivatedPerkClass == PickedButtonWithActorClass->ActorClass) {
										PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
										IsActivated = true;
										break;
									}
								}
							}
							else if (const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
								for (TSubclassOf<APerk> ActivatedPerkClass : PlayerCharacter->ActivatedPerks) {
									if (ActivatedPerkClass == PickedButtonWithActorClass->ActorClass) {
										PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
										IsActivated = true;
										break;
									}
								}
							}
							if (!IsActivated)
								PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
						}
					}
				}
			}
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(Buttons[UIManagerWorldSubsystem->PickedButtonIndex]);
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else {
			if (UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetUnlockButton()->GetVisibility() == ESlateVisibility::Visible) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				if (UIManagerWorldSubsystem->PickedButton == UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetUnlockButton())
					UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton();
				else if (UIManagerWorldSubsystem->PickedButton == UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton())
					UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetUnlockButton();
				else
					UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu) && UIManagerWorldSubsystem->ContainerInventoryMenu->IsInViewport()){
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget) &&
				UIManagerWorldSubsystem->PickedButton == UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget->GetMainButton())
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
			else if (IsValid(Cast<UInventoryScrollBoxEntryWidget>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter())))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
			else
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		if (UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox == UIManagerWorldSubsystem->ContainerInventoryMenu->GetItemsVerticalBox()) {
			UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox = const_cast<UVerticalBox*>(UIManagerWorldSubsystem->ContainerInventoryMenu->GetButtonsVerticalBox());
			UIManagerWorldSubsystem->ContainerInventoryMenu->SetPickedButtonToTakeButton();
		}
		else if (UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox == UIManagerWorldSubsystem->ContainerInventoryMenu->GetButtonsVerticalBox()) {
			UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox = const_cast<UVerticalBox*>(UIManagerWorldSubsystem->ContainerInventoryMenu->GetItemsVerticalBox());
			UIManagerWorldSubsystem->ContainerInventoryMenu->SetPickedButtonToFirstItemEntry();
		}
	}
	if ((IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport()) || 
		(IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget) && UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport())
		|| (IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget) && UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport())
		|| (IsValid(UIManagerWorldSubsystem->CharacterCreationMenu) && UIManagerWorldSubsystem->CharacterCreationMenu->IsInViewport())) {
		if (UButtonWithNeighbors* PickedButtonWithNeighbors = Cast<UButtonWithNeighbors>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithNeighbors))
			for (FSideAndItsButton SideAndItsButton : PickedButtonWithNeighbors->SidesAndTheirButtons)
				if (SideAndItsButton.Side == ESides::RIGHT) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
						if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport())
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
						else if ((IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget) && UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport())
							|| (IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget) && UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport()))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
						else if (IsValid(UIManagerWorldSubsystem->CharacterCreationMenu) && UIManagerWorldSubsystem->CharacterCreationMenu->IsInViewport())
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
					}
					UIManagerWorldSubsystem->PickedButton = SideAndItsButton.NeighborButton;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
				}
	}
}

void APlayerCharacter::InputScrollUp()
{
	if (UButtonWithNeighbors* PickedButtonWithNeighbors = Cast<UButtonWithNeighbors>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithNeighbors)) {
		for (FSideAndItsButton SideAndItsButton : PickedButtonWithNeighbors->SidesAndTheirButtons)
			if (SideAndItsButton.Side == ESides::UP) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
					if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.8f));
					else if (IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget) && UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
					else if (IsValid(UIManagerWorldSubsystem->SaveLoadGameMenuWidget) && UIManagerWorldSubsystem->SaveLoadGameMenuWidget->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1));
					else if (IsValid(UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget) && UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
					else if (IsValid(UIManagerWorldSubsystem->CharacterCreationMenu) && UIManagerWorldSubsystem->CharacterCreationMenu->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				}
				UIManagerWorldSubsystem->PickedButton = SideAndItsButton.NeighborButton;
				if(IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport())
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				else
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
	}
	else if (IsValid(UIManagerWorldSubsystem->BattleMenuWidget) && UIManagerWorldSubsystem->BattleMenuWidget->IsInViewport()) {
		if (UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingAction) {
			TArray<UWidget*> MenuVerticalBoxChildrens{};
			for(UWidget* Widget : UIManagerWorldSubsystem->BattleMenuWidget->GetMenuVerticalBox()->GetAllChildren())
				if(Widget->GetVisibility() != ESlateVisibility::Collapsed && Widget->GetVisibility()!= ESlateVisibility::Hidden)
					MenuVerticalBoxChildrens.Add(Widget);
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[MenuVerticalBoxChildrens.Num() - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = MenuVerticalBoxChildrens.Num() - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
			else {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
		}
		else if (UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingItem && !UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToAttack) {
			TArray<UWidget*> InventoryScrollBoxChildrens = UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren();
			if (InventoryScrollBoxChildrens.Num() > 1) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
				if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
					UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[InventoryScrollBoxChildrens.Num() - 1]);
					UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->ScrollToEnd();
					UIManagerWorldSubsystem->PickedButtonIndex = InventoryScrollBoxChildrens.Num() - 1;
					UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
					UIManagerWorldSubsystem->InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
				else {
					UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
					UIManagerWorldSubsystem->PickedButtonIndex--;
					UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
					UIManagerWorldSubsystem->InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
			}
		}
		else if (UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToAttack || UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToTalk || 
			UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToViewInfo) {
			TArray<UWidget*> AttackMenuVerticalBoxChildrens = UIManagerWorldSubsystem->BattleMenuWidget->GetAttackMenuVerticalBox()->GetAllChildren();
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				UIManagerWorldSubsystem->PickedButtonIndex = AttackMenuVerticalBoxChildrens.Num() - 1;
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex]);
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
			else {
				UIManagerWorldSubsystem->PickedButtonIndex -= 1;
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex]);
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->PlayerMenuWidget) && UIManagerWorldSubsystem->PlayerMenuWidget->IsInViewport()) {
		TArray<UWidget*> PlayerMenuButtons = UIManagerWorldSubsystem->PlayerMenuWidget->GetButtonsStackBox()->GetAllChildren();
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[PlayerMenuButtons.Num() - 1]);
			UIManagerWorldSubsystem->PickedButtonIndex = PlayerMenuButtons.Num() - 1;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else {
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
			UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->PauseMenuWidget) && UIManagerWorldSubsystem->PauseMenuWidget->IsInViewport()) {
		TArray<UWidget*> PauseMenuButtons = UIManagerWorldSubsystem->PauseMenuWidget->GetButtonsStackBox()->GetAllChildren();
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[PauseMenuButtons.Num() - 1]);
			UIManagerWorldSubsystem->PickedButtonIndex = PauseMenuButtons.Num() - 1;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else {
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
			UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->MainMenuWidget) && UIManagerWorldSubsystem->MainMenuWidget->IsInViewport()) {
		TArray<UWidget*> MainMenuButtons = UIManagerWorldSubsystem->MainMenuWidget->GetMainVerticalBox()->GetAllChildren();
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::FromHex("8C8C8CFF"));
		if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MainMenuButtons[MainMenuButtons.Num() - 1]);
			UIManagerWorldSubsystem->PickedButtonIndex = MainMenuButtons.Num() - 1;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else {
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MainMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
			UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport() && 
		UIManagerWorldSubsystem->SpellBattleMenuWidget->CanUseKeyboardButtonSelection && !UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport()) {
			TArray<UWidget*> SpellBattleMenuElementsButtons{};
			if (UIManagerWorldSubsystem->SpellBattleMenuWidget->GetSelectedSpellRange() == ESpellRange::NONE) {
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetNeighborsSpellRangeButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetEveryoneSpellRangeButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetSingleSpellRangeButton());
			}
			else if (UIManagerWorldSubsystem->SpellBattleMenuWidget->GetSelectedSpellElements().Num() >= 0) {
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetWaterElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetEarthElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetDarkElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetLightningElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetHolyElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetWindElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetFireElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetBloodElementButton());
			}
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.8f));
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SpellBattleMenuElementsButtons[SpellBattleMenuElementsButtons.Num() - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = SpellBattleMenuElementsButtons.Num() - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SpellBattleMenuElementsButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
	}
	else if (IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget) && UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport() 
		&& UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->CanUseKeyboardButtonSelection) {
			TArray<UWidget*> LearnedSpellsJournalMenuButtons = UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren();
			if (LearnedSpellsJournalMenuButtons.Num() > 1) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
				if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
					UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(LearnedSpellsJournalMenuButtons[LearnedSpellsJournalMenuButtons.Num() - 1])->GetMainButton();
					UIManagerWorldSubsystem->PickedButtonIndex = LearnedSpellsJournalMenuButtons.Num() - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				}
				else {
					UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(LearnedSpellsJournalMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1])->GetMainButton();
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				}
			}
	}
	else if (IsValid(UIManagerWorldSubsystem->SaveLoadGameMenuWidget) && UIManagerWorldSubsystem->SaveLoadGameMenuWidget->IsInViewport()) {
		TArray<UWidget*> SaveLoadGameMenuWidgetSlotButtons = UIManagerWorldSubsystem->SaveLoadGameMenuWidget->GetSaveSlotsScrollBox()->GetAllChildren();
		if (SaveLoadGameMenuWidgetSlotButtons.Num() > 1) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				UIManagerWorldSubsystem->PickedButton = Cast <USaveSlotEntry> (SaveLoadGameMenuWidgetSlotButtons[SaveLoadGameMenuWidgetSlotButtons.Num() - 1])->GetSlotButton();
				UIManagerWorldSubsystem->PickedButtonIndex = SaveLoadGameMenuWidgetSlotButtons.Num() - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = Cast<USaveSlotEntry>(SaveLoadGameMenuWidgetSlotButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1])->GetSlotButton();
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget) && UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport() && 
		UIManagerWorldSubsystem->SkillBattleMenuWidget->CanUseKeyboardButtonSelection) {
		TArray<UWidget*> SkillBattleMenuButtons{};
		for (UWidget* Widget : UIManagerWorldSubsystem->SkillBattleMenuWidget->GetSkillsScrollBox()->GetAllChildren())
			if (Widget->GetVisibility() == ESlateVisibility::Visible)
				SkillBattleMenuButtons.Add(Widget);
		if (SkillBattleMenuButtons.Num() > 1) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 0.8f));
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(SkillBattleMenuButtons[SkillBattleMenuButtons.Num() - 1])->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = SkillBattleMenuButtons.Num() - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(SkillBattleMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1])->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->PartyMenuWidget) && UIManagerWorldSubsystem->PartyMenuWidget->IsInViewport() ) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
		if (auto* GeneralCharacterInfoWidget = Cast<UPartyMenuGeneralCharacterInfo>
			(UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->
			GetChildAt(UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex)); IsValid(GeneralCharacterInfoWidget)) {
				if (UIManagerWorldSubsystem->PickedButtonIndex == 1) {
					UIManagerWorldSubsystem->PickedButton = GeneralCharacterInfoWidget->GetCharacterNameButton();
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
				}
				else if (UIManagerWorldSubsystem->PickedButtonIndex == 2) {
					UIManagerWorldSubsystem->PickedButton = GeneralCharacterInfoWidget->GetPerksLevelingUpButton();
					UIManagerWorldSubsystem->PickedButtonIndex = 1;
					UIManagerWorldSubsystem->PartyMenuWidget->IsSelectingCharacter = true;
				}
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget) && UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->IsInViewport()) {
		TArray<UWidget*> Buttons = UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->GetButtonsVerticalBox()->GetAllChildren();
		if (Buttons.Num() > 1) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.7f, 0.7f, 0.7f, 1.f));
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(Buttons[Buttons.Num() - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = Buttons.Num() - 1;
				if(IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(Buttons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
				if(IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->PerksLevelingUpMenuWidget) && UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->IsInViewport()
		&& UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetStatsLevelingUpScaleBox()->GetVisibility() != ESlateVisibility::Visible) {
		if (UIManagerWorldSubsystem->PickedButton != UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton() &&
			UIManagerWorldSubsystem->PickedButton != UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetUnlockButton()) {
			TArray<UWidget*> Buttons{};
			bool IsCategory = false;
			for (UWidget* Widget : UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren())
				if (UIManagerWorldSubsystem->PickedButton == Widget) {
					Buttons = UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren();
					break;
				}
			if (Buttons.Num() == 0) {
				for (UWidget* Widget : UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren())
					if (UIManagerWorldSubsystem->PickedButton == Widget) {
						IsCategory = true;
						Buttons = UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren();
						break;
					}
			}
			if (Buttons.Num() > 1) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
					if (IsCategory) {
						for (uint8 Index = 0; Index < UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren().Num(); Index++)
							if (UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetChildAt(Index) == UIManagerWorldSubsystem->PickedButton) {
								if (Index == UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedCategoryIndex)
									UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
								else
									UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
								break;
							}
					}
					else {
						if (auto* const PickedButtonWithActorClass = Cast<UButtonWithActorClass>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithActorClass)) {
							if (PickedButtonWithActorClass->ActorClass == UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass)
								PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
							else {
								bool IsActivated = false;
								PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
								if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner)) {
									for (TSubclassOf<APerk> ActivatedPerkClass : UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->ActivatedPerks) {
										if (ActivatedPerkClass == PickedButtonWithActorClass->ActorClass) {
											PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
											IsActivated = true;
											break;
										}
									}
								}
								else if (const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
									for (TSubclassOf<APerk> ActivatedPerkClass : PlayerCharacter->ActivatedPerks) {
										if (ActivatedPerkClass == PickedButtonWithActorClass->ActorClass) {
											PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
											IsActivated = true;
											break;
										}
									}
								}
								if (!IsActivated)
									PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
							}
						}
					}
				}
				if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
					UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton();
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					if (IsCategory)
						UIManagerWorldSubsystem->CharacterPerksMenuWidget->ScrolledDownOrUpFromCategories = true;
					else
						UIManagerWorldSubsystem->CharacterPerksMenuWidget->ScrolledDownOrUpFromCategories = false;
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
				else {
					UIManagerWorldSubsystem->PickedButton = Cast<UButton>(Buttons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
			}
		}
		else {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			if (UIManagerWorldSubsystem->CharacterPerksMenuWidget->ScrolledDownOrUpFromCategories) {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->
					GetChildAt(UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren().Num() - 1));
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren().Num() - 1;
			}
			else {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->
					GetChildAt(UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren().Num() - 1));
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren().Num() - 1;
			}
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget) && IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->SelectedPanelWidget) && 
		UIManagerWorldSubsystem->InventoryMenuWidget->IsInViewport()) {
			TArray<UWidget*> SelectedScrollBoxChildren = UIManagerWorldSubsystem->InventoryMenuWidget->SelectedPanelWidget->GetAllChildren();
			if (SelectedScrollBoxChildren.Num() > 0) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
					if (UIManagerWorldSubsystem->InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible 
						&& !UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingSpecificItem)
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
					else if (UIManagerWorldSubsystem->InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible 
						&& UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingSpecificItem)
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
					else {
						if (UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingTarget) {
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
						}
						else if (UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingSpecificItem) {
							if (UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->GetVisibility() == ESlateVisibility::Visible 
								&& UIManagerWorldSubsystem->InventoryMenuWidget->SelectedItemButtonIndex >= 0
								&& UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren().Num() > UIManagerWorldSubsystem->InventoryMenuWidget->SelectedItemButtonIndex) {
									if (UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->GetChildAt(UIManagerWorldSubsystem->InventoryMenuWidget->SelectedItemButtonIndex) ==
										Cast<UWidget>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter()))
										UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
									else
										UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
							}
							else
								UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
						}
						else
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
					}
				}
				if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
					if (UInventoryScrollBoxEntryWidget* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[SelectedScrollBoxChildren.Num() - 1]); IsValid(SelectedEntryWidget)) {
						UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
						UIManagerWorldSubsystem->InventoryMenuWidget->SetPickedItem(SelectedEntryWidget->GetItem());
						UIManagerWorldSubsystem->InventoryMenuWidget->SetItemInfo(SelectedEntryWidget->GetItem());
					}
					else if (auto* InventoryTargetEntry = Cast<UInventoryTargetEntry>(SelectedScrollBoxChildren[SelectedScrollBoxChildren.Num() - 1]); IsValid(InventoryTargetEntry))
						UIManagerWorldSubsystem->PickedButton = InventoryTargetEntry->GetNameButtonWithHoverColors();
					else
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[SelectedScrollBoxChildren.Num() - 1]);
					UIManagerWorldSubsystem->PickedButtonIndex = SelectedScrollBoxChildren.Num() - 1;
					UIManagerWorldSubsystem->InventoryMenuWidget->CheckForButtonsBordersVisibilityAndSetGreenColorToButton();
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
				else {
					if (UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingSpecificItem) {
						if (auto* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex - 1]); IsValid(SelectedEntryWidget)) {
							UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
							UIManagerWorldSubsystem->InventoryMenuWidget->SetPickedItem(SelectedEntryWidget->GetItem());
							UIManagerWorldSubsystem->InventoryMenuWidget->SetItemInfo(SelectedEntryWidget->GetItem());
						}
					}
					else if (UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingTarget) {
						if (auto* InventoryTargetEntry = Cast<UInventoryTargetEntry>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex - 1]); IsValid(InventoryTargetEntry))
							UIManagerWorldSubsystem->PickedButton = InventoryTargetEntry->GetNameButtonWithHoverColors();
					}
					else {
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
						UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetMeleeButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetRangeButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetHeadButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetTorseButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetHandButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetLowerArmorButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->CheckForButtonsBordersVisibilityAndSetGreenColorToButton();
					}
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
			}
		 }
	else if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu) && IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox) &&
		UIManagerWorldSubsystem->ContainerInventoryMenu->IsInViewport()) {
			TArray<UWidget*> SelectedVerticalBoxChildren = UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox->GetAllChildren();
			if (SelectedVerticalBoxChildren.Num() > 0) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
					if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget) &&
						UIManagerWorldSubsystem->PickedButton == UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget->GetMainButton())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					else if(IsValid(Cast<UInventoryScrollBoxEntryWidget>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter())))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
					else
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				}
				if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
					if (UInventoryScrollBoxEntryWidget* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedVerticalBoxChildren[SelectedVerticalBoxChildren.Num() - 1]); IsValid(SelectedEntryWidget))
						UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
					else
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedVerticalBoxChildren[SelectedVerticalBoxChildren.Num() - 1]);
					UIManagerWorldSubsystem->PickedButtonIndex = SelectedVerticalBoxChildren.Num() - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
				else {
					if (auto* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedVerticalBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex - 1]); IsValid(SelectedEntryWidget))
						UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
					else
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedVerticalBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
			}
		 }
}

void APlayerCharacter::InputScrollDown()
{
	if (UButtonWithNeighbors* PickedButtonWithNeighbors = Cast<UButtonWithNeighbors>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithNeighbors)) {
		for (FSideAndItsButton SideAndItsButton : PickedButtonWithNeighbors->SidesAndTheirButtons)
			if (SideAndItsButton.Side == ESides::DOWN) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
					if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.8f));
					else if (IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget) && UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
					else if (IsValid(UIManagerWorldSubsystem->SaveLoadGameMenuWidget) && UIManagerWorldSubsystem->SaveLoadGameMenuWidget->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
					else if (IsValid(UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget) && UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
					else if (IsValid(UIManagerWorldSubsystem->CharacterCreationMenu) && UIManagerWorldSubsystem->CharacterCreationMenu->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				}
				UIManagerWorldSubsystem->PickedButton = SideAndItsButton.NeighborButton;
				if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport())
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				else
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
	}
	else if (IsValid(UIManagerWorldSubsystem->BattleMenuWidget) && UIManagerWorldSubsystem->BattleMenuWidget->IsInViewport()) {
		if (UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingAction) {
			TArray<UWidget*> MenuVerticalBoxChildrens{};
			for (UWidget* Widget : UIManagerWorldSubsystem->BattleMenuWidget->GetMenuVerticalBox()->GetAllChildren())
				if (Widget->GetVisibility() != ESlateVisibility::Collapsed && Widget->GetVisibility() != ESlateVisibility::Hidden)
					MenuVerticalBoxChildrens.Add(Widget);
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			if (UIManagerWorldSubsystem->PickedButtonIndex == MenuVerticalBoxChildrens.Num() - 1) {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[0]);
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
		}
		else if (UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingItem && !UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToAttack &&
			UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren().Num() > 0) {
				TArray<UWidget*> InventoryScrollBoxChildrens = UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren();
				if (InventoryScrollBoxChildrens.Num() > 1) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
					if (UIManagerWorldSubsystem->PickedButtonIndex == InventoryScrollBoxChildrens.Num() - 1) {
						UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[0]);
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
						UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
						UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->ScrollToStart();
					}
					else {
						UIManagerWorldSubsystem->PickedButtonIndex++;
						UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex]);
						UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
						UIManagerWorldSubsystem->InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
					}
				}
		}
		else if (UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToAttack || UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToTalk 
			|| UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToViewInfo) {
				TArray<UWidget*> AttackMenuVerticalBoxChildrens = UIManagerWorldSubsystem->BattleMenuWidget->GetAttackMenuVerticalBox()->GetAllChildren();
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				if (UIManagerWorldSubsystem->PickedButtonIndex == AttackMenuVerticalBoxChildrens.Num() - 1) {
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex]);
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
				else {
					UIManagerWorldSubsystem->PickedButtonIndex += 1;
					UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex]);
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->PlayerMenuWidget) && UIManagerWorldSubsystem->PlayerMenuWidget->IsInViewport()) {
		TArray<UWidget*> PlayerMenuButtons = UIManagerWorldSubsystem->PlayerMenuWidget->GetButtonsStackBox()->GetAllChildren();
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		if (UIManagerWorldSubsystem->PickedButtonIndex == PlayerMenuButtons.Num() - 1) {
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[0]);
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else {
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
			UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->PauseMenuWidget) && UIManagerWorldSubsystem->PauseMenuWidget->IsInViewport()) {
		TArray<UWidget*> PauseMenuButtons = UIManagerWorldSubsystem->PauseMenuWidget->GetButtonsStackBox()->GetAllChildren();
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		if (UIManagerWorldSubsystem->PickedButtonIndex == PauseMenuButtons.Num() - 1) {
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[0]);
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else {
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
			UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->MainMenuWidget) && UIManagerWorldSubsystem->MainMenuWidget->IsInViewport()) {
		TArray<UWidget*> MainMenuButtons = UIManagerWorldSubsystem->MainMenuWidget->GetMainVerticalBox()->GetAllChildren();
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::FromHex("8C8C8CFF"));
			if (UIManagerWorldSubsystem->PickedButtonIndex == MainMenuButtons.Num() - 1) {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MainMenuButtons[0]);
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MainMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
	}
	else if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport() 
		&& UIManagerWorldSubsystem->SpellBattleMenuWidget->CanUseKeyboardButtonSelection) {
			TArray<UWidget*> SpellBattleMenuElementsButtons{};
			if (UIManagerWorldSubsystem->SpellBattleMenuWidget->GetSelectedSpellRange() == ESpellRange::NONE) {
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetNeighborsSpellRangeButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetEveryoneSpellRangeButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetSingleSpellRangeButton());
			}
			else if (UIManagerWorldSubsystem->SpellBattleMenuWidget->GetSelectedSpellElements().Num() >= 0) {
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetWaterElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetEarthElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetDarkElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetLightningElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetHolyElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetWindElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetFireElementButton());
				SpellBattleMenuElementsButtons.Add(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetBloodElementButton());
			}
			if (SpellBattleMenuElementsButtons.Num() > 0) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.8f));
				if (UIManagerWorldSubsystem->PickedButtonIndex == SpellBattleMenuElementsButtons.Num() - 1) {
					UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SpellBattleMenuElementsButtons[0]);
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				}
				else {
					UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SpellBattleMenuElementsButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				}
			}
	}
	else if (IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget) && UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport() 
		&& UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->CanUseKeyboardButtonSelection) {
		TArray<UWidget*> LearnedSpellsJournalMenuButtons = UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren();
		if (LearnedSpellsJournalMenuButtons.Num() > 1) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.8f));
			if (UIManagerWorldSubsystem->PickedButtonIndex == LearnedSpellsJournalMenuButtons.Num() - 1) {
				UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(LearnedSpellsJournalMenuButtons[0])->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(LearnedSpellsJournalMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1])->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->SaveLoadGameMenuWidget) && UIManagerWorldSubsystem->SaveLoadGameMenuWidget->IsInViewport()) {
		TArray<UWidget*> SaveLoadGameMenuWidgetSlotButtons = UIManagerWorldSubsystem->SaveLoadGameMenuWidget->GetSaveSlotsScrollBox()->GetAllChildren();
		if (SaveLoadGameMenuWidgetSlotButtons.Num() > 1) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			if (UIManagerWorldSubsystem->PickedButtonIndex == SaveLoadGameMenuWidgetSlotButtons.Num() - 1) {
				UIManagerWorldSubsystem->PickedButton = Cast <USaveSlotEntry>(SaveLoadGameMenuWidgetSlotButtons[0])->GetSlotButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0, 0, 0.8));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = Cast<USaveSlotEntry>(SaveLoadGameMenuWidgetSlotButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1])->GetSlotButton();
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
			}
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget) && UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport() 
		&& UIManagerWorldSubsystem->SkillBattleMenuWidget->CanUseKeyboardButtonSelection) {
		TArray<UWidget*> SkillBattleMenuButtons{};
		for (UWidget* Widget : UIManagerWorldSubsystem->SkillBattleMenuWidget->GetSkillsScrollBox()->GetAllChildren())
			if(Widget->GetVisibility() == ESlateVisibility::Visible)
				SkillBattleMenuButtons.Add(Widget);
		if (SkillBattleMenuButtons.Num() > 1) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 0.8f));
			if (UIManagerWorldSubsystem->PickedButtonIndex == SkillBattleMenuButtons.Num() - 1) {
				UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(SkillBattleMenuButtons[0])->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8f));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(SkillBattleMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1])->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->PartyMenuWidget) && UIManagerWorldSubsystem->PartyMenuWidget->IsInViewport()) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
		if (auto* GeneralCharacterInfoWidget = Cast<UPartyMenuGeneralCharacterInfo>
			(UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->
				GetChildAt(UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex)); IsValid(GeneralCharacterInfoWidget)) {
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				UIManagerWorldSubsystem->PickedButton = GeneralCharacterInfoWidget->GetPerksLevelingUpButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 1;
			}
			else if (UIManagerWorldSubsystem->PickedButtonIndex == 1) {
				UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PartyMenuWidget->GetBackButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 2;
				UIManagerWorldSubsystem->PartyMenuWidget->IsSelectingCharacter = false;
			}
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget) && UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->IsInViewport()) {
		TArray<UWidget*> Buttons = UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->GetButtonsVerticalBox()->GetAllChildren();
		if (Buttons.Num() > 1) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.7f, 0.7f, 0.7f, 1.f));
			if (UIManagerWorldSubsystem->PickedButtonIndex == Buttons.Num() - 1) {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(Buttons[0]);
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(Buttons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->PerksLevelingUpMenuWidget) && UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->IsInViewport()
		&& UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetStatsLevelingUpScaleBox()->GetVisibility() != ESlateVisibility::Visible) {
		if (UIManagerWorldSubsystem->PickedButton != UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton() &&
			UIManagerWorldSubsystem->PickedButton != UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetUnlockButton()) {
			TArray<UWidget*> Buttons{};
			bool IsCategory = false;
			for (UWidget* Widget : UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren())
				if (UIManagerWorldSubsystem->PickedButton == Widget) {
					Buttons = UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren();
					break;
				}
			if (Buttons.Num() == 0) {
				for (UWidget* Widget : UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren())
					if (UIManagerWorldSubsystem->PickedButton == Widget) {
						IsCategory = true;
						Buttons = UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren();
						break;
					}
			}
			if (Buttons.Num() > 1) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
					if (IsCategory) {
						for (uint8 Index = 0; Index < UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetAllChildren().Num(); Index++)
							if (UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetChildAt(Index) == UIManagerWorldSubsystem->PickedButton) {
								if (Index == UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedCategoryIndex)
									UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
								else
									UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
								break;
							}
					}
					else {
						if (auto* const PickedButtonWithActorClass = Cast<UButtonWithActorClass>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithActorClass)) {
							if (PickedButtonWithActorClass->ActorClass == UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass)
								PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
							else {
								bool IsActivated = false;
								PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
								if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner)) {
									for (TSubclassOf<APerk> ActivatedPerkClass : UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->ActivatedPerks) {
										if (ActivatedPerkClass == PickedButtonWithActorClass->ActorClass) {
											PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
											IsActivated = true;
											break;
										}
									}
								}
								else if(const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
									for (TSubclassOf<APerk> ActivatedPerkClass : PlayerCharacter->ActivatedPerks) {
										if (ActivatedPerkClass == PickedButtonWithActorClass->ActorClass) {
											PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
											IsActivated = true;
											break;
										}
									}
								}
								if(!IsActivated)
									PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
							}
						}
					}
				}
				if (UIManagerWorldSubsystem->PickedButtonIndex == Buttons.Num() - 1) {
					UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetBackButton();
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
					if (IsCategory)
						UIManagerWorldSubsystem->CharacterPerksMenuWidget->ScrolledDownOrUpFromCategories = true;
					else
						UIManagerWorldSubsystem->CharacterPerksMenuWidget->ScrolledDownOrUpFromCategories = false;
				}
				else {
					UIManagerWorldSubsystem->PickedButton = Cast<UButton>(Buttons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
			}
		}
		else {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			if (UIManagerWorldSubsystem->CharacterPerksMenuWidget->ScrolledDownOrUpFromCategories)
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetCategoryVerticalBox()->GetChildAt(0));
			else
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetChildAt(0));
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
	}
	else if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget) && IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->SelectedPanelWidget) 
		&& UIManagerWorldSubsystem->InventoryMenuWidget->IsInViewport()) {
			TArray<UWidget*> SelectedScrollBoxChildren = UIManagerWorldSubsystem->InventoryMenuWidget->SelectedPanelWidget->GetAllChildren();
			if (SelectedScrollBoxChildren.Num() > 0) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
					if (UIManagerWorldSubsystem->InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible 
						&& !UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingSpecificItem)
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
					else if (UIManagerWorldSubsystem->InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible 
						&& UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingSpecificItem)
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
					else {
						if (UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingTarget) {
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
						}
						else if (UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingSpecificItem) {
							if (UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->GetVisibility() == ESlateVisibility::Visible)
								if (UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren().Num() > UIManagerWorldSubsystem->InventoryMenuWidget->SelectedItemButtonIndex
									&& UIManagerWorldSubsystem->InventoryMenuWidget->SelectedItemButtonIndex >= 0) {
										if (UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->GetChildAt
											(UIManagerWorldSubsystem->InventoryMenuWidget->SelectedItemButtonIndex) 
											== Cast<UWidget>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter()))
												UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
										else
												UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
								}
								else
									UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
						}
						else
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
					}
				}
				if (UIManagerWorldSubsystem->PickedButtonIndex == SelectedScrollBoxChildren.Num() - 1) {
					if (UInventoryScrollBoxEntryWidget* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[0]); IsValid(SelectedEntryWidget)) {
						UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
						UIManagerWorldSubsystem->InventoryMenuWidget->SetPickedItem(SelectedEntryWidget->GetItem());
						UIManagerWorldSubsystem->InventoryMenuWidget->SetItemInfo(SelectedEntryWidget->GetItem());
					}
					else if(auto* InventoryTargetEntry = Cast<UInventoryTargetEntry>(SelectedScrollBoxChildren[0]); IsValid(InventoryTargetEntry))
						UIManagerWorldSubsystem->PickedButton = InventoryTargetEntry->GetNameButtonWithHoverColors();
					else 
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[0]);
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					UIManagerWorldSubsystem->InventoryMenuWidget->CheckForButtonsBordersVisibilityAndSetGreenColorToButton();
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
				else {
					if (UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingSpecificItem) {
						if (auto* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex + 1]); IsValid(SelectedEntryWidget)) {
							UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
							UIManagerWorldSubsystem->InventoryMenuWidget->SetPickedItem(SelectedEntryWidget->GetItem());
							UIManagerWorldSubsystem->InventoryMenuWidget->SetItemInfo(SelectedEntryWidget->GetItem());
						}
					}
					else if (UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingTarget) {
						if (auto* InventoryTargetEntry = Cast<UInventoryTargetEntry>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex + 1]); IsValid(InventoryTargetEntry))
							UIManagerWorldSubsystem->PickedButton = InventoryTargetEntry->GetNameButtonWithHoverColors();
					}
					else {
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
						UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetMeleeButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetRangeButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetHeadButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetTorseButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetHandButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->GetLowerArmorButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.f));
						UIManagerWorldSubsystem->InventoryMenuWidget->CheckForButtonsBordersVisibilityAndSetGreenColorToButton();
					}
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
			}
		 }
	else if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu) && IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox) &&
		UIManagerWorldSubsystem->ContainerInventoryMenu->IsInViewport()) {
			TArray<UWidget*> SelectedVerticalBoxChildren = UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox->GetAllChildren();
			if (SelectedVerticalBoxChildren.Num() > 0) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
					if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget) &&
						UIManagerWorldSubsystem->PickedButton == UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget->GetMainButton())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					else if (IsValid(Cast<UInventoryScrollBoxEntryWidget>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter())))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
					else
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				}
				if (UIManagerWorldSubsystem->PickedButtonIndex == SelectedVerticalBoxChildren.Num() - 1) {
					if (UInventoryScrollBoxEntryWidget* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedVerticalBoxChildren[0]); IsValid(SelectedEntryWidget))
						UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
					else
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedVerticalBoxChildren[0]);
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
				else {
					if (auto* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedVerticalBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex + 1]); IsValid(SelectedEntryWidget))
						UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
					else
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedVerticalBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
			}
		}
}

void APlayerCharacter::InputAction()
{
	if (CanOpenOtherMenus) {
		AActor* ActorResult = ForwardRay().GetActor();
		if (IsValid(ActorResult)) {
			ChangeLevel(ActorResult);
			PickUpItem(ActorResult);
			DialogueInteract(ActorResult);
			MiscellaneousActions(ActorResult);
		}
	}
	if (IsValid(UIManagerWorldSubsystem) && IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->OnClicked.Broadcast();
}

void APlayerCharacter::InputAttack()
{
	PlayerAnimInstance->SetPlayerIsAttacking(true);
	CanOpenOtherMenus = false;
}

void APlayerCharacter::InputBack()
{
	FString MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (IsValid(Controller) && IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget) && UIManagerWorldSubsystem->InventoryMenuWidget->IsInViewport() 
			&& UIManagerWorldSubsystem->InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible) {
			UIManagerWorldSubsystem->InventoryMenuWidget->BattleMenuItemsBackButtonOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget) && UIManagerWorldSubsystem->InventoryMenuWidget->IsInViewport() 
			&& UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingTarget) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			if(auto* InventoryEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox()->
				GetAllChildren()[UIManagerWorldSubsystem->InventoryMenuWidget->SelectedItemButtonIndex]))
					UIManagerWorldSubsystem->PickedButton = InventoryEntryWidget->GetMainButton();
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->InventoryMenuWidget->SelectedItemButtonIndex;
			UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingTarget = false;
			UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingSpecificItem = true;
			UIManagerWorldSubsystem->InventoryMenuWidget->SelectedPanelWidget = UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryScrollBox();
		}
		else if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget) && UIManagerWorldSubsystem->InventoryMenuWidget->IsInViewport() 
			&& UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingSpecificItem) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(UIManagerWorldSubsystem->InventoryMenuWidget->GetItemTypeStackBox()->
					GetAllChildren()[UIManagerWorldSubsystem->InventoryMenuWidget->SelectedTypeButtonIndex]);
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->InventoryMenuWidget->SelectedTypeButtonIndex;
				UIManagerWorldSubsystem->InventoryMenuWidget->IsSelectingSpecificItem = false;
				UIManagerWorldSubsystem->InventoryMenuWidget->SelectedPanelWidget = UIManagerWorldSubsystem->InventoryMenuWidget->GetItemTypeStackBox();
		}
		else if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget) && UIManagerWorldSubsystem->InventoryMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->InventoryMenuWidget->BackButtonOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->PlayerMenuWidget) && UIManagerWorldSubsystem->PlayerMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->PlayerMenuWidget->CloseButtonOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->BattleMenuWidget) && UIManagerWorldSubsystem->BattleMenuWidget->IsInViewport() 
			&& UIManagerWorldSubsystem->BattleMenuWidget->GetAttackMenuBorder()->GetVisibility() == ESlateVisibility::Visible) {
				UIManagerWorldSubsystem->BattleMenuWidget->AttackMenuBackButtonOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->PauseMenuWidget) && UIManagerWorldSubsystem->PauseMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->PauseMenuWidget->ResumeButtonOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) && IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget)) {
			if (UIManagerWorldSubsystem->SpellBattleMenuWidget->IsInViewport() && !UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport()) {
				if (!UIManagerWorldSubsystem->SpellInfoWidget->IsInViewport())
					UIManagerWorldSubsystem->SpellBattleMenuWidget->BackButtonOnClicked();
				else
					UIManagerWorldSubsystem->SpellBattleMenuWidget->BackToSpellCreationButtonOnClicked();
			}
		}
		else if (IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget) && UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->BackButtonOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget) && UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->SkillBattleMenuWidget->BackButtonWithNeighborsOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget) && UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->BackButtonOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->SaveLoadGameMenuWidget) && UIManagerWorldSubsystem->SaveLoadGameMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->SaveLoadGameMenuWidget->BackButtonWithNeighborsOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->PartyMenuWidget) && UIManagerWorldSubsystem->PartyMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->PartyMenuWidget->BackButtonOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget) && UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->BackButtonOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->SettingsMenuWidget) && UIManagerWorldSubsystem->SettingsMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->SettingsMenuWidget->BackButtonOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->PerksLevelingUpMenuWidget) && UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->BackButtonOnClicked();
		}
		else if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget) && UIManagerWorldSubsystem->JournalMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->JournalMenuWidget->BackButtonOnClicked();
		}
	}
}

void APlayerCharacter::ChangeLevel(const AActor* const ActorResult)
{
	//If ray result isn't null and is level change trigger,loading screen is set, then load level and save a data into the GameInstance. 
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (auto* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance)) {
			if (IsValid(PlayerController))
				if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToDungeon"))) || ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToTown")))) {
					if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToDungeon"))))
						UGameplayStatics::OpenLevel(GetWorld(), "Dungeon");
					else if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToTown"))))
						UGameplayStatics::OpenLevel(GetWorld(), "Town");
					RedemptionGameInstance->SaveGameIntoGameInstance();
					RedemptionGameInstance->IsChangingLevel = true;
				}
		}
}

void APlayerCharacter::MiscellaneousActions(const AActor* const ActorResult)
{
	if (ActorResult->ActorHasTag(FName(TEXT("EndGameActor")))) {
		if(const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)){
			UIManagerWorldSubsystem->StartFinishGameScreenWidget = CreateWidget<UStartFinishGameScreen>(PlayerController,
				RedemptionGameModeBase->GetStartFinishGameScreenClass());
			if (IsValid(UIManagerWorldSubsystem->StartFinishGameScreenWidget)) {
				UIManagerWorldSubsystem->StartFinishGameScreenWidget->AddToViewport(0);
			}
		}
	}
}

void APlayerCharacter::CreateNotification(const FText& NotificationText, float NotificationTime)
{
	if (IsValid(UIManagerWorldSubsystem->NotificationWidget)) {
		if (NotificationQueue.IsEmpty() && !UIManagerWorldSubsystem->NotificationWidget->IsInViewport()) {
			FTimerHandle RemoveNotificationTimerHandle{};
			UIManagerWorldSubsystem->NotificationWidget->AddToViewport();
			UIManagerWorldSubsystem->NotificationWidget->SetNotificationTextBlockText(NotificationText);
			FTimerDelegate RemoveNotificationTimerDelegate = FTimerDelegate::CreateUObject(this, &APlayerCharacter::RemoveNotification, NotificationTime);
			GetWorld()->GetTimerManager().SetTimer(RemoveNotificationTimerHandle, RemoveNotificationTimerDelegate, NotificationTime, false);
		}
		else
			NotificationQueue.Add(NotificationText);
	}
}

void APlayerCharacter::RemoveNotification(const float NewNotificationTime)
{
	if (IsValid(UIManagerWorldSubsystem->NotificationWidget)) {
		if (NotificationQueue.IsEmpty())
			UIManagerWorldSubsystem->NotificationWidget->RemoveFromParent();
		else {
			FTimerHandle RemoveNotificationTimerHandle{};
			UIManagerWorldSubsystem->NotificationWidget->SetNotificationTextBlockText(NotificationQueue.Pop());
			FTimerDelegate RemoveNotificationTimerDelegate = FTimerDelegate::CreateUObject(this, &APlayerCharacter::RemoveNotification, NewNotificationTime);
			GetWorld()->GetTimerManager().SetTimer(RemoveNotificationTimerHandle, RemoveNotificationTimerDelegate, NewNotificationTime, false);
		}
	}
}


void APlayerCharacter::PickUpItem(AActor* const ActorResult)
{
	if (ActorResult->ActorHasTag(FName(TEXT("Loot")))) 
		if (ALootInTheWorld* LootInTheWorld = Cast<ALootInTheWorld>(ActorResult); IsValid(LootInTheWorld)) {
			if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
				UIManagerWorldSubsystem->ContainerInventoryMenu = CreateWidget<UContainerInventoryMenu>(PlayerController, RedemptionGameModeBase->GetContainerInventoryMenuClass());
			if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu)) {
				UIManagerWorldSubsystem->ContainerInventoryMenu->ContainerObject = Cast<ALootInTheWorld>(ActorResult);
				UIManagerWorldSubsystem->ContainerInventoryMenu->FillContainerInventory(LootInTheWorld->GetItemsClasses());
				UIManagerWorldSubsystem->ContainerInventoryMenu->AddToViewport();
				PlayerController->SetPause(true);
				PlayerController->bShowMouseCursor = true;
				PlayerController->bEnableClickEvents = true;
				PlayerController->bEnableMouseOverEvents = true;
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UIManagerWorldSubsystem->PauseMenuWidget, EMouseLockMode::DoNotLock);
				//PlayerController->ActivateTouchInterface(EmptyTouchInterface);
				CanOpenOtherMenus = false;
			}
		}
}

void APlayerCharacter::DialogueInteract(const AActor* const ActorResult)
{
	//If hit result from forward vector is NPC with a dialogue, start the dialogue 
	if (IsValid(ActorResult))
		if (ActorResult->GetClass()->ImplementsInterface(UDialogueActionsInterface::StaticClass())) {
			if (ATownNPC* TownNPC = const_cast<ATownNPC*>(Cast<ATownNPC>(ActorResult)); IsValid(TownNPC)) {
				if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
					if (IsValid(RedemptionGameModeBase->GetDialogueBoxClass()))
						UIManagerWorldSubsystem->DialogueBoxWidget = CreateWidget<UDialogueBox>(Cast<APlayerController>(this->GetController()), RedemptionGameModeBase->GetDialogueBoxClass());
					TownNPC->Execute_StartADialogue(TownNPC);
					IsInDialogue = true;
					FString MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
					//Set non combat enemies' detection to 0, so they won't attack during a dialogue.
					if (MapName == "Dungeon")
						for (TActorIterator<ANonCombatEnemyNPC> ActorItr(GetWorld()); ActorItr; ++ActorItr)
						{
							if (auto* const NonCombatEnemyNPCAIController = ActorItr->GetController<ANonCombatEnemyNPCAIController>(); IsValid(NonCombatEnemyNPCAIController))
								NonCombatEnemyNPCAIController->ReturnToPatrolling();
						}
				}
			}
			//if(IsValid(PlayerController))
			//	PlayerController->ActivateTouchInterface(EmptyTouchInterface);
		}
}

void APlayerCharacter::CopyInfoFromCombatPlayer(const class ACombatPlayerCharacter* const CombatPlayerCharacter)
{
	SkillsMap = CombatPlayerCharacter->GetSkillsMap();
	SkillsProgressMap = CombatPlayerCharacter->GetSkillsProgressMap();
	CurrentHP = CombatPlayerCharacter->CurrentHP;
	CurrentMana = CombatPlayerCharacter->CurrentMana;
	Level = CombatPlayerCharacter->Level;
	CurrentExperience = CombatPlayerCharacter->CurrentExperience;
	NumberOfPerkPoints = CombatPlayerCharacter->NumberOfPerkPoints;
	LevelingUpCounter = CombatPlayerCharacter->LevelingUpCounter;
}

void APlayerCharacter::CheckForwardRayHitResult()
{
	//If hit result from forward vector is NPC or interactive object, add its name to the viewport
	AActor* ActorResult = ForwardRay().GetActor();
	if (IsValid(ActorResult) && IsValid(UIManagerWorldSubsystem->ForwardRayInfoWidget)) {
		if (ActorResult->ActorHasTag(FName(TEXT("NPC")))) {
			if (!UIManagerWorldSubsystem->ForwardRayInfoWidget->IsInViewport()) {
				UIManagerWorldSubsystem->ForwardRayInfoWidget->AddToViewport(0);
				UIManagerWorldSubsystem->ForwardRayInfoWidget->SetMainTextBlockText(FText::FromName(Cast<ACharacterInTheWorld>(ActorResult)->GetCharacterName()));
			}
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToDungeon")))) {
			if (!UIManagerWorldSubsystem->ForwardRayInfoWidget->IsInViewport()) {
				UIManagerWorldSubsystem->ForwardRayInfoWidget->AddToViewport(0);
				UIManagerWorldSubsystem->ForwardRayInfoWidget->SetMainTextBlockText(FText::FromString("Enter the dungeon"));
			}
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToTown")))) {
			if (!UIManagerWorldSubsystem->ForwardRayInfoWidget->IsInViewport()) {
				UIManagerWorldSubsystem->ForwardRayInfoWidget->AddToViewport(0);
				UIManagerWorldSubsystem->ForwardRayInfoWidget->SetMainTextBlockText(FText::FromString("Enter the town"));
			}
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("Loot")))) {
			ALootInTheWorld* LootInTheWorld = Cast<ALootInTheWorld>(ActorResult);
			if (IsValid(LootInTheWorld))
				if (!UIManagerWorldSubsystem->ForwardRayInfoWidget->IsInViewport()) {
					UIManagerWorldSubsystem->ForwardRayInfoWidget->AddToViewport(0);
					UIManagerWorldSubsystem->ForwardRayInfoWidget->SetMainTextBlockText(FText::FromName(LootInTheWorld->GetName()));
			}
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("EndGameActor")))) {
			if (!UIManagerWorldSubsystem->ForwardRayInfoWidget->IsInViewport()) {
				UIManagerWorldSubsystem->ForwardRayInfoWidget->AddToViewport(0);
				UIManagerWorldSubsystem->ForwardRayInfoWidget->SetMainTextBlockText(FText::FromString("Hit the road"));
			}
		}
		//Remove from viewport, if ray didn't hit NPC or interactive object
		else if(UIManagerWorldSubsystem->ForwardRayInfoWidget->IsInViewport())
			UIManagerWorldSubsystem->ForwardRayInfoWidget->RemoveFromParent();
	}
	//Remove from viewport, if ray hit nothing
	else if(!IsValid(ActorResult) && IsValid(UIManagerWorldSubsystem->ForwardRayInfoWidget)) {
		if(UIManagerWorldSubsystem->ForwardRayInfoWidget->IsInViewport())
			UIManagerWorldSubsystem->ForwardRayInfoWidget->RemoveFromParent();
	}
}

void APlayerCharacter::LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance)
{
	if (!GameInstance->IsChangingLevel) {
		//Need this, cause character is floating in the air after a spawn.
		FTransform NewPlayerTransform = GameInstance->PlayerCharacterInstanceDataStruct.PlayerTransform;
		FVector NewLocationVector = NewPlayerTransform.GetLocation();
		NewLocationVector.Z = NewLocationVector.Z - 5;
		NewPlayerTransform.SetLocation(NewLocationVector);
		SetActorTransform(NewPlayerTransform);
	}
	else if (GameInstance->IsChangingLevel && !GameInstance->StartedNewGame) {
		if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Town") {
			SetActorLocation(FVector(-2600.0, -230.0, 525.0));
			SetActorRotation(FRotator(0.0, -180.0, 0.0));
		}
	}
	FMemoryReader MemReader(GameInstance->PlayerCharacterInstanceDataStruct.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Serialize(Ar);
	if (IsValid(PlayerController)) {
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
		PlayerController->SetPause(false);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {

	// Get the local player subsystem
	APlayerController* PC = Cast<APlayerController>(this->GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	// Clear out existing mapping, and add our mapping
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
	// Get the EnhancedInputComponent
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// Bind the actions
	if(IsValid(PEI))
	{
		PEI->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		PEI->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		PEI->BindAction(InputActions->InputJump, ETriggerEvent::Started, this, &APlayerCharacter::InputJump);
		PEI->BindAction(InputActions->InputAction, ETriggerEvent::Started, this, &APlayerCharacter::InputAction);
		PEI->BindAction(InputActions->InputOpenPlayerMenu, ETriggerEvent::Started, this, &APlayerCharacter::InputOpenPlayerMenu);
		PEI->BindAction(InputActions->InputScrollLeft, ETriggerEvent::Started, this, &APlayerCharacter::InputScrollLeft);
		PEI->BindAction(InputActions->InputScrollRight, ETriggerEvent::Started, this, &APlayerCharacter::InputScrollRight);
		PEI->BindAction(InputActions->InputScrollUp, ETriggerEvent::Started, this, &APlayerCharacter::InputScrollUp);
		PEI->BindAction(InputActions->InputScrollDown, ETriggerEvent::Started, this, &APlayerCharacter::InputScrollDown);
		PEI->BindAction(InputActions->InputPause, ETriggerEvent::Started, this, &APlayerCharacter::InputOpenPauseMenu);
		PEI->BindAction(InputActions->InputBack, ETriggerEvent::Started, this, &APlayerCharacter::InputBack);
		PEI->BindAction(InputActions->InputSpellUseSaveLoad, ETriggerEvent::Started, this, &APlayerCharacter::InputSpellUseSaveLoad);
		PEI->BindAction(InputActions->InputUniqueSpellUseSpellInfoToggle, ETriggerEvent::Started, this, &APlayerCharacter::InputUniqueSpellUseSpellInfoToggle);
		PEI->BindAction(InputActions->InputSpellReset, ETriggerEvent::Started, this, &APlayerCharacter::InputSpellReset);
		PEI->BindAction(InputActions->InputOpenLearnedSpells, ETriggerEvent::Started, this, &APlayerCharacter::InputOpenLearnedSpells);
		PEI->BindAction(InputActions->InputOpenSpellInfo, ETriggerEvent::Started, this, &APlayerCharacter::InputOpenSpellInfo);
		PEI->BindAction(InputActions->InputAttack, ETriggerEvent::Started, this, &APlayerCharacter::InputAttack);
	}
}
//Ray of detecting objects in front of a player
FHitResult APlayerCharacter::ForwardRay()
{
	FHitResult HitResult(ForceInit);
	FVector StartTrace = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector EndTrace = (ForwardVector * ForwardRayRange) + StartTrace;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	CQP->AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, *CQP);
	return HitResult;
}

void APlayerCharacter::InitializeStats()
{
	StatsMap.Add(ECharacterStats::STRENGTH, 1);
	StatsMap.Add(ECharacterStats::PERCEPTION, 1);
	StatsMap.Add(ECharacterStats::ENDURANCE, 1);
	StatsMap.Add(ECharacterStats::CHARISMA, 1);
	StatsMap.Add(ECharacterStats::INTELLIGENCE, 1);
	StatsMap.Add(ECharacterStats::WILL, 1);
	StatsMap.Add(ECharacterStats::AGILITY, 1);
	StatsMap.Add(ECharacterStats::LUCK, 1);
}

void APlayerCharacter::InitializeSkills()
{
	SkillsMap.Add(ECharacterSkills::MELEE, 1);
	SkillsMap.Add(ECharacterSkills::RANGE, 1);
	SkillsMap.Add(ECharacterSkills::ASSAULTSPELLS, 1);
	SkillsMap.Add(ECharacterSkills::DEBUFFSPELLS, 1);
	SkillsMap.Add(ECharacterSkills::RESTORATIONSPELLS, 1);
	SkillsMap.Add(ECharacterSkills::BUFFSPELLS, 1);
	SkillsMap.Add(ECharacterSkills::DEFEND, 1);
	SkillsMap.Add(ECharacterSkills::PERSUASION, 1);
}

void APlayerCharacter::InitializeSkillsProgress()
{
	SkillsProgressMap.Add(ECharacterSkills::MELEE, 0);
	SkillsProgressMap.Add(ECharacterSkills::RANGE, 0);
	SkillsProgressMap.Add(ECharacterSkills::ASSAULTSPELLS, 0);
	SkillsProgressMap.Add(ECharacterSkills::DEBUFFSPELLS, 0);
	SkillsProgressMap.Add(ECharacterSkills::RESTORATIONSPELLS, 0);
	SkillsProgressMap.Add(ECharacterSkills::BUFFSPELLS, 0);
	SkillsProgressMap.Add(ECharacterSkills::DEFEND, 0);
	SkillsProgressMap.Add(ECharacterSkills::PERSUASION, 0);
}

void APlayerCharacter::FinishGame()
{
	if (IsValid(PlayerController)) {
		UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	}
}

void APlayerCharacter::RestartBattleMenuWidget()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (IsValid(RedemptionGameModeBase->GetBattleMenuClass()) && IsValid(PlayerController))
			UIManagerWorldSubsystem->BattleMenuWidget = CreateWidget<UBattleMenu>(PlayerController, RedemptionGameModeBase->GetBattleMenuClass());
}

void APlayerCharacter::RestartBattleResultsScreenWidget()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (IsValid(RedemptionGameModeBase->GetBattleResultsScreenClass()) && IsValid(PlayerController))
			UIManagerWorldSubsystem->BattleResultsScreenWidget = CreateWidget<UBattleResultsScreen>(PlayerController, 
				RedemptionGameModeBase->GetBattleResultsScreenClass());
}

void APlayerCharacter::AddNewAllyToAllies(ACombatAllyNPC* const AllyToAdd)
{
	Allies.Add(AllyToAdd);
}

void APlayerCharacter::AddNewSkillToAvailableSkills(const TSubclassOf<ASpell> SkillClassToAdd)
{
	AvailableSkills.Add(SkillClassToAdd);
}

bool APlayerCharacter::GetCanInput() const
{
	return CanOpenOtherMenus;
}

UTouchInterface* APlayerCharacter::GetEmptyTouchInterface() const
{
	return EmptyTouchInterface;
}

UTouchInterface* APlayerCharacter::GetStandardTouchInterface() const
{
	return StandardTouchInterface;
}

const TArray<TSubclassOf<ASpell>>& APlayerCharacter::GetAvailableSkills() const
{
	return AvailableSkills;
}

TArray<ACombatAllyNPC*> APlayerCharacter::GetAllies() const
{
	return Allies;
}

const int8 APlayerCharacter::GetStat(const ECharacterStats StatToGet) const
{
	return *StatsMap.Find(StatToGet);
}

const int8 APlayerCharacter::GetSkill(const ECharacterSkills SkillToGet) const
{
	return *SkillsMap.Find(SkillToGet);
}

const int8 APlayerCharacter::GetSkillsProgress(const ECharacterSkills SkillToGet) const
{
	return *SkillsProgressMap.Find(SkillToGet);
}

float APlayerCharacter::GetHealthPercentage() const
{
	return CurrentHP / MaxHP;
}

float APlayerCharacter::GetManaPercentage() const
{
	return CurrentMana / MaxMana;
}

const TArray<FText>& APlayerCharacter::GetPerksCategoryNames() const
{
	return PerksCategoryNames;
}

const TArray<TSubclassOf<APerk>>& APlayerCharacter::GetAvailablePerks() const
{
	return AvailablePerks;
}

void APlayerCharacter::SetCanInput(bool Value)
{
	CanOpenOtherMenus = Value;
}

void APlayerCharacter::SetAllies(const TArray<TSubclassOf<ACombatAllies>>& NewAllies)
{
}

void APlayerCharacter::SetStat(const ECharacterStats StatToSet, const int8 NewValue)
{
	StatsMap.Emplace(StatToSet, NewValue);
}

void APlayerCharacter::SetSkill(const ECharacterSkills SkillToSet, const int8 NewValue)
{
	SkillsMap.Emplace(SkillToSet, NewValue);
}

void APlayerCharacter::SetSkillsProgress(const ECharacterSkills SkillToSet, const int8 NewValue)
{
	SkillsProgressMap.Emplace(SkillToSet, NewValue);
}

void APlayerCharacter::AddAvailableSkill(const TSubclassOf<ASpell> SkillToAdd)
{
	AvailableSkills.Add(SkillToAdd);
}

void APlayerCharacter::AddSkillsProgress(const ECharacterSkills SkillToAddTo, const int16 ValueToAdd)
{
	SkillsProgressMap.Emplace(SkillToAddTo, *SkillsProgressMap.Find(SkillToAddTo) + ValueToAdd);
}

void APlayerCharacter::AddSkills(const ECharacterSkills SkillToAddTo, const int16 ValueToAdd)
{
	SkillsMap.Emplace(SkillToAddTo, *SkillsMap.Find(SkillToAddTo) + ValueToAdd);
}
