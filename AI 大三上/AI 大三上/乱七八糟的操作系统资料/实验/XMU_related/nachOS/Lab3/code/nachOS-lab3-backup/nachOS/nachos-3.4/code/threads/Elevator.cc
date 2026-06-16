// Elevator.cc
// 
// By ForenewHan 2018.6.15


#include "Elevator.h"
#include "system.h"

Elevator::Elevator(char *debugName, int numFloors, int myID, int *FloorUpEleId, int *FloorDownEleId){
	eleID = myID;
	name = debugName;

	maxFloor = numFloors;
	currentfloor = 1;	// when no one's using, we would stop elevator at first floor

	occupancy = 0;		// when initialize, there is no one in elevator
	maxVolumn = MAX_ELE_VOLUMN;

	direction = 0;		// meas that elevator is still

	state = running;

	// FloorToGo indicates which floor we have to go, if true, set as 1, otherwise as 0
	FloorToGo = new int[numFloors];
	for(int i=0;i<numFloors;i++){
		FloorToGo[i] = -1;
	}

	NoOneInOrOut = new EventBarrier("waiting for riders");
	doorsOpen  = new EventBarrier("waiting for doors");

	floorUpID = FloorUpEleId;
	floorDownID = FloorDownEleId;

}

Elevator::~Elevator(){

	delete []FloorToGo;
	delete NoOneInOrOut;
	delete doorsOpen;
}

void Elevator::initialize(){
	int floorToStop;
	while(state == running){
		floorToStop = checkout();
		if(floorToStop != -1){
			VisitFloor(floorToStop);
			if(direction == 1)
				floorUpID[floorToStop] = eleID;
			else if(direction == 0)
				floorDownID[floorToStop] = eleID;
			OpenDoors();
			CloseDoors();
			if(direction == 1)
				floorUpID[floorToStop] = -1;
			else if(direction == 0)
				floorDownID[floorToStop] = -1;

		}
		else{
			// if there is no one to pick up, then set as stop
			direction = 0;
		}
		currentThread->Yield();
	}
}

// checkout elevator state, to find out next stop to go.
int Elevator::checkout(){
	// case 1, if elevator is still, then go to the first one to press button
	// here we don't consider synchronization, just assume for loop takes no time
	// I think this is suitable for reality, since computer runs much more faster
	// than human motion. And even if that two people press the button at the same
	// time, still, just satisfiy one is ok.
	if(direction == 0){
		for(int i=0;i<maxFloor;i++){
			if(FloorToGo[i] == 1){
				FloorToGo[i] == -1;
				return i;
			}
		}
	}

	// case 2, if elevator goes up, then check if we should pick up someone waiting
	// at the upper levels.
	if(direction == 1){
		for(int i=currentfloor; i<maxFloor; i++){
			if(FloorToGo[i] == 1){
				FloorToGo[i] == -1;
				return i;
			}
		}
	}

	// case 3, if elevator goes down, then check if we should pick up someone waiting
	// at lower levels.
	if(direction == -1){
		for(int i=0;i<currentfloor;i++){
			if(FloorToGo[i] == 1){
				FloorToGo[i] == -1;
				return i;
			}
		}
	}

	// if there is no one on the same road as elevator goes, then assume there is no one 
	// to pick up, and wait for controler to set it's direction as 0;
	return -1;
}

void Elevator::VisitFloor(int floor){
	currentfloor = floor;
}

void Elevator::OpenDoors(){
	if(direction == 1){
		waitEleUp[currentfloor]->Signal();
	}
	else if(direction == -1){
		waitEleDown[currentfloor]->Signal();
	}
	else{
		waitEleUp[currentfloor]->Signal();
		waitEleDown[currentfloor]->Signal();
	}
	doorsOpen->Signal();
	currentThread->Yield();
}

void Elevator::CloseDoors(){
	NoOneInOrOut->Wait();
	NoOneInOrOut->Complete();
}

void Elevator::RequestFloor(int floor){
	FloorToGo[floor] = 1;
}

bool Elevator::Enter(){
	doorsOpen->Wait();
	doorsOpen->Complete();
	if(occupancy < maxVolumn){
		occupancy++;
		return TRUE;
	}
	else{
		return FALSE;
	}
}

bool Elevator::Exit(){
	doorsOpen->Wait();
	doorsOpen->Complete();
	if(occupancy > 0){
		occupancy--;
		return TRUE;
	}
	else{
		return FALSE;
	}
}

