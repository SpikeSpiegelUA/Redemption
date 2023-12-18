
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Characters\CharacterInTheWorld.h"
#include "..\Miscellaneous\MyInputConfigData.h"
#include "..\Dynamics\World\Items\EquipmentItem.h"
#include "..\UI\Menus\BattleMenu.h"
#include "..\UI\Menus\InventoryMenu.h"
#include "..\UI\Menus\PauseMenu.h"
#include "..\UI\Menus\PlayerMenu.h"
#include "..\UI\Menus\SpellBattleMenu.h"
#include "..\UI\Menus\SkillBattleMenu.h"
#include "..\UI\Screens\BattleResultsScreen.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "..\Dynamics\Gameplay\Managers\GameManager.h"
#include "..\Dynamics\Gameplay\Managers\AudioManager.h"
#include "..\Dynamics\Gameplay\Managers\ParticlesManager.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "..\UI\Menus\CombatCharacterInfoMenu.h"
#include "..\Public\UIManagerWorldSubsystem.h"
#include "..\UI\HUD\Dialogue\DialogueBox.h"
#include "..\UI\HUD\Dialogue\ResponsesBox.h"
#include "..\UI\HUD\Dialogue\ResponseEntry.h"
#include "..\UI\HUD\ForwardRayInfo.h"
#include "..\UI\HUD\Notification.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "..\Dynamics\Gameplay\Managers\EffectsSpellsAndSkillsManager.h"
#include "..\UI\Menus\DeathMenu.h"
#include "..\UI\Menus\LearnedSpellsJournalMenu.h"
#include "..\UI\Menus\SaveLoadGameMenu.h"
#include "..\Characters\Combat\CombatAllies.h"
#include "..\UI\SpecificFunction\SpellInfo.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "..\Characters\Animation\PlayerCharacterAnimInstance.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Containers/EnumAsByte.h"
#include "..\UI\HUD\AlliesInfoBars.h"
#include "..\SaveSystem\Interfaces\SavableObjectInterface.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class APlayerCharacter : public ACharacter, public ISavableObjectInterface
{
	GENERATED_BODY()
	/** Camera boom positioning the camera behind the character*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* CameraBoom;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"));
	    class UCameraComponent* FollowCamera;
	
private:
	void CheckForwardRayHitResult();
	void Death();

	UFUNCTION()
		void RemoveNotification();
	UFUNCTION()
		void FinishGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		ABattleManager* BattleManager {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		AAudioManager* AudioManager {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		class AGameManager* GameManager {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		class AEffectsSpellsAndSkillsManager* EffectsManager {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		class AParticlesManager* ParticlesManager{};
	UPROPERTY(BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		URedemptionGameInstance* RedemptionGameInstance {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true), SaveGame)
		TArray<TSubclassOf<ACombatAllies>> Allies{};

	UPROPERTY()
		APlayerController* PlayerController {};
	UPROPERTY()
		USkeletalMeshComponent* PlayerSkeletalMesh {};
	UPROPERTY()
		UPlayerCharacterAnimInstance* PlayerAnimInstance {};
	UPROPERTY()
		UUIManagerWorldSubsystem* UIManagerWorldSubsystem {};

	//Touch interfaces
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mobile", meta = (AllowPrivateAccess = true))
		UTouchInterface* StandardTouchInterface {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mobile", meta = (AllowPrivateAccess = true))
		UTouchInterface* EmptyTouchInterface {};

	//Timer Handles
	FTimerHandle RemoveNotificationTimerHandle{};
	FTimerHandle FinishGameTimerHandle{};
public:
	APlayerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		float TurnRateGamepad{};
	UPROPERTY(EditAnywhere, Category = "Player")
		int ForwardRayRange = 300;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	//Access functions
	class UAlliesInfoBars* GetAlliesInfoBarsWidget() const;
	UPlayerMenu* GetPlayerMenuWidget() const;
	UInventoryMenu* GetInventoryMenuWidget() const;
	UBattleResultsScreen* GetBattleResultsScreenWidget() const;
	TSubclassOf<class UInventoryScrollBoxEntryWidget> GetInventoryScrollBoxEntryClass() const;
	TSubclassOf<class UResponseEntry> GetResponseEntryClass() const;
	bool GetCanInput() const;
	void SetCanInput(bool Value);
	UBattleMenu* GetBattleMenuWidget() const;
	ULearnedSpellsJournalMenu* GetLearnedSpellsJournalMenu() const;
	ABattleManager* GetBattleManager() const;
	UUIManagerWorldSubsystem* GetUIManagerWorldSubsystem() const;
	class AGameManager* GetGameManager() const;
	AAudioManager* GetAudioManager() const;
	AParticlesManager* GetParticlesManager() const;
	UDeathMenu* GetDeathMenuWidget() const;
	USettingsMenu* GetSettingsMenuWidget() const;
	USkillBattleMenu* GetSkillBattleMenuWidget() const;
	class USpellBattleMenu* GetSpellBattleMenuWidget() const;
	class USpellInfo* GetSpellInfoWidget() const;
	class AEffectsSpellsAndSkillsManager* GetEffectsSpellsAndSkillsManager() const;
	URedemptionGameInstance* GetGameInstance() const;
	UInventoryScrollBoxEntryWidget* GetInventoryScrollBoxEntryWidget() const;
	UDialogueBox* GetDialogueBoxWidget() const;
	UForwardRayInfo* GetForwardRayInfoWidget() const;
	UPauseMenu* GetPauseMenuWidget() const;
	UMainMenu* GetMainMenuWidget() const;
	UCombatCharacterInfoMenu* GetCombatCharacterInfoMenuWidget() const;
	USaveLoadGameMenu* GetSaveLoadGameMenuWidget() const;
	UResponsesBox* GetResponsesBox() const;
	UTouchInterface* GetEmptyTouchInterface() const;
	UTouchInterface* GetStandardTouchInterface() const;
	TArray<TSubclassOf<ACombatAllies>> GetAllies();

	//Restore widgets to default state
	void RestartBattleMenuWidget();
	void RestartBattleResultsScreenWidget();

	void SetInventoryScrollBoxEntryWidget(const UInventoryScrollBoxEntryWidget* const NewWidget);
	void SetGameManager(const AGameManager* const NewGameManager);
	void SetBattleManager(const ABattleManager* const NewBattleManager);
	void SetAudioManager(const AAudioManager* const NewAudioManager);
	void SetAllies(const TArray<TSubclassOf<ACombatAllies>>& NewAllies);
protected:
	/**Called for forwards/backward input*/
	void MoveForward(float Value);
	/**Called for side to side input*/
	void MoveRight(float Value);

	/*
	*Called via input to turn at a given rate.
	*@param Rate This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/*
	*Called via input to turn look up/down at a given rate.
	*@param Rate This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins.*/
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops.*/
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	
	void LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance) override;

	//Ray of detecting objects in front of a player
	FHitResult ForwardRay();

	//UI
	//Widget classes to spawn widget instances
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UAlliesInfoBars> AlliesInfoBarsClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UForwardRayInfo> ForwardRayInfoClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class ULoadingScreen> LoadingScreenClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UPlayerMenu> PlayerMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UInventoryScrollBoxEntryWidget> InventoryScrollBoxEntryClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UInventoryMenu> InventoryMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UPauseMenu> PauseMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UBattleMenu> BattleMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UBattleResultsScreen> BattleResultsScreenClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UDialogueBox> DialogueBoxClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UResponsesBox> ResponsesBoxClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UResponseEntry> ResponseEntryClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UNotification> NotificationClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UDeathMenu> DeathMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class USpellBattleMenu> SpellBattleMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class USettingsMenu> SettingsMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UMainMenu> MainMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class ULearnedSpellsJournalMenu> LearnedSpellsJournalMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class USkillBattleMenu> SkillBattleMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class USaveLoadGameMenu> SaveLoadGameMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class USpellInfo> SpellInfoClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UCombatCharacterInfoMenu> CombatCharacterInfoMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class ALootInTheWorld> LootInTheWorldClass{};
	//The widget instances
	UPROPERTY()
		class UForwardRayInfo* ForwardRayInfoWidget{};
	UPROPERTY()
		class UPlayerMenu* PlayerMenuWidget{};
	UPROPERTY()
		class UInventoryScrollBoxEntryWidget* InventoryScrollBoxEntryWidget{};
	UPROPERTY()
		class UInventoryMenu* InventoryMenuWidget{};
	UPROPERTY()
		class UPauseMenu* PauseMenuWidget{};
	UPROPERTY()
		class UBattleMenu* BattleMenuWidget{};
	UPROPERTY()
		class UBattleResultsScreen* BattleResultsScreenWidget{};
	UPROPERTY()
		class UAlliesInfoBars* AlliesInfoBarsWidget{};
	UPROPERTY()
		class UDialogueBox* DialogueBoxWidget{};
	UPROPERTY()
		class UResponsesBox* ResponsesBoxWidget{};
	UPROPERTY()
		class UNotification* NotificationWidget{};
	UPROPERTY()
		class UDeathMenu* DeathMenuWidget{};
	UPROPERTY()
		class USpellBattleMenu* SpellBattleMenuWidget{};
	UPROPERTY()
		class USettingsMenu* SettingsMenuWidget{};
	UPROPERTY()
		class UMainMenu* MainMenuWidget{};
	UPROPERTY()
		class ULearnedSpellsJournalMenu* LearnedSpellsJournalMenuWidget{};
	UPROPERTY()
		class USkillBattleMenu* SkillBattleMenuWidget{};
	UPROPERTY()
		class USaveLoadGameMenu* SaveLoadGameMenuWidget{};
	UPROPERTY()
		class USpellInfo* SpellInfoWidget{};
	UPROPERTY()
		class UCombatCharacterInfoMenu* CombatCharacterInfoMenuWidget{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells")
		TArray<TSubclassOf<ASpell>> AvailableSkills{};
public:
#pragma region
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UInputMappingContext* InputMapping{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UMyInputConfigData* InputActions{};
#pragma endregion
	//Battle mode regarding variables
	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		float CurrentHP = 100;
	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		float MaxHP = 100;
	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		float CurrentMana = 100;
	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		float MaxMana = 100;
	UPROPERTY(EditAnywhere, Category = "Inventory", SaveGame)
		int Gold = 0;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		TArray<AEffect*> Effects;

	bool IsInDialogue = false;
	//When the player wants to check Info of characters, he can select enemies as well as allies.
	bool CanSelectEveryoneAsATarget = false;
	//If a player opened an inventory menu, he can't open a pause menu.
	bool CanOpenOtherMenus = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int Strength = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int Perception = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int Endurance = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int Charisma = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int Intelligence = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int Will = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int Agility = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int Luck = 1;


	//This is used by right button in the battle menu.
	void InputScrollRight();
	//This is used by left button in the battle menu.
	void InputScrollLeft();

	const TArray<TSubclassOf<ASpell>> GetAvailableSkills() const;

private:
#pragma region
	// Handle move input
	void Move(const FInputActionValue& Value);

	// Handle look input
	void Look(const FInputActionValue& Value);

	//Jump input for binding
	void InputJump();

	void InputSpellUseSaveLoad();
	void InputUniqueSpellUseSpellInfoToggle();
	void InputSpellReset();
	void InputOpenLearnedSpells();
	void InputOpenSpellInfo();

	//Action button input for binding
	void InputAction();
	void PickUpItem(AActor* const ActorResult);
	void DialogueInteract(const AActor* const ActorResult);
	void ChangeLevel(const AActor* const ActorResult);
	void NotificationActions(const AActor* const ActorResult);

	//Opens player menu
	void InputOpenPlayerMenu();
	
	void InputOpenPauseMenu();
	void InputBack();
	//Scroll Control
	void InputScrollUp();
	void InputScrollDown();
#pragma endregion
};
