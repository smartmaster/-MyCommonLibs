namespace GuidAndIntPK
{
    using System;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity;
    using System.Data.SqlClient;
    using System.Linq;
    using System.Threading.Tasks;

    public class MyPKEntity
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        [Column(Order =0)]
        public Guid pk_guid { get; set; }
        [Key]
        //[DatabaseGenerated(DatabaseGeneratedOption.Computed)]
        [Column(Order = 1)]
        public int pk_int { get; set; }
        public string Name { get; set; }

        [Timestamp]
        public byte[] time_stamp { get; set; }
    }

    public class GuidAndIntPKModel : DbContext
    {
        // Your context has been configured to use a 'GuidAndIntPKModel' connection string from your application's 
        // configuration file (App.config or Web.config). By default, this connection string targets the 
        // 'GuidAndIntPK.GuidAndIntPKModel' database on your LocalDb instance. 
        // 
        // If you wish to target a different database and/or database provider, modify the 'GuidAndIntPKModel' 
        // connection string in the application configuration file.
        public GuidAndIntPKModel(string connectionString)
            : base(connectionString/*"name=GuidAndIntPKModel"*/)
        {
        }

        // Add a DbSet for each entity type that you want to include in your model. For more information 
        // on configuring and using a Code First model, see http://go.microsoft.com/fwlink/?LinkId=390109.

        // public virtual DbSet<MyEntity> MyEntities { get; set; }
        public virtual DbSet<MyPKEntity> MyEntities { get; set; }
    }

    //public class MyEntity
    //{
    //    public int Id { get; set; }
    //    public string Name { get; set; }
    //}


    public static class Test
    {
        public static async Task Case1(string[] args)
        {
            try
            {
                //////////////////////////////////////////////////////////////////////////
                SqlConnectionStringBuilder conStrBuilder = new SqlConnectionStringBuilder();
                conStrBuilder["data source"] = "10.57.53.157";
                conStrBuilder["initial catalog"] = $"{nameof(GuidAndIntPK)}.{nameof(GuidAndIntPKModel)}";
                conStrBuilder["persist security info"] = true;
                conStrBuilder["user id"] = "sa";
                conStrBuilder["password"] = "Master16516336";
                conStrBuilder["MultipleActiveResultSets"] = true;
                conStrBuilder["App"] = "EntityFramework";

                //////////////////////////////////////////////////////////////////////////
                using (var dbModel = new GuidAndIntPKModel(conStrBuilder.ConnectionString))
                {
                    //////////////////////////////////////////////////////////////////////////
                    dbModel.Database.Log = (message) => Console.WriteLine("{0}", message);
                    dbModel.Database.CreateIfNotExists();

                    MyPKEntity[] entities = new MyPKEntity[]
                    {
                        new MyPKEntity {Name = "Alfred" },
                        new MyPKEntity {Name = "Bonnie" },
                        new MyPKEntity {Name = "Catherine" },
                        new MyPKEntity {Name = "David" },
                        new MyPKEntity {Name = "Eden" }
                    };


                    dbModel.MyEntities.AddRange(entities);
                    await dbModel.SaveChangesAsync();

                    foreach(var eee in dbModel.MyEntities)
                    {
                        Console.WriteLine($"{eee.pk_guid}\t{eee.pk_int}\t{eee.Name}");
                    }

                    const int MODIFY_INDEX = 2;
                    entities[MODIFY_INDEX].Name = "Cake";
                    //entities[MODIFY_INDEX].pk_int++;
                    await dbModel.SaveChangesAsync();

                    foreach (var eee in dbModel.MyEntities)
                    {
                        Console.WriteLine($"{eee.pk_guid}\t{eee.pk_int}\t{eee.Name}");
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