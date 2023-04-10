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

//	========================================
//			SOME COMMON KEYWORD TAGS
//	========================================

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_EffectKeyword_PlayCard)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Attribute_Resource)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Attribute_ActionPoint)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Attribute_HP)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Attribute_STR)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Attribute_INT)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Attribute_DEX)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_Attribute_SavedSkillBonus)


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

/*
 * Target is Left, definited value is right.
 */
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
	OSF_Default UMETA(DisplayName = "Notified Object"),
	OSF_OwningPlayer UMETA(DisplayName = "Notified Object's owner player"),
	OSF_SourceObj UMETA(DisplayName = "Seq -> Source Object"),
	OSF_SourceCtr UMETA(DisplayName = "Seq -> Source Controller"),
	OSF_Data UMETA(DisplayName = "Seq -> Data[Candidates] ? ( Working )"),
	OSF_Target UMETA(DisplayName = "Seq -> Target"),
	// Seq-> other nodes?
	// Seq-> EffectInstances?
	OSF_Custom UMETA(DisplayName = "Custom (UNUSABLE)"),
};

/**
 *
 */
UENUM()
enum class EDMSPresetTargetFlag : uint8
{
	PTF_Self UMETA(DisplayName = "Self"),
	PTF_OC UMETA(DisplayName = "Owning Controller"),
	PTF_Parent UMETA(DisplayName = "Follows Parent Sequence"),
	//PTF_Data UMETA(DisplayName = "From Data[PresetTarget]", ToolTip = "Use targets set by some other source like player action or game system."),
	PTF_None UMETA(DisplayName = "Don't Use Preset Target"),
	PTF_Effect UMETA(DisplayName = "Use targets generated by Effect Node."),
};

/**
 *	std::any 를 BP에 올려서 사용하기 위한 일종의 래퍼.
 *	게임내에 돌아다니는 데이터들의 타입 제한을 조금 더 느슨하게 해보고자 해서 도입
 *	지금 상태가 최선이 아닌거같긴 한데 다른 방법이 있을까에 대한 생각.=>
 *	좀 더 사용 가능한 타입을 제한하고 그냥 union이나 ue5에서 제공하는 비슷한것을 쓴다던지 
 *	특히 시리얼라이즈부분의 이슈가 있어서...
 */
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
	
	void CopyValue(UDMSDataObject* iObj) {this->AnyValue = iObj->AnyValue;}

};

//	위의 DataObject 를 모아서 관리하는 오브젝트
//	현재 FName을 Key로 쓰고 있는데 이런 것들을 전부 GPTag로 바꿀 까 생각 중. ( 계층형 태그 구조가 지원 하는 기능이 좀 매력적이라고 생각함. )
UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSDataObjectSet : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TMap<FGameplayTag, UDMSDataObject*> DataMap;

public:

	FORCEINLINE void SetData(const FGameplayTag& Tag, const std::any& Data, const bool& Inheriting = false) {
		UDMSDataObject* DataObject = NewObject<UDMSDataObject>();
		DataObject->Set(Data);
		DataObject->SetInheriting(Inheriting);
		DataMap.Add(Tag, DataObject);
	}

	FORCEINLINE void SetData(const FGameplayTag& Tag, std::any&& Data, const bool& Inheriting = false) {
		UDMSDataObject* DataObject = NewObject<UDMSDataObject>();
		DataObject->Set(std::move(Data));
		DataObject->SetInheriting(Inheriting);
		DataMap.Add(Tag, DataObject);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddData(const FGameplayTag& Tag, UDMSDataObject* Data, const bool& Inheriting = false) {
		if (Data == nullptr) return;
		Data->SetInheriting(Inheriting);
		DataMap.Add(Tag, Data);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool ContainData(const FGameplayTag& Tag) const {
		return DataMap.Contains(Tag);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UDMSDataObject* GetData(const FGameplayTag& Tag) {
		return ContainData(Tag) ? DataMap[Tag] : nullptr;
	}

	void Inherit(UDMSDataObjectSet* Parent, const bool& InheritAgain=false);

	// actually it's overwrite
	void Merge(UDMSDataObjectSet* iSet);

};






