using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace TestCs
{
    static class UriEscapeChar
    {
        public static void Case0()
        {
            //string str = @"\\10.57.15.25\shareed\Program Files\Common%20Files";
            string str = @"\\10.57.15.25\shareed\Program Files\!#$&'()*+,/:;=?@[]%";
            Uri uri = new Uri(str);
            Console.WriteLine($"{str}");
            Console.WriteLine($"{uri}");
            Console.WriteLine($"{uri.LocalPath}");


            try
            {
                string xmlpath = @"\\10.57.54.219\udp_ds100000\shuli02-test%20(test)@ctcvc01[500567ef-0bef-9370-4f8c-3bdf1015a04e]\VStore\S0000000001\VMSnapshotConfigInfo.vsci";
                var doc = new XmlDocument();
                doc.Load(xmlpath);
            }
            catch(Exception ex)
            {
                Console.WriteLine($"{ex.Message}");
            }
          }

            
    }
}
