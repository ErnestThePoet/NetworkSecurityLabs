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
                string account, string password, string passwordConfirm)
            {
                Account = account;
                Password = password;
                PasswordConfirm = passwordConfirm;
            }
        }

        public event EventHandler<SignUpClickEventArgs>? SignUpClick;

        public static readonly DependencyProperty signupResultProperty =
            DependencyProperty.Register("SignUpResult", typeof(ResultType), typeof(SignUpWrapper));

        public static readonly DependencyProperty signUpMessageProperty =
            DependencyProperty.Register("SignUpMessage", typeof(string), typeof(SignUpWrapper));

        public string SignUpMessage
        {
            get { return (string)GetValue(signUpMessageProperty); }
            set { SetValue(signUpMessageProperty, value); }
        }

        public ResultType SignUpResult
        {
            get { return (ResultType)GetValue(signupResultProperty); }
            set { SetValue(signupResultProperty, value); }
        }

        public SignUpWrapper()
        {
            InitializeComponent();
            spRoot.DataContext = this;
        }

        private void btnSignUp_Click(object sender, RoutedEventArgs e)
        {
            SignUpClick?.Invoke(
                this, new SignUpClickEventArgs(
                    tbAccount.Text, pbPassword.Password, pbPasswordConfirm.Password));
        }
    }
}
