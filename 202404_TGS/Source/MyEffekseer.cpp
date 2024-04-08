//=============================================================================
// 
//  �G�t�F�N�V�A���� [MyEffekseer.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "MyEffekseer.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "pause.h"
#include <Effekseer.h>

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
std::string CMyEffekseer::m_EffectName[CMyEffekseer::EFKLABEL_MAX] =	// �G�t�F�N�g�̃t�@�C����
{
	"data/Effekseer/Laser01.efkefc",			// �T���v���̃��[�U�[
	"data/Effekseer/CounterParticle_01.efkefc",	// �J�E���^�[�̐�
	"data/Effekseer/counter2.efkefc",			// �J�E���^�[�̐�2
	"data/Effekseer/CounterParticle_02.efkefc",	// �J�E���^�[�̃L���L��
	"data/Effekseer/CounterBreak.efkefc",		// �J�E���^�[�u���C�N
	"data/Effekseer/BackgroundFire.efkefc",		// �w�i�̉�
	"data/Effekseer/HitParticle_Red_01.efkefc",	// �q�b�g�}�[�N[��]
	"data/Effekseer/strongATK.efkefc",			// ���U���̃T�C��
	"data/Effekseer/SonicBoom.efkefc",			// �{�X�̃��[�����O
	"data/Effekseer/stonebase_light.efkefc",	// �Δ̌�
	"data/Effekseer/stonebase_begin.efkefc",	// �ΔN��
	"data/Effekseer/transfer.efkefc",			// �ΔN��
	"data/Effekseer/chargeatk_01.efkefc",		// �`���[�W����
	"data/Effekseer/powerloop.efkefc",			// �`���[�W����
	"data/Effekseer/titlelight.efkefc",			// �^�C�g���̌�
	"data/Effekseer/enemyatk_sand.efkefc",		// �G�U���̉�
	"data/Effekseer/normalATK_left.efkefc",		// �ʏ�U�����U��
	"data/Effekseer/normalATK_right.efkefc",	// �ʏ�U���E�U��
	"data/Effekseer/normalATK.efkefc",			// �ʏ�U��
	"data/Effekseer/chargeatk.efkefc",			// �`���[�W�U��
	"data/Effekseer/respawn_start.efkefc",		// �����J�n
	"data/Effekseer/respawn_wind.efkefc",		// �����J�n
	"data/Effekseer/boost2.efkefc",		// �����J�n
};
CMyEffekseer* CMyEffekseer::m_pMyEffekseer = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMyEffekseer::CMyEffekseer()
{
	// �ϐ��̃N���A
	time = 0;
	efkHandle = 0;
	m_Handle.clear();		// �G�t�F�N�g�̃n���h��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMyEffekseer::~CMyEffekseer()
{

}

//==========================================================================
// ��������
//==========================================================================
CMyEffekseer* CMyEffekseer::Create()
{
	if (m_pMyEffekseer == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		m_pMyEffekseer = DEBUG_NEW CMyEffekseer;

		// ����������
		m_pMyEffekseer->Init();
	}
	else
	{
		// �C���X�^���X�擾
		m_pMyEffekseer->GetInstance();
	}

	return m_pMyEffekseer;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMyEffekseer::Init()
{
	time = 0;
	efkHandle = 0;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Effekseer�̃I�u�W�F�N�g�̓X�}�[�g�|�C���^�ŊǗ������B�ϐ����Ȃ��Ȃ�Ǝ����I�ɍ폜�����B

	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	efkManager = ::Effekseer::Manager::Create(8000);

	// Effekseer�̃��W���[�����Z�b�g�A�b�v����
	SetupEffekseerModules(efkManager);
	auto efkRenderer = GetEffekseerRenderer();

	// ���W�n��ݒ肷��B�A�v���P�[�V�����ƈ�v������K�v������B
	efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// ���_�ʒu���m��
	viewerPosition = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// ���e�s���ݒ�
	projectionMatrix.PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// �J�����s���ݒ�
	cameraMatrix.LookAtRH(viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// �G�t�F�N�g�̐ݒ�
//==========================================================================
Effekseer::Handle CMyEffekseer::SetEffect(EFKLABEL label, MyLib::Vector3 pos, MyLib::Vector3 rot, MyLib::Vector3 move, float scale, bool bAutoDeath)
{
	MyEffekseerInfo loacalInfo = {};

	// �ǂݍ���
	loacalInfo.efcRef = LoadEffect(m_EffectName[label]);

	// �G�t�F�N�g�̍Đ�
	loacalInfo.handle = efkManager->Play(m_LoadEffect, 0.0f, 0.0f, 0.0f);

	// �������ݒ�
	loacalInfo.pos = pos;
	loacalInfo.rot = rot;
	loacalInfo.move = move;
	loacalInfo.scale = scale;
	loacalInfo.bAutoDeath = bAutoDeath;
	efkManager->SetLocation(loacalInfo.handle, pos.x, pos.y, pos.z);
	efkManager->SetRotation(loacalInfo.handle, rot.x, rot.y, rot.z);
	efkManager->SetScale(loacalInfo.handle, scale, scale, scale);

	// �v�f�ǉ�
	m_EffectObj.push_back(loacalInfo);
	m_Handle.push_back(loacalInfo.handle);		// �G�t�F�N�g�̃n���h��

	return loacalInfo.handle;
}

//==========================================================================
// �G�t�F�N�g�̐ݒ�
//==========================================================================
Effekseer::Handle CMyEffekseer::SetEffect(Effekseer::Handle** pHandle, EFKLABEL label, MyLib::Vector3 pos, MyLib::Vector3 rot, MyLib::Vector3 move, float scale, bool bAutoDeath)
{
	MyEffekseerInfo loacalInfo = {};

	// �ǂݍ���
	loacalInfo.efcRef = LoadEffect(m_EffectName[label]);

	// �G�t�F�N�g�̍Đ�
	loacalInfo.handle = efkManager->Play(m_LoadEffect, 0.0f, 0.0f, 0.0f);

	// �������ݒ�
	loacalInfo.pos = pos;
	loacalInfo.rot = rot;
	loacalInfo.move = move;
	loacalInfo.scale = scale;
	loacalInfo.bAutoDeath = bAutoDeath;
	efkManager->SetLocation(loacalInfo.handle, pos.x, pos.y, pos.z);
	efkManager->SetRotation(loacalInfo.handle, rot.x, rot.y, rot.z);
	efkManager->SetScale(loacalInfo.handle, scale, scale, scale);

	// �v�f�ǉ�
	m_EffectObj.push_back(loacalInfo);
	m_Handle.push_back(loacalInfo.handle);		// �G�t�F�N�g�̃n���h��

	int idx = m_Handle.size() - 1;
	*pHandle = &m_Handle[idx];	// �|�C���^�ɍŌ�̏���n��

	return loacalInfo.handle;
}

//==========================================================================
// �ǂݍ��ݏ���
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadEffect(std::string efkpath)
{
	// char16_t�ɕϊ�
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(efkpath);

	// �G�t�F�N�g�̓Ǎ�
	auto effect = Effekseer::Effect::Create(efkManager, string16t.c_str());
	m_LoadEffect = effect;

	onLostDevice = [effect]() -> void
	{
		// �ǂݍ��񂾃G�t�F�N�g�̃��\�[�X�͑S�Ĕj������B
		if (effect != nullptr)
		{
			effect->UnloadResources();
		}
	};

	onResetDevice = [effect]() -> void
	{
		// �G�t�F�N�g�̃��\�[�X���ēǂݍ��݂���B
		if (effect != nullptr)
		{
			effect->ReloadResources();
		}
	};

	return effect;
}

//==========================================================================
// �I������
//==========================================================================
void CMyEffekseer::Uninit()
{
	efkManager->StopAllEffects();

	delete m_pMyEffekseer;
	m_pMyEffekseer = nullptr;
}

//==========================================================================
// ��~
//==========================================================================
void CMyEffekseer::Stop(Effekseer::Handle handle)
{
	for (int i = 0; i < static_cast<int>(m_EffectObj.size()); i++)
	{
		Effekseer::Handle loacalhandle = m_EffectObj[i].handle;
		if (loacalhandle == handle)
		{
			// �폜
			m_EffectObj.erase(m_EffectObj.begin() + i);
			m_Handle.erase(m_Handle.begin() + i);
		}
	}

	// ��~
	efkManager->StopEffect(handle);
}

//==========================================================================
// �S�Ē�~
//==========================================================================
void CMyEffekseer::StopAll()
{
	// �S�Ē�~
	efkManager->StopAllEffects();

	for (int i = 0; i < static_cast<int>(m_EffectObj.size()); i++)
	{
		Effekseer::Handle loacalhandle = m_EffectObj[i].handle;
		efkManager->StopEffect(loacalhandle);
	}

	int size = static_cast<int>(m_EffectObj.size());
	for (int i = 0; i < size; i++)
	{
		m_EffectObj[i].bAutoDeath = true;
	}
	m_EffectObj.clear();
	m_Handle.clear();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMyEffekseer::Update()
{
	if (!CManager::GetInstance()->GetPause()->IsPause())
	{

		// �L�[�{�[�h���擾
		CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
		float size = 10.0f;

		for (int i = 0; i < static_cast<int>(m_EffectObj.size()); i++)
		{
			Effekseer::Handle loacalhandle = m_EffectObj[i].handle;

			if (!efkManager->Exists(loacalhandle))
			{// �Đ��I��

				efkManager->StopEffect(loacalhandle);

				// �폜
				if (m_EffectObj[i].bAutoDeath)
				{
					m_EffectObj.erase(m_EffectObj.begin() + i);
					m_Handle.erase(m_Handle.begin() + i);
				}
				else
				{

					// �V���ɃG�t�F�N�g���Đ��������B���W�̓G�t�F�N�g��\���������ꏊ��ݒ肷��
					// (�ʒu���]�A�g��k�����ݒ肵�Ȃ����K�v������)
					m_Handle[i] = efkManager->Play(m_EffectObj[i].efcRef, 0.0f, 0.0f, 0.0f);
					m_EffectObj[i].handle = m_Handle[i];
					efkManager->SetLocation(m_EffectObj[i].handle, m_EffectObj[i].pos.x, m_EffectObj[i].pos.y, m_EffectObj[i].pos.z);
					efkManager->SetRotation(m_EffectObj[i].handle, m_EffectObj[i].rot.x, m_EffectObj[i].rot.y, m_EffectObj[i].rot.z);
					efkManager->SetScale(m_EffectObj[i].handle, m_EffectObj[i].scale, m_EffectObj[i].scale, m_EffectObj[i].scale);
				}
			}
			else
			{// �Đ���

				if (!m_EffectObj[i].move.IsNearlyZero(0.01f))
				{
					// �G�t�F�N�g�̈ړ�
					efkManager->AddLocation(
						loacalhandle,
						Effekseer::Vector3D(m_EffectObj[i].move.x, m_EffectObj[i].move.y, m_EffectObj[i].move.z));
				}


				Effekseer::Vector3D scale;

				efkManager->GetMatrix(m_EffectObj[i].handle).GetScale(scale);

				efkManager->SetScale(m_EffectObj[i].handle, m_EffectObj[i].scale, m_EffectObj[i].scale, m_EffectObj[i].scale);
			}
		}

		// ���C���[�p�����[�^�̐ݒ�
		Effekseer::Manager::LayerParameter layerParameter;
		::Effekseer::Matrix44 invEfkCameraMatrix;
		::Effekseer::Matrix44::Inverse(invEfkCameraMatrix, cameraMatrix);
		layerParameter.ViewerPosition = ::Effekseer::Vector3D(invEfkCameraMatrix.Values[3][0], invEfkCameraMatrix.Values[3][1], invEfkCameraMatrix.Values[3][2]);
		efkManager->SetLayerParameter(0, layerParameter);

		// �}�l�[�W���[�̍X�V
		Effekseer::Manager::UpdateParameter updateParameter;
		efkManager->Update(updateParameter);
	}

	// �`�揈��
	Draw();

	if (!CManager::GetInstance()->GetPause()->IsPause())
	{
		time++;
	}
}

//==========================================================================
// �g���K�[���M
//==========================================================================
void CMyEffekseer::SetTrigger(Effekseer::Handle handle, int idx)
{
	if (!efkManager->Exists(handle))
	{// �Đ��I��
		return;
	}

	efkManager->SendTrigger(handle, idx);
}

//==========================================================================
// �ʒu�X�V
//==========================================================================
void CMyEffekseer::SetPosition(Effekseer::Handle handle, MyLib::Vector3 pos)
{
	if (!efkManager->Exists(handle))
	{// �Đ��I��
		return;
	}

	// �C���f�b�N�X����
	std::vector<Effekseer::Handle>::iterator itr = std::find(m_Handle.begin(), m_Handle.end(), handle);
	int idx = std::distance(m_Handle.begin(), itr);

	// �ʒu���ݒ�
	m_EffectObj[idx].pos = pos;

	efkManager->SetLocation(handle, Effekseer::Vector3D(pos.x, pos.y, pos.z));
}

//==========================================================================
// �����X�V
//==========================================================================
void CMyEffekseer::SetRotation(Effekseer::Handle handle, MyLib::Vector3 rot)
{
	if (!efkManager->Exists(handle))
	{// �Đ��I��
		return;
	}

	efkManager->SetRotation(handle, rot.x, rot.y, rot.z);
}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CMyEffekseer::SetMatrix(Effekseer::Handle handle, D3DXMATRIX mtx)
{
	if (!efkManager->Exists(handle))
	{// �Đ��I��
		return;
	}

	// �O�Ղ̃}�g���b�N�X�擾
	Effekseer::Matrix43 efcmtx;
	efcmtx = CMyEffekseer::GetInstance()->GetMatrix(handle);

	// 4x3�s��Ɍ�����ݒ�
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			efcmtx.Value[i][j] = mtx.m[i][j];
		}
	}

	// �G�t�F�N�g�̃C���X�^���X�ɕϊ��s���ݒ�
	efkManager->SetMatrix(handle, efcmtx);

	// �C���f�b�N�X����
	std::vector<Effekseer::Handle>::iterator itr = std::find(m_Handle.begin(), m_Handle.end(), handle);
	int idx = std::distance(m_Handle.begin(), itr);

	// �X�P�[�����ݒ�
	float scale = m_EffectObj[idx].scale;
	efkManager->SetScale(handle, scale, scale, scale);
}

//==========================================================================
// �}�g���b�N�X�擾
//==========================================================================
Effekseer::Matrix43 CMyEffekseer::GetMatrix(Effekseer::Handle handle)
{
	return efkManager->GetMatrix(handle);
}

//==========================================================================
// �X�P�[���ݒ�
//==========================================================================
void CMyEffekseer::SetScale(Effekseer::Handle handle, float scale)
{
	efkManager->SetScale(handle, scale, scale, scale);
}

//==========================================================================
// �}�g���b�N�X�ݒ�
//==========================================================================
void CMyEffekseer::SetTransform(Effekseer::Handle handle, MyLib::Vector3 pos, MyLib::Vector3 rot)
{
	Effekseer::Matrix43 a;
	a.Translation(pos.x, pos.y, pos.z);
	a.RotationZXY(rot.x, rot.y, rot.z);

	Effekseer::Matrix43 Weapon = efkManager->GetMatrix(handle);
	//Weapon.Indentity();

	Weapon.Multiple(Weapon, Weapon, a);

	efkManager->SetMatrix(handle, Weapon);
}

//==========================================================================
// �I���t���O�擾
//==========================================================================
bool CMyEffekseer::IsDeath(Effekseer::Handle handle)
{
	return !efkManager->Exists(handle);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMyEffekseer::Draw()
{
	// ���Ԃ��X�V����
	efkRenderer->SetTime(time / 60.0f);

	// ���e�s���ݒ�
	efkRenderer->SetProjectionMatrix(projectionMatrix);

	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	D3DXMATRIX appProjectionMatrix = pCamera->GetMtxProjection();

	// Effekseer��CameraMatrix�́A�J�����̎p���s��̋t�s����w���B�J�����s�񂪃J�����̎p���s��ł���ꍇ�́A�t�s�񉻂��Ă����B
	auto invAppCameraMatrix = pCamera->GetMtxView();

	// �����̊���Effekseer�́A���e(�ˉe)�s��ƃJ�����s��𓯊�
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			projectionMatrix.Values[i][j] = appProjectionMatrix.m[i][j];
			cameraMatrix.Values[i][j] = invAppCameraMatrix.m[i][j];
		}
	}

	// �J�����s���ݒ�
	efkRenderer->SetCameraMatrix(cameraMatrix);

	// �G�t�F�N�g�̕`��J�n�������s���B
	efkRenderer->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = efkRenderer->GetCameraProjectionMatrix();
	efkManager->Draw(drawParameter);

	// �G�t�F�N�g�̕`��I������
	efkRenderer->EndRendering();
}

//==========================================================================
// ���W���[���̃Z�b�g�A�b�v
//==========================================================================
void CMyEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �`��f�o�C�X�̍쐬
	auto graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(pDevice);

	// �G�t�F�N�g�̃����_���[�̍쐬
	efkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// �`�惂�W���[���̐ݒ�
	efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(efkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(efkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}