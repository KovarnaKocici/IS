// Copyright 2001-2018 People Can Fly. All Rights Reserved.
#pragma once

#include "UObject/Class.h"
#include "UObject/Package.h"
#include "Utilities/ErrorHandling.h"

#define IS_VALID_ENUM_NAME(EnumType, EnumName) \
	IsValidEnumValue(#EnumType, EnumName.ToString())

#define ENUM_FROM_NAME(EnumType, EnumName) \
	GetEnumValueFromString<EnumType>(#EnumType, EnumName.ToString())

#define ENUM_TO_NAME(EnumType, EnumValue) \
	FName(*GetEnumValueToString(#EnumType, EnumValue))

#define ENUM_TO_STRING(EnumType, EnumValue) \
	GetEnumValueToString(#EnumType, EnumValue)

inline bool IsValidEnumValue(const FString& EnumName, const FString& String)
{
	return FindObject<UEnum>(ANY_PACKAGE, *EnumName, true) != nullptr;
}

template<typename EnumType>
inline FString GetEnumValueToString(const FString& Name, EnumType Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	VALIDATE(EnumPtr != nullptr, return "Invalid");

	return EnumPtr->GetNameStringByValue((int32)Value);
}

template <typename EnumType>
inline EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	VALIDATE(EnumPtr != nullptr, return EnumType(0));

	return (EnumType)EnumPtr->GetIndexByName(FName(*String), EGetByNameFlags::ErrorIfNotFound);
}

