//=============================================================================
// 
//  �v���C���[���� [enemy.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "input.h"
#include "particle.h"
#include "motion.h"
#include "model.h"
#include "elevation.h"
#include "player.h"
#include "camera.h"
#include "ballast.h"
#include "3D_Effect.h"
#include "hp_gauge.h"
#include "shadow.h"
#include "sound.h"
#include "impactwave.h"
#include "stage.h"
#include "objectX.h"
#include "limitarea.h"
#include "debugproc.h"
#include "rockon_marker.h"

// �q�N���X
#include "enemy_boss.h"
#include "enemy_gobelin.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float PLAYER_SERCH = 800.0f;	// �v���C���[�T���͈�
	const float CHACE_DISTABCE = 50.0f;	// �ǂ��|���鎞�̊Ԋu
	const float TIME_DMG = static_cast<float>(10) / static_cast<float>(mylib_const::DEFAULT_FPS);	// �_���[�W����
	const float TIME_DEAD = static_cast<float>(40) / static_cast<float>(mylib_const::DEFAULT_FPS);	// ���S����
	const float TIME_DOWN = static_cast<float>(150) / static_cast<float>(mylib_const::DEFAULT_FPS);	// �_�E������
	const float TIME_FADEOUT = static_cast<float>(30) / static_cast<float>(mylib_const::DEFAULT_FPS);	// ���S����
}
int CEnemy::m_nNumSurvival = 0;			// �����l��

//==========================================================================
// �֐��|�C���^
//==========================================================================
// ��Ԋ֐�
CEnemy::STATE_FUNC CEnemy::m_StateFunc[] =
{
	&CEnemy::StateNone,		// �Ȃ�
	&CEnemy::StateSpawnWait,// �X�|�[���ҋ@
	&CEnemy::StateSpawn,	// �X�|�[��
	&CEnemy::StateDamage,	// �_���[�W
	&CEnemy::StateDead,		// ���S
	&CEnemy::StateFadeOut,	// �t�F�[�h�A�E�g
	&CEnemy::StateWait,		// �ҋ@
	&CEnemy::StateDown,		// �_�E��
	&CEnemy::StateStrongAtk,// ���U��
	&CEnemy::StateDownWait,	// �_�E���ҋ@���
};

// �s���֐�
CEnemy::ACT_FUNC CEnemy::m_ActFuncList[] =
{
	&CEnemy::ActDefault,	// �ʏ�s��
	&CEnemy::ActWait,		// �ҋ@�s��
};

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CListManager<CEnemy> CEnemy::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEnemy::CEnemy(int nPriority) : CObjectChara(nPriority)
{
	// �l�̃N���A
	m_posKnokBack = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// �m�b�N�o�b�N�̈ʒu
	m_type = TYPE_BOSS;	// ���
	m_state = STATE_NONE;	// ���
	m_Oldstate = m_state;	// �O��̏��
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �}�e���A���̐F
	m_TargetPosition = mylib_const::DEFAULT_VECTOR3;	// �ڕW�̈ʒu
	m_pWeaponHandle = 0;		// �G�t�F�N�g�̕���n���h��

	m_Action = ACTION_DEF;		// �s��
	m_fActTime = 0.0f;			// �s���J�E���^�[
	m_fStrongAttackTime = 0.0f;	// ���U���̃^�C�}�[
	m_fStateTime = 0.0f;		// ��ԑJ�ڃJ�E���^�[
	m_nNumChild = 0;			// �q�̐�
	m_bDecrementSurvival = false;	// �����l�������t���O
	m_nTargetPlayerIndex = 0;	// �ǂ��|����v���C���[�̃C���f�b�N�X�ԍ�
	m_bDamageReceived = false;				// �_���[�W�󂯕t������
	m_fDamageReciveTime = 0.0f;				// �_���[�W��t����
	m_bActiveSuperArmor = false;			// �X�[�p�[�A�[�}�[
	m_bActionable = false;		// �s���\��
	m_fDownTime = 0.0f;			// �_�E���J�E���^�[
	m_fRockOnDistance = 0.0f;	// ���b�N�I���̋���
	m_bRockOnAccepting = false;	// ���b�N�I����t

	m_bStateChanging = false;	// ��Ԃ��؂�ւ�����u��
	m_pATKState = nullptr;		// ���̍s���|�C���^
	m_pNextATKState = nullptr;	// ���̍s���|�C���^
	m_pReturnDown = nullptr;	// �_�E�����A
	m_bCatchUp = false;						// �ǂ���������
	m_bInSight = false;						// ���E������
	m_sMotionFrag = SMotionFrag();		// ���[�V�����̃t���O
	m_pParent = nullptr;					// �e�̃|�C���^
	m_pHPGauge = nullptr;					// HP�Q�[�W�̏��
	m_pShadow = nullptr;
	m_pRockOnMarker = nullptr;		// ���b�N�I���}�[�J�[


	memset(&m_pChild[0], 0, sizeof(m_pChild));	// �q�̃|�C���^
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEnemy::~CEnemy()
{

}

