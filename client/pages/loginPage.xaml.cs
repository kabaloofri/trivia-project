using client.communication;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Xml.Linq;
using WpfApp1;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace client.pages
{
    /// <summary>
    /// Interaction logic for loginPage.xaml
    /// </summary>
    public partial class loginPage : Page
    {
        private bool isLogginIn = true;
        bool hasClearedUsername = false;
        bool hasClearedPassword = false;
        public loginPage()
        {
            InitializeComponent();
            username_box.GotFocus += RemoveText;
            password_box.GotFocus += RemovePassword;
            email_box.GotFocus += RemoveText;

            password_box.MouseDown += (object sender, MouseButtonEventArgs e) => { password_box.Focus(); };

            signup_button.Click += ShowSignUp;
            join_button.Click += Join;
            

            connectButton.Click += setIpAndPort;
            settings_button.Click += openSettingsPanel;

            email_border.Visibility = Visibility.Hidden;
        }


        void Join(object sender, EventArgs e)
        {
            //play button click sound
            MainWindow.playSoundEffect("Sounds/click_sound.wav");

            string username = username_box.Text.ToString();
            string password = password_box.Password.ToString();

            if (username == "" || password == "" || !hasClearedUsername || !hasClearedPassword)
            {
                //user did not change the stuff
                var mainWindow = (MainWindow)Application.Current.MainWindow;
                mainWindow.ErrorPopup.ShowMessage("Please set username and password!");

                return;
            }

            Response res;
            if (isLogginIn)
            {
                LoginRequest LR = new LoginRequest(username, password);
                //send login request
                res = Communicator.sendRequest<LoginRequest>(LR);
            }
            else
            {
                string email = email_box.Text.ToString();
                SignupRequest SR = new SignupRequest(username, password, email);
                //send sigup request
                res = Communicator.sendRequest<SignupRequest>(SR);
            }


            if (res.code == (int)Codes.code_error)
            {
                //error! get error request
                ErrorResponse error = Desrializition.DeserializeResponse<ErrorResponse>(res.msg);

                var mainWindow = (MainWindow)Application.Current.MainWindow;
                mainWindow.ErrorPopup.ShowMessage(error.message);

                return;
            }
            //deserialize res
            LoginResponse status = Desrializition.DeserializeResponse<LoginResponse>(res.msg);

            Communicator.username = username; //set username

            //play login sound
            MainWindow.playSoundEffect("Sounds/login_success_sound.wav");

            //move to menu
            var window = (MainWindow)Application.Current.MainWindow;
            window.NavigateToMenu();

        }


        void ShowSignUp(object sender, EventArgs e)
        {
            email_border.Visibility = Visibility.Visible;
            signup_button.Content = "Login";
            signup_button.Click -= ShowSignUp;
            signup_button.Click += SwitchToSignup;
            isLogginIn = false;

            MainWindow.playSoundEffect("Sounds/click_sound.wav");
        }

        private void SwitchToSignup(object sender, RoutedEventArgs e)
        {
            email_border.Visibility = Visibility.Hidden;
            signup_button.Content = "Sign up";
            signup_button.Click -= SwitchToSignup;
            signup_button.Click += ShowSignUp;
            isLogginIn = true;

            //play sound
            MainWindow.playSoundEffect("Sounds/click_sound.wav");
        }

        void RemoveText(object sender, EventArgs e)
        {
            TextBox box = (TextBox)sender;
            box.Text = "";
            box.GotFocus -= RemoveText; //do this only one time
            hasClearedUsername = true;
        }

        void RemovePassword(object sender, EventArgs e)
        {
            password_box2.Text = "";
            hasClearedPassword = true;
        }



        //change ip and port

        void openSettingsPanel(object sendert, EventArgs e)
        {
            if (settings_panel.Visibility == Visibility.Visible)
            {
                return;
            }

            port_box.Text = Settings.port.ToString();
            ip_box.Text = Settings.ip;

            //open the settings panel

            settings_panel.Visibility = Visibility.Visible;

            TranslateTransform transform = new TranslateTransform();
            settings_panel.RenderTransform = transform;

            DoubleAnimation slideDown = new DoubleAnimation()
            {
                From = -settings_panel.Height,
                To = 0,
                Duration = TimeSpan.FromMilliseconds(500),
                EasingFunction = new QuadraticEase() { EasingMode = EasingMode.EaseOut }
            };

            transform.BeginAnimation(TranslateTransform.YProperty, slideDown);
        }

        void setIpAndPort(object sendert, EventArgs e)
        {
            bool isConnected = Communicator.stream != null;
            if (isConnected)
            {
                var mainWindow = (MainWindow)Application.Current.MainWindow;
                mainWindow.ErrorPopup.ShowMessage("already connected to server!");
            }
            else
            {
                Settings.port = int.Parse(port_box.Text);
                Settings.ip = ip_box.Text;
            }


            //close panel

            TranslateTransform transform = new TranslateTransform();
            settings_panel.RenderTransform = transform;

            DoubleAnimation slideDown = new DoubleAnimation()
            {
                From = 0,
                To = -settings_panel.Height,
                Duration = TimeSpan.FromMilliseconds(500),
                EasingFunction = new QuadraticEase() { EasingMode = EasingMode.EaseOut }
            };

            transform.BeginAnimation(TranslateTransform.YProperty, slideDown);
            
            if (!isConnected)
            Communicator.start();
        }
    }
}
