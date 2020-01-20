#pragma once
#include "SmlSocketIOTask.h"


namespace SmartLib
{
	class SocketTaskTest
	{
	public:
		static void Case1()
		{
			{
				//RefPtr<IFile*> file;
				//RefPtr<CircularBuffer> cbReceive;
				//RefPtr<TaskRunner> taskRunner;
				//RefPtr<IAsyncTask*> writer = RefPtr<IAsyncTask*>::Make(new SocketWriteAsyncTask(file, cbReceive, taskRunner));
				//writer.SetDispose(Disposer::DeletePtr<IAsyncTask>);
				//taskRunner->AddRunning(writer);
				//(**writer).RefSelf(writer.Ptr());
			}

		}


		static void Case0()
		{
			{
				RefPtr<IFile*> file;
				RefPtr<CircularBuffer> cbReceive;
				RefPtr<CircularBuffer> cbSend;
				RefPtr<TaskRunner> taskRunner;
				RefPtr<IAsyncTask*> reader = RefPtr<IAsyncTask*>::Make(new SocketIOAsyncTask{ file, cbReceive, cbSend, taskRunner });
				reader.SetDispose(Disposer::DeletePtr<IAsyncTask>);
				taskRunner->AddRunning(reader);
				//(**reader).RefSelf(reader.Ptr());
			}

		}
	};
}