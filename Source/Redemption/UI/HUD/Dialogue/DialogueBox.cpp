// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\DialogueBox.h"

bool UDialogueBox::Initialize()
{
	const bool bSuccess = Super::Initialize();
	DialogueTextBlock->TextDelegate.BindUFunction(this, "GetDialogueText");
	SpeakerNameTextBlock->TextDelegate.BindUFunction(this, "GetSpeakerName");
	if (IsValid(ContinueButton))
		ContinueButton->OnClicked.AddDynamic(this, &UDialogueBox::ContinueButtonOnClicked);
	bIsFocusable = true;
	if (!bSuccess) return false;
	return bSuccess;
}

void UDialogueBox::NativeConstruct()
{
	Super::NativeConstruct();

}

void UDialogueBox::ContinueButtonOnClicked()
{
	//Simulate "Action" key press to continue dialogue
	APlayerController* PlayerController = nullptr;
	if (IsValid(GetWorld()))
		PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(PlayerController))
		PlayerController->InputKey(EKeys::E, EInputEvent::IE_Pressed, 1, false);
}

UBorder* UDialogueBox::GetDialogueBorder() const
{
	return DialogueBorder;
}

UCanvasPanel* UDialogueBox::GetMainCanvasPanel() const
{
	return MainCanvasPanel;
}

USizeBox* UDialogueBox::GetMainSizeBox() const
{
	return MainSizeBox;
}

UTextBlock* UDialogueBox::GetDialogueTextBlock() const
{
	return DialogueTextBlock;
}

UTextBlock* UDialogueBox::GetSpeakerNameTextBlock() const
{
	return SpeakerNameTextBlock;
}

UOverlay* UDialogueBox::GetResponseOverlay() const
{
	return ResponseOverlay;
}

FText UDialogueBox::GetDialogueText() const
{
	return DialogueText;
}

FText UDialogueBox::GetSpeakerName() const
{
	return SpeakerName;
}

void UDialogueBox::SetDialogueText(const FText& NewText)
{
	DialogueText = NewText;
}

void UDialogueBox::SetSpeakerName(const FText& NewText)
{
	SpeakerName = NewText;
}
