using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.Text;
using System.Threading;
using System.Web;

using WcfTransportSecurityCommon;

namespace WcfTransportSecurityLibrary
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Service1" in both code and config file together.
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall, ConcurrencyMode = ConcurrencyMode.Single)]
    public class MyDataService : IMyDataService
    {
        static int s_instanceId = 0;

        private int _instanceId;

        private const string COOKIE_NAME = "smart-wcf-session-object";

        static IWcfObjectRepository<int[]> _objRepo = new WcfObjectRepository<int[]>((long)(TimeSpan.FromMinutes(15).TotalMilliseconds));

        public MyDataService()
        {
            _instanceId = Interlocked.Increment(ref s_instanceId);
        }


        private Object GetObject()
        {
            string guid = OperationContext.Current.GetCookie(COOKIE_NAME);
            int[] rv = null;
            if(string.IsNullOrEmpty(guid))
            {
                rv = new int[1];
                guid = _objRepo.Add(rv, (long)(TimeSpan.FromHours(1).TotalMilliseconds));
                OperationContext.Current.SetCookie(COOKIE_NAME, guid);
            }
            else
            {
                rv = _objRepo.Get(guid);
            }


            return rv;
        }

        private void RemoveObject()
        {
            string guid = OperationContext.Current.GetCookie(COOKIE_NAME);

            if (!string.IsNullOrEmpty(guid))
            {
                _objRepo.Delete(guid);
            }
           
            OperationContext.Current.SetAllCookies(
                OperationContext.Current.GetAllCookies().
                Where(cp => string.Compare(cp.Name, COOKIE_NAME, true) != 0)
                );
        }

        

        public void Begin()
        {
            GetObject();

            //#region TEST 
            //const string COOKIE_NAME_TEST = "hello";
            //const int COOKIE_VALUE_TEST = 100;

            //OperationContext oc = OperationContext.Current;
            //string cookieValueTest = oc.GetCookie(COOKIE_NAME_TEST);
            //if (string.IsNullOrEmpty(cookieValueTest))
            //{
            //    oc.SetCookie(COOKIE_NAME_TEST, COOKIE_VALUE_TEST.ToString());
            //    Console.WriteLine($"set cookie value {COOKIE_VALUE_TEST}");
            //}
            //else
            //{
            //    Console.WriteLine($"cookie value is {cookieValueTest}");
            //    int intCookieValueTest = int.Parse(cookieValueTest) + 1;
            //    oc.SetCookie(COOKIE_NAME_TEST, intCookieValueTest.ToString());
            //}
            //#endregion
        }

        public void End()
        {
            //(_objRepo as IDisposable).Dispose(); //for test purpose only
            RemoveObject();
        }

        public MyData GetData(MyData myData)
        {
            OperationContext oc = OperationContext.Current;

            var remote_ep = (oc.IncomingMessageProperties[RemoteEndpointMessageProperty.Name] as RemoteEndpointMessageProperty);
            string ipport = remote_ep.Address.Any(ch => ch == ':') ? $"[{remote_ep.Address}]:{remote_ep.Port}" : $"{remote_ep.Address}:{remote_ep.Port}";
            Console.WriteLine($"Instance={_instanceId}, GetData called by client={ipport}, SessionId={oc.SessionId}");

            //#region TEST
            //const string COOKIE_NAME_TEST = "hello";
            //const int COOKIE_VALUE_TEST = 100;

            //string cookieValueTest = oc.GetCookie(COOKIE_NAME_TEST);
            //if(string.IsNullOrEmpty(cookieValueTest))
            //{
            //    oc.SetCookie(COOKIE_NAME_TEST, COOKIE_VALUE_TEST.ToString());
            //    Console.WriteLine($"set cookie value {COOKIE_VALUE_TEST}");
            //}
            //else
            //{
            //    Console.WriteLine($"cookie value is {cookieValueTest}");
            //    int intCookieValueTest = int.Parse(cookieValueTest) + 1;
            //    oc.SetCookie(COOKIE_NAME_TEST, intCookieValueTest.ToString());
            //}
            //#endregion

            int[] arr = (GetObject() as int[]);
            Console.WriteLine($"Session object data {arr[0]}");
            arr[0] += 10000;
            

            //var cookies = HttpContext.Current?.Request.Cookies;
            //var cookie = cookies.Get(COOKIE_NAME);
            //Console.WriteLine($"Cookie is {cookie.Name}, {cookie.Value}");

            MyData rv = new MyData
            {
                Offset = arr[0] + myData.Offset,
                Length = arr[0] + myData.Length,
                Name = $"hello {myData.Name}, serverData={arr[0]}",
                Buffer = UTF8Encoding.UTF8.GetBytes(UTF8Encoding.UTF8.GetString(myData.Buffer).ToUpperInvariant())
            };

            return rv;
        }
    }
}
