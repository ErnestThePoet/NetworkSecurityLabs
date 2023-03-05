using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Buffers.Binary;
using System.Net.Sockets;
using System.Diagnostics.CodeAnalysis;

namespace ConnectRadar
{
    internal class ScanPerformer
    {
        // ipv4Address here is still Little Endian
        private readonly record struct ScanTarget(uint ipv4Address, int port);

        private int totalScanCount;
        private int finishedScanCount;

        public event Action<ScanResult>? OnScanUpdate;
        public event Action<double>? OnScanProgressUpdate;

        private void StartSingleScan(object? scanTargetsObj)
        {
            List<ScanTarget> scanTargets =
                (scanTargetsObj as List<ScanTarget>) ?? new List<ScanTarget>();

            foreach (ScanTarget scanTarget in scanTargets)
            {
                IPEndPoint iPEndPoint = new(
                    BinaryPrimitives.ReverseEndianness(scanTarget.ipv4Address),
                    scanTarget.port);
                using Socket serverSocket = new Socket(
                    iPEndPoint.AddressFamily,
                    SocketType.Stream,
                    ProtocolType.Tcp);

                ScanResult scanResult = new ScanResult(
                        iPEndPoint.Address.ToString(), scanTarget.port, true);

                try
                {
                    serverSocket.Connect(iPEndPoint);
                }
                catch (SocketException e)
                {
                    scanResult.isOpen = false;
                }

                OnScanUpdate?.Invoke(scanResult);
                Interlocked.Increment(ref finishedScanCount);
            }

            OnScanProgressUpdate?.Invoke((double)finishedScanCount / totalScanCount);
        }

        public void StartScan(
            uint ipv4AddressStart,
            uint ipv4AddressEnd,
            int portStart,
            int portEnd,
            int threadCount)
        {
            totalScanCount = (int)(ipv4AddressEnd - ipv4AddressStart + 1)
                * (portEnd - portStart + 1);
            if (threadCount > totalScanCount)
            {
                threadCount = totalScanCount;
            }

            finishedScanCount = 0;

            int remainingScanCount = totalScanCount;
            int remainingThreadCount = threadCount;
            int currentScanCountPerThread = remainingScanCount / remainingThreadCount;
            List<ScanTarget> currentScanTargets = new List<ScanTarget>();
            for (uint i = ipv4AddressStart; i <= ipv4AddressEnd; i++)
            {
                for (int j = portStart; j <= portEnd; j++)
                {
                    currentScanTargets.Add(new ScanTarget(i, j));
                    remainingScanCount--;

                    if (currentScanTargets.Count >= currentScanCountPerThread)
                    {
                        new Thread(StartSingleScan).Start(currentScanTargets);
                        remainingThreadCount--;
                        currentScanTargets = new();
                        if (remainingThreadCount > 0)
                        {
                            currentScanCountPerThread =
                                remainingScanCount / remainingThreadCount;
                        }
                    }
                }
            }
        }
    }
}
