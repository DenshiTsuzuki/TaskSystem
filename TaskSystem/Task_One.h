#pragma once
#include<Siv3D.hpp>
#include"TaskSystem.h"

namespace Task_One	{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v�l�[���ƌŗL��
	const string defGroupName("�O���[�v�l�[��");
	const string defTaskName("�ŗL��");

	//--------------------------------------------------------------------------------------------------------
	//���\�[�X�Ǘ�
	class Resorce {
	public:
		using WP = weak_ptr<Resorce>;
		using SP = shared_ptr<Resorce>;
		
	
		//�C���X�^���X
		static WP instance;

		//�R���X�g���N�^�I
		Resorce();

		//�f�X�g���N�^�I
		~Resorce();

		//���\�[�X����
		static Resorce::SP Create();

	//-----------------------------------------------------
	//						�ǉ��ϐ��@
	//-----------------------------------------------------
	};

	//------------------------------------------------------------------------------------------------------
	class Object : public BTask {
	public:
		using WP = weak_ptr<Object>;
		using SP = shared_ptr  <Object>;
		//�f�X�g���N�^�I
		~Object();

		//�����������\�[�X�ւ̃A�N�Z�X
		Resorce::SP res;

		//��������
		static Object::SP Create();
	
		//�R���X�g���N�^�I
		Object();

		//������
		void Init()override;
		
		//�I������
		void Finalize()override;

		//�X�V
		void Update()override;

		//�`��
		void Draw()override;

	//-----------------------------------------------------
	//				�ǉ��ϐ��@�ǉ����\�b�h
	//-----------------------------------------------------
		Vec2 _pos;		//�ʒu
		Rect _shape;	//�`
	};

};

