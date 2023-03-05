using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConnectRadar
{
    internal class ScanSettingsManager
    {
        // Both are in Little Endian
        private uint ip4AddressStart;
        private uint ip4AddressEnd;

        public int portStart { get; set; }
        public int portEnd { get; set; }

        public int threadCount { get; set; }

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

        public uint GetIp4AddressStart()
        {
            return ip4AddressStart;
        }

        public uint GetIp4AddressEnd()
        {
            return ip4AddressEnd;
        }

        public bool CheckScanSettings(out string message)
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
