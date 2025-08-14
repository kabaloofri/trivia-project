using client.communication;
using client.pages;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Windows.Media;            


namespace WpfApp1.pages
{
    /// <summary>
    /// Interaction logic for joinRoomPage.xaml
    /// </summary>
    
    public partial class joinRoomPage : Page
    {
        private Dictionary<Button, int> rooms = new Dictionary<Button, int>();
        private Button selectedButton = null;

        private bool isOpen = true;     
        public joinRoomPage()
        {
            InitializeComponent();


            Select_Button.Click += Select_button;
            this.Unloaded += closePage;

            Thread checkRooms = new Thread(CheckRoomsThread);
            checkRooms.IsBackground = true;
            checkRooms.SetApartmentState(ApartmentState.STA);
            checkRooms.Start();

        }

        private void CheckRoomsThread()
        {
            while (isOpen) //while page is open
            {
                ShowAvailableRooms(); //refresh rooms every 3 seconds with new content from server
                Thread.Sleep(3000);
            }
        }

        private void closePage(object sender, EventArgs e)
        {
            isOpen = false;
        }

        private void Select_button(object sender, RoutedEventArgs e)
        {
            if (selectedButton != null)
            {
                JoinRoomRequest RQ = new JoinRoomRequest();
                RQ.RoomID = rooms[selectedButton];
                Communicator.sendRequest<JoinRoomRequest>(RQ);

                isOpen = false;
                var window = (MainWindow)Application.Current.MainWindow;
                window.main_Frame.Navigate(new WaitingRoom(RQ.RoomID));
            }
        }

        private void ShowAvailableRooms()
        {
            //get all the rooms
            string msg = Communicator.getRequestCode<GetRoomsResponse>() + "0000";
            Response RS = Communicator.sendRequest<string>(msg, false); //no json requierd for this type of request
            GetRoomsResponse GRT = Desrializition.DeserializeResponse<GetRoomsResponse>(RS.msg);

            Application.Current.Dispatcher.Invoke(() => //this is bc the thread dosent own the stuff in the ui, so this accesses the ui regardless
            {

                if (GRT.rooms == null || GRT.rooms.Length == 0)
                {
                    TextBlock noRooms = new TextBlock();
                    noRooms.Text = "No Rooms Found!";
                    noRooms.Height = 60;
                    noRooms.FontSize = 30;
                    noRooms.HorizontalAlignment = HorizontalAlignment.Center;
                    noRooms.Foreground = Brushes.White;
                    if (stack_panel.Children.Count == 0)
                    {
                        this.stack_panel.Children.Insert(0, noRooms);
                    }
                }
                else
                {
                    for (int i = 0; GRT.rooms.Length > i; i++)
                    {
                        bool updatedButton = false;
                        // go through all existing rooms
                        foreach (var room in rooms)
                        {
                            //if the room already exists then only update it
                            if (room.Value == GRT.rooms[i].id)
                            {
                                room.Key.Content = GRT.rooms[i].name + " | players: " + GRT.rooms[i].currentAmountOfPlayers + "/" + GRT.rooms[i].maxPlayers + " | " +
                                GRT.rooms[i].numOfQuestionsInGame + " questions" + " | " + "time per question:  " + GRT.rooms[i].timePerQuestion + " | " + ((RoomStates)GRT.status).ToString().Substring(6);
                                updatedButton = true;
                            }
                        }

                        if (updatedButton)
                        {
                            continue;
                        }

                        Button roomBtn = new Button();
                        //properties
                        roomBtn.Content = GRT.rooms[i].name + " | players: " + GRT.rooms[i].currentAmountOfPlayers + "/" + GRT.rooms[i].maxPlayers + " | ";
                        roomBtn.Content += GRT.rooms[i].numOfQuestionsInGame + " questions" + " | " + "time per question:  " + GRT.rooms[i].timePerQuestion + " | " + ((RoomStates)GRT.status).ToString().Substring(6);
                        roomBtn.FontSize = 20;
                        roomBtn.Height = 60;
                        roomBtn.Name = "Room_Button_" + i;
                        roomBtn.HorizontalAlignment = HorizontalAlignment.Stretch;
                        roomBtn.Width = Double.NaN;
                        roomBtn.Background = Brushes.Gainsboro;

                        roomBtn.BorderBrush = Brushes.Black;
                        roomBtn.BorderThickness = new Thickness(5);
                        //roomBtn.Background = "a7cd5c5c";



                        // Create the Border
                        FrameworkElementFactory borderFactory = new FrameworkElementFactory(typeof(Border));
                        borderFactory.SetValue(Border.CornerRadiusProperty, new CornerRadius(20));
                        borderFactory.SetValue(Border.BackgroundProperty, new TemplateBindingExtension(Button.BackgroundProperty));
                        borderFactory.SetValue(Border.BorderBrushProperty, new TemplateBindingExtension(Button.BorderBrushProperty));
                        borderFactory.SetValue(Border.BorderThicknessProperty, new TemplateBindingExtension(Button.BorderThicknessProperty));
                        borderFactory.SetValue(Border.SnapsToDevicePixelsProperty, true);

                        // Add ContentPresenter inside
                        FrameworkElementFactory contentPresenter = new FrameworkElementFactory(typeof(ContentPresenter));
                        contentPresenter.SetValue(ContentPresenter.HorizontalAlignmentProperty, HorizontalAlignment.Center);
                        contentPresenter.SetValue(ContentPresenter.VerticalAlignmentProperty, VerticalAlignment.Center);

                        borderFactory.AppendChild(contentPresenter);

                        ControlTemplate template = new ControlTemplate(typeof(Button));
                        template.VisualTree = borderFactory;

                        roomBtn.Template = template;

                        //handle events
                        roomBtn.Click += (s, e) =>
                        {
                            if (selectedButton != null)
                            {
                                selectedButton.Background = Brushes.Gainsboro;
                            }

                            selectedButton = roomBtn;
                            selectedButton.Background = Brushes.DodgerBlue;
                        };

                        rooms.Add(roomBtn, GRT.rooms[i].id);// add button and id to rooms

                        if (i != 0)
                        {
                            roomBtn.Margin = new Thickness(0, 5, 0, 0); //add spacing to buttons
                        }
                        this.stack_panel.Children.Insert(0, roomBtn); //add to panel
                    } 
                }
            });
        }


    }
}
