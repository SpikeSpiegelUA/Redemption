// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "..\UI\Screens\LoadingScreen.h"
#include "..\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "..\Dynamics\World\LootInTheWorld.h"
#include "..\Dynamics\Logic\Interfaces\DialogueActionsInterface.h"
#include "..\Characters\NonCombat\TownNPC.h"
#include "..\Dynamics\World\Items\WeaponItem.h"
#include "..\Dynamics\World\Items\ArmorItem.h"
#include "..\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "..\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "..\Characters\Combat\CombatPlayerCharacter.h"
#include "..\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "..\UI\Menus\MainMenu.h"
#include "GameFramework/TouchInterface.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Components/StackBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "..\Miscellaneous\InventoryActions.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "EngineUtils.h"

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

	//HUD
	ForwardRayInfoWidget = nullptr;
	ForwardRayInfoClass = nullptr;

	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void APlayerCharacter::BeginPlay() 
{
	Super::BeginPlay();

	//Find managers in the world and set corresponding variables
	ParticlesManager = Cast<AParticlesManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AParticlesManager::StaticClass()));
	AudioManager = Cast<AAudioManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AAudioManager::StaticClass()));
	BattleManager = Cast<ABattleManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABattleManager::StaticClass()));
	GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
	EffectsManager = Cast<AEffectsSpellsAndSkillsManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEffectsSpellsAndSkillsManager::StaticClass()));
	PlayerController = GetWorld()->GetFirstPlayerController();
	if(IsValid(PlayerController))
		PlayerSkeletalMesh = PlayerController->GetPawn()->FindComponentByClass<USkeletalMeshComponent>();
	if (IsValid(PlayerSkeletalMesh))
		PlayerAnimInstance = Cast<UPlayerCharacterAnimInstance>(PlayerSkeletalMesh->GetAnimInstance());
	RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	//Creating UI widgets...
	if (IsValid(PlayerController)) {
		PlayerController->SetTickableWhenPaused(true);
		if (IsValid(ForwardRayInfoClass))
			ForwardRayInfoWidget = CreateWidget<UForwardRayInfo>(PlayerController, ForwardRayInfoClass);
		if (IsValid(PlayerMenuClass))
			PlayerMenuWidget = CreateWidget<UPlayerMenu>(PlayerController, PlayerMenuClass);
		if (IsValid(InventoryScrollBoxEntryClass))
			InventoryScrollBoxEntryWidget = CreateWidget<UInventoryScrollBoxEntryWidget>(PlayerController, InventoryScrollBoxEntryClass);
		if (IsValid(BattleMenuClass))
			BattleMenuWidget = CreateWidget<UBattleMenu>(PlayerController, BattleMenuClass);
		if (IsValid(BattleResultsScreenClass))
			BattleResultsScreenWidget = CreateWidget<UBattleResultsScreen>(PlayerController, BattleResultsScreenClass);
		if (IsValid(InventoryMenuClass))
			InventoryMenuWidget = CreateWidget<UInventoryMenu>(PlayerController, InventoryMenuClass);
		if (IsValid(PauseMenuClass))
			PauseMenuWidget = CreateWidget<UPauseMenu>(PlayerController, PauseMenuClass);
		if (IsValid(AlliesInfoBarsClass))
			AlliesInfoBarsWidget = CreateWidget<UAlliesInfoBars>(PlayerController, AlliesInfoBarsClass);
		if (IsValid(DialogueBoxClass))
			DialogueBoxWidget = CreateWidget<UDialogueBox>(PlayerController, DialogueBoxClass);
		if (IsValid(ResponsesBoxClass))
			ResponsesBoxWidget = CreateWidget<UResponsesBox>(PlayerController, ResponsesBoxClass);
		if (IsValid(NotificationClass))
			NotificationWidget = CreateWidget<UNotification>(PlayerController, NotificationClass);
		if (IsValid(DeathMenuClass))
			DeathMenuWidget = CreateWidget<UDeathMenu>(PlayerController, DeathMenuClass);
		if (IsValid(SpellBattleMenuClass))
			SpellBattleMenuWidget = CreateWidget<USpellBattleMenu>(PlayerController, SpellBattleMenuClass);
		if (IsValid(LearnedSpellsJournalMenuClass)) 
			LearnedSpellsJournalMenuWidget = CreateWidget<ULearnedSpellsJournalMenu>(PlayerController, LearnedSpellsJournalMenuClass);
		if (IsValid(SettingsMenuClass))
			SettingsMenuWidget = CreateWidget<USettingsMenu>(PlayerController, SettingsMenuClass);
		if (IsValid(MainMenuClass))
			MainMenuWidget = CreateWidget<UMainMenu>(PlayerController, MainMenuClass);
		if (IsValid(SkillBattleMenuClass))
			SkillBattleMenuWidget = CreateWidget<USkillBattleMenu>(PlayerController, SkillBattleMenuClass);
		if (IsValid(SpellInfoClass))
			SpellInfoWidget = CreateWidget<USpellInfo>(PlayerController, SpellInfoClass);
		if (IsValid(CombatCharacterInfoMenuClass))
			CombatCharacterInfoMenuWidget = CreateWidget<UCombatCharacterInfoMenu>(PlayerController, CombatCharacterInfoMenuClass);
		if (IsValid(SaveLoadGameMenuClass))
			SaveLoadGameMenuWidget = CreateWidget<USaveLoadGameMenu>(PlayerController, SaveLoadGameMenuClass);
	}
	//Level change logic
	if (IsValid(RedemptionGameInstance))
		Execute_LoadObjectFromGameInstance(this, RedemptionGameInstance);
	if (IsValid(GetWorld()))
		UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	//Main Menu Turn On Logic
	FString MapName = GetWorld()->GetMapName();
	if (MapName == "UEDPIE_0_MainMenu") {
		MainMenuWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->SetPause(true);
		//PlayerController->ActivateTouchInterface(PlayerCharacter->GetEmptyTouchInterface());
		if (IsValid(UIManagerWorldSubsystem)) {
			UIManagerWorldSubsystem->PickedButton = MainMenuWidget->GetNewGameButton();
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			CanOpenOtherMenus = false;
		}
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, MainMenuWidget, EMouseLockMode::DoNotLock);
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
	if (SpellBattleMenuWidget->IsInViewport())
		SpellBattleMenuWidget->UseButtonOnClicked();
	else if (SkillBattleMenuWidget->IsInViewport())
		SkillBattleMenuWidget->UseButtonWithNeighborsOnClicked();
	else if (SaveLoadGameMenuWidget->IsInViewport())
		SaveLoadGameMenuWidget->SaveLoadButtonWithNeighborsOnClicked();
}

void APlayerCharacter::InputUniqueSpellUseSpellInfoToggle()
{
	if(SpellBattleMenuWidget->IsInViewport() && SpellBattleMenuWidget->GetToggleSpellInfoButton()->GetVisibility() == ESlateVisibility::Visible)
		SpellBattleMenuWidget->ToggleSpellInfoButtonOnClicked();
	else if(SpellBattleMenuWidget->IsInViewport())
		SpellBattleMenuWidget->UseUniqueSpellButtonOnClicked();
}

