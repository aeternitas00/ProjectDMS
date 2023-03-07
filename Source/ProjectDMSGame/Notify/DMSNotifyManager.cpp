// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/DMSNotifyManager.h"
#include "Effect/DMSEffectorInterface.h"
#include "Sequence/DMSSequence.h"

void UDMSNotifyManager::BroadCast(UDMSSequence* NotifyData,bool iChainable)
{
	for (auto Object : NotifyObjects)
	{
		Object->OnNotifyReceived(iChainable,NotifyData);
	}
}

void UDMSNotifyManager::RegisterNotifyObject(TScriptInterface<IDMSEffectorInterface> Object)
{
	NotifyObjects.Add(Object);
}
