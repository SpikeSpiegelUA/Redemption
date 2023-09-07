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
#include "GameFramework/TouchInterface.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\InventoryActions.h"
#include "Blueprint/WidgetLayoutLibrary.h"


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
		if (IsValid(SkillBattleMenuClass))
			SkillBattleMenuWidget = CreateWidget<USkillBattleMenu>(PlayerController, SkillBattleMenuClass);
		if (IsValid(LearnedSpellsJournalMenuClass))
			LearnedSpellsJournalMenuWidget = CreateWidget<ULearnedSpellsJournalMenu>(PlayerController, LearnedSpellsJournalMenuClass);
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

	TArray<AActor*> UIManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUIManager::StaticClass(), UIManagerActors);
	if (UIManagerActors.Num() > 0)
		UIManager = Cast<AUIManager>(UIManagerActors[0]);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!IsInDialogue)
	CheckForwardRayHitResult();
	//Need to move player in battle scene, when input is disable. Use Interp in Tick(). Not the best solution, but this's the only one I came up with
	//Move player to enemy in Battle scene
	/*if (Combat_IsMovingToAttackEnemy) {
		FVector DistanceVector;
		if (BattleManager->SelectedActor) {
			DistanceVector = FVector(BattleManager->SelectedActor->GetActorLocation() - this->GetActorLocation());
			FVector NewPosition = FMath::VInterpTo(this->GetActorLocation(), BattleManager->SelectedActor->GetActorLocation(), DeltaTime, 0.6f);
			this->SetActorLocation(NewPosition);
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), BattleManager->SelectedActor->GetActorLocation());
			Rotation = FRotator(this->GetActorRotation().Pitch, Rotation.Yaw, this->GetActorRotation().Roll);
			this->SetActorRotation(Rotation);
			if (IsValid(PlayerAnimInstance))
				PlayerAnimInstance->SetSpeedToActualSpeed = false;
		}
		//If distance between player and selected enemy is less than 160, turn on animation
		if (DistanceVector.Length() <= 160) {
			Combat_IsMovingToAttackEnemy = false;
			if (IsValid(PlayerAnimInstance)) {
				PlayerAnimInstance->SetPlayerIsAttacking(true);
				PlayerAnimInstance->SetSpeedToActualSpeed = true;
			}
		}
	}
	//Move player back to his start position
	else if (Combat_IsMovingToStartPosition) {
		FVector DistanceVector{};
		if (IsValid(Combat_StartLocation)) {
			DistanceVector = FVector(Combat_StartLocation->GetActorLocation() - this->GetActorLocation());
			FVector NewPosition = FMath::VInterpTo(this->GetActorLocation(), Combat_StartLocation->GetActorLocation(), DeltaTime, 0.6f);
			this->SetActorLocation(NewPosition);
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Combat_StartLocation->GetActorLocation());
			Rotation = FRotator(this->GetActorRotation().Pitch, Rotation.Yaw, this->GetActorRotation().Roll);
			this->SetActorRotation(Rotation);
			if (IsValid(PlayerAnimInstance))
				PlayerAnimInstance->SetSpeedToActualSpeed = false;
		}
		//If distance between player and start position is less than 100, stop
		if (DistanceVector.Length() <= 100) {
			BattleMenuWidget->GetCenterMark()->SetVisibility(ESlateVisibility::Hidden);
			Combat_IsMovingToStartPosition = false;
			this->SetActorRotation(FRotator(0, 180, 0));
			if (IsValid(PlayerAnimInstance))
				PlayerAnimInstance->SetSpeedToActualSpeed = true;
			BattleManager->PlayerTurnController();
		}
	}*/
}
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (CanInput) {
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
	if (CanInput) {
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
	if (CanInput)
		Jump();
}

void APlayerCharacter::InputOpenPlayerMenu()
{
	if (CanInput) {
		PlayerMenuWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->ActivateTouchInterface(EmptyTouchInterface);
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->SetPause(true);
	}
}

void APlayerCharacter::InputOpenPauseMenu()
{
	if (CanInput) {
		PauseMenuWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->ActivateTouchInterface(EmptyTouchInterface);
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->SetPause(true);
	}
}

void APlayerCharacter::InputScrollLeft()
{
	if (IsValid(Controller)) 
		if (!Combat_IsMovingToAttackEnemy)
			if (BattleMenuWidget->IsPreparingToAttack)
				if (BattleManager->IsSelectingAllyAsTarget) {
					TArray<ACombatNPC*> BattleAlliesPlayer = BattleManager->BattleAlliesPlayer;
					//Choose Target with scroll
					if (BattleAlliesPlayer.Num() > 1) {
						if (BattleManager->SelectedCombatNPCIndex + 1 < BattleAlliesPlayer.Num()) 
								BattleManager->SelectNewTarget(BattleAlliesPlayer[BattleManager->SelectedCombatNPCIndex + 1], BattleManager->SelectedCombatNPCIndex + 1);
						else 
								BattleManager->SelectNewTarget(BattleAlliesPlayer[0], 0);
					}
				}
				else {
					TArray<ACombatEnemyNPC*> BattleEnemies = BattleManager->BattleEnemies;
					//Choose Target with scroll
					if (BattleEnemies.Num() > 1) {
						if (BattleManager->SelectedCombatNPCIndex + 1 < BattleEnemies.Num())
							BattleManager->SelectNewTarget(BattleEnemies[BattleManager->SelectedCombatNPCIndex + 1], BattleManager->SelectedCombatNPCIndex + 1);
						else
							BattleManager->SelectNewTarget(BattleEnemies[0], 0);
					}
				}
}

void APlayerCharacter::InputScrollRight()
{
	if (IsValid(Controller)) 
		if(!Combat_IsMovingToAttackEnemy)
			if (BattleMenuWidget->IsPreparingToAttack) 
				if (BattleManager->IsSelectingAllyAsTarget) {
				TArray<ACombatNPC*> BattleAlliesPlayer = BattleManager->BattleAlliesPlayer;
				//Choose target with scroll
				if (BattleAlliesPlayer.Num() > 1) {
					if (BattleManager->SelectedCombatNPCIndex - 1 >= 0) 
							BattleManager->SelectNewTarget(BattleAlliesPlayer[BattleManager->SelectedCombatNPCIndex - 1], BattleManager->SelectedCombatNPCIndex - 1);
					else 
							BattleManager->SelectNewTarget(BattleAlliesPlayer[BattleAlliesPlayer.Num() - 1], BattleAlliesPlayer.Num() - 1);
				}
			}
			else {
				TArray<ACombatEnemyNPC*> BattleEnemies = BattleManager->BattleEnemies;
				//Choose target with scroll
				if (BattleEnemies.Num() > 1) {
					if (BattleManager->SelectedCombatNPCIndex - 1 >= 0)
						BattleManager->SelectNewTarget(BattleEnemies[BattleManager->SelectedCombatNPCIndex - 1], BattleManager->SelectedCombatNPCIndex - 1);
					else
						BattleManager->SelectNewTarget(BattleEnemies[BattleEnemies.Num() - 1], BattleEnemies.Num() - 1);
				}
			}
}

void APlayerCharacter::InputScrollUp()
{
	if (IsValid(Controller) && IsValid(UIManager) && IsValid(BattleMenuWidget)) {
		//Get 
		if (BattleMenuWidget->IsChoosingAction) {
			TArray<UWidget*> MenuVerticalBoxChildrens = BattleMenuWidget->GetMenuVerticalBox()->GetAllChildren();
			if (MenuVerticalBoxChildrens.Num() > 0) {
				if (UIManager->PickedButtonIndex == 0) {
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
					UIManager->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[MenuVerticalBoxChildrens.Num() - 1]);
					UIManager->PickedButtonIndex = MenuVerticalBoxChildrens.Num() - 1;
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				else {
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
					UIManager->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[UIManager->PickedButtonIndex - 1]);
					UIManager->PickedButtonIndex = UIManager->PickedButtonIndex - 1;
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
			}
		}
		else if (BattleMenuWidget->IsChoosingItem && !BattleMenuWidget->IsPreparingToAttack) {
			TArray<UWidget*> InventoryScrollBoxChildrens = InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren();
			if (InventoryScrollBoxChildrens.Num() > 0) {
				if (UIManager->PickedButtonIndex == 0) {
					UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[InventoryScrollBoxChildrens.Num() - 1]);
					InventoryMenuWidget->GetInventoryScrollBox()->ScrollToEnd();
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
					UIManager->PickedButtonIndex = InventoryScrollBoxChildrens.Num() - 1;
					UIManager->PickedButton = EntryWidget->GetMainButton();
					InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				else {
					UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[UIManager->PickedButtonIndex - 1]);
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
					UIManager->PickedButtonIndex--;
					UIManager->PickedButton = EntryWidget->GetMainButton();
					InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
			}
		}
		else if (BattleMenuWidget->IsPreparingToAttack) {
			TArray<UWidget*> AttackMenuVerticalBoxChildrens = BattleMenuWidget->GetAttackMenuVerticalBox()->GetAllChildren();
			if (AttackMenuVerticalBoxChildrens.Num() > 0) {
				if (UIManager->PickedButtonIndex == 0) {
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
					UIManager->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[AttackMenuVerticalBoxChildrens.Num() - 1]);
					UIManager->PickedButtonIndex = AttackMenuVerticalBoxChildrens.Num() - 1;
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				else {
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
					UIManager->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManager->PickedButtonIndex - 1]);
					UIManager->PickedButtonIndex = UIManager->PickedButtonIndex - 1;
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
			}
		}
	}
}

