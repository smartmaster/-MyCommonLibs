using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCsAllInOne
{
    class TestPerm
    {
        int[] _arr;
        Stack<int> _stack = new Stack<int>();
        int _nn;
        int _mm;
        Action<IEnumerable<int>> _onResult;
        public TestPerm(int nn, int mm, Action<IEnumerable<int>> onResult)
        {
            _nn = nn;
            _mm = mm;
            _onResult = onResult;
            _arr = new int[nn];
            for(int ii = 0; ii < nn; ++ ii)
            {
                _arr[ii] = ii + 1;
            }
        }

        void Perm()
        {
            for(int ii = 0; ii < _nn; ++ ii)
            {
                ref int elem = ref _arr[ii];
                if(elem != 0)
                {
                    _stack.Push(elem);
                    elem = 0;

                    if (_stack.Count == _mm)
                    {
                        _onResult?.Invoke(_stack.Reverse());
                    }
                    else
                    {
                        Perm();
                    }

                    elem = _stack.Pop();
                }
            }
        }

        #region test cases
        public static void Case0()
        {
            int index = 0;
            TestPerm perm = new TestPerm(5, 4, (IEnumerable<int> seq) =>
            {
                Console.Write($"[{++index}] ");
                foreach(int data in seq)
                {
                    Console.Write($"{data} ");
                }
                Console.WriteLine();
            });

            perm.Perm();
        }
        #endregion

    }
}
