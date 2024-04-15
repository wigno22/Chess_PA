// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h" 
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "MyWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API UMyWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	 UPROPERTY( meta = (BindWidget))
	 UButton* Button; 

	 UPROPERTY(meta = (BindWidget)) 
	 UTextBlock* Text;

	 UFUNCTION(BlueprintCallable)
	 void HandleButtonClick();
};
