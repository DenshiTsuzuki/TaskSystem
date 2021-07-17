#pragma once
#include <Siv3D.hpp>
#include"TaskSystem.h"
#include"Task_One.h"
void Main()
{
	//起動時生成するタスクをここに書く

	while (System::Update())
	{
		if (MouseL.down()) {
			auto ts = Task_One::Object::Create();
		}

		if (MouseR.down()) {
			auto ts =  ML::Task_System.GetTasks<Task_One::Object>("グループネーム","固有名");
			Print << ts->size();

		}
		ML::Task_System.Update();
		ML::Task_System.Draw();
	}

	ML::Task_System.AllDeleteTask();
}

