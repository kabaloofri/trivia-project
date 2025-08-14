using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WpfApp1.pages;
using client.pages;
using client.communication;
using System.Net.NetworkInformation;
using System.Media;

namespace WpfApp1;

/// <summary>
/// Interaction logic for MainWindow.xaml
/// </summary>
public partial class MainWindow
{
    static MediaPlayer player = new MediaPlayer();
    public MainWindow()
    {
        InitializeComponent();
        this.WindowState = WindowState.Maximized;

        main_Frame.Navigate(new loginPage());
        Communicator.start(); //maybe add setttings tab in the login to set the ip and port
    }
    
    public void NavigateToMenu()
    {
        Application.Current.Dispatcher.Invoke(() => //this is bc the thread dosent own the stuff in the ui, so this accesses the ui regardless
        {
            main_Frame.Navigate(new menuPage());
        });
    }

    public static void setBackroundMusic(string path)
    {
        player.Open(new Uri(path, UriKind.Relative));
        player.Volume = Settings.music_volume;

        //when media ends, play it agian to loop
        player.MediaEnded += (s, e) =>
        {
            player.Position = TimeSpan.Zero;
            player.Play();
        };

        //if failed to play sound, display fail
        player.MediaFailed += (s, e) =>
        {
            MessageBox.Show("error playing sound");
        };
        
        player.Play();
    }

    public static void playSoundEffect(string path)
    {
        MediaPlayer sound = new MediaPlayer();
        sound.Open(new Uri(path, UriKind.Relative));
        sound.Volume = Settings.sound_volume;

        sound.Play();
    }
    
    
}
