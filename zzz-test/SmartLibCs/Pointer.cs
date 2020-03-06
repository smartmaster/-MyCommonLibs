using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace SmartLib
{
    public class Pointer<T>
    {
        public T Object;
    }


    public static class PointerTest
    {
        public static void Case0()
        {
            Pointer<Pointer<Pointer<int>>> pppi = new Pointer<Pointer<Pointer<int>>>
            {
                Object = new Pointer<Pointer<int>>
                {
                    Object = new Pointer<int>
                    {
                        Object = 123
                    }
                }
            };

            Console.WriteLine($"{pppi.Object.Object.Object}");
        }
    }
}
