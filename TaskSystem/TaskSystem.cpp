#pragma once
#include "TaskSystem.h"
#include<algorithm>
TaskSystem::TaskSystem()
{

}

TaskSystem::~TaskSystem()
{
	//�������̃^�X�N�����ׂĔj������
	this->AllDeleteTask();
}

bool TaskSystem::Update()
{
	//�A�b�v�f�[�g���s
	bool HasTask = this->AllUpdate();

	//�������s��ꂽ�ꍇ�̂ݎ��s
	if (HasTask)
	{
		//�������̃^�X�N���ړ�
		MoveTask();

		//�폜�҂��^�X�N���폜
		StateDeleteTask();

		//�^�X�N���X�g���\�[�g
		SortTask();
	}
	return HasTask;
}

void TaskSystem::Draw()
{
	//���ׂĂ̕`����s
	for (auto& it : _task) {
		if (it->_state != BTask::Task_State::Kill) {

			it->Draw();

		}
	}
}

void TaskSystem::AddTask(BTask::SP obj_)
{
	//�����I�u�W�F�N�g�`�F�b�N
	if (obj_ == nullptr) { return; }

	//�o�^�҂��ɓo�^
	_addtask.emplace_back(obj_);
	
	//���o�^
	_taskdata[_addtask.back()->groupname][_addtask.back()->name].emplace_back(obj_);
}

bool TaskSystem::isGroup(const string& gname_)
{
	//�P�ł��������ꍇtrue��Ԃ�
	return _taskdata.count(gname_);
}

bool TaskSystem::isTask(const string& gname_, const string& tname_)
{
	//gname�O���[�v������@�O���[�v����tname_���������ꍇtrue��Ԃ�
	return _taskdata.count(gname_) && _taskdata[gname_].count(tname_);
}

void TaskSystem::Kill_All_GN(const string& gname_, const string& tname_)
{
	//�^�X�N�Ȃ�
	if (this->isTask(gname_, tname_) == false) {
		return;
	}

	//�폜�\������
	for (auto& it : _taskdata[gname_][tname_]) {
		it->Kill();
	}
}

void TaskSystem::Kill_All_G(const string& gname_)
{
	//�O���[�v�Ȃ�
	if (this->isGroup(gname_) == false) {
		return;
	}

	for (auto& it : _taskdata[gname_]) {
		//map�ŊǗ����Ă���2�Ԗڗv�f�ɃA�N�Z�X(�^�X�N�|�C���^)
		for (auto& t_ : it.second) {
			t_->Kill();
		}
	}
}

void TaskSystem::Stop_All_GN(const string& gname_, const string& tname_)
{
	//�^�X�N����
	if (isTask(gname_, tname_) == false) {
		return;
	}

	for (auto& it : this->_taskdata[gname_][tname_]) {
		it->Stop();
	}
}

void TaskSystem::Stop_All_G(const string& gname_)
{
	if (this->isGroup(gname_) == false) {
		return;
	}

	for (auto& it : this->_taskdata[gname_]) {
		for (auto& its : it.second) {
			its->Stop();
		}
	}
}

void TaskSystem::AllKillTask()
{
	//�o�^����Ă��邷�ׂẴ^�X�N��Kill
	//�O���[�v����loop
	for (auto& map : _taskdata) {
		//�^�X�N����loop
		for (auto& it : map.second) {
			//�^�X�N������ԃ��[�v
			for (auto& t_ : it.second) {
				t_->Kill();
			}
		}
	}
}

void TaskSystem::AllDeleteTask()
{
	//���ׂĔj��
	this->_task.clear();
	this->_task.emplace_back();//���ׂ�K�v����
	this->_addtask.clear();
	this->_addtask.emplace_back();

	_taskdata.clear();
}

TaskSystem& TaskSystem::GetInstance()
{
	//�^�X�N�V�X�e���쐬�I���܂͂Ƃ肠���������
	static TaskSystem ts;
	return ts;
}

bool TaskSystem::AllUpdate()
{
	//�^�X�N�����݂��Ȃ������ꍇ
	if (_addtask.empty() && _task.empty()) {
		return false;
	}

	//�o�^�\��̃A�b�v�f�[�g���s
	for (auto& it : _addtask) {
		it->Update();
	}

	//�o�^�ς݃^�X�N
	for (auto& it : _task) {
		switch (it->_state){
		using enum BTask::Task_State;
		case Active:
			it->Update();
			break;
			
		case Kill:
			//��Ԃ�Kill�̏�Ԃ̏ꍇ�͏I������
			it->Finalize();
			it->_state = Delete;
			break;
		}
	}
	return true;
}

void TaskSystem::MoveTask()
{
	//�ǉ��\��^�X�N�Ȃ�
	if (_addtask.empty()) {
		return;
	}

	//����
	_task.insert(_task.end(), _addtask.begin(), _addtask.end());
	_addtask.clear();
	_addtask.shrink_to_fit();
}

void TaskSystem::StateDeleteTask()
{
	//�֐��I�u�W�F�N�g�@�����_��
	auto deleteCondition = [](BTask::SP& obj)
	{
		return (obj->_state == BTask::Task_State::Delete);
	};

	//�I�u�W�F�N�g�̍폜
	{
		//-----------------------------------------------------------------------------------------------------------------
		const auto& removeit = remove_if(_task.begin(), _task.end(), deleteCondition);
		_task.erase(removeit, _task.end());

		//�̈�̐؂��
		_task.shrink_to_fit();
	}

	//�f�[�^�폜
	//�O���[�v���� loop
	for (auto map = _taskdata.begin();map != _taskdata.end();) {
		//�ŗL�������[�v
		for (auto itg = map->second.begin();itg != map->second.end();) {
			//-----------------------------------------------------------------------------------------------------------------
			//�̈�̉��
			const auto& removeit = remove_if(itg->second.begin(), itg->second.end(), deleteCondition);
			itg->second.erase(removeit, itg->second.end());
			
			//�̈�̐؂��
			itg->second.shrink_to_fit();

			//�ŗL���̐�0�̏ꍇ�폜
			if (itg->second.empty()) {
				itg = map->second.erase(itg);
			}
			else {
				++itg;
			}
		}
		//�O���[�v�̒���0�̏ꍇ�폜
		if (map->second.empty()) {
			map = _taskdata.erase(map);
		}
		else {
			++map;
		}
	}
}

void TaskSystem::SortTask()
{
	//�P���Ƀ\�[�g(����)
	sort(_task.begin(), _task.end(),[](BTask::SP& left, BTask::SP& right)
		{
			return (left->_priority < right->_priority);
		}
	);
}





