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

        private void ucAuthWrapper_SubmitAuthClick(object sender, EventArgs e)
        {

        }

        private void ucAuthWrapper_SignUpClick(object sender, EventArgs e)
        {
            uiManager.CurrentPage = AuthClientPage.SignUp;
        }

        private void ucAuthWrapper_ChangePasswordClick(object sender, EventArgs e)
        {

        }

        private void ucAuthWrapper_SaveServerAuthCodeClick(object sender, EventArgs e)
        {

        }

        private async void ucSignUpWrapper_SignUpClick(object sender, SignUpClickEventArgs e)
        {
            uiManager.SignUpResult = ResultType.None;

            var (result, message) = await authManager.CreateUser(
                e.Account, e.Password, e.PasswordConfirm);

            uiManager.SignUpResult = result;
            uiManager.SignUpMessage = message;
        }
    }
}
