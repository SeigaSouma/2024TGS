//=============================================================================
//
// 入力(マウス)処理 [input_mouse.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _INPUT_MOUSE_H_	//このマクロ定義がされていなかったら
#define _INPUT_MOUSE_H_	//二重インクルード防止のマクロを定義する

#include "input.h"

//==========================================================================
// クラス定義
//==========================================================================
// マウスクラス
class CInputMouse : public CInput
{
public:

	// 列挙型定義
	enum BUTTON
	{
		BUTTON_LEFT = 0,
		BUTTON_RIGHT,
		BUTTON_WHEEL,
		BUTTON_SIDE_1,
		BUTTON_SIDE_2,
		BUTTON_MAX
	};

	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(BUTTON nKey);
	MyLib::Vector3 GetMouseMove();
	D3DXVECTOR2 GetPosition();	// 位置取得

	static CInputMouse* GetInstance() { return m_pThisPtr; }	// インスタンス取得
	static CInputMouse* Create(HINSTANCE hInstance, HWND hWnd);	// 生成処理

private:

	//=============================
	// メンバ変数
	//=============================
	DIMOUSESTATE2 m_MouseState;			// 全入力情報の保管
	DIMOUSESTATE2 m_MouseStateTrigger;	// トリガー情報
	DIMOUSESTATE2 m_MouseStateRerease;	// リリース情報
	DIMOUSESTATE2 m_MouseStateRepeat;	// リピート情報
	BYTE m_aOldState[BUTTON::BUTTON_MAX];	// 前回の入力情報を保存
	POINT m_pos;

	static CInputMouse* m_pThisPtr;	// 自身のポインタ
};

#endif