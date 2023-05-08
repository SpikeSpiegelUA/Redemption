// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\DialogueBox.h"

bool UDialogueBox::Initialize()
{
	const bool bSuccess = Super::Initialize();
	DialogueTextBlock->TextDelegate.BindUFunction(this, "GetDialogueText");
	SpeakerNameTextBlock->TextDelegate.BindUFunction(this, "GetSpeakerName");
	if (ContinueButton)
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
	if (GetWorld())
		PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
		PlayerController->InputKey(EKeys::E, EInputEvent::IE_Pressed, 1, false);
}

UBorder* UDialogueBox::GetDialogueBorder()
{
	return DialogueBorder;
}

UCanvasPanel* UDialogueBox::GetMainCanvasPanel()
{
	return MainCanvasPanel;
}

USizeBox* UDialogueBox::GetMainSizeBox()
{
	return MainSizeBox;
}

UTextBlock* UDialogueBox::GetDialogueTextBlock()
{
	return DialogueTextBlock;
}

UTextBlock* UDialogueBox::GetSpeakerNameTextBlock()
{
	return SpeakerNameTextBlock;
}

UOverlay* UDialogueBox::GetResponseOverlay()
{
	return ResponseOverlay;
}

FText UDialogueBox::GetDialogueText()
{
	return DialogueText;
}

FText UDialogueBox::GetSpeakerName()
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
