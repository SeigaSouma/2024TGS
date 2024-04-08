//=============================================================================
// 
//  �����w�b�_�[ [busket.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _BUSKET_H_
#define _BUSKET_H_	// ��d�C���N���[�h�h�~

#include "object.h"
class CPollen_Gauge;

//==========================================================================
// �N���X��`
//==========================================================================
// �̗̓Q�[�W�N���X��`
class CBusket : public CObject
{
public:

	CBusket(int nMaxPollen, int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CBusket();

	static CBusket *Create(int nMaxPollen);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	//�ԕ����炷
	void Lost(void);
	void Boost(void);

	void SetDisp(bool disp);

	//�擾
	int GetPollen(void) { return m_nPollen; }
	bool IsEmpty(void) { return m_bEmpty; }

private:

	CPollen_Gauge* m_pPollenGauge;
	int m_nPollen;				//���݂̉ԕ���
	const int m_nMaxPollen;		//�ő�ԕ���
	bool m_bEmpty;				//������ۂɂȂ������ǂ���
};


#endif