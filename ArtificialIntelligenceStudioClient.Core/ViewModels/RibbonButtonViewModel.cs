using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArtificialIntelligenceStudioClient.Core.ViewModels
{
    public class RibbonButtonViewModel : BindableBase
    {
        public string Title { get; set; }
        public string Description { get; set; }
        /// <summary>
        /// 默认为0:普通按钮
        /// 1: Fluent:ToggleButton, 需要更改
        /// </summary>
        public int ButtonType { get; set; }
        bool _isChecked = false;
        public bool IsChecked
        {
            get { return _isChecked; }
            set { SetProperty(ref _isChecked, value); }
        }
        public object? Icon { get; set; }
        public object? LargeIcon { get; set; }
        public DelegateCommand Command { get; set; }

    }
}
