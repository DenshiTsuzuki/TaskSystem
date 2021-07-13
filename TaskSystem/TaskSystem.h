#pragma once
#include<vector>
#include<unordered_map>
#include<string>
#include<memory>
#include"BTask.h"
/*
	�^�X�N�V�X�e��
*/
class TaskSystem
{
	//�ғ����̃^�X�N
	vector<BTask::SP> _task;
	
	//�ҋ@��
	vector<BTask::SP>	_addtask;

	//�O���[�v�l�[���@�ŗL���@�|�C���^�ۑ�
	unordered_map<string, unordered_map<string, vector<BTask::SP>>>  _taskdata;

	
public:
	
	//�R���X�g���N�^�I
	TaskSystem();

	//�f�X�g���N�^�I
	~TaskSystem();

	//�^�X�N�̃A�b�v�f�[�g���s	�^�X�N���X�g�ɂȂ��ꍇ���s���Ȃ�
	bool Update();

	//�^�X�N�̕`����s
	void Draw();

	//�^�X�N��ǉ�
	void AddTask(BTask::SP obj_);

	//�w�肵���O���[�v�̑��݊m�F
	bool isGroup(const string& gname_);

	//�w�肵���^�X�N�̑��݊m�F
	bool isTask(const string& gname_, const string& tname_);

	//�����^�X�N���ׂč폜
	void Kill_All_GN(const string& gname_, const string& tname_);

	//�O���[�v�w��ł��ׂč폜
	void Kill_All_G(const string& gname_);

	//�����^�X�N���ׂăX�g�b�v
	void Stop_All_GN(const string& gname_, const string& tname_);

	//�O���[�v�w��ŃX�g�b�v
	void Stop_All_G(const string& gname_);

	//���ׂĂ̓o�^�^�X�N�̏�Ԃ�Kill�ɂ���
	void AllKillTask();

	//�f�X�g���N�^�ł�΂��@�^�X�N���ׂĔj��
	void AllDeleteTask();

public:
	//-------------------------------------------------------------------------------------------
	//----------�w�肵���^�X�N�ōŏ��ɏo�Ă���^�X�N��Ԃ�---------------------------------------
	//-------------------------------------------------------------------------------------------
	template<class T>
	shared_ptr<const T> GetOneTask(const string& gname_,const string& tname_) {
		shared_ptr<const T> cpy_task;

		if (isTask(gname_, tname_)) {
			//T�̌^�̃|�C���^�ɃL���X�g
			cpy_task = static_pointer_cast<const T>(_taskdata[gname_][tname_].front());
		}
		return cpy_task;
	}
	
	//-------------------------------------------------------------------------------------------
	//-------------------�w�肵���^�X�N�S�Ă�Vector�ŕԂ�---------------------------------------
	//-------------------------------------------------------------------------------------------
	template<class T>
	shared_ptr<vector<shared_ptr<const T>>> GetTasks(const string& gname_, const string& tname_)
	{
		vector<shared_ptr<const T>> tasks;
		//�|�C���^�^
		size_t i = 4;
		//���݊m�F
		if (this->isTask(gname_, tname_)) {

			tasks.reserve(_taskdata[gname_][tname_].size() * i);

			for (auto& it : _taskdata[gname_][tname_]) {
				tasks.emplace_back(static_pointer_cast<const T>(it));
			}
		}

		shared_ptr<vector<shared_ptr<const T>>> def(tasks);
		return def;
	}

	//�^�X�N�V�X�e���V���O���g��
	static TaskSystem& GetInstance();
	
private:
	//�A�b�v�f�[�g���s
	bool AllUpdate();

	//�ǉ��\��
	void MoveTask();

	//Delete��Ԃ̃^�X�N������
	void StateDeleteTask();

	//priority����Ƀ\�[�g
	void SortTask();


};

namespace ML {
	static TaskSystem& Task_System = TaskSystem::GetInstance();
}