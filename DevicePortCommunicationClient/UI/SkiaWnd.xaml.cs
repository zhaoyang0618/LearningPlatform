using OxyPlot;
using OxyPlot.Annotations;
using OxyPlot.Axes;
using OxyPlot.Legends;
using OxyPlot.Series;
using SkiaSharp;
using SkiaSharp.Views.Desktop;
using System;
using System.Collections.Generic;
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

namespace DevicePortCommunicationClient.UI
{
    /// <summary>
    /// Interaction logic for SkiaWnd.xaml
    /// </summary>
    public partial class SkiaWnd : UserControl
    {
        public event Action OnClosed;
        public SkiaWnd()
        {
            InitializeComponent();
            this.Loaded += OnWndLoaded;
        }

        private void OnWndLoaded(object sender, RoutedEventArgs e)
        {
			DrawPlot();

		}

        private void OnButtonCloseClick(object sender, RoutedEventArgs e)
        {
            OnClosed?.Invoke();
        }

		private void OnPaintSurface(object sender, SKPaintSurfaceEventArgs e)
		{
			// the the canvas and properties
			var canvas = e.Surface.Canvas;

			// make sure the canvas is blank
			canvas.Clear(SKColors.White);

			// draw some text
			var paint = new SKPaint
			{
				Color = SKColors.Black,
				IsAntialias = true,
				Style = SKPaintStyle.Fill,
				TextAlign = SKTextAlign.Center,
				TextSize = 24
			};
			var coord = new SKPoint(e.Info.Width / 2, (e.Info.Height + paint.TextSize) / 2);
			canvas.DrawText("SkiaSharp", coord, paint);

			using var linePaint = new SKPaint()
			{
				Color = (DateTimeOffset.Now.Second % 4 <= 1) ? SKColors.Red : SKColors.Green,
				Style = SKPaintStyle.Stroke,//不填充
				StrokeWidth = 3,
			};
			canvas.DrawRect(10, 50, e.Info.Width - 20, e.Info.Height - 60, linePaint);
		}

		void DrawPlot()
        {
            //var PlotModel = new PlotModel();
            //PlotModel.Axes.Add(new LinearAxis { Position = AxisPosition.Left, Minimum = -2, Maximum = 2 });

            //for (int i = 0; i < 4; i++)
            //{
            //	PlotModel.Series.Add(new LineSeries { LineStyle = LineStyle.Solid });
            //}

            //plotView.Model = PlotModel;

            ////
            //var myModel = new PlotModel { Title = "Example 1", Background = OxyColors.White };
            //myModel.Series.Add(new FunctionSeries(Math.Cos, 0, 10, 0.1, "cos(x)"));
            //myModel.Series.Add(new FunctionSeries(Math.Sin, -10, 10, 0.1, "sin(x)"));
            //myModel.Series.Add(new FunctionSeries(t => 5 * Math.Cos(t), t => 5 * Math.Sin(t), 0, 2 * Math.PI, 0.1, "cos(t),sin(t)"));
            //this.plotView.Model = myModel;

            //Test2();

            //Task.Factory.StartNew(() =>
            //{
            //    while (true)
            //    {
            //        this.Dispatcher.Invoke(() => {
            //            var date = DateTime.Now;
            //            _myPlotModel.Axes[0].Maximum = DateTimeAxis.ToDouble(date.AddSeconds(1));

            //            var lineSer = plotView.Model.Series[0] as LineSeries;
            //            lineSer.Points.Add(new DataPoint(DateTimeAxis.ToDouble(date), rand.Next(100, 300) / 10.0));
            //            if (lineSer.Points.Count > 100)
            //            {
            //                lineSer.Points.RemoveAt(0);
            //            }

            //            lineSer = plotView.Model.Series[1] as LineSeries;
            //            lineSer.Points.Add(new DataPoint(DateTimeAxis.ToDouble(date), rand.Next(350, 750) / 10.0));
            //            if (lineSer.Points.Count > 100)
            //            {
            //                lineSer.Points.RemoveAt(0);
            //            }

            //            _myPlotModel.InvalidatePlot(true);

            //            Thread.Sleep(1000);
            //        });
            //    }
            //});

            Test1();
        }

