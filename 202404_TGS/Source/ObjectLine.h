//=============================================================================
// 
//  オブジェクトラインヘッダー [objectLine.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECT3D_ANIM_H_
#define _OBJECT3D_ANIM_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクトラインクラス
class CObjectLine : public CObject
{
public:

	//=============================
	// 構造体定義
	//=============================
	struct SLineInfo
	{
		MyLib::Vector3 left;
		MyLib::Vector3 right;
		D3DXCOLOR col;

		SLineInfo() : left(0.0f), right(0.0f), col(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)) {}
	};

	CObjectLine(int nPriority = mylib_const::PRIORITY_DEF2D);
	virtual ~CObjectLine();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void SetVtx() override;

	void SetWorldMtx(const D3DXMATRIX mtx) { m_mtxWorld = mtx; }	// マトリックス設定
	D3DXMATRIX GetWorldMtx() const { return m_mtxWorld; }			// マトリックス取得

	static CObjectLine *Create(MyLib::Vector3 left, MyLib::Vector3 right, D3DXCOLOR col);

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス
	SLineInfo m_Info;
};



#endif