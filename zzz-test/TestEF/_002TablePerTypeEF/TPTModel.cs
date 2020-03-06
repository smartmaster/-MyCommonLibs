using System.Data.Entity.Infrastructure;
namespace _002TablePerTypeEF
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity;
    using System.Data.SqlClient;
    using System.Linq;
    using System.Threading.Tasks;

    [Table(nameof(Employer) + "s")]
    public class Employer
    {
        [Key]
        [Column(Order =0)]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int eid { get;set;}
        [Key]
        [Column(Order =1)]
        public int eid1 { get; set; }
        public string Name { get; set; }

        public virtual ICollection<Person> Employees { get; set; } = new HashSet<Person>();
    }

    [Table(nameof(Person) + "s")]
    public class Person
    {
        [Key]
        [Column(Order =0)]
        public int pid { get; set; }
        public string Name { get; set; }

        [ForeignKey(nameof(@Employer))]
        [Column(Order =1)]
        public int fkeid { get; set; }
        [ForeignKey(nameof(@Employer))]
        [Column(Order =2)]
        public int fkeid1 { get; set; }
        public virtual Employer @Employer { get; set; }
    }

    [Table(nameof(Employee) + "s")]
    public class Employee : Person
    {
        public decimal Salary { get; set; }
    }

    [Table(nameof(Teacher) + "s")]
    public class Teacher : Employee
    {
        public string Cource { get; set; }
    }

    [Table(nameof(Fireman) + "s")]
    public class Fireman : Employee
    {
        public int LivesSaved { get; set; }
    }

    public class TPTModel : DbContext
    {
        // Your context has been configured to use a 'TPTModel' connection string from your application's 
        // configuration file (App.config or Web.config). By default, this connection string targets the 
        // '_002TablePerTypeEF.TPTModel' database on your LocalDb instance. 
        // 
        // If you wish to target a different database and/or database provider, modify the 'TPTModel' 
        // connection string in the application configuration file.
        public TPTModel(string connStr)
            : base(connStr/*"name=TPTModel"*/)
        {
        }

        // Add a DbSet for each entity type that you want to include in your model. For more information 
        // on configuring and using a Code First model, see http://go.microsoft.com/fwlink/?LinkId=390109.

        // public virtual DbSet<MyEntity> MyEntities { get; set; }

        public virtual DbSet<Person> People { get; set; }
        public virtual DbSet<Employer> Employers { get; set; }
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
                SqlConnectionStringBuilder conStrBuilder = new SqlConnectionStringBuilder();
                conStrBuilder["data source"] = "10.57.53.157";
                conStrBuilder["initial catalog"] = $"{nameof(_002TablePerTypeEF)}.{nameof(TPTModel)}";
                conStrBuilder["persist security info"] = true;
                conStrBuilder["user id"] = "sa";
                conStrBuilder["password"] = "Master16516336";
                conStrBuilder["MultipleActiveResultSets"] = true;
                conStrBuilder["App"] = "EntityFramework";

                using (var dbModel = new TPTModel(conStrBuilder.ConnectionString))
                {
                    dbModel.Database.Log = (message) => Console.WriteLine("{0}", message);
                    dbModel.Database.CreateIfNotExists();

                    Person[] peopel = new Person[]
                    {
                        new Teacher() { Name = "Andy", Salary = 1000m, Cource = "Astronaut"},
                        new Teacher() { Name = "Bob", Salary = 1001m, Cource = "Bird"},
                        new Teacher() { Name = "Cathy", Salary = 1002m, Cource = "Cat"},
                        new Fireman() { Name = "Frank", Salary = 2000m, LivesSaved = 100 },
                        new Fireman() { Name = "Goodman", Salary = 2001m, LivesSaved = 200 },
                        new Fireman() { Name = "Henry", Salary = 2002m, LivesSaved = 300 },
                    };


                    Employer employer = new Employer()
                    {
                        Name = "Microosoft"
                    };
                    foreach (var ppp in peopel)
                    {
                        employer.Employees.Add(ppp);
                    }
                    

                    dbModel.People.AddRange(peopel);
                    dbModel.Employers.Add(employer);
                    await dbModel.SaveChangesAsync();


                    var queryeeww = from eee in dbModel.Employers.Include(eee => eee.Employees)
                                  from www in eee.Employees
                                  select new {eee, www };
                    foreach(var eeww in queryeeww)
                    {
                        var eee = eeww.eee;
                        var www = eeww.www;
                        var job = www is Teacher ? nameof(Teacher) : www is Fireman ? nameof(Fireman) : "unknown";
                        Console.WriteLine($"{eee.Name}\t{eee.eid}\t{eee.eid1}\t{www.Name}\t{job}");
                    }
                                 


                    var query = from ppp in dbModel.People
                                select ppp;
                    string sqlStr = query.ToString();
                    DbQuery<Person> dq = (DbQuery<Person>)(query);
                    string dqSqlStr = dq.ToString();
                    int compare = string.Compare(sqlStr, dqSqlStr);
                    Console.WriteLine($"the 2 SQL are equal? {compare == 0}");
                   
                    foreach(Person ppp in query)
                    {
                        if (ppp is Teacher)
                        {
                            Teacher ttt = (Teacher)(ppp);
                            Console.WriteLine($"{ttt.pid}\t{ttt.Name}\t{ttt.Salary}\t{ttt.Cource} \t\tTeacher");
                        }
                        else if(ppp is Fireman)
                        {
                            Fireman fff = (Fireman)ppp;
                            Console.WriteLine($"{fff.pid}\t{fff.Name}\t{fff.Salary}\t{fff.LivesSaved} \t\tFireman");
                        }
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