using System;
using Mage;
using Mage.Maths;

namespace Client
{
    class Example 
    {
        public void Start()
        {
        
        }
        public void Update()
        {
            if (!Event.WindowFocused())
            {
                Log.Warning("Window is not focuesed\n");
            }        
        }
        public void End()
        {
            
        }
    }
}