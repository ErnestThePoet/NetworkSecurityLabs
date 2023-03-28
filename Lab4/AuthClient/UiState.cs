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
    enum AuthClientPage
    {
        auth,
        signUp,
        changePassword
    }
    class UiState : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler? PropertyChanged;

        private Visibility lblGoBackVisibility;
        private Visibility authWrapperVisibility;
        private Visibility signUpWrapperVisibility;
        private Visibility changePasswordWrapperVisibility;

        public UiState()
        {
            lblGoBackVisibility = Visibility.Hidden;
            authWrapperVisibility = Visibility.Visible;
            signUpWrapperVisibility = Visibility.Collapsed;
            changePasswordWrapperVisibility = Visibility.Collapsed;
        }

        protected void RaisePropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public void SetCurrentPage(AuthClientPage currentPage)
        {
            switch (currentPage)
            {
                case AuthClientPage.auth:
                    lblGoBackVisibility = Visibility.Hidden;
                    authWrapperVisibility = Visibility.Visible;
                    signUpWrapperVisibility = Visibility.Collapsed;
                    changePasswordWrapperVisibility = Visibility.Collapsed;
                    break;

                case AuthClientPage.changePassword:
                    lblGoBackVisibility = Visibility.Visible;
                    authWrapperVisibility = Visibility.Collapsed;
                    signUpWrapperVisibility = Visibility.Collapsed;
                    changePasswordWrapperVisibility = Visibility.Visible;
                    break;

                case AuthClientPage.signUp:
                    lblGoBackVisibility = Visibility.Visible;
                    authWrapperVisibility = Visibility.Collapsed;
                    signUpWrapperVisibility = Visibility.Visible;
                    changePasswordWrapperVisibility = Visibility.Collapsed;
                    break;
            }
        }

        public Visibility LblGoBackVisibility
        {
            get { return lblGoBackVisibility; }
            set
            {
                lblGoBackVisibility = value;
                RaisePropertyChanged();
            }
        }

        public Visibility AuthWrapperVisibility
        {
            get { return authWrapperVisibility; }
            set
            {
                authWrapperVisibility = value;
                RaisePropertyChanged();
            }
        }

        public Visibility SignUpWrapperVisibility
        {
            get { return signUpWrapperVisibility; }
            set
            {
                signUpWrapperVisibility = value;
                RaisePropertyChanged();
            }
        }

        public Visibility ChangePasswordWrapperVisibility
        {
            get { return changePasswordWrapperVisibility; }
            set
            {
                changePasswordWrapperVisibility = value;
                RaisePropertyChanged();
            }
        }
    }
}
