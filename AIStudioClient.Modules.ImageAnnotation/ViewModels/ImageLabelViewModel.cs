using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AIStudioClient.Modules.ImageAnnotation.ViewModels
{
    public class ImageLabelViewModel
    {
        public string Name { get; set; }
        public string ImageFilePath { get; set; }
        /// <summary>
        /// 标签使用JSON数据保存
        /// </summary>
        public string LabelFilePath
        {
            get
            {
                if(string.IsNullOrEmpty(ImageFilePath))return null;
                return string.Format("{0}.json", ImageFilePath);
            }
        }

        public List<AnnotationItem> AnnotatedObjects { get; set; }
        public ImageLabelViewModel()
        {
            this.AnnotatedObjects = new List<AnnotationItem>();
        }
    }
}
