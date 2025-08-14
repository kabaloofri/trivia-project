#pragma once

#include "Room.h"
#include <map>

class RoomManager
{
private:
	std::map<unsigned int, Room> m_rooms;
	unsigned int m_currID;
public:
	RoomManager();

	bool createRoom(LoggedUser, RoomData);
	void deleteRoom(int ID);
	bool getRoomState(int ID);
	std::vector<RoomData> getRooms();
	Room& getRoom(int ID);

	int getCurrId();
};