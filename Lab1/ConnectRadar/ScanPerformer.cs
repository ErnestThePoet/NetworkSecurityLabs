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
using System.Windows;

namespace ConnectRadar
{
    internal class ScanPerformer
    {
        // ipv4Address here is still Little Endian
        private readonly record struct ScanTarget(uint ipv4Address, int port);

        private int totalScanCount;
        private int finishedScanCount;

        // Must be thread-safe
        public event Action<ScanResult>? OnScanUpdate;
        // Must be thread-safe, progress value is [0,1]
        public event Action<double>? OnScanProgressUpdate;

        public event Action? OnScanCompleted;

        private void RunOnUiThread(Action action)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                action();
            });
        }

        private void StartSingleScan(object? scanTargetsObj)
        {
            List<ScanTarget> scanTargets =
                (scanTargetsObj as List<ScanTarget>) ?? new();

            foreach (ScanTarget scanTarget in scanTargets)
            {
                IPEndPoint iPEndPoint = new(
                    BinaryPrimitives.ReverseEndianness(scanTarget.ipv4Address),
                    scanTarget.port);
                using Socket serverSocket = new(
                    iPEndPoint.AddressFamily,
                    SocketType.Stream,
                    ProtocolType.Tcp);

                ScanResult scanResult = new(
                        iPEndPoint.Address.ToString(), scanTarget.port, true);

                try
                {
                    serverSocket.Connect(iPEndPoint);
                }
                catch (SocketException e)
                {
                    scanResult.isOpen = false;
                }

                RunOnUiThread(() =>
                {
                    OnScanUpdate?.Invoke(scanResult);
                });

                Interlocked.Increment(ref finishedScanCount);
                if (finishedScanCount == totalScanCount)
                {
                    RunOnUiThread(() =>
                    {
                        OnScanCompleted?.Invoke();
                    });
                }
            }

            RunOnUiThread(() =>
            {
                OnScanProgressUpdate?.Invoke(
                    (double)finishedScanCount / totalScanCount);
            });
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
            List<ScanTarget> currentScanTargets = new();
            for (uint i = ipv4AddressStart; i <= ipv4AddressEnd; i++)
            {
                for (int j = portStart; j <= portEnd; j++)
                {
                    currentScanTargets.Add(new(i, j));
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
