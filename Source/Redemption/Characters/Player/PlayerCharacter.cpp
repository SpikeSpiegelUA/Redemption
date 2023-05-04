// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\LoadingScreen.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\PlayerBarsWidget.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\LootInTheWorld.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Interfaces\DialogueActionsInterface.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Other\TownNPC.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include <Kismet/GameplayStatics.h>
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

	TArray<AActor*> BattleManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleManager::StaticClass(), BattleManagerActors);
	TArray<AActor*> GameManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), GameManagerActors);
	if(BattleManagerActors.Num() > 0)
	BattleManager = Cast<ABattleManager>(BattleManagerActors[0]);
	if (GameManagerActors.Num() > 0)
		GameManager = Cast<AGameManager>(GameManagerActors[0]);
}

void APlayerCharacter::BeginPlay() 
{
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController)
		PlayerSkeletalMesh = PlayerController->GetPawn()->FindComponentByClass<USkeletalMeshComponent>();
	if (PlayerSkeletalMesh)
		PlayerAnimInstance = Cast<UPlayerCharacterAnimInstance>(PlayerSkeletalMesh->GetAnimInstance());
	GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	//Creating UI widgets...
	if (PlayerController) {
		if (ForwardRayInfoClass)
			ForwardRayInfoWidget = CreateWidget<UForwardRayInfo>(PlayerController, ForwardRayInfoClass);
		if (PlayerMenuClass)
			PlayerMenuWidget = CreateWidget<UPlayerMenu>(PlayerController, PlayerMenuClass);
		if (InventoryScrollBoxEntryClass)
			InventoryScrollBoxEntryWidget = CreateWidget<UInventoryScrollBoxEntryWidget>(PlayerController, InventoryScrollBoxEntryClass);
		if (BattleMenuClass)
			BattleMenuWidget = CreateWidget<UBattleMenu>(PlayerController, BattleMenuClass);
		if (BattleResultsScreenClass)
			BattleResultsScreenWidget = CreateWidget<UBattleResultsScreen>(PlayerController, BattleResultsScreenClass);
		if (InventoryMenuClass)
			InventoryMenuWidget = CreateWidget<UInventoryMenu>(PlayerController, InventoryMenuClass);
		if (PlayerBarsClass)
			PlayerBarsWidget = CreateWidget<UPlayerBarsWidget>(PlayerController, PlayerBarsClass);
		if (DialogueBoxClass)
			DialogueBoxWidget = CreateWidget<UDialogueBox>(PlayerController, DialogueBoxClass);
		if (ResponsesBoxClass)
			ResponsesBoxWidget = CreateWidget<UResponsesBox>(PlayerController, ResponsesBoxClass);
	}
	//Level change logic
	if (GameInstance) {
		CurrentHP = GameInstance->PlayerCurrentHP;
		CurrentMana = GameInstance->PlayerCurrentMana;
		MaxHP = GameInstance->PlayerMaxHP;
		MaxMana = GameInstance->PlayerMaxMana;
	}
	if (GetWorld()->GetName() == "Dungeon")
		PlayerBarsWidget->AddToViewport();

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
	//Move player to enemy in Battle scene
	if (Battle_IsMovingToAttackEnemy) {
		FVector DistanceVector;
		if (BattleManager->SelectedEnemy) {
			DistanceVector = FVector(BattleManager->SelectedEnemy->GetActorLocation() - this->GetActorLocation());
			FVector NewPosition = FMath::VInterpTo(this->GetActorLocation(), BattleManager->SelectedEnemy->GetActorLocation(), DeltaTime, 0.6f);
			this->SetActorLocation(NewPosition);
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), BattleManager->SelectedEnemy->GetActorLocation());
			Rotation = FRotator(this->GetActorRotation().Pitch, Rotation.Yaw, this->GetActorRotation().Roll);
			this->SetActorRotation(Rotation);
			if (PlayerAnimInstance)
				PlayerAnimInstance->SetSpeedToActualSpeed = false;
		}
		//If distance between player and selected enemy is less than 160, turn on animation
		if (DistanceVector.Length() <= 160) {
			Battle_IsMovingToAttackEnemy = false;
			if (PlayerAnimInstance) {
				PlayerAnimInstance->SetIsAttacking(true);
				PlayerAnimInstance->SetSpeedToActualSpeed = true;
			}
		}
	}
	//Move player back to his start position
	else if (Battle_IsMovingToStartPosition) {
		FVector DistanceVector;
		if (GameManager->GetPlayerBattleSpawn()) {
			DistanceVector = FVector(GameManager->GetPlayerBattleSpawn()->GetActorLocation() - this->GetActorLocation());
			FVector NewPosition = FMath::VInterpTo(this->GetActorLocation(), GameManager->GetPlayerBattleSpawn()->GetActorLocation(), DeltaTime, 0.6f);
			this->SetActorLocation(NewPosition);
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), GameManager->GetPlayerBattleSpawn()->GetActorLocation());
			Rotation = FRotator(this->GetActorRotation().Pitch, Rotation.Yaw, this->GetActorRotation().Roll);
			this->SetActorRotation(Rotation);
			if (PlayerAnimInstance)
				PlayerAnimInstance->SetSpeedToActualSpeed = false;
		}
		//If distance between player and start position is less than 100, stop
		if (DistanceVector.Length() <= 100) {
			BattleMenuWidget->GetCenterMark()->SetVisibility(ESlateVisibility::Hidden);
			Battle_IsMovingToStartPosition = false;
			this->SetActorRotation(FRotator(0, 180, 0));
			if (PlayerAnimInstance)
				PlayerAnimInstance->SetSpeedToActualSpeed = true;
			BattleManager->PlayerTurnController();
		}
	}
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
		if (Controller != nullptr) {
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
	if(CanInput)
		Jump();
}

