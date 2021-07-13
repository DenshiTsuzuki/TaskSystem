#pragma once
#include<vector>
#include<memory>
#include<string>
#include<map>

using namespace std;
//タスクシステム基本クラス
class BTask
{
public:
	using SP = shared_ptr<BTask>;
	using WP = weak_ptr<BTask>;
	//タスクの状態
	enum struct Task_State {
		Active,		//動作中
		Stop,		//停止状態
		Kill,		//破棄
		Ready,		//準備中
		Delete			//無効
	};
	
	//グループネーム
	const string groupname;

	//固有名
	const string name;

	//タスク描画優先順位
	float _priority;

	//状態
	Task_State _state;

public:
	//コンストラクタ！
	BTask(const string& gn_, const string& n_);

	//デストラクタ！
	virtual ~BTask();

	//初期化処理
	virtual void Init() = 0;

	//更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

	//終了
	virtual void Finalize() = 0;

	//削除申請
	void Kill();

	//一時停止
	void Stop();
};