//==========================================================================
// ��������
//==========================================================================
CEnemy* CEnemy::Create(const char* pFileName, MyLib::Vector3 pos, TYPE type)
{
	// �����p�̃I�u�W�F�N�g
	CEnemy* pEnemy = nullptr;

	// �������̊m��
	switch (type)
	{
	case TYPE_BOSS:
		pEnemy = DEBUG_NEW CEnemyBoss;
		break;

	case TYPE_GOBELIN:
		pEnemy = DEBUG_NEW CEnemyGobelin;
		break;

	default:
		return nullptr;
		break;
	}

	if (pEnemy != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ���
		pEnemy->m_type = type;

		// �ʒu�ݒ�
		pEnemy->SetPosition(pos);
		pEnemy->CObject::SetOriginPosition(pos);

		// �e�L�X�g�ǂݍ���
		HRESULT hr = pEnemy->RoadText(pFileName);
		if (FAILED(hr))
		{// ���s���Ă�����
			return nullptr;
		}

		// ����������
		pEnemy->Init();
	}

	return pEnemy;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEnemy::Init()
{
	// �e��ϐ��̏�����
	m_state = STATE_NONE;	// ���
	m_Oldstate = STATE_NONE;
	m_fStateTime = 0.0f;			// ��ԑJ�ڃJ�E���^�[
	m_posKnokBack = GetOriginPosition();	// �m�b�N�o�b�N�̈ʒu
	m_fRockOnDistance = 0.0f;	// ���b�N�I���̋���
	m_bDamageReceived = true;	// �_���[�W��t�t���O

	// ��ނ̐ݒ�
	SetType(TYPE_ENEMY);

	// �����ݒ�
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// �f�t�H���g�ݒ�
	CMotion* pMotion = GetMotion();
	if (pMotion != nullptr)
	{
		pMotion->Set(MOTION_DEF);
	}

	// �����l�����Z
	m_nNumSurvival++;

	return S_OK;
}

//==========================================================================
// �e�L�X�g�ǂݍ���
//==========================================================================
HRESULT CEnemy::RoadText(const char *pFileName)
{
	// �L�����쐬
	HRESULT hr = SetCharacter(pFileName);
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �q�o�^
//==========================================================================
void CEnemy::RegistrChild(CEnemy *pChild)
{
	if (m_nNumChild >= mylib_const::MAX_ENEMY)
	{
		return;
	}

	if (m_pChild[m_nNumChild] == nullptr)
	{
		m_pChild[m_nNumChild] = pChild;
		m_nNumChild++;	// �q�̐����Z
	}
	else
	{
		int n = 0;
	}
}

//==========================================================================
// �q�̃��Z�b�g
//==========================================================================
void CEnemy::ResetChild(CEnemy *pChild)
{
	int nMax = m_nNumChild;

	for (int nCntChild = 0; nCntChild < nMax; nCntChild++)
	{
		if (m_pChild[nCntChild] == nullptr)
		{
			continue;
		}

		if (m_pChild[nCntChild] == pChild)
		{
			m_pChild[nCntChild] = nullptr;
		}
	}
}


//==========================================================================
// �e�̃|�C���^�ݒ�
//==========================================================================
void CEnemy::SetParent(CEnemy *pParent)
{
	if (this != nullptr)
	{
		// �q�̊��蓖��
		if (pParent != nullptr)
		{
			pParent->RegistrChild(this);
		}

		// �e�̃|�C���^�n��
		m_pParent = pParent;
		
	}
	else
	{
		int n = 0;
	}
}

//==========================================================================
// �I������
//==========================================================================
void CEnemy::Uninit()
{
	// �����l������
	if (!m_bDecrementSurvival)
	{
		m_nNumSurvival--;
		m_bDecrementSurvival = true;
	}

	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}

	if (m_pRockOnMarker != nullptr)
	{
		m_pRockOnMarker = nullptr;
	}

	for (int i = 0; i < static_cast<int>(m_pAtkPattern.size()); i++)
	{
		delete m_pAtkPattern[i];
		m_pAtkPattern[i] = nullptr;
	}

	if (m_pReturnDown != nullptr)
	{
		delete m_pReturnDown;
		m_pReturnDown = nullptr;
	}

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectChara::Uninit();
}

//==========================================================================
// ���S����
//==========================================================================
void CEnemy::Kill()
{
	for (int nCntEnemy = 0; nCntEnemy < mylib_const::MAX_ENEMY; nCntEnemy++)
	{// �q�̐�����

		if (m_pChild[nCntEnemy] != nullptr)
		{// �q������ꍇ

			// �����̎q�̐e��nullptr�ɂ���
			m_pChild[nCntEnemy]->m_pParent = nullptr;
		}
	}

	if (m_pParent != nullptr)
	{// �����ɐe������ꍇ

		// �e�̎q��nullptr�ɂ���
		m_pParent->ResetChild(this);
	}

	// HP�Q�[�W������
	if (m_pHPGauge != nullptr)
	{
		m_pHPGauge->Kill();
		m_pHPGauge = nullptr;
	}

	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// ���b�N�I���}�[�J�[
	if (m_pRockOnMarker != nullptr)
	{
		m_pRockOnMarker->Kill();
		m_pRockOnMarker = nullptr;
	}

	// ���b�N�I����t���Ă���
	if (m_bRockOnAccepting)
	{
		CManager::GetInstance()->GetCamera()->SetRockOn(GetPosition(), false);
	}

}


//==========================================================================
// �U����Ԑ؂�ւ�
//==========================================================================
void CEnemy::ChangeATKState(CEnemyState* state)
{
	// ��Ԃ��؂�ւ�����u�ԃt���O
	m_bStateChanging = true;

	if (m_pATKState != nullptr &&
		!m_pATKState->IsCreateFirstTime())
	{
		delete m_pATKState;
	}
	m_pATKState = state;

	m_bCatchUp = false;
	m_bInSight = false;
}

//==========================================================================
// �U������
//==========================================================================
void CEnemy::PerformAttack()
{
	if (m_pATKState == nullptr)
	{
		return;
	}

	if (m_bActionable)
	{
		m_pATKState->Action(this);
	}
}

//==========================================================================
// �U����Ԑ؂�ւ�
//==========================================================================
void CEnemy::DrawingRandomAction()
{
	// �����_���ɃA�N�V�����p�^�[����I�����Ď��s
	if (!m_pAtkPattern.empty())
	{
		if (m_state == STATE_STRONGATK)
		{
			m_state = STATE_NONE;
		}

		int randomIndex = rand() % m_pAtkPattern.size();

		if (!m_pAtkPattern[randomIndex]->IsDirectlyTrans())
		{// ��������ōs������ꍇ

			// ���̍s���ݒ�
			ChangeNextATKState(m_pAtkPattern[randomIndex]);

			// �J�ڑO����
			m_pAtkPattern[randomIndex]->BeforeTransitionProcess(this);
		}
		else
		{
			ChangeATKState(m_pAtkPattern[randomIndex]);
			m_bCatchUp = false;
			m_bInSight = false;

			// ���[�V�����C���f�b�N�X�؂�ւ�
			m_pATKState->ChangeMotionIdx(this);
		}
	}
}

//==========================================================================
// ���̍U���֐؂�ւ�
//==========================================================================
void CEnemy::ChangeNextAction()
{
	// �ۑ����Ă������̍s����ݒ�
	ChangeATKState(m_pNextATKState);
	m_bCatchUp = false;
	m_bInSight = false;

	// ���[�V�����C���f�b�N�X�؂�ւ�
	m_pATKState->ChangeMotionIdx(this);
}

//==========================================================================
// �X�V����
//==========================================================================
void CEnemy::Update()
{
	// ���S�̔���
	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	// ��Ԃ��؂�ւ�����u�ԃt���O
	m_bStateChanging = false;

	// �G�f�B�b�g���͔�����
	if (CGame::GetInstance()->GetElevation()->IsEdit())
	{
		return;
	}

	// �e�̏���
	CObjectChara::Update();

	// �v���C���[���
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(m_nTargetPlayerIndex);
	if (pPlayer == nullptr)
	{
		return;
	}
	m_TargetPosition = pPlayer->GetPosition();

	if (!CGame::GetInstance()->GetGameManager()->IsControll())
	{// �s���s�\

		if (m_state != STATE::STATE_DEAD &&
			m_state != STATE::STATE_FADEOUT)
		{
			RotationTarget();
		}
		return;
	}

	// �ߋ��̈ʒu�ݒ�
	SetOldPosition(GetPosition());

	// �s���ʏ���
	(this->*(m_ActFuncList[m_Action]))();

	// �����蔻��
	Collision();

	// ���S�̔���
	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	// ���[�V�����̐ݒ菈��
	MotionSet();

	// ��ԍX�V
	UpdateState();

	// ���S�̔���
	if (IsDeath() == true)
	{// ���S�t���O�������Ă�����
		return;
	}

	// HP�Q�[�W�̈ʒu�X�V
	if (m_pHPGauge != nullptr)
	{
		// �ʒu�擾
		MyLib::Vector3 pos = GetPosition();

		// �̗͎擾
		int nLife = GetLife();

		m_pHPGauge->UpdatePosition(pos, nLife);
	}

	// �e�̈ʒu�X�V
	if (m_pShadow != nullptr)
	{
		// �ʒu�擾
		MyLib::Vector3 pos = GetPosition();
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

	if (GetPosition().y <= mylib_const::KILL_Y)
	{
		// �G�̏I������
		Kill();
		Uninit();
		return;
	}

	// ��l�̕�
	LimitArea();

	// ���b�N�I����t���Ă���
	if (m_bRockOnAccepting)
	{
		MyLib::Vector3 rockonpos = GetPosition();
		MyLib::Vector3 centerpos = GetCenterPosition();
		rockonpos.x = centerpos.x;
		rockonpos.z = centerpos.z;
		rockonpos.y += GetHeight() * 0.5f;
		CManager::GetInstance()->GetCamera()->SetRockOn(rockonpos, true);
		CManager::GetInstance()->GetCamera()->SetRockOnDistance(m_fRockOnDistance);

		// ���b�N�I���}�[�J�[�̈ʒu�ݒ�
		if (m_pRockOnMarker == nullptr)
		{
			m_pRockOnMarker = CRockOnMarker::Create(rockonpos);
		}
		m_pRockOnMarker->SetPosition(rockonpos);
	}
	else
	{
		if (m_pRockOnMarker != nullptr)
		{
			m_pRockOnMarker->Kill();
			m_pRockOnMarker = nullptr;
		}
	}
}

//==========================================================================
// �����蔻��
//==========================================================================
void CEnemy::Collision()
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �d�͏���
	move.y -= mylib_const::GRAVITY;

	// �ʒu�X�V
	pos += move;

	// �����␳
	if (m_state != STATE_SPAWN && m_state != STATE_DMG && m_state != STATE_DEAD)
	{
		move.x += (0.0f - move.x) * 0.25f;
		move.z += (0.0f - move.z) * 0.25f;
	}

	if (move.x >= 0.1f || move.x <= -0.1f ||
		move.z >= 0.1f || move.z <= -0.1f)
	{// �ړ���
		m_sMotionFrag.bMove = true;
	}
	else
	{
		m_sMotionFrag.bMove = false;
	}

	// �����擾
	bool bLand = false;
	float fHeight = CGame::GetInstance()->GetElevation()->GetHeight(pos, &bLand);

	if (fHeight > pos.y)
	{// �n�ʂ̕���������荂��������

		// �n�ʂ̍����ɕ␳
		pos.y = fHeight;

		if (bLand == true)
		{// ���n���Ă���

			// ���n������
			ProcessLanding();

			// �W�����v�g�p�\�ɂ���
			move.y = 0.0f;
			m_sMotionFrag.bJump = false;
		}
	}


	if (pos.y <= -800.0f)
	{
		pos.y = -800.0f;
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);
}

//==========================================================================
// ���n���̏���
//==========================================================================
void CEnemy::ProcessLanding()
{
	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �W�����v�g�p�\�ɂ���
	if (m_state != STATE_DMG && m_state != STATE_DEAD)
	{
		move.y = 0.0f;
	}
	m_sMotionFrag.bJump = false;

	// �ړ��ʐݒ�
	SetMove(move);
}

//==========================================================================
// �q�b�g����
//==========================================================================
bool CEnemy::Hit(const int nValue, const MyLib::Vector3& hitpos, CGameManager::AttackType atkType)
{
	bool bHit = false;

	// �����擾
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 pos = GetPosition();

	// �̗͎擾
	int nLife = GetLife();

	if (m_bDamageReceived)
	{// �_���[�W��t���̂�

		// ��������
		bHit = true;

		// �s���\����
		if (!m_bActiveSuperArmor)
		{
			m_bActionable = false;
		}

		// �_���[�W���󂯕t���Ȃ�
		m_bDamageReceived = false;

		// �̗͌��炷
		nLife -= nValue;
		UtilFunc::Transformation::ValueNormalize(nLife, GetLifeOrigin(), 0);

		// �̗͐ݒ�
		SetLife(nLife);

		// �_���[�W���Ԑݒ�
		m_fDamageReciveTime = TIME_DMG;
		if (nLife > 0)
		{
			// �_���[�W���Đ�
			//CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_DMG01);
		}
		else
		{// �̗͂��Ȃ��Ȃ�����

			// �s���\����
			m_bActionable = false;

			// �q�b�g�X�g�b�v
			CManager::GetInstance()->SetEnableHitStop(2);

			// �U��
			CManager::GetInstance()->GetCamera()->SetShake(5, 10.0f, 0.0f);

			// ���S��Ԃɂ���
			m_state = STATE_DEAD;

			// �J�ڃJ�E���^�[�ݒ�
			m_fStateTime = TIME_DEAD;

			// �m�b�N�o�b�N�̈ʒu�X�V
			m_posKnokBack = GetPosition();

			// �m�b�N�o�b�N����ɂ���
			m_sMotionFrag.bKnockback = true;

			// ���ꃂ�[�V����
			GetMotion()->Set(MOTION_KNOCKBACK);
			StateDead();

			// �����l�����Z
			if (!m_bDecrementSurvival)
			{
				m_nNumSurvival--;
				m_bDecrementSurvival = true;
			}

			// �����Đ�
			//CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_ENEMYEXPLOSION);

			return true;
		}

		// �ߋ��̏�ԕۑ�
		m_Oldstate = m_state;

		// �_���[�W���Ԑݒ�
		m_fDamageReciveTime = TIME_DMG;

		switch (atkType)
		{
		case CGameManager::ATTACK_NORMAL:
			NormalHitResponse();
			break;

		case CGameManager::ATTACK_STRONG:
			break;

		case CGameManager::ATTACK_COUNTER:
			CounterHitResponse();
			break;

		default:
			break;
		}
	}

	return bHit;
}

//==========================================================================
// �ʏ�q�b�g���̔���
//==========================================================================
void CEnemy::NormalHitResponse()
{
	if (m_pHPGauge == nullptr)
	{
		// �̗̓Q�[�W
		m_pHPGauge = CHP_Gauge::Create(GetHeight(), GetLifeOrigin(), 3.0f);
	}

	// �_���[�W��Ԃɂ���
	if (m_state != STATE_DOWN &&
		m_state != STATE_STRONGATK)
	{
		m_state = STATE_DMG;

		// ���ꃂ�[�V����
		if (!m_bActiveSuperArmor)
		{
			GetMotion()->Set(MOTION_DMG);
		}
	}

	// �J�ڃJ�E���^�[�ݒ�
	m_fStateTime = TIME_DMG;

	// �q�b�g�X�g�b�v
	//CManager::GetInstance()->SetEnableHitStop(5);

	// �U��
	CManager::GetInstance()->GetCamera()->SetShake(10, 15.0f, 0.0f);

	// �m�b�N�o�b�N�̈ʒu�X�V
	m_posKnokBack = GetPosition();
}

//==========================================================================
// �J�E���^�[�q�b�g���̔���
//==========================================================================
void CEnemy::CounterHitResponse()
{
	if (m_pHPGauge == nullptr)
	{
		// �̗̓Q�[�W
		m_pHPGauge = CHP_Gauge::Create(GetHeight(), GetLifeOrigin(), 3.0f);
	}

	// �_�E����Ԃɂ���
	m_state = STATE_DOWN;

	// �J�ڃJ�E���^�[�ݒ�
	m_fStateTime = TIME_DMG;

	// �_�E���J�E���^�[�ݒ�
	m_fDownTime = TIME_DOWN;

	// ���ꃂ�[�V����
	GetMotion()->Set(MOTION_DOWN);

	// �q�b�g�X�g�b�v
	CManager::GetInstance()->SetEnableHitStop(5);

	// �U��
	CManager::GetInstance()->GetCamera()->SetShake(10, 15.0f, 0.0f);

	// �m�b�N�o�b�N�̈ʒu�X�V
	m_posKnokBack = GetPosition();
}

//==========================================================================
// �_���[�W��t���ԍX�V
//==========================================================================
void CEnemy::UpdateDamageReciveTimer()
{
	// �_���[�W�󂯕t�����Ԍ��Z
	m_fDamageReciveTime -= CManager::GetInstance()->GetDeltaTime();
	if (m_fDamageReciveTime <= 0.0f)
	{
		// �_���[�W�󂯕t������
		m_bDamageReceived = true;
		m_fDamageReciveTime = 0.0f;
	}
}

//==========================================================================
// �ʏ�s��
//==========================================================================
void CEnemy::ActDefault()
{
	// �U�����s����
	PerformAttack();
}

//==========================================================================
// �ҋ@�s��
//==========================================================================
void CEnemy::ActWait()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	// �ҋ@���[�V�����ݒ�
	pMotion->Set(MOTION_DEF);

	// �^�[�Q�b�g�̕�������
	RotationTarget();

	// �s���J�E���^�[���Z
	m_fActTime += CManager::GetInstance()->GetDeltaTime();
}

