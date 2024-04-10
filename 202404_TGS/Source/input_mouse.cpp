//=============================================================================
//
// ����(�}�E�X)���� [input_mouse.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input_mouse.h"
#include "debugproc.h"
#include "calculation.h"
#include "manager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float MOUSE_SENS = 0.5f;	// �}�E�X���x�̕␳
}
CInputMouse* CInputMouse::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CInputMouse::CInputMouse()
{
	memset(&m_MouseState, 0, sizeof(DIMOUSESTATE2));
	memset(&m_MouseStateTrigger, 0, sizeof(XINPUT_STATE));
	memset(&m_MouseStateRerease, 0, sizeof(XINPUT_STATE));
	memset(&m_MouseStateRepeat, 0, sizeof(XINPUT_STATE));
	memset(&m_aOldState, 0, sizeof(m_aOldState));	// �O��̓��͏��
	m_pViewMtx = nullptr;	// �r���[�}�g���b�N�X
	m_pPrjMtx = nullptr;	// �v���W�F�N�V�����}�g���b�N�X
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CInputMouse::~CInputMouse()
{

}

//==========================================================================
// ��������
//==========================================================================
CInputMouse* CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pThisPtr != nullptr) {
		return m_pThisPtr;
	}

	// �C���X�^���X����
	m_pThisPtr = DEBUG_NEW CInputMouse;
	if (m_pThisPtr != nullptr) {

		// ����������
		m_pThisPtr->Init(hInstance, hWnd);
	}

	return m_pThisPtr;
}

//==========================================================================
// �}�E�X�̏�����
//==========================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ����������
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�̐ݒ�
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�h�Őݒ�(��Βl��DIPROPAXISMODE_ABS)

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����擾
	m_pDevice->Acquire();

	// �}�E�X�J�[�\���̏���
	//ShowCursor(false);

	return S_OK;
}

//==========================================================================
// �}�E�X�̏I������
//==========================================================================
void CInputMouse::Uninit()
{
	// �I������
	CInput::Uninit();

	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// �}�E�X�̍X�V����
//==========================================================================
void CInputMouse::Update()
{
	DIMOUSESTATE2 mouse; // �}�E�X�̓��͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(mouse), &mouse)))
	{
		// ���͏��̕ۑ�
		m_MouseState = mouse;
	}
	else
	{
		// �}�E�X�ւ̃A�N�Z�X�����l��
		m_pDevice->Acquire();
	}

	GetCursorPos(&m_pos);
	ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &m_pos);

	int x = m_pos.x, y = m_pos.y;
	UtilFunc::Transformation::ValueNormalize(x, 1280, 0);
	UtilFunc::Transformation::ValueNormalize(y, 720, 0);
	m_pos.x = x, m_pos.y = y;

		// �e�L�X�g�̕`��
		CManager::GetInstance()->GetDebugProc()->Print(
			"---------------- �}�E�X��� ----------------\n"
			"�y�ʒu�z[X�F%d Y�F%d]\n",
			m_pos.x, m_pos.y);
}

//==========================================================================
// �}�E�X�̃v���X���
//==========================================================================
bool CInputMouse::GetPress(BUTTON nKey)
{
	return (m_MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================================================
// �}�E�X�̈ړ���
//==========================================================================
MyLib::Vector3 CInputMouse::GetMouseMove()
{
	return MyLib::Vector3(
		((float)m_MouseState.lX) * MOUSE_SENS,
		(-(float)m_MouseState.lY) * MOUSE_SENS,
		(-(float)m_MouseState.lZ) * MOUSE_SENS);
}

//==========================================================================
// �ʒu�擾
//==========================================================================
D3DXVECTOR2 CInputMouse::GetPosition()
{
	return D3DXVECTOR2(m_pos.x, m_pos.y);
}

//==========================================================================
// ���C�̎n�_�擾
//==========================================================================
MyLib::Vector3 CInputMouse::GetNearPosition()
{
	return m_NearPos;
}

//==========================================================================
// ���C�擾
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
	ray = ray.Normal();

	return ray.Normal();
}

//==========================================================================
// �J�����}�g���b�N�X�ݒ�
//==========================================================================
void CInputMouse::SetCameraMtx(D3DXMATRIX* viewMtx, D3DXMATRIX* prjMtx)
{
	m_pViewMtx = viewMtx;
	m_pPrjMtx = prjMtx;
}