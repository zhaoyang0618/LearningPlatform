using AIStudioClient.Modules.ImageAnnotation.ViewModels;
using ArtificialIntelligenceStudioClient.Core;
using ArtificialIntelligenceStudioClient.Core.ViewModels;
using Microsoft.Extensions.Logging;
using Prism.Commands;
using Prism.Events;
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

namespace AIStudioClient.Modules.ImageAnnotation.Views
{
    /// <summary>
    /// Interaction logic for AnnotationWnd.xaml
    /// </summary>
    public partial class AnnotationWnd : UserControl
    {
        IEventAggregator _eventAggregator;
        ImageAnnotationModule _module;
        public AnnotationWnd(ILogger logger, IEventAggregator eventAggregator, ImageAnnotationModule module, AnnotationWndViewModel vm)
        {
            InitializeComponent();
            _logger = logger;
            _eventAggregator = eventAggregator;
            _module = module;
            _viewModel = vm;
            CROSSLINESHOW = false;
            SelectRect = false;
            // 初始化十字交叉线
            lineX = new LineGeometry();
            lineY = new LineGeometry();
            linegroup = new GeometryGroup();
            rect_rois = new List<double[]>();
            rect_Flags = new List<bool>();
            crossline.Data = linegroup;
            linegroup.Children.Add(lineX);
            linegroup.Children.Add(lineY);
            // 工作区添加缩放事件响应
            this.SizeChanged += new System.Windows.SizeChangedEventHandler(PageLightResized);
            // 工作区添加滚轮事件冒泡
            workspace.PreviewMouseWheel += (sender, e) => {
                var eventArg = new MouseWheelEventArgs(e.MouseDevice, e.Timestamp, e.Delta);
                eventArg.RoutedEvent = UIElement.MouseWheelEvent;
                eventArg.Source = sender;
                workspace.RaiseEvent(eventArg);
            };

            InitUI();
            BindEvents();
        }
        // 定义像素分辨率比，屏幕尺寸
        private double widthRatio, heightRatio;
        //private double[] screenSize;
        // 定义十字交叉线
        private LineGeometry lineX, lineY;
        private GeometryGroup linegroup;
        private bool CROSSLINESHOW;
        private bool SelectRect;
        private List<double[]> rect_rois;
        private List<bool> rect_Flags;
        /* ------------- 导航栏按钮点击响应 --------------*/
        // 矩形标注按钮
        private void LabelRectangle(object sender, RoutedEventArgs e)
        {
        }
        // 取消选中按钮
        private const int ROIStart = 7;  // 定义ROI开始索引
        private void SelectCancel(object sender, RoutedEventArgs e)
        {
        }
        /* ------------labelbox框事件响应-------------- */
        // 键盘响应
        private void LabelboxKeyResponse(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Delete)
            {
                LabelboxItemDelete();
                PointClose();
            }
        }
        // 响应Delete按键
        private void LabelboxItemDelete()
        {
            int index = labelbox.SelectedIndex;
            if (index != -1)
            {
                workspace.Children.RemoveAt(index + ROIStart);
                rect_rois.RemoveAt(index);
                rect_Flags.RemoveAt(index);
                labelbox.Items.RemoveAt(index);
                labelbox.SelectedIndex = -1;
                labelbox.Tag = null;
            }
        }
        // 选中标注响应事件
        private void SelectChange(object sender, SelectionChangedEventArgs e)
        {
            int index = labelbox.SelectedIndex;
            if (index == -1) return;
            FlashPoint();
            PointShow();
            Rectangle rectangle = workspace.Children[index + ROIStart] as Rectangle;
            rectangle.Fill = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#5588ffcc"));
            rectangle.Tag = 1;
            if (labelbox.Tag != null)
            {
                int lastindex = (int)labelbox.Tag;
                Rectangle lastrectangle = workspace.Children[lastindex + ROIStart] as Rectangle;
                lastrectangle.Fill = null;
                lastrectangle.Tag = 0;
            }
            labelbox.Tag = index;
        }
        /* ------------工作区事件响应-------------- */
        // 工作区尺寸变化事件响应
        private void PageLightResized(object sender, System.EventArgs e)
        {
            // 窗口及图像的ActualWidth均发生改变
            // 窗口实际尺寸，图像比例下实际尺寸
            var group_image = workspace.FindResource("Imageview") as TransformGroup;
            var scale_image = group_image.Children[0] as ScaleTransform;
            var move_image = group_image.Children[1] as TranslateTransform;
            move_image.X = 0;
            move_image.Y = 0;
            scale_image.ScaleX = 1;
            scale_image.ScaleY = 1;
            DoRoisWheelZoom();
            DoRoisResize();
            FlashPoint();
        }
        // img为可视区域，image为图像
        private bool lMouseDown;
        private bool rMouseDown;
        private Point position;
        private Point imageposition;
        // 鼠标左键按下响应
        private void ImgMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (!SelectRect) { return; }
            if (!image.IsMouseOver && !crossline.IsMouseOver) { return; }
            image.CaptureMouse();
            lMouseDown = true;
            CROSSLINESHOW = false;
            ClearCrossLine();
            position = e.GetPosition(imgcontrol);
            imageposition = e.GetPosition(image);
        }
        // 鼠标左键松开响应
        int rectIndex = 0;
        private void ImgMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (!SelectRect) { return; }
            if (!image.IsMouseOver && !crossline.IsMouseOver) { return; }
            if (!lMouseDown) return;
            Point mouse = e.GetPosition(image);
            image.ReleaseMouseCapture();
            lMouseDown = false;
            CROSSLINESHOW = true;
            string label = string.Format("测试{0:D3}", rectIndex++);// PopupLabel();
            double[] rect_roi = CreateRoi(imageposition, mouse);
            Rectangle rectangle = CreateRectangle(rect_roi);
            if (label != "" && rectangle != null)
            {
                workspace.Children.Add(rectangle); // 矩形控件添加进入工作区
                ListBoxItem item = new ListBoxItem(); // 标签添加到标签盒子
                item.Content = label;
                labelbox.Items.Add(item);
                rect_Flags.Add(false); // ROI标志符添加到标志符列表
                rect_rois.Add(rect_roi); // 标注数据添加到标注列表
            }
            // 清除标注矩形框
            rectanglebox.Width = 0;
            rectanglebox.Height = 0;
        }
        // 鼠标右键按下响应
        private void ImgMouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (!image.IsMouseOver && !crossline.IsMouseOver) { return; }
            image.CaptureMouse();
            rMouseDown = true;
            position = e.GetPosition(imgcontrol);
        }
        // 鼠标右键松开响应
        private void ImgMouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (!image.IsMouseOver && !crossline.IsMouseOver) { return; }
            image.ReleaseMouseCapture();
            rMouseDown = false;
        }
        // 鼠标移动响应
        private void ImgMouseMove(object sender, MouseEventArgs e)
        {
            if (!image.IsMouseOver && !crossline.IsMouseOver)
            {
                ClearCrossLine();
                return;
            }
            if (rMouseDown)
            {
                double[] delta = DoImageMove(imgcontrol, e);
                DoRoisMove(e, delta);
                FlashPoint();
            }
            Point mouse = e.GetPosition(imgcontrol);
            if (lMouseDown)
            {
                DoDrawRectangle(position, mouse);
            }
            FlashCrossLine(mouse);
        }
        // 工作区滚轮事件响应
        private void ImgMouseWheel(object sender, MouseWheelEventArgs e)
        {
            if (!image.IsMouseOver && !crossline.IsMouseOver) { return; }
            var point = e.GetPosition(image);
            var group_image = workspace.FindResource("Imageview") as TransformGroup;
            var delta = e.Delta * 0.002;
            DoImageWheelZoom(group_image, point, delta);
            DoRoisWheelZoom();
            FlashPoint();
            Point mouse = e.GetPosition(imgcontrol);
            FlashCrossLine(mouse);
        }
        // 鼠标离开工作区响应
        private void ImgMouseLeave(object sender, MouseEventArgs e)
        {
            ClearCrossLine();
        }
        /* -------------顶点事件响应--------------- */
        private bool pointMouseDown;
        private Point pposition = new Point();
        private Point pimageposition = new Point();
        // 顶点鼠标左键按下响应
        private void PointMouseDown(object sender, MouseButtonEventArgs e)
        {
            var group_image = workspace.FindResource("Imageview") as TransformGroup;
            var scale_image = group_image.Children[0] as ScaleTransform;
            var move_image = group_image.Children[1] as TranslateTransform;
            pointMouseDown = true;
            Ellipse circle = sender as Ellipse;
            circle.CaptureMouse();
            string tag = (string)circle.Tag;
            int index = labelbox.SelectedIndex;
            double[] rect_roi = rect_rois[index];
            double x = rect_roi[0] * image.ActualWidth * scale_image.ScaleX + move_image.X
                       + (imgcontrol.ActualWidth - image.ActualWidth) / 2;
            double y = rect_roi[1] * image.ActualHeight * scale_image.ScaleY + move_image.Y
                       + (imgcontrol.ActualHeight - image.ActualHeight) / 2;
            double w = rect_roi[2] * image.ActualWidth * scale_image.ScaleX;
            double h = rect_roi[3] * image.ActualHeight * scale_image.ScaleY;
            double ix = rect_roi[0] * image.ActualWidth;
            double iy = rect_roi[1] * image.ActualHeight;
            double iw = rect_roi[2] * image.ActualWidth;
            double ih = rect_roi[3] * image.ActualHeight;
            switch (tag)
            {
                case "1":
                    pposition = new Point(x + w, y + h);
                    pimageposition = new Point(ix + iw, iy + ih);
                    break;
                case "2":
                    pposition = new Point(x, y + h);
                    pimageposition = new Point(ix, iy + ih);
                    break;
                case "3":
                    pposition = new Point(x + w, y);
                    pimageposition = new Point(ix + iw, iy);
                    break;
                case "4":
                    pposition = new Point(x, y);
                    pimageposition = new Point(ix, iy);
                    break;
            }
            Point mouse = e.GetPosition(imgcontrol);
            Rect rect = new Rect(pposition, mouse);
            Rectangle rectangle = workspace.Children[ROIStart + index] as Rectangle;
            rectangle.Width = 0;
            rectangle.Height = 0;
            rectanglebox.Margin = new Thickness(rect.Left * 2 + rect.Width - imgcontrol.ActualWidth,
                                                rect.Top * 2 + rect.Height - imgcontrol.ActualHeight,
                                                0, 0);
            rectanglebox.Width = rect.Width;
            rectanglebox.Height = rect.Height;
            PointClose();
        }
        // 顶点鼠标移动响应
        private void PointMouseMove(object sender, MouseEventArgs e)
        {
            Point mouse = e.GetPosition(imgcontrol);
            if (pointMouseDown)
            {
                DoDrawRectangle(pposition, mouse);
            }
        }
        // 顶点鼠标左键松开响应
        private void PointMouseUp(object sender, MouseButtonEventArgs e)
        {
            pointMouseDown = false;
            Ellipse circle = sender as Ellipse;
            circle.ReleaseMouseCapture();
            rectanglebox.Width = 0;
            rectanglebox.Height = 0;
            // 创建标注roi，添加进标注列表
            int index = labelbox.SelectedIndex;
            Point mouse = e.GetPosition(image);
            double[] rect_roi = CreateRoi(pimageposition, mouse);
            if (image.ActualWidth * image.ActualHeight * rect_roi[2] * rect_roi[3] > 9)
            {
                rect_rois[index] = rect_roi;
            }
            FlashRectangle(index);
            FlashPoint();
            PointShow();
        }
        /* -------------矩形事件响应 ---------------*/
        private bool rectangleMouseDown;
        // 鼠标左键按下响应
        private void SelectRectangle(object sender, MouseEventArgs e)
        {
            Rectangle rectangle = sender as Rectangle;
            if (1 == (int)rectangle.Tag)
            {
                rectangleMouseDown = true;
                imageposition = e.GetPosition(image);
                rectangle.CaptureMouse();
            }
        }
        // 鼠标移动响应
        private void MoveRectangle(object sender, MouseEventArgs e)
        {
            Rectangle rectangle = sender as Rectangle;
            if (rectangleMouseDown && (int)rectangle.Tag == 1)
            {
                int index = labelbox.SelectedIndex;
                Point mouse = e.GetPosition(image);
                double deltax = (mouse.X - imageposition.X) / image.ActualWidth;
                double deltay = (mouse.Y - imageposition.Y) / image.ActualHeight;
                double movex = rect_rois[index][0];
                double movey = rect_rois[index][1];
                rect_rois[index][0] = relu(rect_rois[index][0] + deltax, 0, 1 - rect_rois[index][2]);
                rect_rois[index][1] = relu(rect_rois[index][1] + deltay, 0, 1 - rect_rois[index][3]);
                movex = rect_rois[index][0] - movex;
                movey = rect_rois[index][1] - movey;
                var group_rect = rectangle.RenderTransform as TransformGroup;
                var scale_rect = group_rect.Children[0] as ScaleTransform;
                var move_rect = group_rect.Children[1] as TranslateTransform;
                move_rect.X += movex * image.ActualWidth * scale_rect.ScaleX;
                move_rect.Y += movey * image.ActualHeight * scale_rect.ScaleY;
                imageposition = mouse;
                FlashPoint();
            }
        }
        // 鼠标左键松开响应
        private void FinishRectangle(object sender, MouseEventArgs e)
        {
            rectangleMouseDown = false;
            Rectangle rectangle = sender as Rectangle;
            rectangle.ReleaseMouseCapture();
        }
        /* -------------  */
        // 打标签函数
        //private string PopupLabel()
        //{
        //    var labelpopupwindow = new LabelPopupWindow();
        //    if (screenSize == null)
        //        InitScreenRadio();
        //    double[] win = { labelpopupwindow.Width, labelpopupwindow.Height };
        //    double mx = PointToScreen(Mouse.GetPosition(null)).X; //得到鼠标横坐标
        //    double my = PointToScreen(Mouse.GetPosition(null)).Y; //得到鼠标纵坐标
        //    double w = rectanglebox.Width;
        //    double h = rectanglebox.Height;
        //    double[] rect = { mx - w, my - h, w, h };
        //    double[] locate = CalculateWindowLocation(screenSize, rect, win);
        //    labelpopupwindow.Left = locate[0];
        //    labelpopupwindow.Top = locate[1];
        //    bool? result = labelpopupwindow.ShowDialog();
        //    if (result == true)
        //    {
        //        return labelpopupwindow.label.Text;
        //    }
        //    else
        //    {
        //        return "";
        //    }
        //}
        // 顶点显示函数
        private void PointShow()
        {
            c1.Width = 9;
            c2.Width = 9;
            c3.Width = 9;
            c4.Width = 9;
        }
        // 顶点关闭显示函数
        private void PointClose()
        {
            c1.Width = 0;
            c2.Width = 0;
            c3.Width = 0;
            c4.Width = 0;
        }
        // 顶点位置刷新函数
        private void FlashPoint()
        {
            if (labelbox.SelectedIndex == -1) return;
            var group_image = workspace.FindResource("Imageview") as TransformGroup;
            var scale_image = group_image.Children[0] as ScaleTransform;
            var move_image = group_image.Children[1] as TranslateTransform;
            int index = labelbox.SelectedIndex;
            double x = rect_rois[index][0];
            double y = rect_rois[index][1];
            double w = rect_rois[index][2];
            double h = rect_rois[index][3];
            double x1 = 2 * x * image.ActualWidth * scale_image.ScaleX + 2 * move_image.X - image.ActualWidth;
            double y1 = 2 * y * image.ActualHeight * scale_image.ScaleY + 2 * move_image.Y - image.ActualHeight;
            double x2 = 2 * w * image.ActualWidth * scale_image.ScaleX + x1;
            double y2 = 2 * h * image.ActualHeight * scale_image.ScaleY + y1;
            c1.Margin = new Thickness(x1, y1, 0, 0);
            c2.Margin = new Thickness(x2, y1, 0, 0);
            c3.Margin = new Thickness(x1, y2, 0, 0);
            c4.Margin = new Thickness(x2, y2, 0, 0);
        }
        // 图像平移函数
        private double[] DoImageMove(ContentControl img, MouseEventArgs e)
        {
            if (e.RightButton != MouseButtonState.Pressed) { return new double[2] { 0, 0 }; }
            var group_image = workspace.FindResource("Imageview") as TransformGroup;
            double scale_image = (group_image.Children[0] as ScaleTransform).ScaleX;
            var move_image = group_image.Children[1] as TranslateTransform;
            double movex1 = move_image.X;
            double movey1 = move_image.Y;
            var mouseXY = e.GetPosition(img);
            move_image.X += mouseXY.X - position.X;
            move_image.Y += mouseXY.Y - position.Y;
            position = mouseXY;
            // W+w > 2*move_x > -((2*scale-1)*w + W)  水平平移限制条件
            // H+h > 2*move_y > -((2*scale-1)*h + H)  垂直平移限制条件
            double W = img.ActualWidth;
            double H = img.ActualHeight;
            double w = image.ActualWidth;
            double h = image.ActualHeight;
            if (move_image.X * 2 > W + w - 20)
                move_image.X = (W + w - 20) / 2;
            if (-move_image.X * 2 > (2 * scale_image - 1) * w + W - 20)
                move_image.X = 10 - (scale_image - 0.5) * w - W / 2;
            if (move_image.Y * 2 > H + h - 20)
                move_image.Y = (H + h - 20) / 2;
            if (-move_image.Y * 2 > (2 * scale_image - 1) * h + H - 20)
                move_image.Y = 10 - (scale_image - 0.5) * h - H / 2;
            double movex2 = move_image.X;
            double movey2 = move_image.Y;
            double[] delta = { movex2 - movex1, movey2 - movey1 };
            return delta;
        }
        // ROI跟随图像平移函数
        private void DoRoisMove(MouseEventArgs e, double[] delta)
        {
            if (e.RightButton != MouseButtonState.Pressed) { return; }
            for (int i = 0; i < rect_rois.Count; i++)
            {
                var group_rect = workspace.Children[i + ROIStart].RenderTransform as TransformGroup;
                var move_rect = group_rect.Children[1] as TranslateTransform;
                move_rect.X += delta[0];
                move_rect.Y += delta[1];
            }
        }
        // 鼠标绘制矩形函数
        private void DoDrawRectangle(Point position, Point mouse)
        {
            Rect rect = new Rect(position, mouse);
            double l_margin, t_margin;
            if (rect.Width < imgcontrol.ActualWidth)
            {
                l_margin = 2 * rect.Left + rect.Width - imgcontrol.ActualWidth;
            }
            else
            {
                if (rect.Left > 0)
                    l_margin = 2 * rect.Left;
                else
                    l_margin = 2 * (rect.Left + rect.Width - imgcontrol.ActualWidth);
            }
            if (rect.Height < imgcontrol.ActualHeight)
            {
                t_margin = 2 * rect.Top + rect.Height - imgcontrol.ActualHeight;
            }
            else
            {
                if (rect.Top > 0)
                    t_margin = 2 * rect.Top;
                else
                    t_margin = 2 * (rect.Top + rect.Height - imgcontrol.ActualHeight);
            }
            rectanglebox.Margin = new Thickness(l_margin / 2, t_margin / 2,
                                                -l_margin / 2, -t_margin / 2);
            rectanglebox.Width = rect.Width;
            rectanglebox.Height = rect.Height;
        }
        // 根据图像相对坐标创建ROI数值函数
        private double[] CreateRoi(Point imageposition, Point mouse)
        {
            Rect rect = new Rect(imageposition, mouse);
            double roi_x1 = relu(rect.Left / image.ActualWidth, 0, 1);
            double roi_y1 = relu(rect.Top / image.ActualHeight, 0, 1);
            double roi_x2 = relu((rect.Width + rect.Left) / image.ActualWidth, 0, 1);
            double roi_y2 = relu((rect.Height + rect.Top) / image.ActualHeight, 0, 1);
            double roi_w = roi_x2 - roi_x1;
            double roi_h = roi_y2 - roi_y1;
            double[] rect_roi = { roi_x1, roi_y1, roi_w, roi_h };
            return rect_roi;
        }
        // 创建矩形函数
        private Rectangle CreateRectangle(double[] rect_roi)
        {
            // 创建矩形控件
            Rectangle rectangle = new Rectangle();
            rectangle.Width = rect_roi[2] * image.ActualWidth;
            rectangle.Height = rect_roi[3] * image.ActualHeight;
            if (rectangle.Width * rectangle.Height < 9) return null;
            rectangle.Margin = new Thickness(0, 0,
                                             image.ActualWidth - rectangle.Width,
                                             image.ActualHeight - rectangle.Height);
            rectangle.Stroke = new SolidColorBrush(Colors.Red);
            rectangle.MouseLeftButtonDown += new MouseButtonEventHandler(SelectRectangle);
            rectangle.MouseMove += new MouseEventHandler(MoveRectangle);
            rectangle.MouseLeftButtonUp += new MouseButtonEventHandler(FinishRectangle);
            rectangle.MouseLeave += new MouseEventHandler(FinishRectangle);
            TransformGroup group_rect = new TransformGroup();
            ScaleTransform scale_rect = new ScaleTransform();
            TranslateTransform move_rect = new TranslateTransform();
            var group_image = workspace.FindResource("Imageview") as TransformGroup;
            scale_rect = group_image.Children[0] as ScaleTransform;
            TranslateTransform move_image = group_image.Children[1] as TranslateTransform;
            move_rect.X = rect_roi[0] * image.ActualWidth * scale_rect.ScaleX + move_image.X;
            move_rect.Y = rect_roi[1] * image.ActualHeight * scale_rect.ScaleY + move_image.Y;
            group_rect.Children.Add(scale_rect);
            group_rect.Children.Add(move_rect);
            rectangle.RenderTransform = group_rect;
            rectangle.Cursor = Cursors.Hand;
            rectangle.Tag = 0;
            return rectangle;
        }
        // 图像缩放函数
        private void DoImageWheelZoom(TransformGroup group, Point point, double delta)
        {
            var scale = group.Children[0] as ScaleTransform;
            if (scale.ScaleX + delta < 0.1) return;
            scale.ScaleX += delta;
            scale.ScaleY += delta;
            var move = group.Children[1] as TranslateTransform;
            move.X -= point.X * delta;
            move.Y -= point.Y * delta;
        }
        // ROI缩放函数(ActualW,ActualH不变模式)
        private void DoRoisWheelZoom()
        {
            var group_image = workspace.FindResource("Imageview") as TransformGroup;
            var scale_image = group_image.Children[0] as ScaleTransform;
            var move_image = group_image.Children[1] as TranslateTransform;
            int i = ROIStart;
            foreach (double[] roi in rect_rois)
            {
                var group_rect = workspace.Children[i].RenderTransform as TransformGroup;
                var scale_rect = group_rect.Children[0] as ScaleTransform;
                var move_rect = group_rect.Children[1] as TranslateTransform;
                scale_rect = scale_image;
                move_rect.X = roi[0] * image.ActualWidth * scale_image.ScaleX + move_image.X;
                move_rect.Y = roi[1] * image.ActualHeight * scale_image.ScaleY + move_image.Y;
                i++;
            }
        }
        // ROI缩放函数(ActualW,ActualH改变模式)
        private void DoRoisResize()
        {
            int i = ROIStart;
            foreach (double[] roi in rect_rois)
            {
                Rectangle rectangle = workspace.Children[i] as Rectangle;
                rectangle.Width = roi[2] * image.ActualWidth;
                rectangle.Height = roi[3] * image.ActualHeight;
                rectangle.Margin = new Thickness(0, 0,
                                                 image.ActualWidth - rectangle.Width,
                                                 image.ActualHeight - rectangle.Height);
                var group_rect = rectangle.RenderTransform as TransformGroup;
                var scale_rect = group_rect.Children[0] as ScaleTransform;
                var move_rect = group_rect.Children[1] as TranslateTransform;
                scale_rect = new ScaleTransform(1, 1);
                move_rect.X = roi[0] * image.ActualWidth;
                move_rect.Y = roi[1] * image.ActualHeight;
                i++;
            }
        }
        // 刷新指定矩形框尺寸位置函数
        private void FlashRectangle(int index)
        {
            double[] roi = rect_rois[index];
            Rectangle rectangle = workspace.Children[ROIStart + index] as Rectangle;
            rectangle.Width = roi[2] * image.ActualWidth;
            rectangle.Height = roi[3] * image.ActualHeight;
            rectangle.Margin = new Thickness(0, 0,
                                             image.ActualWidth - rectangle.Width,
                                             image.ActualHeight - rectangle.Height);
            var group_rect = rectangle.RenderTransform as TransformGroup;
            var scale_rect = group_rect.Children[0] as ScaleTransform;
            var move_rect = group_rect.Children[1] as TranslateTransform;
            var group_image = workspace.FindResource("Imageview") as TransformGroup;
            scale_rect = group_image.Children[0] as ScaleTransform;
            var move_image = group_image.Children[1] as TranslateTransform;
            move_rect.X = roi[0] * image.ActualWidth * scale_rect.ScaleX + move_image.X;
            move_rect.Y = roi[1] * image.ActualHeight * scale_rect.ScaleY + move_image.Y;
        }
        // 清除十字交叉线
        private void ClearCrossLine()
        {
            lineX.StartPoint = new Point();
            lineX.EndPoint = new Point();
            lineY.StartPoint = new Point();
            lineY.EndPoint = new Point();
        }
        // 工作区坐标系刷新十字交叉线位置函数
        private void FlashCrossLine(Point mouse)
        {
            if (!CROSSLINESHOW) { return; }
            var group = workspace.FindResource("Imageview") as TransformGroup;
            double scale = (group.Children[0] as ScaleTransform).ScaleX;
            var move = group.Children[1] as TranslateTransform;
            double W = imgcontrol.ActualWidth;
            double H = imgcontrol.ActualHeight;
            double w = image.ActualWidth;
            double h = image.ActualHeight;
            lineX.StartPoint = new Point(mouse.X, Math.Max(0, move.Y + (H - h) / 2));
            lineX.EndPoint = new Point(mouse.X, Math.Min(H, move.Y + h * scale + (H - h) / 2));
            lineY.StartPoint = new Point(Math.Max(0, move.X + (W - w) / 2), mouse.Y);
            lineY.EndPoint = new Point(Math.Min(W, move.X + w * scale + (W - w) / 2), mouse.Y);
        }
        // 整流函数
        public double relu(double input, double Lborder, double Rborder)
        {
            if (input < Lborder) return Lborder;
            else if (input > Rborder) return Rborder;
            else return input;
        }

        // 初始化屏幕、像素比
        private void InitScreenRadio()
        {
            PresentationSource source = PresentationSource.FromVisual(this);
            if(source == null) return;
            Matrix m = source.CompositionTarget.TransformFromDevice;
            widthRatio = m.M11;
            heightRatio = m.M22;
            double w = SystemParameters.PrimaryScreenWidth; //得到屏幕整体宽度
            double h = SystemParameters.PrimaryScreenHeight; //得到屏幕整体高度
            //screenSize = new double[] { w, h };
        }
        // 返回环绕窗口位置
        private double[] CalculateWindowLocation(double[] screen, double[] rect, double[] win)
        {
            double[] locate = { 0, 0 };
            if (rect[2] * widthRatio > win[0] + 40)
                locate[0] = rect[0] * widthRatio + 20;
            else if (rect[0] * widthRatio > win[0] + 40)
                locate[0] = rect[0] * widthRatio - win[0] - 20;
            else
                locate[0] = (rect[0] + rect[2]) * widthRatio + 20;
            if (rect[3] * heightRatio > win[1] + 40)
                locate[1] = rect[1] * heightRatio + 20;
            else if (rect[1] * heightRatio > win[1] + 40)
                locate[1] = rect[1] * heightRatio - win[1] - 20;
            else
                locate[1] = (rect[1] + rect[3]) * heightRatio + 20;
            return locate;
        }

        #region 事件处理
        private void OnWndLoaded(object sender, RoutedEventArgs e)
        {
            var args = new ModuleLoadedEventArgs()
            {
                Title = _module.Title,
                Key = typeof(ImageAnnotationModule).Name,
            };
            args.Buttons.AddRange(_buttons);
            _eventAggregator.GetEvent<ModuleLoadedEvent>().Publish(args);
            //
            InitScreenRadio();

        }

        private void OnWndUnloaded(object sender, RoutedEventArgs e)
        {
            var args = new ModuleUnloadEventArgs()
            {
                Key = typeof(ImageAnnotationModule).Name,
            };
            _eventAggregator.GetEvent<ModuleUnloadEvent>().Publish(args);
        }

        private void OnCommandSelectImageFolder()
        {
            //选择文件夹
            //打开图像文件
            //_logger.LogTrace("OnCommandSelectImageFolder");
            //var dlg = new FolderBrowserDialog();
            var dlg = new Ookii.Dialogs.Wpf.VistaFolderBrowserDialog();
            var ret = dlg.ShowDialog();
            if (ret == true)
            {
                //遍历图像文件
                var folder = dlg.SelectedPath;
                if (_viewModel != null)
                {
                    //获取所有的图像文件
                    _viewModel.ImageFolder = folder;
                }
            }
        }

        private void OnCommandModeRect()
        {
            if(_buttonModeRect != null)
            {
                _buttonModeRect.IsChecked = true;
            }
            if(_buttonModeSelect != null)
            {
                _buttonModeSelect.IsChecked = false;
            }

            //
            SelectRect = true;
            CROSSLINESHOW = true;
            //button_rectangle.Foreground = new SolidColorBrush(Colors.LightYellow);
            image.Cursor = Cursors.Cross;
            crossline.Cursor = Cursors.Cross;

            //SelectRect = !SelectRect;
            //if (SelectRect)
            //{
            //    CROSSLINESHOW = true;
            //    button_rectangle.Foreground = new SolidColorBrush(Colors.LightYellow);
            //    image.Cursor = Cursors.Cross;
            //    crossline.Cursor = Cursors.Cross;
            //}
            //else
            //{
            //    CROSSLINESHOW = false;
            //    button_rectangle.Foreground = new SolidColorBrush(Colors.Black);
            //    image.Cursor = Cursors.Arrow;
            //    crossline.Cursor = Cursors.Arrow;
            //}

        }

        private void OnCommandModeSelect()
        {
            if (_buttonModeRect != null)
            {
                _buttonModeRect.IsChecked = false;
            }
            if (_buttonModeSelect != null)
            {
                _buttonModeSelect.IsChecked = true;
            }
            SelectRect = false;
            CROSSLINESHOW = false;
            //button_rectangle.Foreground = new SolidColorBrush(Colors.Black);
            image.Cursor = Cursors.Arrow;
            crossline.Cursor = Cursors.Arrow;
        }

        private void OnCommandClear()
        {
            labelbox.SelectedIndex = -1;
            if (labelbox.Tag != null)
            {
                int index = (int)labelbox.Tag;
                Rectangle rectangle = workspace.Children[index + ROIStart] as Rectangle;
                if (rectangle == null) return;
                rectangle.Fill = null;
                rectangle.Tag = 0;
                labelbox.Tag = null;
                PointClose();
            }
        }

        #endregion

        #region 辅助函数
        ILogger _logger;
        RibbonButtonViewModel? _buttonSelectImageFolder = null;
        RibbonButtonViewModel? _buttonModeRect = null;
        RibbonButtonViewModel? _buttonModeSelect = null;
        RibbonButtonViewModel? _buttonClear = null;
        DelegateCommand? _delegateCommandSelectImageFolder;
        DelegateCommand? _delegateCommandModeRect;      //用于标注
        DelegateCommand? _delegateCommandModeSelect;    //选择
        DelegateCommand? _delegateCommandClear;         //清空
        List<RibbonButtonViewModel> _buttons = new List<RibbonButtonViewModel>();
        AnnotationWndViewModel? _viewModel = null;
        void InitUI()
        {
            this.DataContext = _viewModel;
            _delegateCommandSelectImageFolder = new DelegateCommand(OnCommandSelectImageFolder);
            _delegateCommandModeRect = new DelegateCommand(OnCommandModeRect);
            _delegateCommandModeSelect = new DelegateCommand(OnCommandModeSelect);
            _delegateCommandClear = new DelegateCommand(OnCommandClear);
            _buttonSelectImageFolder = new RibbonButtonViewModel()
            {
                Title = "选择文件夹",
                Description = "选择图片所在的文件夹",
                Command = _delegateCommandSelectImageFolder,
            };
            _buttons.Add(_buttonSelectImageFolder);
            _buttonModeSelect = new RibbonButtonViewModel()
            {
                Title = "选择模式",
                Description = "用于选择已经标注的对象",
                ButtonType = 1,
                IsChecked = true,
                Command = _delegateCommandModeSelect,
            };
            _buttons.Add(_buttonModeSelect);
            _buttonModeRect = new RibbonButtonViewModel()
            {
                Title = "标注模式",
                Description = "标注模式，可以进行标注",
                ButtonType = 1,
                IsChecked = false,
                Command = _delegateCommandModeRect,
            };
            _buttons.Add(_buttonModeRect);
            _buttonClear = new RibbonButtonViewModel()
            {
                Title = "清空",
                Description = "清除图片中所有的标注",
                Command = _delegateCommandClear,
            };
            _buttons.Add(_buttonClear);
        }

        void BindEvents()
        {
            this.Loaded += OnWndLoaded;
            this.Unloaded += OnWndUnloaded;
        }

        #endregion
    }
}
