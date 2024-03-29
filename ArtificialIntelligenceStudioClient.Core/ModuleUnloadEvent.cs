﻿using Prism.Events;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArtificialIntelligenceStudioClient.Core
{
    public class ModuleUnloadEventArgs : EventArgs
    {
        public string Key { get; set; }
    }

    public class ModuleUnloadEvent : PubSubEvent<ModuleUnloadEventArgs>
    {
    }
}