//==========================================================================
// �ǂ��|��
//==========================================================================
void CEnemy::ActChase(float moveMultiply, float catchLen)
{
	// �ړ����[�V����
	GetMotion()->Set(MOTION_WALK);

	// �ړ��t���O�𗧂Ă�
	m_sMotionFrag.bMove = true;

	// ���擾
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 rot = GetRotation();
	float fMove = GetVelocity();

	// �ړ��ʐݒ�
	move.x += sinf(D3DX_PI + rot.y) * fMove * moveMultiply;
	move.z += cosf(D3DX_PI + rot.y) * fMove * moveMultiply;

	// �ړ��ʐݒ�
	SetMove(move);

	// �ǂ���������
	m_bCatchUp = UtilFunc::Collision::CircleRange3D(GetPosition(), m_TargetPosition, catchLen, 0.0f);
}

//==========================================================================
// �^�[�Q�b�g�̕�������
//==========================================================================
void CEnemy::RotationTarget(float range)
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	// �ڕW�̊p�x�����߂�
	float fRotDest = pos.AngleXZ(m_TargetPosition);

	// �ڕW�Ƃ̍���
	float fRotDiff = fRotDest - rot.y;

	//�p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(fRotDiff);

	//�p�x�̕␳������
	rot.y += fRotDiff * 0.1f;
	UtilFunc::Transformation::RotNormalize(rot.y);

	// �����ݒ�
	SetRotation(rot);

	// �ڕW�̌����ݒ�
	SetRotDest(fRotDest);


	// ���E����
	m_bInSight = UtilFunc::Collision::CollisionViewRange3D(GetPosition(), m_TargetPosition, rot.y, range);
}

