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
    /// Interaction logic for personalStatsPage.xaml
    /// </summary>
    public partial class personalStatsPage : Page
    {
        public personalStatsPage()
        {
            InitializeComponent();

            //TODO: get the user statistics
            string msg = Communicator.getRequestCode<GetPersonalStatsResponse>() + "0000";
            var response = Communicator.sendRequest<string>(msg, false);
            GetPersonalStatsResponse stats = Desrializition.DeserializeResponse<GetPersonalStatsResponse>(response.msg);

            if (stats.status != (int)Codes.code_OK)
            {
                var error = Desrializition.DeserializeResponse<ErrorResponse>(response.msg);

                var mainWindow = (MainWindow)Application.Current.MainWindow;
                mainWindow.ErrorPopup.ShowMessage(error.message);
                return;
            }
            //order: avarge ans time, num of correct answers, total answers, player score, games played
            SetStats(
                int.Parse(stats.statistics[3]),
                int.Parse(stats.statistics[4]),
                int.Parse(stats.statistics[2]),
                int.Parse(stats.statistics[1]),
                double.Parse(stats.statistics[0]));
        }

        private void SetStats(int score, int gamesPlayed, int totalAnswers, int correctAnswers, double averageAnswerTime)
        {
            Score.Text = score.ToString();
            Games_Played.Text = gamesPlayed.ToString();
            Total_Answers.Text = totalAnswers.ToString();
            Correct_Answers.Text = correctAnswers.ToString();
            Wrong_Answers.Text = (totalAnswers - correctAnswers).ToString();
            Average_Answer_Time.Text = averageAnswerTime.ToString();
        }
    }
}
