using HtmlAgilityPack;
using RestSharp;
using System;
using System.Collections.Generic;
using System.IO;
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

namespace BookStudio
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            InitUI();
            BindEvents();
        }

        #region 事件处理
        private async void OnButtonDownloadClick(object sender, RoutedEventArgs e)
        {
            //首先获取网页，然后解析出其中的文件下载地址
            //
            btnDownload.IsEnabled = false;
            // From Web
            var url = textURL.Text;// "http://html-agility-pack.net/";
            var web = new HtmlWeb();
            var doc = await web.LoadFromWebAsync(url);

            //
            //string tempFile = "";
            //string baseUrl = "";
            //using (var writer = File.OpenWrite(tempFile))
            //{
            //    var client = new RestClient(baseUrl);
            //    var request = new RestRequest("Assets/LargeFile.7z") { 
            //        ResponseWriter = responseStream =>
            //        {
            //            using (responseStream)
            //            {
            //                responseStream.CopyTo(writer);
            //            }

            //            return writer;
            //        }
            //    };

            //    var response = await client.DownloadStreamAsync(request);
            //}
            btnDownload.IsEnabled = true;
        }

        #endregion

        #region 辅助函数
        void InitUI()
        {
            textURL.Text = "http://libgen.rs/search.php?&req=London+Mathematical+Society+Lecture+Note+Series&phrase=1&view=simple&column=series&sort=title&sortmode=DESC&page=27";
            textURL.Text = "http://www.sina.com.cn";
        }

        void BindEvents()
        {
            btnDownload.Click += OnButtonDownloadClick;
        }

        #endregion
    }
}
