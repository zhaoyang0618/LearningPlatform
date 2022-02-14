using ControlzEx.Theming;
using Fluent;
using MahApps.Metro.Controls;
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
        public MainWindow()
        {
            InitializeComponent();
            //this.TestContent.Backstage.UseHighestAvailableAdornerLayer = false;
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

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            AI.TorchExample.Test();
        }
    }
}
