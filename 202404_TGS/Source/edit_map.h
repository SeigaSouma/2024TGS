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

//==========================================================================
// クラス定義
//==========================================================================
// マップエディタ―クラス
class CEdit_Map
{
public:

	CEdit_Map();
	~CEdit_Map();
	
	HRESULT Init();
	void Uninit();
	void Update();


	void Save();	// セーブ
	void Load();	// ロード
	void Delete(CObjectX* obj);	// 削除
	void Regist(int idx, MyLib::Vector3 pos, MyLib::Vector3 rot, bool bShadow);	// 割り当て

	static CListManager<CObjectX> GetListObj() { return m_List; }	// リスト取得

private:

	// 構造体定義
	struct SMapModelInfo
	{
		int nIdx;
		std::string fileName;
	};

	HRESULT ReadXFile();

	//=============================
	// メンバ変数
	//=============================
	std::vector<int> m_nModelIdx;	// モデルインデックス
	std::vector<std::string> m_ModelFile;	// モデルインデックス
	std::vector<CObjectX*> m_pObjX;	// オブジェクトXのポインタ
	static CListManager<CObjectX> m_List;	// リスト

};

#endif