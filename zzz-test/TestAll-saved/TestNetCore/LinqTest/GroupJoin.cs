using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestNetCore.LinqTest
{
    class GroupJoin
    {
        public static void Case0()
        {
            var customers = new[]
            {
                new {id = 1, name = "Alice"},
                new {id = 2, name = "Bob"},
                new {id = 3, name = "Cathy"},
            };


            var products = new[]
            {
                new {cid = 1, name = "apple", price = 0.1M},
                new {cid = 1, name = "bee", price = 0.2M},
                new {cid = 2, name = "cat", price = 0.3M},
                new {cid = 2, name = "dog", price = 0.4M},
            };

            var purchas = from c in customers
                          join p in products
                          on c.id equals p.cid into cp
                          
                          select new {c.name, products = cp};


            foreach (var cp in purchas)
            {
                Console.WriteLine($"{cp.name}  bought");
                foreach(var p in cp.products)
                {
                    Console.WriteLine($"\t{p.name} {p.price:C2}");
                }
                if(!cp.products.Any())
                {
                    Console.WriteLine($"\t<nothing>");
                }
                Console.WriteLine();
            }


            var innerjoin = from c in customers
                            join p in products
                            on c.id equals p.cid
                            select new {c.name, product = p.name };
            foreach(var cp in innerjoin)
            {
                Console.WriteLine($"{cp.name} bought {cp.product}");
            }


            var groupby = from pp in products
                          group pp by pp.cid
                          //into g
                          //select g
                          ;
            foreach(var gg in groupby)
            {
                Console.WriteLine($"{gg.Key}");
                foreach(var pp in gg)
                {
                    Console.WriteLine($"\t{pp.name}");
                }
                Console.WriteLine();
            }

        }
    }


}
