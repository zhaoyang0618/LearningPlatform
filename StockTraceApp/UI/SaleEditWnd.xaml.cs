using StockTraceApp.DB;
using StockTraceApp.Models;
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

namespace StockTraceApp.UI
{
    /// <summary>
    /// Interaction logic for SaleEditWnd.xaml
    /// </summary>
    public partial class SaleEditWnd : UserControl
    {
        public event Action OnOK;
        public event Action OnCancel;
        public SaleEditWnd(Sale sale)
        {
            InitializeComponent();
            _stock.Id = sale.Id;
            InitUI();
            BindEvents();
        }

        #region 事件处理
        private void OnButtonOKClick(object sender, RoutedEventArgs e)
        {
            try
            {
                //var rep = new DB.StockRepository(_context);
                //rep.Insert(_stock);
                //_context.SaveChanges();
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
        Sale _stock = new Sale();
        void InitUI()
        {
            this.DataContext = _stock;
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
