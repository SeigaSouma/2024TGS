//=============================================================================
// 
//  �������� [explosion.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "explosion.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "explosion.h"
#include "3D_effect.h"
#include "sound.h"
#include "particle.h"
#include "enemy.h"
#include "calculation.h"
#include "game.h"
#include "player.h"
#include "impactwave.h"

//==========================================================================
//  �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\explosion.png";
	const int TIME_ADDSIZE = 20;		// �T�C�Y���Z����
	const int TIME_FADEOUT = 40;		// �t�F�[�h�A�E�g����
	const int MAX_LIFE = 60 * 2;		// �ő����
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CExplosion::STATE_FUNC CExplosion::m_FuncList[] =
{
	&CExplosion::StateNone,		// �Ȃɂ��Ȃ�
	&CExplosion::StateDamage,	// �_���[�W
	&CExplosion::StateFadeout,	// �t�F�[�h�A�E�g
};

CExplosion::COLLISION_FUNC CExplosion::m_CollisionFuncList[] =	// �����蔻��̃��X�g
{
	&CExplosion::CollisionEnemy,	// �G�Ƃ̔���
	&CExplosion::CollisionPlayer,	// �v���C���[�Ƃ̔���
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CExplosion::CExplosion(int nPriority) : CMeshSphere(nPriority)
{
	// �l�̃N���A
	m_type = TYPE_PLAYER;	// �����̎��
	m_state = STATE_NONE;	// ���
	m_nCntState = 0;		// ��ԑJ�ڃJ�E���^�[
	m_nLife = 0;			// ����
	m_nLifeMax = 0;			// �����̍ő�l
	m_nTexIdx = 0;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nCntEmission = 0;		// �������̃J�E���^�[
	m_fDestSize = 0.0f;		// �ڕW�̃T�C�Y
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CExplosion::~CExplosion()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CExplosion *CExplosion::Create(TYPE type, const MyLib::Vector3 pos, const float fSize)
{
	// �����p�̃I�u�W�F�N�g
	CExplosion *pBullet = nullptr;

	if (pBullet == nullptr)
	{// nullptr��������

		// �������̊m��
		pBullet = DEBUG_NEW CExplosion;

		if (pBullet != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ��ސݒ�
			pBullet->m_type = type;

			// �ʒu���蓖��
			pBullet->SetPosition(pos);

			// �T�C�Y�ݒ�
			pBullet->SetWidthLen(0.0f);
			pBullet->SetHeightLen(0.0f);
			pBullet->SetWidthBlock(16);
			pBullet->SetHeightBlock(16);
			pBullet->m_fDestSize = fSize;	// �ڕW�̃T�C�Y

			// ����������
			pBullet->Init();
		}

		return pBullet;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CExplosion::Init()
{
	HRESULT hr;

	// �e��ϐ��̏�����
	m_nLifeMax = MAX_LIFE;
	m_nLife = m_nLifeMax;	// ����
	SetColor(D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f));

	// �e�N�X�`���̊��蓖��
	m_nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);

	// �e�N�X�`���̊��蓖��
	BindTexture(m_nTexIdx);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTMESH);

	// ����������
	hr = CMeshSphere::Init();
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CExplosion::Uninit()
{
	// �I������
	CMeshSphere::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CExplosion::Update()
{
	// �ʒu�X�V
	UpdatePos();

	// ��ԕʏ���
	(this->*(m_FuncList[m_state]))();

	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	// �������Z
	m_nLife--;

	if (m_nLife <= 0)
	{// �������s������
		m_state = STATE_FADEOUT;
	}

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �ړ�����
//==========================================================================
void CExplosion::UpdatePos()
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �ʒu�X�V
	pos += move;

	// ��]
	rot.y += D3DX_PI * 0.025f;
	UtilFunc::Transformation::RotNormalize(rot.y);

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);
}

//==========================================================================
// �ʏ��ԏ���
//==========================================================================
void CExplosion::StateNone()
{
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();

	fWidthLen = UtilFunc::Correction::EasingEaseInOut(0.0f, m_fDestSize, static_cast<float>((m_nLifeMax - m_nLife)) / static_cast<float>(TIME_ADDSIZE));
	fHeightLen = UtilFunc::Correction::EasingEaseInOut(0.0f, m_fDestSize, static_cast<float>((m_nLifeMax - m_nLife)) / static_cast<float>(TIME_ADDSIZE));
	SetWidthLen(fWidthLen);
	SetHeightLen(fHeightLen);

	if (m_nLifeMax - m_nLife == TIME_ADDSIZE)
	{
		m_state = STATE_DMG;
		return;
	}

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;
	if (m_nCntState <= 0)
	{
		m_nCntState = 0;
		m_state = STATE_NONE;
	}
}

//==========================================================================
// �_���[�W��ԏ���
//==========================================================================
void CExplosion::StateDamage()
{
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();

	fWidthLen += 0.5f;
	fHeightLen += 0.5f;
	SetWidthLen(fWidthLen);
	SetHeightLen(fHeightLen);

	// �����蔻�菈��
	(this->*(m_CollisionFuncList[m_type]))();
}

//==========================================================================
// �t�F�[�h�A�E�g��ԏ���
//==========================================================================
void CExplosion::StateFadeout()
{
	// �F�擾
	D3DXCOLOR col = GetColor();

	// �s�����x�X�V
	col.a = static_cast<float>(m_nLife + TIME_FADEOUT) / static_cast<float>(TIME_FADEOUT);

	// �F�ݒ�
	SetColor(col);

	if (m_nLife + TIME_FADEOUT <= 0)
	{
		// �I������
		Uninit();
		return;
	}

}

//==========================================================================
// �v���C���[�Ƃ̔���
//==========================================================================
void CExplosion::CollisionPlayer()
{

	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// ���X�g���[�v
	while (playerList.ListLoop(&pPlayer))
	{
		// �v���C���[�̏��擾
		MyLib::Vector3 PlayerPosition = pPlayer->GetCenterPosition();
		MyLib::Vector3 PlayerRotation = pPlayer->GetRotation();
		float fPlayerRadius = pPlayer->GetRadius();

		// ���擾
		MyLib::Vector3 pos = GetPosition();
		float fRadius = GetWidthLen();

		if (UtilFunc::Collision::SphereRange(pos, PlayerPosition, fRadius, fPlayerRadius).ishit)
		{// �������Ă�����

			// �q�b�g����
			pPlayer->Hit(1);

			// �I������
			//Uninit();
			break;
		}
	}

}

//==========================================================================
// �G�Ƃ̔���
//==========================================================================
void CExplosion::CollisionEnemy()
{
	// ���擾
	MyLib::Vector3 pos = GetPosition();
	float fRadius = GetWidthLen();
	bool bHit = false;

	// �G�̃��X�g�擾
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;

	// ���X�g���[�v
	while (enemyList.ListLoop(&pEnemy))
	{
		// �G�̏��擾
		MyLib::Vector3 EnemyPosition = pEnemy->GetCenterPosition();
		float fEnemyRadius = pEnemy->GetRadius();

		if (UtilFunc::Collision::SphereRange(pos, EnemyPosition, fRadius, fEnemyRadius).ishit)
		{// �������Ă�����
			bHit = true;
			break;
		}
	}

	if (bHit == true)
	{// �������Ă���

		// �I������
		Uninit();
		return;
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CExplosion::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �r���{�[�h�̕`��
	CMeshSphere::Draw();

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CExplosion::SetVtx()
{
	// ���_�ݒ�
	CMeshSphere::SetVtx();
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CExplosion::SetState(STATE state, int nCntState)
{
	m_state = state;			// ���
	m_nCntState = nCntState;	// ��ԑJ�ڃJ�E���^�[
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
CExplosion::STATE CExplosion::GetState()
{
	return m_state;
}

//==========================================================================
// ��ގ擾
//==========================================================================
CExplosion::TYPE CExplosion::GetType()
{
	return m_type;
}