using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
    static class TestMonitor
    {
        public static void Case0()
        {
            object obj = new object();
            bool locked = false;
            Monitor.Enter(obj, ref locked);
            if (locked)
            {
                Monitor.Exit(obj);
            }
        }


        class MyQueue<T>
        {
            private T[] _array;
            private int _head = 0;
            private int _tail = 0;

            public MyQueue(int size)
            {
                _array = new T[size];
            }

            public void Enqueue(T data)
            {
                lock(_array)
                {
                    while(_tail - _head == _array.Length)
                    {
                        Monitor.Wait(_array);
                    }

                    _array[_tail % _array.Length] = data;
                    ++_tail;

                    Monitor.PulseAll(_array);
                }
            }

            public T Dequeue()
            {
                lock(_array)
                {
                    while(_tail - _head == 0)
                    {
                        Monitor.Wait(_array);
                    }


                    while (_head >= _array.Length)
                    {
                        _head -= _array.Length;
                        _tail -= _array.Length;
                    }

                    T rv = _array[_head];
                    ++_head;

                    Monitor.PulseAll(_array);

                    return rv;
                }
            }
        }


      public static class Test
        {
            public static async Task Case0()
            {
                MyQueue<int> queue = new MyQueue<int>(17);
                Func<int, int, (int, long)> provider = (int begin, int end) =>
                {
                    int count = 0;
                    long sum = 0;
                    for (int ii = begin; ii < end; ++ii)
                    {
                        queue.Enqueue(ii);
                        ++count;
                        sum += ii;
                        Console.WriteLine($"Provider: thread={Thread.CurrentThread.ManagedThreadId}, {nameof(count)}={count}, {nameof(sum)}={sum}");
                    }

                    return (count, sum);
                };

                Func<(int, long)> consumer = () =>
                {
                    int count = 0;
                    long sum = 0;
                    for(; ; )
                    {
                        int data = queue.Dequeue();
                        if(data < 0)
                        {
                            break;
                        }
                        ++count;
                        sum += data;
                        Console.WriteLine($"Consumer: thread={Thread.CurrentThread.ManagedThreadId}, {nameof(count)}={count}, {nameof(sum)}={sum}");
                    }

                    return (count, sum);
                    
                };


                int partCount = 100_000;
                Task<(int, long)> Provider1 = Task.Factory.StartNew(() => provider(0, partCount), TaskCreationOptions.LongRunning);
                Task<(int, long)> Provider2 = Task.Factory.StartNew(() => provider(partCount, partCount + partCount), TaskCreationOptions.LongRunning);
                Task<(int, long)> consumer1 = Task.Factory.StartNew(()=>consumer(), TaskCreationOptions.LongRunning);
                Task<(int, long)> consumer2 = Task.Factory.StartNew(() => consumer(), TaskCreationOptions.LongRunning);

                await Task.WhenAll(Provider1, Provider2);
                queue.Enqueue(-1);
                queue.Enqueue(-1);
                await Task.WhenAll(consumer1, consumer2);

                int countP = Provider1.Result.Item1 + Provider2.Result.Item1;
                long sumP = Provider1.Result.Item2 + Provider2.Result.Item2;

                int countC = consumer1.Result.Item1 + consumer2.Result.Item1;
                long sumC = consumer1.Result.Item2 + consumer2.Result.Item2;

                Debug.Assert(countP == countC);
                Debug.Assert(sumP == sumC);

            }


        }
    }
}

