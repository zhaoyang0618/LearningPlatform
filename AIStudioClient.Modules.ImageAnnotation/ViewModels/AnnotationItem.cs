using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AIStudioClient.Modules.ImageAnnotation.ViewModels
{
    /// <summary>
    /// 标注数据
    /// </summary>
    public class AnnotationItem
    {
        /// <summary>
        /// 对象的Id
        /// </summary>
        public int objId { get; set; }
        /// <summary>
        /// 下面四个参数表示的是位置
        /// 不过需要作一个归一化处理所以取值在0-1之间
        /// </summary>
        public double x { get; set; }
        public double y { get; set; }
        public double w { get; set; }
        public double h { get; set; }
        public string name { get; set; }
    }

}