void APlayerCharacter::InputSpellReset()
{
	if (SpellBattleMenuWidget->IsInViewport()) {
		SpellBattleMenuWidget->ResetButtonOnClicked();
		SpellBattleMenuWidget->ResetUIKeyboardControlLogic();
	}
}

void APlayerCharacter::InputOpenLearnedSpells()
{
	if (SpellBattleMenuWidget->IsInViewport())
		SpellBattleMenuWidget->LearnedSpellsJournalButtonOnClicked();
}

void APlayerCharacter::InputOpenSpellInfo()
{
	if (SpellBattleMenuWidget->IsInViewport())
		SpellBattleMenuWidget->ShowResultSpellButtonOnClicked();
}

void APlayerCharacter::InputOpenPlayerMenu()
{
	if (!PlayerMenuWidget->IsInViewport() && CanOpenOtherMenus) {
		PlayerController->SetPause(true);
		PlayerMenuWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, PlayerMenuWidget, EMouseLockMode::DoNotLock);
		//PlayerController->ActivateTouchInterface(EmptyTouchInterface);
		PlayerMenuWidget->GetInventoryButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButton = PlayerMenuWidget->GetInventoryButton();
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		CanOpenOtherMenus = false;
	}
	else if(PlayerMenuWidget->IsInViewport()) {
		PlayerMenuWidget->RemoveFromParent();
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

void APlayerCharacter::InputOpenPauseMenu()
{
	FString MapName = GetWorld()->GetMapName();
	
	if (!PauseMenuWidget->IsInViewport() && CanOpenOtherMenus) {
		PlayerController->SetPause(true);
		PauseMenuWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, PauseMenuWidget, EMouseLockMode::DoNotLock);
		//PlayerController->ActivateTouchInterface(EmptyTouchInterface);
		PauseMenuWidget->GetResumeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButton = PauseMenuWidget->GetResumeButton();
		CanOpenOtherMenus = false;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
	else if(PauseMenuWidget->IsInViewport()) {
		PauseMenuWidget->RemoveFromParent();
		SettingsMenuWidget->RemoveFromParent();
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
	else if (SettingsMenuWidget->IsInViewport() && MapName != "UEDPIE_0_MainMenu") {
		SettingsMenuWidget->RemoveFromParent();
		PauseMenuWidget->AddToViewport();
		PauseMenuWidget->GetResumeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButton = PauseMenuWidget->GetResumeButton();
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
	else if (SettingsMenuWidget->IsInViewport() && MapName == "UEDPIE_0_MainMenu") {
		SettingsMenuWidget->RemoveFromParent();
		MainMenuWidget->AddToViewport();
		MainMenuWidget->GetNewGameButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButton = MainMenuWidget->GetNewGameButton();
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void APlayerCharacter::InputScrollLeft()
{
	if (IsValid(Controller)) 
		if (BattleMenuWidget->IsPreparingToAttack || BattleMenuWidget->IsPreparingToTalk || BattleMenuWidget->IsPreparingToViewInfo) {
			TArray<ACombatNPC*> TargetsForSelection{};
			//Add BattleEnemies in a loop, cause we need to convert them to the ACombatNPC.
			if (BattleMenuWidget->IsPreparingToViewInfo) {
				TargetsForSelection = BattleManager->BattleAlliesPlayer;
				for (ACombatNPC* CombatNPC : BattleManager->BattleEnemies)
					TargetsForSelection.Add(CombatNPC);
			}
			else if (BattleManager->IsSelectingAllyAsTarget)
				TargetsForSelection = BattleManager->BattleAlliesPlayer;
			else if (!BattleManager->IsSelectingAllyAsTarget)
				for (ACombatNPC* CombatNPC : BattleManager->BattleEnemies)
					TargetsForSelection.Add(CombatNPC);
			//Choose Target with scroll
			if (TargetsForSelection.Num() > 1) {
				int8 CurrentIndex = BattleManager->SelectedCombatNPCIndex;
				int8 StartIndex = BattleManager->SelectedCombatNPCIndex;
				while (true) {
					if (CurrentIndex + 1 < TargetsForSelection.Num()) {
						if (TargetsForSelection[CurrentIndex + 1]->GetCurrentHP() > 0) {
							BattleManager->SelectNewTargetCrosshairLogic(TargetsForSelection, CurrentIndex + 1, CurrentIndex, "Left");
							BattleManager->SelectNewTarget(TargetsForSelection[CurrentIndex + 1], CurrentIndex + 1);
							break;
						}
						else {
							CurrentIndex++;
							if (CurrentIndex == StartIndex)
								break;
						}
					}
					else {
						if (TargetsForSelection[0]->GetCurrentHP() > 0) {
							BattleManager->SelectNewTargetCrosshairLogic(TargetsForSelection, 0, CurrentIndex, "Left");
							BattleManager->SelectNewTarget(TargetsForSelection[0], 0);
							break;
						}
						else {
							CurrentIndex = 0;
							if (CurrentIndex == StartIndex)
								break;
						}
					}
				}
			}
		}
	if ((IsValid(SpellBattleMenuWidget) && SpellBattleMenuWidget->IsInViewport()) || (IsValid(LearnedSpellsJournalMenuWidget) && LearnedSpellsJournalMenuWidget->IsInViewport())
		|| (IsValid(SkillBattleMenuWidget) && SkillBattleMenuWidget->IsInViewport())) {
			if (UButtonWithNeighbors* PickedButtonWithNeighbors = Cast<UButtonWithNeighbors>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithNeighbors))
				for (FSideAndItsButton SideAndItsButton : PickedButtonWithNeighbors->SidesAndTheirButtons)
					if (SideAndItsButton.Side == ESides::LEFT) {
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
						UIManagerWorldSubsystem->PickedButton = SideAndItsButton.NeighborButton;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
					}
	}
}

void APlayerCharacter::InputScrollRight()
{
	if (IsValid(Controller))
		if (BattleMenuWidget->IsPreparingToAttack || BattleMenuWidget->IsPreparingToTalk || BattleMenuWidget->IsPreparingToViewInfo) {
			TArray<ACombatNPC*> TargetsForSelection{};
			//Add BattleEnemies in a loop, cause we need to convert them to the ACombatNPC.
			if (BattleMenuWidget->IsPreparingToViewInfo) {
				TargetsForSelection = BattleManager->BattleAlliesPlayer;
				for(ACombatNPC* CombatNPC : BattleManager->BattleEnemies)
					TargetsForSelection.Add(CombatNPC);
			}
			else if(BattleManager->IsSelectingAllyAsTarget)
				TargetsForSelection = BattleManager->BattleAlliesPlayer;
			else if (!BattleManager->IsSelectingAllyAsTarget) 
				for (ACombatNPC* CombatNPC : BattleManager->BattleEnemies)
					TargetsForSelection.Add(CombatNPC);
			if (TargetsForSelection.Num() > 1) {
				//Choose target with scroll
				int8 CurrentIndex = BattleManager->SelectedCombatNPCIndex;
				int8 StartIndex = BattleManager->SelectedCombatNPCIndex;
				//Need this for a sv suffix.
				while (true) {
					if (CurrentIndex - 1 >= 0) {
						if (TargetsForSelection[CurrentIndex - 1]->GetCurrentHP() > 0) {
							BattleManager->SelectNewTargetCrosshairLogic(TargetsForSelection, CurrentIndex - 1, CurrentIndex, "Right");
							BattleManager->SelectNewTarget(TargetsForSelection[CurrentIndex - 1], CurrentIndex - 1);
							break;
						}
						else {
							CurrentIndex--;
							if (CurrentIndex == StartIndex)
								break;
						}
					}
					else {
						if (TargetsForSelection[TargetsForSelection.Num() - 1]->GetCurrentHP() > 0) {
							BattleManager->SelectNewTargetCrosshairLogic(TargetsForSelection, TargetsForSelection.Num() - 1, CurrentIndex, "Right");
							BattleManager->SelectNewTarget(TargetsForSelection[TargetsForSelection.Num() - 1], TargetsForSelection.Num() - 1);
							break;
						}
						else {
							CurrentIndex = TargetsForSelection.Num() - 1;
							if (CurrentIndex == StartIndex)
								break;
						}
					}
				}
			}
		}
	if ((IsValid(SpellBattleMenuWidget) && SpellBattleMenuWidget->IsInViewport()) || (IsValid(LearnedSpellsJournalMenuWidget) && LearnedSpellsJournalMenuWidget->IsInViewport())
		|| (IsValid(SkillBattleMenuWidget) && SkillBattleMenuWidget->IsInViewport())) {
			if (UButtonWithNeighbors* PickedButtonWithNeighbors = Cast<UButtonWithNeighbors>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithNeighbors))
				for (FSideAndItsButton SideAndItsButton : PickedButtonWithNeighbors->SidesAndTheirButtons)
					if (SideAndItsButton.Side == ESides::RIGHT) {
						if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
							if(SpellBattleMenuWidget->IsInViewport())
								UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
							else if(SkillBattleMenuWidget->IsInViewport())
								UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
						}
						UIManagerWorldSubsystem->PickedButton = SideAndItsButton.NeighborButton;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
					}
	}
}

void APlayerCharacter::InputScrollUp()
{
	if (IsValid(Controller) && IsValid(UIManagerWorldSubsystem)) {
		if (UButtonWithNeighbors* PickedButtonWithNeighbors = Cast<UButtonWithNeighbors>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithNeighbors)) {
			for (FSideAndItsButton SideAndItsButton : PickedButtonWithNeighbors->SidesAndTheirButtons)
				if (SideAndItsButton.Side == ESides::UP) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
						if (SpellBattleMenuWidget->IsInViewport())
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.8f));
						else if (SkillBattleMenuWidget->IsInViewport())
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
						else if (SaveLoadGameMenuWidget->IsInViewport())
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1));
						else if (CombatCharacterInfoMenuWidget->IsInViewport())
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
					}
					UIManagerWorldSubsystem->PickedButton = SideAndItsButton.NeighborButton;
					if(SpellBattleMenuWidget->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
					else
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
		}
		else if (IsValid(BattleMenuWidget) && BattleMenuWidget->IsInViewport()) {
			if (BattleMenuWidget->IsChoosingAction) {
				TArray<UWidget*> MenuVerticalBoxChildrens = BattleMenuWidget->GetMenuVerticalBox()->GetAllChildren();
				if (MenuVerticalBoxChildrens.Num() > 1) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[MenuVerticalBoxChildrens.Num() - 1]);
						UIManagerWorldSubsystem->PickedButtonIndex = MenuVerticalBoxChildrens.Num() - 1;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
					else {
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
						UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
			else if (BattleMenuWidget->IsChoosingItem && !BattleMenuWidget->IsPreparingToAttack) {
				TArray<UWidget*> InventoryScrollBoxChildrens = InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren();
				if (InventoryScrollBoxChildrens.Num() > 1) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
						UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[InventoryScrollBoxChildrens.Num() - 1]);
						InventoryMenuWidget->GetInventoryScrollBox()->ScrollToEnd();
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
						UIManagerWorldSubsystem->PickedButtonIndex = InventoryScrollBoxChildrens.Num() - 1;
						UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
						InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
					else {
						UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
						UIManagerWorldSubsystem->PickedButtonIndex--;
						UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
						InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
			else if (BattleMenuWidget->IsPreparingToAttack || BattleMenuWidget->IsPreparingToTalk || BattleMenuWidget->IsPreparingToViewInfo) {
				TArray<UWidget*> AttackMenuVerticalBoxChildrens = BattleMenuWidget->GetAttackMenuVerticalBox()->GetAllChildren();
				if (AttackMenuVerticalBoxChildrens.Num() > 0) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButtonIndex = AttackMenuVerticalBoxChildrens.Num() - 1;
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex]);
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
					else {
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButtonIndex -= 1;
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex]);
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
		}
		else if (IsValid(PlayerMenuWidget) && PlayerMenuWidget->IsInViewport()) {
			TArray<UWidget*> PlayerMenuButtons{};
			PlayerMenuButtons.Add(PlayerMenuWidget->GetInventoryButton());
			PlayerMenuButtons.Add(PlayerMenuWidget->GetCloseMenuButton());
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[PlayerMenuButtons.Num() - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = PlayerMenuButtons.Num() - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
		else if (IsValid(PauseMenuWidget) && PauseMenuWidget->IsInViewport()) {
			TArray<UWidget*> PauseMenuButtons = PauseMenuWidget->GetButtonsStackBox()->GetAllChildren();
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[PauseMenuButtons.Num() - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = PauseMenuButtons.Num() - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
		else if (IsValid(MainMenuWidget) && MainMenuWidget->IsInViewport()) {
			TArray<UWidget*> MainMenuButtons = MainMenuWidget->GetMainVerticalBox()->GetAllChildren();
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::FromHex("8C8C8CFF"));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MainMenuButtons[MainMenuButtons.Num() - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = MainMenuButtons.Num() - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::FromHex("8C8C8CFF"));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MainMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
		else if (IsValid(SpellBattleMenuWidget) && SpellBattleMenuWidget->IsInViewport() && SpellBattleMenuWidget->CanUseKeyboardButtonSelection && !LearnedSpellsJournalMenuWidget->IsInViewport()) {
			TArray<UWidget*> SpellBattleMenuElementsButtons{};
			if (SpellBattleMenuWidget->GetSelectedSpellRange() == ESpellRange::NONE) {
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetNeighborsSpellRangeButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetEveryoneSpellRangeButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetSingleSpellRangeButton());
			}
			else if (SpellBattleMenuWidget->GetSelectedSpellElements().Num() >= 0) {
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetWaterElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetEarthElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetDarkElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetLightningElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetHolyElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetWindElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetFireElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetBloodElementButton());
			}
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SpellBattleMenuElementsButtons[SpellBattleMenuElementsButtons.Num() - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = SpellBattleMenuElementsButtons.Num() - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
			}
			else {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SpellBattleMenuElementsButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
			}
		}
		else if (IsValid(LearnedSpellsJournalMenuWidget) && LearnedSpellsJournalMenuWidget->IsInViewport() && LearnedSpellsJournalMenuWidget->CanUseKeyboardButtonSelection) {
			TArray<UWidget*> LearnedSpellsJournalMenuButtons = LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren();
			if (LearnedSpellsJournalMenuButtons.Num() > 1) {
				if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
					UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(LearnedSpellsJournalMenuButtons[LearnedSpellsJournalMenuButtons.Num() - 1])->GetMainButton();
					UIManagerWorldSubsystem->PickedButtonIndex = LearnedSpellsJournalMenuButtons.Num() - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
				}
				else {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
					UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(LearnedSpellsJournalMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1])->GetMainButton();
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
				}
			}
		}
		else if (IsValid(SaveLoadGameMenuWidget) && SaveLoadGameMenuWidget->IsInViewport()) {
			TArray<UWidget*> SaveLoadGameMenuWidgetSlotButtons = SaveLoadGameMenuWidget->GetSaveSlotsScrollBox()->GetAllChildren();
			if (SaveLoadGameMenuWidgetSlotButtons.Num() > 1) {
				if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
					UIManagerWorldSubsystem->PickedButton = Cast <USaveSlotEntry> (SaveLoadGameMenuWidgetSlotButtons[SaveLoadGameMenuWidgetSlotButtons.Num() - 1])->GetSlotButton();
					UIManagerWorldSubsystem->PickedButtonIndex = SaveLoadGameMenuWidgetSlotButtons.Num() - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				}
				else {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
					UIManagerWorldSubsystem->PickedButton = Cast<USaveSlotEntry>(SaveLoadGameMenuWidgetSlotButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1])->GetSlotButton();
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				}
			}
		}
		else if (IsValid(SkillBattleMenuWidget) && SkillBattleMenuWidget->IsInViewport() && SkillBattleMenuWidget->CanUseKeyboardButtonSelection) {
			TArray<UWidget*> SkillBattleMenuButtons = SkillBattleMenuWidget->GetSkillsScrollBox()->GetAllChildren();
			if (SkillBattleMenuButtons.Num() > 1) {
				if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.8f));
					UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(SkillBattleMenuButtons[SkillBattleMenuButtons.Num() - 1])->GetMainButton();
					UIManagerWorldSubsystem->PickedButtonIndex = SkillBattleMenuButtons.Num() - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				}
				else {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
					UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(SkillBattleMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1])->GetMainButton();
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				}
			}
		}
		else if (IsValid(InventoryMenuWidget) && IsValid(InventoryMenuWidget->SelectedPanelWidget) && InventoryMenuWidget->IsInViewport()) {
			TArray<UWidget*> SelectedScrollBoxChildren = InventoryMenuWidget->SelectedPanelWidget->GetAllChildren();
			if (SelectedScrollBoxChildren.Num() > 0) {
				if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
						if (InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible)
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
						else
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
					}
					if (InventoryMenuWidget->IsSelectingSpecificItem) {
						if (UInventoryScrollBoxEntryWidget* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[SelectedScrollBoxChildren.Num() - 1]); IsValid(SelectedEntryWidget)) {
							UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
							InventoryMenuWidget->SetPickedItem(SelectedEntryWidget->GetItem());
						}
					}
					else
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[SelectedScrollBoxChildren.Num() - 1]);
					UIManagerWorldSubsystem->PickedButtonIndex = SelectedScrollBoxChildren.Num() - 1;
					InventoryMenuWidget->CheckForButtonsBordersVisibilityAndSetGreenColorToButton();
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				else {
					if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
						if (InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible)
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
						else
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
					}
					if (InventoryMenuWidget->IsSelectingSpecificItem) {
						if (UInventoryScrollBoxEntryWidget* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex - 1]); IsValid(SelectedEntryWidget)) {
							UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
							InventoryMenuWidget->SetPickedItem(SelectedEntryWidget->GetItem());
						}
					}
					else {
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
						InventoryMenuWidget->GetInventoryButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetMeleeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetRangeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetHeadButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetTorseButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetHandButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetLowerArmorButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						Cast<UButton>(InventoryMenuWidget->GetItemTypeStackBox()->GetAllChildren()[InventoryMenuWidget->SelectedTypeButtonIndex])->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
						InventoryMenuWidget->CheckForButtonsBordersVisibilityAndSetGreenColorToButton();
					}
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
			}
		}
	}
}

