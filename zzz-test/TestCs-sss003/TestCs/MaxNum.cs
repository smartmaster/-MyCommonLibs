using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//given a sequence of numbers, for example {111, 30, 9}, output the max number they can made up of 9_30_111 for the exaple

namespace TestCs
{

#if false
    class MaxNum
    {
        private readonly int[] _order;
        private readonly string[] _arrString;

        private MaxNum(params int[] arrInt)
        {
            _arrString = new string[arrInt.Length];
            _order = new int[arrInt.Length];
            for(int ii = 0; ii < _arrString.Length; ++ ii)
            {
                _arrString[ii] = arrInt[ii].ToString();
                _order[ii] = ii;
            }
        }
        private string MakeString(int count)
        {
            StringBuilder sb = new StringBuilder();
            for(int ii = 0; ii < count; ++ ii)
            {
                sb.Append(_arrString[_order[ii]]);
            }
            return sb.ToString();
        }

        private string Sort(int count)
        {
            if(count == 1)
            {
                return _arrString[0];
            }

            Sort(count - 1);
            string max = MakeString(count);
            int[] orderShadow = null;
            for(int kk = count - 2; kk >= 0; --kk)
            {
                int temp = _order[kk];
                _order[kk] = _order[kk + 1];
                _order[kk + 1] = temp;
                string tempMax = MakeString(count);
                if(string.Compare(tempMax, max) > 0)
                {
                    max = tempMax;
                    if(orderShadow == null)
                    {
                        orderShadow = new int[count];
                    }
                    Array.Copy(_order, orderShadow, count);
                }
            }

            if(orderShadow != null)
            {
                Array.Copy(orderShadow, _order, count);
            }

            return max;
        }

        public static void FindMax(params int[] arr)
        {
            MaxNum mn = new MaxNum(arr);
            string result = mn.Sort(arr.Length);
            StringBuilder sb = new StringBuilder();
            sb.Append("{");
            foreach(int val in arr)
            {
                sb.Append(val);
                sb.Append(", ");
            }
            sb.Remove(sb.Length - 2, 2);
            sb.Append("}, result = ");
            sb.Append(result);
            Console.WriteLine(sb.ToString());
        }

        public static void TestCase1()
        {
            FindMax(789, 976, 37, 539, 41);
            FindMax(1, 2, 3, 4, 5, 6, 7, 8, 9);
        }

        public static void TestCase0()
        {
            Random ran = new Random();
            const int count = 5;
            int[] arr = new int[count];
            const int min = 1;
            const int max = 1000;
            for(int ii = 0; ii < arr.Length; ++ ii)
            {
                arr[ii] = ran.Next(min, max);
            }

            FindMax(arr);
        }
    }
#endif


    class MaxSeq
    {
        private readonly List<string> _listStr;


        private MaxSeq(params int[] arrInt)
        {
            _listStr = (from val in arrInt
                        select val.ToString())
                        .ToList();
        }

        private string MakeString(List<int> order)
        {
            StringBuilder sb = new StringBuilder();
            foreach(var index in order)
            {
                sb.Append(_listStr[index]);
            }
            return sb.ToString();
        }

        private (string Result, List<int> Order) Sort(int count)
        {
            if(count == 1)
            {
                return (_listStr[0], new List<int> { 0 });
            }

            (string result, List<int> order) = Sort(count - 1);
            string newResult = result + _listStr[count - 1];
            order.Add(count - 1);
            List<int> newOrder = new List<int> (order);


            for(int ii = count - 2; ii >= 0; -- ii)
            {
                int temp = order[ii];
                order[ii] = order[ii + 1];
                order[ii + 1] = temp;

                string tempStr = MakeString(order);
                if(string.CompareOrdinal(tempStr, newResult) > 0)
                {
                    newResult = tempStr;
                    newOrder = new List<int>(order);
                }
            }

            return (newResult, newOrder);
        }



        public static void FindMax(params int[] arr)
        {

            foreach (var val in arr)
            {
                Console.Write($"{val} ");
            }
            Console.Write(" => ");


            MaxSeq mn = new MaxSeq(arr);
            var result = mn.Sort(arr.Length);

            foreach(var index in result.Order)
            {
                Console.Write($"{arr[index]} ");
            }
            Console.WriteLine();
        }


        public static void TestCase1()
        {
            FindMax(789, 976, 37, 539, 41);
            FindMax(1, 2, 3, 4, 5, 6, 7, 8, 9);
        }

        public static void TestCase0()
        {
            Random ran = new Random();
            const int count = 10;
            int[] arr = new int[count];
            for (int ii = 0; ii < arr.Length; ++ii)
            {
                arr[ii] = ran.Next() % 10000 + 1;
            }

            FindMax(arr);
        }
    }


}
