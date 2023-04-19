using MahApps.Metro.Controls;
using StockTraceApp.DB;
using StockTraceApp.ViewModels;
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
using static System.Net.Mime.MediaTypeNames;

namespace StockTraceApp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        public MainWindow()
        {
            InitializeComponent();
            InitUI();
            BindEvents();
        }

        #region 事件处理
        private void OnWindowLoaded(object sender, RoutedEventArgs e)
        {
            try
            { 
                _context.Database.EnsureCreated();
            }
            catch(Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }
        }

        private void OnSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //工具栏按钮
            if (e.AddedItems == null || e.AddedItems.Count == 0)
                return;

            listitemToolbar.SelectionChanged -= OnSelectionChanged;
            //
            try
            {
                var item = e.AddedItems[0] as ItemMenu;
                if (item != null)
                {
                    //需要把其他的选项的IsSelected变为false
                    item.Command?.Execute(null);
                }
            }
            catch (Exception exp)
            {

            }
            //
            listitemToolbar.SelectionChanged += OnSelectionChanged;

        }

        #endregion

        #region 辅助函数
        private readonly StockDbContext _context = new StockDbContext();
        void InitUI()
        {
            //左侧工具栏
            var tools = new List<ItemMenu>();
            var cmd1 = new Microsoft.Xaml.Behaviors.Core.ActionCommand(() => {
                textSubHeader.Text = "买卖信息";
                gridSub.Children.Clear();
                var wnd = new UI.SaleManagerWnd();
                gridSub.Children.Add(wnd);

            });
            tools.Add(new ItemMenu("股票买卖", MaterialDesignThemes.Wpf.PackIconKind.Sale, cmd1));
            var cmd2 = new Microsoft.Xaml.Behaviors.Core.ActionCommand(() => {
                textSubHeader.Text = "股票信息";
                gridSub.Children.Clear();
                var wnd = new UI.StockManagerWnd();
                gridSub.Children.Add(wnd);
            });
            tools.Add(new ItemMenu("股票管理", MaterialDesignThemes.Wpf.PackIconKind.Stocking, cmd2));
            listitemToolbar.ItemsSource = tools;
        }

        void BindEvents()
        {
            this.Loaded += OnWindowLoaded;
            listitemToolbar.SelectionChanged += OnSelectionChanged;
            listitemToolbar.SelectedIndex = 0;
        }

        #endregion
    }
}
