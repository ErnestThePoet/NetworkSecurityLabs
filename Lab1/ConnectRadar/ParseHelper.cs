using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConnectRadar
{
    internal class ParseHelper
    {
        private const int IP4_ADDRESS_COMPONENT_MIN = 0;
        private const int IP4_ADDRESS_COMPONENT_MAX = 255;

        private const int PORT_MIN = 0;
        private const int PORT_MAX = 65535;

        private const int THREAD_COUNT_MIN = 1;
        private const int THREAD_COUNT_MAX = 9999;

        private const int CONNECT_TIMEOUT_MS_MIN = 500;
        private const int CONNECT_TIMEOUT_MS_MAX = 20000;

        private int ParseRange(string value,int min,int max)
        {
            int parsedValue;

            if (!int.TryParse(value, out parsedValue))
            {
                return 0;
            }

            if (parsedValue < min)
            {
                return min;
            }
            else if (parsedValue > max)
            {
                return max;
            }
            else
            {
                return parsedValue;
            }
        }

        public int ParseIp4AddressComponent(string component)
        {
            return ParseRange(
                component, IP4_ADDRESS_COMPONENT_MIN, IP4_ADDRESS_COMPONENT_MAX);
        }

        public int ParsePort(string port)
        {
            return ParseRange(port, PORT_MIN, PORT_MAX);
        }

        public int ParseThreadCount(string threadCount)
        {
            return ParseRange(threadCount, THREAD_COUNT_MIN, THREAD_COUNT_MAX);
        }

        public int ParseConnectTimeoutMs(string connectTimeoutMs)
        {
            return ParseRange(connectTimeoutMs, 
                CONNECT_TIMEOUT_MS_MIN, 
                CONNECT_TIMEOUT_MS_MAX);
        }
    }
}
