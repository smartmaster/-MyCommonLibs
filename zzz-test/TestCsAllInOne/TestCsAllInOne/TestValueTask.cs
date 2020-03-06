using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestCsAllInOne
{
    class TestValueTask
    {
        private static int FillBufferProc(char[] buffer, int sleepMilSec)
        {
            int sleepTotal = 0;
            Random ran = new Random();
            for(int ii = 0; ii < buffer.Length; ++ ii)
            {
                Thread.Sleep(sleepMilSec);
                sleepTotal += sleepMilSec;
                buffer[ii] = (char)('A' + ran.Next() % 26);
            }
            return sleepTotal;
        }

        private const int BUFFER_SIZE = 26;
        private readonly char[] _buffer = new char[BUFFER_SIZE];
        int _pos = BUFFER_SIZE;


        private Task<int> FillBuffer()
        {
            int sleepMilSec = 1_000;
            Func<int> func = new Func<int>(() => FillBufferProc(_buffer, sleepMilSec));
            var tt = Task<int>.Factory.StartNew(func, TaskCreationOptions.LongRunning);
            return tt;
        }

        public async ValueTask<char> ReadChar()
        {
            if(_pos == BUFFER_SIZE)
            {
                await FillBuffer();
                _pos = 0;
            }

            return _buffer[_pos++];
        }

        public static async Task Case0()
        {
            TestValueTask tvt = new TestValueTask();
            for(int ii = 0; ii < 100; ++ ii)
            {
                char ch = await tvt.ReadChar();
                Console.WriteLine($"read '{ch}' at {DateTime.Now}");
            }
        }
    }
}
