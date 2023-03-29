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

namespace AuthClient
{
    /// <summary>
    /// Interaction logic for AuthWrapper.xaml
    /// </summary>
    public partial class AuthWrapper : UserControl
    {
        public event EventHandler? SubmitAuthClick;
        public event EventHandler? ChangePasswordClick;
        public event EventHandler? SaveServerAuthCodeClick;
        public event EventHandler? SignUpClick;

        public AuthWrapper()
        {
            InitializeComponent();
        }

        private void btnSubmitAuth_Click(object sender, RoutedEventArgs e)
        {
            SubmitAuthClick?.Invoke(this, e);
        }

        private void btnChangePassword_Click(object sender, RoutedEventArgs e)
        {
            ChangePasswordClick?.Invoke(this, e);
        }

        private void btnSaveServerAuthCode_Click(object sender, RoutedEventArgs e)
        {
            SaveServerAuthCodeClick?.Invoke(this, e);
        }

        private void btnSignUp_Click(object sender, RoutedEventArgs e)
        {
            SignUpClick?.Invoke(this, e);
        }
    }
}
