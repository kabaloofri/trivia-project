using System.Text.Json;

namespace client.communication
{
    enum RoomStates
    {
        state_waitingForPlayers,
        state_begginingGame,
        state_gameStarted,
        state_inactive
    }
    struct RoomData
    {
        public int id { set; get; }
        public string name { set; get; }
        public int maxPlayers { set; get; }
        public int currentAmountOfPlayers { get; set; }
        public int numOfQuestionsInGame { set; get; }
        public int timePerQuestion { set; get; }
        public int status { set; get; }
    }
    struct ErrorResponse
    {
        public string message { set; get; }
    }

    struct SignupResponse
    {
        public int status { get; set; }
    }

    struct LoginResponse
    {
        public int status { get; set; }
    }


    struct LogoutResponse
    {
        public int status { get; set; }
    }

    struct GetRoomsResponse
    {
        public int status { set; get; }
        public RoomData[] rooms { set; get; }
    }

    struct GetPlayersInRoomResponse
    {
        public string[] players { get; set; }
    }
    struct GetHighScoreResponse
    {
        public int status { set; get; }
        public string[] statistics { set; get; }

    }

    struct GetPersonalStatsResponse
    {
        public int status { set; get; }
        public string[] statistics { set; get; }
    }

    struct JoinRoomResponse
    {
        public int status { get; set; }
    }

    struct CreateRoomResponse
    {
        public int status { get; set; }
        public int roomId { get; set; }
    }

    struct ChangeRoomSettingsResponse
    {
        public int status { get; set; }
    }




    struct GetRoomStateResponse
    {
        public int status { get; set; }
        public double timeUntilGameBegins { get; set; }
        public string[] players { get; set; }
        public int querstionsCount { get; set; }
        public int answerTimeout { get; set; }
    };

    struct StartGameResponse
    {
        public int status { get; set; }
    }

    struct SubmitAnswerResponse
    {
        public uint status { get; set; }
        public uint correctAnswerId { get; set; }
    }

    struct LeaveRoomResponse
    {
        public int status { get; set; }
    }
    struct CloseRoomResponse
    {
        public int status { get; set; }
    }


    struct LeaveGameResponse
    {
        public int status { get; set; }
    };

    struct GetQuestionResponse
    {
        public int status { get; set; }
        public string question { get; set; }
        public Dictionary<string, string> answers { get; set; }
    };




    struct PlayerResults
    {
        public string username { get; set; }
        public int score { get; set; }
        public int correctAnswerCount { get; set; }
        public int wrongAnswerCount { get; set; }
        public double averageAnswerTime { get; set; }
    };

    struct GetGameResultsResponse
    {
        public int status { get; set; }
        public PlayerResults[] results { get; set; }
    };

    static class Desrializition
    {

        public static Type DeserializeResponse<Type>(string msg)
        {
            return JsonSerializer.Deserialize<Type>(msg);
        }
    }

}