void APlayerCharacter::InputScrollDown()
{
	if (IsValid(Controller) && IsValid(UIManagerWorldSubsystem)) {
		if (UButtonWithNeighbors* PickedButtonWithNeighbors = Cast<UButtonWithNeighbors>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithNeighbors)) {
			for (FSideAndItsButton SideAndItsButton : PickedButtonWithNeighbors->SidesAndTheirButtons)
				if (SideAndItsButton.Side == ESides::DOWN) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
						if (SpellBattleMenuWidget->IsInViewport())
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.8f));
						else if (SkillBattleMenuWidget->IsInViewport())
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
						else if (SaveLoadGameMenuWidget->IsInViewport())
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
						else if (CombatCharacterInfoMenuWidget->IsInViewport())
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
					}
					UIManagerWorldSubsystem->PickedButton = SideAndItsButton.NeighborButton;
					if (SpellBattleMenuWidget->IsInViewport())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
					else
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
		}
		else if (IsValid(BattleMenuWidget) && BattleMenuWidget->IsInViewport()) {
			if (BattleMenuWidget->IsChoosingAction) {
				TArray<UWidget*> MenuVerticalBoxChildrens = BattleMenuWidget->GetMenuVerticalBox()->GetAllChildren();
				if (MenuVerticalBoxChildrens.Num() > 1) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == MenuVerticalBoxChildrens.Num() - 1) {
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[0]);
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
					else {
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
						UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
			else if (BattleMenuWidget->IsChoosingItem && !BattleMenuWidget->IsPreparingToAttack && InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren().Num() > 0) {
				TArray<UWidget*> InventoryScrollBoxChildrens = InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren();
				if (InventoryScrollBoxChildrens.Num() > 1) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == InventoryScrollBoxChildrens.Num() - 1) {
						UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[0]);
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
						UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
						InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
						InventoryMenuWidget->GetInventoryScrollBox()->ScrollToStart();
					}
					else {
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
						UIManagerWorldSubsystem->PickedButtonIndex++;
						UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex]);
						UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
						InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
			else if (BattleMenuWidget->IsPreparingToAttack || BattleMenuWidget->IsPreparingToTalk || BattleMenuWidget->IsPreparingToViewInfo) {
				TArray<UWidget*> AttackMenuVerticalBoxChildrens = BattleMenuWidget->GetAttackMenuVerticalBox()->GetAllChildren();
				if (AttackMenuVerticalBoxChildrens.Num() > 0) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == AttackMenuVerticalBoxChildrens.Num() - 1) {
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex]);
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
					else {
						if (IsValid(UIManagerWorldSubsystem->PickedButton))
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButtonIndex += 1;
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex]);
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
		}
		else if (IsValid(PlayerMenuWidget) && PlayerMenuWidget->IsInViewport()) {
			TArray<UWidget*> PlayerMenuButtons{};
			PlayerMenuButtons.Add(PlayerMenuWidget->GetInventoryButton());
			PlayerMenuButtons.Add(PlayerMenuWidget->GetCloseMenuButton());
			if (UIManagerWorldSubsystem->PickedButtonIndex == PlayerMenuButtons.Num() - 1) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[0]);
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
		else if (IsValid(PauseMenuWidget) && PauseMenuWidget->IsInViewport()) {
			TArray<UWidget*> PauseMenuButtons = PauseMenuWidget->GetButtonsStackBox()->GetAllChildren();
			if (UIManagerWorldSubsystem->PickedButtonIndex == PauseMenuButtons.Num() - 1) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[0]);
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
		else if (IsValid(MainMenuWidget) && MainMenuWidget->IsInViewport()) {
			TArray<UWidget*> MainMenuButtons = MainMenuWidget->GetMainVerticalBox()->GetAllChildren();
			if (UIManagerWorldSubsystem->PickedButtonIndex == MainMenuButtons.Num() - 1) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::FromHex("8C8C8CFF"));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MainMenuButtons[0]);
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::FromHex("8C8C8CFF"));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MainMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
		else if (IsValid(SpellBattleMenuWidget) && SpellBattleMenuWidget->IsInViewport() && SpellBattleMenuWidget->CanUseKeyboardButtonSelection) {
			TArray<UWidget*> SpellBattleMenuElementsButtons{};
			if (SpellBattleMenuWidget->GetSelectedSpellRange() == ESpellRange::NONE) {
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetNeighborsSpellRangeButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetEveryoneSpellRangeButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetSingleSpellRangeButton());
			}
			else if (SpellBattleMenuWidget->GetSelectedSpellElements().Num() >= 0) {
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetWaterElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetEarthElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetDarkElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetLightningElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetHolyElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetWindElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetFireElementButton());
				SpellBattleMenuElementsButtons.Add(SpellBattleMenuWidget->GetBloodElementButton());
			}
			if (UIManagerWorldSubsystem->PickedButtonIndex == SpellBattleMenuElementsButtons.Num() - 1) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SpellBattleMenuElementsButtons[0]);
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
			}
			else {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SpellBattleMenuElementsButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
			}
		}
		else if (IsValid(LearnedSpellsJournalMenuWidget) && LearnedSpellsJournalMenuWidget->IsInViewport() && LearnedSpellsJournalMenuWidget->CanUseKeyboardButtonSelection) {
			TArray<UWidget*> LearnedSpellsJournalMenuButtons = LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren();
			if (LearnedSpellsJournalMenuButtons.Num() > 1) {
				if (UIManagerWorldSubsystem->PickedButtonIndex == LearnedSpellsJournalMenuButtons.Num() - 1) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 03, 0.8f));
					UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(LearnedSpellsJournalMenuButtons[0])->GetMainButton();
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				}
				else {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.8f));
					UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(LearnedSpellsJournalMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1])->GetMainButton();
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 0.8f));
				}
			}
		}
		else if (IsValid(SaveLoadGameMenuWidget) && SaveLoadGameMenuWidget->IsInViewport()) {
			TArray<UWidget*> SaveLoadGameMenuWidgetSlotButtons = SaveLoadGameMenuWidget->GetSaveSlotsScrollBox()->GetAllChildren();
			if (SaveLoadGameMenuWidgetSlotButtons.Num() > 1) {
				if (UIManagerWorldSubsystem->PickedButtonIndex == SaveLoadGameMenuWidgetSlotButtons.Num() - 1) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
					UIManagerWorldSubsystem->PickedButton = Cast <USaveSlotEntry>(SaveLoadGameMenuWidgetSlotButtons[0])->GetSlotButton();
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0, 0, 0.8));
				}
				else {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
					UIManagerWorldSubsystem->PickedButton = Cast<USaveSlotEntry>(SaveLoadGameMenuWidgetSlotButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1])->GetSlotButton();
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
				}
			}
		}
		else if (IsValid(SkillBattleMenuWidget) && SkillBattleMenuWidget->IsInViewport() && SkillBattleMenuWidget->CanUseKeyboardButtonSelection) {
			TArray<UWidget*> SkillBattleMenuButtons = SkillBattleMenuWidget->GetSkillsScrollBox()->GetAllChildren();
			if (SkillBattleMenuButtons.Num() > 1) {
				if (UIManagerWorldSubsystem->PickedButtonIndex == SkillBattleMenuButtons.Num() - 1) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
					UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(SkillBattleMenuButtons[0])->GetMainButton();
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
				}
				else {
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
					UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(SkillBattleMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1])->GetMainButton();
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
				}
			}
		}
		else if (IsValid(InventoryMenuWidget) && IsValid(InventoryMenuWidget->SelectedPanelWidget) && InventoryMenuWidget->IsInViewport()) {
			TArray<UWidget*> SelectedScrollBoxChildren = InventoryMenuWidget->SelectedPanelWidget->GetAllChildren();
			if (SelectedScrollBoxChildren.Num() > 0) {
				if (UIManagerWorldSubsystem->PickedButtonIndex == SelectedScrollBoxChildren.Num() - 1) {
					if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
						if (InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible && !InventoryMenuWidget->IsSelectingSpecificItem)
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
						else if (InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible && InventoryMenuWidget->IsSelectingSpecificItem)
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
						else
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
					}
					if (InventoryMenuWidget->IsSelectingSpecificItem) {
						if (UInventoryScrollBoxEntryWidget* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[0]); IsValid(SelectedEntryWidget)) {
							UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
							InventoryMenuWidget->SetPickedItem(SelectedEntryWidget->GetItem());
						}
					}
					else
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[0]);
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					InventoryMenuWidget->CheckForButtonsBordersVisibilityAndSetGreenColorToButton();
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				else {
					if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
						if (InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible && !InventoryMenuWidget->IsSelectingSpecificItem)
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
						else if (InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible && InventoryMenuWidget->IsSelectingSpecificItem)
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
						else
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
					}
					if (InventoryMenuWidget->IsSelectingSpecificItem) {
						if (UInventoryScrollBoxEntryWidget* SelectedEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex + 1]); IsValid(SelectedEntryWidget)) {
							UIManagerWorldSubsystem->PickedButton = SelectedEntryWidget->GetMainButton();
							InventoryMenuWidget->SetPickedItem(SelectedEntryWidget->GetItem());
						}
					}
					else {
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
						InventoryMenuWidget->GetInventoryButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetMeleeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetRangeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetHeadButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetTorseButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetHandButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						InventoryMenuWidget->GetLowerArmorButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
						Cast<UButton>(InventoryMenuWidget->GetItemTypeStackBox()->GetAllChildren()[InventoryMenuWidget->SelectedTypeButtonIndex])->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
						InventoryMenuWidget->CheckForButtonsBordersVisibilityAndSetGreenColorToButton();
					}
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
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
			NotificationActions(ActorResult);
		}
	}
	if (IsValid(UIManagerWorldSubsystem) && IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->OnClicked.Broadcast();
}

