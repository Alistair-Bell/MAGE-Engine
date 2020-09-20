using System;
using Mage;
using Mage.Maths;

namespace Sandbox
{
    public class Program
    {
        public static void Main(string[] arguments)
        {
            Vector2 vect = new Vector2(1.0f, 2.0f);
            
            Console.Write(string.Format("Main {0}\n", vect, vect));
        }
    }
}

