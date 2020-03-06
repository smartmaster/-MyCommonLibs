using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Blob;
using Microsoft.WindowsAzure.Storage.Table;
using System;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace TestAzureStorage
{

    public class BaseEntity : TableEntity
    {
        public bool IsDeleted { get; set; }
        public DateTime CreatedDate { get; set; }
        public DateTime UpdatedDate { get; set; }
        public string CreatedBy { get; set; }
        public string UpdatedBy { get; set; }
    }


    public class Book : BaseEntity
    {
        public Book()
        {
        }
        public Book(int bookid, string publisher)
        {
            this.RowKey = bookid.ToString();
            this.PartitionKey = publisher;
        }
        public int BookId { get; set; }
        public string BookName { get; set; }
        public string Author { get; set; }
        public string Publisher { get; set; }
    }


    class Program
    {
        const string connectionString =
        @"DefaultEndpointsProtocol=https;" +
        @"AccountName=testscxx;" +
        @"AccountKey=HMCHHQvLEhBtSe08lNF8QC8Pk37e0vMrn+Eky/AkikJZCTs9Lak9cg8y/0DQuQUOtx1KbhNB1i90y9Bjg3aWxQ==;" +
        @"EndpointSuffix=core.windows.net";


        static async Task BlobAsync()
        {
            try
            {
                CloudStorageAccount sa = CloudStorageAccount.Parse(connectionString);
                CloudBlobClient bc = sa.CreateCloudBlobClient();

                const string CON_NAME = "smart-blob-container";
                CloudBlobContainer con = bc.GetContainerReference(CON_NAME);
                bool rr = await con.CreateIfNotExistsAsync();

                const string blobName = "my.txt";
                CloudPageBlob blob = con.GetPageBlobReference(blobName);

                bool del = await blob.DeleteIfExistsAsync();
                await blob.CreateAsync(64*1024, AccessCondition.GenerateIfNotExistsCondition(), new BlobRequestOptions(), new OperationContext());

                using (CloudBlobStream blobStream = await blob.OpenWriteAsync(null, AccessCondition.GenerateEmptyCondition(), new BlobRequestOptions(), new OperationContext()))
                {
                    byte[] sector = new byte[512];
                    for(int ii = 0; ii < sector.Length; ++ ii){sector[ii] = (byte)(ii%26 + 'A');}
                    MemoryStream mm = new MemoryStream(sector);
                    //byte[] buffer = Encoding.UTF8.GetBytes("zzz宏发科技恢复健康哈尔月UI风雅颂的法尔加zzz----");
                    //await blobStream.WriteAsync(buffer, 0, buffer.Length);
                    //await blobStream.WriteAsync(sector, 0, sector.Length - buffer.Length);
                    await blobStream.WriteAsync(sector, 0, sector.Length);
                    if (blobStream.CanSeek)
                    {
                        blobStream.Seek(1024, System.IO.SeekOrigin.Begin);
                    }

                    //buffer = Encoding.UTF8.GetBytes("this is some test");
                    //await blobStream.WriteAsync(buffer, 0, buffer.Length);
                    //await blobStream.WriteAsync(sector, 0, sector.Length - buffer.Length);
                    await blobStream.WriteAsync(sector, 0, sector.Length);

                    await blobStream.FlushAsync();
                }


                using (Stream blobStream = await blob.OpenReadAsync())
                {
                    byte[] buffer = new byte[2048];

                    const int firstReadCount = 8;
                    int rc =   await blobStream.ReadAsync(buffer, 0, firstReadCount);
                    if(blobStream.CanSeek)
                    {
                        blobStream.Seek(rc, SeekOrigin.Begin);
                    }
                    rc = await blobStream.ReadAsync(buffer, 8, buffer.Length - firstReadCount);

                    string text = Encoding.UTF8.GetString(buffer);
                    Console.WriteLine(text);
                }



            }
            catch (Exception ex)
            {

                Console.WriteLine($"{ex}");
            }
        }

        static async Task TableAsync(string[] args)
        {

            try
            {


                // Azure Storage Account and Table Service Instances
                CloudStorageAccount storageAccount;
                CloudTableClient tableClient;
                // Connnect to Storage Account
                //storageAccount = CloudStorageAccount.Parse("UseDevelopmentStorage=true");

                storageAccount = CloudStorageAccount.Parse(connectionString);
                // Create the Table 'Book', if it not exists
                tableClient = storageAccount.CreateCloudTableClient();
                CloudTable table = tableClient.GetTableReference("Book");
                var rr = await table.CreateIfNotExistsAsync();
                // Create a Book instance
                Book book = new Book()
                {
                    Author = "Rami",
                    BookName = "ASP.NET Core With Azure",
                    Publisher = "APress"
                };
                book.BookId = 1;
                book.RowKey = book.BookId.ToString();
                book.PartitionKey = book.Publisher;
                book.CreatedDate = DateTime.UtcNow;
                book.UpdatedDate = DateTime.UtcNow;
                // Insert and execute operations
                TableOperation insertOperation = TableOperation.Insert(book);
                var rr1 = await table.ExecuteAsync(insertOperation);
                //Console.ReadLine();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"{ex}");
            }
        }


        static void Main(string[] args)
        {
            int testCase = 1;
            switch(testCase)
            {
                case 1:
                    {
                        BlobAsync().Wait();
                    }
                    break;
                case 0:
                    {
                        TableAsync(args).Wait();
                    }
                    break;
            }
          
        }
    }
}
