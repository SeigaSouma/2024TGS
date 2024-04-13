//=============================================================================
// 
//  �I�u�W�F�N�g���C���w�b�_�[ [objectLine.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT3D_ANIM_H_
#define _OBJECT3D_ANIM_H_	// ��d�C���N���[�h�h�~

#include "object.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g���C���N���X
class CObjectLine : public CObject
{
public:

	//=============================
	// �\���̒�`
	//=============================
	struct SLineInfo
	{
		MyLib::Vector3 left;
		MyLib::Vector3 right;
		D3DXCOLOR col;

		SLineInfo() : left(0.0f), right(0.0f), col(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)) {}
	};

	CObjectLine(int nPriority = mylib_const::PRIORITY_DEF2D);
	virtual ~CObjectLine();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void SetVtx() override;

	void SetWorldMtx(const D3DXMATRIX mtx) { m_mtxWorld = mtx; }	// �}�g���b�N�X�ݒ�
	D3DXMATRIX GetWorldMtx() const { return m_mtxWorld; }			// �}�g���b�N�X�擾

	static CObjectLine *Create(MyLib::Vector3 left, MyLib::Vector3 right, D3DXCOLOR col);

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	SLineInfo m_Info;
};



#endif