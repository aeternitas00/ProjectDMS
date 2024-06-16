// Copyright JeongWoo Lee


#include "Scenario/DMSScenarioData.h"
#include "Location/DMSLocationData.h"
#include "Logging/MessageLog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

//void UDMSScenarioData::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
//{
//	Super::PostEditChangeChainProperty(PropertyChangedEvent);
//
//	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
//
//    if (PropertyChangedEvent.Property->GetOwnerStruct() == FDMSConnectionData::StaticStruct())
//    {
//        auto temp = Connections;
//        Connections.Empty();
//       
//       for (auto& t : temp) Connections.Add(t);
//        if (OldConnections.Num() != Connections.Num())
//        {
//            Connections = std::move(OldConnections);
//            FNotificationInfo Info(FText::FromString("There is already a duplicated connection."));
//            Info.ExpireDuration = 3.0f;
//            FSlateNotificationManager::Get().AddNotification(Info);
//        }
//        else
//            OldConnections.Empty();
//    }
//    else if (PropertyName == GET_MEMBER_NAME_CHECKED(UDMSScenarioData, StartingLocationIndex))
//    {
//        if ( StartingLocationIndex >= Locations.Num() )
//        {
//            StartingLocationIndex = std::move(OldStartingLocationIndex);
//            FNotificationInfo Info(FText::FromString("Starting Location Index cannot be greater than or equal to the number of locations."));
//            Info.ExpireDuration = 3.0f;
//            FSlateNotificationManager::Get().AddNotification(Info);
//            
//        }
//    }
//}
//
//void UDMSScenarioData::PreEditChange(FProperty* PropertyAboutToChange)
//{
//    Super::PreEditChange(PropertyAboutToChange);
//
//    const FName PropertyName = PropertyAboutToChange->GetFName();
//
//    if (PropertyAboutToChange->GetOwnerStruct() == FDMSConnectionData::StaticStruct())
//    {
//        // Cache the current connections
//        OldConnections = Connections;
//    }
//    else if (PropertyName == GET_MEMBER_NAME_CHECKED(UDMSScenarioData, StartingLocationIndex))
//    {
//        OldStartingLocationIndex = StartingLocationIndex;
//    }
//}