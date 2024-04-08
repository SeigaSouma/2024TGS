//=============================================================================
// 
//  �ԕ��ʃQ�[�W���� [pollen_gauge.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "pollen_gauge.h"
#include "manager.h"
#include "camera.h"
#include "calculation.h"

//==========================================================================
// �萔��`
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
// �ÓI�����o�ϐ��錾
//==========================================================================


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPollen_Gauge::CPollen_Gauge(int nPriority) : CObject(nPriority)
{
	m_nMaxValue = 0;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPollen_Gauge::~CPollen_Gauge()
{

}

//==========================================================================
// ��������
//==========================================================================
CPollen_Gauge* CPollen_Gauge::Create(MyLib::Vector3 pos, int nMaxPollen)
{
	// �����p�̃I�u�W�F�N�g
	CPollen_Gauge* pPollenGauge = nullptr;

	// �������̊m��
	pPollenGauge = DEBUG_NEW CPollen_Gauge;

	if (pPollenGauge != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �l�ݒ�
		pPollenGauge->SetPosition(pos);
		pPollenGauge->SetOriginPosition(pos);
		pPollenGauge->m_nMaxValue = nMaxPollen;

		// ����������
		pPollenGauge->Init();
	}

	return pPollenGauge;
}

//==========================================================================
// ����������
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

		// ��ނ̐ݒ�
		m_apGauge[cnt]->SetType(CObject::TYPE::TYPE_NONE);
		m_apGauge[cnt]->SetSize(D3DXVECTOR2(DEFAULT_WIDTH, DEFAULT_HEIGHT));
		m_apGauge[cnt]->SetMoveFactor(0.25f);	// �ړ��̌W���ݒ�
	}
	
	SetType(CObject::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// �I������
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
	// ���폜
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPollen_Gauge::Update()
{
	for (int i = 0; i < VTXTYPE::VTXTYPE_MAX; i++)
	{
		// �T�C�Y�擾
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
// �`�揈��
//==========================================================================
void CPollen_Gauge::Draw()
{
	for (int i = 0; i < VTXTYPE::VTXTYPE_MAX; i++)
	{
		m_apGauge[i]->Draw();
	}
}

//==========================================================================
// �l�̕␳
//==========================================================================
void CPollen_Gauge::SetValue(int nValue)
{
	UtilFunc::Transformation::ValueNormalize(nValue, m_nMaxValue, 0);

	m_apGauge[VTXTYPE_GAUGE]->SetValue(static_cast<int>(nValue));
}

//==========================================================================
// ���_�ݒ�
//==========================================================================
void CPollen_Gauge::SetVtx(int nIdx)
{
	// ���_�ݒ�
	m_apGauge[nIdx]->SetVtx();

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	D3DXVECTOR2* pTex = m_apGauge[nIdx]->GetTex();

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_apGauge[nIdx]->GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	// �T�C�Y�擾
	D3DXVECTOR2 size = m_apGauge[nIdx]->GetSize();

	// ���_���W�̐ݒ�
	pVtx[0].tex = pTex[0];
	pVtx[1].tex = pTex[1];
	pVtx[2].tex = pTex[2];
	pVtx[3].tex = pTex[3];

	// ���_�o�b�t�@���A�����b�N���b�N
	m_apGauge[nIdx]->GetVtxBuff()->Unlock();
}