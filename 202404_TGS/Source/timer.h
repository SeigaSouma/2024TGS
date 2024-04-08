//=============================================================================
// 
//  �^�C�}�[�w�b�_�[ [timer.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TIMER_H_
#define _TIMER_H_	// ��d�C���N���[�h�h�~

#include "main.h"

//==========================================================================
// �O���錾
//==========================================================================
class CMultiNumber;
class CObject2D;
class CObjectCircleGauge2D;

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int NUM_CIRCLE = 10;	// �~�̐�
	const float START_TIME = 120;	// ��������
}

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�}�[�N���X��`
class CTimer
{
public:

	enum eState
	{
		STATE_WAIT = 0,		// �ҋ@
		STATE_APPEARANCE,	// �o��
		STATE_ADD_LITTLE,	// �������Z
		STATE_ADJ,			// ����
		STATE_MAX
	};

	CTimer(int nPriority = 6);
	~CTimer();

	static CTimer *Create();
	static CTimer *Create(MyLib::Vector3 pos);
	static CTimer *GetInstance() { return m_pTimer; }

	HRESULT Init(MyLib::Vector3 pos);
	void Uninit();
	void Update();
	void Draw();
	float GetTime() { return m_fTime; }

	CTimer::eState GetState() { return m_state; }
	void SetState(eState state) { m_state = state; }

private:

	// �֐����X�g
	typedef void(CTimer::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// ��Ԋ֐����X�g

	// �����o�֐�
	void StateWait();
	void StatAppearance();
	void StatAddLittle();
	void StateAdjustment();

	// �����o�ϐ�
	eState m_state;					// ���
	float m_fStateTime;				// ��Ԏ���
	float m_fTime;					// ����
	MyLib::Vector3 m_pos;				// �ʒu
	MyLib::Vector3 m_posOrigin;		// ���̈ʒu
	bool m_bAddTime;	// �^�C�}�[���Z�̃t���O
	int m_nProgress;	// �i�s��
	CObjectCircleGauge2D *m_apCircle[NUM_CIRCLE];	// ���������̉~
	static CTimer *m_pTimer;	// ���g�̃|�C���^
};



#endif