        private void Test2()
        {
            //
            var _myPlotModel = new PlotModel()
            {
                Title = "Temp & Humi",
                //LegendTitle = "Legend",
                //LegendOrientation = LegendOrientation.Horizontal,
                //LegendPlacement = LegendPlacement.Inside,
                //LegendPosition = LegendPosition.TopRight,
                //LegendBackground = OxyColor.FromAColor(200, OxyColors.Beige),
                //LegendBorder = OxyColors.Black
            };

            //
            var l = new Legend
            {
                LegendTitle = "Temp",
                LegendPlacement = LegendPlacement.Inside,
                LegendPosition = LegendPosition.RightTop,
                LegendBackground = OxyColor.FromAColor(200, OxyColors.White),
                LegendBorder = OxyColors.Black,
            };

            _myPlotModel.Legends.Add(l);

            //X轴
            var _dateAxis = new DateTimeAxis()
            {
                MajorGridlineStyle = LineStyle.Solid,
                MinorGridlineStyle = LineStyle.Dot,
                IntervalLength = 80,
                //IsZoomEnabled = false,
                //IsPanEnabled = false
            };
            _myPlotModel.Axes.Add(_dateAxis);

            //Y轴
            var _valueAxis = new LinearAxis()
            {
                MajorGridlineStyle = LineStyle.Solid,
                MinorGridlineStyle = LineStyle.Dot,
                IntervalLength = 80,
                Angle = 60,
                IsZoomEnabled = false,
                IsPanEnabled = false,
                Maximum = 100,
                Minimum = -1
            };
            _myPlotModel.Axes.Add(_valueAxis);

            //添加标注线，温度上下限和湿度上下限
            var lineTempMaxAnnotation = new OxyPlot.Annotations.LineAnnotation()
            {
                Type = LineAnnotationType.Horizontal,
                Color = OxyColors.Red,
                LineStyle = LineStyle.Solid,
                Y = 10,
                Text = "Temp MAX:10"
            };
            _myPlotModel.Annotations.Add(lineTempMaxAnnotation);

            var lineTempMinAnnotation = new LineAnnotation()
            {
                Type = LineAnnotationType.Horizontal,
                Y = 30,
                Text = "Temp Min:30",
                Color = OxyColors.Red,
                LineStyle = LineStyle.Solid
            };
            _myPlotModel.Annotations.Add(lineTempMinAnnotation);

            var lineHumiMaxAnnotation = new OxyPlot.Annotations.LineAnnotation()
            {
                Type = LineAnnotationType.Horizontal,
                Color = OxyColors.Red,
                LineStyle = LineStyle.Solid,
                //lineMaxAnnotation.MaximumX = 0.8;
                Y = 75,
                Text = "Humi MAX:75"
            };
            _myPlotModel.Annotations.Add(lineHumiMaxAnnotation);

            var lineHumiMinAnnotation = new LineAnnotation()
            {
                Type = LineAnnotationType.Horizontal,
                Y = 35,
                Text = "Humi Min:35",
                Color = OxyColors.Red,
                LineStyle = LineStyle.Solid
            };
            _myPlotModel.Annotations.Add(lineHumiMinAnnotation);

            //添加两条曲线
            var series = new LineSeries()
            {
                Color = OxyColors.Green,
                StrokeThickness = 2,
                MarkerSize = 3,
                MarkerStroke = OxyColors.DarkGreen,
                MarkerType = MarkerType.Diamond,
                Title = "Temp",
            };
            _myPlotModel.Series.Add(series);

            series = new LineSeries()
            {
                Color = OxyColors.Blue,
                StrokeThickness = 2,
                MarkerSize = 3,
                MarkerStroke = OxyColors.BlueViolet,
                MarkerType = MarkerType.Star,
                Title = "Humi",
            };
            _myPlotModel.Series.Add(series);

            var rand = new Random();
            var dateOrigin = DateTime.Now.AddHours(-8);
            for (int i = 0; i < 200; i++)
            {
                var date = dateOrigin.AddMinutes(i * 10);
                var lineSer = _myPlotModel.Series[0] as LineSeries;
                lineSer.Points.Add(new DataPoint(DateTimeAxis.ToDouble(date), rand.Next(100, 300) / 10.0));
                if (lineSer.Points.Count > 100)
                {
                    lineSer.Points.RemoveAt(0);
                }

                lineSer = _myPlotModel.Series[1] as LineSeries;
                lineSer.Points.Add(new DataPoint(DateTimeAxis.ToDouble(date), rand.Next(350, 750) / 10.0));
                if (lineSer.Points.Count > 100)
                {
                    lineSer.Points.RemoveAt(0);
                }

            }

            plotView.Model = _myPlotModel;
        }

