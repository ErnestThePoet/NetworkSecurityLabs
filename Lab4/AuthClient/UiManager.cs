using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace AuthClient
{
    public enum AuthClientPage
    {
        Auth,
        SignUp,
        ChangePassword
    }

    public enum ResultType
    {
        None,
        Initial,
        Success,
        Failure
    }

    class UiManager : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler? PropertyChanged;

        private AuthClientPage currentPage;

        private ResultType clientAuthCodeResult;
        private ResultType authResult;
        private ResultType signUpResult;
        private ResultType changePasswordResult;
        private string clientAuthCodeBase64;
        private string serverAuthCodeBase64;
        private string authMessage;
        private string signUpMessage;
        private string changePasswordMessage;

        public UiManager()
        {
            currentPage = AuthClientPage.Auth;

            clientAuthCodeResult = ResultType.Initial;
            authResult = ResultType.Initial;
            signUpResult = ResultType.None;
            changePasswordResult = ResultType.None;

            clientAuthCodeBase64 = "未生成";
            serverAuthCodeBase64 = string.Empty;
            authMessage = "未认证";
            signUpMessage = string.Empty;
            changePasswordMessage = string.Empty;
        }

        protected void RaisePropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public AuthClientPage CurrentPage
        {
            get { return currentPage; }
            set
            {
                currentPage = value;
                RaisePropertyChanged();
            }
        }

        public ResultType ClientAuthCodeResult
        {
            get { return clientAuthCodeResult; }
            set
            {
                clientAuthCodeResult = value;
                RaisePropertyChanged();
            }
        }

        public ResultType AuthResult
        {
            get { return authResult; }
            set
            {
                authResult = value;
                RaisePropertyChanged();
            }
        }

        public ResultType SignUpResult
        {
            get { return signUpResult; }
            set
            {
                signUpResult = value;
                RaisePropertyChanged();
            }
        }

        public ResultType ChangePasswordResult
        {
            get { return changePasswordResult; }
            set
            {
                changePasswordResult = value;
                RaisePropertyChanged();
            }
        }

        public string ClientAuthCodeBase64
        {
            get { return clientAuthCodeBase64; }
            set
            {
                clientAuthCodeBase64 = value;
                RaisePropertyChanged();
            }
        }

        public string ServerAuthCodeBase64
        {
            get { return serverAuthCodeBase64; }
            set
            {
                serverAuthCodeBase64 = value;
                RaisePropertyChanged();
            }
        }

        public string AuthMessage
        {
            get { return authMessage; }
            set
            {
                authMessage = value;
                RaisePropertyChanged();
            }
        }

        public string SignUpMessage
        {
            get { return signUpMessage; }
            set
            {
                signUpMessage = value;
                RaisePropertyChanged();
            }
        }

        public string ChangePasswordMessage
        {
            get { return changePasswordMessage; }
            set
            {
                changePasswordMessage = value;
                RaisePropertyChanged();
            }
        }
    }
}
