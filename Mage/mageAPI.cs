using System;
using System.Runtime.CompilerServices;

namespace Mage
{
    public class Enitity
    {

    }
    
    public class Log 
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Inform(string message);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Warning(string message);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Error(string message);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void FatalError(string message);
    }
    namespace Maths
    {   
        public class Vector2
        {
            public float x, y;
            
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public extern Vector2();
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public extern Vector2(float x, float y);

        }
        public class Vector3
        {

        }
        public class Vector4
        {
            
        }
        public class Matrix4x4
        {
            
        }
    }

    public enum KeyCode
    {
        Space,
        Apostrophe,
        Comma,
        Minus,
        Period,
        Slash, 
        D0, D1, D2, D3, D4, D5, D6, D7, D8, D9,
        Semicolon,
        Equal,
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        LeftBracket,
        Backslash,
        RightBracket,
        GraveAccent,
        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,
        Right,
        Left,
        Down,
        Up,
        Pageup,
        Pagedown,
        Home,
        End,
        Capslock,
        Scrolllock,
        Numlock,
        Printscreen,
        Pause, 
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, 
        Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9, 
        KeypadDecimal, KeypadDivide, KeypadMultiply, KeypadSubtract, KeypadAdd, KeypadEnter, KeypadEqual,
        LeftShift, LeftControl, LeftAlt, LeftSuper, RightShift, RightControl, RightAlt, RightSuper,
    }
    public enum MouseCode
    {
        Left,
        Right,
        Middle,        
    }
    public partial class Event
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsKeyPressed(KeyCode code);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsKeyRepeated(KeyCode code);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsKeyNotPressed(KeyCode code);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsMouseButtonPressed(MouseCode code);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool IsMouseButtonNotPressed(MouseCode code);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern double GetMouseCoordinateX();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern double GetMouseCoordinateY();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool WindowFocused();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool WindowUnFocused();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void WindowSetCursorPosition(double x, double y);
    }
    
    






}