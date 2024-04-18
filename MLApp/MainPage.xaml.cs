using iText.Kernel.Pdf;
using MLApp.Services;

namespace MLApp
{
    public partial class MainPage : ContentPage
    {
        int count = 0;

        public MainPage()
        {
            InitializeComponent();
        }

        private void OnCounterClicked(object sender, EventArgs e)
        {
            count++;

            if (count == 1)
                CounterBtn.Text = $"Clicked {count} time";
            else
                CounterBtn.Text = $"Clicked {count} times";

            SemanticScreenReader.Announce(CounterBtn.Text);

            //读取PDF文件
            //var reader = new PdfReader(@"D:\bz_F1C8804DA9B8C2B120230203_www.bzfxw.com.pdf");
            //var doc = new PdfDocument(reader);
            //var num = doc.GetNumberOfPages();
            //for(var i = 0; i < num; i++)
            //{
            //    var page = doc.GetPage(num - 1);
            //    if(page == null)
            //        continue;

            //}
            //doc.Close();
            //reader.Close();

            var service = new HealthService();
            service.Confirm();
        }
    }

}
