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
        public string Title { get; set; }
        public List<RibbonButtonViewModel> Buttons { get; set; }
        public ModuleLoadedEventArgs()
        {
            this.Key = String.Empty;
            this.Title = String.Empty;
            this.Buttons = new List<RibbonButtonViewModel>();
        }

    }
    public class ModuleLoadedEvent : PubSubEvent<ModuleLoadedEventArgs>
    {
    }
}
