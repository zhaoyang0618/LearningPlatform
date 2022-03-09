using SkiaSharp;
using SkiaSharp.Views.Desktop;
using System;
using System.Collections.Generic;
using System.Text;
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
	}
}
