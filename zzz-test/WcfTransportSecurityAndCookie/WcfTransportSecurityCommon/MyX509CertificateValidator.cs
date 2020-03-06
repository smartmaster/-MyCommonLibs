using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IdentityModel.Selectors;
using System.Security.Cryptography.X509Certificates;

namespace WcfTransportSecurityCommon
{
  public  class MyX509CertificateValidator : X509CertificateValidator
    {

        public MyX509CertificateValidator()
        {
        }
        public override void Validate(X509Certificate2 certificate)
        {
            Console.WriteLine($"{nameof(certificate.Issuer)}={certificate.Issuer}, {nameof(certificate.Subject)}={certificate.Subject}, {nameof(certificate.Thumbprint)}={certificate.Thumbprint}");
        }
    }
}
