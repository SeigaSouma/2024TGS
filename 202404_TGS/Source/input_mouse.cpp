//=============================================================================
//
// 入力(マウス)処理 [input_mouse.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "input_mouse.h"
#include "debugproc.h"
#include "calculation.h"
#include "manager.h"
#include "camera.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float MOUSE_SENS = 0.5f;	// マウス感度の補正
}
CInputMouse* CInputMouse::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CInputMouse::CInputMouse()
{
	memset(&m_MouseState, 0, sizeof(DIMOUSESTATE2));
	memset(&m_MouseStateTrigger, 0, sizeof(XINPUT_STATE));
	memset(&m_MouseStateRerease, 0, sizeof(XINPUT_STATE));
	memset(&m_MouseStateRepeat, 0, sizeof(XINPUT_STATE));
	memset(&m_aOldState, 0, sizeof(m_aOldState));	// 前回の入力情報
	m_pViewMtx = nullptr;	// ビューマトリックス
	m_pPrjMtx = nullptr;	// プロジェクションマトリックス
	m_OldWorldPos = MyLib::Vector3();	// 前回のワールド座標
	m_Diffposition = MyLib::Vector3();	// 差分
}

//==========================================================================
// デストラクタ
//==========================================================================
CInputMouse::~CInputMouse()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CInputMouse* CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pThisPtr != nullptr) {
		return m_pThisPtr;
	}

	// インスタンス生成
	m_pThisPtr = DEBUG_NEW CInputMouse;
	if (m_pThisPtr != nullptr) {

		// 初期化処理
		m_pThisPtr->Init(hInstance, hWnd);
	}

	return m_pThisPtr;
}

//==========================================================================
// マウスの初期化
//==========================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 初期化処理
	CInput::Init(hInstance, hWnd);

	// 入力デバイスの設定
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定(絶対値はDIPROPAXISMODE_ABS)

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	// マウスへのアクセス権を取得
	m_pDevice->Acquire();

	// マウスカーソルの消去
	//ShowCursor(false);

	return S_OK;
}

//==========================================================================
// マウスの終了処理
//==========================================================================
void CInputMouse::Uninit()
{
	// 終了処理
	CInput::Uninit();

	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// マウスの更新処理
//==========================================================================
void CInputMouse::Update()
{
	DIMOUSESTATE2 mouse; // マウスの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(mouse), &mouse)))
	{
		// 入力情報の保存
		m_MouseState = mouse;
	}
	else
	{
		// マウスへのアクセス権を獲得
		m_pDevice->Acquire();
	}

	GetCursorPos(&m_pos);
	ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &m_pos);

	int x = m_pos.x, y = m_pos.y;
	UtilFunc::Transformation::ValueNormalize(x, 1280, 0);
	UtilFunc::Transformation::ValueNormalize(y, 720, 0);
	m_pos.x = x, m_pos.y = y;

	// テキストの描画
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- マウス情報 ----------------\n"
		"【位置】[X：%d Y：%d]\n",
		m_pos.x, m_pos.y);

	// 位置設定
	m_OldWorldPos = m_WorldPos;
	m_WorldPos = UtilFunc::Transformation::CalcScreenToXZ(GetPosition(), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), *m_pViewMtx, *m_pPrjMtx);
	m_WorldPos.y = UtilFunc::Transformation::CalcScreenToY(GetPosition(), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), *m_pViewMtx, *m_pPrjMtx).y;

	m_Diffposition = m_WorldPos - m_OldWorldPos;

	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr) {
		MyLib::Vector3 rot = pCamera->GetRotation();

		float ratio = 1.0f - (fabsf(rot.z) / static_cast<float>(D3DX_PI));

		m_Diffposition.y = (m_WorldPos.y - m_OldWorldPos.y) * ratio * 200.0f;

		// テキストの描画
		CManager::GetInstance()->GetDebugProc()->Print(
			"【位置】[Y：%f]\n",
			m_WorldPos.y);

		// テキストの描画
		CManager::GetInstance()->GetDebugProc()->Print(
			"【差分】[Y：%f]\n",
			m_Diffposition.y);
	}

}

//==========================================================================
// マウスのプレス情報
//==========================================================================
bool CInputMouse::GetPress(BUTTON nKey)
{
	return (m_MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================================================
// マウスの移動量
//==========================================================================
MyLib::Vector3 CInputMouse::GetMouseMove()
{
	return MyLib::Vector3(
		((float)m_MouseState.lX) * MOUSE_SENS,
		(-(float)m_MouseState.lY) * MOUSE_SENS,
		(-(float)m_MouseState.lZ) * MOUSE_SENS);
}

//==========================================================================
// 位置取得
//==========================================================================
D3DXVECTOR2 CInputMouse::GetPosition()
{
	return D3DXVECTOR2(m_pos.x, m_pos.y);
}

//==========================================================================
// 前回の位置取得
//==========================================================================
MyLib::Vector3 CInputMouse::GetOldWorldPosition()
{
	return m_OldWorldPos;
}

//==========================================================================
// 位置取得
//==========================================================================
MyLib::Vector3 CInputMouse::GetWorldPosition()
{
	return m_WorldPos;
}

//==========================================================================
// レイの始点取得
//==========================================================================
MyLib::Vector3 CInputMouse::GetNearPosition()
{
	return m_NearPos;
}

//==========================================================================
// 差分取得
//==========================================================================
MyLib::Vector3 CInputMouse::GetDiffPosition()
{
	return m_Diffposition;
}

//==========================================================================
// レイ取得
//==========================================================================
MyLib::Vector3 CInputMouse::GetRay()
{
	if (m_pViewMtx == nullptr ||
		m_pPrjMtx == nullptr) {
		return MyLib::Vector3();
	}

	MyLib::Vector3 farpos;
	MyLib::Vector3 ray;

	int Sx = static_cast<int>(m_pos.x), Sy = static_cast<int>(m_pos.y);
	m_NearPos = UtilFunc::Transformation::CalcScreenToWorld(Sx, Sy, 0.0f, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), *m_pViewMtx, *m_pPrjMtx);
	farpos = UtilFunc::Transformation::CalcScreenToWorld(Sx, Sy, 1.0f, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), *m_pViewMtx, *m_pPrjMtx);
	ray = farpos - m_NearPos;
	return ray.Normal();
}

//==========================================================================
// カメラマトリックス設定
//==========================================================================
void CInputMouse::SetCameraMtx(D3DXMATRIX* viewMtx, D3DXMATRIX* prjMtx)
{
	m_pViewMtx = viewMtx;
	m_pPrjMtx = prjMtx;
}