//==========================================================================
// ��ԍX�V����
//==========================================================================
void CEnemy::UpdateState()
{
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_mMatcol.a);

	// �_���[�W��t���ԍX�V
	UpdateDamageReciveTimer();

	// �_�E���J�E���^�[���Z
	m_fDownTime -= CManager::GetInstance()->GetDeltaTime();
	if (m_fDownTime <= 0.0f)
	{
		m_fDownTime = 0.0f;
	}

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();

	if (m_state != STATE_SPAWNWAIT && IsDisp() == false)
	{
		// �`�悷��
		SetEnableDisp(true);
	}
}

//==========================================================================
// �����Ȃ����
//==========================================================================
void CEnemy::StateNone()
{
	// �s���\����
	m_bActionable = true;

	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTime -= CManager::GetInstance()->GetDeltaTime();

	if (m_fStateTime <= 0)
	{// �J�ڃJ�E���^�[��0�ɂȂ�����
		m_fStateTime = 0;
	}
}

//==========================================================================
// �X�|�[���ҋ@
//==========================================================================
void CEnemy::StateSpawnWait()
{
	// �s���\����
	m_bActionable = false;

	// �_���[�W���󂯕t���Ȃ�
	m_bDamageReceived = false;

	// ��ԃJ�E���^�[���Z�b�g
	m_fStateTime = 0.0f;

	// �`�悵�Ȃ�
	SetEnableDisp(false);
}

