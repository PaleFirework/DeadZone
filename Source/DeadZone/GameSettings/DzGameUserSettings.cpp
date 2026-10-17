// Fill out your copyright notice in the Description page of Project Settings.


#include "DzGameUserSettings.h"

float UDzGameUserSettings::GetMouseSpeed() const
{
	//返回鼠标速度
	return 15 + 40 * MouseSensitivity;
}