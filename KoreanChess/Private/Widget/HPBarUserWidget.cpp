// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HPBarUserWidget.h"
#include "Components/ProgressBar.h"



void UHPBarUserWidget::ReactDamage(float Percent)
{
	HPBar->SetPercent(Percent);
	PlayAnimation(Appear);
}

bool UHPBarUserWidget::CheckReactingDamage()
{
	return IsAnimationPlaying(Appear);
}
