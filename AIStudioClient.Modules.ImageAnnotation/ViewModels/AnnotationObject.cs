using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AIStudioClient.Modules.ImageAnnotation.ViewModels
{
    /// <summary>
    /// 需要标注的对象
    /// </summary>
    public class AnnotationObject
    {
        /// <summary>
        /// 唯一
        /// </summary>
        public int id { get; set; }
        /// <summary>
        /// 名称
        /// </summary>
        public string name { get; set; }
        /// <summary>
        /// 这一次是否需要被用到
        /// </summary>
        public int isUsed { get; set; }
    }
}
