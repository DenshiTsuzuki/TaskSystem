#pragma once
#include<Siv3D.hpp>
#include"TaskSystem.h"

namespace Task_One	{
	//タスクに割り当てるグループネームと固有名
	const string defGroupName("グループネーム");
	const string defTaskName("固有名");

	//--------------------------------------------------------------------------------------------------------
	//リソース管理
	class Resorce {
	public:
		using WP = weak_ptr<Resorce>;
		using SP = shared_ptr<Resorce>;
		
	
		//インスタンス
		static WP instance;

		//コンストラクタ！
		Resorce();

		//デストラクタ！
		~Resorce();

		//リソース生成
		static Resorce::SP Create();

	//-----------------------------------------------------
	//						追加変数　
	//-----------------------------------------------------
	};

	//------------------------------------------------------------------------------------------------------
	class Object : public BTask {
	public:
		using WP = weak_ptr<Object>;
		using SP = shared_ptr  <Object>;
		//デストラクタ！
		~Object();

		//生成したリソースへのアクセス
		Resorce::SP res;

		//生成する
		static Object::SP Create();
	
		//コンストラクタ！
		Object();

		//初期化
		void Init()override;
		
		//終了処理
		void Finalize()override;

		//更新
		void Update()override;

		//描画
		void Draw()override;

	//-----------------------------------------------------
	//				追加変数　追加メソッド
	//-----------------------------------------------------
		Vec2 _pos;		//位置
		Rect _shape;	//形
	};

};

