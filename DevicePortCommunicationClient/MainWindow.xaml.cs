using DevicePortCommunicationClient.Device;
using MahApps.Metro.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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
            listMachines.SelectionChanged += OnListMachinesSelectionChanged;
            listModules.SelectionChanged += OnListModulesSelectionChanged;
            btnAddMachine.Click += OnButtonAddMachineClick;
            btnAddCommunication.Click += OnButtonAddCommunicationClick;
            btnAddCommand.Click += OnButtonAddCommandClick;
        }

        #endregion
    }
}
