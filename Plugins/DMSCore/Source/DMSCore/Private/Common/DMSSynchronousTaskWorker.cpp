#include "Common/DMSSynchronousTaskWorker.h"

void UDMSSynchronousTaskWorker::SetupTaskWorkerDelegate(const TArray<UObject*>& iContexts, const FOnTaskCompleted& inCompletedDelegate)
{
	Contexts=iContexts;
	CompletedDelegate=inCompletedDelegate;
}

void UDMSSynchronousTaskWorker::RunTaskWorker(bool iStopIfContFailed)
{
	CurrentContextIndex=0;
	StopIfContFailed = iStopIfContFailed;
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
		CloseTaskQueue(false);
		return;
	}

	if(Contexts.Num() == ++CurrentContextIndex)
		CloseTaskQueue(true);
	else 
		Work();
}


void UDMSSynchronousTaskWorker::OnAllTaskCompleted_Implementation(bool WorkerSucceeded)
{
	// do some cleanup
}

void UDMSSynchronousTaskWorker::CloseTaskQueue(bool WorkerSucceeded)
{
	OnAllTaskCompleted(WorkerSucceeded);
	CompletedDelegate.ExecuteIfBound(WorkerSucceeded);
	MarkAsGarbage();
}
