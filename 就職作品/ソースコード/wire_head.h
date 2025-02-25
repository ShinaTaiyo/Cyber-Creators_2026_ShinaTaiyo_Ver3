//===============================================================================
//
//�P�P���Q�P���F���C���[�����[wire_head.h]
//Author:ShinaTaiyo
//
//===============================================================================

#ifndef _WIREHEAD_H_
#define _WIREHEAD_H_

//==========================================
//�C���N���[�h
//==========================================
#include "main.h"
#include "objectX.h"
//===============================================================================

class CWireHead : public CObjectX
{
public:
	CWireHead(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::WIREHEAD, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);//�R���X�g���N�^
	~CWireHead();                                                                                           //�f�X�g���N�^
	HRESULT Init() override;                                                                                //����������
	void Uninit() override;                                                                                 //�I������
	void Update() override;                                                                                 //�X�V����
	void Draw() override;                                                                                   //�`�揈��
	void SetDeath() override;                                                                               //���S�t���O�ݒ菈��
	static CWireHead* Create(D3DXVECTOR3 Pos,D3DXVECTOR3 Rot,D3DXVECTOR3 Move,D3DXVECTOR3 Scale,int nLife); //��������
	const bool& GetSuccessCollision() const { return m_bCollision; }                                        //�����蔻�肪�����������ǂ���
	const CObject::TYPE& GetCollisionObjType() const { return m_CollisionObjType; }                         //�Փ˂����I�u�W�F�N�g�̎�ނ��擾
	void ResetCoolTime() { m_nCoolTime = s_nMAX_COOLTIME; m_bCollision = false; }                           //�����蔻����s���܂ł̃N�[���^�C�������Z�b�g����
private:
	static constexpr int s_nMAX_COOLTIME = 1;                                                               //�����蔻����s���N�[���^�C���ő�l

	bool CollisionSquare();                                                                                 //�����`�̓����蔻����s��
	bool m_bCollision;                                                                                      //�����蔻�肪�����������ǂ���
	int m_nCoolTime;                                                                                        //�N�[���^�C����ݒ�
	CObject::TYPE m_CollisionObjType;                                                                       //�Փ˂����I�u�W�F�N�g�̃^�C�v���i�[
};
#endif // !_WIRE_H_