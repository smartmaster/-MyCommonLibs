using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.Text;
using System.Threading.Tasks;

namespace WcfTransportSecurityCommon
{
    public class CookiePair
    {
        public string Name { get; set; }
        public string Value { get; set; }
    }

    public static class WcfCookieExtension
    {
        private static string CookiePairsToString(this IEnumerable<CookiePair> pairs)
        {
            StringBuilder sb = new StringBuilder(1024);
            foreach(var pair in pairs)
            {
                sb.AppendFormat("{0}={1}; ", pair.Name, pair.Value);
            }

            return sb.ToString();
        }

        public static void SetAllCookies(this OperationContext oc, IEnumerable<CookiePair> pairs)
        {
            HttpResponseMessageProperty rsp = null;
            if (oc.OutgoingMessageProperties.ContainsKey(HttpResponseMessageProperty.Name))
            {
                rsp = (oc.OutgoingMessageProperties[HttpResponseMessageProperty.Name] as HttpResponseMessageProperty);
            }
            else
            {
                rsp = new HttpResponseMessageProperty();
                oc.OutgoingMessageProperties.Add(HttpResponseMessageProperty.Name, rsp);
            }

            var headers = rsp.Headers;
            headers[System.Net.HttpResponseHeader.SetCookie] = pairs.CookiePairsToString();
        }

        public static IEnumerable<CookiePair> GetAllCookies(this OperationContext oc)
        {
            var request = (oc.IncomingMessageProperties[HttpRequestMessageProperty.Name] as HttpRequestMessageProperty);
            var cookies = request.Headers[System.Net.HttpRequestHeader.Cookie]?.Split(';', ' ', '\t', '\r' ,'\n');

            var pairs = from cookie in (cookies ?? Enumerable.Empty<string>())
                        let pair = cookie.Split('=')
                        select new CookiePair { Name = pair[0], Value = pair[1] };

            return pairs;
        }
        public static string GetCookie(this OperationContext oc, string name)
        {
            var value = from pair in oc.GetAllCookies()
                        where string.Compare(pair.Name, name, true) == 0
                        select pair.Value;

            return value.FirstOrDefault();
        }

        public static void SetCookie(this OperationContext oc, string name, string value)
        {
            var cookies = oc.GetAllCookies().
                Where(cp => string.Compare(cp.Name, name, true) != 0).
                Append(new CookiePair { Name = name, Value = value });
            oc.SetAllCookies(cookies);
        }
    }
}
