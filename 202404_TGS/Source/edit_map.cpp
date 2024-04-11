//=============================================================================
// 
//  �}�b�v�G�f�B�^�\���� [edit_map.cpp]
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
	m_ModelFile.clear();	// ���f���t�@�C��
	m_pObjX.clear();		// �I�u�W�F�N�gX�̃|�C���^
	m_bGrab = false;		// �͂ݔ���
	m_bReGrab = false;		// �Ē͂ݔ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit_Map::~CEdit_Map()
{

}

//==========================================================================
// ��������
//==========================================================================
CEdit_Map* CEdit_Map::Create()
{
	// �������̊m��
	CEdit_Map* pMarker = DEBUG_NEW CEdit_Map;
	if (pMarker != nullptr)
	{
		// ����������
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEdit_Map::Init()
{
	Load();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevive = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �摜�̃��[�h
	for (const auto& file : m_ModelFile)
	{
		m_pTexture.emplace_back();

		HRESULT hr = D3DXCreateTextureFromFileEx(pDevive, file.c_str(), 0, 0, 0, 0, D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
			NULL, NULL, &m_pTexture.back());
	}

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
	ImGuiDragDropFlags src_flags = 0;
	src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
	src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
	//src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip

	ImVec2 imageSize = ImVec2(50, 50);
	for (int i = 0; i < static_cast<int>(m_nModelIdx.size()); i++)
	{
		ImGui::PushID(i); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			// �h���b�O�\�ȗv�f�̕`��
			ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_pTexture[i]), imageSize);

			if (ImGui::BeginDragDropSource(src_flags))
			{
				SDragDropData dragData;
				dragData.nType = m_nModelIdx[i];
				m_DragData.nType = m_nModelIdx[i];

				ImGui::SetDragDropPayload("MY_COORDINATE_TYPE", &dragData, sizeof(SDragDropData));
				ImGui::Text(m_ModelFile[i].c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseDown(0))
			{// UI��ɃJ�[�\�� && �N���b�N
				m_bGrab = true;
			}
		}
		ImGui::PopID();
	}

	// �E�B���h�E�̃}�E�X�z�o�[����
	ImGuiHoveredFlags frag = 128;
	bool bHoverWindow = ImGui::IsWindowHovered(frag);


	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = CInputMouse::GetInstance()->GetRay();
	MyLib::Vector3 mousePos = CInputMouse::GetInstance()->GetNearPosition();
	MyLib::Vector3 mouseWorldPos = CInputMouse::GetInstance()->GetWorldPosition();

	if (m_bGrab && 
		!bHoverWindow) {

		// �E�B���h�E�O && �I�u�W�F�N�g������ԂŐ���
		if (m_DragData.objX == nullptr)
		{
			m_DragData.objX = CObjectX::Create(m_DragData.nType, mouseWorldPos, MyLib::Vector3(0.0f), true);
			m_DragData.objX->SetType(CObject::TYPE_XFILE);
		}
	}
	else {

		// �E�B���h�E�� && �I�u�W�F�N�g�����Ԃō폜
		if (m_DragData.objX != nullptr) {
			m_DragData.objX->Kill();
			m_DragData.objX = nullptr;
		}
	}

	// �h���b�O���ʒu�X�V
	if (m_DragData.objX != nullptr) {
		m_DragData.objX->SetPosition(mouseWorldPos);
	}

	// �z�u
	if (m_bGrab &&
		ImGui::IsMouseReleased(0))
	{// �͂ݒ� && �}�E�X�����[�X

		if (!bHoverWindow) {
			Regist(m_DragData.nType, mouseWorldPos, MyLib::Vector3(0.0f), true);
		}
		m_bGrab = false;
	}





	// �擪��ۑ�
	CObjectX* pObject = nullptr;

	// ���X�g�R�s�[
	std::vector<CObjectX*> pObjectSort;
	while (m_List.ListLoop(&pObject))
	{
		// �v�f�𖖔��ɒǉ�
		pObjectSort.push_back(pObject);
	}

	// Z�\�[�g
	std::sort(pObjectSort.begin(), pObjectSort.end(), CObject::ZSort);

	for (const auto& obj : pObjectSort)
	{
		MyLib::AABB aabb;
		aabb.vtxMin = obj->GetVtxMin();
		aabb.vtxMax = obj->GetVtxMax();

		D3DXMATRIX mat = obj->GetWorldMtx();
		float time = 0.0f;
		MyLib::Vector3 OBpos;

		bool bHit = UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mat, time, &OBpos);

		if (bHit &&
			ImGui::IsMouseClicked(0)) 
		{// ����Ă� && �N���b�N
			
		}

		if(bHit)
			ImGui::Text("iineeeeeeeeeeeeeeeeeeeeeeeeeee");

	}

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
		
		// ���f���C���f�b�N�X����
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