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
            Matrix3 matrix = new Matrix3(2.0f);
            Console.Write(string.Format("vec2 {0}\n", vect));
            Console.Write(string.Format("matrix\n{0}\n", matrix.ToString()));
        }
    }
}

