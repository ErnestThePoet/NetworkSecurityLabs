using System;
using System.Collections.Generic;
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
        ParseHelper parseHelper;
        PortScanManager portScanManager;
        public MainWindow()
        {
            parseHelper = new ParseHelper();
            portScanManager = new PortScanManager();

            InitializeComponent();
        }

        private void OnTbIpComponentTextChanged(
            TextBox current, TextBox? next, bool isStart, int currentIndex)
        {
            int parsedComponent = parseHelper.ParseIp4AddressComponent(current.Text);
            current.Text = parsedComponent.ToString();
            portScanManager.SetIp4AddressComponent(isStart, currentIndex, parsedComponent);

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
            portScanManager.SetPortStart(parsedPort);
        }

        private void tbPortEnd_TextChanged(object sender, TextChangedEventArgs e)
        {
            int parsedPort = parseHelper.ParsePort(tbPortEnd.Text);
            tbPortEnd.Text = parsedPort.ToString();
            portScanManager.SetPortEnd(parsedPort);
        }

        private void tbThreadCount_TextChanged(object sender, TextChangedEventArgs e)
        {
            int parsedThreadCount = parseHelper.ParseThreadCount(tbThreadCount.Text);
            tbThreadCount.Text = parsedThreadCount.ToString();
            portScanManager.SetThreadCount(parsedThreadCount);
        }

        private void btnThreadDec_Click(object sender, RoutedEventArgs e)
        {
            tbThreadCount.Text = (int.Parse(tbThreadCount.Text) - 1).ToString();
        }

        private void btnThreadInc_Click(object sender, RoutedEventArgs e)
        {
            tbThreadCount.Text = (int.Parse(tbThreadCount.Text) + 1).ToString();
        }

        private void btnStart_Click(object sender, RoutedEventArgs e)
        {
            string message;
            if(!portScanManager.CheckScanParameters(out message))
            {
                MessageBox.Show($"【错误】{message}", "错误");
                return;
            }
        }

        private void btnEnd_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
