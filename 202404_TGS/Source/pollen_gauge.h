//=============================================================================
// 
//  �ԕ��ʃQ�[�W�w�b�_�[ [pollen_gauge.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _POLLEN_GAUGE_H_
#define _POLLEN_GAUGE_H_	// ��d�C���N���[�h�h�~

#include "object2D_gauge.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �̗̓Q�[�W�N���X��`
class CPollen_Gauge : public CObject
{
public:

	CPollen_Gauge(int nPriority = 11);
	~CPollen_Gauge();

	static CPollen_Gauge *Create(MyLib::Vector3 pos, int nMaxPollen);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update() override;
	void Draw();

	void SetValue(int nValue);

private:
	// ���_��
	enum VTXTYPE
	{
		VTXTYPE_BG = 0,	// �w�i
		VTXTYPE_GAUGE,	// �Q�[�W����
		VTXTYPE_MAX
	};

	//�����o�֐�
	void SetVtx(int nIdx);

	// �����o�ϐ�
	CObject2D_Gauge* m_apGauge[VTXTYPE_MAX];
	int m_nMaxValue;
};


#endif