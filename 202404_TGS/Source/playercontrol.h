//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERCONTROL_H_
#define _PLAYERCONTROL_H_	// ��d�C���N���[�h�h�~

#include "player.h"

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �U��
//=============================
class CPlayerControlAttack
{
public:

	CPlayerControlAttack() : m_bAttackReserved(false) {}

	virtual void Attack(CPlayer* player);	// �ʏ�U��
	bool IsReserve() { return m_bAttackReserved; }

protected:

	virtual bool IsAttack(CPlayer* player);	// �U���\�t���O�擾

	virtual void StageByReset(CPlayer* player)	// �i�K���̃��Z�b�g����
	{
		// �i�K�擾
		int combostage = player->GetComboStage();

		// �R���{�̒i�K���Z
		combostage++;
		
		player->SetComboStage(combostage);
	}

	bool m_bAttackReserved;	// �U���̗\�񔻒�
};

class CPlayerControlAttack_Level1 : public CPlayerControlAttack
{
public:

	CPlayerControlAttack_Level1() : m_bChargePossible(false) {}

	virtual void Attack(CPlayer* player) override;	// �ʏ�U��

private:
	bool m_bChargePossible;	// �`���[�W�\�t���O
};


//=============================
// �h��
//=============================
class CPlayerControlDefence
{
public:
	CPlayerControlDefence() {}

	virtual void Defence(CPlayer* player);	// �h��
};

// �h�䃌�x��1
class CPlayerControlDefence_Level1 : public CPlayerControlDefence
{
public:
	CPlayerControlDefence_Level1() {}

	virtual void Defence(CPlayer* player) override;	// �h��
};


//=============================
// ���
//=============================
class CPlayerControlAvoid
{
public:
	CPlayerControlAvoid() {}

	virtual void Avoid(CPlayer* player);	// ���
};


//=============================
// �K�[�h
//=============================
class CPlayerGuard
{
public:
	CPlayerGuard() {}

	virtual void HitProcess(CPlayer* player, MyLib::Vector3 enemypos);	// �󂯂����̏���
};

class CPlayerGuard_Level1 : public CPlayerGuard
{
public:
	CPlayerGuard_Level1() {}

	// �󂯂����̏���
	virtual void HitProcess(CPlayer* player, MyLib::Vector3 enemypos) override
	{
		CPlayerGuard::HitProcess(player, enemypos);

		MyLib::Vector3 rot = player->GetRotation();
		player->SetMove(MyLib::Vector3(
			sinf(rot.y) * 20.0f,
			0.0f,
			cosf(rot.y) * 20.0f));
	}
};

class CPlayerGuard_Level2 : public CPlayerGuard
{
public:
	CPlayerGuard_Level2() {}

	// �󂯂����̏���
	virtual void HitProcess(CPlayer* player, MyLib::Vector3 enemypos) override
	{
		CPlayerGuard::HitProcess(player, enemypos);

		MyLib::Vector3 rot = player->GetRotation();
		player->SetMove(MyLib::Vector3(
			sinf(rot.y) * 5.0f,
			0.0f,
			cosf(rot.y) * 5.0f));
	}
};



#endif