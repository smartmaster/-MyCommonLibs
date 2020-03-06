using System;
using System.Collections.Generic;
using System.Text;

namespace TestCsAllInOne
{
    static class FindFirstUnique
    {
        static
        public int FirstUniqChar(string str)
        {
            const int SIZE = 128;
            int[] pos = new int[SIZE];  // 0: no element; -1: duplicated element; >0: position of unique char

            for(int ii = 0; ii < str.Length; ++ ii)
            {
                int tempindex = str[ii];
                if(pos[tempindex] == 0)
                {
                    pos[tempindex] = ii + 1;
                }
                else
                {
                    pos[tempindex] = -1;
                }
            }

            
            int ret = -1;
            int jj = 0;
            for (; jj < SIZE; ++jj)
            {
                if(pos[jj] > 0)
                {
                    ret = pos[jj];
                    break;
                }
            }

            for(int kk = jj + 1; kk < SIZE; ++ kk)
            {
                int temppos = pos[kk];
                if(temppos > 0 && ret > temppos)
                {
                    ret = temppos;
                }
            }


            return ret > 0 ? (ret - 1) : -1;
        }

    }
}
