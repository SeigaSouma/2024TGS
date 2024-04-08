//=============================================================================
// 
//  花粉量ゲージ処理 [pollen_gauge.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "pollen_gauge.h"
#include "manager.h"
#include "camera.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	float DEFAULT_WIDTH = 500.0f;
	float DEFAULT_HEIGHT = 60.0f;
	const char* USE_TEXTURE[] =
	{
		"data\\TEXTURE\\hpgauge\\black.png",
		"data\\TEXTURE\\hpgauge\\pollengauhge.png",
	};
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================


//==========================================================================
// コンストラクタ
//==========================================================================
CPollen_Gauge::CPollen_Gauge(int nPriority) : CObject(nPriority)
{
	m_nMaxValue = 0;
}

//==========================================================================
// デストラクタ
//==========================================================================
CPollen_Gauge::~CPollen_Gauge()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPollen_Gauge* CPollen_Gauge::Create(MyLib::Vector3 pos, int nMaxPollen)
{
	// 生成用のオブジェクト
	CPollen_Gauge* pPollenGauge = nullptr;

	// メモリの確保
	pPollenGauge = DEBUG_NEW CPollen_Gauge;

	if (pPollenGauge != nullptr)
	{// メモリの確保が出来ていたら

		// 値設定
		pPollenGauge->SetPosition(pos);
		pPollenGauge->SetOriginPosition(pos);
		pPollenGauge->m_nMaxValue = nMaxPollen;

		// 初期化処理
		pPollenGauge->Init();
	}

	return pPollenGauge;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPollen_Gauge::Init()
{
	for (int cnt = 0; cnt < VTXTYPE_MAX; cnt++)
	{
		m_apGauge[cnt] = CObject2D_Gauge::Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, m_nMaxValue, USE_TEXTURE[cnt], GetPriority());
		
		if (m_apGauge[cnt] == nullptr)
		{
			return E_FAIL;
		}

		m_apGauge[cnt]->SetPosition(GetPosition());
		m_apGauge[cnt]->SetOriginPosition(GetPosition());

		// 種類の設定
		m_apGauge[cnt]->SetType(CObject::TYPE::TYPE_NONE);
		m_apGauge[cnt]->SetSize(D3DXVECTOR2(DEFAULT_WIDTH, DEFAULT_HEIGHT));
		m_apGauge[cnt]->SetMoveFactor(0.25f);	// 移動の係数設定
	}
	
	SetType(CObject::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPollen_Gauge::Uninit()
{
	for (int cnt = 0; cnt < VTXTYPE_MAX; cnt++)
	{
		if (m_apGauge[cnt] == nullptr)
		{
			continue;
		}
		m_apGauge[cnt]->Uninit();
		m_apGauge[cnt] = nullptr;
	}
	// 情報削除
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CPollen_Gauge::Update()
{
	for (int i = 0; i < VTXTYPE::VTXTYPE_MAX; i++)
	{
		// サイズ取得
		D3DXVECTOR2 size = m_apGauge[i]->GetSize();

		D3DXVECTOR2* pTex = m_apGauge[i]->GetTex();

		float ratio = size.x / 80.0f;
		m_apGauge[i]->Update();

		pTex[1] = D3DXVECTOR2(ratio, 0.0f);
		pTex[3] = D3DXVECTOR2(ratio, 1.0f);
		SetVtx(i);

		m_apGauge[i]->SetEnableDisp(IsDisp());
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CPollen_Gauge::Draw()
{
	for (int i = 0; i < VTXTYPE::VTXTYPE_MAX; i++)
	{
		m_apGauge[i]->Draw();
	}
}

//==========================================================================
// 値の補正
//==========================================================================
void CPollen_Gauge::SetValue(int nValue)
{
	UtilFunc::Transformation::ValueNormalize(nValue, m_nMaxValue, 0);

	m_apGauge[VTXTYPE_GAUGE]->SetValue(static_cast<int>(nValue));
}

//==========================================================================
// 頂点設定
//==========================================================================
void CPollen_Gauge::SetVtx(int nIdx)
{
	// 頂点設定
	m_apGauge[nIdx]->SetVtx();

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	D3DXVECTOR2* pTex = m_apGauge[nIdx]->GetTex();

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_apGauge[nIdx]->GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// サイズ取得
	D3DXVECTOR2 size = m_apGauge[nIdx]->GetSize();

	// 頂点座標の設定
	pVtx[0].tex = pTex[0];
	pVtx[1].tex = pTex[1];
	pVtx[2].tex = pTex[2];
	pVtx[3].tex = pTex[3];

	// 頂点バッファをアンロックロック
	m_apGauge[nIdx]->GetVtxBuff()->Unlock();
}