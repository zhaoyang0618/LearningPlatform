using StockTraceApp.DB;
using StockTraceApp.Models;
using StockTraceApp.ViewModels;
using System;
using System.Collections;
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

namespace StockTraceApp.UI
{
    /// <summary>
    /// Interaction logic for SaleEditWnd.xaml
    /// </summary>
    public partial class SaleEditWnd : UserControl
    {
        public event Action OnOK;
        public event Action OnCancel;
        public SaleEditWnd(Stock stock, Sale sale)
        {
            InitializeComponent();
            _stock = stock;
            _sale.Id = sale.Id;
            _sale.Time = sale.Time;
            _sale.Date = sale.Date;
            _sale.ClosingPrice = sale.ClosingPrice;
            _sale.Price = sale.Price;
            _sale.Direction = sale.Direction;   
            _sale.Flag = sale.Flag;
            _sale.LowestPrice = sale.LowestPrice;
            _sale.HighestPrice = sale.HighestPrice;
            _sale.OffsetCount = sale.OffsetCount;
            _sale.OpeningPrice = sale.OpeningPrice;
            _sale.SaleCount = sale.SaleCount;
            _sale.SaleId = sale.SaleId;
            _sale.ShanghaiHighestPrice = sale.ShanghaiHighestPrice;
            _sale.ShanghaiLowestPrice = sale.ShanghaiLowestPrice;
            _sale.ShenzhenHighestPrice = sale.ShenzhenHighestPrice;
            _sale.ShenzhenLowestPrice = sale.ShenzhenLowestPrice;
            _sale.StockId = sale.StockId;
            InitUI();
            BindEvents();
        }

        #region 事件处理
        private void OnButtonOKClick(object sender, RoutedEventArgs e)
        {
            try
            {
                var rep = new DB.SaleRepository(_context);
                if (_sale.Id == 0)
                {
                    rep.Insert(_sale);
                }
                else
                {
                    rep.Update(_sale);
                }
                _context.SaveChanges();
                OnOK?.Invoke();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }
        }

        #endregion

        #region 辅助函数
        private readonly StockDbContext _context = new StockDbContext();
        Sale _sale = new Sale();
        Stock _stock = null;
        void InitUI()
        {
            this.DataContext = _sale;
            var directions = new List<SaleDirection>();
            directions.Add(new SaleDirection() { Id = 0, Name = "买入" });
            directions.Add(new SaleDirection() { Id = 1, Name = "卖出" });
            cboDirection.ItemsSource = directions;
        }

        void BindEvents()
        {
            btnOK.Click += OnButtonOKClick;
            btnCancel.Click += (s, e) => {
                OnCancel?.Invoke();
            };
        }

        #endregion
    }
}
