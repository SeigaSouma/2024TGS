//=============================================================================
// 
//  �������� [number.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "number.h"

// �h����
#include "number_2D.h"
#include "number_3D.h"
#include "number_Billboard.h"

//==========================================================================
// �}�N����`
//==========================================================================

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CNumber::CNumber(int nPriority)
{
	// �l�̃N���A
	m_objType = OBJECTTYPE_2D;	// �I�u�W�F�N�g�̎��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CNumber::~CNumber()
{

}

//==========================================================================
// ��������
//==========================================================================
CNumber *CNumber::Create(EObjectType objtype, int nPriority)
{
	// �����p�̃I�u�W�F�N�g
	CNumber *pNumber = nullptr;

	if (pNumber == nullptr)
	{// nullptr��������

	 // �������̊m��
		switch (objtype)
		{
		case CNumber::OBJECTTYPE_2D:
			pNumber = DEBUG_NEW CNumber2D;
			break;

		case CNumber::OBJECTTYPE_3D:
			pNumber = DEBUG_NEW CNumber3D(nPriority);
			break;

		case CNumber::OBJECTTYPE_BILLBOARD:
			pNumber = DEBUG_NEW CNumberBillboard;
			break;

		default:
			return nullptr;
			break;
		}

		if (pNumber != nullptr)
		{// �������̊m�ۂ��o���Ă�����

		 // �I�u�W�F�N�g�̎��
			pNumber->m_objType = objtype;

			// ����������
			pNumber->Init(nPriority);
		}
		else
		{
			delete pNumber;
			pNumber = nullptr;
		}

		return pNumber;
	}

	return nullptr;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CNumber::SetPosition(const MyLib::Vector3 pos)
{
}

//==========================================================================
// �ʒu�擾
//==========================================================================
MyLib::Vector3 CNumber::GetPosition() const
{
	return mylib_const::DEFAULT_VECTOR3;
}

//==========================================================================
// �ړ��ʐݒ�
//==========================================================================
void CNumber::SetMove(const MyLib::Vector3 move)
{
}

//==========================================================================
// �ړ��ʎ擾
//==========================================================================
MyLib::Vector3 CNumber::GetMove() const
{
	return mylib_const::DEFAULT_VECTOR3;
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CNumber::SetRotation(const MyLib::Vector3 rot)
{
}

//==========================================================================
// �����擾
//==========================================================================
MyLib::Vector3 CNumber::GetRotation() const
{
	return mylib_const::DEFAULT_VECTOR3;
}

//==========================================================================
// �F�ݒ�
//==========================================================================
void CNumber::SetColor(const D3DXCOLOR col)
{
}

//==========================================================================
// �F�擾
//==========================================================================
D3DXCOLOR CNumber::GetColor() const
{
	return mylib_const::DEFAULT_COLOR;
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CNumber::SetSize(const D3DXVECTOR2 size)
{
}

//==========================================================================
// �T�C�Y�擾
//==========================================================================
D3DXVECTOR2 CNumber::GetSize() const
{
	return D3DXVECTOR2(0.0f, 0.0f);
}

//==========================================================================
// ���̃T�C�Y�̐ݒ�
//==========================================================================
void CNumber::SetSizeOrigin(const D3DXVECTOR2 size)
{
}

//==========================================================================
// ���̃T�C�Y�̎擾
//==========================================================================
D3DXVECTOR2 CNumber::GetSizeOrigin() const
{
	return D3DXVECTOR2(0.0f, 0.0f);
}

//==========================================================================
// �T�C�Y�ݒ�
//==========================================================================
void CNumber::SetSize3D(const MyLib::Vector3 size)
{
}

//==========================================================================
// �T�C�Y�擾
//==========================================================================
MyLib::Vector3 CNumber::GetSize3D() const
{
	return MyLib::Vector3(0.0f, 0.0f, 0.0f);
}

//==========================================================================
// �e�N�X�`�����W�ݒ�
//==========================================================================
void CNumber::SetTex(D3DXVECTOR2 *tex)
{
}

//==========================================================================
// �e�N�X�`�����W�擾
//==========================================================================
D3DXVECTOR2 *CNumber::GetTex()
{
	return nullptr;
}

//==========================================================================
// �I�u�W�F�N�g2D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject2D *CNumber::GetObject2D()
{
	return nullptr;
}

//==========================================================================
// �I�u�W�F�N�g2D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject3D *CNumber::GetObject3D()
{
	return nullptr;
}

//==========================================================================
// �I�u�W�F�N�g�r���{�[�h�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectBillboard *CNumber::GetObjectBillboard()
{
	return nullptr;
}