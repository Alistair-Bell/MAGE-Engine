using System;
using System.Collections.Generic;
using System.Collections;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Mage
{
    #region Debug

    /* Interact with the logging system */
    public partial class Log
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern Inform(string message);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern Warning(string message);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern Error(string message);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern Fatal(string message);
    }
    
    #endregion
}