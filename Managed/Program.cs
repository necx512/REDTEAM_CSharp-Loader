using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CLRHello1
{
    class Program
    {
        static void Main(string[] args)
        {
            spotlessMethod("test");
            return;
        }

        // important: methods called by ExecuteInDefaultAppDomain need to stick to this signature
        static int spotlessMethod(String pwzArgument)
        {
            Console.WriteLine("Hi from CLR");
            return 1;
        }
    }
}