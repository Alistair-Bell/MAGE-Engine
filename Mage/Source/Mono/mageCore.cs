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
}