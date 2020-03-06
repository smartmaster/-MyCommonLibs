using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestCs
{
    static class ManualAsync
    {

        public static int Bar(int pp)
        {
            Thread.Sleep(TimeSpan.FromSeconds(pp));
            return pp * 10;
        }

        public static async Task<int> BarAsync(int pp)
        {
            await Task.Delay(TimeSpan.FromSeconds(pp));
            return pp * 100;
        }

        public static async Task<int> FooAsync(int p0, int p1, int p2)
        {
            int s0 = Bar(p0);
            int a0 = await MyBarAsync(p0);

            int s1 = Bar(p1);
            int a1 = await MyBarAsync(p1);

            int s2 = Bar(p2);
            int a2 = await MyBarAsync(p2);

            int result = s0 + a0 + s1 + a1 + s2 + a2;

            return result;
        }


        public static Task<int> MyFooAsync(int p0, int p1, int p2)
        {
            MyFooAsyncStateMachine myFooAsyncStateMachine = new MyFooAsyncStateMachine
            {
                _p0 = p0,
                _p1 = p1,
                _p2 = p2,
            };
            myFooAsyncStateMachine._builder.Start(ref myFooAsyncStateMachine);
            return myFooAsyncStateMachine._builder.Task;
        }

        class MyFooAsyncStateMachine : IAsyncStateMachine
        {
            public int _p0;
            public int _p1;
            public int _p2;


            private int _s0;
            private int _s1;
            private int _s2;

            private int _a0;
            private int _a1;
            private int _a2;



            private int _state = -1;
            public AsyncTaskMethodBuilder<int> _builder = AsyncTaskMethodBuilder<int>.Create();
            private TaskAwaiter<int> _awaiter;
            private MyFooAsyncStateMachine _self;

            void IAsyncStateMachine.MoveNext()
            {
                //throw new NotImplementedException();

                try
                {
                    switch (_state)
                    {
                        case -1:
                            {
                                _self = this;
                                //int s0 = Bar(p0);
                                //int a0 = await MyBarAsync(p0);

                                _s0 = Bar(_p0);
                                _awaiter = MyBarAsync(_p0).GetAwaiter();
                                if (_awaiter.IsCompleted)
                                {
                                    goto case 0;
                                }
                                else
                                {
                                    _state = 0;
                                    _builder.AwaitUnsafeOnCompleted(ref _awaiter, ref _self);
                                }

                            }
                            break;
                        case 0:
                            {
                                _a0 = _awaiter.GetResult();
                                //int s1 = Bar(p1);
                                //int a1 = await MyBarAsync(p1);
                                _s1 = Bar(_p1);
                                _awaiter = MyBarAsync(_p1).GetAwaiter();
                                if (_awaiter.IsCompleted)
                                {
                                    goto case 1;
                                }
                                else
                                {
                                    _state = 1;
                                    _builder.AwaitUnsafeOnCompleted(ref _awaiter, ref _self);
                                }
                            }
                            break;
                        case 1:
                            {
                                _a1 = _awaiter.GetResult();
                                //int s2 = Bar(p2);
                                //int a2 = await MyBarAsync(p2);
                                _s2 = Bar(_p2);
                                _awaiter = MyBarAsync(_p2).GetAwaiter();
                                if (_awaiter.IsCompleted)
                                {
                                    goto case 2;
                                }
                                else
                                {
                                    _state = 2;
                                    _builder.AwaitUnsafeOnCompleted(ref _awaiter, ref _self);
                                }
                            }
                            break;
                        case 2:
                            {
                                _a2 = _awaiter.GetResult();
                                int result = _s0 + _a0 + _s1 + _a1 + _s2 + _a2;
                                _builder.SetResult(result);

                            }
                            break;
                    }
                }
                catch (Exception ex)
                {

                    _builder.SetException(ex);
                }
                
            }

            void IAsyncStateMachine.SetStateMachine(IAsyncStateMachine stateMachine)
            {
                //throw new NotImplementedException();
            }
        }

        public static Task<int> MyBarAsync(int pp)
        {
            MyBarAsyncStateMachine myBarAsyncStateMachine = new MyBarAsyncStateMachine
            {
                _pp = pp
            };
            myBarAsyncStateMachine._builder.Start(ref myBarAsyncStateMachine);
            return myBarAsyncStateMachine._builder.Task;
        }

        class MyBarAsyncStateMachine : IAsyncStateMachine
        {
            public int _pp;

            private int _state = -1;
            public AsyncTaskMethodBuilder<int> _builder = AsyncTaskMethodBuilder<int>.Create();
            private TaskAwaiter _awaiter;
            private MyBarAsyncStateMachine _self;

            
            void IAsyncStateMachine.MoveNext()
            {
                try
                {
                    switch (_state)
                    {
                        case -1:
                            {
                                _self = this;
                                _awaiter = Task.Delay(TimeSpan.FromSeconds(_pp)).GetAwaiter();
                                if (_awaiter.IsCompleted)
                                {
                                    goto case 0;
                                }
                                else
                                {
                                    _state = 0;
                                    _builder.AwaitUnsafeOnCompleted(ref _awaiter, ref _self);
                                }
                            }
                            break;
                        case 0:
                            {
                                _builder.SetResult(_pp * 100);
                            }
                            break;
                    }
                }
                catch (Exception ex)
                {
                    _builder.SetException(ex);
                    //throw;
                }
               
            }

            void IAsyncStateMachine.SetStateMachine(IAsyncStateMachine stateMachine)
            {
                //throw new NotImplementedException();
            }
        }



        public static async Task<int> LoopAsync(int loop)
        {
            int ii = 0;
            int sum = 0;
            while (ii < loop)
            {
                Console.WriteLine($">>>{sum}<<<");
                sum += await MyBarAsync((ii % 5) + 1);
                Console.WriteLine($"<<<{sum}>>>");
                ++ii;
            }

            return sum;
        }


        public static Task<int> MyLoopAsync(int loop)
        {
            MyLoopAsyncStateMachine MyLoopAsyncStateMachine = new MyLoopAsyncStateMachine
            {
                _loop = loop,
            };
            MyLoopAsyncStateMachine._builder.Start(ref MyLoopAsyncStateMachine);
            return MyLoopAsyncStateMachine._builder.Task;
        }


        class MyLoopAsyncStateMachine : IAsyncStateMachine
        {
            public int _loop;

            private int _ii = 0;
            private int _sum = 0;

            private int _state = -1;
            public AsyncTaskMethodBuilder<int> _builder = AsyncTaskMethodBuilder<int>.Create();
            private TaskAwaiter _awaiter;
            private MyLoopAsyncStateMachine _slef;
            
            void IAsyncStateMachine.MoveNext()
            {
                try
                {
                    switch (_state)
                    {
                        case -1:
                            {
                                _slef = this;
                                _awaiter = MyLoopAsyncStateMachineInnerProc(this).GetAwaiter();
                                if (_awaiter.IsCompleted)
                                {
                                    goto case 0;
                                }
                                else
                                {
                                    _state = 0;
                                    _builder.AwaitOnCompleted(ref _awaiter, ref _slef);
                                }
                            }
                            break;
                        case 0:
                            {
                                if (_ii < _loop)
                                {
                                    _state = -1;
                                    _builder.AwaitOnCompleted(ref _awaiter, ref _slef);
                                }
                                else
                                {
                                    _builder.SetResult(_sum);
                                }
                            }
                            break;
                    }
                }
                catch (Exception ex)
                {

                    _builder.SetException(ex);
                }
                
            }

            void IAsyncStateMachine.SetStateMachine(IAsyncStateMachine stateMachine)
            {
            }


            static Task MyLoopAsyncStateMachineInnerProc(MyLoopAsyncStateMachine myLoopAsyncStateMachine)
            {
                MyLoopAsyncStateMachineInner MyLoopAsyncStateMachineInner = new MyLoopAsyncStateMachineInner
                {
                    _myLoopAsyncStateMachine = myLoopAsyncStateMachine,
                };

                MyLoopAsyncStateMachineInner._builder.Start(ref MyLoopAsyncStateMachineInner);
                return MyLoopAsyncStateMachineInner._builder.Task;
            }

            class MyLoopAsyncStateMachineInner : IAsyncStateMachine
            {
                public MyLoopAsyncStateMachine _myLoopAsyncStateMachine;

                private int _state = -1;
                public AsyncTaskMethodBuilder _builder = AsyncTaskMethodBuilder.Create();
                private TaskAwaiter<int> _awaiter;
                private MyLoopAsyncStateMachineInner _slef;

                void IAsyncStateMachine.MoveNext()
                {
                    //Console.WriteLine($">>>{sum}<<<");
                    //sum += await MyBarAsync((ii % 5) + 1);

                    //Console.WriteLine($"<<<{sum}>>>");
                    //++ii;

                    try
                    {
                        switch (_state)
                        {
                            case -1:
                                {
                                    _slef = this;
                                    Console.WriteLine($">>>{_myLoopAsyncStateMachine._sum}<<<");
                                    _awaiter = MyBarAsync((_myLoopAsyncStateMachine._ii % 5) + 1).GetAwaiter();
                                    if (_awaiter.IsCompleted)
                                    {
                                        goto case 0;
                                    }
                                    else
                                    {
                                        _state = 0;
                                        _builder.AwaitUnsafeOnCompleted(ref _awaiter, ref _slef);
                                    }
                                }
                                break;
                            case 0:
                                {
                                    _myLoopAsyncStateMachine._sum += _awaiter.GetResult();
                                    Console.WriteLine($"<<<{_myLoopAsyncStateMachine._sum}>>>");
                                    ++_myLoopAsyncStateMachine._ii;
                                    _builder.SetResult();
                                }
                                break;
                        }
                    }
                    catch (Exception ex)
                    {
                        _builder.SetException(ex);
                    }
                }

                void IAsyncStateMachine.SetStateMachine(IAsyncStateMachine stateMachine)
                {
                }
            }
        }


        
    }
}
