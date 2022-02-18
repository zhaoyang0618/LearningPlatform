using ArtificialIntelligenceStudioClient.Core.ViewModels;
using Prism.Commands;
using Prism.Events;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArtificialIntelligenceStudioClient.Core
{
    public class ModuleLoadedEventArgs : EventArgs
    {
        public string Key { get; set; }
        public List<RibbonButtonViewModel> Buttons { get; set; }
        
    }
    public class ModuleLoadedEvent : PubSubEvent<ModuleLoadedEventArgs>
    {
    }
}
