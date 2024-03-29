﻿using IronPython.Hosting;
using Microsoft.Scripting.Hosting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
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

namespace PythonExam
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// https://www.codeproject.com/articles/21119/embedding-ironpython-in-wpf-using-c
    /// Python函数调用
    /// https://www.cnblogs.com/wilber2013/p/4491297.html
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        LocalAppContext context = new LocalAppContext();
        private void OnButtonRunClick(object sender, RoutedEventArgs e)
        {
            //执行脚本
            try
            {
                ScriptEngine engine = Python.CreateEngine();

                var scriptIO = engine.Runtime.IO;

                //var ms = new System.IO.MemoryStream();
                //var tw = new System.IO.StringWriter();
                //scriptIO.SetOutput(ms, tw);
                var stream = new TriggerStream();  // TriggerStream是一个能够在写入时触发事件的, 继承了Stream的类.
                stream.PreviewWrite += (ss, ee) => {
                    if(ee.Count > 0)
                    {
                        var data = new byte[ee.Count];
                        Array.Copy(ee.Buffer, ee.Offset, data, 0, ee.Count);
                        var s = Encoding.Default.GetString(data);
                        if (s != null && s.Length > 0)
                        {
                            this.Dispatcher.Invoke(new Action(() => {
                                textMessage.AppendText(s);
                                textMessage.AppendText("\r\n");
                            }));
                        }
                    }
                };
                engine.Runtime.IO.SetOutput(stream, Encoding.Default);  // 这样, 我们可以通过TriggerStream的写入事件来获取写入的内容
                engine.Runtime.IO.SetErrorOutput(stream, Encoding.Default);

                //
                //scriptIO.RedirectToConsole();

                //ScriptSource script = engine.CreateScriptSourceFromFile(@"Script.py");
                var script = engine.CreateScriptSourceFromString(textScript.Text);

                dynamic scope = engine.CreateScope();
                //var sum = scope.add(1, 2);

                scope.SetVariable("localContext", context);
                context.name = "YuZhaoyang";

                dynamic obj = engine.Runtime.UseFile(@"E:\add.py");
                var sum = obj.add(11, 22);
                if(sum != null)
                {
                    textMessage.AppendText("11 + 22 = " + sum.ToString());
                    textMessage.AppendText("\r\n");
                }

                var result = script.Execute(scope);
                sum = scope.add(1, 2);
                if (sum != null)
                {
                    textMessage.AppendText("1 + 2 = " + sum.ToString());
                    textMessage.AppendText("\r\n");
                }

                System.Diagnostics.Debug.WriteLine(context.name);
                var names = scope.GetVariableNames();

                //try
                //{
                //    var c = engine.Execute(textScript.Text);
                //    var a = c.add(1, 2);
                //    if(a != null)
                //    {
                //        textMessage.AppendText("1 + 2 = " + a.ToString());
                //        textMessage.AppendText("\r\n");
                //    }
                //}
                //catch (Exception ex)
                //{
                //    textMessage.AppendText(ex.Message);
                //    textMessage.AppendText("\r\n");
                //}

                

                if (result != null)
                {
                    textMessage.AppendText("返回结果为: " + result.ToString());
                    textMessage.AppendText("\r\n");
                }
                else
                {
                    textMessage.AppendText("result为null");
                    textMessage.AppendText("\r\n");
                }
                //tw.Flush();
                //var text = tw.ToString();
            }
            catch (Exception exp)
            {
                textMessage.AppendText(exp.Message);
                textMessage.AppendText("\r\n");
            }
        }
    }
}