        void Test1()
        {
            int samples = 5;
            int distance = 179;
            int rectPad = 34;
            var model = new PlotModel()
            {
                Background = OxyColors.White,
                Title = "Schedule 1",
                TitleFont = "Arial",
                TitlePadding = 16
            };
            model.Axes.Add(new LinearAxis
            {
                Title = "Samples",
                AxisTitleDistance = 16,
                TitleFontSize = 16,
                Position = AxisPosition.Left,
                AbsoluteMaximum = samples * distance,
                Maximum = samples * distance,
                AbsoluteMinimum = 0,
                MinimumRange = 1,
                MajorStep = distance,
                MinorStep = distance,
                LabelFormatter = y =>
                {
                    int Nr = samples - (int)y / distance + 1;
                    return Nr > samples ? "" : Nr.ToString();
                }
            });
            model.Axes.Add(new LinearAxis
            {
                Title = "Runtime",
                AxisTitleDistance = 16,
                TitleFontSize = 16,
                Minimum = 0,
                Maximum = 1500,
                AbsoluteMinimum = 0,
                MinimumRange = 5,
                Position = AxisPosition.Bottom,
                MajorTickSize = 15,
                MinorTickSize = 5,
                MajorGridlineStyle = LineStyle.Solid,
                MajorGridlineColor = OxyColors.LightGray,
                LabelFormatter = time =>
                {
                    int hour = 0; int min = 0; int sec = 0;
                    sec = (int)time;
                    if (sec >= 60)
                    {
                        min = ((int)sec) / 60;
                        sec = sec % 60;
                    }
                    if (min >= 60)
                    {
                        hour = min / 60;
                        min = min % 60;
                    }
                    return string.Format("{0:D2}:{1:D2}:{2:D2}", hour, min, sec);
                }
            });

            var lineAnnotation = new LineAnnotation()
            {
                Type = LineAnnotationType.Vertical,
                X = 100,
                Text = "当前时间",
                Color = OxyColors.Red,
                LineStyle = LineStyle.Solid
            };
            model.Annotations.Add(lineAnnotation);

            var rect1 = new RectangleAnnotation()
            {
                MinimumX = 0,
                MaximumX = 5.9,
                MinimumY = distance * (samples - 1) + rectPad,
                MaximumY = distance * (samples) - rectPad,
                Fill = OxyColor.FromRgb(155, 188, 243),//sky blue
                Stroke = OxyColors.Transparent
            };
            model.Annotations.Add(rect1);

            var rect2 = new RectangleAnnotation()
            {
                MinimumX = 6,
                MaximumX = 180,
                MinimumY = distance * (samples - 1) + 1.5 * rectPad,
                MaximumY = distance * (samples) - 1.5 * rectPad,
                Fill = OxyColor.FromRgb(191, 2, 1),//red
                Stroke = OxyColors.Transparent
            };

            var rect3 = new RectangleAnnotation()
            {
                MinimumX = 180.1,
                MaximumX = 240,
                MinimumY = distance * (samples - 1) + rectPad,
                MaximumY = distance * (samples) - rectPad,
                Fill = OxyColor.FromRgb(254, 152, 14),
                Stroke = OxyColors.Transparent//organge
            };

            var rect4 = new RectangleAnnotation()
            {
                MinimumX = 180.1,
                MaximumX = 480,
                MinimumY = distance * (samples - 1) + 1.5 * rectPad,
                MaximumY = distance * (samples) - 1.5 * rectPad,
                Fill = OxyColor.FromRgb(254, 191, 20),//yellow
                Stroke = OxyColors.Transparent
            };
            model.Annotations.Add(rect3);
            model.Annotations.Add(rect2);
            model.Annotations.Add(rect4);
            var myController = new PlotController();
            myController.UnbindMouseDown(OxyMouseButton.Right);
            myController.BindMouseDown(OxyMouseButton.Left, PlotCommands.PanAt);
            plotView.Controller = myController;
            //plot1.Model = Example.RectangleAnnotation();
            plotView.Model = model;
            //plotView.Model.MouseDown += Model_MouseDown;
        }
        private void Model_MouseDown(object sender, OxyMouseDownEventArgs e)
        {

            double x = e.Position.X;
            double y = e.Position.Y;
            DataPoint p = OxyPlot.Axes.Axis.InverseTransform(e.Position, plotView.Model.DefaultXAxis, plotView.Model.DefaultYAxis);

            //this.Text = ($"X is {x} and Y is {y} and Inverse is {p.ToString()}");
        }
    }
}
