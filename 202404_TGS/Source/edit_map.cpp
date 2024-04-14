//=============================================================================
// 
//  マップエディタ―処理 [edit_map.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "edit_map.h"
#include "manager.h"
#include "calculation.h"
#include "collisionLine_Box.h"
#include "camera.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* LOADTEXT = "data\\TEXT\\map\\info.txt";				// テクスチャのファイル
}
CListManager<CObjectX> CEdit_Map::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CEdit_Map::CEdit_Map()
{
	// 値のクリア
	m_nModelIdx.clear();	// モデルインデックス
	m_ModelFile.clear();	// モデルファイル
	m_pObjX.clear();		// オブジェクトXのポインタ
	m_bGrab = false;		// 掴み判定
	m_bReGrab = false;		// 再掴み判定
	m_pGrabObj = nullptr;	// 掴みオブジェクト
	m_pHandle = nullptr;	// 移動ハンドル
	m_moveAngle = CHandle_Move::HandleAngle::ANGLE_X;	// 移動の向き
}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit_Map::~CEdit_Map()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CEdit_Map* CEdit_Map::Create()
{
	// メモリの確保
	CEdit_Map* pMarker = DEBUG_NEW CEdit_Map;
	if (pMarker != nullptr)
	{
		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit_Map::Init()
{
	Load();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevive = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 画像のロード
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
// 終了処理
//==========================================================================
void CEdit_Map::Uninit()
{
	delete this;
}

//==========================================================================
// 更新処理
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
		ImGui::PushID(i); // ウィジェットごとに異なるIDを割り当てる
		{
			// ドラッグ可能な要素の描画
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
			{// UI上にカーソル && クリック
				m_bGrab = true;
			}
		}
		ImGui::PopID();
	}

	// ウィンドウのマウスホバー判定
	ImGuiHoveredFlags frag = 128;
	bool bHoverWindow = ImGui::IsWindowHovered(frag);


	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();
	MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();
	MyLib::Vector3 mouseOldWorldPos = pMouse->GetOldWorldPosition();
	MyLib::Vector3 diffpos = pMouse->GetDiffPosition();

	if (m_bGrab && 
		!bHoverWindow) {

		// ウィンドウ外 && オブジェクト無い状態で生成
		if (m_DragData.objX == nullptr)
		{
			m_DragData.objX = CObjectX::Create(m_DragData.nType, mouseWorldPos, MyLib::Vector3(0.0f), true);
			m_DragData.objX->SetType(CObject::TYPE_XFILE);
			m_DragData.objX->CreateCollisionBox();
		}
	}
	else {

		// ウィンドウ内 && オブジェクトある状態で削除
		if (m_DragData.objX != nullptr) {
			m_DragData.objX->Kill();
			m_DragData.objX = nullptr;
		}
	}

	// ドラッグ中位置更新
	if (m_DragData.objX != nullptr) {
		m_DragData.objX->SetState(CObjectX::STATE::STATE_EDIT);

		MyLib::Vector3 setpos = mouseWorldPos;
		setpos.y = 0.0f;
		m_DragData.objX->SetPosition(setpos);

		if (m_DragData.objX->GetCollisionLineBox() != nullptr) {
			m_DragData.objX->GetCollisionLineBox()->SetPosition(setpos);
		}
	}

	// 配置
	if (m_bGrab &&
		ImGui::IsMouseReleased(0))
	{// 掴み中 && マウスリリース

		if (!bHoverWindow) {

			MyLib::Vector3 setpos = mouseWorldPos;
			setpos.y = 0.0f;
			Regist(m_DragData.nType, setpos, MyLib::Vector3(0.0f), true);
		}
		m_bGrab = false;
	}

	// 移動ハンドル判定
	if (!m_bGrab &&
		m_pHandle != nullptr &&
		!m_bReGrab)
	{
		bool bHit = false, bChange = false;

		// リストループ
		CListManager<CObjectX> handleList = CHandle_Move::GetList();
		CObjectX* pArrow = nullptr;
		int i = 0;
		while (handleList.ListLoop(&pArrow))
		{
			pArrow->SetColor(mylib_const::DEFAULT_COLOR);

			if (!bHit && !bHoverWindow)
			{
				MyLib::AABB aabb = pArrow->GetAABB();
				D3DXMATRIX mtx = pArrow->GetWorldMtx();
				float time = 0.0f;

				bHit = UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time);
			}

			if (bHit && !bChange)
			{
				pArrow->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
				bChange = true;

				if (!pKeyboard->GetPress(DIK_LALT) &&
					ImGui::IsMouseClicked(0))
				{// クリック

					m_bReGrab = true;

					// 移動の向き
					m_moveAngle = static_cast<CHandle_Move::HandleAngle>(i);
				}
				return;
			}

			i++;
		}
	}

	// 再移動中
	if (m_bReGrab)
	{
		MyLib::Vector3 pos = m_pGrabObj->GetPosition();

		switch (m_moveAngle)
		{
		case CHandle_Move::ANGLE_Z:
			pos.z += diffpos.z;
			break;

		case CHandle_Move::ANGLE_Y:
			pos.y += diffpos.y * m_pHandle->GetScale();
			break;

		case CHandle_Move::ANGLE_X:
			pos.x += diffpos.x;
			break;
		}

		m_pGrabObj->SetPosition(pos);

		if (m_pGrabObj->GetCollisionLineBox() != nullptr) {
			m_pGrabObj->GetCollisionLineBox()->SetPosition(pos);
		}

		if (m_pHandle != nullptr) {
			m_pHandle->SetPosition(m_pGrabObj->GetPosition());
		}

	}

	if (m_bReGrab &&
		!pKeyboard->GetPress(DIK_LALT) &&
		ImGui::IsMouseReleased(0))
	{// クリック
		m_bReGrab = false;
	}

	if (!pKeyboard->GetPress(DIK_LALT) &&
		!bHoverWindow &&
		ImGui::IsMouseClicked(0))
	{// クリック

		m_pGrabObj = nullptr;

		// 先頭を保存
		CObjectX* pObject = nullptr;

		// リストコピー
		std::vector<CObjectX*> pObjectSort;
		while (m_List.ListLoop(&pObject))
		{
			// 要素を末尾に追加
			pObjectSort.push_back(pObject);
		}

		// Zソート
		std::sort(pObjectSort.begin(), pObjectSort.end(), CObject::ZSortInverse);

		bool bHit = false;
		for (const auto& obj : pObjectSort)
		{
			if (!bHit) {

				MyLib::AABB aabb;
				aabb.vtxMin = obj->GetVtxMin();
				aabb.vtxMax = obj->GetVtxMax();

				D3DXMATRIX mat = obj->GetWorldMtx();
				float time = 0.0f;
				MyLib::Vector3 OBpos;

				bHit = UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mat, time, &OBpos);

				if (bHit)
				{// 被ってる
					obj->SetState(CObjectX::STATE::STATE_EDIT);
					
					// 掴みオブジェクト
					m_pGrabObj = obj;

					if (m_pHandle != nullptr) {
						m_pHandle->SetPosition(m_pGrabObj->GetPosition());
					}

					if (m_pHandle == nullptr) {
						m_pHandle = CHandle_Move::Create(m_pGrabObj->GetPosition());
					}
				}
				else {
					obj->SetState(CObjectX::STATE::STATE_NONE);
				}
			}
			else {
				obj->SetState(CObjectX::STATE::STATE_NONE);
			}
		}

		if (!bHit && m_pHandle != nullptr) {
			m_pHandle->Kill();
			m_pHandle = nullptr;
		}
	}

}

