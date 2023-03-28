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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private UiState uiState;
        public MainWindow()
        {
            InitializeComponent();

            uiState = new UiState();

            gridRoot.DataContext = uiState;
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
            uiState.SetCurrentPage(AuthClientPage.auth);
        }
    }
}
