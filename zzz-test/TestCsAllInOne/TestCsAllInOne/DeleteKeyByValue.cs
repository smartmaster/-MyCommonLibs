using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Text;

namespace TestCsAllInOne
{
    //https://www.codeproject.com/Articles/16343/Copy-and-Rename-Registry-Keys
    // RenameRegistryKey © Copyright 2006 Active Computing
    namespace RenameRegistryKey
    {
    
        public static class RegistryUtilities
        {
            /// <summary>
            /// Renames a subkey of the passed in registry key since 
            /// the Framework totally forgot to include such a handy feature.
            /// </summary>
            /// <param name="regKey">The RegistryKey that contains the subkey 
            /// you want to rename (must be writeable)</param>
            /// <param name="subKeyName">The name of the subkey that you want to rename
            /// </param>
            /// <param name="newSubKeyName">The new name of the RegistryKey</param>
            /// <returns>True if succeeds</returns>
            public static bool RenameSubKey(RegistryKey parentKey,
                string subKeyName, string newSubKeyName)
            {
                CopyKey(parentKey, subKeyName, newSubKeyName);
                parentKey.DeleteSubKeyTree(subKeyName);
                return true;
            }

            /// <summary>
            /// Copy a registry key.  The parentKey must be writeable.
            /// </summary>
            /// <param name="parentKey"></param>
            /// <param name="keyNameToCopy"></param>
            /// <param name="newKeyName"></param>
            /// <returns></returns>
            public static bool CopyKey(RegistryKey parentKey,
                string keyNameToCopy, string newKeyName)
            {
                //Create new key
                RegistryKey destinationKey = parentKey.CreateSubKey(newKeyName);

                //Open the sourceKey we are copying from
                RegistryKey sourceKey = parentKey.OpenSubKey(keyNameToCopy);

                RecurseCopyKey(sourceKey, destinationKey);

                return true;
            }

            private static void RecurseCopyKey(RegistryKey sourceKey, RegistryKey destinationKey)
            {
                //copy all the values
                foreach (string valueName in sourceKey.GetValueNames())
                {
                    object objValue = sourceKey.GetValue(valueName);
                    RegistryValueKind valKind = sourceKey.GetValueKind(valueName);
                    destinationKey.SetValue(valueName, objValue, valKind);
                }

                //For Each subKey 
                //Create a new subKey in destinationKey 
                //Call myself 
                foreach (string sourceSubKeyName in sourceKey.GetSubKeyNames())
                {
                    RegistryKey sourceSubKey = sourceKey.OpenSubKey(sourceSubKeyName);
                    RegistryKey destSubKey = destinationKey.CreateSubKey(sourceSubKeyName);
                    RecurseCopyKey(sourceSubKey, destSubKey);
                }
            }
        }
    }


    public class DeleteKeyByValueTest
    {
        public static void DeleteKeyByValueAPI(string topKeyName, string rootPath, string valueText, bool toDelete, bool ignoreCase)
        {
            using (var delkeyByVal = new DeleteKeyByValue(topKeyName, rootPath, valueText, toDelete, ignoreCase))
            {
                delkeyByVal.Run();
            }
        }
    }
    class DeleteKeyByValue : IDisposable
    {
        bool _ignoreCase = false;
        bool _toDelete = false;
        string _rootPath;
        string _valueText;
        RegistryKey _topKey;
        RegistryKey _root; //to dispose


        //static string ToLowerAZ(this string str)
        //{
        //    StringBuilder sb = new StringBuilder(str.Length + 1);
        //    foreach (var ch in str)
        //    {
        //        const int delta = 'a' - 'A';
        //        if (ch >= 'A' && ch <= 'Z')
        //        {
        //            sb.Append((char)(ch + delta));
        //        }
        //        else
        //        {
        //            sb.Append(ch);
        //        }
        //    }
        //    return sb.ToString();
        //}

        //only handle A-Z
        static string MyAZToLower(ReadOnlySpan<char> buffer)
        {
            StringBuilder sb = new StringBuilder(buffer.Length + 1);
            foreach(var ch in buffer)
            {
                const int delta = 'a' - 'A';
                if(ch >= 'A' && ch <= 'Z')
                {
                    sb.Append((char)(ch + delta));
                }
                else
                {
                    sb.Append(ch);
                }
            }


            return sb.ToString();
        }

        static Dictionary<string, RegistryKey> _dict = new Dictionary<string, RegistryKey>
        {
            [MyAZToLower(nameof(Registry.ClassesRoot))] = Registry.ClassesRoot,
            [MyAZToLower(nameof(Registry.CurrentConfig))] = Registry.CurrentConfig,
            [MyAZToLower(nameof(Registry.CurrentUser))] = Registry.CurrentUser,
            [MyAZToLower(nameof(Registry.LocalMachine))] = Registry.LocalMachine,
            [MyAZToLower(nameof(Registry.PerformanceData))] = Registry.PerformanceData,
            [MyAZToLower(nameof(Registry.Users))] = Registry.Users,
        };

        private static RegistryKey GetTopKeyByName(string topKeyName)
        {
            string lowerKeyName = MyAZToLower(topKeyName);
            return _dict.ContainsKey(lowerKeyName)? _dict[lowerKeyName] : Registry.LocalMachine;
        }

        public DeleteKeyByValue(string topKeyName, string rootPath, string valueText, bool toDelete, bool ignoreCase)
        {
            _topKey = GetTopKeyByName(topKeyName);
            _rootPath = rootPath;
            _valueText = valueText;
            _toDelete = toDelete;
            _ignoreCase = ignoreCase;
        }


        bool MyContainsString(string str1, string str2)
        {
            bool ret = false;
            if(_ignoreCase)
            {
                ret = MyAZToLower(str1).Contains(MyAZToLower(str2));
            }
            else
            {
                ret = str1.Contains(str2);
            }

            return ret;
        }

      private  bool FindValue(string subKeyName)
        {
            using (RegistryKey subKey = _root.OpenSubKey(subKeyName, false))
            {
                bool found = false;
                foreach (var valueName in subKey.GetValueNames())
                {
                    if (subKey.GetValue(valueName) is string valueData)
                    {
                        if(MyContainsString(valueData, _valueText))
                        {
                            Console.WriteLine($@"\{_rootPath}\{subKeyName}");
                            Console.WriteLine($@"{valueName} = {valueData}");
                            Console.WriteLine();
                            found = true;
                        }
                    }
                }

                return found;
            }
        }

       public void Run()
        {
            _root = _topKey.OpenSubKey(_rootPath, _toDelete);
            foreach(var subKeyName in _root.GetSubKeyNames())
            {
                if(FindValue(subKeyName))
                {
                    if (_toDelete)
                    {
                        Console.WriteLine($"Deleting {subKeyName}");
                        Console.WriteLine();
                        _root.DeleteSubKeyTree(subKeyName);
                    }
                }
            }
                
        }


        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects).
                    _root?.Dispose();
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~DeleteKeyByValue() {
        //   // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
        //   Dispose(false);
        // }

        // This code added to correctly implement the disposable pattern.
        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
            Dispose(true);
            // TODO: uncomment the following line if the finalizer is overridden above.
            // GC.SuppressFinalize(this);
        }
        #endregion


    }
}
