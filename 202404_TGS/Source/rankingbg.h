//=============================================================================
// 
//  ランキング背景ヘッダー [rankingbg.h]
//  Author : 石原颯馬
// 
//=============================================================================

#ifndef _RANKING_BG_H_
#define _RANKING_BG_H_	// 二重インクルード防止

#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// 体力ゲージクラス定義
class CRankingBG : public CObject2D
{
public:

	CRankingBG(int nPriority = 0);
	~CRankingBG();

	static CRankingBG*Create(float fSpeed);

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
	enum MOVE
	{
		MOVE_LEFT = 0,
		MOVE_RIGHT,
		MOVE_MAX
	};

	float m_fSpeed;
	float m_fMoveOffset;
	MOVE m_moveDir;
};


#endif