void APlayerCharacter::InputBack()
{
	FString MapName = GetWorld()->GetMapName();
	if (IsValid(Controller) && IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(InventoryMenuWidget) && InventoryMenuWidget->IsInViewport() && InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible) {
			InventoryMenuWidget->BattleMenuItemsBackButtonOnClicked();
		}
		else if (IsValid(InventoryMenuWidget) && InventoryMenuWidget->IsInViewport() && InventoryMenuWidget->IsSelectingSpecificItem) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(InventoryMenuWidget->GetItemTypeStackBox()->GetAllChildren()[InventoryMenuWidget->SelectedTypeButtonIndex]);
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			UIManagerWorldSubsystem->PickedButtonIndex = InventoryMenuWidget->SelectedTypeButtonIndex;
			InventoryMenuWidget->IsSelectingSpecificItem = false;
			InventoryMenuWidget->SelectedPanelWidget = InventoryMenuWidget->GetItemTypeStackBox();
		}
		else if (IsValid(InventoryMenuWidget) && InventoryMenuWidget->IsInViewport() && !InventoryMenuWidget->IsSelectingSpecificItem) {
			InventoryMenuWidget->GetInventoryBorder()->SetVisibility(ESlateVisibility::Visible);
			InventoryMenuWidget->GetLowerArmorInventoryBorder()->SetVisibility(ESlateVisibility::Hidden);
			InventoryMenuWidget->GetHandInventoryBorder()->SetVisibility(ESlateVisibility::Hidden);
			InventoryMenuWidget->GetTorseInventoryBorder()->SetVisibility(ESlateVisibility::Hidden);
			InventoryMenuWidget->GetHeadInventoryBorder()->SetVisibility(ESlateVisibility::Hidden);
			InventoryMenuWidget->GetRangeInventoryBorder()->SetVisibility(ESlateVisibility::Hidden);
			InventoryMenuWidget->GetMeleeInventoryBorder()->SetVisibility(ESlateVisibility::Hidden);
			InventoryMenuWidget->HideNotification();
			if (IsValid(Cast<UButton>(InventoryMenuWidget->GetItemTypeStackBox()->GetAllChildren()[InventoryMenuWidget->SelectedTypeButtonIndex])))
				Cast<UButton>(InventoryMenuWidget->GetItemTypeStackBox()->GetAllChildren()[InventoryMenuWidget->SelectedTypeButtonIndex])->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
			if (IsValid(UIManagerWorldSubsystem)) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
				InventoryMenuWidget->SetPickedItem(nullptr);
				InventoryMenuWidget->RemoveFromParent();
				if (InventoryMenuWidget->GetNotInBattleMenuIncludedCanvasPanel()->GetVisibility() == ESlateVisibility::Visible) {
					PlayerMenuWidget->AddToViewport();
					UIManagerWorldSubsystem->PickedButton = PlayerMenuWidget->GetInventoryButton();
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				else if (InventoryMenuWidget->GetNotInBattleMenuIncludedCanvasPanel()->GetVisibility() == ESlateVisibility::Hidden) {
					BattleMenuWidget->AddToViewport();
					UIManagerWorldSubsystem->PickedButton = BattleMenuWidget->GetAttackButton();
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
		}
		else if (PlayerMenuWidget->IsInViewport()) {
			PlayerMenuWidget->RemoveFromParent();
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
		else if (IsValid(BattleMenuWidget) && BattleMenuWidget->IsInViewport() && BattleMenuWidget->GetAttackMenuBorder()->GetVisibility() == ESlateVisibility::Visible) {
			BattleMenuWidget->AttackMenuBackButtonOnClicked();
		}
		else if (PauseMenuWidget->IsInViewport()) {
			PauseMenuWidget->RemoveFromParent();
			SettingsMenuWidget->RemoveFromParent();
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
		else if (SpellBattleMenuWidget->IsInViewport() && !LearnedSpellsJournalMenuWidget->IsInViewport()) {
			if (!SpellInfoWidget->IsInViewport())
				SpellBattleMenuWidget->BackButtonOnClicked();
			else
				SpellBattleMenuWidget->BackToSpellCreationButtonOnClicked();
		}
		else if (LearnedSpellsJournalMenuWidget->IsInViewport()) {
			LearnedSpellsJournalMenuWidget->BackButtonOnClicked();
		}
		else if (SkillBattleMenuWidget->IsInViewport()) {
			SkillBattleMenuWidget->BackButtonWithNeighborsOnClicked();
		}
		else if (CombatCharacterInfoMenuWidget->IsInViewport()) {
			CombatCharacterInfoMenuWidget->BackButtonOnClicked();
		}
		else if (SaveLoadGameMenuWidget->IsInViewport()) {
			SaveLoadGameMenuWidget->BackButtonWithNeighborsOnClicked();
		}
		else if (SettingsMenuWidget->IsInViewport() && MapName != "UEDPIE_0_MainMenu") {
			SettingsMenuWidget->RemoveFromParent();
			PauseMenuWidget->AddToViewport();
			PauseMenuWidget->GetResumeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			UIManagerWorldSubsystem->PickedButton = PauseMenuWidget->GetResumeButton();
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
		}
		if (SettingsMenuWidget->IsInViewport() && MapName == "UEDPIE_0_MainMenu") {
			SettingsMenuWidget->RemoveFromParent();
			MainMenuWidget->AddToViewport();
			MainMenuWidget->GetNewGameButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			UIManagerWorldSubsystem->PickedButton = MainMenuWidget->GetNewGameButton();
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
		}
	}
}

void APlayerCharacter::ChangeLevel(const AActor* const ActorResult)
{
	//If ray result isn't null and is level change trigger,loading screen is set, then load level and save a data into the GameInstance. 
	if (IsValid(LoadingScreenClass) && IsValid(PlayerController) && IsValid(RedemptionGameInstance))
		if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToDungeon"))) || ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToTown")))) {
			UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
			ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(PlayerController, LoadingScreenClass);
			LoadingScreen->AddToViewport();
			if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToDungeon"))))
				UGameplayStatics::OpenLevel(GetWorld(), "Dungeon");
			else if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToTown"))))
				UGameplayStatics::OpenLevel(GetWorld(), "Town");
			//Save PlayerCharacter into the GameInstance.
			{
				RedemptionGameInstance->PlayerCharacterInstanceDataStruct.PlayerTransform.SetLocation(FVector(-1488.0, 1488.0, -1488.0));
				FMemoryWriter MemWriter(RedemptionGameInstance->PlayerCharacterInstanceDataStruct.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
				Ar.ArIsSaveGame = true;
				Serialize(Ar);
			}
			//Save level actors into the GameInstance.
			if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Town")
				RedemptionGameInstance->TownActors.Empty();
			else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Dungeon")
				RedemptionGameInstance->DungeonActors.Empty();
			for (FActorIterator It(GetWorld()); It; ++It)
			{
				AActor* Actor = *It;
				if (!IsValid(Actor) || !Actor->Implements<USavableObjectInterface>() || IsValid(Cast<APlayerCharacter>(Actor)))
				{
					continue;
				}
				if (Actor->Implements<USavableObjectInterface>()) {
					FActorGameInstanceData ActorGameInstanceData{};
					ActorGameInstanceData.ActorName = Actor->GetFName();
					ActorGameInstanceData.ActorTransform = Actor->GetTransform();
					ActorGameInstanceData.ActorClass = Actor->GetClass();
					FMemoryWriter MemWriter(ActorGameInstanceData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar);
					if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Town")
						RedemptionGameInstance->TownActors.Add(ActorGameInstanceData);
					else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Dungeon")
						RedemptionGameInstance->DungeonActors.Add(ActorGameInstanceData);
				}
			}
		}
}

