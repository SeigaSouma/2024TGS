//=============================================================================
// 
//  �����L���O�w�i�w�b�_�[ [rankingbg.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _RANKING_BG_H_
#define _RANKING_BG_H_	// ��d�C���N���[�h�h�~

#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �̗̓Q�[�W�N���X��`
class CRankingBG : public CObject2D
{
public:

	CRankingBG(int nPriority = 0);
	~CRankingBG();

	static CRankingBG*Create(float fSpeed);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
	enum MOVE
	{
		MOVE_LEFT = 0,
		MOVE_RIGHT,
		MOVE_MAX
	};

	float m_fSpeed;
	float m_fMoveOffset;
	MOVE m_moveDir;
};


#endif