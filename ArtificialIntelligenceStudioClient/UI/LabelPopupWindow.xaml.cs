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
using System.Windows.Shapes;

namespace ArtificialIntelligenceStudioClient.UI
{
    /// <summary>
    /// Interaction logic for LabelPopupWindow.xaml
    /// </summary>
    public partial class LabelPopupWindow : Window
    {
        public string Label { get; set; }
        public LabelPopupWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }
    }
}
