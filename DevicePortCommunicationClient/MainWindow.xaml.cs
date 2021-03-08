using DevicePortCommunicationClient.Device;
using MahApps.Metro.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Net.Sockets;
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

namespace DevicePortCommunicationClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        public MainWindow()
        {
            InitializeComponent();
            InitUI();
            BindEvents();
        }

        #region 事件处理

        private void OnWindowUnloaded(object sender, RoutedEventArgs e)
        {
            client.Stop();
        }

        private void OnWindowLoaded(object sender, RoutedEventArgs e)
        {
            try
            {
                client.Start("10.10.10.10", 3010);
            }
            catch (Exception exp)
            {
                logger.Error(exp, "连接莱宾格喷码机异常");
            }
        }

        private void OnListMachinesSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if(e.AddedItems != null && e.AddedItems.Count> 0)
            {
                //只选取第一个
                var machine = e.AddedItems[0] as MachineDevice;
                if(machine != null)
                {
                    listModules.ItemsSource = machine.CommModules;
                }
            }
        }

        private void OnListModulesSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems != null && e.AddedItems.Count > 0)
            {
                //只选取第一个
                var module = e.AddedItems[0] as CommModule;
                if (module != null)
                {
                    listCommands.ItemsSource = module.IoTCommands;
                }
            }
        }

        private void OnButtonAddMachineClick(object sender, RoutedEventArgs e)
        {
            var dlg = new UI.MachineDeviceEditDialog(new Device.MachineDevice());
            dlg.OnOK += () => {
                //复制数据
                listMachines.Items.Add(dlg.Device);
                gridMain.IsEnabled = true;
                gridDialog.Visibility = Visibility.Collapsed;
            };
            dlg.OnCancel += () => {
                gridMain.IsEnabled = true;
                gridDialog.Visibility = Visibility.Collapsed;
            };
            gridDialog.Children.Clear();
            gridDialog.Children.Add(dlg);
            gridMain.IsEnabled = false;
            gridDialog.Visibility = Visibility.Visible;
        }


        private void OnButtonAddCommunicationClick(object sender, RoutedEventArgs e)
        {
            //首先需要有当前选中的设备
            var sel = listMachines.SelectedItem;
            if(null == sel)
            {
                return;
            }

            var machine = sel as MachineDevice;
            if (machine == null)
            {
                return;
            }

            var dlg = new UI.CommDeviceEditDialog(machine, new CommModule());
            dlg.OnOK += () =>
            {
                //复制数据
                machine.CommModules.Add(dlg.Module);
                //listModules.ItemsSource = machine.CommModules;
                listModules.Items.Refresh();
                gridMain.IsEnabled = true;
                gridDialog.Visibility = Visibility.Collapsed;
            };
            dlg.OnCancel += () =>
            {
                gridMain.IsEnabled = true;
                gridDialog.Visibility = Visibility.Collapsed;
            };
            gridDialog.Children.Clear();
            gridDialog.Children.Add(dlg);
            gridMain.IsEnabled = false;
            gridDialog.Visibility = Visibility.Visible;
        }

        private void OnButtonAddCommandClick(object sender, RoutedEventArgs e)
        {
            //添加指令
            //首先要求选择了通讯模组
            var sel = listModules.SelectedItem;
            if (null == sel)
            {
                return;
            }

            var module = sel as CommModule;
            if (module == null)
            {
                return;
            }

            var dlg = new UI.IoTCommandEditDialog(new IoTCommand());
            dlg.OnOK += () => {
                //复制数据
                module.IoTCommands.Add(dlg.Command);
                listCommands.Items.Refresh();
                gridMain.IsEnabled = true;
                gridDialog.Visibility = Visibility.Collapsed;
            };
            dlg.OnCancel += () => {
                gridMain.IsEnabled = true;
                gridDialog.Visibility = Visibility.Collapsed;
            };
            gridDialog.Children.Clear();
            gridDialog.Children.Add(dlg);
            gridMain.IsEnabled = false;
            gridDialog.Visibility = Visibility.Visible;
        }

        private void OnButtonPrintClick(object sender, RoutedEventArgs e)
        {
            //喷码打印
            var client = new TcpClient();
            try
            {
                string ip = "192.168.1.100";
                int port = 1000;
                client.Connect(ip, port);
                NetworkStream ns = client.GetStream();

                string cmd = "^0*BEGINLJSCRIPT\r\n";
                cmd += "BEGINLJSCRIPT [(V1.1.0.0)]\r\n";
                cmd += "JLPAR [85 0 0 0 2 0 0 25000 00:00 0]\r\n";
                cmd += "BEGINJOB [ 0 () ]\r\n";
                cmd += "JOBPAR [ 20000 0 0 350 0 0 0 1 1 0]\r\n";
                cmd += "OBJ [0 1 0 0 (ISO7_24x18) (Text) 1 0 0 0 0 1 0 0 0 0]\r\n";
                cmd += "OBJ [0 62 0 0 (ISO7_12X8) ({c}) 1 0 0 0 0 0 0 0 0 0]\r\n";
                cmd += "CNT [ 4 1000 0 8000 1 1 1 1 10 0 1]\r\n";
                cmd += "OBJ [0 99 0 0 (ISO7_7X5) ({t}) 1 0 0 0 0 0 0 0 0 0]\r\n";
                cmd += "TIME [ (dd.mm.yyyy) 0]\r\n";
                cmd += "RPLDAY [ (MON) (TUE) (WED) (THU) (FRI) (SAT) (SUN) ]\r\n";
                cmd += "RPLMON [ (JAN) (FEB) (MAR) (APR) (MAY) (JUN) (JUL) (AUG) (SEP) (OCT)\r\n";
                cmd += "(NOV) (DEC) ]\r\n";
                cmd += "ENDJOB []\r\n";
                cmd += "ENDLJSCRIPT []\r\n";
                cmd += "^0*ENDLJSCRIPT\r\n";
                byte[] data = Encoding.ASCII.GetBytes(cmd);
                //发送数据
                ns.Write(data, 0, data.Length);
                ns.Close();
            }
            catch
            {

            }
            finally
            {
                client.Close();
            }
        }

        Client.MobusClient client = new Client.MobusClient();
        private void OnButtonQueryClick(object sender, RoutedEventArgs e)
        {
            //查询
            //var client = new TcpClient();
            //try
            //{
            //    //string ip = "192.168.1.100";
            //    //莱宾格喷码机
            //    string ip = "10.10.10.10";
            //    int port = 3010;
            //    client.Connect(ip, port);
            //    NetworkStream ns = client.GetStream();

            //    string cmd = "^?EX\r\n";
            //    byte[] data = Encoding.ASCII.GetBytes(cmd);
            //    //发送数据
            //    ns.Write(data, 0, data.Length);

            //    //接收数据
            //    while(ns.DataAvailable)
            //    {
            //        var d = new byte[256];
            //        var len = ns.Read(d, 0, 256);
            //        //
            //        string message = Encoding.ASCII.GetString(data, 0, len);//只将接收到的数据进行转化
            //    }
            //    //
            //    ns.Close();
            //}
            //catch
            //{

            //}
            //finally
            //{
            //    client.Close();
            //}

            string cmd = "^0?EL\r\n";
            client.Send(cmd);
        }

        #endregion

        #region 一些测试函数
        byte[] TestByte
        {
            get;set;
        }
        byte[] TestByte2
        {
            get; set;
        }
        private void OnButtonTestAwait1Click(object sender, RoutedEventArgs e)
        {
            //
            var source = new byte[] { 1, 2, 3, 4, 5, 6, 7, 8 };
            var spanSource = new ReadOnlySpan<byte>(source, 0, 6);
            TestByte = new byte[3];
            CopyData(spanSource, 1, TestByte, 3);
            TestByte2 = new byte[2];
            CopyData(spanSource, 3, TestByte2, 2);
            //测试await的用法
            TestAwait1();
            logger.Debug("测试异步await1");
        }
        private async void OnButtonTestAwait2Click(object sender, RoutedEventArgs e)
        {
            //测试await的用法
            await TestAwait2();
            logger.Debug("测试异步await2");
        }

        async void TestAwait1()
        {
            logger.Debug("TestAwait1开始");
            await Task.Run(()=> {
                System.Threading.Thread.Sleep(1000);
            });
            logger.Debug("TestAwait1结束");
        }

        async Task TestAwait2()
        {
            logger.Debug("TestAwait2开始");
            await Task.Run(() => {
                System.Threading.Thread.Sleep(1000);
            });
            logger.Debug("TestAwait2结束");
        }

        void CopyData(ReadOnlySpan<byte> src, int offset, byte[] dst, int length)
        {
            Span<byte> target = new Span<byte>(dst);
            var source = src.Slice(offset, length);
            source.CopyTo(target);
        }

        #endregion

        #region 辅助函数
        NLog.Logger logger = NLog.LogManager.GetCurrentClassLogger();

        void InitUI()
        {
            try
            {
                var context = DAL.IoTDbContext.Context;
                var m = new DAL.DBMachineDevice()
                {
                    Id = Guid.NewGuid().ToString(),
                    Name = "印刷机",
                    DBCommModules = new List<DAL.DBCommModule>(),
                };
                context.DBMachineDevices.Add(m);
                context.SaveChanges();
            }
            catch (Exception exp)
            {
                System.Diagnostics.Debug.WriteLine(exp);
                logger.Error(exp, "数据库存储");
            }
        }

        void BindEvents()
        {
            this.Loaded += OnWindowLoaded;
            this.Unloaded += OnWindowUnloaded;
            listMachines.SelectionChanged += OnListMachinesSelectionChanged;
            listModules.SelectionChanged += OnListModulesSelectionChanged;
            btnAddMachine.Click += OnButtonAddMachineClick;
            btnAddCommunication.Click += OnButtonAddCommunicationClick;
            btnAddCommand.Click += OnButtonAddCommandClick;
            btnPrint.Click += OnButtonPrintClick;
            btnQuery.Click += OnButtonQueryClick;
        }

        #endregion

    }
}
