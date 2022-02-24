using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AIStudioClient.Modules.ImageAnnotation.ViewModels
{
    internal class ImageLabelViewModel
    {
        public string Name { get; set; }
        public string ImageFilePath { get; set; }
        public string LabelFilePath
        {
            get
            {
                if(string.IsNullOrEmpty(ImageFilePath))return null;
                return string.Format("{0}.txt", ImageFilePath);
            }
        }
    }
}