//==========================================================================
// セーブ
//==========================================================================
void CEdit_Map::Save()
{
	// ファイルを開く
	FILE* pFile = fopen("data\\TEXT\\map\\save_info.txt", "w");
	if (pFile == nullptr)
	{// ファイルが開けなかった場合
		return;
	}

	// ファイルに書き出す
	fprintf(pFile,
		"\n"
		"#------------------------------------------------------------------------------\n"
		"# モデル数\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", (int)m_nModelIdx.size());

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# モデルファイル名\n"
		"#------------------------------------------------------------------------------\n");

	// モデルファイル名分
	for (int i = 0; i < (int)m_nModelIdx.size(); i++)
	{
		fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", m_ModelFile[i].c_str(), i);
	}

	//=============================
	// モデル
	//=============================
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# モデルの配置\n"
		"#==============================================================================\n");

	// リストループ
	CObjectX* pObj = nullptr;
	while (m_List.ListLoop(&pObj))
	{
		MyLib::Vector3 pos = pObj->GetPosition();	// 位置
		MyLib::Vector3 rot = pObj->GetRotation();	// 向き
		int nShadow = 0;							// 影使うかどうか
		int nType = 0;								// 種類

		if (pObj->GetUseShadow() == true)
		{// 使っている場合
			nShadow = 1;
		}
		
		// モデルインデックス検索
		std::vector<int>::iterator nIdx = std::find(m_nModelIdx.begin(), m_nModelIdx.end(), pObj->GetIdxXFile());

		// 出力
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

	fprintf(pFile, "\nEND_SCRIPT		# この行は絶対消さないこと！");

	//ファイルを閉じる
	fclose(pFile);
}

