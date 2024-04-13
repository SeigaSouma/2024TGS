//=============================================================================
// 
// 移動ハンドル処理 [handle_Move.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _HANDLE_MOVE_H_
#define _HANDLE_MOVE_H_		// 二重インクルード防止のマクロを定義する

#include "objectX.h"
class CObjectLine;

//==========================================================================
// クラス定義
//==========================================================================
// 移動ハンドルクラス
class CHandle_Move : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum HandleAngle
	{
		ANGLE_Z = 0,
		ANGLE_Y,
		ANGLE_X,
		ANGLE_MAX
	};

	CHandle_Move(int nPriority = 4);
	~CHandle_Move();

	//  オーバーライド関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetPosition(const MyLib::Vector3& pos) override;	// 位置設定

	float GetScale() { return m_scale; }
	void Kill();

	static CHandle_Move* Create(const MyLib::Vector3& pos);
	static CListManager<CObjectX> GetList() { return m_List; }
private:

	//=============================
	// メンバ変数
	//=============================
	MyLib::AABB m_AABB;
	CObjectX* m_pArrow[ANGLE_MAX];		// ラインのオブジェクト
	CObjectLine* m_pLine[ANGLE_MAX];	// ラインのオブジェクト
	float m_scale;
	static CListManager<CObjectX> m_List;	// 移動ハンドルリスト
};
#endif