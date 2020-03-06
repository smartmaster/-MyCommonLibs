using System;
using System.Collections.Generic;
using System.IdentityModel.Selectors;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WcfTransportSecurityCommon
{
    public class MyCustomUserNameValidator : UserNamePasswordValidator
    {
 
        public override void Validate(string userName, string password)
        {
            if (null == userName || null == password)
            {
                throw new ArgumentNullException();
            }

            Console.WriteLine($"user={userName}, password={password}");
        }
    }


}
