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
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_EffectKeyword_Cost)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_EffectType_Activatable)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_EffectType_Effect)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_EffectType_Cost)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DMS_EffectType_Commit)

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
	T_Decision=0 UMETA(DisplayName = "Decision"),
	T_Before UMETA(DisplayName = "Before"),
	T_During UMETA(DisplayName = "During"),
	T_After UMETA(DisplayName = "After"),
	T_Null UMETA(DisplayName = "Null (Any)"),
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

/**
 * LEGACY 
 */
UENUM()
enum class EDMSObjectSelectorFlag : uint8
{
	OSF_Default UMETA(DisplayName = "Notified Object"),
	OSF_OwningPlayer UMETA(DisplayName = "Notified Object's owner player"),
	OSF_SourceObj UMETA(DisplayName = "Seq -> Source Object"),
	OSF_SourceCtr UMETA(DisplayName = "Seq -> Source Controller"),
	OSF_EffectNode UMETA(DisplayName = "Seq -> EffectNode -> Generate Condition Targets"),
	OSF_Target UMETA(DisplayName = "Seq -> Target"),
	// Seq-> other nodes?
	// Seq-> EffectInstances?
	OSF_Custom UMETA(DisplayName = "Custom ( Condition Object )"),
};

/**
 * LEGACY
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
	/**
	 * Store Real value.
	 */
	std::any AnyValue;

	/**
	 * Inheriting option. see UDMSDataObjectSet::Inherit
	 */
	bool Inheriting;
public:
	UDMSDataObject():Inheriting(false) {}

	/**
	 * Setter of AnyValue.
	 * std::forward???
	 */
	void Set(const std::any& iValue) { AnyValue = iValue;/*emplace*/ }
	void Set(std::any&& iValue) { AnyValue = std::move(iValue); }

	/**
	 * Getter of AnyValue with template type.
	 */
	template<typename T>
	T Get() { return std::any_cast<T>(AnyValue); }

	/**
	 * Check AnyValue is storing template type.
	 */
	template<typename T>
	bool TypeCheck() { return std::any_cast<T>(&AnyValue) != nullptr; }

	/**
	 * Getter of Inheriting.
	 */
	bool IsInheriting() {return Inheriting;}

	/**
	 * Setter of Inheriting.
	 */
	void SetInheriting(const bool& i) { Inheriting=i; }

	/**
	 * Copy Value only function.
	 */
	void CopyValue(UDMSDataObject* iObj) {this->AnyValue = iObj->AnyValue;}
};

//	위의 DataObject 를 모아서 관리하는 오브젝트
UCLASS(BlueprintType)
class PROJECTDMSGAME_API UDMSDataObjectSet : public UObject
{
	GENERATED_BODY()

protected:
	/**
	 * <Tag, data object> map. stores datas.
	 */
	UPROPERTY()
	TMap<FGameplayTag, UDMSDataObject*> DataMap;

public:

	/**
	 * Add data with std::any param. Instancing UDMSDataObject automatically.
	 * @param	Tag							Key.
	 * @param	Data						Value.
	 * @param	Inheriting					New data's inheriting option.
	 */
	FORCEINLINE void SetData(const FGameplayTag& Tag, const std::any& Data, const bool& Inheriting = false) {
		UDMSDataObject* DataObject = NewObject<UDMSDataObject>();
		DataObject->Set(Data);
		DataObject->SetInheriting(Inheriting);
		DataMap.Add(Tag, DataObject);
	}

	/**
	 * Add data with std::any param. Instancing UDMSDataObject automatically.
	 * @param	Tag							Key.
	 * @param	Data						Value.
	 * @param	Inheriting					New data's inheriting option.
	 */
	FORCEINLINE void SetData(const FGameplayTag& Tag, std::any&& Data, const bool& Inheriting = false) {
		UDMSDataObject* DataObject = NewObject<UDMSDataObject>();
		DataObject->Set(std::move(Data));
		DataObject->SetInheriting(Inheriting);
		DataMap.Add(Tag, DataObject);
	}

	/**
	 * Add data directly.
	 * @param	Tag							Key.
	 * @param	Data						Value.
	 * @param	Inheriting					New data's inheriting option.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddData(const FGameplayTag& Tag, UDMSDataObject* Data, bool Inheriting = false) {
		if (Data == nullptr) return;
		Data->SetInheriting(Inheriting);
		DataMap.Add(Tag, Data);
	}

	/**
	 * Check DataMap contains param key.
	 * @param	Tag							Searching key.
	 * @return	true if DataMap contains Tag.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool ContainData(const FGameplayTag& Tag) const {
		return DataMap.Contains(Tag);
	}

	/**
	 * Get data from DataMap
	 * @param	Tag							Searching key.
	 * @return	DataMap[Tag] : null ptr if No such key.
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UDMSDataObject* GetData(const FGameplayTag& Tag) {
		return ContainData(Tag) ? DataMap[Tag] : nullptr;
	}

	/**
	 * Templated Get validated data from DataMap 
	 * @param	Tag							Searching key.
	 * @return	true if DataMap contains Tag.
	 * @return	DataMap[Tag] : null ptr if No such key.
	 */
	template<typename T>
	bool GetValidDataValue(const FGameplayTag& Tag, T& outValue ) {
		bool rv = ContainData(Tag);
		if (rv) rv = rv && DataMap[Tag]->TypeCheck<T>();
		if (rv) outValue = DataMap[Tag]->Get<T>();
		return rv;
	}
	/**
	 * Inherit datas from param data set.
	 * @param	Parent						Parent data set. Data set will be copied from this.
	 * @param	InheritAgain				true if inherited data will be inherited again.
	 */
	void Inherit(UDMSDataObjectSet* Parent);


	/**
	 * Merge with param data set. temp) It's override for soon.
	 * @param	iSet						Merging data set. 
	 * ... some conflict option will be added.
	 */
	void Merge(UDMSDataObjectSet* iSet);

};

