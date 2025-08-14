using client.communication;
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
using WpfApp1;


namespace client.pages
{

    /// <summary>
    /// Interaction logic for createRoom.xaml
    /// </summary>
    public partial class createRoomPage : Page
    {
        public createRoomPage()
        {
            InitializeComponent();
            createRoom.Click += CreateRoom_Click;
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            MainWindow.playSoundEffect("Sounds/click_sound.wav");

            if (roomName_textBox.Text == "")
            {
                var mainWindow = (MainWindow)Application.Current.MainWindow;
                mainWindow.ErrorPopup.ShowMessage("Room name is Empty! please add a name to your room.");
                return;
            }

            CreateRoomRequest CR = new CreateRoomRequest(
            roomName_textBox.Text,
            (int)numOfPlayers_slider.Value,
            (int)NumOfQuestions_slider.Value,
            (int)timePerQuestion_slider.Value);
            var res = Communicator.sendRequest<CreateRoomRequest>(CR);
            

            if (res.code != (int)Codes.code_error) //if room creation was successful
            {
                var response = Desrializition.DeserializeResponse<CreateRoomResponse>(res.msg);

                var window = (MainWindow)Application.Current.MainWindow;
                window.main_Frame.Navigate(new WaitingRoom(response.roomId));
            }
            else
            {
                var error = Desrializition.DeserializeResponse<ErrorResponse>(res.msg);

                var mainWindow = (MainWindow)Application.Current.MainWindow;
                mainWindow.ErrorPopup.ShowMessage(error.message);
            }
        }
        private void numOfQuestion_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            int val = Convert.ToInt32(e.NewValue);
            string msg = String.Format("{0}", val);
            this.NumOfQuestion.Text = msg;
        }
        private void timePerQuestion_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            int val = Convert.ToInt32(e.NewValue);
            string msg = String.Format("{0}", val);
            this.timePerQuestion.Text = msg;
        }
        private void numOfPlayers_Changed(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            int val = Convert.ToInt32(e.NewValue);
            string msg = String.Format("{0}", val);
            this.numOfPlayers.Text = msg;
        }


    }


}

