////============================================================================
//// 
//// �Q�O�Q�T�N�S���Q�W���F�R���W�����n�N���X [bounding_box.h]
//// Author : �Ŗ����z
//// 
////============================================================================
//
//#ifndef _BOUNDING_BOX_H_
//#define _BOUNDING_BOX_H_	// ��d�C���N���[�h�h�~
//
////****************************************************
//// �C���N���[�h�t�@�C��
////****************************************************
//#include "collision.h" //CollisionStrategy���N���X������̂�
//
////****************************************************
//// �{�b�N�X���E�����蔻��N���X
////****************************************************
//class CCollisionInfo_BoundingBox : public CCollisionInfo
//{
//public:
//	CCollisionInfo_BoundingBox();                       // �R���X�g���N�^
//	~CCollisionInfo_BoundingBox() override;             // �f�X�g���N�^
//
//	//< Setter>
//	void SetUpdateInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld,D3DXVECTOR3 Move,D3DXVECTOR3 Size,D3DXVECTOR3 VtxMax,D3DXVECTOR3 VtxMin,
//		bool bIsLanding,bool bIsLandingOld,bool bPushX,bool bPushXOld);// ���X�V�p�i����O�ɑS�Ă̏����蓮�ōX�V�j
//    //< Setter/Getter>
//	void SetSize(const D3DXVECTOR3 Size);                   // �T�C�Y�ݒ�
//	const D3DXVECTOR3& GetSize() const;                     // �T�C�Y�擾
//
//	//<setter/getter>
//	void SetVtxMax(D3DXVECTOR3 VtxMax);                     // �ő咸�_�ݒ�
//	const D3DXVECTOR3& GetVtxMax() const;                   // �ő咸�_�擾
//
//	//<setter/getter>
//	void SetMove(D3DXVECTOR3 Move);                         // �ړ��ʐݒ�
//	const D3DXVECTOR3& GetMove() const;                     // �ړ��ʎ擾
//
//	//<setter/getter>
//	void SetVtxMin(D3DXVECTOR3 VtxMin);                     // �ŏ����_�ݒ�
//	const D3DXVECTOR3& GetVtxMin() const;                   // �ŏ����_�擾
//
//    //<setter/getter>
//	void SetPushX(bool bPush);                              // X���ɉ����o����Ă������ǂ����ݒ�
//	const bool& GetPushX() const;                           // X���ɉ����o����Ă������ǂ����擾
//
//	//<setter/getter>
//	void SetPushXOld(bool bPush);                           // 1f�O��X���ɉ����o����Ă������ǂ����ݒ�
//	const bool& GetPushXOld() const;                        // 1f�O��X���ɉ����o����Ă������ǂ����擾
//
//private:
//	D3DXVECTOR3 m_Size;   // �T�C�Y
//	D3DXVECTOR3 m_Move;   // �ړ���
//	D3DXVECTOR3 m_VtxMax; // �ő咸�_
//	D3DXVECTOR3 m_VtxMin; // �ŏ����_
//	bool m_bPushX;        // X���ɉ����o����Ă������ǂ���
//	bool m_bPushXOld;     // 1f�O��X���ɓ������Ă������ǂ���
//};
//#endif // _COLLISION_H_