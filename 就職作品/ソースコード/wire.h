//===============================================================================
//
//�P�P���P�V���F���C���[�����[wire.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _WIRE_H_
#define _WIRE_H_

//==========================================
//�C���N���[�h
//==========================================
#include "main.h"
#include "meshcylinder.h"
#include "wire_head.h"
//===============================================================================

//==========================================
//�O���錾
//==========================================
class CPlayer;

class CWire : public CMeshCylinder
{
public:
	enum class WIRETYPE
	{//���C���[�̎��
		NORMAL = 0,
		ROPE,
		MAX
	};

	CWire(WIRETYPE WireType,float fRadius,float fHeight,int nNumDivsionXZ,D3DXVECTOR3 Pos,D3DXVECTOR3 Rot,int nNumDivisionY,int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::WIRE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_3D);//�R���X�g���N�^
	~CWire() override;//�f�X�g���N�^
	HRESULT Init() override; //����������
	void Uninit() override;  //�I������
	void Update() override;  //�X�V����
	void Draw() override;    //�`�揈��
	void SetDeath() override;//���S�t���O�ݒ菈��
	static CWire* Create(WIRETYPE Type,D3DXVECTOR3 Pos, D3DXVECTOR3 Rot,float fRadius, float fHeight,int nNumDivisionXZ,int nNumDivisionY);

	//���C���[�̓��擾
	CWireHead* GetWireHead() { return m_pWireHead; }

	//�v���C���[�̃|�C���^�ݒ�
	void SetPlayerPointer(CPlayer* pPlayer) {m_pPlayer = pPlayer; }

	//�X�V���邩�ǂ���
	void SetUseUpdate(bool bUse) { m_bUseUpdate = bUse; }
protected:
	static const string s_WIRE_FILENAME[static_cast<int>(WIRETYPE::MAX)];
private:
	WIRETYPE m_Type;//���C���[�̎��
	CPlayer* m_pPlayer;//�v���C���[�̃|�C���^�̃C���X�^���X
	CWireHead* m_pWireHead;//���C���[�̓�
	bool m_bUseUpdate;//�X�V���邩�ǂ���
};
#endif // !_WIRE_H_