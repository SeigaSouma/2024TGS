//=============================================================================
// 
//  マップエディタ―ヘッダー [edit_map.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EDIT_MAP_H_
#define _EDIT_MAP_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップエディタ―クラス
class CEdit_Map
{
public:

	//=============================
	// 構造体定義
	//=============================
	struct SDragDropData
	{
		int nType;		// 種類
		CObjectX* objX;	// オブジェクトX

		SDragDropData() : nType(0), objX(nullptr) {}
	};

	CEdit_Map();
	~CEdit_Map();
	
	HRESULT Init();
	void Uninit();
	void Update();


	void Save();	// セーブ
	void Load();	// ロード
	void Delete(CObjectX* obj);	// 削除
	void Regist(int idx, MyLib::Vector3 pos, MyLib::Vector3 rot, bool bShadow);	// 割り当て

	static CEdit_Map* Create();	// 生成処理
	static CListManager<CObjectX> GetListObj() { return m_List; }	// リスト取得

private:

	//=============================
	// 構造体定義
	//=============================
	struct SMapModelInfo
	{
		int nIdx;
		std::string fileName;
	};

	//=============================
	// メンバ関数
	//=============================
	HRESULT ReadXFile();	// ファイル読み込み
	void ChangeHandle();	// ハンドル切り替え

	//=============================
	// メンバ変数
	//=============================
	std::vector<int> m_nModelIdx;				// モデルインデックス
	std::vector<std::string> m_ModelFile;		// モデルインデックス
	std::vector<LPDIRECT3DTEXTURE9> m_pTexture;	// テクスチャのポインタ
	std::vector<CObjectX*> m_pObjX;			// オブジェクトXのポインタ

	LPDIRECT3DTEXTURE9 m_HandleTex[CHandle::HandleType::TYPE_MAX];	// テクスチャのポインタ


	SDragDropData m_DragData;	// ドラッグ中データ
	bool m_bGrab;	// 掴み判定
	bool m_bReGrab;	// 再掴み判定

	CObjectX* m_pCopyObj;				// コピーオブジェクト
	CObjectX* m_pGrabObj;				// 掴みオブジェクト
	CHandle* m_pHandle;					// 移動ハンドル
	CHandle::HandleType m_HandleType;	// ハンドルの種類
	CHandle::HandleAngle m_moveAngle;	// 移動の向き

	static CListManager<CObjectX> m_List;	// リスト

};

#endif