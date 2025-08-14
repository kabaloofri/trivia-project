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
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WpfApp1;

namespace client.user_conrtols
{
    /// <summary>
    /// Interaction logic for popupError.xaml
    /// </summary>
    public partial class popupError : UserControl
    {
        public popupError()
        {
            InitializeComponent();

        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            Storyboard sb = (Storyboard)this.Resources["SlideUpAnimation"];
            sb.Begin();
        }

        public void ShowMessage(string message)
        {
            Application.Current.Dispatcher.Invoke(() => //this is bc the thread dosent own the stuff in the ui, so this accesses the ui regardless
            {
                //play sound
                MainWindow.playSoundEffect("Sounds/error_sound.wav");

                //errorMsg.Text = msg; //use &#x0a; to resemble a newline (\n)
                errorMsg.Text = message;
                this.Visibility = Visibility.Visible;

                Storyboard sb = (Storyboard)this.Resources["popupAnimation"];
                sb.Begin();

                // Optionally hide after a few seconds
                Task.Delay(3000).ContinueWith(_ =>
                {
                    Dispatcher.Invoke(() =>
                    {
                        this.Visibility = Visibility.Collapsed;
                    });
                });
            });
        }
    }
}
