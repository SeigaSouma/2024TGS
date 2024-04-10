//=============================================================================
// 
//  �퓬�J�n���� [edit_map.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit_map.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* LOADTEXT = "data\\TEXT\\map\\info.txt";				// �e�N�X�`���̃t�@�C��
}
CListManager<CObjectX> CEdit_Map::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEdit_Map::CEdit_Map()
{
	// �l�̃N���A
	m_nModelIdx.clear();	// ���f���C���f�b�N�X
	m_pObjX.clear();		// �I�u�W�F�N�gX�̃|�C���^
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit_Map::~CEdit_Map()
{

}
//
////==========================================================================
//// ��������
////==========================================================================
//CEdit_Map* CEdit_Map::Create()
//{
//	// �������̊m��
//	CEdit_Map* pMarker = DEBUG_NEW CEdit_Map;
//
//	if (pMarker != nullptr)
//	{// �������̊m�ۂ��o���Ă�����
//
//		// ����������
//		pMarker->Init();
//	}
//
//	return pMarker;
//}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEdit_Map::Init()
{

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEdit_Map::Uninit()
{

}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_Map::Update()
{
	
}

//==========================================================================
// �Z�[�u
//==========================================================================
void CEdit_Map::Save()
{
	// �t�@�C�����J��
	FILE* pFile = fopen("data\\TEXT\\map\\save_info.txt", "w");
	if (pFile == nullptr)
	{// �t�@�C�����J���Ȃ������ꍇ
		return;
	}

	// �t�@�C���ɏ����o��
	fprintf(pFile,
		"\n"
		"#------------------------------------------------------------------------------\n"
		"# ���f����\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", (int)m_nModelIdx.size());

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# ���f���t�@�C����\n"
		"#------------------------------------------------------------------------------\n");

	// ���f���t�@�C������
	for (int i = 0; i < (int)m_nModelIdx.size(); i++)
	{
		fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", m_ModelFile[i].c_str(), i);
	}

	//=============================
	// ���f��
	//=============================
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# ���f���̔z�u\n"
		"#==============================================================================\n");

	// ���X�g���[�v
	CObjectX* pObj = nullptr;
	while (m_List.ListLoop(&pObj))
	{
		MyLib::Vector3 pos = pObj->GetPosition();	// �ʒu
		MyLib::Vector3 rot = pObj->GetRotation();	// ����
		int nShadow = 0;							// �e�g�����ǂ���
		int nType = 0;								// ���

		if (pObj->GetUseShadow() == true)
		{// �g���Ă���ꍇ
			nShadow = 1;
		}
		
		std::vector<int>::iterator nIdx = std::find(m_nModelIdx.begin(), m_nModelIdx.end(), pObj->GetIdxXFile());

		// �o��
		fprintf(pFile,
			"MODELSET\n"
			"\tTYPE = %d\n"
			"\tPOS = %.2f %.2f %.2f\n"
			"\tROT = %.2f %.2f %.2f\n"
			"\tSHADOW = %d\n"
			"END_MODELSET\n\n",
			(*nIdx), pos.x, pos.y, pos.z,
			rot.x, rot.y, rot.z, nShadow);
	}

	fprintf(pFile, "\nEND_SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI");

	//�t�@�C�������
	fclose(pFile);
}

//==========================================================================
// ���[�h
//==========================================================================
void CEdit_Map::Load()
{
	std::string FileName;	// �t�@�C����
	char aComment[MAX_COMMENT] = {};	//�R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�
	int nCntTexture = 0;				// �e�N�X�`���ǂݍ��݃J�E���g

	// X�t�@�C���ǂݍ���
	ReadXFile();

	// �}�b�v�t�@�C�����J��
	FILE* pFile = fopen(LOADTEXT, "r");
	if (pFile == nullptr)
	{//�t�@�C�����J�����ꍇ
		return;
	}

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���f���̐ݒ�
		if (strcmp(&aComment[0], "MODELSET") == 0)
		{
			int nType = 0, nShadow = 0;
			MyLib::Vector3 pos, rot;

			while (strcmp(&aComment[0], "END_MODELSET"))
			{// END_MODELSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TYPE") == 0)
				{// TYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &nType);		// ���f����ނ̗�
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POS��������ʒu�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &pos.x);	// X���W
					fscanf(pFile, "%f", &pos.y);	// Y���W
					fscanf(pFile, "%f", &pos.z);	// Z���W
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROT������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &rot.x);	// X�̌���
					fscanf(pFile, "%f", &rot.y);	// Y�̌���
					fscanf(pFile, "%f", &rot.z);	// Z�̌���
				}

				if (strcmp(&aComment[0], "SHADOW") == 0)
				{// SHADOW��������e�g�p

					fscanf(pFile, "%s", &aComment[0]);		// =�̕�
					fscanf(pFile, "%d", &nShadow);			// �e���g�����ǂ���
				}

			}// END_MODELSET�̂�����

			// �ǉ�
			bool bShadow = (nShadow == 1);
			Regist(nType, pos, rot, bShadow);
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���
			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);
}

//==========================================================================
// ���f���ǂݍ��ݏ���
//==========================================================================
HRESULT CEdit_Map::ReadXFile()
{
	char aComment[MAX_COMMENT] = {};	// �R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�

	// �t�@�C�����J��
	FILE* pFile = fopen(LOADTEXT, "r");

	if (pFile == nullptr)
	{// �t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	int modelNum = 0;
	m_nModelIdx.clear();
	m_ModelFile.clear();

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���f�����̐ݒ�
		if (strcmp(&aComment[0], "NUM_MODEL") == 0)
		{// NUM_MODEL��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &nFileNum);		// ���f����
		}

		while (modelNum != nFileNum)
		{// ���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			// ������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			// ���f�����̐ݒ�
			if (strcmp(&aComment[0], "MODEL_FILENAME") == 0)
			{// NUM_MODEL��������

				fscanf(pFile, "%s", &aComment[0]);	// =�̕�
				fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

				// �ǉ�
				m_ModelFile.push_back(&aComment[0]);
				m_nModelIdx.push_back(0);

				// �C���f�b�N�X�擾
				m_nModelIdx.back() = CXLoad::GetInstance()->XLoad(aComment);

				modelNum++;	// ���f�������Z
			}
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���

			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// �폜
//==========================================================================
void CEdit_Map::Delete(CObjectX* obj)
{
	// �I������
	std::vector<CObjectX*>::iterator itr = std::find(m_pObjX.begin(), m_pObjX.end(), obj);

	// �v�f�����������ꍇ�̂ݍ폜
	if (itr != m_pObjX.end())
	{
		// ���X�g����폜
		m_List.Delete((*itr));

		(*itr)->Kill();
		m_pObjX.erase(itr);
	}
}

//==========================================================================
// ���蓖��
//==========================================================================
void CEdit_Map::Regist(int idx, MyLib::Vector3 pos, MyLib::Vector3 rot, bool bShadow)
{

	m_pObjX.emplace_back();

	// �^�C�v�̕��𐶐�
	m_pObjX.back() = CObjectX::Create(m_nModelIdx[idx], pos, rot, bShadow);
	m_pObjX.back()->SetType(CObject::TYPE_XFILE);

	// ���X�g�ɒǉ�
	m_List.Regist(m_pObjX.back());
}