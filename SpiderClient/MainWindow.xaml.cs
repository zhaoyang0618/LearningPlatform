using AngleSharp;
using AngleSharp.Dom;
using AngleSharp.Scripting;
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
        private async void OnButtonGetContentClick(object sender, RoutedEventArgs e)
        {
            var config = Configuration.Default.WithDefaultLoader();
            //var config = new Configuration().WithJavaScript();
            var address = textUrl.Text; //"https://en.wikipedia.org/wiki/List_of_The_Big_Bang_Theory_episodes";
            var context = BrowsingContext.New(config);
            var document = await context.OpenAsync(address);
            if(document != null)
            {
                var src = document.Source;
                if(src != null)
                {
                    textHTML.Text = src.Text;
                }
                var root = document.GetRoot();
                if(root != null)
                {
                    var col = root.ChildNodes;
                    OutputNodes(col);
                }

                var div = "div.s-text-content";
                var celss = document.QuerySelectorAll(div);
                //var cellSelector = "tr.vevent td:nth-child(3)";
                //var cells = document.QuerySelectorAll(cellSelector);
                //var titles = cells.Select(m => m.TextContent);
            }
        }

        #region 辅助函数
        void OutputNodes(INodeList nodes)
        {
            if (nodes == null) return;
            foreach(var v in nodes)
            {
                System.Diagnostics.Debug.WriteLine(String.Format("<{0}> - {1} - {2}", v.NodeName, v.NodeType, v.NodeValue));
                OutputNodes(v.ChildNodes);
                System.Diagnostics.Debug.WriteLine(String.Format("</{0}>", v.NodeName));
            }
        }
        #endregion
    }
}
