
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
#include "..\UI\Menus\DetailedCharacterInfoMenu.h"
#include "..\Characters\Combat\CombatAllyNPC.h"
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
#include "..\UI\Menus\PartyMenu.h"
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
	//Use in constructor only.
	void InitializeStats();
	//Use in constructor only.
	void InitializeSkills();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
		TArray<ACombatAllyNPC*> Allies{};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame, meta = (AllowPrivateAccess = true))
	TMap<ECharacterStats, int> StatsMap{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame, meta = (AllowPrivateAccess = true))
	TMap<ECharacterSkills, int> SkillsMap{};
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
	class UPartyMenu* GetPartyMenuWidget() const;
	class UDetailedCharacterInfoMenu* GetDetailedCharacterInfoMenuWidget() const;
	UTouchInterface* GetEmptyTouchInterface() const;
	UTouchInterface* GetStandardTouchInterface() const;
	TArray<ACombatAllyNPC*> GetAllies();
	const TArray<TSubclassOf<ASpell>> GetAvailableSkills() const;

	const TSubclassOf<UDetailedCharacterInfoMenu> GetDetailedCharacterInfoMenuClass() const;
	const TSubclassOf<UPartyMenu> GetPartyMenuClass() const;

	//Restore widgets to default state
	void RestartBattleMenuWidget();
	void RestartBattleResultsScreenWidget();
	void AddNewAllyToAllies(const ACombatAllyNPC* const AllyToAdd);

	void SetInventoryScrollBoxEntryWidget(const UInventoryScrollBoxEntryWidget* const NewWidget);
	void SetDetailedCharacterInfoMenuWidget(const UDetailedCharacterInfoMenu* const NewWidget);
	void SetPartyMenuWidget(const UPartyMenu* const NewWidget);
	void SetGameManager(const AGameManager* const NewGameManager);
	void SetBattleManager(const ABattleManager* const NewBattleManager);
	void SetAudioManager(const AAudioManager* const NewAudioManager);
	void SetAllies(const TArray<TSubclassOf<ACombatAllies>>& NewAllies);
	const int8 GetStat(const ECharacterStats StatToGet) const;
	const int8 GetSkill(const ECharacterSkills SkillToGet) const;
	void SetStat(const ECharacterStats StatToSet, const int8 NewValue);
	void SetSkill(const ECharacterSkills SkillToSet, const int8 NewValue);
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
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UAlliesInfoBars> AlliesInfoBarsClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UForwardRayInfo> ForwardRayInfoClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class ULoadingScreen> LoadingScreenClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UPlayerMenu> PlayerMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UInventoryScrollBoxEntryWidget> InventoryScrollBoxEntryClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UInventoryMenu> InventoryMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UPauseMenu> PauseMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UBattleMenu> BattleMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UBattleResultsScreen> BattleResultsScreenClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UDialogueBox> DialogueBoxClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UResponsesBox> ResponsesBoxClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UResponseEntry> ResponseEntryClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UNotification> NotificationClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UDeathMenu> DeathMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class USpellBattleMenu> SpellBattleMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class USettingsMenu> SettingsMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UMainMenu> MainMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class ULearnedSpellsJournalMenu> LearnedSpellsJournalMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class USkillBattleMenu> SkillBattleMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class USaveLoadGameMenu> SaveLoadGameMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class USpellInfo> SpellInfoClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UCombatCharacterInfoMenu> CombatCharacterInfoMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UPartyMenu> PartyMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UDetailedCharacterInfoMenu> DetailedCharacterInfoMenuClass{};
	UPROPERTY(EditAnywhere, Category = "World")
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
	UPROPERTY()
		class UPartyMenu* PartyMenuWidget{};
	UPROPERTY()
		class UDetailedCharacterInfoMenu* DetailedCharacterInfoMenuWidget{};

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
	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		int EvasionChance = 5;
	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		int TargetingChance = 25;
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
		int Level = 1;

	//General character information.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "General Information")
		FString CharacterName{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "General Information")
		UTexture* CharacterPortrait{};
	//This is used by right button in the battle menu.
	void InputScrollRight();
	//This is used by left button in the battle menu.
	void InputScrollLeft();

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
