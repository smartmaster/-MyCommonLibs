using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace TestCs
{
    struct MySs
    {
        public int _xx;
        public int _yy;
    }
    static class TestStructScope
    {

        public static async Task RefStructAsync(/*ref*/ MySs ss) //Async methods cannot have ref or out parameters
        {
            Console.WriteLine($"befor async - {nameof(ss._xx)}={ss._xx}, {nameof(ss._yy)}={ss._yy}");
            await Task.Delay(20_000);
            Console.WriteLine($"after async - {nameof(ss._xx)}={ss._xx}, {nameof(ss._yy)}={ss._yy}");
        }
        public static void RefStruct(ref MySs ss)
        {
            Console.WriteLine($"befor async - {nameof(ss._xx)}={ss._xx}, {nameof(ss._yy)}={ss._yy}");
            Task.Delay(20_000).Wait();
            Console.WriteLine($"after async - {nameof(ss._xx)}={ss._xx}, {nameof(ss._yy)}={ss._yy}");
        }

        public static void Case0()
        {
            MySs ss = new MySs
            {
                _xx = 100,
                _yy = 100,
            };
            RefStruct(ref ss);
        }


        public static async Task Case1Async()
        {
            int _10sec = 10_000;
            MySs myss = new MySs();
            await Task.Delay(_10sec);

            RefStruct(ref myss);
            await Task.Delay(_10sec);

            RefStruct(ref myss);
            await Task.Delay(_10sec);
        }

        class Case1AsyncStateMachine : IAsyncStateMachine
        {
            private int __10sec = 10_000;
            private  MySs _myss = new MySs();

            public AsyncTaskMethodBuilder _builder;
            private int _state = -1;
            private TaskAwaiter _awaiter;
            private Case1AsyncStateMachine _self;
            
            void IAsyncStateMachine.MoveNext()
            {
                //int _10sec = 10_000;
                //MySs myss = new MySs();
                
                //await Task.Delay(_10sec);

                //RefStruct(ref myss);
                //await Task.Delay(_10sec);

                //RefStruct(ref myss);
                //await Task.Delay(_10sec);

                switch(_state)
                {
                    case -1:
                        {
                            _self = this;
                            _awaiter = Task.Delay(__10sec).GetAwaiter();
                            if(_awaiter.IsCompleted)
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
                            RefStruct(ref _myss);
                            _awaiter = Task.Delay(__10sec).GetAwaiter();
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
                            RefStruct(ref _myss);
                            _awaiter = Task.Delay(__10sec).GetAwaiter();
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
                            _builder.SetResult();
                        }
                        break;
                }
            }

            void IAsyncStateMachine.SetStateMachine(IAsyncStateMachine stateMachine)
            {
                //throw new NotImplementedException();
            }
        }

    }
}
