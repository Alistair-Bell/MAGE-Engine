using System;
using System.Collections.Generic;
using System.Collections;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Mage
{
    namespace Debug
    {
        /* Interact with the logging system */
        public partial class Log
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public static extern void Inform(string message);
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public static extern void Warning(string message);
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public static extern void Error(string message);
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public static extern void Fatal(string message);
        }
    }
    namespace Input
    {
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
            PageUp,
            PageDown,
            Home,
            End,
            Capslock,
            Scrolllock,
            NumLock,
            PrintScreen,
            Pause,
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
            Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9, KeypadDecimal, KeypadDivide, KeypadMultiply, KeypadSubtract, KeypadAdd, KeypadEnter, KeypadEqual,
            LeftShift, LeftControl, LeftAlt, LeftSuper,
            RightShift, RightControl, RightAlt, RightSuper,
        }
        public enum ButtonState
        {
            Down,
            Repeat,
            Released,
        }
        public enum MouseButton
        {
            Left,
            Right,
            Middle
        }

        public partial class KeyBoard
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public static extern bool IsKeyDown(KeyCode code);
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public static extern bool IsKeyReleased(KeyCode code);
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public static extern bool IsKeyRepeat(KeyCode code);

            public static bool IsKey(KeyCode code, ButtonState state)
            {
                switch (state)
                {
                    case ButtonState.Down:
                        return IsKeyDown(code);
                    case ButtonState.Released:
                        return IsKeyReleased(code);
                    case ButtonState.Repeat:
                        return IsKeyRepeat(code);
                }
                return false;
            }
        }
        public partial class Mouse
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public static extern bool IsMouseButtonDown(MouseButton button);
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public static extern bool IsMouseButtonReleased(MouseButton button);
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            public static extern bool IsMouseButtonRepeat(MouseButton button);

            public static bool IsMouseButton(MouseButton button, ButtonState state)
            {
                switch (state)
                {
                    case ButtonState.Down:
                        return IsMouseButtonDown(button);
                    case ButtonState.Released:
                        return IsMouseButtonReleased(button);
                    case ButtonState.Repeat:
                        return IsMouseButtonRepeat(button);
                }
                return false;
            }

        }
    }
}