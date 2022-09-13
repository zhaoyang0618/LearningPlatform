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
        private void OnButtonMaxSubArrayClick(object sender, RoutedEventArgs e)
        {
            //排序算法
            this.IsEnabled = false;
            var a = new int[10];
            var rnd = new Random();
            for(int i=0;i<10;i++)
            {
                a[i] = rnd.Next(-100, 100);
            }
            foreach(var v in a)
            {
                System.Diagnostics.Debug.Write(v);
                System.Diagnostics.Debug.Write("; ");
            }
            System.Diagnostics.Debug.Write("\n");
            //
            var (low, high, sum) = FindMaximumSubArray.find(a);
            System.Diagnostics.Debug.WriteLine(String.Format("{0} - {1}: {2}", low, high, sum));
            //
            this.IsEnabled = true;
        }
    }
}
