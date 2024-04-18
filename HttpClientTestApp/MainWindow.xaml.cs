using RestSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http.Headers;
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

namespace HttpClientTestApp
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

        private void OnButtonTestClick(object sender, RoutedEventArgs e)
        {
            var url = textUrl.Text;
            if(string.IsNullOrEmpty(url))
            {
                MessageBox.Show("请输入URL");
                return;
            }

            try
            {
                this.IsEnabled = false;
                var info = new Info();
                System.Net.ServicePointManager.SecurityProtocol = System.Net.SecurityProtocolType.Tls12;
                RestSharp.RestClient client = new RestSharp.RestClient(url);
                var request = new RestRequest();

                request.Method = Method.PUT;
                request.AddHeader("token", "{\"agentId\":\"nsJ3of7aG\",\"appKey\":\"78f6234ddn2fncc0G4G4\",\"appSecret\":\"f81bb7d385fcd8d8704fa6b935561c28bdbc48a6d021c51f764910b3bb044b89\"}");
                request.AddHeader("Accept", "application/oapi.zyws.v1.0+json");
                request.AddJsonBody(info);
                var response = client.Execute(request);
                System.Diagnostics.Debug.WriteLine(response);
                if (response != null)
                {
                    if (response.Content != null)
                    {
                        System.Diagnostics.Debug.WriteLine($"{response.Content}");
                        textMsg.Text = response.Content;
                    }
                    else
                    {
                        textMsg.Text = "Response的Content为null";
                    }
                }
                else
                {
                    textMsg.Text = "Response为null";
                }    

                //restClient.DefaultRequestHeaders.Add("token", "{\"agentId\":\"nsJ3of7aG\",\"appKey\":\"78f6234ddn2fncc0G4G4\",\"appSecret\":\"f81bb7d385fcd8d8704fa6b935561c28bdbc48a6d021c51f764910b3bb044b89\"}");
                ////_client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
                //_client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/oapi.zyws.v1.0+json"));
                //restClient.PutJsonAsync(url, info);
            }
            catch (Exception ex)
            {
                textMsg.Text = ex.Message;
            }
            finally
            {
                this.IsEnabled = true;
            }
        }
    }
    public class Info
    {
        public string appointmentId { get; set; } = "BgKCOCB9P";
        public string startTime { get; set; }
        public string endTime { get; set; }
    }

}
