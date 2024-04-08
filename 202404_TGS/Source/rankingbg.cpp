//=============================================================================
// 
//  �����L���O�w�i���� [rankingbg.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "rankingbg.h"
#include "manager.h"
#include "camera.h"
#include "pollen_gauge.h"
#include "game.h"
#include "gamemanager.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\ranking_bg.png";
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRankingBG::CRankingBG(int nPriority) : CObject2D(nPriority)
{
	//������
	m_fMoveOffset = 0.0f;
	m_fSpeed = 0.0f;
	m_moveDir = MOVE_LEFT;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRankingBG::~CRankingBG()
{

}

//==========================================================================
// ��������
//==========================================================================
CRankingBG* CRankingBG::Create(float fSpeed)
{
	// �����p�̃I�u�W�F�N�g
	CRankingBG* pBusket = nullptr;

	// �������̊m��
	pBusket = DEBUG_NEW CRankingBG;

	if (pBusket != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		//�����l
		pBusket->m_fSpeed = fSpeed;

		// ����������
		pBusket->Init();

		// ��ނ̐ݒ�
		pBusket->SetType(TYPE_HPGAUGE);
	}

	return pBusket;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CRankingBG::Init()
{
	CObject2D::Init();
	BindTexture(CTexture::GetInstance()->Regist(TEXTURE));
	m_moveDir = MOVE_RIGHT;
	SetSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRankingBG::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CRankingBG::Update()
{
	//�ړ�
	float fMove = 0.0f;
	switch (m_moveDir)
	{
	case CRankingBG::MOVE_LEFT:
		m_fMoveOffset -= m_fSpeed;
		if (m_fMoveOffset <= 0.0f)
		{
			m_moveDir = MOVE_RIGHT;
		}
		fMove = UtilFunc::Correction::EasingEaseIn(0.0f, 0.25f, m_fMoveOffset);
		break;
	case CRankingBG::MOVE_RIGHT:
		m_fMoveOffset += m_fSpeed;
		if (m_fMoveOffset >= 1.0f)
		{
			m_moveDir = MOVE_LEFT;
		}
		fMove = UtilFunc::Correction::EasingEaseIn(0.0f, 0.25f, m_fMoveOffset);
		break;
	case CRankingBG::MOVE_MAX:
		break;
	default:
		break;
	}

	//�e�N�X�`���ݒ�
	D3DXVECTOR2 aTex[4];
	aTex[0] = D3DXVECTOR2(fMove, 0.25f);
	aTex[1] = D3DXVECTOR2(0.75f + fMove, 0.25f);
	aTex[2] = D3DXVECTOR2(fMove, 1.0f);
	aTex[3] = D3DXVECTOR2(0.75f + fMove, 1.0f);
	SetTex(&aTex[0]);

	CObject2D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CRankingBG::Draw()
{
	CObject2D::Draw();
}