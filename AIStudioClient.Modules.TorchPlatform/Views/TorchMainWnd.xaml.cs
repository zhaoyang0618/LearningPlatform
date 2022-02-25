using ArtificialIntelligenceStudioClient.Core;
using ArtificialIntelligenceStudioClient.Core.ViewModels;
using Microsoft.Extensions.Logging;
using Prism.Commands;
using Prism.Events;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace AIStudioClient.Modules.TorchPlatform.Views
{
    /// <summary>
    /// Interaction logic for TorchMainWnd.xaml
    /// </summary>
    public partial class TorchMainWnd : UserControl
    {
        public TorchMainWnd(ILogger logger, IEventAggregator eventAggregator, TorchPlatformModule module)
        {
            InitializeComponent();
            _logger = logger;
            _eventAggregator = eventAggregator;
            _module = module;
            InitUI();
            BindEvents();
        }

        #region 事件处理
        private void OnWndLoaded(object sender, RoutedEventArgs e)
        {
            var args = new ModuleLoadedEventArgs()
            {
                Title = _module.Title,
                Key = typeof(TorchPlatformModule).Name,
            };
            args.Buttons.AddRange(_buttons);
            _eventAggregator.GetEvent<ModuleLoadedEvent>().Publish(args);
        }

        private void OnWndUnloaded(object sender, RoutedEventArgs e)
        {
            var args = new ModuleUnloadEventArgs()
            {
                Key = typeof(TorchPlatformModule).Name,
            };
            _eventAggregator.GetEvent<ModuleUnloadEvent>().Publish(args);
        }

        private void OnCommandBPNet()
        {
        }

        #endregion

        #region 辅助函数
        ILogger _logger;
        IEventAggregator _eventAggregator;
        TorchPlatformModule _module;
        RibbonButtonViewModel? _buttonBPNet = null;
        DelegateCommand? _delegateCommandSelectImageFolder;
        List<RibbonButtonViewModel> _buttons = new List<RibbonButtonViewModel>();

        void InitUI()
        {
            _delegateCommandSelectImageFolder = new DelegateCommand(OnCommandBPNet);
            _buttonBPNet = new RibbonButtonViewModel()
            {
                Title = "BP神经网络",
                Description = "BP神经网络",
                Command = _delegateCommandSelectImageFolder,
            };
            _buttons.Add(_buttonBPNet);
        }

        void BindEvents()
        {
            this.Loaded += OnWndLoaded;
            this.Unloaded += OnWndUnloaded;
        }
        #endregion
    }
}
