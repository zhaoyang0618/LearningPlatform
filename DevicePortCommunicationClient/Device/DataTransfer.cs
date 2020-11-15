using socket.core.Client;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DevicePortCommunicationClient.Device
{
    /// <summary>
    /// 传输方式:目前就是TCP/IP的网络传输
    /// 一发一收:
    /// 这里使用网络上开源的Socket.Core
    /// 直接使用其源代码
    /// 试试能否加入心跳机制
    /// 重新连接
    /// </summary>
    class DataTransfer
    {
        private static NLog.Logger logger = NLog.LogManager.GetCurrentClassLogger();
        //在工控行业；很少会超过这个字节数
        //receiveBufferSize = 256
        TcpPushClient client = new TcpPushClient(256); 
        public string IpAddress
        {
            get;set;
        }

        public int Port
        {
            get;set;
        }

        /// <summary>
        /// 单位毫秒
        /// </summary>
        private int IntervalTime
        {
            get;set;
        }

        #region 事件
        public event Action OnTick;
        public event Action OnConnected;
        public event Action OnDisconnected;
        public event Action<byte[]> OnDataReceived;

        #endregion

        public DataTransfer(string ip, int port)
        {
            IpAddress = ip;
            Port = port;
            IntervalTime = 30 * 1000;
            //
            client.OnConnect += OnLocalConnect;
            client.OnReceive += OnLocalReceive;
            client.OnSend += OnLocalSend;
            client.OnClose += OnLocalClose;
        }

        private void OnLocalClose()
        {
            //关闭
            logger.Trace("[{0}:{1}]连接关闭", IpAddress, Port);
        }

        private void OnLocalSend(int obj)
        {
            LastTime = DateTime.Now;
            //发送成功
            logger.Trace("[{0}:{1}]数据发送完成: {2}", IpAddress, Port, obj);
        }

        private void OnLocalReceive(byte[] obj)
        {
            //接收到的数据
            LastTime = DateTime.Now;
            logger.Trace("[{0}:{1}]接收到数据: {2}", IpAddress, Port, obj.Length);
            OnDataReceived?.Invoke(obj);
        }

        private void OnLocalConnect(bool obj)
        {
            //连接成功
            logger.Trace("[{0}:{1}]连接成功", IpAddress, Port);
            OnConnected?.Invoke();
        }

        //建立连接
        public void StartConnect()
        {
            client.Connect(IpAddress, Port);
        }

        public void StopConnect()
        {
            client.Close();
        }

        public void SendData(byte[] data)
        {
            if(client != null)
            {
                client.Send(data, 0, data.Length);
            }
        }

        object _lastTimeLock = new object();
        DateTime _lastTime = DateTime.Now;
        public DateTime LastTime
        {
            get
            {
                lock(_lastTimeLock)
                {
                    return _lastTime;
                }
            }
            set
            {
                lock (_lastTimeLock)
                {
                    _lastTime = value;
                }
            }
        }
    }
}
