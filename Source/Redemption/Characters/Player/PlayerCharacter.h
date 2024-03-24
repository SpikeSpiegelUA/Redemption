
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Characters\CharacterInTheWorld.h"
#include "..\Miscellaneous\MyInputConfigData.h"
#include "..\Dynamics\World\Items\Equipment\EquipmentItem.h"
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
#include "..\Dynamics\Gameplay\Managers\LevelingUpManager.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "..\UI\Menus\CombatCharacterInfoMenu.h"
#include "..\Public\UIManagerWorldSubsystem.h"
#include "..\UI\HUD\Dialogue\DialogueBox.h"
#include "..\UI\HUD\Dialogue\ResponsesBox.h"
#include "..\UI\HUD\Dialogue\ResponseEntry.h"
#include "..\UI\HUD\ForwardRayInfo.h"
#include "..\UI\HUD\Notification.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\Effect.h"
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
	UPROPERTY()
		UUIManagerWorldSubsystem* UIManagerWorldSubsystem{};
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
	bool GetCanInput() const;
	void SetCanInput(bool Value);
	UTouchInterface* GetEmptyTouchInterface() const;
	UTouchInterface* GetStandardTouchInterface() const;
	TArray<ACombatAllyNPC*> GetAllies() const;
	const TArray<TSubclassOf<ASpell>>& GetAvailableSkills() const;

	//Restore widgets to default state
	void RestartBattleMenuWidget();
	void RestartBattleResultsScreenWidget();

	void AddNewAllyToAllies(ACombatAllyNPC* const AllyToAdd);
	void AddNewSkillToAvailableSkills(const TSubclassOf<ASpell> SkillClassToAdd);

	void SetAllies(const TArray<TSubclassOf<ACombatAllies>>& NewAllies);
	const int8 GetStat(const ECharacterStats StatToGet) const;
	const int8 GetSkill(const ECharacterSkills SkillToGet) const;
	const int8 GetSkillsProgress(const ECharacterSkills SkillToGet) const;
	void SetStat(const ECharacterStats StatToSet, const int8 NewValue);
	void SetSkill(const ECharacterSkills SkillToSet, const int8 NewValue);
	void SetSkillsProgress(const ECharacterSkills SkillToSet, const int8 NewValue);

	void AddSkillsProgress(const ECharacterSkills SkillToAddTo, const int16 ValueToAdd);
	void AddSkills(const ECharacterSkills SkillToAddTo, const int16 ValueToAdd);

	//Copy current health, mana, skills and skills' progress.
	void CopyInfoFromCombatPlayer(const class ACombatPlayerCharacter* const CombatPlayerCharacter);
protected:
	void CheckForwardRayHitResult();
	//Use in constructor only.
	void InitializeStats();
	//Use in constructor only.
	void InitializeSkills();
	//Use in constructor only.
	void InitializeSkillsProgress();

	UFUNCTION()
	void RemoveNotification();
	UFUNCTION()
	void FinishGame();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	TArray<ACombatAllyNPC*> Allies{};

	UPROPERTY()
	APlayerController* PlayerController{};
	UPROPERTY()
	USkeletalMeshComponent* PlayerSkeletalMesh{};
	UPROPERTY()
	UPlayerCharacterAnimInstance* PlayerAnimInstance{};

	//Touch interfaces
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mobile", meta = (AllowPrivateAccess = true))
	UTouchInterface* StandardTouchInterface{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mobile", meta = (AllowPrivateAccess = true))
	UTouchInterface* EmptyTouchInterface{};

	//Timer Handles
	FTimerHandle RemoveNotificationTimerHandle{};
	FTimerHandle FinishGameTimerHandle{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame, meta = (AllowPrivateAccess = true))
	TMap<ECharacterStats, int> StatsMap{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame, meta = (AllowPrivateAccess = true))
	TMap<ECharacterSkills, int> SkillsMap{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame, meta = (AllowPrivateAccess = true))
	TMap<ECharacterSkills, int> SkillsProgressMap{};

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	
	void LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance) override;

	//Ray of detecting objects in front of a player
	FHitResult ForwardRay();

	UPROPERTY(EditAnywhere, Category = "World")
		TSubclassOf<class ALootInTheWorld> LootInTheWorldClass{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spells", SaveGame)
	TArray<TSubclassOf<ASpell>> AvailableSkills{};

	// Handle move input
	void Move(const FInputActionValue& Value);

	// Handle look input
	void Look(const FInputActionValue& Value);

	//Jump input for binding
	void InputJump();

#pragma region
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
	UFUNCTION()
	float GetHealthPercentage() const;
	UFUNCTION()
	float GetManaPercentage() const;

	//Perks variables.
#pragma region
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perks")
	TArray<FText> PerksCategoryNames{};
	//Currently is limited to 9 perks, 3 per category. Indexes 3,4,5 are in the Second category, etc. There are perks, that the player has to buy.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perks")
	TArray<TSubclassOf<APerk>> AvailablePerks{};
#pragma endregion
public:
#pragma region
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UInputMappingContext* InputMapping{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UMyInputConfigData* InputActions{};
#pragma endregion
	//Battle mode regarding variables
	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		float CurrentHP = 110;
	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		float MaxHP = 110;
	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		float CurrentMana = 110;
	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		float MaxMana = 110;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int CurrentExperience = 0;
	//Count how many "leveling ups" the character has.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int LevelingUpCounter = 0;

	//General character information.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "General Information")
		FString CharacterName{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "General Information")
		UTexture* CharacterPortrait{};
	//This is used by right button in the battle menu.
	void InputScrollRight();
	//This is used by left button in the battle menu.
	void InputScrollLeft();

	const TArray<FText>& GetPerksCategoryNames() const;
	const TArray<TSubclassOf<APerk>>& GetAvailablePerks() const;
	//Perks variables.

	//Active perks.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perks", SaveGame)
	TArray<TSubclassOf<APerk>> ActivatedPerks{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perks", SaveGame)
	int NumberOfPerkPoints = 0;
};
