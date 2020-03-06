namespace _001SimpleMKEF
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
        [Column(Order = 0)]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int cid { get; set; }

        [Key]
        [Column(Order=1)]
        //[DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int cid1 { get; set; }

        public string Name { get; set; }

        public virtual ICollection<Order> Orders { get; set; } = new HashSet<Order>();
    }

    [Table(nameof(Product) + "s")]
    public class Product
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        [Column(Order = 0)]
        public int pid { get; set; }

        [Key]
        //[DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        [Column(Order = 1)]
        public int pid1 { get; set; }
        public string Name { get; set; }

        public virtual ICollection<Order> Orders { get; set; } = new HashSet<Order>();
    }

    [Table(nameof(Order) + "s")]
    public class Order
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        [Column(Order =0)]
        public int Oid { get; set; }

        [Key]
        //[DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        [Column(Order = 1)]
        public int Oid1 { get; set; }

        [ForeignKey(nameof(@Customer))]
        [Column(Order = 2)]
        public int fkcid { get; set; }

        [ForeignKey(nameof(@Customer))]
        [Column(Order = 3)]
        public int fkcid1 { get; set; }

        [ForeignKey(nameof(@Product))]
        [Column(Order = 4)]
        public int fkpid { get; set; }

        [ForeignKey(nameof(@Product))]
        [Column(Order = 5)]
        public int fkpid1 { get; set; }

        public virtual Customer @Customer { get; set; }
        public virtual Product @Product { get; set; }
    }

    public class SimpleMKEFModel : DbContext
    {

        //private string m_connectionString = $"data source=10.57.53.157;initial catalog={nameof(_001SimpleMKEF)}.{nameof(SimpleEFModel)};persist security info=True;user id=sa;password=Master16516336;MultipleActiveResultSets=True;App=EntityFramework";
        // Your context has been configured to use a 'SimpleEFModel' connection string from your application's 
        // configuration file (App.config or Web.config). By default, this connection string targets the 
        // '_000SimpleEF.SimpleEFModel' database on your LocalDb instance. 
        // 
        // If you wish to target a different database and/or database provider, modify the 'SimpleEFModel' 
        // connection string in the application configuration file.
        public SimpleMKEFModel(string connectionString)
            : base(connectionString/*"name=SimpleMKEFModelSQL"*/)
        {
        }

        // Add a DbSet for each entity type that you want to include in your model. For more information 
        // on configuring and using a Code First model, see http://go.microsoft.com/fwlink/?LinkId=390109.

        // public virtual DbSet<MyEntity> MyEntities { get; set; }
        public virtual DbSet<Customer> Customers { get; set; }
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
                conStrBuilder["initial catalog"] = $"{nameof(_001SimpleMKEF)}.{nameof(SimpleMKEFModel)}";
                conStrBuilder["persist security info"] = true;
                conStrBuilder["user id"] = "sa";
                conStrBuilder["password"] = "Master16516336";
                conStrBuilder["MultipleActiveResultSets"] = true;
                conStrBuilder["App"] = "EntityFramework";
                
                using (var dbModel = new SimpleMKEFModel(conStrBuilder.ConnectionString))
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


                    var ooocpLazy = from ooo in dbModel.Orders //.Include(ooo => ooo.Customer).Include(ooo => ooo.Product)
                                    select new { ooo.Customer, ooo.Product };
                    foreach (var ocp in ooocpLazy)
                    {
                        var ccc = ocp.Customer;
                        var ppp = ocp.Product;
                        Console.WriteLine($"{ccc.Name}, \t{ccc.cid}, \t{ccc.cid1}, \t{ppp.Name}, \t{ppp.pid}, \t{ppp.pid1}");
                    }


                    var CustomerNameProductName = from ccc in dbModel.Customers.Include(ccc => ccc.Orders)
                              from ooo in ccc.Orders
                              select new {ccc, ooo.Product};
                    foreach (var cp in CustomerNameProductName)
                    {
                        var ccc = cp.ccc;
                        var ppp = cp.Product;
                        Console.WriteLine($"{ccc.Name}, \t{ccc.cid}, \t{ccc.cid1}, \t{ppp.Name}, \t{ppp.pid}, \t{ppp.pid1}");
                    }

                    var ooocp = from ooo in dbModel.Orders.Include(ooo => ooo.Customer).Include(ooo => ooo.Product)
                             select new {ooo.Customer, ooo.Product };
                    foreach (var ocp in ooocp)
                    {
                        var ccc = ocp.Customer;
                        var ppp = ocp.Product;
                        Console.WriteLine($"{ccc.Name}, \t{ccc.cid}, \t{ccc.cid1}, \t{ppp.Name}, \t{ppp.pid}, \t{ppp.pid1}");
                    }


                   


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