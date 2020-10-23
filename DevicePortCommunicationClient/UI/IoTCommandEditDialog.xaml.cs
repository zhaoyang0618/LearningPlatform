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
    /// Interaction logic for IoTCommandEditDialog.xaml
    /// </summary>
    public partial class IoTCommandEditDialog : UserControl
    {
        public event Action OnOK;
        public event Action OnCancel;
        public IoTCommand Command
        {
            get;
            private set;
        }

        public IoTCommandEditDialog(IoTCommand cmd)
        {
            InitializeComponent();
            this.Command = new IoTCommand()
            {
                Command = cmd.Command,
                CommandData = cmd.CommandData,
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
            this.DataContext = this.Command;
        }

        void BindEvents()
        {
            btnOK.Click += OnButtonOKClick;
            btnCancel.Click += OnButtonCancelClick;
        }

        #endregion
    }
}
