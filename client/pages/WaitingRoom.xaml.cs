using client.communication;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;
using System.Windows.Media;
using WpfApp1;

namespace client.pages
{
    /// <summary>
    /// Interaction logic for WaitingRoom.xaml
    /// </summary>
    public partial class WaitingRoom : Page
    {
        private bool isAdmin;
        private int roomID;
        private int numOfQuestions;
        private int timePerQuestion;
        Thread updateRoom;

        private bool isOpen = true;
        public WaitingRoom(int RoomID)
        {
            InitializeComponent();

            this.roomID = RoomID; //set roomID

            //get room update from server (with room member/admin handler)
            var state = GetRoomStatus(roomID);
            numOfQuestions = state.querstionsCount;
            timePerQuestion = state.answerTimeout;

            //first player in list is admin
            if (Communicator.username != state.players[0])
            {
                isAdmin = false;
                Start_Game_button.Visibility = Visibility.Collapsed; //hide start game button
                settings_button.Visibility = Visibility.Collapsed; // hide the settings button
            }
            else
            {
                isAdmin = true;
                Start_Game_button.Click += StartGame;

                leave_game_button.Content = "Close Room";

                settings_button.Click += openSettings; //connect the settings buttons
                this.apply_button.Click += applySettings;
                question_count_slider.ValueChanged += questionCountChanged;
                time_Per_Question_slider.ValueChanged += timePerQuestionChanged;
            }

            leave_game_button.Click += LeaveRoom;

            this.Unloaded += closePage;

            updateRoom = new Thread(UpdateRoomThread);
            updateRoom.IsBackground = true;
            updateRoom.SetApartmentState(ApartmentState.STA);
            updateRoom.Start(); //make new backround thread to check for room status and players every 3 seconds
        }

        private void UpdateRoomThread()
        {
            while (isOpen) //while page is open
            {
                Application.Current.Dispatcher.Invoke(() => //this is bc the thread dosent own the stuff in the ui, so this accesses the ui regardless
                {
                    var state = GetRoomStatus(roomID); // set the players in the room

                    //if waiting for players, load players
                    if (state.status == (int)RoomStates.state_waitingForPlayers)
                    {
                        SetPlayers(state.players);// set players, question count and time per question
                        question_count.Text = state.querstionsCount.ToString();
                        time_count.Text = state.answerTimeout.ToString();
                    }
                    else if (state.status == (int)RoomStates.state_begginingGame && !isAdmin)
                    {
                        //TODO: maybe start countdowns

                        var window = (MainWindow)Application.Current.MainWindow;
                        isOpen = false;
                        window.main_Frame.Navigate(new GameRoom(timePerQuestion, numOfQuestions, roomID));
                    }
                    else
                    {

                        // send to the server to leave the room
                        var req = Communicator.getRequestCode<LeaveRoomResponse>() + "0000";
                        var res = Communicator.sendRequest<string>(req, false);
                        var mainWindow = (MainWindow)Application.Current.MainWindow;
                        mainWindow.ErrorPopup.ShowMessage("Room admin has closed the room!");

                        isOpen = false;
                        mainWindow.NavigateToMenu();
                    }

                });
                Thread.Sleep(2000); //refresh rooms every 2 seconds with new content from server
            }
        }

        private void closePage(object sender, EventArgs e)
        {
            isOpen = false;
        }

        private GetRoomStateResponse GetRoomStatus(int roomID)
        {
            string req = Communicator.getRequestCode<GetRoomStateResponse>() + "0000";

            var res = Communicator.sendRequest<string>(req, false);

            var response = Desrializition.DeserializeResponse<GetRoomStateResponse>(res.msg); //get players in room


            return response;
        }

        private void StartGame(object sender, RoutedEventArgs e)
        {
            //stop the set players thread
            isOpen = false;
            updateRoom.Join();

            var req = Communicator.getRequestCode<StartGameResponse>() + "0000";
            var res = Communicator.sendRequest<string>(req, false);
            var response = Desrializition.DeserializeResponse<StartGameResponse>(res.msg);

            //TODO: maybe start countdowns

            var window = (MainWindow)Application.Current.MainWindow;
            window.main_Frame.Navigate(new GameRoom(timePerQuestion, numOfQuestions, roomID)); //TODO: navigate to game
        }
        public void SetPlayers(string[] players)
        {
            this.admin_name_textBlock.Text = players[0];

            this.stack_panel_members.Children.Clear(); //clear all members

            for (int i = 1; i < players.Length; i++)
            {
                var member = new TextBlock();
                member.Text = players[i];
                member.FontSize = 40;
                member.TextAlignment = TextAlignment.Center;
                //member.Margin = new Thickness(0, 0, 0, 0);

                stack_panel_members.Children.Add(member); // add the new members back
            }
        }

        public void LeaveRoom(object sender, EventArgs e)
        {
            //stop the thread
            isOpen = false;
            updateRoom.Join();

            if (isAdmin)
            {
                //close room
                var req = Communicator.getRequestCode<CloseRoomResponse>() + "0000";
                var res = Communicator.sendRequest<string>(req, false);
            }
            else
            {
                //leave room
                var req = Communicator.getRequestCode<LeaveRoomResponse>() + "0000";
                var res = Communicator.sendRequest<string>(req, false);
                var response = Desrializition.DeserializeResponse<LeaveRoomResponse>(res.msg);
            }

            var window = (MainWindow)Application.Current.MainWindow;
            window.NavigateToMenu(); //navigate back to main menu
        }





        // room settings

        void openSettings(object sender, EventArgs e)
        {
            //open the settings panel

            if (settings_panel.Visibility == Visibility.Visible)
            {
                return;
            }

            settings_panel.Visibility = Visibility.Visible;

            question_count_value.Text = numOfQuestions.ToString();
            question_time_value.Text = timePerQuestion.ToString();

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

        void applySettings(object sender, EventArgs e)
        {
            timePerQuestion = (int)time_Per_Question_slider.Value;
            numOfQuestions = (int)question_count_slider.Value;

            var request = new ChangeRoomSettingsRequest();
            request.questionCount = numOfQuestions;
            request.answerTimeout = timePerQuestion;

            var answer = Communicator.sendRequest<ChangeRoomSettingsRequest>(request);
            
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
            settings_panel.Visibility = Visibility.Hidden;
        }

        
        void timePerQuestionChanged(object sender, EventArgs e)
        {
            question_time_value.Text = time_Per_Question_slider.Value.ToString();
        }
        void questionCountChanged(object sender, EventArgs e)
        {
            question_count_value.Text = question_count_slider.Value.ToString();
        }
    }
}
