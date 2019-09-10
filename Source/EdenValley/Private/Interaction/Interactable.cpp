// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction\Interactable.h"

// Sets default values
AInteractable::AInteractable() : Name(""), Action("")
{}

void AInteractable::Interact_Implementation(APlayerController * Controller)
{
}

FString AInteractable::GetInteractText() const
{
	return FString::Printf(TEXT("%s: Press F to %s"), *Name, *Action);
}