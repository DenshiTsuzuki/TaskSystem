#pragma once
#include<vector>
#include<memory>
#include<string>
#include<map>

using namespace std;
//�^�X�N�V�X�e����{�N���X
class BTask
{
public:
	using SP = shared_ptr<BTask>;
	using WP = weak_ptr<BTask>;
	//�^�X�N�̏��
	enum struct Task_State {
		Active,		//���쒆
		Stop,		//��~���
		Kill,		//�j��
		Ready,		//������
		Delete			//����
	};
	
	//�O���[�v�l�[��
	const string groupname;

	//�ŗL��
	const string name;

	//�^�X�N�`��D�揇��
	float _priority;

	//���
	Task_State _state;

public:
	//�R���X�g���N�^�I
	BTask(const string& gn_, const string& n_);

	//�f�X�g���N�^�I
	virtual ~BTask();

	//����������
	virtual void Init() = 0;

	//�X�V
	virtual void Update() = 0;

	//�`��
	virtual void Draw() = 0;

	//�I��
	virtual void Finalize() = 0;

	//�폜�\��
	void Kill();

	//�ꎞ��~
	void Stop();
};

