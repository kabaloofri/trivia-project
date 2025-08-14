#include "roomManager.h"


#include <thread>
#include <chrono>

RoomManager::RoomManager()
{
    m_currID = 100;
    m_rooms = std::map<unsigned int, Room>();
}

bool RoomManager::createRoom(LoggedUser user, RoomData data)
{
    Room room(data, user);

    //check if a room with the same name exists
    for (auto rm : getRooms())
    {
        if (rm.name == data.name)
        {
            return false; //return false if it does
        }
    }

    m_rooms.emplace(data.id ,room);
    return true;
}

void RoomManager::deleteRoom(int ID)
{
    m_rooms.erase(m_rooms.find(ID));
}

bool RoomManager::getRoomState(int ID)
{
    return m_rooms[ID].getRoomData().status;
}

std::vector<RoomData> RoomManager::getRooms()
{
    std::vector<RoomData> roomData;
    for (auto& room : m_rooms)
    {
        if (room.second.getRoomData().status == state_waitingForPlayers)
        {
            roomData.push_back(room.second.getRoomData());
        }
    }
    
    return roomData;
}

Room& RoomManager::getRoom(int ID)
{
    return m_rooms[ID];
}

int RoomManager::getCurrId()
{
    return m_currID++; //add the id for next room

}

