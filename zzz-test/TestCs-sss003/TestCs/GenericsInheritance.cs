using System;
using System.Collections.Generic;
using System.Text;

namespace TestCs
{
    class LinkEntry<T>
        where T : LinkEntry<T>
    {
        public T _prev;
        public T _next;

        public static void Connect(LinkEntry<T> e1, LinkEntry<T> e2)
        {
            if(null != e1)
            {
                e1._next = (T)e2;
            }
            
            if(null != e2)
            {
                e2._prev = (T)e1;
            }
        }
    }


    class MyEntry : LinkEntry<MyEntry>
    {
        public int _data;



        public static void Case0()
        {
            MyEntry[] arr = new MyEntry[]
            {
            new MyEntry{_data = 1},
            new MyEntry{_data = 2},
            new MyEntry{_data = 3},
            new MyEntry{_data = 4},
            new MyEntry{_data = 5},
            new MyEntry{_data = 6},
            };

            for(int ii = 0; ii < arr.Length - 1; ++ ii)
            {
                Connect(arr[ii], arr[ii + 1]);
            }

            var head = arr[0];
            while(head != null)
            {
                Console.Write($"{head._data} ");
                head = head._next;
            }
            Console.WriteLine();


            var tail = arr[arr.Length - 1];
            while(tail != null)
            {
                Console.Write($"{tail._data} ");
                tail = tail._prev;
            }
            Console.WriteLine();
         }
    }
   
}
