using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
    static class TestAwaitAsync
    {
        class RunAsyncStateMachine : IAsyncStateMachine
        {
            private int _state = 0;
            private AsyncTaskMethodBuilder<int> _asyncBuiler = AsyncTaskMethodBuilder<int>.Create();
            private TaskAwaiter<int> _awaiter;
            private RunAsyncStateMachine _self;


            private int sum;
            private int delta;
            private int delay;

            public Task<int> Start()
            {
                _self = this;
                _asyncBuiler.Start(ref _self);
                return _asyncBuiler.Task;
            }
            public void MoveNext()
            {
                switch(_state)
                {
                    case 0:
                        {
                            _state = 1;

                            sum = 1_000;
                            delta = 3_000;
                            delay = sum;
                            Console.WriteLine("00000000000000000000000");
                            Console.WriteLine($"{nameof(delay)}={delay}, {nameof(sum)}={sum}");
                            sum += delta;
                            _awaiter = MyDelayReturn(sum).GetAwaiter();
                            if(_awaiter.IsCompleted)
                            {
                                goto case 1;
                            }
                            else
                            {
                                MySleep(3600, 1000);
                                _self = this;
                                _asyncBuiler.AwaitUnsafeOnCompleted(ref _awaiter, ref _self);
                            }
                        }
                        break;
                    case 1:
                        {
                            _state = 2;

                            delay = _awaiter.GetResult();
                            Console.WriteLine("1111111111111111111111");
                            Console.WriteLine($"{nameof(delay)}={delay}, {nameof(sum)}={sum}");
                            sum += delta;
                            _awaiter = MyDelayReturn(sum).GetAwaiter();
                            if(_awaiter.IsCompleted)
                            {
                                goto case 2;
                            }
                            else
                            {
                                MySleep(3600, 1000);
                                _self = this;
                                _asyncBuiler.AwaitUnsafeOnCompleted(ref _awaiter, ref _self);
                            }

                        }
                        break;
                    case 2:
                        {
                            _state = 3;

                            delay = _awaiter.GetResult();
                            Console.WriteLine("22222222222222222222222");
                            Console.WriteLine($"{nameof(delay)}={delay}, {nameof(sum)}={sum}");
                            sum += delta;
                            _awaiter = MyDelayReturn(sum).GetAwaiter();
                            if(_awaiter.IsCompleted)
                            {
                                goto case 3;
                            }
                            else
                            {
                                MySleep(3600, 1000);
                                _self = this;
                                _asyncBuiler.AwaitUnsafeOnCompleted(ref _awaiter, ref _self);
                            }
                        }
                        break;
                    case 3:
                        {
                            _state = 4;

                            delay = _awaiter.GetResult();
                            Console.WriteLine("33333333333333333333333333");
                            Console.WriteLine($"{nameof(delay)}={delay}, {nameof(sum)}={sum}");
                            sum += delta;
                            Console.WriteLine();
                            _asyncBuiler.SetResult(sum);
                        }
                        break;
                }
                //throw new NotImplementedException();
            }

            public void SetStateMachine(IAsyncStateMachine stateMachine)
            {
                throw new NotImplementedException();
            }
        }

        public static Task<int> RunByStateMachine()
        {
            var statMachine = new RunAsyncStateMachine();
            return statMachine.Start();
        }
        public static async Task<int> RunByAsyncAwait()
        {
            int sum = 1_000;
            int delta = 3_000;
            int delay = sum;

            Console.WriteLine("00000000000000000000000");
            Console.WriteLine($"{nameof(delay)}={delay}, {nameof(sum)}={sum}");
            sum += delta;
            delay = await MyDelayReturn(sum);


            Console.WriteLine("1111111111111111111111");
            Console.WriteLine($"{nameof(delay)}={delay}, {nameof(sum)}={sum}");
            sum += delta;
            delay = await MyDelayReturn(sum);


            Console.WriteLine("22222222222222222222222");
            Console.WriteLine($"{nameof(delay)}={delay}, {nameof(sum)}={sum}");
            sum += delta;
            delay = await MyDelayReturn(sum);


            Console.WriteLine("33333333333333333333333333");
            Console.WriteLine($"{nameof(delay)}={delay}, {nameof(sum)}={sum}");
            sum += delta;
            Console.WriteLine();
            
            return sum;
        }

        public static void MySleep(int loopCount, int milSec)
        {
            for(int ii = 0; ii < loopCount; ++ ii)
            {
                Thread.Sleep(milSec);
            }
            return;
        }
        public static Task<int> MyDelayReturn(int milSec)
        {
            Task<int> tt = Task.Factory.StartNew<int>(
                LocalDelayProc,
                TaskCreationOptions.LongRunning
              );
            return tt;


            int LocalDelayProc()
            {
                Console.WriteLine($"i am sleeping for {milSec} milSec.  thread id is {Thread.CurrentThread.ManagedThreadId}.");
                MySleep((milSec + 999)/1000, 1000);
                return milSec;
            }
        }
    }
}
