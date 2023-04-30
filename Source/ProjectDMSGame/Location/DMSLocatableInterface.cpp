// Fill out your copyright notice in the Description page of Project Settings.


#include "Location/DMSLocatableInterface.h"
#include "Location/DMSLocationBase.h"
// Add default functionality here for any IDMSLocatableInterface functions that are not pure virtual.

int IDMSLocatableInterface::GetDistanceWith_Implementation(const TScriptInterface<IDMSLocatableInterface>& OtherObject)
{
    UObject* ThisObj = Cast<UObject>(this);

    if (ThisObj == nullptr) return -1;

	ADMSLocationBase* CurrentLoc = Execute_GetCurrentLocation(ThisObj);
	ADMSLocationBase* TargetLoc = OtherObject->Execute_GetCurrentLocation(OtherObject.GetObject());

    if(CurrentLoc==nullptr || TargetLoc == nullptr) return -1;

    if (CurrentLoc == TargetLoc) return 0;
    // �Ÿ� ���ϱ�
    int Distance = 0;

    TArray<ADMSLocationBase*> VisitedNodes;
    TArray<ADMSLocationBase*> CurrentNodes;
    TArray<ADMSLocationBase*> NextNodes;

    // ���� ��� �߰�
    CurrentNodes.Add(CurrentLoc);

    while (CurrentNodes.Num() > 0)
    {
        // ���� ������ �湮�� ��� ����Ʈ�� �߰�
        for (ADMSLocationBase* VisitedNode : CurrentNodes)
        {
            VisitedNodes.AddUnique(VisitedNode);
        }

        // ���� ������ ��带 ã�� ���� ���� ������ �̿� ��带 ã��
        for (ADMSLocationBase* CurrentNode : CurrentNodes)
        {
            for (ADMSLocationBase* NextNode : CurrentNode->ConnectingLocations)
            {
                // �̹� �湮�� ����� ��� �н�
                if (VisitedNodes.Contains(NextNode))
                {
                    continue;
                }

                // Ÿ�� ��带 ã�� ��� �Ÿ� ��ȯ
                if (NextNode == TargetLoc)
                {
                    return Distance + 1;
                }

                // �̿� ��带 ���� ���� ��� ����Ʈ�� �߰�
                NextNodes.AddUnique(NextNode);
            }
        }

        // ���� ���� ��� ����Ʈ�� ���� ��� ����Ʈ�� ����
        CurrentNodes = NextNodes;
        NextNodes.Empty();

        // �Ÿ� 1 ����
        Distance++;
    }

    // Ÿ�� ��带 ã�� ���� ��� ���Ѵ� ��ȯ
    return TNumericLimits<int>::Max();
}

bool IDMSLocatableInterface::LocatingTo_Implementation(ADMSLocationBase* TargetLocation)
{
    UObject* ThisObj = Cast<UObject>(this);

    if (ThisObj==nullptr || !ThisObj->Implements<UDMSLocatableInterface>() ) return false;

    TScriptInterface<IDMSLocatableInterface> Interface(ThisObj);

    if ( INDEX_NONE != TargetLocation->ActorsOnLocation.Find(Interface))
        return false;

    ADMSLocationBase::MoveActorToDMSLocation(TargetLocation,Interface);

    return true;
}