void APlayerCharacter::InputScrollDown()
{
	if (IsValid(Controller) && IsValid(UIManager) && IsValid(BattleMenuWidget)) {
		if (BattleMenuWidget->IsChoosingAction) {
			TArray<UWidget*> MenuVerticalBoxChildrens = BattleMenuWidget->GetMenuVerticalBox()->GetAllChildren();
			if (MenuVerticalBoxChildrens.Num() > 0) {
				if (UIManager->PickedButtonIndex == MenuVerticalBoxChildrens.Num() - 1) {
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
					UIManager->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[0]);
					UIManager->PickedButtonIndex = 0;
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				else {
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
					UIManager->PickedButton = Cast<UButton>(MenuVerticalBoxChildrens[UIManager->PickedButtonIndex + 1]);
					UIManager->PickedButtonIndex = UIManager->PickedButtonIndex + 1;
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
			}
		}
		else if (BattleMenuWidget->IsChoosingItem && !BattleMenuWidget->IsPreparingToAttack && InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren().Num() > 0) {
			TArray<UWidget*> InventoryScrollBoxChildrens = InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren();
			if (InventoryScrollBoxChildrens.Num() > 0) {
				if (UIManager->PickedButtonIndex == InventoryScrollBoxChildrens.Num() - 1) {
					UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[0]);
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
					UIManager->PickedButtonIndex = 0;
					UIManager->PickedButton = EntryWidget->GetMainButton();
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
					InventoryMenuWidget->GetInventoryScrollBox()->ScrollToStart();
				}
				else {
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
					UIManager->PickedButtonIndex++;
					UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBoxChildrens[UIManager->PickedButtonIndex]);
					UIManager->PickedButton = EntryWidget->GetMainButton();
					InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
			}
		}
		else if (BattleMenuWidget->IsPreparingToAttack) {
			TArray<UWidget*> AttackMenuVerticalBoxChildrens = BattleMenuWidget->GetAttackMenuVerticalBox()->GetAllChildren();
			if (AttackMenuVerticalBoxChildrens.Num() > 0) {
				if (UIManager->PickedButtonIndex == AttackMenuVerticalBoxChildrens.Num() - 1) {
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
					UIManager->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[0]);
					UIManager->PickedButtonIndex = 0;
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				else {
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
					UIManager->PickedButton = Cast<UButton>(AttackMenuVerticalBoxChildrens[UIManager->PickedButtonIndex + 1]);
					UIManager->PickedButtonIndex = UIManager->PickedButtonIndex + 1;
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
			}
		}
	}
}

void APlayerCharacter::InputAction()
{
	if (CanInput) {
		AActor* ActorResult = ForwardRay().GetActor();
		if (IsValid(ActorResult)) {
			ChangeLevel(ActorResult);
			PickUpItem(ActorResult);
			DialogueInteract(ActorResult);
			NotificationActions(ActorResult);
		}
	}
	if (IsValid(BattleMenuWidget) && BattleMenuWidget->IsInViewport())
		ActionButtonBattleMenuInteraction();
	if (IsValid(InventoryMenuWidget) && InventoryMenuWidget->IsInViewport())
		ActionButtonInventoryMenuInteraction();
	if (IsValid(BattleResultsScreenWidget) && BattleResultsScreenWidget->IsInViewport())
		ActionButtonBattleResultsScreenInteraction();
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
		if (GameInstance->KilledEnemies < 3) {
			GetWorld()->GetTimerManager().ClearTimer(RemoveNotificationTimerHandle);
			NotificationWidget->AddToViewport(); 
			NotificationWidget->SetNotificationTextBlockText(FText::FromString("You need to kill all enemies before proceeding!"));
			GetWorld()->GetTimerManager().SetTimer(RemoveNotificationTimerHandle, this, &APlayerCharacter::RemoveNotification, 3, false);
		}
		else if(GameInstance->KilledEnemies >= 3 && !GetWorld()->GetTimerManager().IsTimerActive(FinishGameTimerHandle)) {
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
			if(IsValid(PlayerController))
				PlayerController->ActivateTouchInterface(EmptyTouchInterface);
		}
}

void APlayerCharacter::ActionButtonBattleMenuInteraction()
{
	if (IsValid(BattleMenuWidget) && IsValid(UIManager)) {
		if (BattleMenuWidget->IsChoosingAction) {
			if (UIManager->PickedButton == BattleMenuWidget->GetAttackButton())
				BattleMenuWidget->AttackButtonOnClicked();
			else if (UIManager->PickedButton == BattleMenuWidget->GetDefendButton())
				BattleMenuWidget->DefendButtonOnClicked();
			else if (UIManager->PickedButton == BattleMenuWidget->GetItemButton())
				BattleMenuWidget->ItemButtonOnClicked();
		}
		else if (BattleMenuWidget->IsPreparingToAttack) {
			if (UIManager->PickedButton == BattleMenuWidget->GetAttackMenuBackButton())
				BattleMenuWidget->AttackMenuBackButtonOnClicked();
			else if (UIManager->PickedButton == BattleMenuWidget->GetAttackActionButton())
				BattleMenuWidget->AttackActionButtonOnClicked();
		}
	}
}

void APlayerCharacter::ActionButtonInventoryMenuInteraction() {
	if(IsValid(BattleMenuWidget))
		if (BattleMenuWidget->IsChoosingItem && !BattleMenuWidget->IsPreparingToAttack) {
			UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(UIManager->PickedButton->GetOuter()->GetOuter());
			if(IsValid(EntryWidget))
				InventoryMenuWidget->BattleMenuItemsUseButtonOnClicked();
		}
}

void APlayerCharacter::ActionButtonBattleResultsScreenInteraction()
{
	if(IsValid(BattleResultsScreenWidget))
		BattleResultsScreenWidget->ContinueButtonOnClicked();
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

void APlayerCharacter::GetHit_Implementation (int ValueOfAttack, const TArray<FElementAndItsPercentageStruct>& ContainedElements)
{
	int ValueOfArmor = 0;
	if(IsValid(InventoryMenuWidget) && IsValid(InventoryMenuWidget->EquipedHead))
		ValueOfArmor += InventoryMenuWidget->EquipedHead->GetArmorValue();
	if (IsValid(InventoryMenuWidget) && IsValid(InventoryMenuWidget->EquipedTorse))
		ValueOfArmor += InventoryMenuWidget->EquipedTorse->GetArmorValue();
	if (IsValid(InventoryMenuWidget) && IsValid(InventoryMenuWidget->EquipedHand))
		ValueOfArmor += InventoryMenuWidget->EquipedHand->GetArmorValue();
	if (IsValid(InventoryMenuWidget) && IsValid(InventoryMenuWidget->EquipedLowerArmor))
		ValueOfArmor += InventoryMenuWidget->EquipedLowerArmor->GetArmorValue();
	int ValueOfArmorBeforeEffects = ValueOfArmor;
	for (AEffect* Effect : Effects) {
		if (IsValid(Effect) && Effect->GetAreaOfEffect() == EEffectArea::ARMOR) {
			if (AEffectWithPlainModifier* EffectWithPlainModifier = Cast<AEffectWithPlainModifier>(Effect); IsValid(EffectWithPlainModifier)) {
				if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
					ValueOfArmor += ValueOfArmorBeforeEffects + Effect->GetEffectStat();
				else
					ValueOfArmor += ValueOfArmorBeforeEffects - Effect->GetEffectStat();
			}
			else {
				if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
					ValueOfArmor += ValueOfArmorBeforeEffects * (Effect->GetEffectStat() - 1);
				else
					ValueOfArmor -= ValueOfArmorBeforeEffects / Effect->GetEffectStat();
			}
		}
	}
	if (CurrentHP - (ValueOfAttack - ValueOfArmor /10) <= 0)
		CurrentHP = 0;
	else
		CurrentHP -= (ValueOfAttack - ValueOfArmor /10);
	if (IsValid(PlayerAnimInstance)) {
		if (CurrentHP <= 0)
			PlayerAnimInstance->SetPlayerIsDead(true);
		else if(!PlayerAnimInstance->GetPlayerIsBlock() && CurrentHP > 0)
			PlayerAnimInstance->SetPlayerGotHit(true);
	}
}

void APlayerCharacter::GetHitWithBuffOrDebuff_Implementation(const TArray<class AEffect*>& HitEffects)
{
	for(AEffect* Effect : HitEffects)
		Effects.Add(Effect);
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
	return CanInput;
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

AUIManager* APlayerCharacter::GetUIManager() const
{
	return UIManager;
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

class USkillBattleMenu* APlayerCharacter::GetSkillBattleMenuWidget() const
{
	return SkillBattleMenuWidget;
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
	CanInput = Value;
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
