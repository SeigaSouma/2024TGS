//=============================================================================
// 
// 移動ハンドル処理 [handle_Move.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "handle_Move.h"
#include "manager.h"
#include "calculation.h"
#include "3D_Effect.h"
#include "camera.h"
#include "objectLine.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace 
{
	const char* MODEL[] =
	{
		"data\\MODEL\\handle\\arrow_blue.x",
		"data\\MODEL\\handle\\arrow_green.x",
		"data\\MODEL\\handle\\arrow_red.x",
	};
	const float LENGTH = 200.0f;

	const D3DXCOLOR COLOR[] =
	{
		{0.0f, 0.0f, 1.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f},
	};
}
CListManager<CObjectX> CHandle_Move::m_List = {};	// 移動ハンドルリスト

//==========================================================================
// コンストラクタ
//==========================================================================
CHandle_Move::CHandle_Move(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	memset(m_pArrow, 0, sizeof(m_pArrow));
}

//==========================================================================
// デストラクタ
//==========================================================================
CHandle_Move::~CHandle_Move()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CHandle_Move* CHandle_Move::Create(const MyLib::Vector3& pos)
{
	// メモリ確保
	CHandle_Move* pBox = DEBUG_NEW CHandle_Move;

	if (pBox != nullptr)
	{
		pBox->SetPosition(pos);
		// 初期化処理
		pBox->Init();
	}

	return pBox;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CHandle_Move::Init()
{

	// 種類の設定
	SetType(TYPE_OBJECT2D);

	MyLib::Vector3 pos[HandleAngle::ANGLE_MAX], rot[HandleAngle::ANGLE_MAX];

	pos[0] = MyLib::Vector3(0.0f, 0.0f, -LENGTH);
	pos[1] = MyLib::Vector3(0.0f, LENGTH, 0.0f);
	pos[2] = MyLib::Vector3(LENGTH, 0.0f, 0.0f);

	rot[0] = MyLib::Vector3(0.0f, 0.0f, 0.0f);
	rot[1] = MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f);
	rot[2] = MyLib::Vector3(0.0f, -D3DX_PI * 0.5f, 0.0f);

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		m_pArrow[i] = CObjectX::Create(MODEL[i], pos[i], rot[i], false);
		m_pArrow[i]->SetOriginPosition(pos[i]);
		m_List.Regist(m_pArrow[i]);
	}

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		m_pLine[i] = CObjectLine::Create(0.0f, pos[i], COLOR[i]);	// ラインのオブジェクト
		m_pLine[i]->SetEnableZSort(true);
	}

	Update();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CHandle_Move::Uninit()
{
	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pArrow[i] == nullptr){
			continue;
		}
		m_List.Delete(m_pArrow[i]);
		m_pArrow[i] = nullptr;
	}

	// 終了処理
	Release();
}

//==========================================================================
// 削除処理
//==========================================================================
void CHandle_Move::Kill()
{

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pArrow[i] == nullptr) {
			continue;
		}
		m_List.Delete(m_pArrow[i]);
		m_pArrow[i]->Uninit();
		m_pArrow[i] = nullptr;
	}

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pLine[i] == nullptr) {
			continue;
		}
		m_pLine[i]->Uninit();
		m_pLine[i] = nullptr;
	}

	// 終了処理
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CHandle_Move::Update()
{
	MyLib::Vector3 pos = GetPosition();
	
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 cameraPosition = pCamera->GetPositionV();
	D3DXMATRIX projection = pCamera->GetMtxProjection();
	D3DXMATRIX view = pCamera->GetMtxView();
	D3DVIEWPORT9 viewport = pCamera->GetViewPort();


	// カメラから矢印までの距離を計算
	MyLib::Vector3 t = (pos - cameraPosition);
	float distanceToArrow = t.Length();

	// 矢印のモデルの拡大率を設定
	m_scale = 1.0f * (distanceToArrow / 2000.0f);

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pArrow[i] == nullptr) {
			continue;
		}

		// カメラに対して常に同じ見え方にする
		{
			MyLib::Vector3 position = m_pArrow[i]->GetPosition();
			MyLib::Vector3 originpos = m_pArrow[i]->GetOriginPosition();

			m_pArrow[i]->SetPosition(pos + originpos * m_scale);
			m_pArrow[i]->SetScale(m_scale);
			m_pLine[i]->SetLeftPosition(pos);
			m_pLine[i]->SetRightPosition(m_pArrow[i]->GetPosition());
		}

		// 色設定
		if (m_pArrow[i]->GetColor() != mylib_const::DEFAULT_COLOR) {
			m_pLine[i]->SetColor({0.0f, 1.0f, 1.0f, 1.0f});
		}
		else {
			m_pLine[i]->SetColor(COLOR[i]);
		}
	}

}

//==========================================================================
// 描画処理
//==========================================================================
void CHandle_Move::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	// 常に描画する

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pArrow[i]->GetColor() != mylib_const::DEFAULT_COLOR){
			m_pArrow[i]->Draw(m_pArrow[i]->GetColor());
		}
		else {
			m_pArrow[i]->Draw();
		}
	}

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==========================================================================
// 位置設定
//==========================================================================
void CHandle_Move::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);

	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 cameraPosition = pCamera->GetPositionV();

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pArrow[i] == nullptr) {
			continue;
		}

		// カメラに対して常に同じ見え方にする
		{
			MyLib::Vector3 position = m_pArrow[i]->GetPosition();
			MyLib::Vector3 originpos = m_pArrow[i]->GetOriginPosition();

			// カメラから矢印までの距離を計算
			MyLib::Vector3 t = (pos - cameraPosition);
			float distanceToArrow = t.Length();

			// 矢印のモデルの拡大率を設定
			float scaleFactor = 1.0f * (distanceToArrow / 2000.0f);

			m_pArrow[i]->SetPosition(pos + originpos * scaleFactor);
			m_pLine[i]->SetLeftPosition(pos);
			m_pLine[i]->SetRightPosition(m_pArrow[i]->GetPosition());
		}
	}
}