//==========================================================================
// �o��
//==========================================================================
void CEnemy::StateSpawn()
{
	// �s���\����
	m_bActionable = false;

}

//==========================================================================
// �_���[�W
//==========================================================================
void CEnemy::StateDamage()
{
	if (!m_bActiveSuperArmor)
	{
		// �s���\����
		m_bActionable = false;
	}

	// �_���[�W���󂯕t���Ȃ�
	m_bDamageReceived = false;

#if _DEBUG
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
#endif

	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTime -= CManager::GetInstance()->GetDeltaTime();

	if (m_fStateTime <= 0.0f)
	{
		// ���̍s�����I
		if (!m_bActiveSuperArmor)
		{
			ChangeATKState(m_pATKState);
			m_pATKState->ChangeMotionIdx(this);
		}

		// �ߋ��̏�Ԃɂ���
		m_state = m_Oldstate;
	}

	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	if (!m_bActiveSuperArmor)
	{
		// �_���[�W���[�V�����ݒ�
		pMotion->Set(MOTION_DMG);
	}
}

//==========================================================================
// ���S
//==========================================================================
void CEnemy::StateDead()
{
	// �s���\����
	m_bActionable = false;

	// �_���[�W���󂯕t���Ȃ�
	m_bDamageReceived = false;

	SetMove(0.0f);

	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();
	if (nType == MOTION::MOTION_KNOCKBACK && pMotion->IsFinish() == true)
	{
		// ���̍s�����I
		m_state = STATE::STATE_FADEOUT;
		m_fStateTime = TIME_FADEOUT;
		return;
	}

	/*if (nType != MOTION::MOTION_KNOCKBACK)
	{
		pMotion->Set(MOTION::MOTION_KNOCKBACK);
	}*/
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CEnemy::StateFadeOut()
{
	// �s���\����
	m_bActionable = false;

	// �_���[�W���󂯕t���Ȃ�
	m_bDamageReceived = false;

	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	m_sMotionFrag.bMove = false;	// �ړ�����OFF
	m_sMotionFrag.bATK = false;		// �U������OFF

	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTime -= CManager::GetInstance()->GetDeltaTime();

	// �F�ݒ�
	m_mMatcol.a = m_fStateTime / TIME_FADEOUT;

	if (m_fStateTime <= 0.0f)
	{
		// �G�̏I������
		Kill();
		Uninit();
		return;
	}
}

//==========================================================================
// �ҋ@���
//==========================================================================
void CEnemy::StateWait()
{
	// �s���\����
	m_bActionable = true;

	return;
}

//==========================================================================
// �_�E�����
//==========================================================================
void CEnemy::StateDown()
{
	// �s���\����
	m_bActionable = false;

	if (m_fDownTime <= 0.0f)
	{
		m_state = STATE_NONE;

		// �s���\����
		m_bActionable = true;

		// �_�E�����A
		if (m_pReturnDown != nullptr)
		{
			ChangeATKState(m_pReturnDown);
			m_pATKState->ChangeMotionIdx(this);
		}
		else
		{
			// �s�����I
			DrawingRandomAction();
		}
		return;
	}

	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();
	if (nType != MOTION_DOWN)
	{
		// �_�E�����[�V�����ݒ�
		pMotion->Set(MOTION_DOWN);
	}
}

//==========================================================================
// ���U��
//==========================================================================
void CEnemy::StateStrongAtk()
{
	// �s���\����
	m_bActionable = true;
}

//==========================================================================
// �_�E���ҋ@���
//==========================================================================
void CEnemy::StateDownWait()
{
	// �s���\����
	m_bActionable = false;
	m_sMotionFrag.bMove = false;	// �ړ�����OFF
	m_sMotionFrag.bATK = false;		// �U������OFF
}

//==========================================================================
// ��l�̕�
//==========================================================================
void CEnemy::LimitArea()
{
	// ���g�̒l���擾
	MyLib::Vector3 pos = GetPosition();

	// ��l�̕ǎ擾
	CListManager<CLimitArea> limitareaList = CLimitArea::GetListObj();
	CLimitArea* pLimitArea = nullptr;

	while (limitareaList.ListLoop(&pLimitArea))
	{
		CLimitArea::sLimitEreaInfo info = pLimitArea->GetLimitEreaInfo();

		// ��l�̕ǂ�K�p
		if (pos.x + GetRadius() >= info.fMaxX) { pos.x = info.fMaxX - GetRadius(); }
		if (pos.x - GetRadius() <= info.fMinX) { pos.x = info.fMinX + GetRadius(); }
		if (pos.z + GetRadius() >= info.fMaxZ) { pos.z = info.fMaxZ - GetRadius(); }
		if (pos.z - GetRadius() <= info.fMinZ) { pos.z = info.fMinZ + GetRadius(); }
	}

	// �l��K�p
	SetPosition(pos);


	float fLength = sqrtf(pos.x * pos.x + pos.z * pos.z) + GetRadius();

	if (fLength > mylib_const::RADIUS_STAGE)
	{// �␳
		D3DXVec3Normalize(&pos, &pos);

		pos *= mylib_const::RADIUS_STAGE - GetRadius();

		SetPosition(pos);
	}
}

//==========================================================================
// ���[�V�����̐ݒ�
//==========================================================================
void CEnemy::SetMotion(int motionIdx)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	pMotion->Set(motionIdx);
}

