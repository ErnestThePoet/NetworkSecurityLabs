using System;
using System.Collections.Generic;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;

namespace AuthClient
{
    [ValueConversion(typeof(AuthClientPage), typeof(Visibility))]
    public class LblGoBackVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (AuthClientPage)value == AuthClientPage.Auth
                ? Visibility.Hidden
                : Visibility.Visible;
        }

        // Meaningless
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return AuthClientPage.Auth;
        }
    }

    [ValueConversion(typeof(AuthClientPage), typeof(Visibility))]
    public class AuthWrapperVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (AuthClientPage)value == AuthClientPage.Auth
                ? Visibility.Visible
                : Visibility.Collapsed;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return AuthClientPage.Auth;
        }
    }

    [ValueConversion(typeof(AuthClientPage), typeof(Visibility))]
    public class SignUpWrapperVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (AuthClientPage)value == AuthClientPage.SignUp
                ? Visibility.Visible
                : Visibility.Collapsed;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return AuthClientPage.SignUp;
        }
    }

    [ValueConversion(typeof(AuthClientPage), typeof(Visibility))]
    public class ChangePasswordWrapperVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (AuthClientPage)value == AuthClientPage.ChangePassword
                ? Visibility.Visible
                : Visibility.Collapsed;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return AuthClientPage.ChangePassword;
        }
    }

    [ValueConversion(typeof(ResultType), typeof(Brush))]
    public class ClientAuthCodeBrushConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (ResultType)value == ResultType.Initial
                ? new SolidColorBrush(Colors.Gray)
                : new SolidColorBrush(Colors.Blue);
        }

        // Meaningless
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return ResultType.Initial;
        }
    }

    [ValueConversion(typeof(ResultType), typeof(Brush))]
    public class AuthMessageBrushConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            switch ((ResultType)value)
            {
                case ResultType.Initial:
                    return new SolidColorBrush(Colors.Gray);
                case ResultType.Failure:
                    return new SolidColorBrush(Colors.Red);
                case ResultType.Success:
                    return new SolidColorBrush(Colors.Green);
                default:
                    return new SolidColorBrush(Colors.Black);
            }
        }

        // Meaningless
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return ResultType.Initial;
        }
    }

    [ValueConversion(typeof(ResultType), typeof(Visibility))]
    public class ServerAuthCodeVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (ResultType)value == ResultType.Success
                ? Visibility.Visible
                : Visibility.Hidden;
        }

        // Meaningless
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return ResultType.Initial;
        }
    }

    [ValueConversion(typeof(ResultType), typeof(Visibility))]
    public class FailureNoneResultVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (ResultType)value == ResultType.Failure
                ? Visibility.Visible
                : Visibility.Hidden;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (Visibility)value == Visibility.Visible
                ? ResultType.Failure
                : ResultType.None;
        }
    }

    [ValueConversion(typeof(ResultType), typeof(Brush))]
    public class FailureNoneResultBrushConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (ResultType)value == ResultType.Failure
                ? new SolidColorBrush(Colors.Red)
                : new SolidColorBrush(Colors.Green);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return ((SolidColorBrush)value).Color == Colors.Red
                ? ResultType.Failure
                : ResultType.None;
        }
    }

    [ValueConversion(typeof(ResultType), typeof(Visibility))]
    public class ResultVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (ResultType)value == ResultType.None
                ? Visibility.Hidden
                : Visibility.Visible;
        }

        // Meaningless
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return ResultType.None;
        }
    }

    [ValueConversion(typeof(ResultType), typeof(Brush))]
    public class ResultBrushConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            switch ((ResultType)value)
            {
                case ResultType.Success:
                    return new SolidColorBrush(Colors.Green);
                case ResultType.Failure:
                    return new SolidColorBrush(Colors.Red);
                case ResultType.Initial:
                    return new SolidColorBrush(Colors.Gray);
                default:
                    return new SolidColorBrush(Colors.Black);
            }
        }

        // Meaningless
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return ResultType.None;
        }
    }
}
