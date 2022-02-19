using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArtificialIntelligenceStudioClient.Modules.ImageAnnotation.ViewModels
{
    internal class AnnotationWndViewModel : BindableBase
    {
        public string ImageFolder { get; set; }

        public AnnotationWndViewModel()
        {
            //或者有一个默认值
            this.ImageFolder = string.Empty;
        }
    }
}
