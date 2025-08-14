using client.communication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Media;
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
using WpfApp1;
using WpfApp1.pages;

namespace client.pages
{
    /// <summary>
    /// Interaction logic for menu.xaml
    /// </summary>
    public partial class menuPage : Page
    {
        public menuPage()
        {
            InitializeComponent();
            logout_button.Click += logout;
            joinroom.Click += Joinroom_Click;
            createroom.Click += Createroom_Click;
            statistics.Click += Statistics_Click;
            leaderboard.Click += Leaderboard_Click;

            MainWindow.setBackroundMusic("Sounds/main_menu_music.wav");
        }

        private void Leaderboard_Click(object sender, RoutedEventArgs e)
        {
            main_frame.Navigate(new leaderboardsPage());
            //click sound
            MainWindow.playSoundEffect("Sounds/click_sound.wav");
        }

        private void Statistics_Click(object sender, RoutedEventArgs e)
        {
            main_frame.Navigate(new personalStatsPage());
            //click sound
            MainWindow.playSoundEffect("Sounds/click_sound.wav");
        }

        private void Createroom_Click(object sender, RoutedEventArgs e)
        {
            main_frame.Navigate(new createRoomPage());
            //click sound
            MainWindow.playSoundEffect("Sounds/click_sound.wav");
        }

        private void Joinroom_Click(object sender, RoutedEventArgs e)
        {
            main_frame.Navigate(new joinRoomPage());
            //click sound
            MainWindow.playSoundEffect("Sounds/click_sound.wav");
        }


        private void logout(object sender, EventArgs e)
        {
            //click sound
            MainWindow.playSoundEffect("Sounds/click_sound.wav");

            var req = Communicator.getRequestCode<LogoutResponse>() + "0000";
            communication.Communicator.sendRequest<string>(req, false);

            var mainWindow = (MainWindow)Application.Current.MainWindow;
            mainWindow.main_Frame.Navigate(new loginPage());
        }
    }
}
