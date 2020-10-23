using System;
using System.Collections.Generic;
using System.Text;

namespace DevicePortCommunicationClient.Device
{
    /// <summary>
    /// 指令传输:有获取类型，也有设置类型
    /// </summary>
    class CommandTransform
    {
        public IoTCommand Command
        {
            get;set;
        }
    }
}
