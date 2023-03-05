using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConnectRadar
{
    internal class PortScanManager
    {
        private uint ip4AddressStart;
        private uint ip4AddressEnd;

        private int portStart;
        private int portEnd;

        private int threadCount;

        public void SetIp4AddressComponent(bool isStart, int index, int component)
        {
            unsafe
            {
                fixed (uint* addressStartPtr = &ip4AddressStart,
                    addressEndPtr = &ip4AddressEnd)
                {
                    uint* addressPtr = isStart ? addressStartPtr : addressEndPtr;
                    *((byte*)addressPtr + index) = (byte)component;
                }
            }
        }

        public void SetPortStart(int portStart)
        {
            this.portStart = portStart;
        }

        public void SetPortEnd(int portEnd)
        {
            this.portEnd = portEnd;
        }

        public void SetThreadCount(int threadCount)
        {
            this.threadCount = threadCount;
        }

        public bool CheckScanParameters(out string message)
        {
            if (ip4AddressStart > ip4AddressEnd)
            {
                message = "起始IPv4地址大于结束IPv4地址";
                return false;
            }

            if (portStart > portEnd)
            {
                message = "起始端口号大于结束端口号";
                return false;
            }

            message = "";
            return true;
        }
    }
}
