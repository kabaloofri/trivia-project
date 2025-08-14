using client.communication;
using System.Configuration;
using System.Data;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;

namespace WpfApp1;

/// <summary>
/// Interaction logic for App.xaml
/// </summary>
public partial class App : Application
{
    void App_DispatcherUnhandledException(object sender, DispatcherUnhandledExceptionEventArgs e)
    {
        // Process unhandled exception
        var mainWindow = (MainWindow)Application.Current.MainWindow;
        mainWindow.ErrorPopup.ShowMessage(e.Exception.Message);
        //Prevent default unhandled exception processing
        e.Handled = true;
    }
}

