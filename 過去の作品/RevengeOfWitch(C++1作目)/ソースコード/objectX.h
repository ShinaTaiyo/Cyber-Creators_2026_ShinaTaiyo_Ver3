//=======================================================================================
//
//�U���S���F���f����`�悷��[objectX.h]
//Author:ShinaTaiyo
//
//=======================================================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//==============================================
//�C���N���[�h
//==============================================
#include "main.h"
#include "object.h"
//=======================================================================================

//=======================================================
//�I�u�W�F�N�gX�N���X
//=======================================================
class CObjectX : public CObject
{
public:
	static const int m_nMAX_MAT = 100;                                                      //�}�e���A���̍ő吔
	typedef enum
	{
		OBJECTXTYPE_BLOCK = 0,        //�u���b�N
		OBJECTXTYPE_MODEL,            //���f��
		OBJECTXTYPE_PLAYER,           //�v���C���[
		OBJECTXTYPE_ENEMY,            //�G
		OBJECTXTYPE_ITEM,             //�A�C�e��
		OBJECTXTYPE_MARKER,           //�}�[�J�[
		OBJECTXTYPE_ATTACK,           //�U��
		OBJECTXTYPE_BOSS,             //�{�X
		OBJECTXTYPE_TRAP,             //�g���b�v
		OBJECTXTYPE_SIGNBOARD,        //���ĊŔ�
		OBJECTXTYPE_AIMODEL,          //AI���f��
		OBJECTXTYPE_MAX
	}OBJECTXTYPE;

	typedef enum
	{
		ORIGINPOSTYPE_SENTER = 0,
		ORIGINPOSTYPE_DOWN,
	    ORIGINPOSTYPE_MAX
	}ORIGINPOSTYPE;

	typedef enum
	{
		ROTTYPE_NORMAL = 0,    //���ʂ̌���
		ROTTYPE_Z,             //Z�����ɉ�]������
		ROTTYPE_MAX          
	}ROTTYPE;

	typedef enum
	{
		MOVETYPE_NONE = 0,     //�ړ��^�C�v�F����
		MOVETYPE_TRUE,         //�ړ��^�C�v�F����
		MOVETYPE_MAX
	}MOVETYPE;

	typedef enum
	{
		COLORTYPE_RED = 0,
		COLORTYPE_GREEN,
		COLORTYPE_BRUE,
		COLORTYPE_ALPHA,
		COLORTYPE_MAX
	}COLORTYPE;

	//=================================
    //���f�����\����
    //=================================
	typedef struct
	{
		LPD3DXMESH pMesh;                           //���b�V���i���_���j�ւ̃|�C���^�i���f���̎�ސ����j
		LPD3DXBUFFER pBuffMat;                      //�}�e���A���ւ̃|�C���^
		DWORD dwNumMat;                             //�}�e���A���̐�
		LPDIRECT3DTEXTURE9* pTexture;               //�e�N�X�`���ւ̃|�C���^
		D3DCOLORVALUE* Diffuse;          //�F����
		D3DCOLORVALUE* FormarDiffuse;    //���̐F����
	}ObjectXInfo;
	//============================================================================================================

