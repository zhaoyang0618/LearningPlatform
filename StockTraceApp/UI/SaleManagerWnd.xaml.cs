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
    /// Interaction logic for SaleManagerWnd.xaml
    /// </summary>
    public partial class SaleManagerWnd : UserControl
    {
        public SaleManagerWnd()
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
            var sale = new Sale();
            var wnd = new SaleEditWnd(sale);
            wnd.OnOK += () => {
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

        private void OnStocksSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if(e.AddedItems.Count > 0)
            {
                var stock = e.AddedItems[0] as Stock;
                if(stock != null)
                {
                    textCode.Text = stock.Code;
                    textName.Text = stock.Name;
                }
            }
        }
        #endregion

        #region 辅助函数
        ObservableCollection<Stock> _stocks = new ObservableCollection<Stock>();
        DB.StockDbContext _db = new DB.StockDbContext();
        void InitUI()
        {
            cboStocks.ItemsSource = _stocks;
        }

        void BindEvents()
        {
            cboStocks.SelectionChanged += OnStocksSelectionChanged;
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
                            if(_stocks.Count > 0)
                            {
                                cboStocks.SelectedIndex = 0;
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
