using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Text.Json;
using System.Windows;
using WpfApp1;

namespace client.communication
{

    public struct GetPlayersInRoomRequest
    {
        public int RoomID { get; set; }
    }

    struct JoinRoomRequest
    {
        public int RoomID { get; set; }
    }

    struct CreateRoomRequest
    {
        public string roomName { set; get; }
        public int maxUsers { set; get; }
        public int questionsCount { set; get; }
        public int answerTimeout { set; get; }

        public CreateRoomRequest(string name, int max, int count, int time)
        {
            roomName = name;
            maxUsers = max;
            questionsCount = count;
            answerTimeout = time;
        }

    }

    struct ChangeRoomSettingsRequest
    {
        public int questionCount { get; set; }
        public int answerTimeout { get; set; }
    }

    struct SubmitAnswerRequest
    {
        public int AnswerID { get; set; }
        public float answerTime { get; set; }
    }
    struct LoginRequest
    {
        public string username { get; set; }
        public string password { get; set; }

        public LoginRequest(string name, string pass)
        {
            this.username = name;
            this.password = pass;
        }
    }

    struct GetQuestionRequest
    {
        public int status;
    }
    struct leaveRoomRequest
    {
        public int status;
    }

    struct LeaveGameRequest
    {
        public uint status;
    }

    struct SignupRequest
    {
        public string username { get; set; }
        public string password { get; set; }
        public string mail { get; set; }
        public SignupRequest(string name, string pass, string mail)
        {
            this.username = name;
            this.password = pass;
            this.mail = mail;
        }
    }

    struct Response
    {
        public int code { get; set; }
        public string msg { get; set; }

        public Response(int code, string msg)
        {
            this.code = code;
            this.msg = msg;
        }
    }

    enum Codes
    {
        code_OK = 0,
        code_error = -1,

        code_logIn = 1,
        code_signUp,

        code_logOut,
        code_getRooms,
        code_getPlayersInRoom,
        code_getHighScore,
        code_getPersonalStats,
        code_joinRoom,
        code_createRoom,

        code_changeSettings,
        code_closeRoom,
        code_startGame,
        code_getRoomState,
        code_leaveRoom,

        code_leaveGame,
        code_getQuestion,
        code_submitAnswer,
        code_getGameResults,
    };

    static class Communicator
    {
        public static TcpClient socket = new TcpClient();

        public static NetworkStream stream;
        public static string username;

        public static void start()
        {
            Task.Run(() =>
            {
                try
                {
                    IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(Settings.ip), Settings.port);
                    socket.Connect(serverEndPoint);
                    stream = new NetworkStream(socket.Client);
                }
                catch
                {
                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        var mainWindow = (MainWindow)Application.Current.MainWindow;
                        mainWindow.ErrorPopup.ShowMessage("error connecting to server!");
                        stream = null;
                    });
                }
            });
        }

        /*template function to send requets to the server.
        use: sendRequest<LoginRequest>(request)
        if you need to send only code and msg, then use like this:
        sendRequest<string>(code+0000, false)
        */
        public static Response sendRequest<Type>(Type req, bool hasJson = true)
        {
            try
            {
                // *** serialize response ***

                string request;
                ASCIIEncoding encoder = new ASCIIEncoding();
                if (hasJson)
                {
                    string json = JsonSerializer.Serialize(req);
                    request = getRequestCode<Type>() + json.Length.ToString("0000") + json;
                }
                else
                {
                    request = req.ToString();
                }
                //serialize request and add code based on type, padded length and the msg

                //encode request
                var data = encoder.GetBytes(request);

                // *** send request ***
                stream.Write(data, 0, data.Length);
                stream.Flush();//send request


                // *** get code ***

                //try to get the server responce
                //get code
                var buffer = new byte[2];
                int bytesRead = stream.Read(buffer, 0, 2);

                var temp = encoder.GetString(buffer);

                int code = Int32.Parse(temp); //get code

                // *** get length ***

                buffer = new byte[4];
                bytesRead += stream.Read(buffer, 0, 4); //get length

                if (bytesRead < 5)
                {
                    MessageBox.Show("erorr getting response header!");
                }

                temp = encoder.GetString(buffer); // decode length

                int len = Int32.Parse(temp);

                // *** get rest of json ***

                buffer = new byte[len];
                stream.Read(buffer, 0, len);

                string msg = encoder.GetString(buffer);

                return new Response(code, msg);

                //could handle requsts here
            }
            catch (Exception e)
            {
                MessageBox.Show("error with server! server might have disconnected, or is offline." + e.Message);
                return new Response(-1, "");
            }
        }



        public static string getRequestCode<Type>()
        {
            return getCode<Type>().ToString().PadLeft(2, '0'); //convert request code to binary
        }
        private static int getCode<Type>()
        {
            if (typeof(LoginRequest) == typeof(Type)) return (int)Codes.code_logIn;
            else if (typeof(SignupRequest) == typeof(Type)) return (int)Codes.code_signUp;
            else if (typeof(GetPlayersInRoomRequest) == typeof(Type)) return (int)Codes.code_getPlayersInRoom;
            else if (typeof(CreateRoomRequest) == typeof(Type)) return (int)Codes.code_createRoom;
            else if (typeof(LogoutResponse) == typeof(Type)) return (int)Codes.code_logOut;
            else if (typeof(JoinRoomRequest) == typeof(Type)) return (int)Codes.code_joinRoom;
            else if (typeof(ChangeRoomSettingsRequest) == typeof(Type)) return (int)Codes.code_changeSettings;
            else if (typeof(LeaveGameRequest) == typeof(Type)) return (int)Codes.code_leaveGame;
            else if (typeof(GetQuestionRequest) == typeof(Type)) return (int)Codes.code_getQuestion;
            else if (typeof(SubmitAnswerRequest) == typeof(Type)) return (int)Codes.code_submitAnswer;

            else if (typeof(GetRoomsResponse) == typeof(Type)) return (int)Codes.code_getRooms;
            else if (typeof(GetHighScoreResponse) == typeof(Type)) return (int)Codes.code_getHighScore;
            else if (typeof(GetPersonalStatsResponse) == typeof(Type)) return (int)Codes.code_getPersonalStats;

            else if (typeof(GetRoomStateResponse) == typeof(Type)) return (int)Codes.code_getRoomState;
            else if (typeof(CloseRoomResponse) == typeof(Type)) return (int)Codes.code_closeRoom;
            else if (typeof(LeaveRoomResponse) == typeof(Type)) return (int)Codes.code_leaveRoom;
            else if (typeof(StartGameResponse) == typeof(Type)) return (int)Codes.code_startGame;
            else if (typeof(ChangeRoomSettingsResponse) == typeof(Type)) return (int)Codes.code_changeSettings;

            else if (typeof(LeaveGameResponse) == typeof(Type)) return (int)Codes.code_leaveGame;
            else if (typeof(GetGameResultsResponse) == typeof(Type)) return (int)Codes.code_getGameResults;


            else
            {
                MessageBox.Show("error getting request code!");
                return -1;
            }
        }
    }
}