void APlayerCharacter::NotificationActions(const AActor* const ActorResult)
{
	if (ActorResult->ActorHasTag(FName(TEXT("FinishGame"))) && IsValid(RedemptionGameInstance) && IsValid(NotificationWidget)) {
		if (RedemptionGameInstance->InstanceKilledEnemies < 3) {
			GetWorld()->GetTimerManager().ClearTimer(RemoveNotificationTimerHandle);
			NotificationWidget->AddToViewport(); 
			NotificationWidget->SetNotificationTextBlockText(FText::FromString("You need to kill all enemies before proceeding!"));
			GetWorld()->GetTimerManager().SetTimer(RemoveNotificationTimerHandle, this, &APlayerCharacter::RemoveNotification, 3, false);
		}
		else if(RedemptionGameInstance->InstanceKilledEnemies >= 3 && !GetWorld()->GetTimerManager().IsTimerActive(FinishGameTimerHandle)) {
			NotificationWidget->AddToViewport();
			NotificationWidget->SetNotificationTextBlockText(FText::FromString("Congratulations!!!"));
			GetWorld()->GetTimerManager().SetTimer(FinishGameTimerHandle, this, &APlayerCharacter::FinishGame, 3, false);
		}
	}
}

void APlayerCharacter::PickUpItem(AActor* const ActorResult)
{
		if (ActorResult->ActorHasTag(FName(TEXT("Loot")))) {
			ALootInTheWorld* LootInTheWorld = Cast<ALootInTheWorld>(ActorResult);
			bool IsInInventory = false;
			if (IsValid(LootInTheWorld)) {
				for (uint8 i = 0; i < LootInTheWorld->GetItemsClasses().Num(); i++) {
					AGameItem* GameItem = NewObject<AGameItem>(this, LootInTheWorld->GetItemsClasses()[i]);
					RedemptionGameInstance->InstanceItemsInTheInventory.Add(LootInTheWorld->GetItemsClasses()[i]);
					if (IsValid(GameItem)) {
						//Get ScrollBox corresponding to the item's type
						UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(InventoryMenuWidget, GameItem);
						//Check if this item is already in the inventory. If yes, than just add to AmountOfItems and change text, if not, then add new inventory widget
						InventoryActions::IfItemAlreadyIsInInventory(GetWorld(), CurrentScrollBox, GameItem);
					}
				}
				//Destroy container with loot
				ActorResult->Destroy();
			}
		}
}

