using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConnectRadar
{
    internal record struct ScanResult(string ipv4Address,int port,bool isOpen);
}
