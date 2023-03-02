// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 	========================================
 *
 *	DMS_CORE_MODULE
 *
 *	ENUMS and Data Object that used often in DMS.
 *
 *	========================================
 */

#include "ProjectDMS.h"
#include <any>
#include "DMSCommons.generated.h"

/**
 * 
 */

class UDMSAttributeComponent;
class UDMSSequence;

UENUM(BlueprintType)
enum class EDMSModifierType : uint8
{
	MT_Add UMETA(DisplayName = "Add"),
	MT_Sub UMETA(DisplayName = "Substract"),
	MT_Set UMETA(DisplayName = "Set (Override)"),
	MT_Div UMETA(DisplayName = "Divide"),
	MT_Mul UMETA(DisplayName = "Multiply"),
};

UENUM(/*BlueprintType*/)
enum class EDMSTimingFlag : uint8
{
	T_Before UMETA(DisplayName = "Before"),
	T_During UMETA(DisplayName = "During"),
	T_After UMETA(DisplayName = "After"),
	T_Null UMETA(DisplayName = "NULL (Undefined)"),
};

UENUM(/*BlueprintType*/)
enum class EDMSComparisonOperator : uint8
{
	BO_Less UMETA(DisplayName = "<"),
	BO_LessEqual UMETA(DisplayName = "<="),
	BO_Equal UMETA(DisplayName = "=="),
	BO_GreaterEqual UMETA(DisplayName = ">="),
	BO_Greater UMETA(DisplayName = ">")
};

UENUM()
enum class EDMSObjectSelectorFlag : uint8
{
	OSF_Default UMETA(DisplayName = "Default ( Timing : Notified Object , State : No check )"),
	OSF_SourceObj UMETA(DisplayName = "Seq -> Source Object"),
	OSF_SourceCtr UMETA(DisplayName = "Seq -> Source Controller"),
	OSF_Data UMETA(DisplayName = "Seq -> Data[Candidates] ? ( Working )"),
	OSF_Target UMETA(DisplayName = "Seq -> Target"),
	// Seq-> other nodes?
	// Seq-> EffectInstances?
	OSF_Custom UMETA(DisplayName = "Custom (UNUSABLE)"),
};

UENUM()
enum class EDMSPresetTargetFlag : uint8
{
	PTF_Self UMETA(DisplayName = "Self"),
	PTF_OC UMETA(DisplayName = "Owning Controller"),
	PTF_Data UMETA(DisplayName = "From Data[PresetTarget]"),
	PTF_None UMETA(DisplayName = "Don't Use Preset Target"),
	PTF_Custom UMETA(DisplayName = "Custom (UNUSABLE)"),
};

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSDataObject : public UObject
{
	GENERATED_BODY()

protected:
	std::any AnyValue;

	bool Inheriting;
public:
	UDMSDataObject():Inheriting(false) {}

	// std::foward?
	//
	void Set(const std::any& iValue) { AnyValue = iValue;/*emplace*/ }
	void Set(std::any&& iValue) { AnyValue = std::move(iValue); }


	template<typename T>
	T Get() { return std::any_cast<T>(AnyValue); }

	//std::any Get() { return AnyValue; }

	template<typename T>
	bool TypeCheck() { return std::any_cast<T>(&AnyValue) != nullptr; }

	bool IsInheriting() {return Inheriting;}
	void SetInheriting(const bool& i) { Inheriting=i; }
};

UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSDataObjectSet : public UObject
{
	GENERATED_BODY()

protected:
	TMap<FName, UDMSDataObject*> DataMap;

public:

	// T&& , forward?
	FORCEINLINE void SetData(FName Name, const std::any& Data, const bool& Inheriting = false) {
		UDMSDataObject* DataObject = NewObject<UDMSDataObject>();
		DataObject->Set(Data);
		DataObject->SetInheriting(Inheriting);
		DataMap.Add(Name, DataObject);
	}

	FORCEINLINE void SetData(FName Name, std::any&& Data, const bool& Inheriting = false) {
		UDMSDataObject* DataObject = NewObject<UDMSDataObject>();
		DataObject->Set(std::move(Data));
		DataObject->SetInheriting(Inheriting);
		DataMap.Add(Name, DataObject);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddData(FName Name, UDMSDataObject* Data,const bool& Inheriting=false) { 
		if(Data==nullptr) return;
		Data->SetInheriting(Inheriting); 
		DataMap[Name] = Data; 
	}

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE bool ContainData(const FName& Name) const { return DataMap.Contains(Name); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UDMSDataObject* GetData(/*const*/FName/*&*/ Name) { return ContainData(Name) ? DataMap[Name] : nullptr; }

	void Inherit(UDMSDataObjectSet* Parent, const bool& InheritAgain=false);

};






