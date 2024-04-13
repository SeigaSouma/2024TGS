//=============================================================================
// 
// �����蔻��{�b�N�X���� [collisionLine_Box.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "collisionLine_Box.h"
#include "manager.h"
#include "calculation.h"
#include "3D_Effect.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCollisionLine_Box::CCollisionLine_Box(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	memset(m_pLine, 0, sizeof(m_pLine));
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCollisionLine_Box::~CCollisionLine_Box()
{

}

//==========================================================================
// ��������
//==========================================================================
CCollisionLine_Box* CCollisionLine_Box::Create(const MyLib::AABB& aabb, const D3DXCOLOR& col)
{
	
	// �������̊m��
	CCollisionLine_Box* pBox = DEBUG_NEW CCollisionLine_Box;

	if (pBox != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		pBox->m_AABB = aabb;
		pBox->m_LineColor = col;

		// ����������
		pBox->Init();
	}

	return pBox;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCollisionLine_Box::Init()
{

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// �{�b�N�X�̒��_
	MyLib::Vector3 vtx[8];
	vtx[0] = m_AABB.vtxMin; // �ŏ����_
	vtx[1] = MyLib::Vector3(m_AABB.vtxMin.x, m_AABB.vtxMin.y, m_AABB.vtxMax.z);
	vtx[2] = MyLib::Vector3(m_AABB.vtxMin.x, m_AABB.vtxMax.y, m_AABB.vtxMin.z);
	vtx[3] = MyLib::Vector3(m_AABB.vtxMin.x, m_AABB.vtxMax.y, m_AABB.vtxMax.z);

	vtx[4] = MyLib::Vector3(m_AABB.vtxMax.x, m_AABB.vtxMin.y, m_AABB.vtxMin.z);
	vtx[5] = MyLib::Vector3(m_AABB.vtxMax.x, m_AABB.vtxMin.y, m_AABB.vtxMax.z);
	vtx[6] = MyLib::Vector3(m_AABB.vtxMax.x, m_AABB.vtxMax.y, m_AABB.vtxMin.z);
	vtx[7] = m_AABB.vtxMax; // �ő咸�_

#if 0
	// ��ʂ̕�
	m_pLine[0] = CObjectLine::Create(vtx[0], vtx[1], m_LineColor);
	m_pLine[1] = CObjectLine::Create(vtx[1], vtx[5], m_LineColor);
	m_pLine[2] = CObjectLine::Create(vtx[5], vtx[4], m_LineColor);
	m_pLine[3] = CObjectLine::Create(vtx[4], vtx[0], m_LineColor);

	// ��ʂ̕�
	m_pLine[4] = CObjectLine::Create(vtx[3], vtx[2], m_LineColor);
	m_pLine[5] = CObjectLine::Create(vtx[2], vtx[6], m_LineColor);
	m_pLine[6] = CObjectLine::Create(vtx[6], vtx[7], m_LineColor);
	m_pLine[7] = CObjectLine::Create(vtx[7], vtx[3], m_LineColor);

	// ���ʂ̕�
	m_pLine[8] = CObjectLine::Create(vtx[0], vtx[2], m_LineColor);
	m_pLine[9] = CObjectLine::Create(vtx[1], vtx[3], m_LineColor);
	m_pLine[10] = CObjectLine::Create(vtx[4], vtx[6], m_LineColor);
	m_pLine[11] = CObjectLine::Create(vtx[5], vtx[7], m_LineColor);
#else

	// �C���f�b�N�X�i�[
	int idx[12][2] = 
	{
		{0, 1}, {1, 5}, {5, 4}, {4, 0}, // ��ʂ̕�
		{3, 2}, {2, 6}, {6, 7}, {7, 3}, // ��ʂ̕�
		{0, 2}, {1, 3}, {4, 6}, {5, 7}  // ���ʂ̕�
	};

	// ���C������
	for (int i = 0; i < 12; ++i) 
	{
		m_pLine[i] = CObjectLine::Create(vtx[idx[i][0]], vtx[idx[i][1]], m_LineColor);
	}
#endif

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCollisionLine_Box::Uninit()
{
	for (int i = 0; i < 12; i++)
	{
		if (m_pLine[i] == nullptr){
			continue;
		}
		m_pLine[i] = nullptr;
	}

	// �I������
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CCollisionLine_Box::Kill()
{
	for (int i = 0; i < 12; i++)
	{
		if (m_pLine[i] == nullptr) {
			continue;
		}
		m_pLine[i]->Uninit();
		m_pLine[i] = nullptr;
	}

	// �I������
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CCollisionLine_Box::Update()
{
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	bool bDisp = IsDisp();

	for (int i = 0; i < 12; ++i)
	{
		if (m_pLine[i] == nullptr) {
			continue;
		}
		m_pLine[i]->SetPosition(pos);
		m_pLine[i]->SetRotation(rot);
		m_pLine[i]->SetEnableDisp(bDisp);
	}
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CCollisionLine_Box::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);
	for (int i = 0; i < 12; ++i)
	{
		if (m_pLine[i] == nullptr) {
			continue;
		}
		m_pLine[i]->SetPosition(pos);
	}
}