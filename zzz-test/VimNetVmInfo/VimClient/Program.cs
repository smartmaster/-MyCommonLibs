using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.ServiceModel;
using System.ServiceModel.Security;
using System.Text;
using System.Threading.Tasks;




namespace VimClient
{
    class Program
    {
        static int Main(string[] args)
        {
            int testCase = 0;
            switch (testCase)
            {
                case 1:
                    {
                        using (MyCollectionDisposable cd = new MyCollectionDisposable())
                        {
                            const int TOTAL_COUNT = 10;
                            for (int ii = 0; ii < TOTAL_COUNT; ++ ii)
                            {
                                int temp = ii;
                                cd.Add(()=>
                                {
                                    Console.WriteLine($"{temp}");
                                });
                            }

                            const int STEP = 2;
                            for (int ii = 0; ii < TOTAL_COUNT; ii += STEP)
                            {
                                cd.Undo(ii);
                            }
                        }
                    }
                    break;
                case 0:
                    {
                        if(args.Length < 7)
                        {
                            Console.WriteLine("this.exe <Protocal> <ServerName> <Port> <User> <Password> <VmPath> <ToListAllVMs>");
                            Console.WriteLine("Protocal - http or https");
                            Console.WriteLine("ServerName - vcenter or esxi");
                            Console.WriteLine("Port");
                            Console.WriteLine("User");
                            Console.WriteLine("Password");
                            Console.WriteLine("VmPath - for example '[datastore2-9T-raid0] sonmi01-004-rps2-veeam/sonmi01-004-rps2-veeam.vmx'");
                            Console.WriteLine("ToListAllVMs - 1 or 0");
                            Console.WriteLine("");
                            return -1;
                        }
                        int index = 0;
                        MainSearchVM_In inParam = new MainSearchVM_In()
                        {
                            Protocal = args[index++],
                            ServerName = args[index++],
                            Port = Int32.Parse(args[index++]),
                            User = args[index++],
                            Password = args[index++],
                            VmPath = args[index++],
                            ToListAllVMs = Int32.Parse(args[index++]) != 0
                        };

                        MainSearchVM_Out outRet = new MainSearchVM_Out();

                        MyTestCases.MainFindVM(inParam, outRet);
                    }
                    break;
            }
            return 0;
        }
    }
}
