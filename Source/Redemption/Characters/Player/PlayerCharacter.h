
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\MyInputConfigData.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\EquipmentItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\InventoryMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\PauseMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\PlayerMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SpellBattleMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\BattleResultsScreen.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\GameManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\AudioManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
#include "UIManagerWorldSubsystem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\DialogueBox.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\ResponsesBox.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\ResponseEntry.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\ForwardRayInfo.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Notification.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\EffectsSpellsAndSkillsManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\DeathMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\LearnedSpellsJournalMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatAllies.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\SkillEntryWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include <EnhancedInputComponent.h>
#include <InputActionValue.h>
#include "EnhancedInputSubsystems.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\PlayerCharacterAnimInstance.h"
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
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\AlliesInfoBars.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class APlayerCharacter : public ACharacter
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
	UPROPERTY(BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		URedemptionGameInstance* GameInstance {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
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
	UDeathMenu* GetDeathMenuWidget() const;
	USettingsMenu* GetSettingsMenuWidget() const;
	class USpellBattleMenu* GetSpellBattleMenuWidget() const;
	class AEffectsSpellsAndSkillsManager* GetEffectsSpellsAndSkillsManager() const;
	URedemptionGameInstance* GetGameInstance() const;
	UInventoryScrollBoxEntryWidget* GetInventoryScrollBoxEntryWidget() const;
	UDialogueBox* GetDialogueBoxWidget() const;
	UForwardRayInfo* GetForwardRayInfoWidget() const;
	UPauseMenu* GetPauseMenuWidget() const;
	UMainMenu* GetMainMenuWidget() const;
	UResponsesBox* GetResponsesBox() const;
	UTouchInterface* GetEmptyTouchInterface() const;
	UTouchInterface* GetStandardTouchInterface() const;
	TArray<TSubclassOf<ACombatAllies>> GetAllies() const;

	//Restore widgets to default state
	void RestartBattleMenuWidget();
	void RestartBattleResultsScreenWidget();

	void SetInventoryScrollBoxEntryWidget(UInventoryScrollBoxEntryWidget* NewWidget);
	void SetGameManager(AGameManager* const &NewGameManager);
	void SetBattleManager(ABattleManager* const &NewBattleManager);
	void SetAudioManager(AAudioManager* const &NewAudioManager);

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
		TSubclassOf<class USkillEntryWidget> SkillEntryClass{};
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
	//The widget instances
	UPROPERTY()
		class UForwardRayInfo* ForwardRayInfoWidget{};
	UPROPERTY()
		class UPlayerMenu* PlayerMenuWidget{};
	UPROPERTY()
		class UInventoryScrollBoxEntryWidget* InventoryScrollBoxEntryWidget{};
	UPROPERTY()
		USkillEntryWidget* SkillEntryWidget{};
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

public:
#pragma region
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UInputMappingContext* InputMapping{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UMyInputConfigData* InputActions{};
#pragma endregion
	//Battle mode regarding variables
	UPROPERTY(EditAnywhere, Category = "Combat")
		float CurrentHP = 100;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float MaxHP = 100;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float CurrentMana = 100;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float MaxMana = 100;
	UPROPERTY(EditAnywhere, Category = "Inventory")
		int Gold = 0;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		TArray<AEffect*> Effects;
	//Ultimately just spells, that I couldn't come up with crafting  formula for. Like armor debuff, weapon boost, etc.
	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<class ASpell*> Skills;

	bool IsInDialogue = false;
	//If a player opened an inventory menu, he can't open a pause menu.
	bool CanOpenOtherMenus = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Strength = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Perception = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Endurance = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Charisma = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Intelligence = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Will = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Agility = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Luck = 1;

	//Scroll Control
	//These are used by left/right button in battle menu
	void InputScrollRight();
	void InputScrollLeft();

private:
#pragma region
	// Handle move input
	void Move(const FInputActionValue& Value);

	// Handle look input
	void Look(const FInputActionValue& Value);

	//Jump input for binding
	void InputJump();

	//Action button input for binding
	void InputAction();
	void PickUpItem(AActor* const& ActorResult);
	void DialogueInteract(AActor* const &ActorResult);
	void ChangeLevel(const AActor* const &ActorResult);
	void NotificationActions(const AActor* const &ActorResult);

	//Opens player menu
	void InputOpenPlayerMenu();
	
	void InputOpenPauseMenu();
	void InputBack();
	//Scroll Control
	void InputScrollUp();
	void InputScrollDown();
#pragma endregion
};
