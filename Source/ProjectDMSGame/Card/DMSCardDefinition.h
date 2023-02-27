// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectDMS.h"
#include "Engine/DataAsset.h"
#include "Common/DMSCommons.h"
#include "Common/DMSTimingCondition.h"
#include "Effect/DMSEffectDefinition.h"
#include "DMSCardDefinition.generated.h"


/**
 *  Non mutatable Card's original data / definition.
 * 
 * 	========================================
 *
	카드 프로퍼티에 대해

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UCardFragment>> Fragments;
	형태로 프로퍼티를 파편화 시켜서 하는 구조 발견...

	에디터 상에서 어레이에 아이템을 넣는 순간 UCardFragment 하위 클래스의 새 인스턴스가 생기면서 값을 지정할 수 있게됨..
	-> 필요한 종류의 프로퍼티 클래스만 이어붙이기 가능. 

 *
 * =========================================
 * 
 * 최우선적으로 로딩할 데이터 어셋에서 참조하는 또다른 데이터는 어지간하면 레퍼런싱되어 같이 로딩되기에 데이터 어셋의 하위 데이터는 데이터 어셋화 하지 않고 UObj 형태로 만들고 연결해버리는듯.
 * 빡빡하게 SoftPtr 같은걸 써서 로딩 최적화가 필요한 부분도 아니고 ( 어짜피 카드 하나를 사용하면 거의 다 딸려와서 필요한 내용이기에 ) 
 * 뭔가 추가할것 같다면 또 다른 DataAsset 만들지말고 생각해 보는게 좋을듯 함.
 * (LyraPickup -> LyraInventory 참조)
 * 
 */
class UDMSEffect_Cost;
class UDMSEffectNode_TypeBehavior;


UCLASS(Blueprintable, BlueprintType, Const, ClassGroup = (Card))
class PROJECTDMSGAME_API UDMSCardDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// ----------------------------------------------------------------------------------------- //
	// 기본적인 카드 구성 요소들은 인게임내에서 변경 가능성에 대해 생각해봐야함.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText CardName;

	// Using Data registry Connect with type behavior?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FName DefaultCardType; // ENUM?

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	//TSubclassOf<UDMSEffectNode_TypeBehavior> DefaultCardType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText EffectText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText FlavorText;
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	//FBrush Image;
		
	// ----------------------------------------------------------------------------------------- //
	// DEPRECATED
	
	// UI 연동하는 부분과 연결 필요성 있음. ( 타겟등 플레이 전에 리버트 가능한 Status condition check 구현 )

	// Card's Effect.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	UDMSEffectSet* CardEffects;

	// Card's Play Cost -> visualizing info
	// 플레이 '전'에 선택해야 요소들에 대한 셀렉터들도 여기에
	// 사용 가능한 코스트의 종류가 여러개일 경우를 대비해 Node 대신 Set
	// 코스트 구현 방식에 대해 재고? (지불 가능 할지에 대한 예측이 가능해야 함 => 별도의 클래스화가 맞는가? Effect의 파생클래스 )
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	UDMSEffectSet* CostAndPlayCondition;

	// ----------------------------------------------------------------------------------------- //


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = CardEffect)
	TMap<FName,UDMSEffectSet*> CardEffectSets;
};
