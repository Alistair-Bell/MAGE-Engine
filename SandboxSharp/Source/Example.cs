using System;
using Mage;
using Mage.Maths;

namespace Sandbox
{
    public class Program
    {
        public static void Main(string[] arguments)
        {
            Vector2 vect2 = new Vector2(1.0f, 2.0f);
            Matrix3 mat3 = new Matrix3(2.0f);
            Matrix4 mat4 = new Matrix4(2.0f);
            Console.Write(string.Format("vec2 {0}\n", vect2));
            Console.Write(string.Format("mat3\n{0}\n", mat3.ToString()));
            Console.Write(string.Format("mat4\n{0}\n", mat4.ToString()));
        }
    }
}

