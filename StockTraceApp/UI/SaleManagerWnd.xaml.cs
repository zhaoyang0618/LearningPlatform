using LiveChartsCore;
using LiveChartsCore.Defaults;
using LiveChartsCore.Kernel.Sketches;
using LiveChartsCore.SkiaSharpView;
using LiveChartsCore.SkiaSharpView.Painting;
using SkiaSharp;
using StockTraceApp.Models;
using StockTraceApp.ViewModels;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
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
            //_observablePoints = new ObservableCollection<DateTimePoint>();
            //_observableSalePoints = new ObservableCollection<DateTimePoint>();
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
            if(_currentStock == null)
                return;

            var sale = new Sale()
            {
                StockId = _currentStock.Id,
                Date = DateTime.Now,
                Time = DateTime.Now,
            };
            var wnd = new SaleEditWnd(_currentStock, sale);
            wnd.OnOK += () => {
                ListSales(_currentStock.Id);
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
                if (stock != null)
                {
                    textCode.Text = stock.Code;
                    textName.Text = stock.Name;
                    _currentStock = stock;
                    ListSales(_currentStock.Id);
                }
                else
                {
                    _currentStock = null;
                    //清空数据
                }
            }
        }

        private void SaleSeries_ChartPointPointerDown(IChartView chart, LiveChartsCore.Kernel.ChartPoint<DateTimePoint, LiveChartsCore.SkiaSharpView.Drawing.BezierPoint<LiveChartsCore.SkiaSharpView.Drawing.Geometries.CircleGeometry>, LiveChartsCore.SkiaSharpView.Drawing.Geometries.LabelGeometry>? point)
        {
            if (point?.Visual is null) return;
            Trace.WriteLine("Clicked");
            //point.Visual.Fill = new SolidColorPaint(SKColors.Red);
            //chart.Invalidate(); // <- ensures the canvas is redrawn after we set the fill
            //Trace.WriteLine($"Clicked on {point.Model?.Name}, {point.Model?.SalesPerDay} items sold per day");
        }

        private void OnChartPointClicked(SaleViewModel? obj)
        {
            if(obj == null) return;
            if(_currentStock == null) return;
            var wnd = new SaleEditWnd(_currentStock, obj.Model);
            wnd.OnOK += () => {
                ListSales(_currentStock.Id);
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
        Stock? _currentStock = null;
        DB.StockDbContext _db = new DB.StockDbContext();
        void InitUI()
        {
            //var buySeries = new LineSeries<DateTimePoint>
            //{
            //    Name = "Buy",
            //    //TooltipLabelFormatter =
            //    //    chartPoint => $"{new DateTime((long)chartPoint.SecondaryValue):HH:mm:ss}: {chartPoint.PrimaryValue}",
            //    Values = _observablePoints,
            //    //Fill = null
            //};
            //buySeries.ChartPointPointerDown += SaleSeries_ChartPointPointerDown;
            //var saleSeries = new LineSeries<DateTimePoint>
            //{
            //    Name = "Sale",
            //    //TooltipLabelFormatter =
            //    //    chartPoint => $"{new DateTime((long)chartPoint.SecondaryValue):HH:mm:ss}: {chartPoint.PrimaryValue}",
            //    Values = _observableSalePoints,
            //    //Fill = null
            //};
            //saleSeries.ChartPointPointerDown += SaleSeries_ChartPointPointerDown;
            //Series = new ObservableCollection<ISeries>
            //{
            //    buySeries,
            //    saleSeries
            //};
            //
            XAxes = new LiveChartsCore.SkiaSharpView.Axis[]
                {
                        new LiveChartsCore.SkiaSharpView.Axis
                        {
                            Labeler = value => { try{return new DateTime((long) value).ToString("yyyy-MM-dd"); }catch{ return string.Empty; }  },
                            LabelsRotation = 15,
                            LabelsPaint = new SolidColorPaint(SKColors.White),

                            // when using a date time type, let the library know your unit // mark
                            UnitWidth = TimeSpan.FromDays(1).Ticks, // mark

                            // if the difference between our points is in hours then we would:
                            // UnitWidth = TimeSpan.FromHours(1).Ticks,

                            // since all the months and years have a different number of days
                            // we can use the average, it would not cause any visible error in the user interface
                            // Months: TimeSpan.FromDays(30.4375).Ticks
                            // Years: TimeSpan.FromDays(365.25).Ticks

                            // The MinStep property forces the separator to be greater than 1 day.
                            //MinStep = TimeSpan.FromMilliseconds(100).Ticks // mark
                            MinStep = TimeSpan.FromHours(1).Ticks // mark
                }
                };

            //
            //charts.Series = this.Series;
            charts.Series = _saleChart.Series;
            charts.XAxes = this.XAxes;
            charts.LegendTextPaint = new SolidColorPaint(SKColors.White) { FontFamily = "微软雅黑" };
            _saleChart.OnPointClicked += OnChartPointClicked;

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
            _stocks.Clear();
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

        SaleChartViewModel _saleChart = new SaleChartViewModel();
        //private readonly ObservableCollection<DateTimePoint> _observablePoints;
        //private readonly ObservableCollection<DateTimePoint> _observableSalePoints;

        //public ObservableCollection<ISeries> Series { get; set; }
        public IEnumerable<ICartesianAxis> XAxes { get; set; }

        void ListSales(int stockId)
        {
            _saleChart.Clear();
            //_observablePoints.Clear();
            Task.Run(() => {
                try
                {
                    var rep = new DB.SaleRepository(_db);
                    var list = rep.Get(x => x.StockId.Equals(stockId), x => x.OrderBy(y => y.Date));
                    if (list != null)
                    {
                        this.Dispatcher.Invoke(() => {
                            foreach (var item in list)
                            {
                                _saleChart.AddSale(item);
                                //var date = new DateTime(item.Date.Year, item.Date.Month, item.Date.Day, item.Time.Hour, item.Time.Minute, item.Time.Second);
                                //if (item.Direction == 0)
                                //{
                                //    //买入
                                //    _observablePoints.Add(new DateTimePoint(date, item.Price));
                                //}
                                //else
                                //{
                                //    //卖出
                                //    _observableSalePoints.Add(new DateTimePoint(date, item.Price));
                                //}
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
