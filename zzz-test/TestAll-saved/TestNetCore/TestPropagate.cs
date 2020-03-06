using System;
using System.Collections.Generic;
using System.Text;

namespace TestNetCore
{
    struct Inner
    //class Inner
    {
        public int innerValue;
    }

    struct Outer
    //class Outer
    {
        public int outerValue;
        public Inner innerObject;
    }

    class TestPropagate
    {
        public static void TestInParam(in Outer outerObject)
        {
            //outerObject.outerValue = 100;
            //outerObject.innerObject = new Inner();
            //outerObject.innerObject.innerValue = 200;

        }
    }
}
