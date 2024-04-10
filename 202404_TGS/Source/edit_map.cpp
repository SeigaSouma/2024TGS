//=============================================================================
// 
//  戦闘開始処理 [edit_map.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "edit_map.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* LOADTEXT = "data\\TEXT\\map\\info.txt";				// テクスチャのファイル
}
CListManager<CObjectX> CEdit_Map::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CEdit_Map::CEdit_Map()
{
	// 値のクリア
	m_nModelIdx.clear();	// モデルインデックス
	m_pObjX.clear();		// オブジェクトXのポインタ
}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit_Map::~CEdit_Map()
{

}
//
////==========================================================================
//// 生成処理
////==========================================================================
//CEdit_Map* CEdit_Map::Create()
//{
//	// メモリの確保
//	CEdit_Map* pMarker = DEBUG_NEW CEdit_Map;
//
//	if (pMarker != nullptr)
//	{// メモリの確保が出来ていたら
//
//		// 初期化処理
//		pMarker->Init();
//	}
//
//	return pMarker;
//}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit_Map::Init()
{

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEdit_Map::Uninit()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CEdit_Map::Update()
{
	
}

//==========================================================================
// セーブ
//==========================================================================
void CEdit_Map::Save()
{
	// ファイルを開く
	FILE* pFile = fopen("data\\TEXT\\map\\save_info.txt", "w");
	if (pFile == nullptr)
	{// ファイルが開けなかった場合
		return;
	}

	// ファイルに書き出す
	fprintf(pFile,
		"\n"
		"#------------------------------------------------------------------------------\n"
		"# モデル数\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", (int)m_nModelIdx.size());

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# モデルファイル名\n"
		"#------------------------------------------------------------------------------\n");

	// モデルファイル名分
	for (int i = 0; i < (int)m_nModelIdx.size(); i++)
	{
		fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", m_ModelFile[i].c_str(), i);
	}

	//=============================
	// モデル
	//=============================
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# モデルの配置\n"
		"#==============================================================================\n");

	// リストループ
	CObjectX* pObj = nullptr;
	while (m_List.ListLoop(&pObj))
	{
		MyLib::Vector3 pos = pObj->GetPosition();	// 位置
		MyLib::Vector3 rot = pObj->GetRotation();	// 向き
		int nShadow = 0;							// 影使うかどうか
		int nType = 0;								// 種類

		if (pObj->GetUseShadow() == true)
		{// 使っている場合
			nShadow = 1;
		}
		
		std::vector<int>::iterator nIdx = std::find(m_nModelIdx.begin(), m_nModelIdx.end(), pObj->GetIdxXFile());

		// 出力
		fprintf(pFile,
			"MODELSET\n"
			"\tTYPE = %d\n"
			"\tPOS = %.2f %.2f %.2f\n"
			"\tROT = %.2f %.2f %.2f\n"
			"\tSHADOW = %d\n"
			"END_MODELSET\n\n",
			(*nIdx), pos.x, pos.y, pos.z,
			rot.x, rot.y, rot.z, nShadow);
	}

	fprintf(pFile, "\nEND_SCRIPT		# この行は絶対消さないこと！");

	//ファイルを閉じる
	fclose(pFile);
}

//==========================================================================
// ロード
//==========================================================================
void CEdit_Map::Load()
{
	std::string FileName;	// ファイル名
	char aComment[MAX_COMMENT] = {};	//コメント用
	int nFileNum = 0;					// ファイルの数
	int nCntTexture = 0;				// テクスチャ読み込みカウント

	// Xファイル読み込み
	ReadXFile();

	// マップファイルを開く
	FILE* pFile = fopen(LOADTEXT, "r");
	if (pFile == nullptr)
	{//ファイルが開けた場合
		return;
	}

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// モデルの設定
		if (strcmp(&aComment[0], "MODELSET") == 0)
		{
			int nType = 0, nShadow = 0;
			MyLib::Vector3 pos, rot;

			while (strcmp(&aComment[0], "END_MODELSET"))
			{// END_MODELSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	// 確認する

				if (strcmp(&aComment[0], "TYPE") == 0)
				{// TYPEが来たら種類読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &nType);		// モデル種類の列挙
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POSが来たら位置読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &pos.x);	// X座標
					fscanf(pFile, "%f", &pos.y);	// Y座標
					fscanf(pFile, "%f", &pos.z);	// Z座標
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROTが来たら向き読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &rot.x);	// Xの向き
					fscanf(pFile, "%f", &rot.y);	// Yの向き
					fscanf(pFile, "%f", &rot.z);	// Zの向き
				}

				if (strcmp(&aComment[0], "SHADOW") == 0)
				{// SHADOWが来たら影使用

					fscanf(pFile, "%s", &aComment[0]);		// =の分
					fscanf(pFile, "%d", &nShadow);			// 影を使うかどうか
				}

			}// END_MODELSETのかっこ

			// 追加
			bool bShadow = (nShadow == 1);
			Regist(nType, pos, rot, bShadow);
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);
}

//==========================================================================
// モデル読み込み処理
//==========================================================================
HRESULT CEdit_Map::ReadXFile()
{
	char aComment[MAX_COMMENT] = {};	// コメント用
	int nFileNum = 0;					// ファイルの数

	// ファイルを開く
	FILE* pFile = fopen(LOADTEXT, "r");

	if (pFile == nullptr)
	{// ファイルが開けた場合
		return E_FAIL;
	}

	int modelNum = 0;
	m_nModelIdx.clear();
	m_ModelFile.clear();

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// モデル数の設定
		if (strcmp(&aComment[0], "NUM_MODEL") == 0)
		{// NUM_MODELがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &nFileNum);		// モデル数
		}

		while (modelNum != nFileNum)
		{// モデルの数分読み込むまで繰り返し

			// 文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			// モデル名の設定
			if (strcmp(&aComment[0], "MODEL_FILENAME") == 0)
			{// NUM_MODELがきたら

				fscanf(pFile, "%s", &aComment[0]);	// =の分
				fscanf(pFile, "%s", &aComment[0]);	// ファイル名

				// 追加
				m_ModelFile.push_back(&aComment[0]);
				m_nModelIdx.push_back(0);

				// インデックス取得
				m_nModelIdx.back() = CXLoad::GetInstance()->XLoad(aComment);

				modelNum++;	// モデル数加算
			}
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける

			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// 削除
//==========================================================================
void CEdit_Map::Delete(CObjectX* obj)
{
	// 終了する
	std::vector<CObjectX*>::iterator itr = std::find(m_pObjX.begin(), m_pObjX.end(), obj);

	// 要素が見つかった場合のみ削除
	if (itr != m_pObjX.end())
	{
		// リストから削除
		m_List.Delete((*itr));

		(*itr)->Kill();
		m_pObjX.erase(itr);
	}
}

//==========================================================================
// 割り当て
//==========================================================================
void CEdit_Map::Regist(int idx, MyLib::Vector3 pos, MyLib::Vector3 rot, bool bShadow)
{

	m_pObjX.emplace_back();

	// タイプの物を生成
	m_pObjX.back() = CObjectX::Create(m_nModelIdx[idx], pos, rot, bShadow);
	m_pObjX.back()->SetType(CObject::TYPE_XFILE);

	// リストに追加
	m_List.Regist(m_pObjX.back());
}