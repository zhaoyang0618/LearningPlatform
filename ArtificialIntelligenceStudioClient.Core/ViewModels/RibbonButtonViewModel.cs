using Prism.Commands;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArtificialIntelligenceStudioClient.Core.ViewModels
{
    public class RibbonButtonViewModel
    {
        public string Title { get; set; }
        public string Description { get; set; }
        public object? Icon { get; set; }
        public object? LargeIcon { get; set; }
        public DelegateCommand Command { get; set; }

    }
}
