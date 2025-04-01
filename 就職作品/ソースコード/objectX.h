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

	//=================================
	//�`����\����
	//=================================
	struct DrawInfo
	{
		//�e
		bool bUseShadow = true;//�e���g�p���邩�ǂ���!

		//�`����g�p���邩�ǂ���
		bool bUseDraw = true;//!

		//�J�����O���邩�ǂ���
		bool bUseCulling = false;

		//�q�}�g���b�N�X�ɂ��邩�ǂ���
		bool bUseMatrixChild = false;

		//�F
		int nChengeColorTime = 0;                    //�F��ς��鎞��!
		bool bColorChenge = false;                   //�F��ς��Ă��邩�ǂ���!
		bool bBlinkingColor = false;                 //�F��_�ł����邩�ǂ���

		//���[���h�ϊ��s��
		D3DXMATRIX mtxWorld;                     //�}�g���b�N�X���[���h!
	    D3DXMATRIX * pMtxParent = nullptr;            //�e�}�g���b�N�X 
		D3DXCOLOR Color = { 1.0f,1.0f,1.0f,1.0f };    //���݂̓���̐F����

		//�e
		void SetUseShadow(bool bUse) { bUseShadow = bUse; }
		const bool& GetUseDraw() const { return bUseDraw; }

		//�`�悷�邩�ǂ���
		void SetUseDraw(bool bUse) { bUseDraw = bUse; }

		//�q�}�g���b�N�X�ɂ��邩�ǂ���
		void SetUseMatrixChild(bool bUse,D3DXMATRIX * CopyMatrixParent) { bUseMatrixChild = bUse; pMtxParent = CopyMatrixParent; }

		//�J�����O���邩�ǂ���
		void SetUseCulling(bool bUse) { bUseCulling = bUse; }

		//�F���w�莞�ԕς������鏈��
		void ChengeColorProcess(CObjectX * pObjX);

        //���[���h�ϊ��s��
		void SetMtxParent(D3DXMATRIX* pCopyMtxParent) {pMtxParent = pCopyMtxParent; }              //�e�}�g���b�N�X���擾                   
		D3DXMATRIX& GetMatrixWorld() { return mtxWorld; }                     //���[���h�ϊ��s��̎擾
		//================================================================================================================================================
	};
	//============================================================================================================

	struct PosInfo
	{
		//==========================================================
        //�ʒu
        //==========================================================
		D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };                              //�ʒu
		D3DXVECTOR3 PosOld = { 0.0f,0.0f,0.0f };                           //1f�O�̈ʒu
		D3DXVECTOR3 PosFuture = { 0.0f,0.0f,0.0f };                        //1f��̈ʒu
		D3DXVECTOR3 SupportPos = { 0.0f,0.0f,0.0f };                       //�ŏ��ɐݒu���ꂽ�ʒu
		D3DXVECTOR3 WorldPos = { 0.0f,0.0f,0.0f };                         //���[���h���W�����߂�
		D3DXVECTOR3 SenterPos = { 0.0f,0.0f,0.0f };                        //���S�_�̈ʒu
		//=================================================================================================================
    
	    //�ʒu
		void SetPos(D3DXVECTOR3 CopyPos) { Pos = CopyPos; }                                   //�ʒu�̐ݒ�
		const D3DXVECTOR3& GetPos() const { return Pos; }                                     //�ʒu�̎擾

		//1f�O�̈ʒu
		void SetPosOld(D3DXVECTOR3 CopyPosOld) { PosOld = CopyPosOld; }                       //1f�O�̈ʒu�̐ݒ�
		const D3DXVECTOR3& GetPosOld() const { return PosOld; }                               //1f�O�̈ʒu��ݒ�

		//�����ʒu
		void SetSupportPos(D3DXVECTOR3 CopySupportPos) { SupportPos = CopySupportPos; }       //�ݒu�ʒu��ݒ肷��
		D3DXVECTOR3& GetSupportPos() { return SupportPos; }                                   //�ݒu�ʒu���擾����

		//���S�_�̈ʒu
		D3DXVECTOR3& GetSenterPos() { return SenterPos; }                                     //���S�_���擾����

		//1f��̈ʒu
		void SetPosFuture(D3DXVECTOR3 CopyPosFuture) { PosFuture = CopyPosFuture; }           //1f��̈ʒu��ݒ肷��
		const D3DXVECTOR3& GetPosFuture() const { return PosFuture; }                         //1f��̈ʒu���擾����

		//���[���h���W
		const D3DXVECTOR3& GetWorldPos() const { return WorldPos; }                           //���[���h���W���擾����i����cosnt�͌Ăяo����Œl��ύX�o���Ȃ����A
		                                                                                      //�E��const�́A�֐����Œl��ύX�o���Ȃ�����

		//============================================================================================================
	};

	struct RotInfo
	{
		//�ϐ�
		D3DXVECTOR3 Rot = {0.0f,0.0f,0.0f};                              //����!
		D3DXVECTOR3 AddRot = { 0.0f,0.0f,0.0f};                          //���Z�������!
		D3DXVECTOR3 RotAim = { 0.0f,0.0f,0.0f};                          //�ړI�̌��� 
		bool bUseAddRot = false;   //�����̉��Z���g�p���邩�ǂ���!

		//�֐�
		void SetRot(D3DXVECTOR3 CopyRot) { Rot = CopyRot; }                                           //�����̐ݒ�
		D3DXVECTOR3& GetRot() { return Rot; }                                                         //�����̎擾

		void SetUseAddRot(bool bUse, D3DXVECTOR3 CopyRot) { bUseAddRot = bUse; AddRot = CopyRot; }    //�����̉��Z��ݒ�
		bool& GetUseAddRot() { return bUseAddRot; }                                                   //�����̉��Z���g�p���Ă��邩�ǂ���
		D3DXVECTOR3& GetAddRot() { return AddRot; }                                                   //�����̉��Z�ʂ��擾

		void SetRotAim(D3DXVECTOR3 CopyRotAim) { RotAim = CopyRotAim; }                               //�ړI�̌�����ݒ�
		const D3DXVECTOR3& GetRotAim() const { return RotAim; }                                       //�ړI�̌������擾
	};

	struct SizeInfo
	{
		//*�ϐ�

		//���_�̍ŏ��l
		D3DXVECTOR3 VtxMin = { 0.0f,0.0f,0.0f };                           //���ꂼ��̍��W�̃��f���̈ʒu�̍ŏ��l!
		D3DXVECTOR3 OriginVtxMin = { 0.0f,0.0f,0.0f };                     //���X�̍ŏ��l!

		//���_�̍ő�l
		D3DXVECTOR3 VtxMax = { 0.0f,0.0f,0.0f };                           //���ꂼ��̍��W�̃��f���̈ʒu�̍ő�l!
		D3DXVECTOR3 OriginVtxMax = { 0.0f,0.0f,0.0f };                     //���X�̍ő�l!

		//�T�C�Y
		D3DXVECTOR3 Size = { 0.0f,0.0f,0.0f };                             //�T�C�Y!

		//XZ�̃T�C�Y��ς���
		bool bSwapVtxXZ = false;

		//�ϐ�
		D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f };                            //�傫��!
		D3DXVECTOR3 FormarScale = { 1.0f,1.0f,1.0f };                      //���̊g�嗦!
		D3DXVECTOR3 MultiScale = { 0.0f,0.0f,0.0f };                       //��Z�g�嗦
		D3DXVECTOR3 AddScale = { 0.0f,0.0f,0.0f };                         //���Z����g�嗦��ݒ�!
		bool bUseMultiScale = false;                                       //�g�嗦����Z���邩�ǂ���
		bool bUseAddScaling = false;                                       //�g�嗦���g�p���邩�ǂ���!


		//*�֐�
		D3DXVECTOR3& GetVtxMax() { return VtxMax; }                                          //�e���_�ő�l�̎擾
		void SetVtxMax(D3DXVECTOR3 CopyVtxMax) { VtxMax = CopyVtxMax; }
		D3DXVECTOR3& GetOriginVtxMax() { return OriginVtxMax; }                              //���̊e���_�ő�l�̎擾
		void SetOriginVtxMax(D3DXVECTOR3 CopyVtxMax) { OriginVtxMax = CopyVtxMax; }                   //���̍ő咸�_�̐ݒ�

		//�ŏ����_
		D3DXVECTOR3& GetVtxMin() { return VtxMin; }                                          //�e���_�ŏ��l�̎擾
		void SetVtxMin(D3DXVECTOR3 CopyVtxMin) { VtxMin = CopyVtxMin; }
		D3DXVECTOR3& GetOriginVtxMin() { return OriginVtxMin; }                              //���̊e���_�ŏ��l�̎擾
		void SetOriginVtxMin(D3DXVECTOR3 CopyVtxMin) { OriginVtxMin = CopyVtxMin; }                   //���̍ŏ����_�̐ݒ�

		D3DXVECTOR3 GetSize() { return Size; }                                                //X�I�u�W�F�N�g�̃T�C�Y���擾

		//�ő咸�_�ƍŏ����_���X���b�v
		void ActiveSwapVtxMaxMin()
		{
			if (bSwapVtxXZ == true)
			{
				swap(OriginVtxMax.x, OriginVtxMax.z);
				swap(OriginVtxMin.x, OriginVtxMin.z);
			}
		}
		//�֐�
		void SetScale(D3DXVECTOR3 CopyScale) { Scale = CopyScale; }                                  //�g�嗦�̐ݒ�
		D3DXVECTOR3& GetScale() { return Scale; }                                            //�g�嗦�̎擾

		//���̊g�嗦
		void SetFormarScale(D3DXVECTOR3 CopyScale) { FormarScale = CopyScale; }                      //���̊g�嗦��ݒ�
		D3DXVECTOR3& GetFormarScale() { return FormarScale; }                                //���̊g�嗦���擾

		//���Z�g�嗦
		void SetUseAddScale(D3DXVECTOR3 CopyAddScale, bool bUse);                                  //�g�嗦�̉��Z���g�p���邩�ǂ���

		//��Z�g�嗦
		void SetUseMultiScale(bool bUse, D3DXVECTOR3 CopyScale) { bUseMultiScale = bUse; MultiScale = CopyScale; }

		void SetUseSwapVtxXZ(bool bUse) { bSwapVtxXZ = bUse; }
		const bool& GetUseSwapVtxXZ()const { return bSwapVtxXZ; }

		//����
		void AddScaleProcess();//�g�嗦���Z����
		void MultiScaleProcess();//�g�嗦��Z����
		void DecideVtxMaxMinProcess();//�g�嗦���Q�Ƃ��Ē��_�̍ő�ŏ������߂鏈��
	};

	struct MoveInfo
	{
		//*�ϐ�

		//�ړ���
		D3DXVECTOR3 Move = { 0.0f,0.0f,0.0f };                             //�ړ���!
		D3DXVECTOR3 AddMove = { 0.0f,0.0f,0.0f };                          //���Z�ړ���
		//����
		float fInertia = m_fNORMAL_INERTIA;                               //����!
		bool bUseInteria = false;                             //�����������邩�ǂ���!
		//�d��
		bool bUseGravity = false;                             //�d�͂��g�p���邩�ǂ���!
		float fGravityPower = m_fNORMAL_GRAVITY;                          //�d�͂̑傫��!
		//��Z����
		bool bUseMultiSpeed = false;                                  //��Z���������邩�ǂ���!
		D3DXVECTOR3 MultiSpeed = { 0.0f,0.0f,0.0f };                               //��Z�����x!
		//����
		bool bUseAddSpeed = false;                                    //�������g�p���邩�ǂ���!
		D3DXVECTOR3 AddSpeed = { 0.0f,0.0f,0.0f };                                 //�����x

		//�ʒu�̍X�V�����邩�ǂ���
		bool bUseUpdatePos = true;                           //�ʒu�̍X�V���g�p���邩�ǂ���!

		//*�֐�
		
		//�ړ���
		void SetMove(D3DXVECTOR3 CopyMove) { Move = CopyMove; }//�ړ��ʂ�ݒ肷��
		const D3DXVECTOR3& GetMove() const { return Move; }//�ړ��ʂ��擾����

		void SetAddMove(D3DXVECTOR3 CopyMove) { AddMove = CopyMove; }//���Z�ړ��ʂ�ݒ�
		const D3DXVECTOR3& GetAddMove() const { return AddMove; }//���Z�ړ��ʂ��擾

		//����
		void SetUseInteria(bool bUse, float fCopyInertia) { bUseInteria = bUse; fInertia = fCopyInertia; }//�����������邩�ǂ���
		float& GetInertia() { return fInertia; }//�������擾����
		void SetUndoInertia() { fInertia = m_fNORMAL_INERTIA; }//�����̑傫�������ɖ߂�
		bool& GetUseInteria() { return bUseInteria; }//�������|���邩�ǂ������擾

		//�d��
		void SetUseGravity(bool bUse, float fCopyGravityPower) { bUseGravity = bUse; fGravityPower = fCopyGravityPower; }//�d�͂�ݒ肷��
		const bool& GetUseGravity() const { return bUseGravity; }//�d�͂��g�p���邩�ǂ�����ݒ�
		const float& GetGravity() const { return fGravityPower; }//�d�͂��擾����
		//��Z����
		void SetUseMultiSpeed(bool bUse, D3DXVECTOR3 CopyMultiSpeed) { MultiSpeed = CopyMultiSpeed; bUseMultiSpeed = bUse; }//��Z�������g�p���邩�ǂ���

		//���Z���x
		void SetUseAddSpeed(bool bUse, D3DXVECTOR3 CopyAddSpeed) { bUseAddSpeed = bUse; AddSpeed = CopyAddSpeed; }//�������g�p���邩�ǂ���
		//============================================================================================================

		//===================================
		//�ʒu�X�V�֌W
		//===================================
		void SetUseUpdatePos(bool bUse) { bUseUpdatePos = bUse; }//�ʒu�̍X�V���g�p���邩�ǂ���
		void GravityProcess();                                   //�d�͂̏���
		void MultiSpeedProcess();                                //��Z��������
		void AddSpeedProcess();                                  //��������
		//=================================================================================================================
	};

	//�̗͍\����
	struct LifeInfo
	{
		//*�ϐ�
		bool bHitStop = false;                        //�q�b�g�X�g�b�v!
		int nHitStopTime = 0;                             //�q�b�g�X�g�b�v����!
		void HitStopProcess();                        //�q�b�g�X�g�b�v�̏���

		//�����I�ɑ̗͂����炷���ǂ���
		bool bAutoSubLife = false;                            //�����I�ɑ̗͂����炵�����邩�ǂ���!

		//�̗�
		int nLife = 1;                                    //�̗�!
		int nMaxLife = 1;                                 //�ő�̗�!

		//�F
		bool bUseRatioLifeAlpha = false;                      //�̗͂̊����œ����x��ς��邩�ǂ���!

		//���S
		bool bAutoDeath = false;                             //�̗͂O�Ŏ����I�ɔj������邩�ǂ���
		//============================================================================================================

		//�̗�
		void SetLife(int nCopyLife) { nLife = nCopyLife; }                    //�̗͂�ݒ肷��
		int& GetLife() { return nLife; }                              //�̗͂��擾����

		//�ő�̗�
		void SetMaxLife(int nCopyMaxLife) { nMaxLife = nCopyMaxLife; }        //�ő�̗͂�ݒ肷��
		int& GetMaxLife() { return nMaxLife; }                        //�ő�̗͂��擾����

		//�q�b�g�X�g�b�v
		void SetHitStopTime(int nCopyHitStopTime) { nHitStopTime = nCopyHitStopTime; }//�q�b�g�X�g�b�v���Ԃ�ݒ肷��
		bool& GetHitStop() { return bHitStop; }                       //�q�b�g�X�g�b�v��Ԃ��ǂ������擾����

		//�����I�ɑ̗͂����炷
		void SetAutoSubLife(bool bUse) { bAutoSubLife = bUse; }       //�̗͂������I�Ɍ��炷���ǂ�����ݒ�
		int& GetHitStopTime() { return nHitStopTime; }                //�q�b�g�X�g�b�v���Ԃ��擾����

		//�̗͂ɂ���ē����x��ς��邩�ǂ���
		void SetUseRatioLifeAlpha(bool bUse) { bUseRatioLifeAlpha = bUse; }

		//�̗͂O�Ŏ����I�ɔj�����邩�ǂ���
		void SetAutoDeath(bool bUse) { bAutoDeath = bUse; }
		const bool& GetAutoDeath() const { return bAutoDeath; }

		//�̗͂������I�Ɍ��炷����
		void AutoSubLifeProcess();

		//�̗͂̊����ɉ����ĐF������ς��鏈��
		void RatioLifeAlphaColorProcess(CObjectX * pObjX);

		//�̗͂��O�ɂȂ����玩���I�Ɏ��S�t���O�𔭓����鏈��
		void AutoDeathProcess(CObjectX* pObjX);
	};

	//�����蔻��\����
	struct CollisionInfo
	{
		//�����`
		struct Square
		{
			//�ϐ�
			bool bPushOutFirst[static_cast<int>(AXIS::MAX)] = {};                  

			//�֐�
			void ResetPushOutFirstFlag() //���ꂼ��̎��̔����D�悷��t���O�����Z�b�g����
			{
				for (int nCnt = 0; nCnt < static_cast<int>(AXIS::MAX); nCnt++)
				{
					bPushOutFirst[nCnt] = false;
				}
			}
			void SetPushOutFirstFlag(AXIS Axis,bool bFlag)//���ꂼ��̎��̔����D�悷�邩�ǂ�����ݒ�
			{
				bPushOutFirst[static_cast<int>(Axis)] = bFlag;
			}
			const bool& GetPushOutFlag(AXIS Axis)//���ꂼ��̎��̔����D�悷�邩�ǂ������擾
			{ 
				return bPushOutFirst[static_cast<int>(Axis)];
			}
		};

		//���
		struct State
		{
		private:
			bool bIsLanding = false;                                      //�n�ʂɂ��邩�ǂ���
			bool bIsLandingOld = false;                                   //1f�O�ɒn�ʂɂ��邩�ǂ���
			bool bIsWalling = false;                                      //�ǂɂ��邩�ǂ���
			bool bIsWallingOld = false;                                   //1f�O�ɕǂɂ��邩�ǂ���
		public:
			const bool& GetLanding() const { return bIsLanding; }         //�n�ʂɂ��邩�ǂ������擾
			const bool& GetLandingOld() const { return bIsLandingOld; }   //1f�O�ɒn�ʂɂ��邩�ǂ������擾����
			const bool& GetWalling() const { return bIsWalling; }         //�ǂɂ��邩�ǂ������擾
			const bool& GetWallingOld() const { return bIsWallingOld; }   //1f�O�ɕǂɂ��邩�ǂ������擾
			void SetLanding(bool bFlag) { bIsLanding = bFlag; }           //�n�ʂɂ��邩�ǂ�����ݒ�
			void SetLandingOld(bool bFlag) { bIsLandingOld = bFlag; }     //1f�O�ɒn�ʂɂ��邩�ǂ������擾����
			void SetWalling(bool bFlag) { bIsWalling = bFlag; }           //�ǂɂ��邩�ǂ�����ݒ�
			void SetWallingOld(bool bFlag) { bIsWallingOld = bFlag; }     //1f�O�ɕǂɂ��邩�ǂ������擾����
			void ResetState() { bIsLanding = false; bIsWalling = false; } //�t���O�����Z�b�g����
		};

		//�ϐ�
		State StateInfo = {};                            //������
		Square SquareInfo = {};                          //�����`�̓����蔻��

		//�֐�
		State & GetState() { return StateInfo; }         //�����Ԃ̎擾
		Square & GetSquareInfo() { return SquareInfo; }  //�����`�̔�����̎擾
	};


	CObjectX(int nPri = 0, bool bUseintPri = false,CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                                                           //�R���X�g���N�^
	~CObjectX() override;                                                 //�f�X�g���N�^
	HRESULT Init() override;                                              //����������
	void Uninit() override;                                               //�I������
	void Update() override;                                               //�X�V����
	void Draw() override;                                                 //�`�揈��
	void SetDeath() override;                                             //���S�t���O�ݒ菈��
	void BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture,D3DCOLORVALUE * pDiffuse);//���f���������蓖�Ă�

	void UpdatePos();                                                     //�ʒu�̍X�V����
	//==========================================================
	//�ÓI�����o�錾
	//==========================================================
	static void SetCommonDraw(bool bDraw) { s_bCOMMON_DRAWSHADOW = bDraw; }    //�e��`�悷�邩�ǂ���
	
	//�N�H�[�^�j�I��
	void SetAxis(float fAxis) { m_fAxis = fAxis; }                             //��]�p��ݒ�
	const float& GetAxis() const { return m_fAxis; }                           //��]�p���擾 
	void SetVecAxis(D3DXVECTOR3 VecAxis) { m_VecAxis = VecAxis; }              //��]����ݒ�
	const D3DXVECTOR3& GetVecAxis() const { return m_VecAxis; }                //��]�����擾

	//============================================================================================================

	//==========================================================
	//�g�嗦
	//==========================================================
	//============================================================================================================
	
	//==========================================================
	//�̗͊֌W
	//==========================================================
	virtual void SetDamage(int nDamage, int nHitStopTime);                                   //�_���[�W��^����
	//============================================================================================================

	//==========================================================
	//�T�C�Y
	//==========================================================
	void CalculateSenterPos();
	virtual void SetSize();                                                                  //�T�C�Y��ݒ肷�� 
	//============================================================================================================

	//==========================================================
	//�G�f�B�^
	//==========================================================

	//�I�u�W�F�N�gX�̎��
	void SetObjXType(OBJECTXTYPE ObjXType) { m_nObjXType = ObjXType; }                                           //�I�u�W�F�N�gX�̕��ނ�ݒ�
	OBJECTXTYPE GetObjXType() { return m_nObjXType; }                                                            //�I�u�W�F�N�gX�̃^�C�v���擾����
																							                     
	//�^�C�v�ԍ�																			                        
	void SetTypeNum(int nTypeNum) { m_nTypeNum = nTypeNum; }                                                     //�I�u�W�F�N�g���Ƃ̃^�C�v�ԍ���ݒ�
	int GetTypeNum() { return m_nTypeNum;}                                                                       //�I�u�W�F�N�g���Ƃ̃^�C�v�ԍ���ݒ�
																							                     
	//���\������������																	                        
	void ManagerChooseControlInfo() override;                                                                    //�X�e�[�W�}�l�[�W���[�����𑀍삷��
																							                     
	void SaveInfoTxt(fstream& WritingFile) override;                                                             //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override; //�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	//================================================================================================================================================

	//==========================================================
	//���f�����
	//==========================================================
	CObjectX::ObjectXInfo& GetObjectXInfo() { return m_ObjectXInfo; }
	//================================================================================================================================================

	//===================================
	//�`��
	//===================================
	//�_�����}�e���A���Ƀe�N�X�`�����蓖�āi�|�C���^�̃|�C���^�ɂȂ��Ă邩����v�j
	void SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture);

	//�F����
	void SetColor(D3DXCOLOR col, int nColChengeTime, bool bChoose, bool bSetAlpha,bool bBlinking);

	//=================================================================================================================

	//===================================
	//�e�\����
	//===================================

	//�`����̎擾
	DrawInfo& GetDrawInfo() { return m_DrawInfo; }

	//�ʒu���̎擾
	PosInfo& GetPosInfo() { return m_PosInfo; }

	//�������̎擾
	RotInfo& GetRotInfo() { return m_RotInfo; }

	//�T�C�Y���̎擾
	SizeInfo& GetSizeInfo() { return m_SizeInfo; }

	//�ړ��ʏ��̎擾
	MoveInfo& GetMoveInfo() { return m_MoveInfo; }

	//�̗͏��̎擾
	LifeInfo& GetLifeInfo() { return m_LifeInfo; }

	//�����蔻����̎擾
	CollisionInfo& GetCollisionInfo() { return m_CollisionInfo; }

	//=================================================================================================================

	//==========================================================
	//����֌W
	//==========================================================

	//�����`�̉����o������

	//X
	void SetExtrusionCollisionSquareX(bool bSuccess) { m_bExtrusionCollisionSquareX = bSuccess; }
	const bool& GetExtrusionCollisionSquareX() const { return m_bExtrusionCollisionSquareX; }

	//Y
	void SetExtrusionCollisionSquareY(bool bSuccess) { m_bExtrusionCollisionSquareY = bSuccess; }
	const bool& GetExtrusionCollisionSquareY() const { return m_bExtrusionCollisionSquareY; }

	//Z
	void SetExtrusionCollisionSquareZ(bool bSuccess) { m_bExtrusionCollisionSquareZ = bSuccess; }
	const bool& GetExtrusionCollisionSquareZ() const { return m_bExtrusionCollisionSquareZ; }

	//============================================================================================================


    //==========================================================
	//�ÓI�����o�擾
	//==========================================================
	static const float GetNormalGravity() { return m_fNORMAL_GRAVITY; }//�ʏ�d�͂��擾����
	static const float GetNormalInertia() { return m_fNORMAL_INERTIA; }//�������擾
	//=================================================================================================================
