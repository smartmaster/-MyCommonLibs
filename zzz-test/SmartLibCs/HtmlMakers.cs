using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace SmartLib
{


    class HtmlFormMaker
    {
        private readonly StringBuilder _sb = new StringBuilder();
        public void MakePropDiv(string propName)
        {
            _sb.AppendFormat("<div class=\"form-group\">"); _sb.AppendLine();
            _sb.AppendFormat("\t<label asp-for=\"{0}\"></label>", propName); _sb.AppendLine();
            _sb.AppendFormat("\t<div><span asp-validation-for=\"{0}\" class=\"text-danger\"></span></div>", propName); _sb.AppendLine();
            _sb.AppendFormat("\t<input asp-for=\"{0}\" class=\"form-control\"/>", propName); _sb.AppendLine();
            _sb.Append("</div>"); _sb.AppendLine();
        }


        public void MakeHead(string modelType, string action = "AAAAA", string controller = "AAAAA", string httpMethod = "AAAAA")
        {
            _sb.AppendFormat("@model {0}", modelType); _sb.AppendLine(); _sb.AppendLine();
            _sb.Append("<div asp-validation-summary=\"ModelOnly\" class=\"text-danger\"></div>"); _sb.AppendLine(); _sb.AppendLine();
            _sb.AppendFormat("<form asp-action=\"{0}\" asp-controller=\"{1}\" method=\"{2}\">", action, controller, httpMethod); _sb.AppendLine();
        }

        public void MakeFoot(string submitButton = "AAAAA", string action = "AAAAA", string controler = "AAAAA", string cancelButton = "AAAAA")
        {
            _sb.AppendFormat("<button type=\"submit\" class=\"btn btn-primary\" >{0}</button>", submitButton); _sb.AppendLine();
            _sb.AppendFormat("<a class=\"btn btn-secondary\" asp-action=\"{0}\" asp-controller=\"{1}\">{2}</a>", action, controler, cancelButton); _sb.AppendLine();
            _sb.Append("</form>"); _sb.AppendLine();
        }

        public void Clear()
        {
            _sb.Clear();
        }

        public string Make<T>()
        {
            Type type = typeof(T);

            MakeHead(type.FullName);

            PropertyInfo[] pis = type.GetProperties();
            foreach(var pi in pis)
            {
                MakePropDiv(pi.Name);
            }

            MakeFoot();


            return _sb.ToString();
        }
    }


    enum HtmlElementType
    {
        Tag,
        Text
    };

    class HtmlAttribute
    {
        public string Name { get; set; }

        public string Value { get; set; }
    }

    class HtmlElement
    {
        public HtmlElementType @Type { get; set; } = HtmlElementType.Tag;
        public string Name { get; set; }

        //private int tabs = 0;

        public IList<HtmlAttribute> Attributes { get; set; } = new List<HtmlAttribute>();
        public IList<HtmlElement> Children { get; set; } = new List<HtmlElement>();

       public  void AddAttribute(string name, string value)
        {
            Attributes.Add(new HtmlAttribute
            {
                Name = name,
                Value = value
            });
        }

#if false
        public void AddTag(string tagName)
        {
            Children.Add(new HtmlElement
            {
                Type = HtmlElementType.Tag,
                Name = tagName
            });
        }
#endif
        public void AddText(string text)
        {
            Children.Add(new HtmlElement
            {
                Type = HtmlElementType.Text,
                Name = text
            });
        }


        public void AddChild(HtmlElement child)
        {
            Children.Add(child);
        }
        public void ToHtmlString(StringBuilder sb, int tabs)
        {
            if (Type == HtmlElementType.Text)
            {
                sb.Append(Name);
            }
            else
            {
                ++tabs;
                sb.Append('\t', tabs);
                sb.AppendFormat("<{0}", Name);
                foreach(var aa in Attributes)
                {
                    sb.AppendFormat(" {0}=\"{1}\"", aa.Name, aa.Value);
                }
                sb.Append(">");

                if (Children.Any(iii => iii.Type == HtmlElementType.Tag))
                {
                    sb.AppendLine();
                }
                

                foreach (var cc in Children)
                {
                    cc.ToHtmlString(sb, tabs);
                }

                if (Children.Any(iii => iii.Type == HtmlElementType.Tag))
                {
                    sb.Append('\t', tabs);
                }
                sb.AppendFormat("</{0}>", Name);
                sb.AppendLine();
                --tabs;
            }
        }
    }

   

    class HtmlTableMaker
    {
        private readonly StringBuilder _sb = new StringBuilder();

        HtmlElement MakeTh(string propName)
        {
            HtmlElement th = new HtmlElement { Name = nameof(th) };
            th.AddText(propName);
            return th;
        }

        HtmlElement MakeTr(Type type)
        {
            HtmlElement tr = new HtmlElement { Name = nameof(tr)};
            var pis = type.GetProperties();
            foreach(var pi in pis)
            {
                tr.AddChild(MakeTh(pi.Name));
            }

            return tr;
        }

        HtmlElement MakeThead(Type type)
        {
            HtmlElement thead = new HtmlElement { Name = nameof(thead)};
            thead.AddChild(MakeTr(type));
            return thead;
        }


        HtmlElement MakeTd(string propValue)
        {
            HtmlElement td = new HtmlElement { Name = nameof(td) };
            td.AddText(propValue);
            return td;
        }

        HtmlElement MakeTr(object obj, Type type)
        {
            HtmlElement tr = new HtmlElement { Name = nameof(tr)};

            var pis = type.GetProperties();
            foreach (var pi in pis)
            {
                //string propValue = pi.GetValue(obj)?.ToString();
                tr.AddChild(MakeTd($"@item.{pi.Name}"));
            }

            return tr;
        }

        HtmlElement MakeTbody<T>()
        {
            StringBuilder sb = new StringBuilder();
            HtmlElement tbody = new HtmlElement { Name = nameof(tbody)};

            sb.AppendLine("@foreach(var item in Model)");
            sb.AppendLine("{");
            tbody.AddText(sb.ToString());
            
            tbody.AddChild(MakeTr(null, typeof(T)));

            sb.Clear();
            sb.AppendLine("}");
            tbody.AddText(sb.ToString());

            return tbody;
        }

        public string Make<T>()
        {
            _sb.AppendLine($"@model IEnumerable<{typeof(T).FullName}>");
            _sb.AppendLine();

            //////////////////////////////////////////////////////////////////////////
            HtmlElement table = new HtmlElement{Name = nameof(table)};
            table.AddAttribute("class", "table table-sm table-bordered table-hover");


            table.AddChild(MakeThead(typeof(T)));
            table.AddChild(MakeTbody<T>());

            table.ToHtmlString(_sb, -1);

            return _sb.ToString();
            
        }

        public void Clear()
        {
            _sb.Clear();
        }
    }


    class HtmlSingleObjectTableMaker
    {
        private readonly StringBuilder _sb = new StringBuilder();

        HtmlElement MakeTh(string propName)
        {
            HtmlElement th = new HtmlElement { Name = nameof(th)};
            th.AddText($"@nameof(Model.{propName})");
            return th;
        }

        HtmlElement MakeTd(string propName)
        {
            HtmlElement td = new HtmlElement { Name = nameof(td) };
            td.AddText($"@Model.{propName}");
            return td;
        }

        HtmlElement MakeTr(string propName)
        {
            HtmlElement tr = new HtmlElement { Name = nameof(tr) };
            tr.AddChild(MakeTh(propName));
            tr.AddChild(MakeTd(propName));
            return tr;
        }

        HtmlElement MakeTable<T>()
        {
            HtmlElement table = new HtmlElement { Name = nameof(table)};
            table.AddAttribute("class", "table table-sm table-bordered table-hover");

            var pis = typeof(T).GetProperties();
            foreach(var pi in pis)
            {
                table.AddChild(MakeTr(pi.Name));
            }
            
            return table;
        }


        public string Make<T>()
        {
            HtmlElement table = MakeTable<T>();
            _sb.AppendLine($"@model {typeof(T).FullName}");
            table.ToHtmlString(_sb, -1);
            return _sb.ToString();
        }

        public void Clear()
        {
            _sb.Clear();
        }

        

    }
    public static class HtmlMakerAPI
    {
        public static string MakeForm<T>()
        {
            HtmlFormMaker maker = new HtmlFormMaker();
            return maker.Make<T>();
        }


        public static string MakeTable<T>()
        {
            HtmlTableMaker maker = new HtmlTableMaker();
            return maker.Make<T>();
        }

        public static string MakeSingleObjectTable<T>()
        {
            HtmlSingleObjectTableMaker maker = new HtmlSingleObjectTableMaker();
            return maker.Make<T>();
        }


        public static string[] MakeAll<T>()
        {
            string[] htmls = new string[3];

            htmls[0] = MakeForm<T>();
            htmls[1] = MakeTable<T>();
            htmls[2] = MakeSingleObjectTable<T>();

            return htmls;
        }
    }


   

    public static class HtmlFormMakerTest
    {
        public class TestEmployee
        {
            public int Age { get; set; }
            public string Name { get; set; }

            public bool CanWorkNightly { get; set; }
        }

        public static  void Case2()
        {
            string form = HtmlMakerAPI.MakeForm<TestEmployee>();
            Console.WriteLine($"{form}");


            string table = HtmlMakerAPI.MakeTable<TestEmployee>();
            Console.WriteLine($"{table}");

            string soTable = HtmlMakerAPI.MakeSingleObjectTable<TestEmployee>();
            Console.WriteLine($"{soTable}");
        }

        public static void Case1()
        {
            TestEmployee ee = new TestEmployee
            {
                Age = 999999,
                Name = "ZZZZZZZZZZZZZZZZZ",
                CanWorkNightly = true
            };

            IList<TestEmployee> items = new List<TestEmployee>();
            for (int ii = 0; ii < 6; ++ ii)
            {
                items.Add(ee);
            }

            HtmlTableMaker maker = new HtmlTableMaker();

            string html = maker.Make<TestEmployee>();

            Console.WriteLine($"{html}");
        }
        
        public static void Case0()
        {

            HtmlFormMaker maker = new HtmlFormMaker();
            string html = maker.Make<TestEmployee>();
            Console.WriteLine($"{html}");
        }
    }
}
