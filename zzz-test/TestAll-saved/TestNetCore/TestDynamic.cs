using System;
using System.Collections.Generic;
using System.Dynamic;
using System.Text;

namespace TestNetCore
{
    class TestDynamic : DynamicObject
    {
        public override bool TryInvokeMember(InvokeMemberBinder binder, object[] args, out object result)
        {
            Console.WriteLine($"Method: {binder.Name}");

            Console.WriteLine("Arguments:");
            foreach(var aa in args)
            {
                Console.WriteLine(aa);
            }
            result = null;

            if(string.Equals(binder.Name, "Name"))
            {
                result = "who am i";
            }
            return true;
        }


#region TestCases
        public static void Case0()
        {
            dynamic dd = new TestDynamic();
            dd.Eat("hello", 123M, 456.789);
            string name = dd.Name();
        }
#endregion
    }
}
