# 🎲 Trivia Project

An online multiplayer trivia game you can play with friends!
Built as an 11th-grade final project in Magshimim. Features a C++ server and a C# client for a complete multiplayer experience.

## 🚀 Features

 - 🔑 Login System: Username and password stored securely in an SQLite database.

 - 🧵 Multithreaded Server: Each client runs in a separate thread for smooth performance.

 - 🌐 Networking: Built with WinAPI sockets for reliable connections.

 - 🎨 WPF Client: Simple and user-friendly interface.

 - 📦 Data Handling: Data is converted to binary and transmitted using JSON (nlohmann/json).

## 🛠 Tech Stack

 - Server: C++, Multithreading

 - Client: C# with WPF

 - Database: SQLite

 - Networking: WinAPI sockets

 - Data Serialization: JSON ([nlohmann/json](https://github.com/nlohmann/json))

## 💾 Installation / Download

The project was developed in Visual Studio:

1. **Server**

   Open TriviaServer.sln in Visual Studio.

   Build & run the server.

2. **Client**

   Open Client.sln in another Visual Studio instance.

   Build & run the client.

   Connect to the server using your local IP address.

## 📚 Additional Info

Detailed UML of the server architecture available in [draw.io](https://app.diagrams.net) 

Upload the TriviaServer/Trivia-ServerDesign.xml file to the diagram to view it.
