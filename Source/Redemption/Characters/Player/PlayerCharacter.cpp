// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\LoadingScreen.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\LootInTheWorld.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Interfaces\DialogueActionsInterface.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\NonCombat\TownNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\WeaponItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\ArmorItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ElementsActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatPlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "GameFramework/TouchInterface.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Components/StackBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\InventoryActions.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include <Blueprint/WidgetBlueprintLibrary.h>


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
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	TArray<AActor*> BattleManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleManager::StaticClass(), BattleManagerActors);
	TArray<AActor*> GameManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), GameManagerActors);
	TArray<AActor*> AudioManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAudioManager::StaticClass(), AudioManagerActors);
	TArray<AActor*> EffectsManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEffectsSpellsAndSkillsManager::StaticClass(), EffectsManagerActors);
	if (AudioManagerActors.Num() > 0)
		AudioManager = Cast<AAudioManager>(AudioManagerActors[0]);
	if (BattleManagerActors.Num() > 0)
		BattleManager = Cast<ABattleManager>(BattleManagerActors[0]);
	if (GameManagerActors.Num() > 0)
		GameManager = Cast<AGameManager>(GameManagerActors[0]);
	if (EffectsManagerActors.Num() > 0)
		EffectsManager = Cast<AEffectsSpellsAndSkillsManager>(EffectsManagerActors[0]);

	PlayerController = GetWorld()->GetFirstPlayerController();
	if(IsValid(PlayerController))
		PlayerSkeletalMesh = PlayerController->GetPawn()->FindComponentByClass<USkeletalMeshComponent>();
	if (IsValid(PlayerSkeletalMesh))
		PlayerAnimInstance = Cast<UPlayerCharacterAnimInstance>(PlayerSkeletalMesh->GetAnimInstance());
	GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	//Creating UI widgets...
	if (IsValid(PlayerController)) {
		SkillEntryWidget = CreateWidget<USkillEntryWidget>(PlayerController, SkillEntryClass);
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
	}
	//Level change logic
	if (IsValid(GameInstance)) {
		CurrentHP = GameInstance->InstancePlayerCurrentHP;
		CurrentMana = GameInstance->InstancePlayerCurrentMana;
		MaxHP = GameInstance->InstancePlayerMaxHP;
		MaxMana = GameInstance->InstancePlayerMaxMana;
		Strength = GameInstance->InstancePlayerStrength;
		Perception = GameInstance->InstancePlayerPerception;
		Endurance = GameInstance->InstancePlayerEndurance;
		Charisma = GameInstance->InstancePlayerCharisma;
		Intelligence = GameInstance->InstancePlayerIntelligence;
		Agility = GameInstance->InstancePlayerAgility;
		Luck = GameInstance->InstancePlayerLuck;
		Allies = GameInstance->InstanceAllies;
	}
	if (IsValid(GetWorld()))
		UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
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

