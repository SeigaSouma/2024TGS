//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playercontrol.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "enemy.h"

namespace
{
	const float STAMINA_AVOID = 30.0f;	// 回避のスタミナ消費量
	const float LENGTH_AUTOFACE = 200.0f;	// 自動で向く長さ
}

//==========================================================================
// 攻撃可能フラグ取得
//==========================================================================
bool CPlayerControlAttack::IsAttack(CPlayer* player)
{
	// インプット情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// 現在の種類取得
	CMotion* pMotion = player->GetMotion();

	if ((pMotion->IsGetCombiable() || pMotion->IsGetCancelable()) &&
		!player->IsJump())
	{
		return true;
	}
	return false;
}

//==========================================================================
// 通常攻撃
//==========================================================================
void CPlayerControlAttack::Attack(CPlayer* player)
{
	
}

//==========================================================================
// 通常攻撃
//==========================================================================
void CPlayerControlAttack_Level1::Attack(CPlayer* player)
{

}



//==========================================================================
// 防御
//==========================================================================
// 基底
void CPlayerControlDefence::Defence(CPlayer* player)
{
	
}

// レベル1
void CPlayerControlDefence_Level1::Defence(CPlayer* player)
{
	
}

//==========================================================================
// 回避
//==========================================================================
void CPlayerControlAvoid::Avoid(CPlayer* player)
{

}




//==========================================================================
// ガード
//==========================================================================
// 受けた時の処理
void CPlayerGuard::HitProcess(CPlayer* player, MyLib::Vector3 enemypos)
{
	
	// 位置取得
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 rot = player->GetRotation();

	// 目標の角度設定
	float fRotDest = atan2f((pos.x - enemypos.x), (pos.z - enemypos.z));
	player->SetRotation(MyLib::Vector3(rot.x, fRotDest, rot.z));
	player->SetRotDest(fRotDest);

	player->SetMove(MyLib::Vector3(
		sinf(fRotDest) * 40.0f,
		0.0f,
		cosf(fRotDest) * 40.0f));
}