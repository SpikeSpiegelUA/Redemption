#include "CombatAllyNPC.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Redemption/Miscellaneous/BitmapsActions.h"
#include "UIManagerWorldSubsystem.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

// Sets default values
ACombatAllyNPC::ACombatAllyNPC()
{
	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACombatAllyNPC::BeginPlay()
{
	Super::BeginPlay();
	if (URedemptionGameInstance* RedemptionGameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(RedemptionGameInstance))
		Execute_LoadObjectFromGameInstance(this, RedemptionGameInstance);
	if (const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()))
		if (const auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
			if (UIManagerWorldSubsystem->AlliesInfoBarsWidget->IsInViewport()) {
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[PlayerCharacter->GetAllies().Num() + 1]->PercentDelegate.Clear();
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[PlayerCharacter->GetAllies().Num() + 1]->PercentDelegate.Clear();
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesInfoVerticalBoxes()[PlayerCharacter->GetAllies().Num() + 1]->SetVisibility(ESlateVisibility::Visible);
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[PlayerCharacter->GetAllies().Num() + 1]->PercentDelegate.BindUFunction(this, "GetHealthPercentage");
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[PlayerCharacter->GetAllies().Num() + 1]->PercentDelegate.BindUFunction(this, "GetManaPercentage");
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[PlayerCharacter->GetAllies().Num() + 1]->SynchronizeProperties();
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[PlayerCharacter->GetAllies().Num() + 1]->SynchronizeProperties();
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesNameTextBlockes()[PlayerCharacter->GetAllies().Num() + 1]->SetText(FText::FromName(this->GetCharacterName()));
			}
}

// Called every frame
void ACombatAllyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACombatAllyNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACombatAllyNPC::LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance)
{
	if (IsValid(GameInstance)) {
		for (FCombatAllyNPCGameInstanceData CombatAllyNPCGameInstanceData : GameInstance->CombatAllyNPCs) {
			if (CombatAllyNPCGameInstanceData.ActorName == this->GetFName())
			{
				FMemoryReader MemReader(CombatAllyNPCGameInstanceData.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				Ar.ArIsSaveGame = true;
				// Convert binary array back into actor's variables
				Serialize(Ar);
				break;
			}
		}
	}
}

