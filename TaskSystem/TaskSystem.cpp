#pragma once
#include "TaskSystem.h"
#include<algorithm>
TaskSystem::TaskSystem()
{

}

TaskSystem::~TaskSystem()
{
	//生存中のタスクをすべて破棄する
	this->AllDeleteTask();
}

bool TaskSystem::Update()
{
	//アップデート実行
	bool HasTask = this->AllUpdate();

	//処理が行われた場合のみ実行
	if (HasTask)
	{
		//準備中のタスクを移動
		MoveTask();

		//削除待ちタスクを削除
		StateDeleteTask();

		//タスクリストをソート
		SortTask();
	}
	return HasTask;
}

void TaskSystem::Draw()
{
	//すべての描画実行
	for (auto& it : _task) {
		if (it->_state != BTask::Task_State::Kill) {

			it->Draw();

		}
	}
}

void TaskSystem::AddTask(BTask::SP obj_)
{
	//無効オブジェクトチェック
	if (obj_ == nullptr) { return; }

	//登録待ちに登録
	_addtask.emplace_back(obj_);
	
	//情報登録
	_taskdata[_addtask.back()->groupname][_addtask.back()->name].emplace_back(obj_);
}

bool TaskSystem::isGroup(const string& gname_)
{
	//１つでもあった場合trueを返す
	return _taskdata.count(gname_);
}

bool TaskSystem::isTask(const string& gname_, const string& tname_)
{
	//gnameグループがあり　グループ内にtname_があった場合trueを返す
	return _taskdata.count(gname_) && _taskdata[gname_].count(tname_);
}

void TaskSystem::Kill_All_GN(const string& gname_, const string& tname_)
{
	//タスクなし
	if (this->isTask(gname_, tname_) == false) {
		return;
	}

	//削除申請処理
	for (auto& it : _taskdata[gname_][tname_]) {
		it->Kill();
	}
}

void TaskSystem::Kill_All_G(const string& gname_)
{
	//グループなし
	if (this->isGroup(gname_) == false) {
		return;
	}

	for (auto& it : _taskdata[gname_]) {
		//mapで管理している2番目要素にアクセス(タスクポインタ)
		for (auto& t_ : it.second) {
			t_->Kill();
		}
	}
}

void TaskSystem::Stop_All_GN(const string& gname_, const string& tname_)
{
	//タスク無し
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
	//登録されているすべてのタスクをKill
	//グループ名でloop
	for (auto& map : _taskdata) {
		//タスク名でloop
		for (auto& it : map.second) {
			//タスクがある間ループ
			for (auto& t_ : it.second) {
				t_->Kill();
			}
		}
	}
}

void TaskSystem::AllDeleteTask()
{
	//すべて破棄
	this->_task.clear();
	this->_task.emplace_back();//調べる必要あり
	this->_addtask.clear();
	this->_addtask.emplace_back();

	_taskdata.clear();
}

TaskSystem& TaskSystem::GetInstance()
{
	//タスクシステム作成！いまはとりあえずこれで
	static TaskSystem ts;
	return ts;
}

bool TaskSystem::AllUpdate()
{
	//タスクが存在しなかった場合
	if (_addtask.empty() && _task.empty()) {
		return false;
	}

	//登録予定のアップデート実行
	for (auto& it : _addtask) {
		it->Update();
	}

	//登録済みタスク
	for (auto& it : _task) {
		switch (it->_state){
		using enum BTask::Task_State;
		case Active:
			it->Update();
			break;
			
		case Kill:
			//状態がKillの状態の場合は終了処理
			it->Finalize();
			it->_state = Delete;
			break;
		}
	}
	return true;
}

void TaskSystem::MoveTask()
{
	//追加予定タスクなし
	if (_addtask.empty()) {
		return;
	}

	//結合
	_task.insert(_task.end(), _addtask.begin(), _addtask.end());
	_addtask.clear();
	_addtask.shrink_to_fit();
}

void TaskSystem::StateDeleteTask()
{
	//関数オブジェクト　ラムダ式
	auto deleteCondition = [](BTask::SP& obj)
	{
		return (obj->_state == BTask::Task_State::Delete);
	};

	//オブジェクトの削除
	{
		//-----------------------------------------------------------------------------------------------------------------
		const auto& removeit = remove_if(_task.begin(), _task.end(), deleteCondition);
		_task.erase(removeit, _task.end());

		//領域の切りつめ
		_task.shrink_to_fit();
	}

	//データ削除
	//グループ数分 loop
	for (auto map = _taskdata.begin();map != _taskdata.end();) {
		//固有名分ループ
		for (auto itg = map->second.begin();itg != map->second.end();) {
			//-----------------------------------------------------------------------------------------------------------------
			//領域の解放
			const auto& removeit = remove_if(itg->second.begin(), itg->second.end(), deleteCondition);
			itg->second.erase(removeit, itg->second.end());
			
			//領域の切りつめ
			itg->second.shrink_to_fit();

			//固有名の数0の場合削除
			if (itg->second.empty()) {
				itg = map->second.erase(itg);
			}
			else {
				++itg;
			}
		}
		//グループの中が0の場合削除
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
	//単純にソート(昇順)
	sort(_task.begin(), _task.end(),[](BTask::SP& left, BTask::SP& right)
		{
			return (left->_priority < right->_priority);
		}
	);
}





