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
        public partial class Generic
        {
            public static float PiSquared   = 9.86960440109f;
            public static float Pi          = 3.14159265358f;
            public static float HalfPi      = 1.57079632679f;
            public static float Epsilon     = 1e-06f;

            public static bool IsEqualApproximate(float a, float b)
            {
                /* Taken from https://github.com/godotengine/godot/blob/45766f1adc83710a0484dfa15733b3d4fdbea2f1/modules/mono/glue/GodotSharp/GodotSharp/Core/Maths.cs */
                if (a == b)
                {
                    return true;
                }
                
                float tolerance = Epsilon * Math.Abs(a);
                if (tolerance < Epsilon)
                {
                    tolerance = Epsilon;
                }
                return Math.Abs(a - b) < tolerance;
            }
        }
        #endregion

        #region Vectors
        
        /*
            C# implimentation of the mageVector2 structure (see mageAPI.h)
        */
        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        public struct Vector2
        {
            public float X;
            public float Y;
            
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
                    }
                }
                set
                {
                    switch (vectorIndex)
                    {
                        case 0:
                            this.X = value;
                            return;
                        case 1:
                            this.Y = value;
                            return;
                        default:
                            throw new IndexOutOfRangeException();
                    }
                }
            }

            public Vector2(float x = 0.0f, float y = 0.0f)
            {
                this.X = x;
                this.Y = x; 
            }
            
            /* Operator overloads */
            public static Vector2 operator+(Vector2 left, Vector2 right) => new Vector2(left.X + right.X, left.Y + right.Y);
            public static Vector2 operator-(Vector2 left, Vector2 right) => new Vector2(left.X - right.X, left.Y - right.Y);
            public static Vector2 operator*(Vector2 left, Vector2 right) => new Vector2(left.X * right.X, left.Y * right.Y);
            public static Vector2 operator/(Vector2 left, Vector2 right) => new Vector2(left.X / right.X, left.Y / right.Y);
            public static bool operator==(Vector2 left, Vector2 right) => (left.X == right.X && left.Y == right.Y);
            public static bool operator!=(Vector2 left, Vector2 right) => !(left == right);

            public static bool operator<(Vector2 left, Vector2 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    return left.X < right.X;
                }
                return left.Y < right.Y;
            }
            public static bool operator>(Vector2 left, Vector2 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    return left.X > right.X;
                }
                return left.Y > right.Y;
            }
            public static bool operator<=(Vector2 left, Vector2 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    return left.X <= right.X;
                }
                return left.Y <= right.Y;
            }
            public static bool operator>=(Vector2 left, Vector2 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    return left.X >= right.X;
                }
                return left.Y >= right.Y;
            }
            public override bool Equals(Object obj)
            {
                if ((obj == null) || this.GetType() != obj.GetType())
                {
                    return false;
                }
                Vector2 vector = (Vector2) obj;
                return this == vector;
            }
            public override int GetHashCode()
            {
                return this.X.GetHashCode() ^ this.Y.GetHashCode();
            }

            public override string ToString()
            {
                return string.Format("Vector2 [{0} {1}]", new object[] { this.Y.ToString(), this.Y.ToString() });
            }
            public static void Log(Vector2 logging)
            {
                Debug.Log.Inform(logging.ToString());
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

        /*
            C# implimentation of the mageVector3 structure (see mageAPI.h)
        */
        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        public struct Vector3
        {
            public float X;
            public float Y;
            public float Z;

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
                        case 2:
                            return this.Z;
                        default:
                            throw new IndexOutOfRangeException();
                    }
                }
                set
                {
                    switch (vectorIndex)
                    {
                        case 0:
                            this.X = value;
                            return;
                        case 1:
                            this.Y = value;
                            return;
                        case 2:
                            this.Z = value;
                            return;
                        default:
                            throw new IndexOutOfRangeException();
                    }
                }
            }
            
            public Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
            {
                this.X = x;
                this.Y = y;
                this.Z = z;
            }

            /* Operator overloads */
            public static Vector3 operator+(Vector3 left, Vector3 right) => new Vector3(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
            public static Vector3 operator-(Vector3 left, Vector3 right) => new Vector3(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
            public static Vector3 operator*(Vector3 left, Vector3 right) => new Vector3(left.X * right.X, left.Y * right.Y, left.Z * right.Z);
            public static Vector3 operator/(Vector3 left, Vector3 right) => new Vector3(left.X / right.X, left.Y / right.Y, left.Z / right.Z);
            public static bool operator==(Vector3 left, Vector3 right) => (left.X == right.X && left.Y == right.Y && left.Z == right.Z);
            public static bool operator!=(Vector3 left, Vector3 right) => !(left == right);

            public static bool operator<(Vector3 left, Vector3 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    if (Generic.IsEqualApproximate(left.Y, right.Y))
                    {
                        return left.Z < right.Z;
                    }
                    return left.Y < right.Y;    
                }
                return left.X < right.X;
            }
            public static bool operator>(Vector3 left, Vector3 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    if (Generic.IsEqualApproximate(left.Y, right.Y))
                    {
                        return left.Z > right.Z;
                    }
                    return left.Y > right.Y;    
                }
                return left.X > right.X;
            }
            public static bool operator<=(Vector3 left, Vector3 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    if (Generic.IsEqualApproximate(left.Y, right.Y))
                    {
                        return left.Z <= right.Z;
                    }
                    return left.Y <= right.Y;    
                }
                return left.X <= right.X;
            }
            public static bool operator>=(Vector3 left, Vector3 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    if (Generic.IsEqualApproximate(left.Y, right.Y))
                    {
                        return left.Z >= right.Z;
                    }
                    return left.Y >= right.Y;    
                }
                return left.X >= right.X;
            }
            public override bool Equals(Object obj)
            {
                if ((obj == null) || this.GetType() != obj.GetType())
                {
                    return false;
                }
                Vector3 vector = (Vector3) obj;
                return this == vector;
            }
            public override int GetHashCode()
            {
                return this.X.GetHashCode() ^ this.Y.GetHashCode() ^ this.Z.GetHashCode();
            }

            public override string ToString()
            {
                return string.Format("Vector3 [{0} {1} {2}]", new object[] { this.Y.ToString(), this.Y.ToString(), this.Z.ToString() });
            }
            
            public static void Log(Vector3 logging)
            {
                Debug.Log.Inform(logging.ToString());
            }

            public void Add(Vector3 other)
            {
                this.X += other.X;
                this.Y += other.Y;
                this.Z += other.Z;
            }
            public void Subtract(Vector3 other)
            {
                this.X -= other.X;
                this.Y -= other.Y;
                this.Z -= other.Z;
            }
            public void Multiply(Vector3 other)
            {
                this.X *= other.X;
                this.Y *= other.Y;
                this.Z *= other.Z;
            }
            public void Divide(Vector3 other)
            {
                this.X /= other.X;
                this.Y /= other.Y;
                this.Z /= other.Z;
            }
        }

        /*
            C# implimentation of the mageVector4 structure (see mageAPI.h)
        */
        [Serializable]
        [StructLayout(LayoutKind.Sequential)]
        public struct Vector4
        {
            public float X;
            public float Y;
            public float Z;
            public float W;

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
                        case 2:
                            return this.Z;
                        case 3:
                            return this.W;
                        default:
                            throw new IndexOutOfRangeException();
                    }
                }
                set
                {
                    switch (vectorIndex)
                    {
                        case 0:
                            this.X = value;
                            return;
                        case 1:
                            this.Y = value;
                            return;
                        case 2:
                            this.Z = value;
                            return;
                        case 3:
                            this.W = value;
                            return;
                        default:
                            throw new IndexOutOfRangeException();
                    }
                }
            }

            public Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
            {
                this.X = x;
                this.Y = y;
                this.Z = z;
                this.W = w;
            }
            
            /* Operator overloads */
            public static Vector4 operator+(Vector4 left, Vector4 right) => new Vector4(left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W);
            public static Vector4 operator-(Vector4 left, Vector4 right) => new Vector4(left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W);
            public static Vector4 operator*(Vector4 left, Vector4 right) => new Vector4(left.X * right.X, left.Y * right.Y, left.Z * right.Z, left.W * right.W);
            public static Vector4 operator/(Vector4 left, Vector4 right) => new Vector4(left.X / right.X, left.Y / right.Y, left.Z / right.Z, left.W / right.W);
            public static bool operator==(Vector4 left, Vector4 right) => (left.X == right.X && left.Y == right.Y && left.Z == right.Z && left.W == right.W);
            public static bool operator!=(Vector4 left, Vector4 right) => !(left == right);

            public static bool operator<(Vector4 left, Vector4 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    if (Generic.IsEqualApproximate(left.Y, right.Y))
                    {
                        if (Generic.IsEqualApproximate(left.Z, right.Z))
                        {
                            return left.W < right.W;
                        }
                        return left.Z < right.Z;
                    }
                    return left.Y < right.Y;    
                }
                return left.X < right.X;
            }
            public static bool operator>(Vector4 left, Vector4 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    if (Generic.IsEqualApproximate(left.Y, right.Y))
                    {
                        if (Generic.IsEqualApproximate(left.Z, right.Z))
                        {
                            return left.W > right.W;
                        }
                        return left.Z > right.Z;
                    }
                    return left.Y > right.Y;    
                }
                return left.X > right.X;
            }
            public static bool operator<=(Vector4 left, Vector4 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    if (Generic.IsEqualApproximate(left.Y, right.Y))
                    {
                        if (Generic.IsEqualApproximate(left.Z, right.Z))
                        {
                            return left.W <= right.W;
                        }
                        return left.Z <= right.Z;
                    }
                    return left.Y <= right.Y;    
                }
                return left.X <= right.X;
            }
            public static bool operator>=(Vector4 left, Vector4 right)
            {
                if (Generic.IsEqualApproximate(left.X, right.X))
                {
                    if (Generic.IsEqualApproximate(left.Y, right.Y))
                    {
                        if (Generic.IsEqualApproximate(left.Z, right.Z))
                        {
                            return left.W >= right.W;
                        }
                        return left.Z >= right.Z;
                    }
                    return left.Y >= right.Y;    
                }
                return left.X >= right.X;
            }
            public override bool Equals(Object obj)
            {
                if ((obj == null) || this.GetType() != obj.GetType())
                {
                    return false;
                }
                Vector4 vector = (Vector4) obj;
                return this == vector;
            }
            public override int GetHashCode()
            {
                return this.X.GetHashCode() ^ this.Y.GetHashCode() ^ this.Z.GetHashCode();
            }

            public override string ToString()
            {
                return string.Format("Vector4 [{0} {1} {2} {3}]", new object[] { this.Y.ToString(), this.Y.ToString(), this.Z.ToString(), this.W.ToString() });
            }
        
            public static void Log(Vector4 logging)
            {
                Debug.Log.Inform(logging.ToString());
            }

            public void Add(Vector4 other)
            {
                this.X += other.X;
                this.Y += other.Y;
                this.Z += other.Z;
                this.W += other.W;
            }
            public void Subtract(Vector4 other)
            {
                this.X -= other.X;
                this.Y -= other.Y;
                this.Z -= other.Z;
                this.W -= other.W;
            }
            public void Multiply(Vector4 other)
            {
                this.X *= other.X;
                this.Y *= other.Y;
                this.Z *= other.Z;
                this.W *= other.W;
            }
            public void Divide(Vector4 other)
            {
                this.X /= other.X;
                this.Y /= other.Y;
                this.Z /= other.Z;
                this.W /= other.W;
            }
            
            /*
                C# implimentation of the mageMatrix3 structure (see mageAPI.h)
            */
            [Serializable]
            [StructLayout(LayoutKind.Sequential)]
            public class Matrix3
            {
                public float[] Elements;

                public Matrix3(float[] values)
                {
                    this.Elements = new float[9];
                    this.Elements = values;  
                }
                public Matrix3(Vector3 column0, Vector3 column1, Vector3 column2)
                {
                    /* Column 0 */
                    this.Elements[0]    = column0.X;
                    this.Elements[3]    = column0.Y;
                    this.Elements[6]    = column0.Z;

                    /* Column 1 */
                    this.Elements[1]    = column1.X;
                    this.Elements[4]    = column1.Y;
                    this.Elements[7]    = column1.Z;

                    /* Column 2 */
                    this.Elements[2]    = column2.X;
                    this.Elements[5]    = column2.Y;
                    this.Elements[8]    = column2.Z;
                }
                public Matrix3(float diagonal)
                {
                    this.Elements[0] = diagonal;
                    this.Elements[4] = diagonal;
                    this.Elements[8] = diagonal;
                }
                public Matrix3()
                {
                    this.Elements = new Matrix3(1.0f).Elements;
                }
                
            }
            /*
                C# implimentation of the mageMatrix3 structure (see mageAPI.h)
            */
            [Serializable]
            [StructLayout(LayoutKind.Sequential)]
            public class Matrix4
            {
                public float[] Elements;

                public Matrix4(float[] values)
                {
                    this.Elements = new float[16];
                    this.Elements = values;  
                }
                public Matrix4(Vector4 column0, Vector4 column1, Vector4 column2, Vector4 column3)
                {
                    /* Column 0 */
                    this.Elements[0]    = column0.X;
                    this.Elements[4]    = column0.Y;
                    this.Elements[8]    = column0.Z;
                    this.Elements[12]   = column0.W;

                    /* Column 1 */
                    this.Elements[1]    = column1.X;
                    this.Elements[5]    = column1.Y;
                    this.Elements[9]    = column1.Z;
                    this.Elements[13]   = column1.W;

                    /* Column 2 */
                    this.Elements[2]    = column2.X;
                    this.Elements[6]    = column2.Y;
                    this.Elements[10]   = column2.Z;
                    this.Elements[14]   = column2.W;

                    /* Column 3 */
                    this.Elements[3]    = column3.X;
                    this.Elements[7]    = column3.Y;
                    this.Elements[11]   = column3.Z;
                    this.Elements[15]   = column3.W;
                }
                public Matrix4(float diagonal)
                {
                    this.Elements[0]    = diagonal;
                    this.Elements[5]    = diagonal;
                    this.Elements[10]   = diagonal;
                    this.Elements[15]   = diagonal;
                }
                public Matrix4()
                {
                    this.Elements = new Matrix4(1.0f).Elements;
                }

            }
        }




        #endregion
    }
}