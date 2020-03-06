namespace _000SimpleEF
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity;
    using System.Data.SqlClient;
    using System.Linq;
    using System.Threading.Tasks;

    [Table(nameof(Customer) + "s")]
    public class Customer
    {
        [Key]
        public int cid { get; set; }
        public string Name { get; set; }

        public virtual ICollection<Order> Orders { get; set; } = new HashSet<Order>();
    }

    [Table(nameof(Product) + "s")]
    public class Product
    {
        [Key]
        public int pid { get; set; }
        public string Name { get; set; }

        public virtual ICollection<Order> Orders { get; set; } = new HashSet<Order>();
    }

    [Table(nameof(Order) + "s")]
    public class Order
    {
        [Key]
        public int Oid { get; set; }

        [ForeignKey(nameof(@Customer))]
        public int fkcid { get; set; }

        [ForeignKey(nameof(@Product))]
        public int fkpid { get; set; }

        public virtual Customer @Customer { get; set; }
        public virtual Product @Product { get; set; }
    }

    public class SimpleEFModel : DbContext
    {
        // Your context has been configured to use a 'SimpleEFModel' connection string from your application's 
        // configuration file (App.config or Web.config). By default, this connection string targets the 
        // '_000SimpleEF.SimpleEFModel' database on your LocalDb instance. 
        // 
        // If you wish to target a different database and/or database provider, modify the 'SimpleEFModel' 
        // connection string in the application configuration file.
        public SimpleEFModel(string connStr)
            : base(connStr)
        {
        }

        // Add a DbSet for each entity type that you want to include in your model. For more information 
        // on configuring and using a Code First model, see http://go.microsoft.com/fwlink/?LinkId=390109.

        // public virtual DbSet<MyEntity> MyEntities { get; set; }
        public virtual DbSet<Customer> Customers { get; set;}
        public virtual DbSet<Product> Products { get; set; }
        public virtual DbSet<Order> Orders { get; set; }
    }


    public static class Test
    {
        public static async Task Case1(string[] args)
        {
            try
            {
                SqlConnectionStringBuilder conStrBuilder = new SqlConnectionStringBuilder();
                conStrBuilder["data source"] = "10.57.53.157";
                conStrBuilder["initial catalog"] = $"{nameof(_000SimpleEF)}.{nameof(SimpleEFModel)}";
                conStrBuilder["persist security info"] = true;
                conStrBuilder["user id"] = "sa";
                conStrBuilder["password"] = "Master16516336";
                conStrBuilder["MultipleActiveResultSets"] = true;
                conStrBuilder["App"] = "EntityFramework";


                using (var dbModel = new SimpleEFModel(conStrBuilder.ConnectionString))
                {
                    dbModel.Database.Log = (message) => Console.WriteLine("{0}", message);
                    dbModel.Database.CreateIfNotExists();

                    //////////////////////////////////////////////////////////////////////////
                    Customer[] customers = new Customer[]
                    {
                        new Customer() {Name = "Alice" },
                        new Customer() {Name = "Bob" },
                        new Customer() {Name = "Cathy" },
                        new Customer() {Name = "David" },
                    };

                    Product[] products = new Product[]
                    {
                        new Product() {Name= "Apple" },
                        new Product() {Name= "Bee" },
                        new Product() {Name= "Cake" },
                        new Product() {Name= "Dog" },
                    };


                    Order[] orders = new Order[]
                    {
                        new Order (){@Customer = customers[0], @Product = products[0] },
                        new Order (){@Customer = customers[1], @Product = products[1] },
                        new Order (){@Customer = customers[2], @Product = products[2] },
                        new Order (){@Customer = customers[3], @Product = products[3] },
                    };

                    dbModel.Customers.AddRange(customers);
                    dbModel.Products.AddRange(products);
                    dbModel.Orders.AddRange(orders);

                    await dbModel.SaveChangesAsync();


                }
            }
            catch (System.Exception ex)
            {
                Console.WriteLine($"{ex.Message}");
                throw;
            }
        }
    }

}