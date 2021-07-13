#pragma once
#include<vector>
#include<unordered_map>
#include<string>
#include<memory>
#include"BTask.h"
/*
	タスクシステム
*/
class TaskSystem
{
	//稼働中のタスク
	vector<BTask::SP> _task;
	
	//待機中
	vector<BTask::SP>	_addtask;

	//グループネーム　固有名　ポインタ保存
	unordered_map<string, unordered_map<string, vector<BTask::SP>>>  _taskdata;

	
public:
	
	//コンストラクタ！
	TaskSystem();

	//デストラクタ！
	~TaskSystem();

	//タスクのアップデート実行	タスクリストにない場合実行しない
	bool Update();

	//タスクの描画実行
	void Draw();

	//タスクを追加
	void AddTask(BTask::SP obj_);

	//指定したグループの存在確認
	bool isGroup(const string& gname_);

	//指定したタスクの存在確認
	bool isTask(const string& gname_, const string& tname_);

	//同名タスクすべて削除
	void Kill_All_GN(const string& gname_, const string& tname_);

	//グループ指定ですべて削除
	void Kill_All_G(const string& gname_);

	//同名タスクすべてストップ
	void Stop_All_GN(const string& gname_, const string& tname_);

	//グループ指定でストップ
	void Stop_All_G(const string& gname_);

	//すべての登録タスクの状態をKillにする
	void AllKillTask();

	//デストラクタでよばれる　タスクすべて破棄
	void AllDeleteTask();

public:
	//-------------------------------------------------------------------------------------------
	//----------指定したタスクで最初に出てくるタスクを返す---------------------------------------
	//-------------------------------------------------------------------------------------------
	template<class T>
	shared_ptr<const T> GetOneTask(const string& gname_,const string& tname_) {
		shared_ptr<const T> cpy_task;

		if (isTask(gname_, tname_)) {
			//Tの型のポインタにキャスト
			cpy_task = static_pointer_cast<const T>(_taskdata[gname_][tname_].front());
		}
		return cpy_task;
	}
	
	//-------------------------------------------------------------------------------------------
	//-------------------指定したタスク全てをVectorで返す---------------------------------------
	//-------------------------------------------------------------------------------------------
	template<class T>
	shared_ptr<vector<shared_ptr<const T>>> GetTasks(const string& gname_, const string& tname_)
	{
		vector<shared_ptr<const T>> tasks;
		//ポインタ型
		size_t i = 4;
		//存在確認
		if (this->isTask(gname_, tname_)) {

			tasks.reserve(_taskdata[gname_][tname_].size() * i);

			for (auto& it : _taskdata[gname_][tname_]) {
				tasks.emplace_back(static_pointer_cast<const T>(it));
			}
		}

		shared_ptr<vector<shared_ptr<const T>>> def(tasks);
		return def;
	}

	//タスクシステムシングルトン
	static TaskSystem& GetInstance();
	
private:
	//アップデート実行
	bool AllUpdate();

	//追加予定
	void MoveTask();

	//Delete状態のタスクを消す
	void StateDeleteTask();

	//priorityを基にソート
	void SortTask();


};

namespace ML {
	static TaskSystem& Task_System = TaskSystem::GetInstance();
}