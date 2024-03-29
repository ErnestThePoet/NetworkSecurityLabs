﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ConnectRadar
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private ParseHelper parseHelper;
        private ScanSettingsManager scanSettingsManager;
        private ScanPerformer scanPerformer;
        public ObservableCollection<ScanResult> scanResults { get; set; }

        public MainWindow()
        {
            parseHelper = new();
            scanSettingsManager = new();
            scanPerformer = new();
            scanResults = new();

            scanPerformer.OnScanUpdate += OnScanUpdate;
            scanPerformer.OnScanProgressUpdate += OnScanProgressUpdate;
            scanPerformer.OnScanCompleted += OnScanCompleted;

            InitializeComponent();

            DataContext = this;
        }

        private void OnTbIpComponentTextChanged(
            TextBox current, TextBox? next, bool isStart, int currentIndex)
        {
            int parsedComponent = parseHelper.ParseIp4AddressComponent(current.Text);
            current.Text = parsedComponent.ToString();
            scanSettingsManager.SetIp4AddressComponent(isStart, currentIndex, parsedComponent);

            if (next != null && current.Text.Length >= 3)
            {
                next.Focus();
                next.SelectAll();
            }
        }

        private void tbIpStart1_TextChanged(object sender, TextChangedEventArgs e)
        {
            OnTbIpComponentTextChanged(tbIpStart1, tbIpStart2, true, 3);
        }

        private void tbIpStart2_TextChanged(object sender, TextChangedEventArgs e)
        {
            OnTbIpComponentTextChanged(tbIpStart2, tbIpStart3, true, 2);
        }

        private void tbIpStart3_TextChanged(object sender, TextChangedEventArgs e)
        {
            OnTbIpComponentTextChanged(tbIpStart3, tbIpStart4, true, 1);
        }

        private void tbIpStart4_TextChanged(object sender, TextChangedEventArgs e)
        {
            OnTbIpComponentTextChanged(tbIpStart4, tbIpEnd1, true, 0);
        }

        private void tbIpEnd1_TextChanged(object sender, TextChangedEventArgs e)
        {
            OnTbIpComponentTextChanged(tbIpEnd1, tbIpEnd2, false, 3);
        }

        private void tbIpEnd2_TextChanged(object sender, TextChangedEventArgs e)
        {
            OnTbIpComponentTextChanged(tbIpEnd2, tbIpEnd3, false, 2);
        }

        private void tbIpEnd3_TextChanged(object sender, TextChangedEventArgs e)
        {
            OnTbIpComponentTextChanged(tbIpEnd3, tbIpEnd4, false, 1);
        }

        private void tbIpEnd4_TextChanged(object sender, TextChangedEventArgs e)
        {
            OnTbIpComponentTextChanged(tbIpEnd4, null, false, 0);
        }

        private void tbPortStart_TextChanged(object sender, TextChangedEventArgs e)
        {
            int parsedPort = parseHelper.ParsePort(tbPortStart.Text);
            tbPortStart.Text = parsedPort.ToString();
            scanSettingsManager.portStart = parsedPort;
        }

        private void tbPortEnd_TextChanged(object sender, TextChangedEventArgs e)
        {
            int parsedPort = parseHelper.ParsePort(tbPortEnd.Text);
            tbPortEnd.Text = parsedPort.ToString();
            scanSettingsManager.portEnd = parsedPort;
        }

        private void tbThreadCount_TextChanged(object sender, TextChangedEventArgs e)
        {
            int parsedThreadCount = parseHelper.ParseThreadCount(tbThreadCount.Text);
            tbThreadCount.Text = parsedThreadCount.ToString();
            scanSettingsManager.threadCount = parsedThreadCount;
        }

        private void tbConnectTimeoutMs_TextChanged(object sender, TextChangedEventArgs e)
        {
            int parsedConnectTimeoutMs = 
                parseHelper.ParseConnectTimeoutMs(tbConnectTimeoutMs.Text);
            tbConnectTimeoutMs.Text = parsedConnectTimeoutMs.ToString();
            scanSettingsManager.connectTimeoutMs = parsedConnectTimeoutMs;
        }

        private void btnStart_Click(object sender, RoutedEventArgs e)
        {
            string message;
            if (!scanSettingsManager.CheckScanSettings(out message))
            {
                MessageBox.Show($"【错误】{message}", "错误");
                return;
            }

            scanResults.Clear();
            pbProgress.Value = 0;
            btnStart.IsEnabled = false;

            scanPerformer.StartScan(
                scanSettingsManager.GetIp4AddressStart(),
                scanSettingsManager.GetIp4AddressEnd(),
                scanSettingsManager.portStart,
                scanSettingsManager.portEnd,
                scanSettingsManager.threadCount,
                scanSettingsManager.connectTimeoutMs);
        }

        private void OnScanUpdate(ScanResult scanResult)
        {
            lock (scanResults)
            {
                scanResults.Add(scanResult);
            }
        }

        private void OnScanProgressUpdate(double progress)
        {
            lock (pbProgress)
            {
                pbProgress.Value = progress * 100;
            }
        }

        private void OnScanCompleted()
        {
            pbProgress.Value = 100;
            btnStart.IsEnabled = true;
        }
    }
}
