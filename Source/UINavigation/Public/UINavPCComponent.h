﻿// Copyright (C) 2019 Gon�alo Marques - All Rights Reserved

#pragma once

#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Data/CountdownPhase.h"
#include "Data/InputRestriction.h"
#include "Data/InputType.h"
#include "Data/NavigationDirection.h"
#include "UINavPCComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UINAVIGATION_API UUINavPCComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	class APlayerController* PC;

	ENavigationDirection Direction = ENavigationDirection::None;

	float PreviousX = -1.f;
	float PreviousY = -1.f;

	ECountdownPhase CountdownPhase = ECountdownPhase::None;

	ENavigationDirection CallbackDirection;
	float TimerCounter = 0.f;

	/*************************************************************************/

	void TimerCallback();
	void SetTimer(ENavigationDirection NavigationDirection);

	void VerifyDefaultInputs();

	/**
	*	Searches all the Input Actions relevant to UINav plugin and saves them in a map
	*/
	void FetchUINavActionKeys();

	/**
	*	Returns the input type of the given key
	*
	*	@param Key The specified key	
	*	@return The input type of the given key
	*/
	EInputType GetKeyInputType(FKey Key);

	/**
	*	Returns the input type of the given action
	*
	*	@return The input type of the given action
	*/
	EInputType GetActionInputType(FString Action);

	/**
	*	Verifies if a new input type is being used
	*
	*	@param Key The specified key
	*	@param PressedKey The pressed key
	*/
	void VerifyInputTypeChangeByKey(FKey Key);

	/**
	*	Verifies if a new input type is being used
	*
	*	@param Action The specified action
	*/
	void VerifyInputTypeChangeByAction(FString Action);

	/**
	*	Notifies to the active UUINavWidget that the input type changed
	*
	*	@param NewInputType The new input type that is being used
	*/
	void NotifyInputTypeChange(EInputType NewInputType);

	virtual void Activate(bool bReset) override;
	void SetupInput();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UUINavPCComponent();

	//Indicates whether the player can navigate the widget
	UPROPERTY(BlueprintReadWrite, Category = UINavController)
		bool bAllowNavigation = true;

	UPROPERTY(BlueprintReadWrite, Category = UINavController)
		class UUINavWidget* ActiveWidget;

	UPROPERTY(BlueprintReadWrite, Category = UINavController)
		EInputType CurrentInputType = EInputType::Keyboard;

	/*
	Indicates whether navigation will occur periodically after the player
	holds down a navigation key
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UINavController)
		bool bChainNavigation = true;

	/*
	The amount of time the player needs to hold a key for the navigation to
	start occuring periodically
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UINavController)
		float InputHeldWaitTime = 0.5f;

	/*
	The amount of time that passes before the navigation chains further
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UINavController)
		float NavigationChainFrequency = 0.2f;

	/*
	The required value for an axis to be considered for rebinding
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UINavController)
		float RebindThreshold = 0.5f;

	TMap<FString, TArray<FKey>> KeyMap = TMap<FString, TArray<FKey>>();

	TArray<FString> PressedActions;

	/*
	Holds the key icons for gamepad
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UINavController)
		UDataTable* GamepadKeyIconData;
	/*
	Holds the key icons for mouse and keyboard
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UINavController)
		UDataTable* KeyboardMouseKeyIconData;

	/*
	Holds the key names for gamepad
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UINavController)
		UDataTable* GamepadKeyNameData;
	/*
	Holds the key names for mouse and keyboard
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UINavController)
		UDataTable* KeyboardMouseKeyNameData;

	/**
	*	Notifies the controller that a mouse is being used
	*/
	void NotifyMouseInputType();

	/**
	*	Notifies the controller that the given key was just pressed
	*
	*	@param PressedKey The pressed key
	*/
	void NotifyKeyPressed(FKey PressedKey);

	/**
	*	Notifies the controller that the given key was just released
	*
	*	@param ReleasedKey The released key
	*/
	void NotifyKeyReleased(FKey ReleasedKey);

	/**
	*	Indicates whether the pressed key is associated with the return action
	*
	*	@param PressedKey The pressed key
	*/
	bool IsReturnKey(FKey PressedKey);

	/**
	*	Executes a Menu Action by its name
	*
	*	@param Action The action's name
	*	@param bPressed Whether the action was pressed or released
	*/
	void ExecuteActionByName(FString Action, bool bPressed);

	/**
	*	Executes a Menu Action by its key
	*
	*	@param PressedKey The given key
	*	@param bPressed Whether the action was pressed or released
	*/
	void ExecuteActionByKey(FKey ActionKey, bool bPressed);

	/**
	*	Returns the action that contains the given key
	*
	*	@param PressedKey The given key
	*/
	FString FindActionByKey(FKey ActionKey);

	/**
	*	Called when an action key is pressed
	*
	*	@param ActionName The name of the action
	*/
	FReply OnActionPressed(FString ActionName);

	/**
	*	Called when an action key is released
	*
	*	@param ActionName The name of the action
	*/
	FReply OnActionReleased(FString ActionName);

	//Get first found key associated with the given input name
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = UINavController)
		FKey GetInputKey(FName ActionName, EInputRestriction InputRestriction);

	//Get first found Icon associated with the given input name
	//Will search the icon table
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = UINavController)
		class UTexture2D* GetInputIcon(FName ActionName, EInputRestriction InputRestriction);

	//Get first found name associated with the given input name
	//Will search the name table, if name can't be found will return the key's display name
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = UINavController)
		FString GetInputName(FName ActionName, EInputRestriction InputRestriction);

	//Get all keys associated with the input with the given name
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = UINavController)
		TArray<FKey> GetInputKeysFromName(FName InputName);

	void MenuUp();
	void MenuDown();
	void MenuLeft();
	void MenuRight();
	void MenuSelect();
	void MenuReturn();

	void MenuUpRelease();
	void MenuDownRelease();
	void MenuLeftRelease();

	void MouseInputWorkaround();

	void ClearTimer();

	void StartMenuUp();
	void StartMenuDown();
	void StartMenuLeft();
	void StartMenuRight();
	void MenuRightRelease();

	FORCEINLINE EInputType GetCurrentInputType() const { return CurrentInputType; }

	FORCEINLINE UUINavWidget* GetActiveWidget() const { return ActiveWidget; }

		
};
