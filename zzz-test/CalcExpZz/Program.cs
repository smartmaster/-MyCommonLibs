using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace CalcExpZz
{
    class Exp
    {
        public LinkedListNode<object> first;
        public LinkedListNode<object> last;
        public int count;

        public Exp(LinkedListNode<object> first = null, LinkedListNode<object> last = null, int count = 0)
        {
            this.first = first;
            this.last = last;
            this.count = count;
        }

        public string StrExp
        {
            get
            {
                StringBuilder sb = new StringBuilder();
                var ff = first;
                var ee = last?.Next;
                while (ff != ee)
                {
                    if(null != ff.Value)
                    {
                        sb.Append(ff.Value);
                    }
                    ff = ff.Next;
                }

                return sb.ToString();
            }
        }
    }

    static class Helper
    {

        static bool IsLeftBracket(object ooo)
        {
            return ((ooo != null) && (ooo is char ch) && (ch == '('));
        }

        static bool IsRightBracket(object ooo)
        {
            return ((ooo != null) && (ooo is char ch) && (ch == ')'));
        }

        public static void Clear(LinkedListNode<object> first, LinkedListNode<object> last)
        {
            var end = last?.Next;
            while (first != end)
            {
                first.Value = null;
                first = first.Next;
            }
        }


        public static void RemoveAllNull(LinkedList<object> llst)
        {
            var cur = llst.First;
            while(cur != null)
            {
                var next = cur.Next;
                if(null == cur.Value)
                {
                    llst.Remove(cur);
                }
                cur = next;
            }
        }

        public static LinkedListNode<object> Find(LinkedListNode<object> first, LinkedListNode<object> last, Func<object, bool> func)
        {
            var end = last?.Next;
            LinkedListNode<object> rr = null;
            while(first != end)
            {
                if(func(first.Value))
                {
                    rr = first;
                    break;
                }
                first = first.Next;
            }

            return rr;
        }

        public static LinkedListNode<object> ReverseFind(LinkedListNode<object> rfirst, LinkedListNode<object> rlast, Func<object, bool> func)
        {
            var rend = rlast?.Previous;
            LinkedListNode<object> rr = null;
            while (rfirst != rend)
            {
                if (func(rfirst.Value))
                {
                    rr = rfirst;
                    break;
                }
                rfirst = rfirst.Previous;
            }

            return rr;
        }


        public static Exp FindPair(LinkedListNode<object> first, LinkedListNode<object> last, Func<object, bool> leftFunc = null, Func<object, bool> rightFunc = null)
        {
            if(null == leftFunc)
            {
                leftFunc = IsLeftBracket;
            }

            if(null == rightFunc)
            {
                rightFunc = IsRightBracket;
            }

            Exp rr = new Exp();
            var ff = Find(first, last, leftFunc);
            if(null != ff)
            {
                int stack = 1;
                rr.count = 1;
                rr.first = ff;
                rr.last = ff.Next;
                var end = last?.Next;
                while(rr.last != end)
                {
                    if(leftFunc(rr.last.Value))
                    {
                        ++stack;
                    }
                    else if(rightFunc(rr.last.Value))
                    {
                        --stack;
                    }

                    if(null != rr.last.Value)
                    {
                        ++rr.count;
                    }

                    if(0 == stack)
                    {
                        break;
                    }

                    rr.last = rr.last.Next;
                }

                if(0 != stack)
                {
                    var expTemp = new Exp(first, last);
                    throw new Exception($"Invali expression: {expTemp.StrExp}");
                }
            }

            return rr;
        }

        public static LinkedList<object> StringToExpresssionList(string str)
        {
            LinkedList<object> llst = new LinkedList<object>();
            for (int ii = 0; ii < str.Length; ++ii)
            {
                if (char.IsNumber(str[ii]))
                {
                    int start = ii;
                    int len = 0;
                    while (ii < str.Length && char.IsNumber(str[ii]))
                    {
                        ++ii;
                        ++len;
                    }
                    llst.AddLast(int.Parse(str.Substring(start, len)));
                    --ii; //move one step backward 
                }
                else
                {
                    llst.AddLast(str[ii]);
                }
            }

            return llst;
        }
    }

    class CalcExp
    {
        private LinkedList<object> m_llst;
        public string StringExp { get; set; }
        public string CurrentStringExp
        {
            get
            {
                StringBuilder sb = new StringBuilder();
                foreach (object obj in m_llst)
                {
                    if (null != obj)
                    {
                        sb.Append(obj);
                    }
                }

                return sb.ToString();
            }
        }
        

        public CalcExp(string str)
        {
            StringExp = str;
            StringBuilder sb = new StringBuilder(str);
            sb.Replace(" ", "");
            sb.Replace("\t", "");
            m_llst = Helper.StringToExpresssionList(sb.ToString());
            m_llst.AddFirst('(');
            m_llst.AddLast(')');
        }

        int CalcBasicExp(Exp exp)
        {
            int res = 0;


            var leftNumberNode = Helper.Find(exp.first, exp.last, ooo => null != ooo);
            var opNode = Helper.Find(leftNumberNode.Next, exp.last, ooo => null != ooo);
            var rightNumberNode = Helper.Find(opNode.Next, exp.last, ooo => null != ooo); ;

            if ((opNode.Value is char ch) &&
                    (leftNumberNode.Value is int leftNumber) &&
                    (rightNumberNode.Value is int rightNumber))
            {

                switch (ch)
                {
                    case '+':
                        {
                            res = leftNumber + rightNumber;
                        }
                        break;
                    case '-':
                        {
                            res = leftNumber - rightNumber;
                        }
                        break;
                    case '*':
                        {
                            res = leftNumber * rightNumber;
                        }
                        break;
                    case '/':
                        {
                            res = leftNumber / rightNumber;
                        }
                        break;
                }
            }
            else
            {
                throw new Exception($"Invalid expresion: {exp.StrExp}");
            }

            return res;
        }

        void ProcessExpression(Exp exp)
        {
            ////////////////////////////////////////////////////////////////
            var begin = exp.first;
            for (;;)
            {
                var pair = Helper.FindPair(begin, exp.last);
                if (null != pair.last && null != pair.last)
                {
                    begin = pair.last.Next;
                    ProcessBrackets(pair);
                }
                else
                {
                    break;
                }
            }

            //////////////////////////////////////////////
            ProcessMulDiv(exp);
            ProcessAddSub(exp);
        }
        void ProcessBrackets(Exp exp)
        {
            if(exp.count == 3)
            {
                exp.first.Value = null;
                exp.last.Value = null;
            }
            else if(exp.count == 5)
            {
                int rr = CalcBasicExp(new Exp(exp.first.Next, exp.last.Previous));
                Helper.Clear(exp.first, exp.last);
                exp.first.Value = rr;
            }
            else
            {
                ProcessExpression(new Exp(exp.first.Next, exp.last.Previous));
                exp.first.Value = null;
                exp.last.Value = null;
            }
        }

        void ProcessMulDiv(Exp exp)
        {
            for(;;)
            {
                var opNode = Helper.Find(exp.first, exp.last, ooo => (ooo is char chTemp) && (chTemp == '*' || chTemp == '/'));
                if(null == opNode)
                {
                    break;
                }


                var firstNode = Helper.ReverseFind(opNode.Previous, exp.first, ooo => ooo != null);
                var lastNode = Helper.Find(opNode.Next, exp.last, ooo => ooo != null);

                if ((opNode.Value is char ch) &&
                    (firstNode.Value is int leftNumber) &&
                    (lastNode.Value is int rightNumber))
                {
                    int res = 0;
                    switch (ch)
                    {
                        case '+':
                            {
                                res = leftNumber + rightNumber;
                            }
                            break;
                        case '-':
                            {
                                res = leftNumber - rightNumber;
                            }
                            break;
                        case '*':
                            {
                                res = leftNumber * rightNumber;
                            }
                            break;
                        case '/':
                            {
                                res = leftNumber / rightNumber;
                            }
                            break;
                    }

                    firstNode.Value = null;
                    opNode.Value = null;
                    lastNode.Value = null;

                    firstNode.Value = res;

                }
                else
                {
                    throw new Exception($"Invalid expresion: {exp.StrExp}");
                }

               
            }
        }

        void ProcessAddSub(Exp exp)
        {
            for (;;)
            {
                var opNode = Helper.Find(exp.first, exp.last, ooo => (ooo is char chTemp) && (chTemp == '+' || chTemp == '-'));
                if (null == opNode)
                {
                    break;
                }


                var firstNode = Helper.ReverseFind(opNode.Previous, exp.first, ooo => ooo != null);
                var lastNode = Helper.Find(opNode.Next, exp.last, ooo => ooo != null);

                if ((opNode.Value is char ch) &&
                    (firstNode.Value is int leftNumber) &&
                    (lastNode.Value is int rightNumber))
                {
                    int res = 0;
                    switch (ch)
                    {
                        case '+':
                            {
                                res = leftNumber + rightNumber;
                            }
                            break;
                        case '-':
                            {
                                res = leftNumber - rightNumber;
                            }
                            break;
                        case '*':
                            {
                                res = leftNumber * rightNumber;
                            }
                            break;
                        case '/':
                            {
                                res = leftNumber / rightNumber;
                            }
                            break;
                    }

                    firstNode.Value = null;
                    opNode.Value = null;
                    lastNode.Value = null;

                    firstNode.Value = res;

                }
                else
                {
                    throw new Exception($"Invalid expresion: {exp.StrExp}");
                }


            }
        }

        public void Calc()
        {
            ProcessExpression(new Exp(m_llst.First, m_llst.Last));
        }

        public int Result()
        {

            Helper.RemoveAllNull(m_llst);

            if (m_llst.Count != 1)
            {
                throw new Exception($"Unexpected results {nameof(m_llst.Count)}={m_llst.Count}");
            }

            if (m_llst.First.Value is int result)
            {
                return result;
            }
            else
            {
                throw new Exception($"Unexpected results {nameof(m_llst.First.Value)}={m_llst.First.Value}");
            }

        }
    }


  static  class Program
    {
        static void TestCase0()
        {

            string strExp = "3*80+90/7-50-09+(01-90)/40";
            var llst0 = Helper.StringToExpresssionList(strExp);

            object[] arr = new object[] { 1, 2, 3, "aa", "bb", "cc" };
            LinkedList<object> llst = new LinkedList<object>(arr);

            LinkedListNode<object> ff = Helper.Find(llst.First, llst.Last, ooo => ooo is string);

            var res = Helper.FindPair(llst.First, llst.Last, ooo => (ooo is int ii) && (ii == 2), ooo => (ooo is string str) && (str == "bb"));

            Helper.Clear(llst.First, llst.Last);

            Console.WriteLine();
        }

        static void TestCase1()
        {
            CalcExp ce = new CalcExp("(6-7)*(5+9)");
            ce.Calc();
        }

        static void TestCase2()
        {
            (string strExp, int result)[] strExps = new(string strExp, int result)[]
            {
                ("((((1 + 2 + 3))))", ((((1 + 2 + 3))))),
                ("(1 + 2 +3 + ((4+5)))+(1 + 2 +3 + ((4+5)))*(1 + 2 +3 + ((4+5)))+(1 + 2 +3 + ((4+5)))", (1 + 2 +3 + ((4+5)))+(1 + 2 +3 + ((4+5)))*(1 + 2 +3 + ((4+5)))+(1 + 2 +3 + ((4+5)))),
                ("10 + 2 * 6", 10 + 2 * 6),
                ("100 * 2 + 12", 100 * 2 + 12),
                ("100 * ( 2 + 12 )", 100 * ( 2 + 12 )),
                ("100 * ( 2 + 12 ) / 14", 100 * ( 2 + 12 ) / 14),
                ("(6-7)*(5+9)", (6-7)*(5+9)),
                ("1+3*(4+7)", 1+3*(4+7)),
                ("((1+2))", ((1+2))),
                ("3*8+9/7-5-9+(1-9)/4", 3*8+9/7-5-9+(1-9)/4),
                ("0*8+0/6-9+(7-1)", 0*8+0/6-9+(7-1))
            };

            foreach (var tt in strExps)
            {
                CalcExp ce = new CalcExp(tt.strExp);
                ce.Calc();
                Console.WriteLine($"{ce.StringExp} = {ce.Result()}, {tt.result}, {tt.result == ce.Result()}");
            }
        }

        static void Main(string[] args)
        {
            int testCase = 0;
            if (args.Length > 0)
            {
                int.TryParse(args[0], out testCase);
            }

            switch (testCase)
            {
                case 2:
                    {
                        TestCase2();
                    }
                    break;
                case 1:
                    {
                        TestCase1();
                    }
                    break;
                case 0:
                    {
                        TestCase0();
                    }
                    break;
            }

            Console.WriteLine("Hello World!");
        }
    }
}
