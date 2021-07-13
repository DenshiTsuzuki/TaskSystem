#include "BTask.h"
BTask::BTask(const string& gn_, const string& n_) :
	groupname(gn_),
	name(n_),
	_priority(0.f),
	_state(Task_State::Active)
{

}

BTask::~BTask() {

}
void BTask::Kill()
{
	this->_state = Task_State::Kill;
}

void BTask::Stop()
{
	this->_state = Task_State::Stop;
}