private:
	//===============================================
    //�ÓI�����o
    //===============================================
	static constexpr float m_fNORMAL_INERTIA = 0.5f;//���ʂ̊���
	static constexpr float m_fNORMAL_GRAVITY = 1.0f;//���ʂ̏d��
	static bool s_bCOMMON_DRAWSHADOW;               //�`��
	//=================================================================================================================

	void DrawShadow();                                                       //�e��`�悷��

	//==========================================================
	//�N�H�[�^�j�I��
	//==========================================================
    float m_fAxis;//�N�H�[�^�j�I���̉�]�p
	D3DXVECTOR3 m_VecAxis;//�N�H�[�^�j�I���̉�]��
	//==========================================================================================================================================================
	
	//===============================================
	//���f�����
	//===============================================
	ObjectXInfo m_ObjectXInfo;                      //���f�����̍\����!
	//===========================================================================================================================================================
	
	//===================================
	//�`��֌W
	//===================================
	void SetFormarColor();                     //���̐F�����ɖ߂�
	void SetOnlyColor(D3DXCOLOR Col);          //�F������ݒ肷��
	void SetOnlyFormarColor();                 //�f�[�^�͕ς����ɃA���t�@�l�����ݒ肷��
	//=================================================================================================================

	//===================================
	//���f���C���f�b�N�X
	//===================================
	int m_nIndexObjectX;
	//==================================================================================================================
	
	//===================================
	//�e�\����
	//===================================
	DrawInfo m_DrawInfo;           //�`����
	PosInfo m_PosInfo;             //�ʒu���
	RotInfo m_RotInfo;             //�������
	SizeInfo m_SizeInfo;           //�T�C�Y���
	MoveInfo m_MoveInfo;           //�ړ��ʏ��
	LifeInfo m_LifeInfo;           //�̗͏��
	CollisionInfo m_CollisionInfo; //�����蔻����
	//==================================================================================================================

	//===================================
	//����֌W
	//===================================
	
	//�����`�̉����o������
	bool m_bExtrusionCollisionSquareX;//X
	bool m_bExtrusionCollisionSquareY;//Y
	bool m_bExtrusionCollisionSquareZ;//Z

	//�n�ʂɂ��邩�ǂ���
	bool m_bIsLanding;

	//============================================================================================================


	//===================================
	//�G�f�B�^�֌W
	//===================================
	int m_nManagerType;                             //�}�l�[�W���[�ŌĂяo����鎞�̃^�C�v�ԍ�!
	int m_nTypeNum;                                 //���f�����Ƃ̃^�C�v�ԍ�!
	OBJECTXTYPE m_nObjXType;                        //�I�u�W�F�N�gX�̕���!

	void ChengeEditScale();                         //�g�嗦��ύX����
	void ChengeEditScaleX();                        //�g�嗦X��ύX����
	void ChengeEditScaleY();                        //�g�嗦Y��ύX����
	void ChengeEditScaleZ();                        //�g�嗦Z��ύX����
	void ChengeEditPos();                           //�ʒu��ύX����
	void EditLife();                                //�̗͂�ύX����
	void ChengeEditSwapVtxXZ();                     //�ő咸�_�ƍŏ����_��ς��邩�ǂ���
	//=================================================================================================================

};
//============================================================================================================
#endif