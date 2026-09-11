// YaSolo
#include "UserInterface/QuestSystem/ScoreWidget.h"

#include "Components/TextBlock.h"
#include "QuestSystem/OrderSubSystem.h"


void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>();
	if (OrderSubSystem)
	{
		OrderSubSystem->OnScoreChanged.AddDynamic(this, &UScoreWidget::HandleScoreChanged);
	}
	
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(OrderSubSystem->GetScore()));
	}
}

void UScoreWidget::NativeDestruct()
{
	if (UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>())
	{
		OrderSubSystem->OnScoreChanged.RemoveDynamic(this, &UScoreWidget::HandleScoreChanged);
	}
	
	Super::NativeDestruct();
}

void UScoreWidget::HandleScoreChanged(int32 NewScore)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(NewScore));
	}
}
