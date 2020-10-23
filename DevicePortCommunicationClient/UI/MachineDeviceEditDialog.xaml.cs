using DevicePortCommunicationClient.Device;
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
    /// Interaction logic for MachineDeviceEditDialog.xaml
    /// </summary>
    public partial class MachineDeviceEditDialog : UserControl
    {
        public event Action OnOK;
        public event Action OnCancel;
        public MachineDevice Device
        {
            get;
            private set;
        }

        public MachineDeviceEditDialog(MachineDevice device)
        {
            InitializeComponent();
            //复制数据
            this.Device = new MachineDevice()
            {
                Id = device.Id,
                Name = device.Name,
                CommModules = new List<CommModule>(),
            };
            InitUI();
            BindEvents();
        }

        #region 事件处理
        private void OnButtonOKClick(object sender, RoutedEventArgs e)
        {
            OnOK?.Invoke();
        }

        private void OnButtonCancelClick(object sender, RoutedEventArgs e)
        {
            OnCancel?.Invoke();
        }


        #endregion

        #region 辅助函数
        NLog.Logger logger = NLog.LogManager.GetCurrentClassLogger();

        void InitUI()
        {
            this.DataContext = this.Device;
        }

        void BindEvents()
        {
            btnOK.Click += OnButtonOKClick;
            btnCancel.Click += OnButtonCancelClick;
        }

        #endregion

    }
}
