using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestQuiz
{
    public class AllCombinationOfResult
    {
        private readonly long m_result;
        private readonly Action<IEnumerable<long>> m_action;
        private readonly Stack<long> m_stack = new Stack<long>();
        private long m_currentSum = 0;
        private long m_currentProduct = 1;
        private List<long> m_listFactor;

        public AllCombinationOfResult(long result, Action<IEnumerable<long>> action)
        {
            m_result = result;
            m_action = action;
        }


        public void CalcForSum()
        {
            for(long ii = 1; ii <= m_result-1; ++ ii)
            {
                if(m_stack.Count == 0 || ii >= m_stack.Peek())
                {
                    m_currentSum += ii;
                    m_stack.Push(ii);

                    if(m_currentSum == m_result)
                    {
                        m_action?.Invoke(m_stack.Reverse());
                    }
                    else if(m_currentSum < m_result)
                    {
                        CalcForSum();
                    }

                    m_currentSum -= m_stack.Pop();
                }
            }
        }


        private void PrepareForProduct()
        {
            m_listFactor = new List<long>();
            for(int ii = 2; ii <= m_result / 2; ++ ii)
            {
                if(m_result % ii == 0)
                {
                    m_listFactor.Add(ii);
                }
            }
        }

        private void DoCalcForProduct()
        {
            foreach (long ii in m_listFactor)
            {
                if (m_stack.Count == 0 || ii >= m_stack.Peek())
                {
                    m_currentProduct *= ii;
                    m_stack.Push(ii);

                    if (m_currentProduct == m_result)
                    {
                        m_action?.Invoke(m_stack.Reverse());
                    }
                    else if (m_currentProduct < m_result)
                    {
                        DoCalcForProduct();
                    }

                    m_currentProduct /= m_stack.Pop();
                }
            }
        }

        public void CalcForProduct()
        {
            PrepareForProduct();
            DoCalcForProduct();
        }
    }

    public static class AllCombinationOfSumTest
    {
        public static void Case0()
        {
            long index = 0;
            StringBuilder sb = new StringBuilder();
            Action<IEnumerable<long>> action = seq =>
            {
                sb.AppendFormat($"[{index++,10:D8}] ");
                foreach (long vv in seq)
                {
                    sb.AppendFormat($"{vv} ");
                }
                sb.AppendLine();
                Console.Write(sb.ToString());
                sb.Clear();
            };


            long[] testData = new long[] {2, 3, 4, 5, 7, 11, 29};
            foreach(long sum in testData)
            {
                index = 0;
                sb.Clear();
                Console.WriteLine($"++++++++++++++++++++++++++={sum}=++++++++++++++++++++++++++");

                AllCombinationOfResult acs = new AllCombinationOfResult(sum, action);
                acs.CalcForSum();

                Console.WriteLine();
                Console.WriteLine();
            }


            long[] testData2 = new long[] { 29, 64, 100, 4566897, 2*3*4*5*6*7*8*9};
            foreach (long product in testData2)
            {
                index = 0;
                sb.Clear();
                Console.WriteLine($"**************************={product}=**************************");

                AllCombinationOfResult acs = new AllCombinationOfResult(product, action);
                acs.CalcForProduct();

                Console.WriteLine();
                Console.WriteLine();
            }

        }
    }
}
