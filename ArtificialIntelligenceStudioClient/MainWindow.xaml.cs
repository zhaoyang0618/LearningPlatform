﻿using ControlzEx.Theming;
using Fluent;
using MahApps.Metro.Controls;
using Prism.Modularity;
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
        public MainWindow(IModuleManager moduleManager)
        {
            InitializeComponent();
            //this.TestContent.Backstage.UseHighestAvailableAdornerLayer = false;
            _moduleManager = moduleManager;
            InitUI();
            this.Loaded += this.MahMetroWindow_Loaded;
            this.Closed += this.MahMetroWindow_Closed;

        }

        private void MahMetroWindow_Loaded(object sender, RoutedEventArgs e)
        {
            this.TitleBar = this.FindChild<RibbonTitleBar>("RibbonTitleBar");
            this.TitleBar.InvalidateArrange();
            this.TitleBar.UpdateLayout();

            // We need this inside this window because MahApps.Metro is not loaded globally inside the Fluent.Ribbon Showcase application.
            // This code is not required in an application that loads the MahApps.Metro styles globally.
            ThemeManager.Current.ChangeTheme(this, ThemeManager.Current.DetectTheme(Application.Current));
            ThemeManager.Current.ThemeChanged += this.SyncThemes;
        }

        private void SyncThemes(object sender, ThemeChangedEventArgs e)
        {
            if (e.Target == this)
            {
                return;
            }

            ThemeManager.Current.ChangeTheme(this, e.NewTheme);
        }

        private void MahMetroWindow_Closed(object sender, EventArgs e)
        {
            ThemeManager.Current.ThemeChanged -= this.SyncThemes;
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
        private void OnButtonTestClick(object sender, RoutedEventArgs e)
        {
            AI.TorchExample.Test();
        }

        private void OnButtonAnnotationClick(object sender, RoutedEventArgs e)
        {
            var wnd = new UI.ImageAnnotationWnd();
            gridChild.Children.Add(wnd);
        }

        #endregion

        #region 辅助函数
        void InitUI()
        {
            //var item = new RibbonTabItem();
            //item.Header = "图像标注";
            //item.Tag = "图像标注";

            //var group = new RibbonGroupBox();

            //var btn = new Fluent.Button();
            //btn.Header = "选择文件夹";
            //group.Items.Add(btn);

            //item.Groups.Add(group);

            //ribbonMain.Tabs.Add(item);

            ////ribbonMain.SelectedTabItem = item;
            //ribbonMain.SelectedTabIndex = 1;
            ////Prism.Regions.RegionManager.SetRegionName(item)
            ////ribbonMain.Tabs.Add
        }

        void AddTabItem(string title)
        {
            RibbonTabItem cur = null;
            foreach (var tab in ribbonMain.Tabs)
            {
                if(tab.Tag != null && tab.Tag.Equals(title))
                {
                    cur = tab;
                    break;
                }
            }

            if(cur == null)
            {
                cur = new RibbonTabItem();
                cur.Header = title;
                cur.Tag = title;

                var group = new RibbonGroupBox();

                var btn = new Fluent.Button();
                btn.Header = "选择文件夹";
                group.Items.Add(btn);

                cur.Groups.Add(group);

                ribbonMain.Tabs.Add(cur);
            }

            ribbonMain.SelectedTabItem = cur;
        }

        #endregion

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            //首先查找是否已经存在,如果不存在，就添加
            AddTabItem("图像标注");
        }
    }
}
