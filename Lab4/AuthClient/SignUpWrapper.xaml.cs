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
    /// Interaction logic for SignUpWrapper.xaml
    /// </summary>
    public partial class SignUpWrapper : UserControl
    {
        public class SignUpClickEventArgs : EventArgs
        {
            public string Account { get; set; }
            public string Password { get; set; }
            public string PasswordConfirm { get; set; }

            public SignUpClickEventArgs(
                string account, string password,string passwordConfirm)
            {
                Account = account;
                Password = password;
                PasswordConfirm = passwordConfirm;
            }
        }

        public event EventHandler<SignUpClickEventArgs>? SignUpClick;

        public ResultType Result
        {
            get { return (ResultType)GetValue(resultProperty); }
            set { SetValue(resultProperty, value); }
        }

        public static readonly DependencyProperty resultProperty =
            DependencyProperty.Register("Result", typeof(ResultType), typeof(SignUpWrapper));

        public string Message
        {
            get { return (string)GetValue(messageProperty); }
            set { SetValue(messageProperty, value); }
        }

        public static readonly DependencyProperty messageProperty =
            DependencyProperty.Register("Message", typeof(string), typeof(SignUpWrapper));

        public SignUpWrapper()
        {
            InitializeComponent();
            spRoot.DataContext = this;
        }

        private void btnSignUp_Click(object sender, RoutedEventArgs e)
        {
            SignUpClick?.Invoke(
                this, new SignUpClickEventArgs(
                    tbAccount.Text,pbPassword.Password,pbPasswordConfirm.Password));
        }
    }
}
