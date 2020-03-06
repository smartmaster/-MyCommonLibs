using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
    class TestAsyncIOTask
    {
        public static Task Case1()
        {
            AsyncMethodStateMachine stateMachine = new AsyncMethodStateMachine();
            return stateMachine.Start();
        }
        class AsyncMethodStateMachine : IAsyncStateMachine
        {
            int _state = 1;
            AsyncTaskMethodBuilder _builder = new AsyncTaskMethodBuilder();
            TaskAwaiter _awaiter;
            AsyncMethodStateMachine _self;

            const string fileName = @"f:\000-TestAsyncIOTask.txt";
            const int BUFFER_SIZE = 16 * 1024;
            byte[] buffer;// = new byte[BUFFER_SIZE * 4];
            FileStream fs;

            public Task Start()
            {
                _self = this;
                _builder.Start(ref _self);
                return _builder.Task;
            }
            public void MoveNext()
            {
                switch(_state)
                {
                    case 1:
                        {
                            buffer = new byte[BUFFER_SIZE * 128];
                            Random ran = new Random();
                            ran.NextBytes(buffer);
                            fs = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.None, BUFFER_SIZE, FileOptions.Asynchronous);
                            //await fs.WriteAsync(buffer, 0, buffer.Length);
                            _awaiter = fs.WriteAsync(buffer, 0, buffer.Length).GetAwaiter();
                            if(_awaiter.IsCompleted)
                            {
                                goto case 2;
                            }
                            else
                            {
                                _state = 2;
                                _self = this;
                                _builder.AwaitUnsafeOnCompleted(ref _awaiter, ref _self);
                            }
                        }
                        break;
                    case 2:
                        {
                            Console.WriteLine("Write finished!");
                            fs?.Dispose();
                            _builder.SetResult();
                        }
                        break;
                }
            }

            public void SetStateMachine(IAsyncStateMachine stateMachine)
            {
                throw new NotImplementedException();
            }
        }
        public static async Task Case0()
        {
            const string fileName = @"f:\000-TestAsyncIOTask.txt";
            const int BUFFER_SIZE = 16 * 1024;
            byte[] buffer = new byte[BUFFER_SIZE * 4];
            using (FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.None, BUFFER_SIZE, FileOptions.Asynchronous))
            {
                await fs.WriteAsync(buffer, 0, buffer.Length);
                Console.WriteLine("Write finished!");
            }
        }
    }
}
