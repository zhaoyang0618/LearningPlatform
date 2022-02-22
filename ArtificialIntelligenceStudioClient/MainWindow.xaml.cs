using ArtificialIntelligenceStudioClient.Core;
using ArtificialIntelligenceStudioClient.Core.ViewModels;
using ControlzEx.Theming;
using Fluent;
using MahApps.Metro.Controls;
using Prism.Events;
using Prism.Modularity;
using Prism.Regions;
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

namespace ArtificialIntelligenceStudioClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : IRibbonWindow
    {
        IModuleManager _moduleManager = null;
        IRegionManager _regionManager = null;
        LocalAppContext _localAppContext = null;
        IEventAggregator _eventAggregator;
        public MainWindow(IModuleManager moduleManager, IRegionManager regionManager, LocalAppContext localAppContext, IEventAggregator eventAggregator)
        {
            InitializeComponent();
            //this.TestContent.Backstage.UseHighestAvailableAdornerLayer = false;
            _moduleManager = moduleManager;
            _regionManager = regionManager;
            _localAppContext = localAppContext;
            _eventAggregator = eventAggregator;
            InitUI();
            BindEvents();
            this.Loaded += this.MahMetroWindowLoaded;
            this.Closed += this.MahMetroWindowClosed;

        }

        #region TitelBar

        /// <summary>
        /// Gets ribbon titlebar
        /// </summary>
        public RibbonTitleBar TitleBar
        {
            get { return (RibbonTitleBar)this.GetValue(TitleBarProperty); }
            private set { this.SetValue(TitleBarPropertyKey, value); }
        }

        // ReSharper disable once InconsistentNaming
        private static readonly DependencyPropertyKey TitleBarPropertyKey = DependencyProperty.RegisterReadOnly(nameof(TitleBar), typeof(RibbonTitleBar), typeof(MainWindow), new PropertyMetadata());

#pragma warning disable WPF0060
        /// <summary>Identifies the <see cref="TitleBar"/> dependency property.</summary>
        public static readonly DependencyProperty TitleBarProperty = TitleBarPropertyKey.DependencyProperty;
#pragma warning restore WPF0060

        #endregion

        #region 事件处理
        private void MahMetroWindowLoaded(object sender, RoutedEventArgs e)
        {
            this.TitleBar = this.FindChild<RibbonTitleBar>("RibbonTitleBar");
            this.TitleBar.InvalidateArrange();
            this.TitleBar.UpdateLayout();

            // We need this inside this window because MahApps.Metro is not loaded globally inside the Fluent.Ribbon Showcase application.
            // This code is not required in an application that loads the MahApps.Metro styles globally.
            ThemeManager.Current.ChangeTheme(this, ThemeManager.Current.DetectTheme(Application.Current));
            ThemeManager.Current.ThemeChanged += this.OnSyncThemes;
        }

        private void MahMetroWindowClosed(object sender, EventArgs e)
        {
            ThemeManager.Current.ThemeChanged -= this.OnSyncThemes;
        }

        private void OnSyncThemes(object sender, ThemeChangedEventArgs e)
        {
            if (e.Target == this)
            {
                return;
            }

            ThemeManager.Current.ChangeTheme(this, e.NewTheme);
        }

        private void OnModuleLoadedEvent(ModuleLoadedEventArgs arg)
        {
            //这个时候加载菜单
            AddTabItem(arg.Key, arg.Title, arg.Buttons);
        }

        private void OnModuleUnloadEvent(ModuleUnloadEventArgs arg)
        {
            //这个时候删除菜单
            RemoveTabItem(arg.Key);
        }

        private void OnButtonHomeClick(object sender, RoutedEventArgs e)
        {
            //首页
            //_moduleManager.LoadModule<Modules.HomeModule>();
            _regionManager.RequestNavigate(RegionNames.ContentRegion, "HomeWnd");
        }

        private void OnButtonExitClick(object sender, RoutedEventArgs e)
        {
            Close();
        }

        #endregion

        #region 辅助函数
        void InitUI()
        {
            //动态创建菜单
            if (_localAppContext != null && _localAppContext.Modules != null)
            {
                foreach (var module in _localAppContext.Modules)
                {
                    //最后一个位置需要留给“退出”按钮
                    var btn = new Fluent.Button()
                    {
                        Header = module.moduleName,
                    };
                    btn.Click += (s, e) => {
                        //打开对应的内容
                        if(!_moduleManager.IsModuleInitialized(module.moduleName))
                        {
                            _moduleManager.LoadModule(module.moduleName);
                        }
                        _regionManager.RequestNavigate(RegionNames.ContentRegion, module.viewName);
                    };

                    menuMain.Items.Add(btn);
                }
            }    
        }

        void BindEvents()
        {
            _eventAggregator.GetEvent<ModuleLoadedEvent>().Subscribe(OnModuleLoadedEvent);
            _eventAggregator.GetEvent<ModuleUnloadEvent>().Subscribe(OnModuleUnloadEvent);

        }

        void AddTabItem(string key, string title, List<RibbonButtonViewModel> buttons)
        {
            if (buttons == null || buttons.Count == 0) return;
            RibbonTabItem cur = null;
            foreach (var tab in ribbonMain.Tabs)
            {
                if (tab.Tag != null && tab.Tag.Equals(key))
                {
                    cur = tab;
                    break;
                }
            }

            if (cur == null)
            {
                cur = new RibbonTabItem();
                cur.Header = title;
                cur.Tag = key;

                var group = new RibbonGroupBox();

                foreach (var button in buttons)
                {
                    if(button.ButtonType == 0)
                    {
                        var btn = new Fluent.Button();
                        btn.DataContext = button;
                        btn.Header = button.Title;
                        btn.Command = button.Command;
                        group.Items.Add(btn);
                    }
                    else
                    {
                        var btn = new Fluent.ToggleButton();
                        btn.DataContext = button;
                        btn.Header = button.Title;
                        Binding myBinding = new Binding("IsChecked");
                        myBinding.Source = button;
                        BindingOperations.SetBinding(btn, Fluent.ToggleButton.IsCheckedProperty, myBinding);
                        btn.Command = button.Command;
                        group.Items.Add(btn);
                    }
                }

                cur.Groups.Add(group);

                ribbonMain.Tabs.Add(cur);
            }

            ribbonMain.SelectedTabItem = cur;
        }

        void RemoveTabItem(string key)
        {
            RibbonTabItem cur = null;
            foreach (var tab in ribbonMain.Tabs)
            {
                if (tab.Tag != null && tab.Tag.Equals(key))
                {
                    cur = tab;
                    break;
                }
            }

            if (cur != null)
            {
                ribbonMain.Tabs.Remove(cur);
                ribbonMain.SelectedTabItem = null;
            }
        }

        #endregion
    }
}
