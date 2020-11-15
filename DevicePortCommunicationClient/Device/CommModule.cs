using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Taiji.Utils;

namespace DevicePortCommunicationClient.Device
{
    /// <summary>
    /// 用于通信模块:
    /// 很多通讯模块一个时间只能发送一条指令，所以这个地方最好是由一个线程进行处理
    /// 使用TCP/IP
    /// </summary>
    public class CommModule
    {
        /// <summary>
        /// 唯一编码
        /// </summary>
        public string Id
        {
            get;set;
        }

        /// <summary>
        /// 名称
        /// </summary>
        public string Name
        {
            get;set;
        }

        /// <summary>
        /// IP地址
        /// </summary>
        public string IPAddress
        {
            get;set;
        }

        /// <summary>
        /// 通讯端口
        /// </summary>
        public int Port
        {
            get;set;
        }

        /// <summary>
        /// 获取数据的时间间隔: 秒
        /// </summary>
        public int Interval
        {
            get; set;
        }

        public List<IoTCommand> IoTCommands
        {
            get;set;
        }


        /// <summary>
        /// 开启连接: 同时
        /// </summary>
        public void StartConnect()
        {
            dataTransfer = new DataTransfer(IPAddress, Port);
            dataTransfer.StartConnect();
            task = Task.Run(()=> {
                RunThread();
            });
        }

        /// <summary>
        /// 结束连接
        /// </summary>
        public void StopConnect()
        {
            cancellationTokenSource.Cancel();
            //可以稍微等待一会儿
            //最多等待一秒钟
            if (task != null)
            {
                for(int i=0;i<10;i++)
                {
                    if(task.IsCompleted)
                    {
                        break;
                    }

                    System.Threading.Thread.Sleep(100);
                }
            }

            //关闭连接
            if(dataTransfer != null)
            {
                dataTransfer.StopConnect();
                dataTransfer = null;
            }
        }

        DataTransfer dataTransfer = null;
        Task task = null;
        CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
        void RunThread()
        {
            var token = cancellationTokenSource.Token;
            while (true)
            {
                if(token.IsCancellationRequested)
                {
                    break;
                }

                //不停的发送消息，接收数据
                if(IoTCommands != null && IoTCommands.Count > 0)
                {
                    foreach (var cmd in IoTCommands)
                    {
                        if (token.IsCancellationRequested)
                        {
                            break;
                        }

                        //发送
                        if (dataTransfer != null)
                        {
                            dataTransfer.SendData(cmd.Command.ToHexBytes());
                            //等待接收数据
                        }
                    }
                }

                //
                System.Threading.Thread.Sleep(10);
            }
        }
    }
}