//==========================================================================
// ロード
//==========================================================================
void CEdit_Map::Load()
{
	char aComment[MAX_COMMENT] = {};	//コメント用
	int nFileNum = 0;					// ファイルの数
	int nCntTexture = 0;				// テクスチャ読み込みカウント

	// Xファイル読み込み
	ReadXFile();

	// マップファイルを開く
	FILE* pFile = fopen(LOADTEXT, "r");
	if (pFile == nullptr)
	{//ファイルが開けた場合
		return;
	}

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// モデルの設定
		if (strcmp(&aComment[0], "MODELSET") == 0)
		{
			int nType = 0, nShadow = 0;
			MyLib::Vector3 pos, rot;

			while (strcmp(&aComment[0], "END_MODELSET"))
			{// END_MODELSETが来るまで繰り返し

				fscanf(pFile, "%s", &aComment[0]);	// 確認する

				if (strcmp(&aComment[0], "TYPE") == 0)
				{// TYPEが来たら種類読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%d", &nType);		// モデル種類の列挙
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POSが来たら位置読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &pos.x);	// X座標
					fscanf(pFile, "%f", &pos.y);	// Y座標
					fscanf(pFile, "%f", &pos.z);	// Z座標
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROTが来たら向き読み込み

					fscanf(pFile, "%s", &aComment[0]);	// =の分
					fscanf(pFile, "%f", &rot.x);	// Xの向き
					fscanf(pFile, "%f", &rot.y);	// Yの向き
					fscanf(pFile, "%f", &rot.z);	// Zの向き
				}

				if (strcmp(&aComment[0], "SHADOW") == 0)
				{// SHADOWが来たら影使用

					fscanf(pFile, "%s", &aComment[0]);		// =の分
					fscanf(pFile, "%d", &nShadow);			// 影を使うかどうか
				}

			}// END_MODELSETのかっこ

			// 追加
			bool bShadow = (nShadow == 1);
			Regist(nType, pos, rot, bShadow);
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);
}

//==========================================================================
// モデル読み込み処理
//==========================================================================
HRESULT CEdit_Map::ReadXFile()
{
	char aComment[MAX_COMMENT] = {};	// コメント用
	int nFileNum = 0;					// ファイルの数

	// ファイルを開く
	FILE* pFile = fopen(LOADTEXT, "r");
	if (pFile == nullptr)
	{// ファイルが開けた場合
		return E_FAIL;
	}

	int modelNum = 0;
	m_nModelIdx.clear();
	m_ModelFile.clear();

	while (1)
	{// END_SCRIPTが来るまで繰り返す

		// 文字列の読み込み
		fscanf(pFile, "%s", &aComment[0]);

		// モデル数の設定
		if (strcmp(&aComment[0], "NUM_MODEL") == 0)
		{// NUM_MODELがきたら

			fscanf(pFile, "%s", &aComment[0]);	// =の分
			fscanf(pFile, "%d", &nFileNum);		// モデル数
		}

		while (modelNum != nFileNum)
		{// モデルの数分読み込むまで繰り返し

			// 文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			// モデル名の設定
			if (strcmp(&aComment[0], "MODEL_FILENAME") == 0)
			{// NUM_MODELがきたら

				fscanf(pFile, "%s", &aComment[0]);	// =の分
				fscanf(pFile, "%s", &aComment[0]);	// ファイル名

				// 追加
				m_ModelFile.push_back(&aComment[0]);
				m_nModelIdx.push_back(0);

				// インデックス取得
				m_nModelIdx.back() = CXLoad::GetInstance()->XLoad(aComment);

				modelNum++;	// モデル数加算
			}
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// 終了文字でループを抜ける
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// 削除
//==========================================================================
void CEdit_Map::Delete(CObjectX* obj)
{
	// 終了する
	std::vector<CObjectX*>::iterator itr = std::find(m_pObjX.begin(), m_pObjX.end(), obj);

	// 要素が見つかった場合のみ削除
	if (itr != m_pObjX.end())
	{
		// リストから削除
		m_List.Delete((*itr));

		(*itr)->Kill();
		m_pObjX.erase(itr);
	}
}

//==========================================================================
// 割り当て
//==========================================================================
void CEdit_Map::Regist(int idx, MyLib::Vector3 pos, MyLib::Vector3 rot, bool bShadow)
{

	m_pObjX.emplace_back();

	// タイプの物を生成
	m_pObjX.back() = CObjectX::Create(m_nModelIdx[idx], pos, rot, bShadow);
	m_pObjX.back()->SetType(CObject::TYPE_XFILE);
	m_pObjX.back()->CreateCollisionBox();

	// リストに追加
	m_List.Regist(m_pObjX.back());

	if (m_bGrab) {

		// リストコピー
		CObjectX* pObject = nullptr;
		while (m_List.ListLoop(&pObject))
		{
			pObject->SetState(CObjectX::STATE::STATE_NONE);
		}

		// 掴みオブジェクト
		m_pGrabObj = m_pObjX.back();

		m_pObjX.back()->SetState(CObjectX::STATE::STATE_EDIT);

		if (m_pHandle != nullptr) {
			m_pHandle->SetPosition(m_pGrabObj->GetPosition());
		}

		if (m_pHandle == nullptr) {
			m_pHandle = CHandle_Move::Create(m_pGrabObj->GetPosition());
		}
	}
}