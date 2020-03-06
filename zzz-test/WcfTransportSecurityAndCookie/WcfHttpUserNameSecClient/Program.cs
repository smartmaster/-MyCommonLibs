using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WcfTransportSecurityCommon;

namespace WcfHttpUserNameSecClient
{
    class Program
    {
        static async Task Main(string[] args)
        {
           // System.Net.ServicePointManager.ServerCertificateValidationCallback +=
           //(sender, certificate, chain, sslpolicyerrors) =>
           //{
           //    var cert2 = (certificate as System.Security.Cryptography.X509Certificates.X509Certificate2);
           //    Console.WriteLine($"{nameof(certificate.Issuer)}={certificate.Issuer}, {nameof(certificate.Subject)}={certificate.Subject}, {nameof(cert2.Thumbprint)}={cert2.Thumbprint}");
           //    return true;
           //};


            using (MyDataServiceReference.MyDataServiceClient client = new MyDataServiceReference.MyDataServiceClient())
            {
                //client.ClientCredentials.ClientCertificate.Certificate = new System.Security.Cryptography.X509Certificates.X509Certificate2(@"f:\smart-wcf-client.p12", "ccc");

                //var serverCert = client.ClientCredentials.ServiceCertificate;
                //serverCert.Authentication.CertificateValidationMode = System.ServiceModel.Security.X509CertificateValidationMode.Custom;
                //serverCert.Authentication.CustomCertificateValidator = new MyX509CertificateValidator();
                //serverCert.Authentication.RevocationMode = System.Security.Cryptography.X509Certificates.X509RevocationMode.NoCheck;

                //client.ClientCredentials.ServiceCertificate.SslCertificateAuthentication = serverCert.Authentication;
                //client.ClientCredentials.ServiceCertificate.SslCertificateAuthentication = new System.ServiceModel.Security.X509ServiceCertificateAuthentication
                //{
                //    CertificateValidationMode = System.ServiceModel.Security.X509CertificateValidationMode.Custom,
                //    CustomCertificateValidator = new MyX509CertificateValidator(),
                //    RevocationMode = System.Security.Cryptography.X509Certificates.X509RevocationMode.NoCheck,
                //    TrustedStoreLocation = System.Security.Cryptography.X509Certificates.StoreLocation.LocalMachine
                //};

                client.ClientCredentials.ServiceCertificate.SslCertificateAuthentication = client.ClientCredentials.ServiceCertificate.Authentication;

                client.ClientCredentials.UserName.UserName = "everyone";
                client.ClientCredentials.UserName.Password = "password";

                await client.BeginAsync();

                MyDataServiceReference.MyData myData = new MyDataServiceReference.MyData()
                {
                    Offset = 100,
                    Length = 200,
                    Name = "Captain Jack",
                    Buffer = UTF8Encoding.UTF8.GetBytes("hello world")
                };


                const int LOOP_COUNT = 5;
                for (int ii = 0; ii < LOOP_COUNT; ++ii)
                {
                    MyDataServiceReference.MyData rv = await client.GetDataAsync(myData);
                    Console.WriteLine($"{nameof(rv.Offset)}={rv.Offset}, {nameof(rv.Length)}={rv.Length}, {nameof(rv.Name)}={rv.Name}, {nameof(rv.Buffer)}={UTF8Encoding.UTF8.GetString(rv.Buffer)}");
                }

                await client.EndAsync();
            }
        }
    }
}