	CObjectX(int nPriority = 3);                                                           //�R���X�g���N�^
	~CObjectX() override;                                                 //�f�X�g���N�^
	HRESULT Init() override;                                              //����������
	void Uninit() override;                                               //�I������
	void Update() override;                                               //�X�V����
	void Draw() override;                                                 //�`�揈��
	void BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture,D3DCOLORVALUE * pDiffuse);//���f���������蓖�Ă�

	void SetColor(COLORTYPE type,int nColChengeTime);                                       //�F������ݒ肷��
	void SetColorAlphaOnly(float fAlpha);                                                   //�F�̓����x������ݒ肷��
	void SetOriginalColor(D3DXCOLOR col, int nColChengeTime);                               //�I���W�i���̐F������ݒ肷��
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }                                           //�ʒu�̐ݒ�
	void SetPosOld(D3DXVECTOR3 PosOld) { m_PosOld = PosOld; }                               //1f�O�̈ʒu�̐ݒ�
	void SetSupportPos(D3DXVECTOR3 SupportPos) { m_SupportPos = SupportPos; }               //�ݒu�ʒu��ۑ�����
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }                                           //�����̐ݒ�
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }                                   //�g�嗦�̐ݒ�
	void SetFormarScale(D3DXVECTOR3 Scale) { m_FormarScale = Scale; }                       //���̊g�嗦��ݒ�
	D3DXVECTOR3& GetFormarScale() { return m_FormarScale; }                                 //���̊g�嗦���擾
	D3DXVECTOR3 &GetPos() { return m_Pos; }                                                 //�ʒu�̎擾
	D3DXVECTOR3* GetPosAddress() { return &m_Pos; }                                         //�ʒu�̃A�h���X���擾
	D3DXVECTOR3 &GetPosOld() { return m_PosOld;}                                            //1f�O�̈ʒu��ݒ�
	D3DXVECTOR3 & GetSupportPos() { return m_SupportPos; }                                  //�ݒu�ʒu���擾����
	D3DXVECTOR3 &GetRot() { return m_Rot; }                                                 //�����̎擾
	D3DXVECTOR3 & GetScale() { return m_Scale; }                                            //�g�嗦�̎擾
	D3DXVECTOR3 & GetVtxMax() { return m_VtxMax; }                                          //�e���_�ő�l�̎擾
	D3DXVECTOR3 & GetVtxMin() { return m_VtxMin; }                                          //�e���_�ŏ��l�̎擾
	D3DXVECTOR3 & GetOriginVtxMax() { return m_OriginVtxMax; }                              //���̊e���_�ő�l�̎擾
	D3DXVECTOR3 & GetOriginVtxMin() { return m_OriginVtxMin; }                              //���̊e���_�ŏ��l�̎擾
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }                                       //�ړ��ʂ�ݒ肷��
	D3DXVECTOR3& GetMove() { return m_Move; }                                               //�ړ��ʂ��擾����
	D3DXVECTOR3 GetSize() { return m_Size; }                                                //X�I�u�W�F�N�g�̃T�C�Y���擾
	void SetMoveType(MOVETYPE movetype) { m_MoveType = movetype; }                          //�ړ��^�C�v��ݒ肷��
	MOVETYPE GetMoveType() { return m_MoveType; }                                           //�ړ��^�C�v���擾����
	void SetSize();                                                                         //�T�C�Y��ݒ肷�� 
	void SetSizeAnother(D3DXVECTOR3 Size) { m_Size = Size;}                                 //���l�łɃT�C�Y��ݒ肷��
	int GetManagerType() { return m_nManagerType; }//�}�l�[�W���[�ŌĂяo�����ۂ̃^�C�v�ԍ�
	void SetManagerType(int nManagerType) { m_nManagerType = nManagerType; }//�}�l�[�W���[�ŌĂяo�����ۂ̃^�C�v�ԍ���ݒ肷��
	CObjectX::ObjectXInfo & GetObjectXInfo(){return m_ObjectXInfo;}
	//============================================
	//�G�f�B�^����p
	//============================================
	void SetObjXType(OBJECTXTYPE ObjXType) { m_nObjXType = ObjXType; }                      //�I�u�W�F�N�gX�̕��ނ�ݒ�
	OBJECTXTYPE GetObjXType() { return m_nObjXType; }                                       //�I�u�W�F�N�gX�̃^�C�v���擾����

	void SetTypeNum(int nTypeNum) { m_nTypeNum = nTypeNum; }                                //�I�u�W�F�N�g���Ƃ̃^�C�v�ԍ���ݒ�
	int GetTypeNum() { return m_nTypeNum;}                                                  //�I�u�W�F�N�g���Ƃ̃^�C�v�ԍ���ݒ�

	void SetRotType(ROTTYPE RecvRotType);                                                   //�����̎��        
	ROTTYPE& GetRotType() { return m_RotType; }                                             //�����̎�ނ��擾

	virtual void DispInfo();                        //���\������
	//================================================================================================================================================

	D3DXMATRIX &GetMatrixWorld() { return m_mtxWorld; }

	//============================================
	//�����p
	//============================================
	void SetUseInteria(bool bUseInteria) { m_bUseInteria = bUseInteria;}    //�����������邩�ǂ���
	float& GetInertia() { return m_fInertia; }                                              //�������擾����
	void SetInertia(float fInertia) { m_fInertia = fInertia; }                              //������ݒ肷��
	void SetUndoInertia() { m_fInertia = m_fNORMAL_INERTIA; }                               //�����̑傫�������ɖ߂�
	bool& GetUseInteria() { return m_bUseInteria; }                                         //�������|���邩�ǂ������擾
	//================================================================================================================================================

	void SetLanding(bool bLanding) { m_bIsLanding = bLanding; }                                                    //�n�ʂɂ��邩�ǂ�����ݒ�
	bool& GetLanding() { return m_bIsLanding; }                                                                    //�n�ʂɂ��邩�ǂ������擾

	void SetWalling(bool bWalling) { m_bIsWalling = bWalling; }                             //�ǂɂ������Ă��邩�ǂ�����ݒ�
	bool& GetWalling() { return m_bIsWalling; }                                             //�ǂɂ������Ă��邩�ǂ������擾

	void SetJumping(bool bJumping) { m_bIsJumping = bJumping; }                             //�W�����v���Ă��邩�ǂ�����ݒ�
	bool& GetJumping() { return m_bIsJumping; }                                             //�W�����v���Ă��邩�ǂ������擾

	void SetLiftLanding(bool bLiftLanding) { m_bIsLiftLanding = bLiftLanding; }             //���t�g�ɏ���Ă��邩�ǂ���
	bool& GetLiftLanding() { return m_bIsLiftLanding; }                                     //���t�g�ɏ���Ă��邩�ǂ������擾
	CObject*& GetLandingObj() { return m_pLiftLandingObj; }                                  
	void SetLandingObj(CObject* pObj) { m_pLiftLandingObj = pObj; }                         //����Ă���I�u�W�F�N�g��ݒ�

	//=============================
	//�y�A�����g�֌W
	//=============================
	void SetParentInfo(D3DXVECTOR3 ParentPos, D3DXVECTOR3 ParentRot, D3DXVECTOR3 ParentScale, bool bUse)//�I�t�Z�b�g����ݒ�
	{
		m_ParentSetPos = ParentPos; m_ParentSetRot = ParentRot; m_ParentSetScale = ParentScale; m_bUseParent = bUse;
	}
	//void SetUseParent(D3DXMATRIX * mtxWorld){ m_ParentMtxWorld = mtxWorld;}
	D3DXVECTOR3& GetParentPos() { return m_ParentSetPos; }         //�e�̈ʒu���擾
	D3DXVECTOR3& GetParentRot() { return m_ParentSetRot; }         //�e�̌������擾
	D3DXVECTOR3& GetParentScale() { return m_ParentSetScale; }     //�e�̊g�嗦���擾
	bool GetUseParent() { return m_bUseParent; }                   //�e���g�p���邩�ǂ���

	D3DXVECTOR3& GetOffSetPos() { return m_OffSetPos; }            //�I�t�Z�b�g�ʒu���擾
	D3DXVECTOR3& GetOffSetRot() { return m_OffSetRot; }            //�I�t�Z�b�g�������擾
	D3DXVECTOR3& GetOffSetScale() { return m_OffSetScale; }        //�I�t�Z�b�g�g�嗦���擾


	//=================================================================================================================

	//======================================================
	//�������Z�֌W
	//======================================================
	void SetUseAddRot(bool bUse, D3DXVECTOR3 Rot) { m_bUseAddRot = bUse; m_AddRot = Rot;}
	bool& GetUseAddRot() { return m_bUseAddRot; }//�����̉��Z���g�p���Ă��邩�ǂ���
	D3DXVECTOR3& GetAddRot() { return m_AddRot; }//�����̉��Z�ʂ��擾
	//======================================================
	//�̗͊֌W
	//======================================================
	void SetLife(int nLife) { m_nLife = nLife; }                    //�̗͂�ݒ肷��
	void SetMaxLife(int nMaxLife) { m_nMaxLife = nMaxLife; }        //�ő�̗͂�ݒ肷��
	int& GetLife() { return m_nLife; }                              //�̗͂��擾����
	int& GetMaxLife() { return m_nMaxLife; }                        //�ő�̗͂��擾����
	bool& GetHitStop() { return m_bHitStop; }                       //�q�b�g�X�g�b�v��Ԃ��ǂ������擾����
	void SetAutoSubLife(bool bUse) { m_bAutoSubLife = bUse; }       //�̗͂������I�Ɍ��炷���ǂ�����ݒ�
	void SetHitStopTime(int nHitStopTime) { m_nHitStopTime = nHitStopTime; }//�q�b�g�X�g�b�v���Ԃ�ݒ肷��
	int& GetHitStopTime() { return m_nHitStopTime; }                //�q�b�g�X�g�b�v���Ԃ��擾����
	virtual void SetDamage(int nDamage, int nHitStopTime);          //�_���[�W��^����
	virtual void SetHeal(int nHeal,D3DXCOLOR col,float fWidth,float fHeight);                  //�񕜗ʂ�^����
	//=================================================================================================================

    //===================================
	//�d�͊֌W
	//===================================
	void SetNotUseGravityTime(int nTime) { m_nNotUseGravityTime = nTime; m_bTimeChengeGravity = true; }//�d�͂��g��Ȃ����Ԃ�ݒ�
	void SetUseGravity(bool bUse) { m_bUseGravity = bUse; }               //�d�͂��g�p���邩�ǂ�����ݒ�
	bool& GetUseGravity() { return m_bUseGravity; }                       //�d�͂��g�p���邩�ǂ�����ݒ�
	//=================================================================================================================

	//===================================
	//�u���b�N�����蔻��n
	//===================================
	void SetUseBlockCollision(bool bUse) { m_bUseBlockCollision = bUse;}                    //�u���b�N�Ƃ̓����蔻����g�p���邩�ǂ���
	bool& GetUseBlockCollision() { return m_bUseBlockCollision; }                           //�u���b�N�Ƃ̓����蔻����g�p���邩�ǂ���
	bool& GetSuccessBlockCollisionX() { return m_bSuccessBlockCollisionX; }                 //�u���b�N�Ƃ�X�����̓����蔻�肪�����������ǂ���
	bool& GetSuccessBlockCollisionY() { return m_bSuccessBlockCollisionY; }                 //�u���b�N�Ƃ�Y�����̓����蔻�肪�����������ǂ���
	bool& GetSuccessBlockCollisionZ() { return m_bSuccessBlockCollisionY; }                 //�u���b�N�Ƃ�Z�����̓����蔻�肪�����������ǂ���
	//=================================================================================================================

	//===================================
	//�e�N�X�`���֌W
	//===================================
	void SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture);//�_�����}�e���A���̃e�N�X�`����ς���
	//=================================================================================================================

	//===================================
	//�����֌W
	//===================================
	void SetSummonObjectDeath(bool bDeath) { m_bSummonObjectDeath = bDeath; }               //���������I�u�W�F�N�g�������t���O
	bool& GetSummonObjectDeath() { return m_bSummonObjectDeath; }                           //���������I�u�W�F�N�g���������ǂ������擾
	//=================================================================================================================

	//===================================
	//�e�֌W
	//===================================
	void SetUseShadow(bool bUse) { m_bUseShadow = bUse; }
	//=================================================================================================================

	void SetUseGravity(float fGravityPower);                //�d�͂��g�p���邩�ǂ���

	void SetUseAddScale(D3DXVECTOR3 AddScale, bool bUse);              //�g�嗦�̉��Z���g�p���邩�ǂ���

	//===================================
	//���S�_�֌W
	//===================================
	D3DXVECTOR3& GetSenterPos() { return m_SenterPos;}//���S�_���擾����
	//=================================================================================================================

	//===================================
	//�F�����֌W
	//===================================
	void SetUseRatioLifeAlpha(bool bUse) { m_bUseRatioLifeAlpha = bUse; }//�̗͂̊����ŃA���t�@�l��ς��邩�ǂ���
	//=================================================================================================================

	//===================================
	//����֌W
	//===================================
	bool& GetDodgeState() { return m_bDodge; }//�����Ԃ��ǂ���
	//=================================================================================================================


	void SetUseMultiSpeed(bool bUse, D3DXVECTOR3 MultiSpeed) { m_MultiSpeed = MultiSpeed; m_bUseMultiSpeed = bUse; }//��Z�������g�p���邩�ǂ���

	void SetUseAddSpeed(bool bUse, D3DXVECTOR3 AddSpeed) { m_bUseAddSpeed = bUse; m_AddSpeed = AddSpeed; }//�������g�p���邩�ǂ���

	//===================================
	//�ʒu�X�V�֌W
	//===================================
	void SetUseUpdatePos(bool bUse) { m_bUseUpdatePos = bUse; }//�ʒu�̍X�V���g�p���邩�ǂ���
	void SetUseAdjustOriginPos(bool bUse) { m_bUseAdjustOriginPos = bUse; }//���_�ɓ��B�����Ƃ��Ɉʒu�̕␳���s�����ǂ���
	//=================================================================================================================

	//===================================
	//�`��֌W
	//===================================
	void SetUseDraw(bool bUse) { m_bUseDraw = bUse; }
	//=================================================================================================================

