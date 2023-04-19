using SkiaSharp;
using StockTraceApp.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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

namespace StockTraceApp.UI
{
    /// <summary>
    /// Interaction logic for StockManagerWnd.xaml
    /// </summary>
    public partial class StockManagerWnd : UserControl
    {
        public StockManagerWnd()
        {
            InitializeComponent();
            InitUI();
            BindEvents();
        }

        #region 事件处理
        private void OnWndLoaded(object sender, RoutedEventArgs e)
        {
            //获取股票数据
            ListStocks();
        }

        private void OnButtonAddClick(object sender, RoutedEventArgs e)
        {
            //添加
            var stock = new Stock();
            var wnd = new StockEditWnd(stock);
            wnd.OnOK += ()=>{
                ListStocks();
                gridContainer.Children.Clear();
                gridSub.Visibility = Visibility.Collapsed;
            };
            wnd.OnCancel += () => {
                gridContainer.Children.Clear();
                gridSub.Visibility = Visibility.Collapsed;
            };

            gridContainer.Children.Clear();
            gridContainer.Children.Add(wnd);
            gridSub.Visibility = Visibility.Visible;
        }
        #endregion

        #region 辅助函数
        ObservableCollection<Stock> _stocks = new ObservableCollection<Stock>();
        DB.StockDbContext _db = new DB.StockDbContext();
        void InitUI()
        {
            datagridStocks.ItemsSource = _stocks;
        }

        void BindEvents()
        {
            this.Loaded += OnWndLoaded;
            btnAdd.Click += OnButtonAddClick;
        }

        void ListStocks()
        {
            Task.Run(() => {
                try
                {
                    var rep = new DB.StockRepository(_db);
                    var list = rep.Get();
                    if (list != null)
                    {
                        this.Dispatcher.Invoke(() => {
                            foreach (var item in list)
                            {
                                _stocks.Add(item);
                            }
                        });
                    }
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Debug.WriteLine(ex);
                }
            });
        }
        #endregion
    }
}