//==========================================================================
// �U��������
//==========================================================================
void CEnemy::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	return;
}

//==========================================================================
// �U�����蒆����
//==========================================================================
void CEnemy::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	//return;
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// ����̈ʒu
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *pATKInfo);

	if (pATKInfo->fRangeSize == 0.0f)
	{
		return;
	}

	if (pATKInfo->bEndAtk)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(weponpos, MyLib::Vector3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), pATKInfo->fRangeSize, 10, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

	//============================
	// �v���C���[�Ɣ���
	//============================
	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// ���X�g���[�v
	while (playerList.ListLoop(&pPlayer))
	{
		// �R���C�_�[�̐��J��Ԃ�
		std::vector<SphereCollider> colliders = pPlayer->GetSphereColliders();
		for (const auto& collider : colliders)
		{
			MyLib::HitResult hitresult = UtilFunc::Collision::SphereRange(weponpos, collider.center, pATKInfo->fRangeSize, collider.radius);
			if (hitresult.ishit)
			{// ���̔���

				MyLib::HitResult_Character result = pPlayer->Hit(pATKInfo->nDamage, this, pATKInfo->AtkType);

				if (result.ishit || result.isdeath)
				{
					pATKInfo->bEndAtk = true;
				}

				if (result.isdamage)
				{// �������Ă���

					// �v���C���[�̌���
					MyLib::Vector3 PlayerRot = pPlayer->GetRotation();

					// �v���C���[�̈ʒu
					MyLib::Vector3 PlayerPos = pPlayer->GetPosition();

					// �^�[�Q�b�g�ƓG�Ƃ̌���
					float fRot = atan2f((pos.x - PlayerPos.x), (pos.z - PlayerPos.z));

					// �����𐳖ʂɂ���
					fRot = D3DX_PI + fRot;
					UtilFunc::Transformation::RotNormalize(fRot);

					// �����ݒ�
					pPlayer->SetRotation(MyLib::Vector3(PlayerRot.x, fRot, PlayerRot.z));
					pPlayer->SetRotDest(fRot);
				}
				break;
			}
		}
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CEnemy::Draw()
{
	if (m_state == STATE_FADEOUT)
	{
		CObjectChara::Draw(m_mMatcol.a);
	}
	else if (m_mMatcol != D3DXCOLOR(1.0f, 1.0f, 1.0f, m_mMatcol.a))
	{
		// �I�u�W�F�N�g�L�����̕`��
		CObjectChara::Draw(m_mMatcol);
	}
	else
	{
		// �I�u�W�F�N�g�L�����̕`��
		CObjectChara::Draw();
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CEnemy::SetState(STATE state)
{
	m_state = state;
}

//==========================================================================
// �G�̏��擾
//==========================================================================
CEnemy *CEnemy::GetEnemy()
{
	// �������g�̃|�C���^���擾
	return this;
}


//==========================================================================
// ��Ԃ̃f�X�g���N�^
//==========================================================================
CEnemyState::~CEnemyState()
{
	if (m_pFlinchAction != nullptr)
	{
		delete m_pFlinchAction;
		m_pFlinchAction = nullptr;
	}
}

//==========================================================================
// �s���O�s��
//==========================================================================
void CEnemyBeforeAction::Action(CEnemy* boss)
{
	// ���[�V�����擾
	CMotion* pMotion = boss->GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();
	if (nType == m_nIdxMotion && pMotion->IsFinish() == true)
	{// �X�e�b�v�I��

		// ���̍s���ݒ�
		boss->ChangeNextAction();

		// �ҋ@���[�V�����ݒ�
		pMotion->Set(CEnemy::MOTION_DEF);
		return;
	}

	if (nType != m_nIdxMotion)
	{
		// ���[�V�����ݒ�
		pMotion->Set(m_nIdxMotion);
	}
}


//==========================================================================
// ���ݍs���ݒ�
//==========================================================================
void CEnemyState::SetFlinchAction(CEnemyFlinch* pFlinch)
{
	m_bFinchAction = true;
	m_pFlinchAction = pFlinch;
}

//==========================================================================
// ���݂֐؂�ւ�
//==========================================================================
void CEnemyState::ChangeFlinchAction(CEnemy* boss)
{
	if (m_pFlinchAction == nullptr)
	{
		return;
	}
	boss->ChangeATKState(m_pFlinchAction);
	boss->GetATKState()->ChangeMotionIdx(boss);
}

//==========================================================================
// ���ݍs��
//==========================================================================
void CEnemyFlinch::Action(CEnemy* boss)
{
	// ���[�V�����擾
	CMotion* pMotion = boss->GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();
	if (nType == m_nIdxMotion && pMotion->IsFinish() == true)
	{// �X�e�b�v�I��

		// ���̍s�����I
		boss->DrawingRandomAction();

		// �ҋ@���[�V�����ݒ�
		pMotion->Set(CEnemy::MOTION_DEF);
		return;
	}

	if (nType != m_nIdxMotion)
	{
		// ���̍s�����I
		boss->DrawingRandomAction();
	}
}

//==========================================================================
// �_�E�����A
//==========================================================================
void CEnemyReturnDown::Action(CEnemy* boss)
{
	// ���[�V�����擾
	CMotion* pMotion = boss->GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	int nType = pMotion->GetType();
	if (nType == m_nIdxMotion && pMotion->IsFinish() == true)
	{// �X�e�b�v�I��

		// ���̍s�����I
		boss->DrawingRandomAction();

		// �ҋ@���[�V�����ݒ�
		pMotion->Set(CEnemy::MOTION_DEF);
		return;
	}

	if (nType != m_nIdxMotion)
	{
		// ���[�V�����ݒ�
		pMotion->Set(m_nIdxMotion);
	}
}

//==========================================================================
// �U������
//==========================================================================
void CEnemyAttack::Attack(CEnemy* boss)
{
	// ���薈�̌������킹
	if ((m_bSetAngleBeforeAttack && boss->GetMotion()->IsBeforeInAttack()) ||
		(m_bSetAngleNotAttacking && !boss->GetMotion()->IsAttacking()) &&
		!boss->GetMotion()->IsAllAfterAttack())
	{
		// �^�[�Q�b�g�̕�������
		boss->RotationTarget();
	}

	// ���[�V�����擾
	CMotion* pMotion = boss->GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	int nType = pMotion->GetType();

	// �A���U������ꍇ
	if (m_bChainAttack &&
		nType == m_nIdxMotion &&
		pMotion->IsGetCombiable())
	{
		// �A���U���֑J��
		ChangeChainAttack(boss);
		return;
	}

	if (nType == m_nIdxMotion && pMotion->IsFinish() == true)
	{// �U�����I����Ă���

		// ���̍s�����I
		boss->DrawingRandomAction();

		// �ҋ@���[�V�����ݒ�
		pMotion->Set(CEnemy::MOTION_DEF);

		return;
	}

	if (nType != m_nIdxMotion)
	{
		// ���[�V�����ݒ�
		pMotion->Set(m_nIdxMotion);
	}
}


//==========================================================================
// �ߐڍU���̍s��
//==========================================================================
void CEnemyProximity::Action(CEnemy* boss)
{
	bool bAct = true;	// �s���ł��邩�̃t���O

	// ���E������
	if (!boss->IsCatchUp() ||
		!boss->IsInSight())
	{
		// �^�[�Q�b�g�̕�������
		boss->RotationTarget();
		bAct = false;
	}

	// �͈͊O���Ǐ]
	if (!boss->IsCatchUp())
	{
		// �ǂ��|��
		boss->ActChase(1.0f, m_fAttackLength);
		bAct = false;
	}

	if (!bAct)
	{
		return;
	}

	// �U���O�s��
	if (!m_bBeforeAttackAction)
	{
		BeforeAttack(boss);
	}

	// �U������
	if (m_bBeforeAttackAction)
	{
		Attack(boss);
	}
}

//==========================================================================
// �������U���̍s��
//==========================================================================
void CEnemyRemote::Action(CEnemy* boss)
{
	bool bAct = true;	// �s���ł��邩�̃t���O

	// ���E������
	// �^�[�Q�b�g�̕�������
	boss->RotationTarget(45.0f);
	if (!boss->IsInSight())
	{
		// �^�[�Q�b�g�̕�������
		boss->RotationTarget(45.0f);
		bAct = false;
	}

	if (!bAct)
	{
		return;
	}
	// �U������
	Attack(boss);
}