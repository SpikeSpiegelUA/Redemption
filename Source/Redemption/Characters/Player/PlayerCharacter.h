
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\MyInputConfigData.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\EquipmentItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\InventoryMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\PauseMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\PlayerMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SkillBattleMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\BattleResultsScreen.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\GameManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\AudioManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\DialogueBox.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\ResponsesBox.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\ResponseEntry.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\ForwardRayInfo.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Notification.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\EffectsSpellsAndSkillsManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\DeathMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\LearnedSpellsJournalMenu.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include <EnhancedInputComponent.h>
#include <InputActionValue.h>
#include "EnhancedInputSubsystems.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\PlayerCharacterAnimInstance.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Interfaces\BattleActionsInterface.h"
#include "Containers/EnumAsByte.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\PlayerBarsWidget.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class APlayerCharacter : public ACharacter, public IBattleActionsInterface
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
		AGameManager* GameManager {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		class AEffectsSpellsAndSkillsManager* EffectsManager {};
	UPROPERTY(BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		URedemptionGameInstance* GameInstance {};

	UPROPERTY()
		APlayerController* PlayerController {};
	UPROPERTY()
		USkeletalMeshComponent* PlayerSkeletalMesh {};
	UPROPERTY()
		UPlayerCharacterAnimInstance* PlayerAnimInstance {};
	UPROPERTY()
		AUIManager* UIManager {};

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
	class UPlayerBarsWidget* GetPlayerBarsWidget() const;
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
	AUIManager* GetUIManager() const;
	AGameManager* GetGameManager() const;
	AAudioManager* GetAudioManager() const;
	UDeathMenu* GetDeathMenuWidget() const;
	class USkillBattleMenu* GetSkillBattleMenuWidget() const;
	class AEffectsSpellsAndSkillsManager* GetEffectsSpellsAndSkillsManager() const;
	URedemptionGameInstance* GetGameInstance() const;
	UInventoryScrollBoxEntryWidget* GetInventoryScrollBoxEntryWidget() const;
	UDialogueBox* GetDialogueBoxWidget() const;
	UForwardRayInfo* GetForwardRayInfoWidget() const;
	UResponsesBox* GetResponsesBox() const;
	UTouchInterface* GetEmptyTouchInterface() const;
	UTouchInterface* GetStandardTouchInterface() const;

	//Function to call, when an enemy got hit. Parameters for a standard attack.
	void GetHit_Implementation(int ValueOfAttack, const TArray<ESpellElements>& ContainedElements) override;
	//Function to call, when an enemy got hit. Parameters for a buff/debuff attack.
	void GetHitWithBuffOrDebuff_Implementation(class AEffect* const& Effect) override;

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
		TSubclassOf<class UPlayerBarsWidget> PlayerBarsClass{};
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
		TSubclassOf<class USkillBattleMenu> SkillBattleMenuClass{};
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
		class UInventoryMenu* InventoryMenuWidget{};
	UPROPERTY()
		class UPauseMenu* PauseMenuWidget{};
	UPROPERTY()
		class UBattleMenu* BattleMenuWidget{};
	UPROPERTY()
		class UBattleResultsScreen* BattleResultsScreenWidget{};
	UPROPERTY()
		class UPlayerBarsWidget* PlayerBarsWidget{};
	UPROPERTY()
		class UDialogueBox* DialogueBoxWidget{};
	UPROPERTY()
		class UResponsesBox* ResponsesBoxWidget{};
	UPROPERTY()
		class UNotification* NotificationWidget{};
	UPROPERTY()
		class UDeathMenu* DeathMenuWidget{};
	UPROPERTY()
		class USkillBattleMenu* SkillBattleMenuWidget{};
	UPROPERTY()
		class ULearnedSpellsJournalMenu* LearnedSpellsJournalMenuWidget{};

	bool CanInput = true;

public:
#pragma region
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UInputMappingContext* InputMapping{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UMyInputConfigData* InputActions{};
#pragma endregion
	//Battle mode regarding variables
	UPROPERTY(EditAnywhere, Category = "Battle")
		float CurrentHP = 100;
	UPROPERTY(EditAnywhere, Category = "Battle")
		float MaxHP = 100;
	UPROPERTY(EditAnywhere, Category = "Battle")
		float CurrentMana = 100;
	UPROPERTY(EditAnywhere, Category = "Battle")
		float MaxMana = 100;
	UPROPERTY(EditAnywhere, Category = "Battle")
		int ArmorValue{};
	UPROPERTY(EditAnywhere, Category = "Battle")
		int AttackValue{};
	UPROPERTY(VisibleAnywhere, Category = "Battle")
		TArray<AEffect*> Effects;
	//Variables for movement in battle scene
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle")
	    bool Battle_IsMovingToAttackEnemy = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle")
		bool Battle_IsMovingToStartPosition = false;

	bool IsInDialogue = false;
	
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
	void ActionButtonBattleMenuInteraction();
	void ActionButtonBattleResultsScreenInteraction();
	void ActionButtonInventoryMenuInteraction();
	void ChangeLevel(const AActor* const &ActorResult);
	void NotificationActions(const AActor* const &ActorResult);

	//Opens player menu
	void InputOpenPlayerMenu();
	
	void InputOpenPauseMenu();

	//Scroll Control
	void InputScrollUp();
	void InputScrollDown();
#pragma endregion
};
