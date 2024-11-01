using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CLRHello1
{
    public class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hi");
            return;
        }

        // important: methods called by ExecuteInDefaultAppDomain need to stick to this signature
        public static int spotlessMethod(String pwzArgument)
        {
            Console.WriteLine("Hi from CLR");
            return 1;
        }
    }
}