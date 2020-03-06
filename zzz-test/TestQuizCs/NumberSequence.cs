using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestQuiz
{
    public class NumberSequence
    {
        private int[] m_array;
        private int m_count;

        private Stack<int> m_result;

        private Action<IEnumerable<int>> m_action;


        private void DoPermutate()
        {
            for (int ii = 0; ii < m_array.Length; ++ii)
            {
                if (m_array[ii] != -1)
                {
                    m_result.Push(m_array[ii]);
                    m_array[ii] = -1;
                    if (m_result.Count == m_count)
                    {
                        m_action?.Invoke(m_result.Reverse());
                    }
                    else
                    {
                        DoPermutate();
                    }
                    m_array[ii] = m_result.Pop();
                }
            }
        }


        private void DoCombinate()
        {
            for (int ii = 0; ii < m_array.Length; ++ii)
            {
                if ((m_array[ii] != -1) &&
                    (m_result.Count == 0 || m_array[ii] > m_result.Peek()))
                {
                    m_result.Push(m_array[ii]);
                    m_array[ii] = -1;
                    if (m_result.Count == m_count)
                    {
                        m_action?.Invoke(m_result.Reverse());
                    }
                    else
                    {
                        DoCombinate();
                    }
                    m_array[ii] = m_result.Pop();
                }
            }
        }


        private void InitStartup(int total, int count, Action<IEnumerable<int>> action)
        {
            m_array = new int[total];
            for (int ii = 0; ii < total; ++ii)
            {
                m_array[ii] = ii;
            }
            m_count = count;

            m_result = new Stack<int>();
            m_action = action;
        }

        public void Permutate(int total, int count, Action<IEnumerable<int>> action)
        {
            InitStartup(total, count, action);
            DoPermutate();
        }

        public void Combinate(int total, int count, Action<IEnumerable<int>> action)
        {
            InitStartup(total, count, action);
            DoCombinate();
        }
    }


    public static class PermutationTest
    {
        public static void Case0()
        {
            NumberSequence pp = new NumberSequence();

            int index = 0;
            StringBuilder sb = new StringBuilder();
            Action<IEnumerable<int>> action = seq =>
            {
                sb.AppendFormat($"[{index++,10:D8}] ");
                foreach (int ii in seq)
                {
                    sb.AppendFormat($"{ii} ");
                }
                sb.AppendLine();
                Console.Write(sb.ToString());
                sb.Clear();
            };

            pp.Permutate(6, 6, action);

            Console.Out.WriteLine();

            index = 0;
            pp.Combinate(6, 2, action);
        }
    }
}
