using System;
using System.Collections.Generic;
using System.Reflection;
using System.Reflection.Emit;
using System.Text;

namespace TestCsAllInOne
{

    static class TestILCallVirtual
    {
        class AA
        {
            private string _name = nameof(AA);
            public virtual void Print()
            {
                Console.WriteLine($"I am {_name}");
            }
        }

        class BB : AA
        {
            private string _name = nameof(BB);
            public override void Print()
            {
                Console.WriteLine($"I am {_name}");
            }
        }

        delegate void ILCallMethod(AA aa);
        static ILCallMethod CreateDelegate(Type tt, string funcname, bool useCallVirt)
        {
            MethodInfo mi = tt.GetMethod(funcname);

            DynamicMethod dm = new DynamicMethod(
                nameof(ILCallMethod) + "_" + tt.Name,
                typeof(void),
                new Type[] { typeof(AA) }
                );

            ILGenerator ilgen = dm.GetILGenerator();
            ilgen.Emit(OpCodes.Ldarg_0);
            if(useCallVirt)
            {
                ilgen.Emit(OpCodes.Callvirt, mi);
            }
            else
            {
                ilgen.Emit(OpCodes.Call, mi);
            }
            
            ilgen.Emit(OpCodes.Ret);

            return (ILCallMethod)dm.CreateDelegate(typeof(ILCallMethod));
        }

       public static void Case0()
        {
            bool useCallVirtual = false;
            do
            {
                useCallVirtual = !useCallVirtual;

                ILCallMethod func = CreateDelegate(typeof(AA), nameof(AA.Print), useCallVirtual);
                AA aa = new AA();
                BB bb = new BB();
                AA ba = (AA)bb;
                func(aa);
                func(bb);
                func(ba);
                Console.WriteLine();
            } while (useCallVirtual);

            //useCallVirtual = false;
            do
            {
                useCallVirtual = !useCallVirtual;

                ILCallMethod func = CreateDelegate(typeof(BB), nameof(AA.Print), useCallVirtual);
                AA aa = new AA();
                BB bb = new BB();
                AA ba = (AA)bb;
                func(aa);
                func(bb);
                func(ba);
                Console.WriteLine();
            } while (useCallVirtual);
        }
    }
}
