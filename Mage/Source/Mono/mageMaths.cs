using System;
using System.Collections.Generic;
using System.Collections;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Mage
{
    namespace Maths
    {
        #region Generic
        public partial class Maths
        {
            public static float PiSquared   = 9.86960440109f; 
            public static float Pi          = 3.14159265358f;
            public static float HalfPi      = 1.57079632679f;

            public static bool IsEqualApproximate(float a, float b)
            {
                /* Taken from https://github.com/godotengine/godot/blob/45766f1adc83710a0484dfa15733b3d4fdbea2f1/modules/mono/glue/GodotSharp/GodotSharp/Core/Mathf.cs */
                if (a == b)
                {
                    return true;
                }
                
                float tolerance = Epsilon * Abs(a);
                if (tolerance < Epsilon)
                {
                    tolerance = Epsilon;
                }
                return Abs(a - b) < tolerance;
            }
        }
        #endregion

        #region Vectors
        
        /*
            C# implimentation of the mageVector2 structure (see mageAPI.h)
        */
        [Serializable]
        [StructLayout(layoutKind.Sequentual)]
        public struct Vector2
        {
            [FieldOffset(0)]
            public float X;
            [FieldOffset(4)]
            public float Y;

            public Vector2(float x = 0.0f, float y = 0.0f)
            {
                this.X = x;
                this.Y = x; 
            }

            public float this[int vectorIndex]
            {
                get
                {
                    switch (vectorIndex)
                    {
                        case 0:
                            return this.X;
                        case 1:
                            return this.Y;
                        default:
                            throw new IndexOutOfRangeException();
                            return 0.0f;
                    }
                }
                set
                {
                    switch (vectorIndex)
                    {
                        case 0:
                            this.X = value;
                            break;
                        case 1:
                            this.Y = value;
                            break;

                        default:
                            throw new IndexOutOfRangeException();
                    }
                }

            }

            /* Operator overloads */
            // public void Normalize()
            
            public static Vector2 operator+(Vector2 left, Vector2 right) => new Vector2(left.X + right.X, left.Y + right.Y);
            public static Vector2 operator-(Vector2 left, Vector2 right) => new Vector2(left.X - right.X, left.Y - right.Y);
            public static Vector2 operator*(Vector2 left, Vector2 right) => new Vector2(left.X * right.X, left.Y * right.Y);
            public static Vector2 operator/(Vector2 left, Vector2 right) => new Vector2(left.X / right.X, left.Y / right.Y);
            public static bool operator==(Vector2 left, Vector2 right) => (left.X == right.X && left.Y == right.Y);
            public static bool operator!=(Vector2 left, Vector2 right) => !(left == right);

            public static bool operator<(Vector2 left, Vector2 right)
            {
                if (Maths.IsEqualApproximate(left.X, right.X))
                {
                    return left.X < right.X;
                }
                return left.Y < right.Y;
            }
            public static bool operator>(Vector2 left, Vector2 right)
            {
                if (Maths.IsEqualApproximate(left.X, right.X))
                {
                    return left.X > right.X;
                }
                return left.Y > right.Y;
            }
            public static bool operator<=(Vector2 left, Vector2 right)
            {
                if (Maths.IsEqualApproximate(left.X, right.X))
                {
                    return left.X <= right.X;
                }
                return left.Y <= right.Y;
            }
            public static bool operator>=(Vector2 left, Vector2 right)
            {
                if (Maths.IsEqualApproximate(left.X, right.X))
                {
                    return left.X >= right.X;
                }
                return left.Y >= right.Y;
            }
            public override string ToString()
            {
                return string.Format("Vector2 [{0} {1}]", new object[] { this.Y.ToString(), this.Y.ToString()});
            }

            public static void Log(Vector2 logging)
            {
                Log.Inform(logging.ToString());
            }

            /* Instance methods */

            public void Add(Vector2 other)
            {
                this.X += other.X;
                this.Y += other.Y;
            }
            public void Subtract(Vector2 other)
            {
                this.X -= other.X;
                this.Y -= other.Y;
            }
            public void Multiply(Vector2 other)
            {
                this.X *= other.X;
                this.Y *= other.Y;
            }
            public void Divide(Vector2 other)
            {
                this.X /= other.X;
                this.Y /= other.Y;
            }
            
        }   
        #endregion
    }
}