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
				RefPtr<IFile*> file;
				RefPtr<CircularBuffer> cbReceive;
				RefPtr<TaskRunner> taskRunner;
				RefPtr<IAsyncTask*> writer = RefPtr<IAsyncTask*>::Make(new SocketWriteAsyncTask(file, cbReceive, taskRunner));
				writer.SetDispose(Disposer::DeletePtr<IAsyncTask>);
				(**writer).RefSelf(writer.Ptr());
			}

		}


		static void Case0()
		{
			{
				RefPtr<IFile*> file;
				RefPtr<CircularBuffer> cbReceive;
				RefPtr<TaskRunner> taskRunner;
				RefPtr<IAsyncTask*> reader = RefPtr<IAsyncTask*>::Make(new SocketReadAsyncTask(file, cbReceive, taskRunner));
				reader.SetDispose(Disposer::DeletePtr<IAsyncTask>);
				(**reader).RefSelf(reader.Ptr());
			}

		}
	};
}