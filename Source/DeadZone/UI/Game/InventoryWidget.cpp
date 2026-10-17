// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "CommonTextBlock.h"
#include "ItemFrameWidget.h"
#include "Components/UniformGridPanel.h"
#include "DeadZone/Components/PackageComponent.h"
#include "DeadZone/Sence/SceneStorageActor.h"

void UInventoryWidget::HandleItemFrameLeftClicked(UItemFrameWidget* ClickedFrame)
{
	
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	for (UWidget* Child : ItemGrid->GetAllChildren())
	{
		if (UItemFrameWidget* ItemFrame = Cast<UItemFrameWidget>(Child))
		{
			ItemFrame->SetInventoryWidget(this);
		}
	}
}

void UInventoryWidget::SetText(const FText& InText)
{
	if (TextBlock)
	{
		TextBlock->SetText(InText);
	}
}

bool UInventoryWidget::AddItem(int32 ID, int32 Amount)
{
	if (ItemGrid)
	{
		for (UWidget* Child : ItemGrid->GetAllChildren())
		{
			if (UItemFrameWidget* ItemFrame = Cast<UItemFrameWidget>(Child))
			{
				if (ItemFrame->IsEmpty())
				{
					ItemFrame->SetItem(ID, Amount);
					return true;
				}
			}
		}
	}
	return false;
}

void UInventoryWidget::AddNearItemActor(ASceneItemActor* ItemActor)
{
	if (ItemGrid)
	{
		for (UWidget* Child : ItemGrid->GetAllChildren())
		{
			if (UItemFrameWidget* ItemFrame = Cast<UItemFrameWidget>(Child))
			{
				if (ItemFrame->IsEmpty())
				{
					ItemFrame->SetItem(ItemActor);
					return;
				}
			}
		}
	}
}

void UInventoryWidget::RemoveNearItemActor(ASceneItemActor* ItemActor)
{
	if (ItemGrid)
	{
		for (UWidget* Child : ItemGrid->GetAllChildren())
		{
			if (UItemFrameWidget* ItemFrame = Cast<UItemFrameWidget>(Child))
			{
				if (ItemFrame->GetSceneItemActor() == ItemActor)
				{
					ItemFrame->RemoveItem();
					return;
				}
			}
		}
	}
}

void UInventoryWidget::SetNear(bool IsNear)
{
	IsNearWidget = IsNear;
}

void UInventoryWidget::SetStorage(bool IsStorage)
{
	IsStorageWidget = IsStorage;
}

void UInventoryWidget::InitItemGrid(TMap<int, FInventorySlot> Items)
{
	if (!ItemGrid) return;
	
	const int32 ChildCount = ItemGrid->GetChildrenCount();
	
	for (int32 i = 0; i < ChildCount; ++i)
	{
		UWidget* ChildWidget = ItemGrid->GetChildAt(i);
		
		if (UItemFrameWidget* ItemFrame = Cast<UItemFrameWidget>(ChildWidget))
		{
			ItemFrame->RemoveItem();
		}
	}
	
	for (const TPair<int32, FInventorySlot>& Pair : Items)
	{
		const int32 SlotIndex = Pair.Key;
		const FInventorySlot& SlotData = Pair.Value;
		
		if (SlotIndex >= 0 && SlotIndex < ChildCount)
		{
			UWidget* ChildWidget = ItemGrid->GetChildAt(SlotIndex);
			if (UItemFrameWidget* ItemFrame = Cast<UItemFrameWidget>(ChildWidget))
			{
				ItemFrame->InitItemFrame(SlotData.ItemID, SlotData.Count);
			}
		}
	}
}

void UInventoryWidget::Save(ASceneStorageActor* StorageActor)
{
	TMap<int32, FInventorySlot>& StorageItems = StorageActor->GetItems();
	StorageItems.Empty();
	
	const int32 ChildCount = ItemGrid->GetChildrenCount();
	for (int32 i = 0; i < ChildCount; ++i)
	{
		UWidget* ChildWidget = ItemGrid->GetChildAt(i);
		
		if (UItemFrameWidget* ItemFrame = Cast<UItemFrameWidget>(ChildWidget))
		{
			if (!ItemFrame->IsEmpty())
			{
				FInventorySlot InventorySlot;

				InventorySlot.ItemID = ItemFrame->GetActorID();
				InventorySlot.Count = ItemFrame->GetAmount();

				StorageItems.Add(i, InventorySlot);
			}
		}
	}
}

int32 UInventoryWidget::ConsumeItemsByID(int32 ID, int32 NeedAmount)
{
	if (!ItemGrid || NeedAmount <= 0) return 0;
	int32 AmountLeftToConsume = NeedAmount;
	
	const int32 ChildCount = ItemGrid->GetChildrenCount();
	
	for (int32 i = 0; i < ChildCount; ++i)
	{
		if (AmountLeftToConsume <= 0) break;

		UWidget* ChildWidget = ItemGrid->GetChildAt(i);
		UItemFrameWidget* ItemFrame = Cast<UItemFrameWidget>(ChildWidget);

		if (ItemFrame && !ItemFrame->IsEmpty() && ItemFrame->GetActorID() == ID)
		{
			int32 CurrentFrameAmount = ItemFrame->GetAmount();

			if (CurrentFrameAmount <= AmountLeftToConsume)
			{
				AmountLeftToConsume -= CurrentFrameAmount;
				ItemFrame->RemoveItem();
			}
			else
			{
				int32 NewAmount = CurrentFrameAmount - AmountLeftToConsume;
				
				ItemFrame->SetAmount(NewAmount); 
                
				AmountLeftToConsume = 0;
			}
		}
	}
	return NeedAmount - AmountLeftToConsume;
}


void UInventoryWidget::AddItemFrames(int32 Count)
{
	if (!ItemGrid)
	{
		return;
	}

	if (!ItemGrid || !ItemFrameWidgetClass)
	{
		return;
	}

	constexpr int32 ColumnCount = 5;

	for (int32 i = 0; i < Count; ++i)
	{
		UItemFrameWidget* ItemFrame =
			CreateWidget<UItemFrameWidget>(
				GetWorld(),
				ItemFrameWidgetClass
			);

		if (!ItemFrame)
		{
			continue;
		}

		ItemFrame->SetInventoryWidget(this);

		const int32 Index = ItemGrid->GetChildrenCount();

		ItemGrid->AddChildToUniformGrid(
			ItemFrame,
			Index / ColumnCount,
			Index % ColumnCount
		);
	}
}


