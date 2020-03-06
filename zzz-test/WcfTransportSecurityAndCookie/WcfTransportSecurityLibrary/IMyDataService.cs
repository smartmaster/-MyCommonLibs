using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace WcfTransportSecurityLibrary
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IService1" in both code and config file together.
    [ServiceContract(SessionMode = SessionMode.NotAllowed)]
    public interface IMyDataService
    {
        [OperationContract]
        void Begin();

        [OperationContract]
        MyData GetData(MyData myData);


        [OperationContract]
        void End();
    }

    // Use a data contract as illustrated in the sample below to add composite types to service operations.
    // You can add XSD files into the project. After building the project, you can directly use the data types defined there, with the namespace "WcfTransportSecurityServiceLibrary.ContractType".
    [DataContract]
    public class MyData
    {
        [DataMember(Order = 100)]
        public long Offset { get; set; }
        [DataMember(Order = 200)]
        public long Length { get; set; }
        [DataMember(Order = 300)]
        public string Name { get; set; }
        [DataMember(Order = 400)]
        public byte[] Buffer { get; set; }
    }
}