void APlayerCharacter::DialogueInteract(const AActor* const ActorResult)
{
	//If hit result from forward vector is NPC with a dialogue, start the dialogue 
	if (IsValid(ActorResult))
		if (ActorResult->GetClass()->ImplementsInterface(UDialogueActionsInterface::StaticClass())) {
			if(ATownNPC* TownNPC = const_cast<ATownNPC*>(Cast<ATownNPC>(ActorResult)); IsValid(TownNPC))
				TownNPC->Execute_StartADialogue(TownNPC);
			//if(IsValid(PlayerController))
			//	PlayerController->ActivateTouchInterface(EmptyTouchInterface);
		}
}

void APlayerCharacter::CheckForwardRayHitResult()
{
	//If hit result from forward vector is NPC or interactive object, add its name to the viewport
	AActor* ActorResult = ForwardRay().GetActor();
	if (IsValid(ActorResult) && IsValid(ForwardRayInfoWidget)) {
		if (ActorResult->ActorHasTag(FName(TEXT("NPC")))) {
			if (!ForwardRayInfoWidget->IsInViewport()) {
				ForwardRayInfoWidget->AddToViewport(0);
				ForwardRayInfoWidget->SetMainTextBlockText(FText::FromName(Cast<ACharacterInTheWorld>(ActorResult)->GetCharacterName()));
			}
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToDungeon")))) {
			if (!ForwardRayInfoWidget->IsInViewport()) {
				ForwardRayInfoWidget->AddToViewport(0);
				ForwardRayInfoWidget->SetMainTextBlockText(FText::FromString("Enter the dungeon"));
			}
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToTown")))) {
			if (!ForwardRayInfoWidget->IsInViewport()) {
				ForwardRayInfoWidget->AddToViewport(0);
				ForwardRayInfoWidget->SetMainTextBlockText(FText::FromString("Enter the town"));
			}
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("Loot")))) {
			ALootInTheWorld* LootInTheWorld = Cast<ALootInTheWorld>(ActorResult);
			if (IsValid(LootInTheWorld))
				if (!ForwardRayInfoWidget->IsInViewport()) {
					ForwardRayInfoWidget->AddToViewport(0);
					ForwardRayInfoWidget->SetMainTextBlockText(FText::FromName(LootInTheWorld->GetName()));
			}
		}
		//Remove from viewport, if ray didn't hit NPC or interactive object
		else if(ForwardRayInfoWidget->IsInViewport())
				ForwardRayInfoWidget->RemoveFromParent();	
	}
	//Remove from viewport, if ray hit nothing
	else if(!IsValid(ActorResult) && IsValid(ForwardRayInfoWidget)) {
		if(ForwardRayInfoWidget->IsInViewport())
		ForwardRayInfoWidget->RemoveFromParent();
	}
}

