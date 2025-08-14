using client.communication;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace client.pages
{
    /// <summary>
    /// Interaction logic for afterGameScreen.xaml
    /// </summary>
    public partial class afterGameScreen : Page
    {
        int roomId;
        int navigateToMenuPage;
        public afterGameScreen(int roomid)
        {
            this.roomId = roomid;
            InitializeComponent();
            OUT.Click += OUT_Click;
            string req = Communicator.getRequestCode<GetGameResultsResponse>() + "0000";
            var res = Communicator.sendRequest<string>(req, false);

            GetGameResultsResponse response = Desrializition.DeserializeResponse<GetGameResultsResponse>(res.msg);

            //navigate to menu?
            navigateToMenuPage = response.status;

            //sort the users by they score
            Array.Sort(response.results, delegate (PlayerResults x, PlayerResults y) { return x.score.CompareTo(y.score); });

            //find the user place
            for (int i = 0; response.results.Length > i; i++)
            {
                if (response.results[i].username == communication.Communicator.username)
                {
                    place_text.Text += (i + 1).ToString();
                }
            }

            //write the results of all the users
            var s = new string[response.results.Length * 5];
            for (int j = 0; response.results.Length > j; j++)
            {
                s[0 + j * 5] = response.results[j].username;
                s[1 + j * 5] = "" + response.results[j].score;
                s[2 + j * 5] = "" + response.results[j].correctAnswerCount;
                s[3 + j * 5] = "" + response.results[j].wrongAnswerCount;
                s[4 + j * 5] = "" + response.results[j].averageAnswerTime;
            }



            setAfterGameStats(s);
        }

        private void OUT_Click(object sender, RoutedEventArgs e)
        {
            if(Convert.ToBoolean(navigateToMenuPage))
            {
                main_frame.Navigate(new menuPage());
            }
            else
            {
                main_frame.Navigate(new WaitingRoom(roomId));
            }
        }


        //stats should come in- username1, score1, correct1, wrong1, averageTime1, username2, score2...sorted by score
        void setAfterGameStats(string[] stats)
        {


            for (int i = 0; i < stats.Length; i += 5)
            {
                Brush color = Brushes.White;
                int fontSize = 25;

                var text = CreateText(stats[i]); //username

                if (text.Text == Communicator.username) //if current stat is the users stats
                {
                    color = Brushes.MediumBlue; //set the color to light blue and bigger font size
                    fontSize = 35;
                }

                text.Foreground = color;
                Usernames.Children.Add(text);
                text.FontSize = fontSize;

                
                text = CreateText(stats[i + 1]); //score
                text.Foreground = color;
                scores.Children.Add(text);
                text.FontSize = fontSize;


                text = CreateText(stats[i + 2]); //correct answers
                text.Foreground = color;
                correct_answers.Children.Add(text);
                text.FontSize = fontSize;


                text = CreateText(stats[i + 3]); //wrong answers
                text.Foreground = color;
                wrong_answers.Children.Add(text);
                text.FontSize = fontSize;


                text = CreateText(stats[i + 4]); //average answer time
                text.Foreground = color;
                average_time.Children.Add(text);
                text.FontSize = fontSize;
            }
        }

        TextBlock CreateText(string text)
        {
            TextBlock textBlock = new TextBlock();

            textBlock.Foreground = Brushes.White;
            textBlock.FontSize = 30;

            textBlock.HorizontalAlignment = HorizontalAlignment.Center;
            textBlock.VerticalAlignment = VerticalAlignment.Center;

            textBlock.Text = text;
            return textBlock;
        }
    }
}
