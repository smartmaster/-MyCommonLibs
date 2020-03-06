using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;
using System.Threading;

namespace TestCsAllInOne
{
    class TestClrMethodTable
    {
        class vMyObj
        {
            static int _classCount = 0;
            private int _id;

            public vMyObj()
            {
                _id = Interlocked.Increment(ref _classCount);
            }

            public void Print()
            {
                Console.WriteLine($"{nameof(Print)}: this is object with id {_id}");
            }

            public override string ToString()
            {
                return $"{nameof(ToString)}: {nameof(vMyObj)} with id {_id}";
            }


            public string FieldName()
            {
                return nameof(_id);
            }
        }


        public static void Case0()
        {
            //Console.WriteLine("Press enter to start testing...");
            //Console.ReadLine();

            vMyObj obj = new vMyObj();
            Type tt = obj.GetType();
            FieldInfo fi = tt.GetField(obj.FieldName(), BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
            MethodInfo miPrint = tt.GetMethod(nameof(vMyObj.Print), BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
            MethodInfo miToString = tt.GetMethod(nameof(vMyObj.ToString), BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);

            int fieldVal = (int)fi.GetValue(obj);
            miPrint.Invoke(obj, new object[] { });
            string me = (string)miToString.Invoke(obj, new object[] { });
            Console.WriteLine($"{me}");
            return;
        }
    }
}
