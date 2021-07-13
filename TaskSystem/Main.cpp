#pragma once
#include <Siv3D.hpp>
#include"TaskSystem.h"
#include"Task_One.h"
void Main()
{

	while (System::Update())
	{
		if (MouseL.down()) {
			auto ts = Task_One::Object::Create();
		}

		if (MouseR.down()) {
			ML::Task_System.Kill_All_G("グループネーム");
		}
		ML::Task_System.Update();
		ML::Task_System.Draw();
	}

	ML::Task_System.AllDeleteTask();
}

