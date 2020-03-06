using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
    static class TestAsyncMethodBuilder
    {
        public static int MySleep(int loop, int milsec)
        {
            for(int ii = 0; ii < loop; ++ ii)
            {
                Thread.Sleep(milsec);
            }
            return (loop * milsec);
        }
        public static Task<int> MyDelayAsync(int loop, int sleep)
        {

            Func<int> func = new Func<int>(MyLocalFunc);

            Task<int> ti = Task.Factory.StartNew(
                func, 
                TaskCreationOptions.LongRunning);
            return ti;

            int MyLocalFunc()
            {
                int rr = MySleep(loop, sleep);
                return rr;
            }
        }

        public static void PrintLine(StringBuilder sb, char ch, int count)
        {
            sb.Clear();
            sb.Append(ch, count);
            Console.WriteLine(sb.ToString());
        }
        public static async Task<int> MethodAutoAsync()
        {
            char ch = '1';
            const int count = 20;
            StringBuilder sb = new StringBuilder();
            PrintLine(sb, ch, count);
            Console.WriteLine("Begin async method");
            Console.WriteLine();
            int loop = 1;
            const int delta = 1;
            const int milsec = 1000;
            int rr = 0;
            rr += await MyDelayAsync(loop, milsec);

            ++ch;
            PrintLine(sb, ch, count);
            Console.WriteLine($"The result is {rr}");
            Console.WriteLine();
            loop += delta;
            rr += await MyDelayAsync(loop, milsec);


            ++ch;
            PrintLine(sb, ch, count);
            Console.WriteLine($"The result is {rr}");
            Console.WriteLine();
            loop += delta;
            rr += await MyDelayAsync(loop, milsec);

            ++ch;
            PrintLine(sb, ch, count);
            Console.WriteLine($"The result is {rr}");
            Console.WriteLine("End async method");
            Console.WriteLine();

            return rr;
        }

        #region mannually composed method 
        class MethodManualAsyncStateMachine : IAsyncStateMachine
        {
            int _state = 1;
            AsyncTaskMethodBuilder<int> _atmBuiler = AsyncTaskMethodBuilder<int>.Create();
            TaskAwaiter<int> _awaiter;
            MethodManualAsyncStateMachine _self;

            bool _debgugToSleep;

            char ch = '1';
            const int count = 20;
            StringBuilder sb = new StringBuilder();
            int loop = 1;
            const int delta = 1;
            const int milsec = 1000;
            int rr = 0;

            public MethodManualAsyncStateMachine(bool debgugToSleep)
            {
                _debgugToSleep = debgugToSleep;
            }

            public Task<int> Start()
            {
                MethodManualAsyncStateMachine self = this;
                _atmBuiler.Start(ref self);
                return _atmBuiler.Task;
            }

            public void MoveNext()
            {
                switch(_state)
                {
                    case 1:
                        {
                            _state = 2;

                            PrintLine(sb, ch, count);
                            Console.WriteLine("Begin async method");
                            Console.WriteLine();
                            //rr += await MyDelayAsync(loop, milsec);
                            _awaiter = MyDelayAsync(loop, milsec).GetAwaiter();
                            if(_awaiter.IsCompleted)
                            {
                                goto case 2;
                            }
                            else
                            {
                                if(_debgugToSleep)
                                {
                                    MySleep(3600 * 3, 1000);
                                }
                                _self = this;
                                //_atmBuiler.AwaitOnCompleted(ref _awaiter, ref _self);
                                _atmBuiler.AwaitUnsafeOnCompleted(ref _awaiter, ref _self);
                            }

                        }
                        break;
                    case 2:
                        {
                            _state = 3;

                            rr += _awaiter.GetResult();

                            ++ch;
                            PrintLine(sb, ch, count);
                            Console.WriteLine($"The result is {rr}");
                            Console.WriteLine();
                            loop += delta;
                            //rr += await MyDelayAsync(loop, milsec);
                            _awaiter = MyDelayAsync(loop, milsec).GetAwaiter();
                            if(_awaiter.IsCompleted)
                            {
                                goto case 3;
                            }
                            else
                            {
                                if (_debgugToSleep)
                                {
                                    MySleep(3600 * 3, 1000);
                                }
                                _self = this;
                                //_atmBuiler.AwaitOnCompleted(ref _awaiter, ref _self);
                                _atmBuiler.AwaitUnsafeOnCompleted(ref _awaiter, ref _self);
                            }
                        }
                        break;
                    case 3:
                        {
                            _state = 4;

                            rr += _awaiter.GetResult();

                            ++ch;
                            PrintLine(sb, ch, count);
                            Console.WriteLine($"The result is {rr}");
                            Console.WriteLine();
                            loop += delta;
                            //rr += await MyDelayAsync(loop, milsec);
                            _awaiter = MyDelayAsync(loop, milsec).GetAwaiter();
                            if(_awaiter.IsCompleted)
                            {
                                goto case 4;
                            }
                            else
                            {
                                if (_debgugToSleep)
                                {
                                    MySleep(3600 * 3, 1000);
                                }
                                _self = this;
                                //_atmBuiler.AwaitOnCompleted(ref _awaiter, ref _self);
                                _atmBuiler.AwaitUnsafeOnCompleted(ref _awaiter, ref _self);
                            }
                        }
                        break;
                    case 4:
                        {
                            _state = -1;

                            rr += _awaiter.GetResult();
                            ++ch;
                            PrintLine(sb, ch, count);
                            Console.WriteLine($"The result is {rr}");
                            Console.WriteLine("End async method");
                            Console.WriteLine();
                            _atmBuiler.SetResult(rr);
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

        public static Task<int> MethodManualAsync()
        {
            bool debgugToSleep = true;
            MethodManualAsyncStateMachine stateMachine = new MethodManualAsyncStateMachine(debgugToSleep);
            return stateMachine.Start();
        }
        #endregion
    }
}