void APlayerCharacter::InputOpenPlayerMenu()
{
	if (CanInput) {
		PlayerMenuWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->SetPause(true);
	}
}

void APlayerCharacter::InputScrollLeft()
{
	if (Controller != nullptr) {
		if (!Battle_IsMovingToAttackEnemy) {
			if (BattleMenuWidget->IsPreparingToAttack) {
				TArray<ACombatEnemyNPC*> BattleEnemies = BattleManager->BattleEnemies;
				//Choose enemy with scroll
				if (BattleEnemies.Num() > 1) {
					if (BattleManager->SelectedEnemyIndex + 1 < BattleEnemies.Num())
						BattleManager->SelectNewEnemy(BattleEnemies[BattleManager->SelectedEnemyIndex + 1], BattleManager->SelectedEnemyIndex + 1);
					else
						BattleManager->SelectNewEnemy(BattleEnemies[0], 0);
				}
			}
		}
	}
}

void APlayerCharacter::InputScrollRight()
{
	if (Controller != nullptr) {
		if(!Battle_IsMovingToAttackEnemy){
			if (BattleMenuWidget->IsPreparingToAttack) {
				TArray<ACombatEnemyNPC*> BattleEnemies = BattleManager->BattleEnemies;
				//Choose enemy with scroll
				if (BattleEnemies.Num() > 1) {
					if (BattleManager->SelectedEnemyIndex - 1 >= 0)
						BattleManager->SelectNewEnemy(BattleEnemies[BattleManager->SelectedEnemyIndex - 1], BattleManager->SelectedEnemyIndex - 1);
					else
						BattleManager->SelectNewEnemy(BattleEnemies[BattleEnemies.Num() - 1], BattleEnemies.Num() - 1);
				}
			}
	    }
	}
}

