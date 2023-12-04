// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "AC_Inventory.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOn_AddItem, UAC_Inventory*, Inventory, FDataTableRowHandle, Table, int, Slot, int, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShow_AddHud, FDataTableRowHandle, Table, int, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOn_Items_Swapped, int, Slot1, int, Slot2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOn_ItemRemoved);

UCLASS(ClassGroup = (Custom), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class BEYONDONLINE2_API UAC_Inventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Inventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
		FOn_AddItem On_AddItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
		FShow_AddHud Show_AddHud;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
		FOn_Items_Swapped On_ItemsSwapped;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Inventory")
		FOn_ItemRemoved On_ItemRemoved;

	//UFUNCTION(Client, Reliable)
	//	void OC_AddItem(UAC_Inventory* Inventory, FDataTableRowHandle Table, int Slot, int Quantity);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> Data_HotBar;


	//UFUNCTION(Server, Reliable, WithValidation)
	//	void Server_ChangeHotbarSlot(int SlotIndex, int NewSlot);

	UFUNCTION(BlueprintCallable)
	void Init_DataHotBar();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool isShop;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	int ContainerSize = 40;
};
