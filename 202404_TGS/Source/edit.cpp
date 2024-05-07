//=============================================================================
// 
//  エディット処理 [edit.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectX.h"
#include "XLoad.h"
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "map.h"
#include "game.h"
#include "elevation.h"

//==========================================================================
// マクロ定義
//==========================================================================
#define MOVE		(10.0f)
#define MOVE_SLOW	(1.0f)

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CObjectX *CEdit::m_pObjX = nullptr;
int CEdit::m_nNumAll = 0;	// 総数
int CEdit::m_nType = 0;		// タイプ
bool CEdit::m_bShadow = false;	// 影を使うかどうか

//==========================================================================
// コンストラクタ
//==========================================================================
CEdit::CEdit(int nPriority) : CObject(nPriority)
{
	m_posOld = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// 前回の位置
	m_bShadow = true;	// 影を使うかどうか

	// 総数加算
	m_nNumAll++;
}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit::~CEdit()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CEdit *CEdit::Create()
{
	// 生成用のオブジェクト
	CEdit *pObjectX = nullptr;

	if (pObjectX == nullptr)
	{// nullptrだったら

		// メモリの確保
		pObjectX = DEBUG_NEW CEdit;

		//if (pObjectX->GetID() < 0)
		//{// メモリ確保に失敗していたら

		//	delete pObjectX;
		//	return nullptr;
		//}

		if (pObjectX != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			HRESULT hr = pObjectX->Init();

			if (FAILED(hr))
			{// 失敗していたら
				return nullptr;
			}

			// 種類設定
			pObjectX->SetType(TYPE_EDIT);
		}

		return pObjectX;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 生成処理
	m_pObjX = m_pObjX->Create(CXLoad::GetInstance()->GetMyObject(m_nType)->filename.c_str());
	m_pObjX->SetType(TYPE_EDIT);

	if (m_pObjX == nullptr)
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit::Init(const char *pFileName)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	
	// 生成処理
	m_pObjX = m_pObjX->Create(pFileName);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEdit::Uninit()
{
	// オブジェクトの破棄
	CObject::Release();
}

//==========================================================================
// 破棄の処理
//==========================================================================
void CEdit::Release()
{
	// 終了処理
	m_pObjX->CObjectX::Uninit();
	CEdit::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CEdit::Update()
{
	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// 位置取得
	MyLib::Vector3 pos = m_pObjX->GetPosition();

	// 向き取得
	MyLib::Vector3 rot = m_pObjX->GetRotation();

	// 操作
	Control(m_pObjX);

	// 種類変更
	ChangeType();

	// 掴み移動
	GrabModel();

	if (pInputKeyboard->GetTrigger(DIK_RETURN))
	{// ENTERで配置

		// タイプの物を生成
		MyMap::Regist(m_nType, pos, rot, m_bShadow);
	}

	if (pInputKeyboard->GetTrigger(DIK_F9))
	{// セーブ

		// テキストにセーブ
		MyMap::SaveText();
	}

	// デバッグ情報
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[ マップエディタ ]------------------\n"
		"<モデル配置>   [ENTER]\n"
		"<ファイル保存> [F9] 【data/TEXT/edit_info.txt】\n"
		"<移動>         高速：[↑,↓,←,→]　低速：[W,A,S,D]\n"
		"<回転>         [LSHIFT, RSHIFT]\n"
		"<上昇,下降>    [I,K]\n"
		"<種類変更>     [1, 2][%d]\n"
		"<影の使用状況> [3][%d]\n"
		"<掴み移動>     [SPACE]\n"
		"<削除>         [DELETE]\n"
		"<位置>         [X：%f Y：%f Z：%f]\n"
		"<向き>         [X：%f Y：%f Z：%f]\n"
		"\n", m_nType, (int)m_bShadow, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z);
}

//==========================================================================
// 操作処理
//==========================================================================
void CEdit::Control(CObjectX *pObjX)
{
	// カメラの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// カメラの向き取得
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// 位置取得
	MyLib::Vector3 pos = pObjX->GetPosition();

	// 向き取得
	MyLib::Vector3 rot = pObjX->GetRotation();

	if (pInputKeyboard->GetPress(DIK_LEFT))
	{// ←キーが押された,左移動

		if (pInputKeyboard->GetPress(DIK_UP))
		{// A+W,左上移動

			pos.x += sinf(-D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE;
			pos.z += cosf(-D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE;
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN))
		{// A+S,左下移動

			pos.x += sinf(-D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE;
			pos.z += cosf(-D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE;
		}
		else
		{// A,左移動

			pos.x += sinf(-D3DX_PI * MOVE_LR + Camerarot.y) * MOVE;
			pos.z += cosf(-D3DX_PI * MOVE_LR + Camerarot.y) * MOVE;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_RIGHT))
	{// Dキーが押された,右移動

		if (pInputKeyboard->GetPress(DIK_UP))
		{// D+W,右上移動

			pos.x += sinf(D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE;
			pos.z += cosf(D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE;
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN))
		{// D+S,右下移動

			pos.x += sinf(D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE;
			pos.z += cosf(D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE;
		}
		else
		{// D,右移動

			pos.x += sinf(D3DX_PI * MOVE_LR + Camerarot.y) * MOVE;
			pos.z += cosf(D3DX_PI * MOVE_LR + Camerarot.y) * MOVE;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_UP))
	{// Wが押された、奥移動

		pos.x += sinf(Camerarot.y) * MOVE;
		pos.z += cosf(Camerarot.y) * MOVE;
	}
	else if (pInputKeyboard->GetPress(DIK_DOWN))
	{// Sが押された、手前移動

		pos.x += sinf(D3DX_PI + Camerarot.y) * MOVE;
		pos.z += cosf(D3DX_PI + Camerarot.y) * MOVE;
	}

	if (pInputKeyboard->GetPress(DIK_A))
	{// ←キーが押された,左移動

		if (pInputKeyboard->GetPress(DIK_W))
		{// A+W,左上移動

			pos.x += sinf(-D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(-D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE_SLOW;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{// A+S,左下移動

			pos.x += sinf(-D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(-D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE_SLOW;
		}
		else
		{// A,左移動

			pos.x += sinf(-D3DX_PI * MOVE_LR + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(-D3DX_PI * MOVE_LR + Camerarot.y) * MOVE_SLOW;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D))
	{// Dキーが押された,右移動

		if (pInputKeyboard->GetPress(DIK_W))
		{// D+W,右上移動

			pos.x += sinf(D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(D3DX_PI * MOVE_LRDW + Camerarot.y) * MOVE_SLOW;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{// D+S,右下移動

			pos.x += sinf(D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(D3DX_PI * MOVE_LRUP + Camerarot.y) * MOVE_SLOW;
		}
		else
		{// D,右移動

			pos.x += sinf(D3DX_PI * MOVE_LR + Camerarot.y) * MOVE_SLOW;
			pos.z += cosf(D3DX_PI * MOVE_LR + Camerarot.y) * MOVE_SLOW;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W))
	{// Wが押された、奥移動

		pos.x += sinf(Camerarot.y) * MOVE_SLOW;
		pos.z += cosf(Camerarot.y) * MOVE_SLOW;
	}
	else if (pInputKeyboard->GetPress(DIK_S))
	{// Sが押された、手前移動

		pos.x += sinf(D3DX_PI + Camerarot.y) * MOVE_SLOW;
		pos.z += cosf(D3DX_PI + Camerarot.y) * MOVE_SLOW;
	}

	if (pInputKeyboard->GetTrigger(DIK_LSHIFT))
	{//回転

		//rot.y += D3DX_PI * 0.05f;
		rot.y += D3DXToRadian(5);
	}
	if (pInputKeyboard->GetTrigger(DIK_RSHIFT))
	{// 回転

		//rot.y -= D3DX_PI * 0.05f;
		rot.y -= D3DXToRadian(5);
	}

	if (pInputKeyboard->GetPress(DIK_I))
	{// Iキーが押された,上昇

		pos.y += MOVE;
	}
	else if (pInputKeyboard->GetPress(DIK_K))
	{// Kキーが押された,下降

		pos.y -= MOVE;
	}

	// 角度正規化
	UtilFunc::Transformation::RotNormalize(rot.y);


	//// 着地したかどうか
	//bool bLand = false;

	//// 高さ取得
	//float fHeight = CGame::GetInstance()->GetElevation()->GetHeight(pos, bLand);

	//if (bLand)
	//{
	//	// 高さ代入
	//	pos.y = fHeight;
	//}
	//else
	//{
	//	pos.y = -100.0f;
	//}

	// 向き設定
	pObjX->SetRotation(rot);

	// 位置設定
	pObjX->SetPosition(pos);
}

//==========================================================================
// 種類変更
//==========================================================================
void CEdit::ChangeType()
{
	int nNumAll = MyMap::GetNumModelAll();

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// 配置するオブジェクト変更
	if (pInputKeyboard->GetTrigger(DIK_1))
	{// 1が押された

		// 色の種類更新
		m_nType = (m_nType + (nNumAll - 1)) % nNumAll;

		int nIdx = CXLoad::GetInstance()->XLoad(MyMap::GetModelFileName(m_nType));

		// オブジェクト割り当て
		m_pObjX->BindXData(nIdx);
	}
	else if (pInputKeyboard->GetTrigger(DIK_2))
	{// 2が押された

		// 色の種類更新
		m_nType = (m_nType + 1) % nNumAll;

		// オブジェクト割り当て
		int nIdx = CXLoad::GetInstance()->XLoad(MyMap::GetModelFileName(m_nType));
		m_pObjX->BindXData(nIdx);
	}

	if (pInputKeyboard->GetTrigger(DIK_3))
	{// 3が押された

		// 影の使用状況切り替え
		m_bShadow = m_bShadow ? false : true;
	}

}

//==========================================================================
// モデル掴む
//==========================================================================
void CEdit::GrabModel()
{

}

//==========================================================================
// モデル削除
//==========================================================================
void CEdit::DeleteModel()
{

}

//==========================================================================
// 描画処理
//==========================================================================
void CEdit::Draw()
{

}


//==========================================================================
// 外部テキスト読み込み処理
//==========================================================================
HRESULT CEdit::ReadText()
{
	return S_OK;;
}

//==========================================================================
// 総数取得
//==========================================================================
int CEdit::GetNumAll()
{
	return m_nNumAll;
}