void APlayerCharacter::LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance)
{
	if(GameInstance->PlayerCharacterInstanceDataStruct.PlayerTransform.GetLocation() != FVector(0.0, 0.0, 0.0) && 
		GameInstance->PlayerCharacterInstanceDataStruct.PlayerTransform.GetLocation() != FVector(-1488.0, 1488.0, -1488.0))
			SetActorTransform(GameInstance->PlayerCharacterInstanceDataStruct.PlayerTransform);
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

void APlayerCharacter::Death()
{

}

void APlayerCharacter::RemoveNotification()
{
	if(IsValid(NotificationWidget))
		NotificationWidget->RemoveFromParent();
}

void APlayerCharacter::FinishGame()
{
	if (IsValid(PlayerController) && IsValid(LoadingScreenClass)) {
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(PlayerController, LoadingScreenClass);
		if(IsValid(LoadingScreen))
			LoadingScreen->AddToViewport();
		UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	}
}

UInventoryScrollBoxEntryWidget* APlayerCharacter::GetInventoryScrollBoxEntryWidget() const
{
	return InventoryScrollBoxEntryWidget;
}

void APlayerCharacter::RestartBattleMenuWidget()
{
	if (IsValid(BattleMenuClass) && IsValid(PlayerController))
			BattleMenuWidget = CreateWidget<UBattleMenu>(PlayerController, BattleMenuClass);
}

void APlayerCharacter::RestartBattleResultsScreenWidget()
{
	if (IsValid(BattleResultsScreenClass) && IsValid(PlayerController))
			BattleResultsScreenWidget = CreateWidget<UBattleResultsScreen>(PlayerController, BattleResultsScreenClass);
}

UForwardRayInfo* APlayerCharacter::GetForwardRayInfoWidget() const
{
	return ForwardRayInfoWidget;
}

UPauseMenu* APlayerCharacter::GetPauseMenuWidget() const
{
	return PauseMenuWidget;
}

UMainMenu* APlayerCharacter::GetMainMenuWidget() const
{
	return MainMenuWidget;
}

UCombatCharacterInfoMenu* APlayerCharacter::GetCombatCharacterInfoMenuWidget() const
{
	return CombatCharacterInfoMenuWidget;
}

USaveLoadGameMenu* APlayerCharacter::GetSaveLoadGameMenuWidget() const
{
	return SaveLoadGameMenuWidget;
}

UDialogueBox* APlayerCharacter::GetDialogueBoxWidget() const
{
	return DialogueBoxWidget;
}

UPlayerMenu* APlayerCharacter::GetPlayerMenuWidget() const
{
	return PlayerMenuWidget;
}

UInventoryMenu* APlayerCharacter::GetInventoryMenuWidget() const
{
	return InventoryMenuWidget;
}

UBattleResultsScreen* APlayerCharacter::GetBattleResultsScreenWidget() const
{
	return BattleResultsScreenWidget;
}

TSubclassOf<class UInventoryScrollBoxEntryWidget> APlayerCharacter::GetInventoryScrollBoxEntryClass() const
{
	return InventoryScrollBoxEntryClass;
}

TSubclassOf<class UResponseEntry> APlayerCharacter::GetResponseEntryClass() const
{
	return ResponseEntryClass;
}

bool APlayerCharacter::GetCanInput() const
{
	return CanOpenOtherMenus;
}

UBattleMenu* APlayerCharacter::GetBattleMenuWidget() const
{
	return BattleMenuWidget;
}

ULearnedSpellsJournalMenu* APlayerCharacter::GetLearnedSpellsJournalMenu() const
{
	return LearnedSpellsJournalMenuWidget;
}

ABattleManager* APlayerCharacter::GetBattleManager() const
{
	return BattleManager;
}

UUIManagerWorldSubsystem* APlayerCharacter::GetUIManagerWorldSubsystem() const
{
	return UIManagerWorldSubsystem;
}

AGameManager* APlayerCharacter::GetGameManager() const
{
	return GameManager;
}

AAudioManager* APlayerCharacter::GetAudioManager() const
{
	return AudioManager;
}

AParticlesManager* APlayerCharacter::GetParticlesManager() const
{
	return ParticlesManager;
}

UDeathMenu* APlayerCharacter::GetDeathMenuWidget() const
{
	return DeathMenuWidget;
}

USettingsMenu* APlayerCharacter::GetSettingsMenuWidget() const
{
	return SettingsMenuWidget;
}

USkillBattleMenu* APlayerCharacter::GetSkillBattleMenuWidget() const
{
	return SkillBattleMenuWidget;
}

class USpellBattleMenu* APlayerCharacter::GetSpellBattleMenuWidget() const
{
	return SpellBattleMenuWidget;
}

USpellInfo* APlayerCharacter::GetSpellInfoWidget() const
{
	return SpellInfoWidget;
}

class AEffectsSpellsAndSkillsManager* APlayerCharacter::GetEffectsSpellsAndSkillsManager() const
{
	return EffectsManager;
}

URedemptionGameInstance* APlayerCharacter::GetGameInstance() const
{
	return RedemptionGameInstance;
}

UResponsesBox* APlayerCharacter::GetResponsesBox() const
{
	return ResponsesBoxWidget;
}

UTouchInterface* APlayerCharacter::GetEmptyTouchInterface() const
{
	return EmptyTouchInterface;
}

UTouchInterface* APlayerCharacter::GetStandardTouchInterface() const
{
	return StandardTouchInterface;
}

const TArray<TSubclassOf<ASpell>> APlayerCharacter::GetAvailableSkills() const
{
	return AvailableSkills;
}

TArray<TSubclassOf<ACombatAllies>> APlayerCharacter::GetAllies()
{
	return Allies;
}

class UAlliesInfoBars* APlayerCharacter::GetAlliesInfoBarsWidget() const
{
	return AlliesInfoBarsWidget;
}

void APlayerCharacter::SetInventoryScrollBoxEntryWidget(const UInventoryScrollBoxEntryWidget* const NewWidget) 
{
	InventoryScrollBoxEntryWidget = const_cast<UInventoryScrollBoxEntryWidget*>(NewWidget);
}

void APlayerCharacter::SetCanInput(bool Value)
{
	CanOpenOtherMenus = Value;
}

void APlayerCharacter::SetGameManager(const AGameManager* const NewGameManager)
{
	GameManager = const_cast<AGameManager*>(NewGameManager);
}

void APlayerCharacter::SetBattleManager(const ABattleManager* const NewBattleManager)
{
	BattleManager = const_cast<ABattleManager*>(NewBattleManager);
}

void APlayerCharacter::SetAudioManager(const AAudioManager* const NewAudioManager)
{
	AudioManager = const_cast<AAudioManager*>(NewAudioManager);
}

void APlayerCharacter::SetAllies(const TArray<TSubclassOf<ACombatAllies>>& NewAllies)
{
}
