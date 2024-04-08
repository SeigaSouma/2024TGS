//=============================================================================
// 
//  花粉量ゲージヘッダー [pollen_gauge.h]
//  Author : 石原颯馬
// 
//=============================================================================

#ifndef _POLLEN_GAUGE_H_
#define _POLLEN_GAUGE_H_	// 二重インクルード防止

#include "object2D_gauge.h"

//==========================================================================
// クラス定義
//==========================================================================
// 体力ゲージクラス定義
class CPollen_Gauge : public CObject
{
public:

	CPollen_Gauge(int nPriority = 11);
	~CPollen_Gauge();

	static CPollen_Gauge *Create(MyLib::Vector3 pos, int nMaxPollen);

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update() override;
	void Draw();

	void SetValue(int nValue);

private:
	// 頂点列挙
	enum VTXTYPE
	{
		VTXTYPE_BG = 0,	// 背景
		VTXTYPE_GAUGE,	// ゲージ部分
		VTXTYPE_MAX
	};

	//メンバ関数
	void SetVtx(int nIdx);

	// メンバ変数
	CObject2D_Gauge* m_apGauge[VTXTYPE_MAX];
	int m_nMaxValue;
};


#endif