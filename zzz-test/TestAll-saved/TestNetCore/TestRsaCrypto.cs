using System;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace TestNetCore
{

    public interface ICrypto
    {
        byte[] Enc(byte[] plain);
        byte[] Dec(byte[] secret);


        string Enc(string plain);
        string Dec(string secret);


        byte[] PublicKey { get; }
        byte[] PrivateKey { get;}
    }
    class MyRsaCrypto : ICrypto, IDisposable
    {
        private readonly RSACryptoServiceProvider _rsa = new RSACryptoServiceProvider();


        public byte[] PublicKey
        {
            get
            {
                return _rsa.ExportCspBlob(false);
            }
        }
        public byte[] PrivateKey
        {
            get
            {
                return _rsa.ExportCspBlob(true);
            }
        }

        public byte[] Dec(byte[] secret)
        {
            using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider())
            {
                rsa.ImportCspBlob(PrivateKey);
                return rsa.Decrypt(secret, true);
            }
        }

        public string Dec(string secret)
        {
            //base64 --> enc --> utf8 --> string
            byte[] enc = Convert.FromBase64String(secret);
            byte[] utf8 = Dec(enc);
            return Encoding.UTF8.GetString(utf8);
        }

        public byte[] Enc(byte[] plain)
        {
            using (RSACryptoServiceProvider rsa = new RSACryptoServiceProvider())
            {
                rsa.ImportCspBlob(PublicKey);
                return rsa.Encrypt(plain, true);
            }
        }

        public string Enc(string plain)
        {
            //string --> utf8 --> enc --> base64
            byte[] utf8 = Encoding.UTF8.GetBytes(plain);
            byte[] enc = Enc(utf8);
            return Convert.ToBase64String(enc);
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
                    _rsa?.Dispose();
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~MyRsaCrypto() {
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


    class MyAesCrypto : ICrypto, IDisposable
    {
        private readonly AesCryptoServiceProvider _aes = new AesCryptoServiceProvider();

        public byte[] PublicKey
        {
            get
            {
                return _aes.IV;
            }
        }

        public byte[] PrivateKey
        {
            get
            {
                return _aes.Key;
            }
        }

        public byte[] Dec(byte[] secret)
        {
            using (AesCryptoServiceProvider aes = new AesCryptoServiceProvider())
            {
                aes.IV = PublicKey;
                aes.Key = PrivateKey;
                //aes.Padding = PaddingMode.ANSIX923;

                using (ICryptoTransform trans = aes.CreateDecryptor())
                using (MemoryStream ms = new MemoryStream(secret))
                using (CryptoStream cs = new CryptoStream(ms, trans, CryptoStreamMode.Read, true))
                using(MemoryStream plainMs = new MemoryStream())
                {
                    cs.CopyTo(plainMs);
                    return plainMs.ToArray();
                }
            }
        }

        public string Dec(string secret)
        {
            //base64 --> enc --> utf8 --> string
            byte[] enc = Convert.FromBase64String(secret);
            byte[] utf8 = Dec(enc);
            return Encoding.UTF8.GetString(utf8);
        }

        public byte[] Enc(byte[] plain)
        {
            using (AesCryptoServiceProvider aes = new AesCryptoServiceProvider())
            {
                aes.IV = PublicKey;
                aes.Key = PrivateKey;
                //aes.Padding = PaddingMode.ANSIX923;

                using (ICryptoTransform trans = aes.CreateDecryptor())
                using (MemoryStream secretMs = new MemoryStream())
                using (CryptoStream cs = new CryptoStream(secretMs, trans, CryptoStreamMode.Write, true))
                using (MemoryStream plainMs = new MemoryStream(plain))
                {
                    plainMs.CopyTo(cs);
                    return secretMs.ToArray();
                }
            }
        }

        public string Enc(string plain)
        {
            //string --> utf8 --> enc --> base64
            byte[] utf8 = Encoding.UTF8.GetBytes(plain);
            byte[] enc = Enc(utf8);
            return Convert.ToBase64String(enc);
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
                    _aes?.Dispose();
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                // TODO: set large fields to null.

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~MyAecCrypto() {
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


    static class MyRsaCryptoTest
    {
        public static void Case0(ICrypto crypto)
        {
           
                string plain = "看韩剧发绥化市访客富裕一如衣服";
                Console.WriteLine($@"plain: {plain}");
                string secret = crypto.Enc(plain);
                Console.WriteLine($@"secret: {secret}");
                string plain2 = crypto.Dec(secret);
                Console.WriteLine($@"plain2: {plain2}");
        }

        public static void Case1()
        {
            using (var crypto = new MyRsaCrypto())
            {
                Case0(crypto);
            }


            using (var crypto = new MyAesCrypto())
            {
                Case0(crypto);
            }
        }


        
    }
}
