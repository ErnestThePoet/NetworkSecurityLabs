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
        public class SubmitAuthClickEventArgs : EventArgs
        {
            public string Account { get; set; }
            public string Password { get; set; }

            public SubmitAuthClickEventArgs(string account, string password)
            {
                Account = account;
                Password = password;
            }
        }

        public event EventHandler<SubmitAuthClickEventArgs>? SubmitAuthClick;
        public event EventHandler? ChangePasswordClick;
        public event EventHandler? SaveServerAuthCodeClick;
        public event EventHandler? SignUpClick;

        public static readonly DependencyProperty clientAuthCodeResultProperty =
            DependencyProperty.Register("ClientAuthCodeResult", typeof(ResultType), typeof(AuthWrapper));

        public static readonly DependencyProperty clientAuthCodeBase64Property =
            DependencyProperty.Register("ClientAuthCodeBase64", typeof(string), typeof(AuthWrapper));

        public static readonly DependencyProperty authResultProperty =
            DependencyProperty.Register("AuthResult", typeof(ResultType), typeof(AuthWrapper));

        public static readonly DependencyProperty authMessageProperty =
            DependencyProperty.Register("AuthMessage", typeof(string), typeof(AuthWrapper));

        public static readonly DependencyProperty serverAuthCodeBase64Property =
            DependencyProperty.Register("ServerAuthCodeBase64", typeof(string), typeof(AuthWrapper));

        public ResultType ClientAuthCodeResult
        {
            get { return (ResultType)GetValue(clientAuthCodeResultProperty); }
            set { SetValue(clientAuthCodeResultProperty, value); }
        }

        public string ClientAuthCodeBase64
        {
            get { return (string)GetValue(clientAuthCodeBase64Property); }
            set { SetValue(clientAuthCodeBase64Property, value); }
        }

        public ResultType AuthResult
        {
            get { return (ResultType)GetValue(authResultProperty); }
            set { SetValue(authResultProperty, value); }
        }

        public string AuthMessage
        {
            get { return (string)GetValue(authMessageProperty); }
            set { SetValue(authMessageProperty, value); }
        }

        public string ServerAuthCodeBase64
        {
            get { return (string)GetValue(serverAuthCodeBase64Property); }
            set { SetValue(serverAuthCodeBase64Property, value); }
        }

        public AuthWrapper()
        {
            InitializeComponent();
        }

        private void btnSubmitAuth_Click(object sender, RoutedEventArgs e)
        {
            SubmitAuthClick?.Invoke(
                this, 
                new (tbAccount.Text,pbPassword.Password));
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
