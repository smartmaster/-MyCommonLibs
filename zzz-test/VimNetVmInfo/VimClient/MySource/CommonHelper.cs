
using System;
using System.Collections.Generic;
using System.IdentityModel.Selectors;
using System.IdentityModel.Tokens;
using System.IO;
using System.Net.Security;
using System.Runtime.Serialization;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using System.Reflection;
using System.Linq;
using System.Collections;

namespace VimClient
{
    static partial class StaticHelpers
    {
        public static void PrintException(Exception ex, int level)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append('-', level); sb.AppendFormat("{0}", ex.GetType().FullName); sb.AppendLine();
            sb.Append('-', level); sb.AppendFormat("{0}", ex.Message); sb.AppendLine();
            sb.Append('-', level); sb.AppendFormat("{0}", ex); sb.AppendLine();
            Console.WriteLine("{0}", sb.ToString());

            if (null != ex.InnerException)
            {
                PrintException(ex.InnerException, level + 1);
            }
        }

        public static string FormatThumbPrint(string thumbprint)
        {
            if (string.IsNullOrEmpty(thumbprint)) return string.Empty;

            StringBuilder sb = new StringBuilder();

            int index = 0;
            foreach(char ch in thumbprint)
            {
                sb.Append(ch);
                ++ index;
                if(0 == (index & 1) && index != thumbprint.Length)
                {
                    sb.Append(':');
                }
            }
            return sb.ToString();
        }
    }


    static class DeepClone
    {

        private static IEnumerable<object> CollectionToEnumerable(ICollection coll)
        {
            foreach (var item in coll)
            {
                yield return item;
            }
        }
        public static object Clone(object obj)
        {
            //////////////////////////////////////////////////////////////////////////
            if (null == obj)
            {
                return null;
            }

            //////////////////////////////////////////////////////////////////////////
            //if (obj is string || obj is decimal)
            //{
            //    return obj;
            //}


            //////////////////////////////////////////////////////////////////////////
            Type tt = obj.GetType();
#if true
            if (tt.IsPrimitive || tt == typeof(decimal) || tt == typeof(string) || tt == typeof(object) || tt.IsEnum)
            {
                return obj;
            }
#else // for test purpose
            if (tt.IsPrimitive)
            {
                return obj;
            }
            if (tt == typeof(decimal))
            {
                return obj;
            }
            if (tt == typeof(string))
            {
                return obj;
            }
            if (tt == typeof(object))
            {
                return obj;
            }
            if (tt.IsEnum)
            {
                return obj;
            }
#endif


            //////////////////////////////////////////////////////////////////////////
            if (tt.IsArray)
            {
                Array oldArr = (Array)(obj);
                Type itemType = tt.GetElementType();
                Array newArr = Array.CreateInstance(itemType, oldArr.Length);
                for (int ii = 0; ii < newArr.Length; ++ii)
                {
                    object oldItem = oldArr.GetValue(ii);
                    object newItem = Clone(oldItem);
                    newArr.SetValue(newItem, ii);
                }
                return newArr;
            }



            //////////////////////////////////////////////////////////////////////////
            ConstructorInfo conInfo = tt.GetConstructor(BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public, null, Type.EmptyTypes, null);
            object newObj = conInfo.Invoke(null);

            //////////////////////////////////////////////////////////////////////////
            if (obj is IDictionary)
            {
                IDictionary oldDic = (obj as IDictionary);
                IDictionary newDic = (newObj as IDictionary);
                var items = CollectionToEnumerable(oldDic.Keys).Zip(CollectionToEnumerable(oldDic.Values), (k, v) => new { k, v });
                foreach (var item in items)
                {
                    object newK = Clone(item.k);
                    object newV = Clone(item.v);
                    newDic.Add(newK, newV);
                }

                return newObj;
            }

            //////////////////////////////////////////////////////////////////////////
            if (obj is ICollection)
            {
                ICollection oldCollecion = (obj as ICollection);
                MethodInfo methodAdd = tt.GetMethod(nameof(ICollection<object>.Add), BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
                foreach (var item in oldCollecion)
                {
                    object newItem = Clone(item);
                    methodAdd.Invoke(newObj, new object[] { newItem });
                }

                return newObj;
            }

            //////////////////////////////////////////////////////////////////////////
            FieldInfo[] ffii = tt.GetFields(BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
            foreach (FieldInfo fi in ffii)
            {
                object oldValue = fi.GetValue(obj);
                object newValue = Clone(oldValue);
                fi.SetValue(newObj, newValue);
            }
            return newObj;
        }
    }

    static class TryCatchAll
    {
#if false
        public static Exception RunFunc<RetType>(Func<RetType> func, out RetType retValue)
        {
            Exception retEx = null;
            retValue = default(RetType);
            try
            {
                retValue = func();
            }
            catch(Exception ex)
            {
                retEx = ex;
            }
            return retEx;
        }
#endif

        public static Exception Run(Action action, bool toThrowExceprion = false)
        {
            Exception retEx = null;
            try
            {
                action();
            }
            catch (Exception ex)
            {
                StaticHelpers.PrintException(ex, 0);
                if (toThrowExceprion)
                {
                    throw;
                }
                retEx = ex;
            }
            return retEx;
        }
    }


    class MyCollectionDisposable : IDisposable
    {
        private List<Action> m_actions = new List<Action>();

       public int Add(Action action)
        {
            try
            {
                m_actions.Add(action);
                return m_actions.Count - 1;
            }
            catch (System.Exception ex)
            {
                StaticHelpers.PrintException(ex, 0);
                action?.Invoke();
                throw;
            }
        }

        public void Undo(int index)
        {
            m_actions[index] = null;
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
                    for (int ii = m_actions.Count - 1; ii >= 0; --ii)
                    {

                        try
                        {
                            m_actions[ii]?.Invoke();
                        }
                        catch (System.Exception ex)
                        {
                            Console.WriteLine($"Disposed action with exception {ex}");
                        }
                        finally
                        {
                            m_actions[ii] = null;
                        }
                      
                      
                    }
                    m_actions.Clear();
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~MyCollectionDisposable() {
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


#if false
    class MyDisposable : IDisposable
    {
        private Action _actionDispose;

        public MyDisposable(Action actionDispose)
        {
            _actionDispose = actionDispose;
        }

        public void Dispose()
        {
            Action actionDispose = _actionDispose;
            if (null != actionDispose)
            {
                actionDispose();
                _actionDispose = null;
            }
        }
    }

    public class MyEnterExit
    {
        private long _index = 0;

        public void Enter(string message)
        {
            ++_index;
            Console.WriteLine("+++++++++++++++++++++++++++++++++++++++++++++Enter[{0}] [{1}]+++++++++++++++++++++++++++++++++++++++++++++", _index, message);
        }

        public void Exit(string message)
        {
            Console.WriteLine("---------------------------------------------Exit[{0}] [{1}]---------------------------------------------", _index, message);
            --_index;
        }
    }

    public static class RunEnterExit
    {
        static void Run(MyEnterExit ee, string message, Action act)
        {
            ee.Enter(message);
            using (var MyDisposable = new MyDisposable(() => ee.Exit(message)))
            {
                act();
            }
        }
    }
#endif
   
   
    public class MyCertVerify
    {
        public string Thumbprint { get; private set; }
        public bool RemoteCertificateValidationCallback(
            object sender,
            X509Certificate certificate,
            X509Chain chain,
            SslPolicyErrors sslPolicyErrors
            )
        {
            X509Certificate2 cert2 = new X509Certificate2(certificate);
            Thumbprint = cert2.Thumbprint;
            return true;
        }
    }

    public class MyX509CertificateValidator : X509CertificateValidator
    {
        string allowedIssuerName;
        public MyX509CertificateValidator(string allowedIssuerName)
        {
            if (allowedIssuerName == null)
            {
                //throw new ArgumentNullException("allowedIssuerName");
            }

            this.allowedIssuerName = allowedIssuerName;
        }
        public override void Validate(X509Certificate2 certificate)
        {
            // Check that there is a certificate. 
            if (certificate == null)
            {
                throw new ArgumentNullException("certificate");
            }

            // Check that the certificate issuer matches the configured issuer 
            if (allowedIssuerName != null && allowedIssuerName != certificate.IssuerName.Name)
            {
                throw new SecurityTokenValidationException
                  ("Certificate was not issued by a trusted issuer");
            }
        }
    }

    public static class MyDataSerializer
    {
        public static void WriteObject<T_Object>(ref T_Object obj, string fileName)
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Create))
            using (XmlDictionaryWriter writer = XmlDictionaryWriter.CreateTextWriter(fs, Encoding.UTF8))
            {
                DataContractSerializer ser = new DataContractSerializer(typeof(T_Object));
                ser.WriteObject(writer, obj);
                writer.Close();
                fs.Close();
            }
        }

        public static void ReadObject<T_Object>(out T_Object obj, string fileName)
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Open))
            using (XmlDictionaryReader reader = XmlDictionaryReader.CreateTextReader(fs, new XmlDictionaryReaderQuotas()))
            {
                DataContractSerializer ser = new DataContractSerializer(typeof(T_Object));
                obj = (T_Object)ser.ReadObject(reader, true);
                reader.Close();
                fs.Close();
            }
        }

        public static void WriteObjectXml<T_Object>(ref T_Object obj, string fileName)
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Create))
            using (XmlDictionaryWriter writer = XmlDictionaryWriter.CreateTextWriter(fs, Encoding.UTF8))
            {
                XmlSerializer ser = new XmlSerializer(typeof(T_Object));
                ser.Serialize(writer, obj);
                writer.Close();
                fs.Close();
            }
        }

        public static void ReadObjectXml<T_Object>(out T_Object obj, string fileName)
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Open))
            using (XmlDictionaryReader reader = XmlDictionaryReader.CreateTextReader(fs, new XmlDictionaryReaderQuotas()))
            {
                XmlSerializer ser = new XmlSerializer(typeof(T_Object));
                obj = (T_Object)ser.Deserialize(reader);
                reader.Close();
                fs.Close();
            }
        }
    }
}