// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatNPC.h"
#include "CombatAllies.generated.h"

/**
 * 
 */
UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum ESkillsLeveledUp
{
	SkillsLeveledUpNone = 0x0000,
	SkillsLeveledUpMelee = 0x0001,
	SkillsLeveledUpRange = 0x0002,
	SkillsLeveledUpAssaultSpells = 0x0004,
	SkillsLeveledUpDebuffSpells = 0x0008,
	SkillsLeveledUpRestorationSpells = 0x0010,
	SkillsLeveledUpBuffSpells = 0x0020,
	SkillsLeveledUpDefend = 0x0040,
	SkillsLeveledUpPersuasion = 0x0080,
};

ENUM_CLASS_FLAGS(ESkillsLeveledUp)

UCLASS()
class REDEMPTION_API ACombatAllies : public ACombatNPC
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACombatAllies();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		class UFloatingManaBarWidget* FloatingManaBarWidget {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UWidgetComponent* FloatingManaBarComponentWidget{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information")
		UTexture* CharacterPortrait{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		TMap<ECharacterSkills, int> SkillsProgressMap{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "/Script/Redemption.ESkillsLeveledUp"), Category = "Role-playing System")
		int32 SkillsLeveledUpBitmaskCode{};

	//Use in constructor only.
	void InitializeSkillsProgress();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Variables for movement in battle scene
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle")
		bool IsMovingToAttackEnemy = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle")
		bool IsMovingToStartPosition = false;

	void StartMovingToEnemy();
	void StartMovingToStartLocation();
	void ResetSkillsLeveledUp();

	//When aiming with a range weapon, center mark is flying around the screen with a certain speed. Set that speed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
		float CenterMarkMovingSpeed{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int CurrentExperience = 0;

	const UTexture* GetCharacterPortrait() const;
	UFloatingManaBarWidget* GetFloatingManaBarWidget() const;

	const int16 GetSkillsProgress(const ECharacterSkills SkillToGet) const;
	void SetSkillsProgress(const ECharacterSkills SkillToGet, const int16 NewValue);

	const bool GetSkillsLeveledUp(const ESkillsLeveledUp SkillToGet) const;
	void SetSkillsLeveledUp(const ESkillsLeveledUp SkillToSet, const bool Value);

	void AddSkillsProgress(const ECharacterSkills SkillToAddTo, const int16 ValueToAdd);
	void AddSkills(const ECharacterSkills SkillToAddTo, const int16 ValueToAdd);

	const TMap<ECharacterSkills, int>& GetSkillsMap() const;
	const TMap<ECharacterSkills, int>& GetSkillsProgressMap() const;
};
