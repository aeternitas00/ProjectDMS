// Fill out your copyright notice in the Description page of Project Settings.


#include "Location/DMSLocatableInterface.h"
#include "Location/DMSLocationBase.h"
#include "Common/DMSSpawnableDataBase.h"
// Add default functionality here for any IDMSLocatableInterface functions that are not pure virtual.

ADMSLocationBase* IDMSLocatableInterface::GetCurrentLocation_Implementation()
{
    auto ThisActor = Cast<ADMSSpawnableBase>(this);
    if (!ThisActor) return nullptr; // Custom implement required
    if(ThisActor->IsA<ADMSLocationBase>()) return Cast<ADMSLocationBase>(ThisActor);
    auto Container = ThisActor->GetOwningContainer();
    auto ContainerOwnerActor = Container ? Container->GetOwner() : nullptr;
    return ContainerOwnerActor && ContainerOwnerActor->Implements<UDMSLocatableInterface>() ? Execute_GetCurrentLocation(ContainerOwnerActor) : nullptr;
}

int IDMSLocatableInterface::GetDistanceWith_Implementation(const TScriptInterface<IDMSLocatableInterface>& OtherObject)
{
    UObject* ThisObj = Cast<UObject>(this);

    if (ThisObj == nullptr) return -1;

	ADMSLocationBase* CurrentLoc = Execute_GetCurrentLocation(ThisObj);
	ADMSLocationBase* TargetLoc = OtherObject->Execute_GetCurrentLocation(OtherObject.GetObject());

    if(CurrentLoc==nullptr || TargetLoc == nullptr) return -1;

    if (CurrentLoc == TargetLoc) return 0;

    // 거리 구하기 BFS
    int Distance = 0;

    TArray<ADMSLocationBase*> VisitedNodes;
    TArray<ADMSLocationBase*> CurrentNodes;
    TArray<ADMSLocationBase*> NextNodes;

    // 시작 노드 추가
    CurrentNodes.Add(CurrentLoc);

    while (CurrentNodes.Num() > 0)
    {
        // 현재 노드들을 방문한 노드 리스트에 추가
        for (ADMSLocationBase* VisitedNode : CurrentNodes)  {
            VisitedNodes.AddUnique(VisitedNode);
        }

        // 다음 레벨의 노드를 찾기 위해 현재 노드들의 이웃 노드를 찾음
        for (ADMSLocationBase* CurrentNode : CurrentNodes)
        {
            for (ADMSLocationBase* NextNode : CurrentNode->ConnectingLocations)
            {
                // 이미 방문한 노드인 경우 패스
                if (VisitedNodes.Contains(NextNode))
                {
                    continue;
                }

                // 타겟 노드를 찾은 경우 거리 반환
                if (NextNode == TargetLoc) {
                    return Distance + 1;
                }

                // 이웃 노드를 다음 레벨 노드 리스트에 추가
                NextNodes.AddUnique(NextNode);
            }
        }

        // 다음 레벨 노드 리스트를 현재 노드 리스트로 변경
        CurrentNodes = NextNodes;
        NextNodes.Empty();

        // 거리 1 증가
        Distance++;
    }

    // 타겟 노드를 찾지 못한 경우 무한대 반환
    return TNumericLimits<int>::Max();
}

bool IDMSLocatableInterface::LocatingTo_Implementation(ADMSLocationBase* TargetLocation)
{
    UObject* ThisObj = Cast<UObject>(this);

    if (ThisObj==nullptr || !ThisObj->Implements<UDMSLocatableInterface>() ) return false;

    TScriptInterface<IDMSLocatableInterface> Interface(ThisObj);

    return ADMSLocationBase::MoveActorToDMSLocation(TargetLocation,Interface);
}
