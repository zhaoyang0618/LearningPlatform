using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using Taiji.Utils;

namespace DevicePortCommunicationClient.Device
{
    /// <summary>
    /// 这里包含一次通信; 以IP和端口作为索引
    /// 应该不需要搞成接口: 
    /// </summary>
    class DeviceCommunication
    {
        //需要加一把锁，一个事件只能发送一个:
        //心跳和正常的通讯可能会冲突
        DataTransfer _transfer = null;
        CommModule _module = null;

        public void DoCommunication(CancellationToken token)
        {
            if(null != _transfer && null != _module && _module.IoTCommands != null)
            {
                foreach (var cmd in _module.IoTCommands)
                {
                    if(token.IsCancellationRequested)
                    {
                        break;
                    }

                    //进行一次通讯:
                    _transfer.SendData(cmd.Command.ToHexBytes());
                }
            }
        }

        //心跳
        public void DoTick()
        {
            if (null != _module && _module.IoTCommands != null)
            {
                //进行一次心跳: 如果失败，需要进行重连
                //_transfer.SendData(cmd.Command.ToHexBytes());
            }
        }
    }
}
