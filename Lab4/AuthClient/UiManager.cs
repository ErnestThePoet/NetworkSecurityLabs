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

        private ResultType authResult;
        private ResultType signUpResult;
        private ResultType changePasswordResult;
        private string authMessage;
        private string signUpMessage;
        private string changePasswordMessage;

        public UiManager()
        {
            currentPage = AuthClientPage.Auth;

            authResult = ResultType.Initial;
            signUpResult = ResultType.None;
            changePasswordResult = ResultType.None;

            authMessage = "未认证";
            signUpMessage = "";
            changePasswordMessage = "";
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
