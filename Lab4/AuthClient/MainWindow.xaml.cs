using Microsoft.Win32;
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
using static AuthClient.AuthWrapper;
using static AuthClient.SignUpWrapper;

namespace AuthClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private UiManager uiManager;
        private AuthManager authManager;
        public MainWindow()
        {
            InitializeComponent();

            uiManager = new UiManager();
            authManager = new AuthManager();

            gridRoot.DataContext = uiManager;
        }

        private void TitleBar_CloseClick(object sender, EventArgs e)
        {
            Close();
        }

        private void TitleBar_TitlebarDrag(object sender, EventArgs e)
        {
            DragMove();
        }

        private void lblGoBack_MouseUp(object sender, MouseButtonEventArgs e)
        {
            uiManager.CurrentPage = AuthClientPage.Auth;
        }

        private async void ucAuthWrapper_SubmitAuthClick(object sender, SubmitAuthClickEventArgs e)
        {
            var (clientAuthCodeResult, clientAuthCodeBase64,
                result, message) = await authManager.SubmitAuthAsync(
                e.Account, e.Password);

            uiManager.ClientAuthCodeResult = clientAuthCodeResult;

            if (clientAuthCodeResult == ResultType.Success)
            {
                uiManager.ClientAuthCodeBase64 = clientAuthCodeBase64!;
            }

            if (result == ResultType.Success)
            {
                uiManager.ServerAuthCodeBase64 = Convert.ToBase64String(
                    authManager.ServerAuthCode);
            }

            uiManager.AuthResult = result;
            uiManager.AuthMessage = message;
        }

        private void ucAuthWrapper_SignUpClick(object sender, EventArgs e)
        {
            uiManager.CurrentPage = AuthClientPage.SignUp;
        }

        private void ucAuthWrapper_ChangePasswordClick(object sender, EventArgs e)
        {
            if (!authManager.IsAuthorized)
            {
                MessageBox.Show(
                    "请先认证",
                    "错误",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);
            }
            else
            {
                uiManager.CurrentPage = AuthClientPage.ChangePassword;
            }
        }

        private void ucAuthWrapper_SaveServerAuthCodeClick(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.FileName = "server_auth_code.txt";
            saveFileDialog.Filter = "Text (*.txt) | *.txt";
            if (saveFileDialog.ShowDialog()??false)
            {
                authManager.SaveServerAuthCode(saveFileDialog.FileName);
            }
        }

        private async void ucSignUpWrapper_SignUpClick(object sender, SignUpClickEventArgs e)
        {
            uiManager.SignUpResult = ResultType.None;

            var (result, message) = await authManager.CreateUserAsync(
                e.Account, e.Password, e.PasswordConfirm);

            uiManager.SignUpResult = result;
            uiManager.SignUpMessage = message;
        }
    }
}
