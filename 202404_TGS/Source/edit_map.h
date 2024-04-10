//=============================================================================
// 
//  �}�b�v�G�f�B�^�\�w�b�_�[ [edit_map.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EDIT_MAP_H_
#define _EDIT_MAP_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectX.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�G�f�B�^�\�N���X
class CEdit_Map
{
public:

	CEdit_Map();
	~CEdit_Map();
	
	HRESULT Init();
	void Uninit();
	void Update();


	void Save();	// �Z�[�u
	void Load();	// ���[�h
	void Delete(CObjectX* obj);	// �폜
	void Regist(int idx, MyLib::Vector3 pos, MyLib::Vector3 rot, bool bShadow);	// ���蓖��

	static CListManager<CObjectX> GetListObj() { return m_List; }	// ���X�g�擾

private:

	// �\���̒�`
	struct SMapModelInfo
	{
		int nIdx;
		std::string fileName;
	};

	HRESULT ReadXFile();

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<int> m_nModelIdx;	// ���f���C���f�b�N�X
	std::vector<std::string> m_ModelFile;	// ���f���C���f�b�N�X
	std::vector<CObjectX*> m_pObjX;	// �I�u�W�F�N�gX�̃|�C���^
	static CListManager<CObjectX> m_List;	// ���X�g

};

#endif