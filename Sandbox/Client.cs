using System;
using Mage;
using Mage.Maths;

namespace Client
{
    class Example 
    {
        public void Start()
        {      
            Log.Inform($"Hello World {10}\n");
            Log.Warning($"Hello World {10}\n");
            Log.Error($"Hello World {10}\n");
            Log.FatalError($"Hello World {10}\n");   
        }
        public void Update()
        {
                
        }
        public void Destroy()
        {

        }
    }
}