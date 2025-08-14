using client.communication;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Threading;
using WpfApp1;

namespace client.pages
{
    /// <summary>
    /// Interaction logic for GameRoom.xaml
    /// </summary>
    public partial class GameRoom : Page
    {
        //define varaibles
        Dictionary<int, Button> answers = new Dictionary<int, Button>();
        DispatcherTimer timer;
        DispatcherTimer colorTimer;
        private float elapsedTime;
        float timePerQuestion;
        int questionCount;
        int currentQuestion = 0;
        int userAnswerIndex;
        int correctAnswerIndex;
        int roomId;

        public GameRoom(int timePerQuestion, int numOfQuestions, int roomid)
        {
            //initialize values
            this.timePerQuestion = timePerQuestion;
            questionCount = numOfQuestions;
            roomId = roomid;

            //init the question time dispatcher
            timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(0.1);
            timer.Tick += Question_Timer;

            //init the time for change colors
            colorTimer = new DispatcherTimer();
            colorTimer.Interval = TimeSpan.FromSeconds(2);
            colorTimer.Tick += Color_Timer;

            InitializeComponent();

            //clicks
            b1.Click += B1_Click;
            b2.Click += B2_Click;
            b3.Click += B3_Click;
            b4.Click += B4_Click;
            leavegame.Click += Leavegame_Click;

            getQuestion();
        }



        private void getQuestion()
        {
            //send request
            var req = Communicator.getRequestCode<GetQuestionRequest>() + "0000";
            var RS = Communicator.sendRequest<string>(req, false);


            if (RS.code == (int)communication.Codes.code_getQuestion)
            {
                GetQuestionResponse Question = Desrializition.DeserializeResponse<GetQuestionResponse>(RS.msg);

                var q0 = Question.answers.ElementAt(0);
                var q1 = Question.answers.ElementAt(1);
                var q2 = Question.answers.ElementAt(2);
                var q3 = Question.answers.ElementAt(3);
                //change the answers
                b1.Content = WebUtility.HtmlDecode(q0.Value);
                b2.Content = WebUtility.HtmlDecode(q1.Value);
                b3.Content = WebUtility.HtmlDecode(q2.Value);
                b4.Content = WebUtility.HtmlDecode(q3.Value); //parse the data thru a html decoder to avoid &quot; in the question


                //change the id of the answers
                if (answers.Count() == 0)
                {
                    answers.Add(int.Parse(q0.Key), b1);
                    answers.Add(int.Parse(q1.Key), b2);
                    answers.Add(int.Parse(q2.Key), b3);
                    answers.Add(int.Parse(q3.Key), b4);
                }
                else
                {
                    answers[int.Parse(q0.Key)] = b1;
                    answers[int.Parse(q1.Key)] = b2;
                    answers[int.Parse(q2.Key)] = b3;
                    answers[int.Parse(q3.Key)] = b4;
                }

                //change the question
                p1.Text = WebUtility.HtmlDecode(Question.question);

                //change the question num
                currentQuestions.Text = (currentQuestion + 1) + "/" + (questionCount);
                currentQuestion++;

                elapsedTime = timePerQuestion;
                timer.Start();
            }
            else
            {
                var error = Desrializition.DeserializeResponse<ErrorResponse>(RS.msg);

                var mainWindow = (MainWindow)Application.Current.MainWindow;
                mainWindow.ErrorPopup.ShowMessage(error.message);
            }
        }
        private void Leavegame_Click(object sender, RoutedEventArgs e)
        {
            LeaveGameRequest LGR;
            LGR.status = (int)Codes.code_OK;
            communication.Communicator.sendRequest<LeaveGameRequest>(LGR);
            main_frame.Navigate(new menuPage());
        }





        private void Question_Timer(object sender, EventArgs e)
        {
            elapsedTime -= 0.1f;

            if (elapsedTime <= 0)
            {
                sendAnswer(answers.ElementAt(0).Key);
                timer.Stop();
                return;
            }

            if (elapsedTime < 10)
                TimerLabel.Text = "00:0" + ((int)elapsedTime).ToString();

            else
                TimerLabel.Text = "00:" + ((int)elapsedTime).ToString();

        }

        //return the answers to normal color
        private void Color_Timer(object sender, EventArgs e)
        {
            colorTimer.Stop();
            answers[userAnswerIndex].Background = Brushes.Black;
            answers[correctAnswerIndex].Background = Brushes.Black;
            if (currentQuestion == questionCount)
            {
                main_frame.Navigate(new afterGameScreen(roomId));
            }

            else
            {
                getQuestion();
            }
        }
         
        public void sendAnswer(int choice)
        {
            SubmitAnswerRequest request = new SubmitAnswerRequest();
            request.AnswerID = choice;
            request.answerTime = timePerQuestion - elapsedTime; 

            Response RS = communication.Communicator.sendRequest<SubmitAnswerRequest>(request);
            SubmitAnswerResponse A = Desrializition.DeserializeResponse<SubmitAnswerResponse>(RS.msg);

            //change colors of buttons
            if (choice != A.correctAnswerId)
            {
                answers[choice].Background = Brushes.Red;
            }
            answers[(int)A.correctAnswerId].Background = Brushes.Green;

            //stop the other dispatcher
            timer.Stop();

            //timer for change color
            correctAnswerIndex = (int)A.correctAnswerId;
            this.userAnswerIndex = choice;
            colorTimer.Start();



        }


        private void B1_Click(object sender, RoutedEventArgs e)
        {
            sendAnswer(0);

        }
        private void B2_Click(object sender, RoutedEventArgs e)
        {
            sendAnswer(1);
        }
        private void B3_Click(object sender, RoutedEventArgs e)
        {
            sendAnswer(2);
        }

        private void B4_Click(object sender, RoutedEventArgs e)
        {
            sendAnswer(3);
        }

    }
}
