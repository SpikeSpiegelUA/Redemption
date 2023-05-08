
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\MyInputConfigData.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\EquipmentItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\InventoryMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\PlayerMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\BattleResultsScreen.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\GameManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\DialogueBox.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\ResponsesBox.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\ResponseEntry.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\ForwardRayInfo.h"
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

	UPROPERTY(BlueprintReadOnly, Category = "GeneralInformation", meta = (AllowPrivateAccess = true))
		ABattleManager* BattleManager;
	UPROPERTY(BlueprintReadOnly, Category = "GeneralInformation", meta = (AllowPrivateAccess = true))
		AGameManager* GameManager;
	UPROPERTY(BlueprintReadOnly, Category = "GeneralInformation", meta = (AllowPrivateAccess = true))
		URedemptionGameInstance* GameInstance;

	UPROPERTY()
		APlayerController* PlayerController;
	UPROPERTY()
		USkeletalMeshComponent* PlayerSkeletalMesh;
	UPROPERTY()
		UPlayerCharacterAnimInstance* PlayerAnimInstance;
	UPROPERTY()
		AUIManager* UIManager;
public:
	APlayerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		float TurnRateGamepad;
	UPROPERTY(EditAnywhere, Category = "Player")
		int ForwardRayRange = 300;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	//Access functions
	UPlayerMenu* GetPlayerMenuWidget();
	UInventoryMenu* GetInventoryMenuWidget();
	UBattleResultsScreen* GetBattleResultsScreenWidget();
	TSubclassOf<class UInventoryScrollBoxEntryWidget> GetInventoryScrollBoxEntryClass();
	TSubclassOf<class UResponseEntry> GetResponseEntryClass();
	bool GetCanInput();
	void SetCanInput(bool Value);
	UBattleMenu* GetBattleMenuWidget();
	ABattleManager* GetBattleManager();
	AGameManager* GetGameManager();
	URedemptionGameInstance* GetGameInstance();
	UInventoryScrollBoxEntryWidget* GetInventoryScrollBoxEntryWidget();
	UDialogueBox* GetDialogueBoxWidget();
	UForwardRayInfo* GetForwardRayInfoWidget();
	UResponsesBox* GetResponsesBox();

	virtual void GetHit(int ValueOfAttack, EquipmentDamageType TypeOfDamage) override;

	//Restore widgets to default state
	void RestartBattleMenuWidget();
	void RestartBattleResultsScreenWidget();

	void SetInventoryScrollBoxEntryWidget(UInventoryScrollBoxEntryWidget* NewWidget);

	int8 MeleeAttackValue = 1;
	int8 RangeAttackValue = 0;
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
	    TSubclassOf<class UForwardRayInfo> ForwardRayInfoClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class ULoadingScreen> LoadingScreenClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UPlayerMenu> PlayerMenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UInventoryScrollBoxEntryWidget> InventoryScrollBoxEntryClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UInventoryMenu> InventoryMenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UBattleMenu> BattleMenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UBattleResultsScreen> BattleResultsScreenClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UPlayerBarsWidget> PlayerBarsClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UDialogueBox> DialogueBoxClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UResponsesBox> ResponsesBoxClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UResponseEntry> ResponseEntryClass;
	//The widget instances
	UPROPERTY()
		class UForwardRayInfo* ForwardRayInfoWidget;
	UPROPERTY()
		class UPlayerMenu* PlayerMenuWidget;
	UPROPERTY()
		class UInventoryScrollBoxEntryWidget* InventoryScrollBoxEntryWidget;
	UPROPERTY()
		UInventoryMenu* InventoryMenuWidget;
	UPROPERTY()
		UBattleMenu* BattleMenuWidget;
	UPROPERTY()
		UBattleResultsScreen* BattleResultsScreenWidget;
	UPROPERTY()
		UPlayerBarsWidget* PlayerBarsWidget;
	UPROPERTY()
		UDialogueBox* DialogueBoxWidget;
	UPROPERTY()
		UResponsesBox* ResponsesBoxWidget;

	bool CanInput = true;

public:
#pragma region
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
		class UMyInputConfigData* InputActions;
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
		int8 ArmorValue;
	UPROPERTY(EditAnywhere, Category = "Battle")
		int16 AttackValue;
	//Variables for movement in battle scene
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle")
	    bool Battle_IsMovingToAttackEnemy = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle")
		bool Battle_IsMovingToStartPosition = false;

	bool IsInDialogue = false;



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
	void PickUpItem(AActor* ActorResult);
	void DialogueInteract(AActor* ActorResult);
	void ActionButtonBattleMenuInteraction();
	void ActionButtonBattleResultsScreenInteraction();
	void ActionButtonInventoryMenuInteraction();
	void ChangeLevel(AActor* ActorResult);

	//Opens player menu
	void InputOpenPlayerMenu();

	//Scroll Control
	void InputScrollRight();
	void InputScrollLeft();
	void InputScrollUp();
	void InputScrollDown();
#pragma endregion
};