protected:
	static const float GetNormalGravity() { return m_fNORMAL_GRAVITY; }//��{�̏d�͂��擾����
	virtual void GravityProcess();                  //�d�͂̏���
	void AdjusOriginPosProcess();                   //���_�ɓ��B�����Ƃ��Ɉʒu�̕␳���s�����ǂ���
	void CommonBlockCollision();//���ʂ̃u���b�N�Ƃ̓����蔻��

private:
	//===============================================
	//�ÓI�����o
	//===============================================
	static constexpr float m_fNORMAL_INERTIA = 0.5f;//���ʂ̊���
	static constexpr float m_fNORMAL_GRAVITY = 1.0f;//���ʂ̏d��
	//=================================================================================================================

	void DrawShadow(D3DXVECTOR3 Pos);                                                       //�e��`�悷��
	D3DXVECTOR3 m_Pos;                              //�ʒu
	D3DXVECTOR3 m_SupportPos;                       //�ŏ��ɐݒu���ꂽ�ʒu
	D3DXVECTOR3 m_PosOld;                           //1f�O�̈ʒu
	D3DXVECTOR3 m_Rot;                              //����
	D3DXVECTOR3 m_Scale;                            //�傫��
	D3DXVECTOR3 m_FormarScale;                      //���̊g�嗦
	D3DXVECTOR3 m_Size;                             //�T�C�Y
	D3DXVECTOR3 m_VtxMin;                           //���ꂼ��̍��W�̃��f���̈ʒu�̍ŏ��l
	D3DXVECTOR3 m_OriginVtxMin;                     //���X�̍ŏ��l
	D3DXVECTOR3 m_VtxMax;                           //���ꂼ��̍��W�̃��f���̈ʒu�̍ő�l
	D3DXVECTOR3 m_OriginVtxMax;                     //���X�̍ő�l
	D3DXVECTOR3 m_Move;                             //�ړ���
	D3DXMATRIX m_mtxWorld;                          //�}�g���b�N�X���[���h
	MOVETYPE m_MoveType;                            //�ړ��^�C�v
	DWORD m_SizeFVF;                                //���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* m_pVtxBuff;                               //���_�o�b�t�@�ւ̃|�C���^
	ObjectXInfo m_ObjectXInfo;                      //���f�����̍\����

	int m_nNumVtx;                                  //���_��
	int m_nNumModel;                                //���Ԗڂ̃��f�����H
	int m_nIdxShadow;                               //�e�̃C���f�b�N�X
	int m_nManagerType;                             //�}�l�[�W���[�ŌĂяo����鎞�̃^�C�v�ԍ�

	//===================================
	//�ʒu�X�V�֌W
	//===================================
	bool m_bUseUpdatePos;                           //�ʒu�̍X�V���g�p���邩�ǂ���
	void UpdatePos();                               //�ʒu���X�V������
	bool m_bUseAdjustOriginPos;                     //���_�ɓ��B���Ƃ��Ɉʒu�̕␳���s�����ǂ���

	//===================================
	//�̗͊֌W
	//===================================
	int m_nLife;                                    //�̗�
	int m_nMaxLife;                                 //�ő�̗�

	bool m_bHitStop;                                //�q�b�g�X�g�b�v
	int m_nHitStopTime;                             //�q�b�g�X�g�b�v����

	bool m_bAutoSubLife;                            //�����I�ɑ̗͂����炵�����邩�ǂ���

	//============================================================================================================

	//===================================
	//�e�֌W
	//===================================
	bool m_bUseShadow;//�e���g�p���邩�ǂ���
	//=================================================================================================================


	//===================================
	//�d�͊֌W
	//===================================
	bool m_bUseGravity;                             //�d�͂��g�p���邩�ǂ���
	int m_nNotUseGravityTime;                       //�d�͂��g��Ȃ�����
	bool m_bTimeChengeGravity;                      //�d�͂�ON�AOFF�����Ԃŕς��邩�ǂ���
	//============================================================================================================

	//===================================
	//�����֌W
	//===================================
	bool m_bSummonObjectDeath;                       //�I�u�W�F�N�gX�����������I�u�W�F�N�g�������t���O
	//============================================================================================================

	//===================================
	//�`��֌W
	//===================================
	bool m_bUseDraw;
	//============================================================================================================

	//===================================
	//��]�֌W
	//===================================
	D3DXVECTOR3 m_AddRot;//���Z�������
	bool m_bUseAddRot;   //�����̉��Z���g�p���邩�ǂ���
	//=================================================================================================================

	float m_fInertia;                               //����
	int m_nChengeColorTime;                         //�F��ς��鎞��
	bool m_bColorChenge;                            //�F��ς��Ă��邩�ǂ���
	bool m_bUse;                                    //�g�p���Ă��邩�ǂ���
	bool m_bUseInteria;                             //�����������邩�ǂ���

	bool m_bIsLanding;                              //�n�ʂɂ��邩�ǂ���
	bool m_bIsWalling;                              //�ǂɂ������Ă��邩�ǂ���
	bool m_bIsLiftLanding;                          //���t�g�̏�ɏ���Ă��邩�ǂ���
	bool m_bIsJumping;                              //�W�����v���Ă��邩�ǂ���

	CObject* m_pLiftLandingObj;                       //����Ă��郊�t�g���i�[����

	//===================================
	//�y�A�����g�n
	//===================================
	D3DXVECTOR3 m_ParentSetPos;                          //�e�̈ʒu
	D3DXVECTOR3 m_ParentSetRot;                          //�e�̌���
	D3DXVECTOR3 m_ParentSetScale;                        //�e�̊g�嗦

	D3DXVECTOR3 m_OffSetPos;                             //�I�t�Z�b�g�̈ʒu
	D3DXVECTOR3 m_OffSetRot;                             //�I�t�Z�b�g�̌���
	D3DXVECTOR3 m_OffSetScale;                           //�I�t�Z�b�g�̊g�嗦

	D3DXMATRIX m_ParentMtxWorld;                         //�e�̃}�g���b�N�X���[���h
	bool m_bUseParent;                                   //�e���g�p���邩�ǂ���
	//============================================================================================================

	//===================================
	//�����o���̎��Ɏg���K�{���
	//===================================
	int m_nTypeNum;                                 //���f�����Ƃ̃^�C�v�ԍ�
	ROTTYPE m_RotType;                              //����
	OBJECTXTYPE m_nObjXType;                        //�I�u�W�F�N�gX�̕���
	//============================================================================================================

	//===================================
	//�u���b�N�����蔻��n
	//===================================
	bool m_bUseBlockCollision;                                            //�u���b�N�Ƃ̓����蔻����g�p���邩�ǂ���
	bool m_bSuccessBlockCollisionX;                                       //X�����̓����蔻�肪�����������ǂ���
	bool m_bSuccessBlockCollisionY;                                       //Y�����̓����蔻�肪�����������ǂ���
	bool m_bSuccessBlockCollisionZ;                                       //Z�����̓����蔻�肪�����������ǂ���
	//=================================================================================================================

	//===================================
	//���S�_�֌W
	//===================================
	D3DXVECTOR3 m_SenterPos;
	void CalculateSenterPos();
	//=================================================================================================================

	//===================================
	//����֌W
	//===================================
	bool m_bDodge;//�����Ԃ��ǂ���
	//=================================================================================================================

	//===================================
	//�F�����֌W
	//===================================
	bool m_bUseRatioLifeAlpha;//�̗͂̊����œ����x��ς��邩�ǂ���
	//=================================================================================================================

	//===================================
	//�֐�
	//===================================
	void SetFormarColor();            //���̐F�����ɖ߂� 
	void OffSetUpdate();              //�I�t�Z�b�g�̏����X�V����
	void ChengeGravityProcess();      //�d�͂�ς��鏈��
	void HitStopProcess();            //�q�b�g�X�g�b�v�̏���
    //=================================================================================================================

	bool m_bUseMultiSpeed;                                  //��Z���������邩�ǂ���
	D3DXVECTOR3 m_MultiSpeed;                               //��Z�����x

	bool m_bUseAddSpeed;                                    //�������g�p���邩�ǂ���
	D3DXVECTOR3 m_AddSpeed;                                 //�����x

	float m_fGravityPower;                                  //�d�͂̑傫��

	bool m_bUseAddScaling;                                  //�g�嗦���g�p���邩�ǂ���
	D3DXVECTOR3 m_AddScale;                                 //���Z����g�嗦��ݒ�

	//===================================
	//���f�����֌W
	//===================================
	int m_nModelInfoIdx;//���f�����̃A�h���X

	//===================================
	//�G�f�B�^�n
	//===================================
	void ChengeEditScale();             //�g�嗦��ύX����
	void ChengeEditScaleX();
	void ChengeEditScaleY();
	void ChengeEditScaleZ();

	void ChengeEditPos();               //�ʒu���ړ�����
};
//============================================================================================================
#endif