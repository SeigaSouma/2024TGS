//=============================================================================
// 
//  ランキング背景処理 [rankingbg.cpp]
//  Author : 石原颯馬
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
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\ranking_bg.png";
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================


//==========================================================================
// コンストラクタ
//==========================================================================
CRankingBG::CRankingBG(int nPriority) : CObject2D(nPriority)
{
	//初期化
	m_fMoveOffset = 0.0f;
	m_fSpeed = 0.0f;
	m_moveDir = MOVE_LEFT;
}

//==========================================================================
// デストラクタ
//==========================================================================
CRankingBG::~CRankingBG()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CRankingBG* CRankingBG::Create(float fSpeed)
{
	// 生成用のオブジェクト
	CRankingBG* pBusket = nullptr;

	// メモリの確保
	pBusket = DEBUG_NEW CRankingBG;

	if (pBusket != nullptr)
	{// メモリの確保が出来ていたら

		//初期値
		pBusket->m_fSpeed = fSpeed;

		// 初期化処理
		pBusket->Init();

		// 種類の設定
		pBusket->SetType(TYPE_HPGAUGE);
	}

	return pBusket;
}

//==========================================================================
// 初期化処理
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
// 終了処理
//==========================================================================
void CRankingBG::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CRankingBG::Update()
{
	//移動
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

	//テクスチャ設定
	D3DXVECTOR2 aTex[4];
	aTex[0] = D3DXVECTOR2(fMove, 0.25f);
	aTex[1] = D3DXVECTOR2(0.75f + fMove, 0.25f);
	aTex[2] = D3DXVECTOR2(fMove, 1.0f);
	aTex[3] = D3DXVECTOR2(0.75f + fMove, 1.0f);
	SetTex(&aTex[0]);

	CObject2D::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CRankingBG::Draw()
{
	CObject2D::Draw();
}