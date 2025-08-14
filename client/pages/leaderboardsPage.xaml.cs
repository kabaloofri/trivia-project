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
using System.Windows.Documents.Serialization;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WpfApp1;

namespace client.pages
{
    /// <summary>
    /// Interaction logic for leaderboardsPage.xaml
    /// </summary>
    public partial class leaderboardsPage : Page
    {
        public leaderboardsPage()
        {
            InitializeComponent();

            //get high scores;
            string req = Communicator.getRequestCode<GetHighScoreResponse>() + "0000";
            var response = Communicator.sendRequest<string>(req, false);
            GetHighScoreResponse leaderboards = Desrializition.DeserializeResponse<GetHighScoreResponse>(response.msg);
            
            if (leaderboards.status != (int)Codes.code_OK)
            {
                var error = Desrializition.DeserializeResponse<ErrorResponse>(response.msg);

                var mainWindow = (MainWindow)Application.Current.MainWindow;
                mainWindow.ErrorPopup.ShowMessage(error.message);
                return;
            }
            //format is name1,score1, name2, score2...
            var stats = new string[10];
            for (int i = 0; i < stats.Length; i++)
            {
                if (i < leaderboards.statistics.Length)
                {
                    stats[i] = leaderboards.statistics[i]; //transfer all stats to the stats list and if empty then stay empty
                }
            }


            SetLeaderboard(
                stats[0] + " - " + stats[1],
                stats[2] + " - " + stats[3],
                stats[4] + " - " + stats[5],
                stats[6] + " - " + stats[7],
                stats[8] + " - " + stats[9]);
        }

        private void SetLeaderboard(string score1, string score2, string score3, string score4, string score5)
        {
            p1.Text = score1;
            p2.Text = score2;
            p3.Text = score3;
            p4.Text = score4;
            p5.Text = score5;
        }

    }
}
