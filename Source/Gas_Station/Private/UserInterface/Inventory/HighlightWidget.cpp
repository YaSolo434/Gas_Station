// YaSolo
#include "UserInterface/Inventory/HighlightWidget.h"
#include "Components/TextBlock.h"
#include "Items/ItemBase.h"
#include "Items/ItemBase.h"

void UHighlightWidget::UpdateWidget(const UItemBase* SelectedItem) const
{
	NameText->SetText(SelectedItem->DescriptiveText.DisplayName);
}