Building::Building(char *debugname, int numFloors, int numElevators){
	name = debugname;
	eleNum = numElevators;				// tell how many elevators we are going to create
	elevator = new Elevator*[eleNum];	// create eleNum elevators
	floors = numFloors;					// tell how many floors we have 

	for(int i=0;i<eleNum;i++){
		elevator[i] = new Elevator("elevator", floors, i, FloorUpEleId, FloorDownEleId);
	}


	waitEleUp = new EventBarrier*[floors];		// initialize eventBarriers, every floor set an Barrier, for up
	for(int i=0;i<floors;i++){
		waitEleUp[i] = new EventBarrier("waiting for Ele");
	}

	waitEleDown = new EventBarrier*[floors];		// initialize eventBarriers, every floor set an Barrier, for down
	for(int i=0;i<floors;i++){
		waitEleDown[i] = new EventBarrier("waiting for Ele");
	}

	FloorUpEleId = new int[floors];
	FloorDownEleId = new int[floors];
	for(int i=0;i<floors;i++){
		FloorUpEleId[i] = -1;
		FloorDownEleId[i] = -1;
	}

	for(int i=0;i<eleNum;i++){
		elevator[i]->initialize();
	}
}

Building::~Building(){
	delete []elevator;

	for(int i=0;i<floors;i++){
		delete waitEleUp[i];
	}
	delete []waitEleUp;

	for(int i=0;i<floors;i++){
		delete waitEleDown[i];
	}
	delete []waitEleDown;

	delete []FloorUpEleId;
	delete []FloorDownEleId;

}

void Building::CallUp(int fromFloor){
	int direction = 1;		// 1 stands for go up
	int eleId = chooseBestElevator(fromFloor, direction);
	addToEleStop(eleId, fromFloor, 1);
}

void Building::CallDown(int fromFloor){
	int direction = -1;		// -1 stands for go down
	int eleId = chooseBestElevator(fromFloor, direction);
	addToEleStop(eleId, fromFloor, -1);
}

// find the best elevator that suits our journey, set the elevator ID and return
int Building::chooseBestElevator(int fromFloor, int direction){

	int decisionEle = -1;			// which elevator to call
	int distance = floors;			// the longest distance, we compare with current distance, choose the nearest elevator
	int currentDistance;			// the current distance, we use this to make sure which elevator to choose
	bool IsEleCorrectSide;			// judge if current elevator is at top of callFloor or not

	// our stratege here is to call the free elevator first
	for(int i=0;i<eleNum;i++){			
		if(elevator[i]->direction == 0){		// judge if elevator is free, if it's free, then its direction is set to 0
			currentDistance = (elevator[i]->currentfloor > fromFloor) ? 
					(elevator[i]->currentfloor - fromFloor) : (fromFloor - elevator[i]->currentfloor);
			if(currentDistance < distance){
				distance = currentDistance;
				decisionEle = i;
			}
		}
	}
	// if we find an elevator, then return.
	if(decisionEle != -1){
		return decisionEle;
	}

	// if all elevators are busy, then try to find the nearest one with same direction
	for(int i=0;i<eleNum;i++){
		IsEleCorrectSide = (fromFloor - elevator[i]->currentfloor) * direction >= 0;
		if(elevator[i]->direction == direction && IsEleCorrectSide){		
			currentDistance = (elevator[i]->currentfloor > fromFloor) ? 
					(elevator[i]->currentfloor - fromFloor) : (fromFloor - elevator[i]->currentfloor);
			if(currentDistance < distance){
				distance = currentDistance;
				decisionEle = i;
			}
		}
	}
	// if we find an elevator, then return.
	if(decisionEle != -1){
		return decisionEle;
	}

	// if all elevators are busy and not in same direction, then find the first one
	return 1;

}

void Building::addToEleStop(int eleID, int fromFloor, int dir){
	elevator[eleID]->FloorToGo[fromFloor] = 1;
	elevator[eleID]->direction = dir;

}

Elevator* Building::AwaitUp(int fromFloor){
	waitEleUp[fromFloor]->Wait();
	waitEleUp[fromFloor]->Complete();
	int eleID = FloorUpEleId[fromFloor];
	return elevator[eleID];
}

Elevator* Building::AwaitDown(int fromFloor){
	waitEleDown[fromFloor]->Wait();
	waitEleDown[fromFloor]->Complete();
	int eleID = FloorDownEleId[fromFloor];
	return elevator[eleID];
}

