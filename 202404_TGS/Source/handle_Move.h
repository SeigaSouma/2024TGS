//=============================================================================
// 
// �ړ��n���h������ [handle_Move.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _HANDLE_MOVE_H_
#define _HANDLE_MOVE_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "objectX.h"
class CObjectLine;

//==========================================================================
// �N���X��`
//==========================================================================
// �ړ��n���h���N���X
class CHandle_Move : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum HandleAngle
	{
		ANGLE_Z = 0,
		ANGLE_Y,
		ANGLE_X,
		ANGLE_MAX
	};

	CHandle_Move(int nPriority = 4);
	~CHandle_Move();

	//  �I�[�o�[���C�h�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�

	float GetScale() { return m_scale; }
	void Kill();

	static CHandle_Move* Create(const MyLib::Vector3& pos);
	static CListManager<CObjectX> GetList() { return m_List; }
private:

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::AABB m_AABB;
	CObjectX* m_pArrow[ANGLE_MAX];		// ���C���̃I�u�W�F�N�g
	CObjectLine* m_pLine[ANGLE_MAX];	// ���C���̃I�u�W�F�N�g
	float m_scale;
	static CListManager<CObjectX> m_List;	// �ړ��n���h�����X�g
};
#endif