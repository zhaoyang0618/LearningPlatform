using Algs4.Utils;
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

namespace Algs4
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

        private void OnButtonSortClick(object sender, RoutedEventArgs e)
        {
            //排序算法
            this.IsEnabled = false;
            string a = "SHELLSORTEXAMPLE";
            System.Diagnostics.Debug.WriteLine("插入排序");
            InsertionSort.sort(a.ToCharArray());
            System.Diagnostics.Debug.WriteLine("Shell排序");
            ShellSort.sort(a.ToCharArray());
            this.IsEnabled = true;
        }
    }
}