void APlayerCharacter::InputScrollUp()
{
	if (Controller != nullptr) {
		//Get 
		if (BattleMenuWidget->IsChoosingAction) {
			TArray<UWidget*> MenuVerticalBoxChildrens = BattleMenuWidget->GetMenuVerticalBox()->GetAllChildren();
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
		else if (BattleMenuWidget->IsChoosingItem && !BattleMenuWidget->IsPreparingToAttack) {
			TArray<UWidget*> InventoryScrollBoxChildrens = InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren();
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
				UIManager-> PickedButton = EntryWidget->GetMainButton();
				InventoryMenuWidget->SetPickedItem(EntryWidget->GetItem());
				UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
		else if (BattleMenuWidget->IsPreparingToAttack) {
			TArray<UWidget*> AttackMenuVerticalBoxChildrens = BattleMenuWidget->GetAttackMenuVerticalBox()->GetAllChildren();
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

void APlayerCharacter::InputScrollDown()
{
	if (Controller != nullptr) {
		if (BattleMenuWidget->IsChoosingAction) {
			TArray<UWidget*> MenuVerticalBoxChildrens = BattleMenuWidget->GetMenuVerticalBox()->GetAllChildren();
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
		else if (BattleMenuWidget->IsChoosingItem && !BattleMenuWidget->IsPreparingToAttack && InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren().Num() > 0) {
			TArray<UWidget*> InventoryScrollBoxChildrens = InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren();
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
		else if (BattleMenuWidget->IsPreparingToAttack) {
			TArray<UWidget*> AttackMenuVerticalBoxChildrens = BattleMenuWidget->GetAttackMenuVerticalBox()->GetAllChildren();
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

void APlayerCharacter::InputAction()
{
	if (CanInput) {
		AActor* ActorResult = ForwardRay().GetActor();
		if (ActorResult) {
			ChangeLevel(ActorResult);
			PickUpItem(ActorResult);
			DialogueInteract(ActorResult);
		}
	}
	if (BattleMenuWidget->IsInViewport())
		ActionButtonBattleMenuInteraction();
	if (InventoryMenuWidget->IsInViewport())
		ActionButtonInventoryMenuInteraction();
	if(BattleResultsScreenWidget->IsInViewport())
		ActionButtonBattleResultsScreenInteraction();
}

void APlayerCharacter::ChangeLevel(AActor* ActorResult)
{
	//If ray result isn't null and is level change trigger,loading screen is set, then load level 
	if (LoadingScreenClass) {
		if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToDungeon")))) {
			UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
			GameInstance->PlayerCurrentHP = CurrentHP;
			GameInstance->PlayerCurrentMana = CurrentMana;
			ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(PlayerController, LoadingScreenClass);
			LoadingScreen->AddToViewport();
			UGameplayStatics::OpenLevel(GetWorld(), "Dungeon");
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToTown")))) {
			UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
			GameInstance->PlayerCurrentHP = CurrentHP;
			GameInstance->PlayerCurrentMana = CurrentMana;
			ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(PlayerController, LoadingScreenClass);
			LoadingScreen->AddToViewport();
			UGameplayStatics::OpenLevel(GetWorld(), "Town");
		}
	}
}

void APlayerCharacter::PickUpItem(AActor* ActorResult)
{
		if (ActorResult->ActorHasTag(FName(TEXT("Loot")))) {
			ALootInTheWorld* LootInTheWorld = Cast<ALootInTheWorld>(ActorResult);
			bool IsInInventory = false;
			if (LootInTheWorld) {
				for (int i = 0; i < LootInTheWorld->GetItemsClasses().Num(); i++) {
					AGameItem* GameItem = NewObject<AGameItem>(this, LootInTheWorld->GetItemsClasses()[i]);
					GameInstance->InstanceItemsInTheInventory.Add(LootInTheWorld->GetItemsClasses()[i]);
					if (GameItem) {
						IsInInventory = false;
						//Get ScrollBox corresponding to the item's type
						UScrollBox* CurrentScrollBox = nullptr;
						if (GameItem->GetType() == ItemType::EQUIPMENT) {
							AEquipmentItem* EquipmentItem = Cast<AEquipmentItem>(GameItem);
							if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::MELEE)
								CurrentScrollBox = InventoryMenuWidget->GetMeleeInventoryScrollBox();
							else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::RANGE)
								CurrentScrollBox = InventoryMenuWidget->GetRangeInventoryScrollBox();
							else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::HEAD)
								CurrentScrollBox = InventoryMenuWidget->GetHeadInventoryScrollBox();
							else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::TORSE)
								CurrentScrollBox = InventoryMenuWidget->GetTorseInventoryScrollBox();
							else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::HAND)
								CurrentScrollBox = InventoryMenuWidget->GetHandInventoryScrollBox();
							else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::LOWERARMOR)
								CurrentScrollBox = InventoryMenuWidget->GetLowerArmorInventoryScrollBox();
						}
						else
							CurrentScrollBox = InventoryMenuWidget->GetInventoryScrollBox();
						//Check if this item is already in the inventory. If yes, than just add to AmountOfItems and change text, if not, then add new inventory widget
						for (int j = 0; j < CurrentScrollBox->GetChildrenCount(); j++) {
							UInventoryScrollBoxEntryWidget* CurrentWidget = Cast<UInventoryScrollBoxEntryWidget>(CurrentScrollBox->GetAllChildren()[j]);
							if (CurrentWidget)
								if (CurrentWidget->GetItem()->GetItemName() == GameItem->GetItemName()) {
									IsInInventory = true;
									CurrentWidget->AmountOfItems += 1;
									FString NameString = GameItem->GetItemName().ToString() + FString("(" + FString::FromInt(CurrentWidget->AmountOfItems) + ")");
									CurrentWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
									break;
								}
						}
						if (!IsInInventory) {
							InventoryScrollBoxEntryWidget = CreateWidget<UInventoryScrollBoxEntryWidget>(GetWorld(), InventoryScrollBoxEntryClass);
							InventoryScrollBoxEntryWidget->GetMainTextBlock()->SetText(FText::FromName(GameItem->GetItemName()));
							InventoryScrollBoxEntryWidget->SetItem(GameItem);
							InventoryScrollBoxEntryWidget->AddToViewport();
							CurrentScrollBox->AddChild(InventoryScrollBoxEntryWidget);
						}
					}
				}
				//Destroy container with loot
				ActorResult->Destroy();
			}
		}
}

void APlayerCharacter::DialogueInteract(AActor* ActorResult)
{
	//If hit result from forward vector is NPC with a dialogue, start the dialogue 
	if (ActorResult) 
		if (ActorResult->GetClass()->ImplementsInterface(UDialogueActionsInterface::StaticClass())) {
			ATownNPC* TownNPC = Cast<ATownNPC>(ActorResult);
			TownNPC->StartADialogue();
		}
}

void APlayerCharacter::ActionButtonBattleMenuInteraction()
{
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

void APlayerCharacter::ActionButtonInventoryMenuInteraction() {
	if (BattleMenuWidget->IsChoosingItem && !BattleMenuWidget->IsPreparingToAttack) {
		UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(UIManager->PickedButton->GetOuter()->GetOuter());
		if(EntryWidget)
			InventoryMenuWidget->BattleMenuItemsUseButtonOnClicked();
	}
}

void APlayerCharacter::ActionButtonBattleResultsScreenInteraction()
{
	BattleResultsScreenWidget->ContinueButtonOnClicked();
}

void APlayerCharacter::CheckForwardRayHitResult()
{
	//If hit result from forward vector is NPC or interactive object, add its name to the viewport
	AActor* ActorResult = ForwardRay().GetActor();
	if (ActorResult) {
		if (ActorResult->ActorHasTag(FName(TEXT("NPC")))) {
			if (ForwardRayInfoWidget) 
			{
				if (!ForwardRayInfoWidget->IsInViewport()) {
					ForwardRayInfoWidget->AddToViewport(0);
					ForwardRayInfoWidget->SetMainTextBlockText(FText::FromName(Cast<ACharacterInTheWorld>(ActorResult)->GetCharacterName()));
				}
			}
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToDungeon")))) {
			if (ForwardRayInfoWidget)
			{
				if (!ForwardRayInfoWidget->IsInViewport()) {
					ForwardRayInfoWidget->AddToViewport(0);
					ForwardRayInfoWidget->SetMainTextBlockText(FText::FromString("Enter the dungeon"));
				}
			}
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("ChangeLevelToTown")))) {
			if (ForwardRayInfoWidget)
			{
				if (!ForwardRayInfoWidget->IsInViewport()) {
					ForwardRayInfoWidget->AddToViewport(0);
					ForwardRayInfoWidget->SetMainTextBlockText(FText::FromString("Enter the town"));
				}
			}
		}
		else if (ActorResult->ActorHasTag(FName(TEXT("Loot")))) {
			if (ForwardRayInfoWidget)
			{
				ALootInTheWorld* LootInTheWorld = Cast<ALootInTheWorld>(ActorResult);
				if (LootInTheWorld)
					if (!ForwardRayInfoWidget->IsInViewport()) {
						ForwardRayInfoWidget->AddToViewport(0);
						ForwardRayInfoWidget->SetMainTextBlockText(FText::FromName(LootInTheWorld->GetName()));
				}
			}
		}
		//Remove from viewport, if ray didn't hit NPC or interactive object
		else {
			if(ForwardRayInfoWidget)
				if(ForwardRayInfoWidget->IsInViewport())
			ForwardRayInfoWidget->RemoveFromParent();
		}	
	}
	//Remove from viewport, if ray hit nothing
	else {
		if (ForwardRayInfoWidget)
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
	PEI->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	PEI->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	PEI->BindAction(InputActions->InputJump, ETriggerEvent::Started, this, &APlayerCharacter::InputJump);
	PEI->BindAction(InputActions->InputAction, ETriggerEvent::Started, this, &APlayerCharacter::InputAction);
	PEI->BindAction(InputActions->InputOpenPlayerMenu, ETriggerEvent::Started, this, &APlayerCharacter::InputOpenPlayerMenu);
	PEI->BindAction(InputActions->InputScrollLeft, ETriggerEvent::Started, this, &APlayerCharacter::InputScrollLeft);
	PEI->BindAction(InputActions->InputScrollRight, ETriggerEvent::Started, this, &APlayerCharacter::InputScrollRight);
	PEI->BindAction(InputActions->InputScrollUp, ETriggerEvent::Started, this, &APlayerCharacter::InputScrollUp);
	PEI->BindAction(InputActions->InputScrollDown, ETriggerEvent::Started, this, &APlayerCharacter::InputScrollDown);
}
//Ray of detecting objects in front of a player
FHitResult APlayerCharacter::ForwardRay()
{
	FHitResult HitResult(ForceInit);
	FVector StartTrace = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector EndTrace = (ForwardVector * ForwardRayRange) + StartTrace;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, *CQP);
	return HitResult;
}

void APlayerCharacter::GetHit(int ValueOfAttack, EquipmentDamageType TypeOfDamage)
{
	if (CurrentHP - (ValueOfAttack - ArmorValue) <= 0)
		CurrentHP = 0;
	else
		CurrentHP -= (ValueOfAttack - ArmorValue);
	if (PlayerAnimInstance)
		if (CurrentHP <= 0)
			PlayerAnimInstance->SetIsDead(true);
}

void APlayerCharacter::Death()
{

}

UInventoryScrollBoxEntryWidget* APlayerCharacter::GetInventoryScrollBoxEntryWidget()
{
	return InventoryScrollBoxEntryWidget;
}

void APlayerCharacter::RestartBattleMenuWidget()
{
	if (BattleMenuClass)
		if (PlayerController)
			BattleMenuWidget = CreateWidget<UBattleMenu>(PlayerController, BattleMenuClass);
}

void APlayerCharacter::RestartBattleResultsScreenWidget()
{
	if (BattleResultsScreenClass)
		if (PlayerController)
			BattleResultsScreenWidget = CreateWidget<UBattleResultsScreen>(PlayerController, BattleResultsScreenClass);
}

UForwardRayInfo* APlayerCharacter::GetForwardRayInfoWidget()
{
	return ForwardRayInfoWidget;
}

UDialogueBox* APlayerCharacter::GetDialogueBoxWidget()
{
	return DialogueBoxWidget;
}

UPlayerMenu* APlayerCharacter::GetPlayerMenuWidget()
{
	return PlayerMenuWidget;
}

UInventoryMenu* APlayerCharacter::GetInventoryMenuWidget()
{
	return InventoryMenuWidget;
}

UBattleResultsScreen* APlayerCharacter::GetBattleResultsScreenWidget()
{
	return BattleResultsScreenWidget;
}

TSubclassOf<class UInventoryScrollBoxEntryWidget> APlayerCharacter::GetInventoryScrollBoxEntryClass()
{
	return InventoryScrollBoxEntryClass;
}

TSubclassOf<class UResponseEntry> APlayerCharacter::GetResponseEntryClass()
{
	return ResponseEntryClass;
}

bool APlayerCharacter::GetCanInput()
{
	return CanInput;
}

UBattleMenu* APlayerCharacter::GetBattleMenuWidget()
{
	return BattleMenuWidget;
}

ABattleManager* APlayerCharacter::GetBattleManager()
{
	return BattleManager;
}

AGameManager* APlayerCharacter::GetGameManager()
{
	return GameManager;
}

URedemptionGameInstance* APlayerCharacter::GetGameInstance()
{
	return GameInstance;
}

void APlayerCharacter::SetCanInput(bool Value)
{
	CanInput = Value;
}

UResponsesBox* APlayerCharacter::GetResponsesBox()
{
	return ResponsesBoxWidget;
}

void APlayerCharacter::SetInventoryScrollBoxEntryWidget(UInventoryScrollBoxEntryWidget* NewWidget) {
	InventoryScrollBoxEntryWidget = NewWidget;
}