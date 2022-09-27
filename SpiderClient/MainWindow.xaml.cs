using AngleSharp;
using AngleSharp.Dom;
using AngleSharp.Scripting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
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

namespace SpiderClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 获取网页内容
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void OnButtonGetUrlClick(object sender, RoutedEventArgs e)
        {
            try
            {
                this.IsEnabled = false;
                var config = Configuration.Default.WithDefaultLoader();
                //var config = new Configuration().WithJavaScript();
                var address = textUrl.Text; //"https://en.wikipedia.org/wiki/List_of_The_Big_Bang_Theory_episodes";
                var context = BrowsingContext.New(config);
                var document = await context.OpenAsync(address);
                if (document != null)
                {
                    _currentDocument = document;
                    var src = document.Source;
                    if (src != null)
                    {
                        textHTML.Text = src.Text;
                    }
                    //var root = document.GetRoot();
                    //if(root != null)
                    //{
                    //    var col = root.ChildNodes;
                    //    OutputNodes(col);
                    //}

                    //textHTML.Clear();
                    //var div = "ul.uni-blk-list01";
                    //var celss = document.QuerySelectorAll(div);
                    //if(celss != null)
                    //{
                    //    foreach(var ul in celss)
                    //    {
                    //        //获取子元素
                    //        var li = ul.ChildNodes;
                    //        foreach(var l in li)
                    //        {
                    //            if (l != null)
                    //            {
                    //                var a = l.ChildNodes.GetElementsByTagName("a");
                    //                if(a != null)
                    //                {
                    //                    foreach(var c in    a)
                    //                    {
                    //                        var url = c.GetAttribute("href");
                    //                        var content = c.TextContent;
                    //                        //System.Diagnostics.Debug.WriteLine(url);
                    //                        //System.Diagnostics.Debug.WriteLine(content);
                    //                    }
                    //                }
                    //            }
                    //        }
                    //    }
                    //}
                    ////var cellSelector = "tr.vevent td:nth-child(3)";
                    ////var cells = document.QuerySelectorAll(cellSelector);
                    ////var titles = cells.Select(m => m.TextContent);
                }
                this.IsEnabled = true;
            }
            catch (Exception exp)
            {
                textHTML.Text = exp.Message;
                this.IsEnabled = true;
            }
        }

        private void OnButtonGetContentClick(object sender, RoutedEventArgs e)
        {
            try
            {
                this.IsEnabled = false;
                textContent.Clear();
                if (_currentDocument != null)
                {
                    var toQuery = textAttr.Text;
                    var celss = _currentDocument.QuerySelectorAll(toQuery);
                    if (celss != null)
                    {
                        foreach (var ul in celss)
                        {
                            textContent.AppendText(ul.InnerHtml);
                            textContent.AppendText("\r\n");
                            ////获取子元素
                            //var li = ul.ChildNodes;
                            //foreach (var l in li)
                            //{
                            //    if (l != null)
                            //    {
                            //        var a = l.ChildNodes.GetElementsByTagName("a");
                            //        if (a != null)
                            //        {
                            //            foreach (var c in a)
                            //            {
                            //                var url = c.GetAttribute("href");
                            //                var content = c.TextContent;
                            //                textHTML.AppendText(url);
                            //                textHTML.AppendText("\r\n");
                            //                textHTML.AppendText(content);
                            //                textHTML.AppendText("\r\n");
                            //            }
                            //        }
                            //    }
                            //}
                        }
                    }
                }
                this.IsEnabled = true;
            }
            catch (Exception exp)
            {
                textContent.Text = exp.Message;
                this.IsEnabled = true;
            }
        }
        #region 辅助函数
        IDocument? _currentDocument = null;
        void OutputNodes(INodeList nodes)
        {
            //if (nodes == null) return;
            //foreach(var v in nodes)
            //{
            //    System.Diagnostics.Debug.WriteLine(String.Format("<{0}> - {1} - {2}", v.NodeName, v.NodeType, v.NodeValue));
            //    OutputNodes(v.ChildNodes);
            //    System.Diagnostics.Debug.WriteLine(String.Format("</{0}>", v.NodeName));
            //}
        }
        #endregion
    }
}
