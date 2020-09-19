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
            float X;
            float Y;
            float Z;

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
            float X;
            float Y;
            float Z;
            float W;

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

        
        
        }




        #endregion
    }
}