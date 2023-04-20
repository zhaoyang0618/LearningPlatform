using LiveChartsCore.Kernel.Sketches;
using LiveChartsCore.Kernel;
using LiveChartsCore.Measure;
using LiveChartsCore.SkiaSharpView.Drawing.Geometries;
using LiveChartsCore.SkiaSharpView.Painting;
using LiveChartsCore.SkiaSharpView;
using LiveChartsCore;
using SkiaSharp;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LiveChartsCore.Defaults;
using System.Collections.ObjectModel;
using StockTraceApp.Models;
using static System.Runtime.InteropServices.JavaScript.JSType;
using LiveChartsCore.SkiaSharpView.Drawing;

namespace StockTraceApp.ViewModels
{
    public class SaleChartViewModel
    {
        private readonly ObservableCollection<SaleViewModel> _observablePoints;
        private readonly ObservableCollection<SaleViewModel> _observableSalePoints;
        public event Action<SaleViewModel?>? OnPointClicked;
        public SaleChartViewModel()
        {
            _observablePoints = new ObservableCollection<SaleViewModel>();
            _observableSalePoints = new ObservableCollection<SaleViewModel>();

            var buySeries = new LineSeries<SaleViewModel>
            {
                Name = "买入",
                Values = _observablePoints,
                TooltipLabelFormatter = point => $"{point.Model?.Name}: {point.Model?.Count}",
                DataLabelsPaint = new SolidColorPaint(new SKColor(30, 30, 30)),
                DataLabelsFormatter = point => $"{point.Model?.Count} {point.Model?.Name}",
                DataLabelsPosition = DataLabelsPosition.End,
                Mapping = (fruit, point) =>
                {
                    point.PrimaryValue = fruit.Value??0; // use the SalesPerDay property in this series // mark
                    point.SecondaryValue = fruit.DateTime.Ticks;
                }
            };

            // notice that the event signature is different for every series
            // use the  IDE intellisense to help you (see more bellow in this article). // mark
            //salesPerDaysSeries.ChartPointPointerDown += OnPointerDown; // mark
            //salesPerDaysSeries.ChartPointPointerHover += OnPointerHover; // mark
            //salesPerDaysSeries.ChartPointPointerHoverLost += OnPointerHoverLost; // mark
            buySeries.ChartPointPointerDown += OnChartPointPointerDown;

            var saleSeries = new LineSeries<SaleViewModel>
            {
                Name = "卖出",
                Values = _observableSalePoints,
                TooltipLabelFormatter = point => $"{point.Model?.Name}, sold {point.Model?.Count} items",
                DataLabelsPaint = new SolidColorPaint(new SKColor(30, 30, 30)),
                DataLabelsFormatter = point => $"{point.Model?.Count} {point.Model?.Name}",
                DataLabelsPosition = DataLabelsPosition.End,
                Mapping = (fruit, point) =>
                {
                    point.PrimaryValue = fruit.Value ?? 0; // use the SalesPerDay property in this series // mark
                    point.SecondaryValue = fruit.DateTime.Ticks;
                }
            };

            // notice that the event signature is different for every series
            // use the  IDE intellisense to help you (see more bellow in this article). // mark
            //salesPerDaysSeries.ChartPointPointerDown += OnPointerDown; // mark
            //salesPerDaysSeries.ChartPointPointerHover += OnPointerHover; // mark
            //salesPerDaysSeries.ChartPointPointerHoverLost += OnPointerHoverLost; // mark
            saleSeries.ChartPointPointerDown += OnChartPointPointerDown;

            Series = new ISeries[] { buySeries, saleSeries };
        }

        public ISeries[] Series { get; set; }
        public void Clear()
        {
            _observablePoints.Clear();
            _observableSalePoints.Clear();
        }

        public void AddSale(Sale sale)
        {
            if (sale.Direction == 0)
            {
                //买入
                _observablePoints.Add(new SaleViewModel() { Model = sale});
            }
            else
            {
                //卖出
                _observableSalePoints.Add(new SaleViewModel() { Model = sale });
            }
        }

        private void OnPointerDown(IChartView chart, ChartPoint<SaleViewModel, RoundedRectangleGeometry, LabelGeometry>? point)
        {
            if (point?.Visual is null) return;
            point.Visual.Fill = new SolidColorPaint(SKColors.Red);
            chart.Invalidate(); // <- ensures the canvas is redrawn after we set the fill
            Trace.WriteLine($"Clicked on {point.Model?.Name}, {point.Model?.Count} items sold per day");
        }

        private void OnChartPointPointerDown(IChartView chart, ChartPoint<SaleViewModel, BezierPoint<CircleGeometry>, LabelGeometry>? point)
        {
            //if (point?.Visual is null) return;
            //point.Visual.FillPath = new SolidColorPaint(SKColors.Red);
            //chart.Invalidate(); // <- ensures the canvas is redrawn after we set the fill
            OnPointClicked?.Invoke(point?.Model);
        }

        private void OnPointerHover(IChartView chart, ChartPoint<SaleViewModel, RoundedRectangleGeometry, LabelGeometry>? point)
        {
            if (point?.Visual is null) return;
            point.Visual.Fill = new SolidColorPaint(SKColors.Yellow);
            chart.Invalidate();
            Trace.WriteLine($"Pointer entered on {point.Model?.Name}");
        }

        private void OnPointerHoverLost(IChartView chart, ChartPoint<SaleViewModel, RoundedRectangleGeometry, LabelGeometry>? point)
        {
            if (point?.Visual is null) return;
            point.Visual.Fill = null;
            chart.Invalidate();
            Trace.WriteLine($"Pointer left {point.Model?.Name}");
        }
    }
}
