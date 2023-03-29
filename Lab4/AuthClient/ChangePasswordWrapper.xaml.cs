using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Principal;
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
using static AuthClient.SignUpWrapper;

namespace AuthClient
{
    /// <summary>
    /// Interaction logic for ChangePasswordWrapper.xaml
    /// </summary>
    public partial class ChangePasswordWrapper : UserControl
    {
        public class ChangePasswordClickEventArgs : EventArgs
        {
            public string PasswordOld { get; set; }
            public string PasswordNew { get; set; }
            public string PasswordNewConfirm { get; set; }

            public ChangePasswordClickEventArgs(
                string passwordOld, string passwordNew, string passwordNewConfirm)
            {
                PasswordOld = passwordOld;
                PasswordNew = passwordNew;
                PasswordNewConfirm = passwordNewConfirm;
            }
        }

        public event EventHandler<ChangePasswordClickEventArgs>? ChangePasswordClick;

        public static readonly DependencyProperty changePasswordResultProperty =
            DependencyProperty.Register("ChangePasswordResult", typeof(ResultType), typeof(ChangePasswordWrapper));

        public static readonly DependencyProperty changePasswordMessageProperty =
            DependencyProperty.Register("ChangePasswordMessage", typeof(string), typeof(ChangePasswordWrapper));

        public ResultType ChangePasswordResult
        {
            get { return (ResultType)GetValue(changePasswordResultProperty); }
            set { SetValue(changePasswordResultProperty, value); }
        }

        public string ChangePasswordMessage
        {
            get { return (string)GetValue(changePasswordMessageProperty); }
            set { SetValue(changePasswordMessageProperty, value); }
        }

        public ChangePasswordWrapper()
        {
            InitializeComponent();
            spRoot.DataContext = this;
        }

        public void ClearFields()
        {
            pbOldPassword.Clear();
            pbNewPassword.Clear();
            pbNewPasswordConfirm.Clear();
        }

        private void btnChangePassword_Click(object sender, RoutedEventArgs e)
        {
            ChangePasswordClick?.Invoke(
                this, 
                new(pbOldPassword.Password,
                    pbNewPassword.Password,
                    pbNewPasswordConfirm.Password));
        }
    }
}
