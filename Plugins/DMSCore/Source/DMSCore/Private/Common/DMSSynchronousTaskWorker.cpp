#include "Common/DMSSynchronousTaskWorker.h"

void UDMSSynchronousTaskWorker::SetupTaskWorkerDelegate(const TArray<UObject*>& iContexts, const FOnTaskCompleted& inCompletedDelegate)
{
	Contexts=iContexts;
	CompletedDelegate.BindLambda([=](bool Param){inCompletedDelegate.ExecuteIfBound(Param);});
}

void UDMSSynchronousTaskWorker::SetupTaskWorkerDelegate_Native(const TArray<UObject*>& iContexts, const FOnTaskCompletedNative& inCompletedDelegate)
{
	Contexts=iContexts;
	CompletedDelegate=inCompletedDelegate;
}

void UDMSSynchronousTaskWorker::RunTaskWorker(bool iStopIfContFailed)
{
	CurrentContextIndex=0;
	StopIfContFailed = iStopIfContFailed;

	if(Contexts.Num()==0)
	{
		CloseTaskWorker(true);
		return;
	}
	Work();
}

void UDMSSynchronousTaskWorker::Work_Implementation()
{
	//auto CurrentContext = GetCurrentContext();
	//Do something with CurrentContext
	//and return success or failure
	CompleteSingleTask(true);
}

void UDMSSynchronousTaskWorker::CompleteSingleTask(bool Succeeded)
{
	if(StopIfContFailed && !Succeeded){
		CloseTaskWorker(false);
		return;
	}

	if(Contexts.Num() == ++CurrentContextIndex)
		CloseTaskWorker(true);
	else 
		Work();
}


void UDMSSynchronousTaskWorker::OnAllTaskCompleted_Implementation(bool WorkerSucceeded)
{
	// do some cleanup
}

void UDMSSynchronousTaskWorker::CloseTaskWorker(bool WorkerSucceeded)
{
	OnAllTaskCompleted(WorkerSucceeded);
	CompletedDelegate.ExecuteIfBound(WorkerSucceeded);
	// TODO :: Improved GC Preventing
	//RemoveFromRoot();
	MarkAsGarbage();
}
