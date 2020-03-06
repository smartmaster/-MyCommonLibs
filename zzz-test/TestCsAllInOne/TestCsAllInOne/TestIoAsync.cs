


using System;
using System.IO;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;

namespace TestCs
{
    public static class TestIoAsync
    {
        public static async Task IoAsync(string filePath)
        {
            FileStream fs = new FileStream(
                filePath,//string path, 
                FileMode.Create,//FileMode mode, 
                FileAccess.Write,//FileAccess access, 
                FileShare.None,//FileShare share, 
                4096,//int bufferSize, 
                FileOptions.Asynchronous//,//FileOptions options
                );

            byte[] buffer = new byte[4095];
            await fs.WriteAsync(buffer, 0, buffer.Length);

            await fs.FlushAsync();

            fs.Dispose();
        }


        public class IoAsyncMachine : IAsyncStateMachine
        {
            private string _filePath;
            private byte[] _buffer;
            private FileStream _fs;

            private int _state = 0;
            private AsyncTaskMethodBuilder _atm = AsyncTaskMethodBuilder.Create();
            private TaskAwaiter _awaiter = default;


            private IoAsyncMachine(string filePath)
            {
                _filePath = filePath;
            }

            void IAsyncStateMachine.MoveNext()
            {
                try
                {
                    switch (_state)
                    {
                        case 0:
                            {
                                _state = 1;
                                _fs = new FileStream(
                                    _filePath,//string path, 
                                    FileMode.Create,//FileMode mode, 
                                    FileAccess.Write,//FileAccess access, 
                                    FileShare.None,//FileShare share, 
                                    4096,//int bufferSize, 
                                    FileOptions.Asynchronous//,//FileOptions options
                                    );
                                _buffer = new byte[4097];
                                Task twrite = _fs.WriteAsync(_buffer, 0, _buffer.Length);
                                _awaiter = twrite.GetAwaiter();
                                if(_awaiter.IsCompleted)
                                {
                                    goto case 1;
                                }
                                else
                                {
                                    IoAsyncMachine self = this;
                                    _atm.AwaitUnsafeOnCompleted(ref _awaiter, ref self);
                                }
                            }
                            break;

                        case 1:
                            {
                                _state = 2;
                                Task tflush = _fs.FlushAsync();
                                _awaiter = tflush.GetAwaiter();
                                if (_awaiter.IsCompleted)
                                {
                                    goto case 2;
                                }
                                else
                                {
                                    IoAsyncMachine self = this;
                                    _atm.AwaitUnsafeOnCompleted(ref _awaiter, ref self);
                                }
                            }
                            break;

                        case 2:
                            {
                                _state = 3;
                                _fs.Dispose();
                                _atm.SetResult();
                            }
                            break;

                        case 3:
                            {
                                _state = 4;
                                throw new Exception("should not go here!");
                            }
                            break;
                    }
                }
                catch(Exception ex)
                {
                    _atm.SetException(ex);
                }
                
            }

            void IAsyncStateMachine.SetStateMachine(IAsyncStateMachine stateMachine)
            {
                throw new NotImplementedException();
            }

            private Task Start()
            {
                IoAsyncMachine self = this;
                _atm.Start(ref self);
                return _atm.Task;
            }


            public static Task IoAsync(string filePath)
            {
                var stateMachine = new IoAsyncMachine(filePath);
                return stateMachine.Start();
            }
        }
    }
}