void APlayerCharacter::InputOpenPlayerMenu()
{
	if (!PlayerMenuWidget->IsInViewport() && CanOpenOtherMenus) {
		PlayerMenuWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->SetPause(true);
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
	if (!PauseMenuWidget->IsInViewport() && CanOpenOtherMenus) {
		PauseMenuWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->SetPause(true);
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
	else if (SettingsMenuWidget->IsInViewport()) {
		SettingsMenuWidget->RemoveFromParent();
		PauseMenuWidget->AddToViewport();
		PauseMenuWidget->GetResumeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButton = PauseMenuWidget->GetResumeButton();
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void APlayerCharacter::InputBack()
{

}

void APlayerCharacter::InputScrollRight()
{
	if (IsValid(Controller)) 
		if (BattleMenuWidget->IsPreparingToAttack || BattleMenuWidget->IsPreparingToTalk)
			if (BattleManager->IsSelectingAllyAsTarget) {
				TArray<ACombatNPC*> BattleAlliesPlayer = BattleManager->BattleAlliesPlayer;

				//Choose Target with scroll
				if (BattleAlliesPlayer.Num() > 1) {
					int8 CurrentIndex = BattleManager->SelectedCombatNPCIndex;
					int8 StartIndex = BattleManager->SelectedCombatNPCIndex;
					while (true) {
						if (CurrentIndex + 1 < BattleAlliesPlayer.Num()) {
							if (BattleAlliesPlayer[CurrentIndex + 1]->GetCurrentHP() > 0) {
								BattleManager->SelectNewTarget(BattleAlliesPlayer[CurrentIndex + 1], CurrentIndex + 1);
								break;
							}
							else {
								CurrentIndex++;
								if (CurrentIndex == StartIndex)
									break;
							}
						}
						else {
							if (BattleAlliesPlayer[0]->GetCurrentHP() > 0) {
								BattleManager->SelectNewTarget(BattleAlliesPlayer[0], 0);
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
			else {
				TArray<ACombatEnemyNPC*> BattleEnemies = BattleManager->BattleEnemies;
				//Choose Target with scroll
				if (BattleEnemies.Num() > 1) {
					int8 CurrentIndex = BattleManager->SelectedCombatNPCIndex;
					int8 StartIndex = BattleManager->SelectedCombatNPCIndex;
					while (true) {
						if (CurrentIndex + 1 < BattleEnemies.Num()) {
							if (BattleEnemies[CurrentIndex + 1]->GetCurrentHP() > 0) {
								BattleManager->SelectNewTarget(BattleEnemies[CurrentIndex + 1], CurrentIndex + 1);
								break;
							}
							else {
								CurrentIndex++;
								if (CurrentIndex == StartIndex)
									break;
							}
						}
						else {
							if (BattleEnemies[0]->GetCurrentHP() > 0) {
								BattleManager->SelectNewTarget(BattleEnemies[0], 0);
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
}

void APlayerCharacter::InputScrollLeft()
{
	if (IsValid(Controller))
		if (BattleMenuWidget->IsPreparingToAttack || BattleMenuWidget->IsPreparingToTalk)
			if (BattleManager->IsSelectingAllyAsTarget) {
				TArray<ACombatNPC*> BattleAlliesPlayer = BattleManager->BattleAlliesPlayer;
				//Choose target with scroll
				int8 CurrentIndex = BattleManager->SelectedCombatNPCIndex;
				int8 StartIndex = BattleManager->SelectedCombatNPCIndex;
				while (true) {
					if (BattleAlliesPlayer.Num() > 1) {
						if (CurrentIndex - 1 >= 0) {
							if (BattleAlliesPlayer[CurrentIndex - 1]->GetCurrentHP() > 0) {
								BattleManager->SelectNewTarget(BattleAlliesPlayer[CurrentIndex - 1], CurrentIndex - 1);
								break;
							}
							else {
								CurrentIndex--;
								if (CurrentIndex == StartIndex)
									break;
							}
						}
						else {
							if (BattleAlliesPlayer[BattleAlliesPlayer.Num() - 1]->GetCurrentHP() > 0) {
								BattleManager->SelectNewTarget(BattleAlliesPlayer[BattleAlliesPlayer.Num() - 1], BattleAlliesPlayer.Num() - 1);
								break;
							}
							else {
								CurrentIndex = BattleAlliesPlayer.Num() - 1;
								if (CurrentIndex == StartIndex)
									break;
							}
						}
					}
				}
			}
			else {
				TArray<ACombatEnemyNPC*> BattleEnemies = BattleManager->BattleEnemies;
				//Choose target with scroll
				int8 CurrentIndex = BattleManager->SelectedCombatNPCIndex;
				int8 StartIndex = BattleManager->SelectedCombatNPCIndex;
				while (true) {
					if (BattleEnemies.Num() > 1) {
						if (CurrentIndex - 1 >= 0) {
							if (BattleEnemies[CurrentIndex - 1]->GetCurrentHP() > 0) {
								BattleManager->SelectNewTarget(BattleEnemies[CurrentIndex - 1], CurrentIndex - 1);
								break;
							}
							else {
								CurrentIndex--;
								if (CurrentIndex == StartIndex)
									break;
							}
						}
						else {
							if (BattleEnemies[BattleEnemies.Num() - 1]->GetCurrentHP() > 0) {
								BattleManager->SelectNewTarget(BattleEnemies[BattleEnemies.Num() - 1], BattleEnemies.Num() - 1);
								break;
							}
							else {
								CurrentIndex = BattleEnemies.Num() - 1;
								if (CurrentIndex == StartIndex)
									break;
							}
						}
					}
				}
			}
}

void APlayerCharacter::InputScrollUp()
{
	if (IsValid(Controller) && IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(BattleMenuWidget) && BattleMenuWidget->IsInViewport()) {
			if (BattleMenuWidget->IsChoosingAction) {
				TArray<UWidget*> MenuVerticalBoxChildrens = BattleMenuWidget->GetMenuVerticalBox()->GetAllChildren();
				if (MenuVerticalBoxChildrens.Num() > 0) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[MenuVerticalBoxChildrens.Num() - 1]);
						UIManagerWorldSubsystem->PickedButtonIndex = MenuVerticalBoxChildrens.Num() - 1;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
					else {
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
						UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
			else if (BattleMenuWidget->IsChoosingItem && !BattleMenuWidget->IsPreparingToAttack) {
				TArray<UWidget*> InventoryScrollBoxChildrens = InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren();
				if (InventoryScrollBoxChildrens.Num() > 0) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
						UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[InventoryScrollBoxChildrens.Num() - 1]);
						InventoryMenuWidget->GetInventoryScrollBox()->ScrollToEnd();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
						UIManagerWorldSubsystem->PickedButtonIndex = InventoryScrollBoxChildrens.Num() - 1;
						UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
						InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
					else {
						UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
						UIManagerWorldSubsystem->PickedButtonIndex--;
						UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
						InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
			else if (BattleMenuWidget->IsPreparingToAttack) {
				TArray<UWidget*> AttackMenuVerticalBoxChildrens = BattleMenuWidget->GetAttackMenuVerticalBox()->GetAllChildren();
				if (AttackMenuVerticalBoxChildrens.Num() > 0) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[AttackMenuVerticalBoxChildrens.Num() - 1]);
						UIManagerWorldSubsystem->PickedButtonIndex = AttackMenuVerticalBoxChildrens.Num() - 1;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
					else {
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
						UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
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
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[PlayerMenuButtons.Num() - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = PlayerMenuButtons.Num() - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
		else if (IsValid(PauseMenuWidget) && PauseMenuWidget->IsInViewport()) {
			TArray<UWidget*> PauseMenuButtons{};
			PauseMenuButtons.Add(PauseMenuWidget->GetResumeButton());
			PauseMenuButtons.Add(PauseMenuWidget->GetLoadButton());
			PauseMenuButtons.Add(PauseMenuWidget->GetSettingsButton());
			PauseMenuButtons.Add(PauseMenuWidget->GetMainMenuButton());
			if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[PauseMenuButtons.Num() - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = PauseMenuButtons.Num() - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
		else if (IsValid(InventoryMenuWidget) && IsValid(InventoryMenuWidget->SelectedPanelWidget) && InventoryMenuWidget->IsInViewport()) {
			TArray<UWidget*> SelectedScrollBoxChildren = InventoryMenuWidget->SelectedPanelWidget->GetAllChildren();
			if (SelectedScrollBoxChildren.Num() > 0) {
				if (UIManagerWorldSubsystem->PickedButtonIndex == 0) {
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
					if (InventoryMenuWidget->IsSelectingSpecificItem)
						UIManagerWorldSubsystem->PickedButton = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[SelectedScrollBoxChildren.Num() - 1])->GetMainButton();
					else
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[SelectedScrollBoxChildren.Num() - 1]);
					UIManagerWorldSubsystem->PickedButtonIndex = SelectedScrollBoxChildren.Num() - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				else {
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
					if (InventoryMenuWidget->IsSelectingSpecificItem)
						UIManagerWorldSubsystem->PickedButton = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex - 1])->GetMainButton();
					else
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex - 1]);
					UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex - 1;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
			}
		}
	}
}

void APlayerCharacter::InputScrollDown()
{
	if (IsValid(Controller) && IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(BattleMenuWidget) && BattleMenuWidget->IsInViewport()) {
			if (BattleMenuWidget->IsChoosingAction) {
				TArray<UWidget*> MenuVerticalBoxChildrens = BattleMenuWidget->GetMenuVerticalBox()->GetAllChildren();
				if (MenuVerticalBoxChildrens.Num() > 0) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == MenuVerticalBoxChildrens.Num() - 1) {
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[0]);
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
					else {
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
						UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
			else if (BattleMenuWidget->IsChoosingItem && !BattleMenuWidget->IsPreparingToAttack && InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren().Num() > 0) {
				TArray<UWidget*> InventoryScrollBoxChildrens = InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren();
				if (InventoryScrollBoxChildrens.Num() > 0) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == InventoryScrollBoxChildrens.Num() - 1) {
						UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[0]);
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
						UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
						InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
						InventoryMenuWidget->GetInventoryScrollBox()->ScrollToStart();
					}
					else {
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
						UIManagerWorldSubsystem->PickedButtonIndex++;
						UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex]);
						UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
						InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
			else if (BattleMenuWidget->IsPreparingToAttack) {
				TArray<UWidget*> AttackMenuVerticalBoxChildrens = BattleMenuWidget->GetAttackMenuVerticalBox()->GetAllChildren();
				if (AttackMenuVerticalBoxChildrens.Num() > 0) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == AttackMenuVerticalBoxChildrens.Num() - 1) {
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[0]);
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
					else {
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
						UIManagerWorldSubsystem->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
						UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
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
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[0]);
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PlayerMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
		else if (IsValid(PauseMenuWidget) && PauseMenuWidget->IsInViewport()) {
			TArray<UWidget*> PauseMenuButtons{};
			PauseMenuButtons.Add(PauseMenuWidget->GetResumeButton());
			PauseMenuButtons.Add(PauseMenuWidget->GetLoadButton());
			PauseMenuButtons.Add(PauseMenuWidget->GetSettingsButton());
			PauseMenuButtons.Add(PauseMenuWidget->GetMainMenuButton());
			if (UIManagerWorldSubsystem->PickedButtonIndex == PauseMenuButtons.Num() - 1) {
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[0]);
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(PauseMenuButtons[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
				UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
		else if (IsValid(InventoryMenuWidget) && IsValid(InventoryMenuWidget->SelectedPanelWidget) && InventoryMenuWidget->IsInViewport()) {
			TArray<UWidget*> SelectedScrollBoxChildren = InventoryMenuWidget->SelectedPanelWidget->GetAllChildren();
				if (SelectedScrollBoxChildren.Num() > 0) {
					if (UIManagerWorldSubsystem->PickedButtonIndex == SelectedScrollBoxChildren.Num() - 1) {
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
							if(InventoryMenuWidget->IsSelectingSpecificItem)
								UIManagerWorldSubsystem->PickedButton = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[0])->GetMainButton();
							else
								UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[0]);
							UIManagerWorldSubsystem->PickedButtonIndex = 0;
							if(IsValid(UIManagerWorldSubsystem->PickedButton))
								UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
					else {
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
						UE_LOG(LogTemp, Warning, TEXT("The index value is: %d"), UIManagerWorldSubsystem->PickedButtonIndex);
						UE_LOG(LogTemp, Warning, TEXT("The selected panel widget name is %s"), *InventoryMenuWidget->SelectedPanelWidget->GetName());
						if (InventoryMenuWidget->IsSelectingSpecificItem && IsValid(Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex + 1])))
							UIManagerWorldSubsystem->PickedButton = Cast<UInventoryScrollBoxEntryWidget>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex + 1])->GetMainButton();
						else {
							UIManagerWorldSubsystem->PickedButton = Cast<UButton>(SelectedScrollBoxChildren[UIManagerWorldSubsystem->PickedButtonIndex + 1]);
							InventoryMenuWidget->GetInventoryButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
							InventoryMenuWidget->GetMeleeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
							InventoryMenuWidget->GetRangeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
							InventoryMenuWidget->GetHeadButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
							InventoryMenuWidget->GetTorseButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
							InventoryMenuWidget->GetHandButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
							InventoryMenuWidget->GetLowerArmorButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
							Cast<UButton>(InventoryMenuWidget->GetItemTypeStackBox()->GetAllChildren()[InventoryMenuWidget->SelectedButtonIndex])->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
							if (InventoryMenuWidget->GetInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
								InventoryMenuWidget->GetInventoryButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
							if (InventoryMenuWidget->GetMeleeInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
								InventoryMenuWidget->GetMeleeButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
							if (InventoryMenuWidget->GetRangeInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
								InventoryMenuWidget->GetRangeButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
							if (InventoryMenuWidget->GetHeadInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
								InventoryMenuWidget->GetHeadButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
							if (InventoryMenuWidget->GetTorseInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
								InventoryMenuWidget->GetTorseButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
							if (InventoryMenuWidget->GetHandInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
								InventoryMenuWidget->GetHandButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
							if (InventoryMenuWidget->GetLowerArmorInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
								InventoryMenuWidget->GetLowerArmorButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
						}
						UIManagerWorldSubsystem->PickedButtonIndex = UIManagerWorldSubsystem->PickedButtonIndex + 1;
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

void APlayerCharacter::ChangeLevel(const AActor* const &ActorResult)
{
	//If ray result isn't null and is level change trigger,loading screen is set, then load level 
	if (IsValid(LoadingScreenClass) && IsValid(PlayerController) && IsValid(GameInstance)) {
		if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToDungeon")))) {
			UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
			GameInstance->InstancePlayerCurrentHP = CurrentHP;
			GameInstance->InstancePlayerCurrentMana = CurrentMana;
			ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(PlayerController, LoadingScreenClass);
			LoadingScreen->AddToViewport();
			UGameplayStatics::OpenLevel(GetWorld(), "Dungeon");
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToTown")))) {
			UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
			GameInstance->InstancePlayerCurrentHP = CurrentHP;
			GameInstance->InstancePlayerCurrentMana = CurrentMana;
			ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(PlayerController, LoadingScreenClass);
			LoadingScreen->AddToViewport();
			UGameplayStatics::OpenLevel(GetWorld(), "Town");
		}
	}
}

void APlayerCharacter::NotificationActions(const AActor* const& ActorResult)
{
	if (ActorResult->ActorHasTag(FName(TEXT("FinishGame"))) && IsValid(GameInstance) && IsValid(NotificationWidget)) {
		if (GameInstance->InstanceKilledEnemies < 3) {
			GetWorld()->GetTimerManager().ClearTimer(RemoveNotificationTimerHandle);
			NotificationWidget->AddToViewport(); 
			NotificationWidget->SetNotificationTextBlockText(FText::FromString("You need to kill all enemies before proceeding!"));
			GetWorld()->GetTimerManager().SetTimer(RemoveNotificationTimerHandle, this, &APlayerCharacter::RemoveNotification, 3, false);
		}
		else if(GameInstance->InstanceKilledEnemies >= 3 && !GetWorld()->GetTimerManager().IsTimerActive(FinishGameTimerHandle)) {
			NotificationWidget->AddToViewport();
			NotificationWidget->SetNotificationTextBlockText(FText::FromString("Congratulations!!!"));
			GetWorld()->GetTimerManager().SetTimer(FinishGameTimerHandle, this, &APlayerCharacter::FinishGame, 3, false);
		}
	}
}

void APlayerCharacter::PickUpItem(AActor* const& ActorResult)
{
		if (ActorResult->ActorHasTag(FName(TEXT("Loot")))) {
			ALootInTheWorld* LootInTheWorld = Cast<ALootInTheWorld>(ActorResult);
			bool IsInInventory = false;
			if (IsValid(LootInTheWorld)) {
				for (int i = 0; i < LootInTheWorld->GetItemsClasses().Num(); i++) {
					AGameItem* GameItem = NewObject<AGameItem>(this, LootInTheWorld->GetItemsClasses()[i]);
					GameInstance->InstanceItemsInTheInventory.Add(LootInTheWorld->GetItemsClasses()[i]);
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

void APlayerCharacter::DialogueInteract(AActor* const &ActorResult)
{
	//If hit result from forward vector is NPC with a dialogue, start the dialogue 
	if (IsValid(ActorResult))
		if (ActorResult->GetClass()->ImplementsInterface(UDialogueActionsInterface::StaticClass())) {
			if(ATownNPC* TownNPC = Cast<ATownNPC>(ActorResult); IsValid(TownNPC))
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

UDeathMenu* APlayerCharacter::GetDeathMenuWidget() const
{
	return DeathMenuWidget;
}

USettingsMenu* APlayerCharacter::GetSettingsMenuWidget() const
{
	return SettingsMenuWidget;
}

class USpellBattleMenu* APlayerCharacter::GetSpellBattleMenuWidget() const
{
	return SpellBattleMenuWidget;
}

class AEffectsSpellsAndSkillsManager* APlayerCharacter::GetEffectsSpellsAndSkillsManager() const
{
	return EffectsManager;
}

URedemptionGameInstance* APlayerCharacter::GetGameInstance() const
{
	return GameInstance;
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

TArray<TSubclassOf<ACombatAllies>> APlayerCharacter::GetAllies() const
{
	return Allies;
}

class UAlliesInfoBars* APlayerCharacter::GetAlliesInfoBarsWidget() const
{
	return AlliesInfoBarsWidget;
}

void APlayerCharacter::SetInventoryScrollBoxEntryWidget(UInventoryScrollBoxEntryWidget* NewWidget) 
{
	InventoryScrollBoxEntryWidget = NewWidget;
}

void APlayerCharacter::SetCanInput(bool Value)
{
	CanOpenOtherMenus = Value;
}

void APlayerCharacter::SetGameManager(AGameManager* const &NewGameManager)
{
	GameManager = NewGameManager;
}

void APlayerCharacter::SetBattleManager(ABattleManager* const &NewBattleManager)
{
	BattleManager = NewBattleManager;
}

void APlayerCharacter::SetAudioManager(AAudioManager* const &NewAudioManager)
{
	AudioManager = NewAudioManager;
}
