// YaSolo
#include "UserInterface/Inventory/HighlightWidget.h"
#include "Components/TextBlock.h"
#include "Items/ItemBase.h"

void UHighlightWidget::UpdateWidget(const UItemBase* SelectedItem) const
{
	NameText->SetText(SelectedItem->DescriptiveText.DisplayName);
}

void UHighlightWidget::PlayFadeIn()
{
	if (FadeInAnim)
	{
		bIsFadingAnimReverse = false;
		PlayAnimation(FadeInAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	}
}

void UHighlightWidget::PlayFadeOut()
{
	if (FadeInAnim)
	{
		bIsFadingAnimReverse = true;
		PlayAnimation(FadeInAnim, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
	}
}

void UHighlightWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (bIsFadingAnimReverse && Animation == FadeInAnim)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}
