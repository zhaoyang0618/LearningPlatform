using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DevicePortCommunicationClient.Device
{
    /// <summary>
    /// 设备数据采集中心
    /// 管理各个线程，管理需要采集的数据队列
    /// 队列中应该就是一次通信，
    /// </summary>
    class DeviceDataGatherManager
    {
        public int ThreadNum
        {
            get;set;
        }

        public void Clear()
        {
            lock(_communicationListLock)
            {
                _communicationList.Clear();
            }
        }

        public void Add(DeviceCommunication c)
        {
            lock (_communicationListLock)
            {
                _communicationList.Enqueue(c);
            }
        }

        public void Add(IEnumerable<DeviceCommunication> list)
        {
            lock (_communicationListLock)
            {
                foreach(var v in list)
                {
                    _communicationList.Enqueue(v);
                }
            }
        }

        object _communicationListLock = new object();
        Queue<DeviceCommunication> _communicationList = new Queue<DeviceCommunication>();
        List<Task> _taskList = new List<Task>();
        CancellationTokenSource _cancellationTokenSource = new CancellationTokenSource();

        public DeviceDataGatherManager()
        {
            this.ThreadNum = 8;
        }

        /// <summary>
        /// 开始通讯；不停的获取数据
        /// 这里需要开启线程
        /// </summary>
        public void Start()
        {
            if(ThreadNum <= 0)
            {
                ThreadNum = 2;
            }

            for(int i=0;i<ThreadNum;i++)
            {
                _taskList.Add(Task.Run(()=> {
                    RunThread();
                }));
            }
        }

        /// <summary>
        /// 结束通讯
        /// </summary>
        public void Stop()
        {
            //这个等待可能需要一段时间
            _cancellationTokenSource.Cancel();
        }

        /// <summary>
        /// 执行和设备的通信线程: 需要保证同一个IP的是在同一个线程之中进行执行
        /// </summary>
        private void RunThread()
        {
            var token = _cancellationTokenSource.Token;
            while(true)
            {
                if(token.IsCancellationRequested)
                {
                    break;
                }

                DeviceCommunication cur = null;
                lock (_communicationListLock)
                {
                    if(_communicationList.Count > 0)
                    {
                        cur = _communicationList.Dequeue();
                    }
                }

                if(cur != null)
                {
                    cur.DoCommunication(token);
                }

                //
                System.Threading.Thread.Sleep(10);
            }
        }

        /// <summary>
        /// 心跳线程
        /// </summary>
        private void TickThread()
        {
            var token = _cancellationTokenSource.Token;
            while (true)
            {
                if (token.IsCancellationRequested)
                {
                    break;
                }

                DeviceCommunication cur = null;
                lock (_communicationListLock)
                {
                    if (_communicationList.Count > 0)
                    {
                        cur = _communicationList.Dequeue();
                    }
                }

                if (cur != null)
                {
                    cur.DoTick();
                }

                //
                System.Threading.Thread.Sleep(10);
            }
        }
    }
}
