//=======================================================================================================================================================================================================
//
// �v���C���[�̏���
// Author : Shina Taiyo
//
//=======================================================================================================================================================================================================
#include "player.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "emitter.h"
#include <stdio.h>
#include <string.h>
#include "model.h"
#include "scrap.h"
#include "particle.h"
#include "debugproc.h"
#include "gauge.h"
#include "level.h"
#include "battery.h"
#include "enemy.h"
#include "attackmodel.h"
#include "boss.h"
#include "field.h"
#include "wall.h"
#include "dummy.h"
#include "Rader.h"
#include "effect.h"

//=======================================================================================================================================================================================================
// �O���[�o���ϐ�
//=======================================================================================================================================================================================================
LPDIRECT3DTEXTURE9 g_apTexturePlayer[MAX_PLAYERPARTS2][MAX_PLAYER_WEAPON][MAX_MODELMAT] = {};	//�e�N�X�`���ւ̃|�C���^
LPD3DXMESH g_pMeshPlayer = NULL;//���b�V���i���_���j�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatPlayer = NULL;//�}�e���A���ւ̃|�C���^
DWORD g_dwNumMatPlayer = 0;//�}�e���A���̐�
D3DXVECTOR3 g_posPlayer;//�ʒu
D3DXVECTOR3 g_rotPlayer;//����
D3DXMATRIX g_mtxWorldPlayer;//���[���h�}�g���b�N�X
int g_nDelay = 0;//�f�B���C�J�E���g
int g_nDelay2 = 0;//�f�B���C�Q
int g_nDecideLevelUpPoint[MAX_LEVEL];//�e���x���A�b�v�ɕK�v�Ȍo���l�ʂ����߂�
Player g_aPlayer;//�v���C���[�̏��

//============================
// �v���C���[�p�[�c���֘A
//============================
ModelParts g_aPlayerInfo[MAX_PLAYERPARTS2];//�v���C���[�̓ǂݍ��ݏ��
int g_nCntParts = 0;//�v���C���[�̃p�[�c��
ModelParts g_aWeaponInfo[MAX_PLAYER_WEAPON];//����̓ǂݍ��ݏ��
int g_nCntWeapon = 0;//����̎��

//============================
// ���[�V�������֘A
//============================
PlayerKey g_aPlayerKey[MAX_PLAYER_WEAPON][MAX_PLAYERPARTS2];//�e�L�[���
PlayerKeySet g_aPlayerKeySet[MAX_PLAYER_WEAPON][MAX_KEY][MAX_MOTION];//�e�L�[�Z�b�g���
PlayerMotionSet g_aPlayerMotionSet[MAX_PLAYER_WEAPON][MAX_MOTION];//���[�V�������

//=======================================================================================================================================================================================================
// ����������
//=======================================================================================================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^�Ǝ擾
	D3DXMATERIAL* pMat;//�}�e���A���̃f�[�^�ւ̃|�C���^
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^

	//==========================================================================================
	// �v���C���[���̏�����
	//==========================================================================================
	g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, -2300.0f);
	g_aPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_aPlayer.targetrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.nIdxShadow = -1;//�e�̃C���f�b�N�X
	g_aPlayer.nIdxRaderDisplay = -1;//���[�_�[�f�B�X�v���C�̃C���f�b�N�X
	g_aPlayer.fWidth = 5.0f;
	g_aPlayer.fHeight = 10.0f;
	g_aPlayer.bIsLanding = false;//�n�ʂɂ��邩�ǂ���
	g_aPlayer.nStateOld = 0;//1f�O�̏�Ԃ�ۑ�����
	g_aPlayer.State = PLAYERSTATE_NUTORAL;//�v���C���[�̏�Ԃ�\���ϐ�
	//g_aPlayer.vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�v���C���[�S�̂̍ő咸�_�l
	//g_aPlayer.vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�v���C���[�S�̂̍ŏ����_�l
	g_aPlayer.nMode = 0;//���[�h�̎��
	g_aPlayer.bMotionFlag = false;//���[�V�������g�p��Ԃɂ���
	g_aPlayer.nHp = 1500;
	g_aPlayer.bUse = true;
	g_aPlayer.nAttackTime = 0;//�U�������Ă��鎞�Ԃ��v������
	g_aPlayer.bCompulsionAttack = false;//�����I�ɍU����Ԃɂ���
	g_aPlayer.bMove = false;//�v���C���[�������Ă��邩�ǂ����̔���
	g_aPlayer.bInvincible = false;//���G��Ԃɂ���
	g_aPlayer.nInvincibleCnt = 0; //���G��Ԃɂ���J�E���g
	g_aPlayer.nNumLandingModel = 0;//�ǂ̃��f���̏�ɏ���Ă��邩
	g_aPlayer.fMoveX = 0.0f;//X�����̈ړ���
	g_aPlayer.fMoveZ = 0.0f;//Z�����̈ړ���
	g_aPlayer.nCntCombo = 0;//�R���{����J�E���g
	g_aPlayer.fCombo = 1.0f;//�R���{�{��

	//============================================
	//�ア�U�����󂯂����̕ϐ�
	//============================================
	g_aPlayer.bWeakDamageReaction = false;//�ア�U�����󂯂��Ƃ��ɐF�Ƃ���ς���
	g_aPlayer.nWeakDamageReactionTime = 0;//�ア�U�����󂯂��Ƃ��Ƀ_���[�W��Ԃɂ��鎞�Ԃ��J�E���g����

	//=================================
	//�o�b�e���[�[�d�֘A
	//=================================

	//�Ǐ[�d
	g_aPlayer.bCompulsionHealWall = false;//�����I�ɕǉ񕜏�Ԃɂ��鏈��
	g_aPlayer.nHealWallTime = 0;//�ǉ񕜂����Ă��鎞�Ԃ��v������
	g_aPlayer.HealWallSaveRot = NULL_VECTOR3;//�[�d���̌�����ۑ�����
	g_aPlayer.bChargingFlag = false;//�[�d����true�ɂ��āA�o�b�e���[�̉񕜂�����t���O
	g_aPlayer.nNumUseWallOutlet = 0;//�g�p���Ă���ǃR���Z���g�̃��f���ԍ����L�^����

	//���[�d
	g_aPlayer.bCompulsionHealGround = false;//�����I�ɏ��񕜏�Ԃɂ��鏈��
	g_aPlayer.nHealGroundTime= 0;//���񕜂����Ă��鎞�Ԃ��v������
	g_aPlayer.HealGroundSaveRot = NULL_VECTOR3;//���[�d���̌�����ۑ�����
	g_aPlayer.bChargingGroundFlag = false;//�[�d����true�ɂ��āA�o�b�e���[�̉񕜂�����t���O
	g_aPlayer.nNumUseGroundOutlet = 0;//�g�p���Ă��鏰�R���Z���g�̃��f���ԍ����L�^����

	//=======================================================================
	// �X�e�[�^�X
	//=======================================================================

	//=================================
	// ���у��{�{�̂̃X�e�[�^�X
	//=================================
	g_aPlayer.nLevel = 0;				//���x��
	g_aPlayer.nTibiroboScrap = 0;		//���у��{�X�N���b�v�l����
	g_aPlayer.nTotalTibiroboScrap = 0;	//���у��{�X�N���b�v���v�l����
	g_aPlayer.nMaxLevelUpPoint = 0;		//���̃��x���܂łɕK�v�ȃX�N���b�v��
	g_aPlayer.nBattery = 1000;			//���у��{�̃o�b�e���[�i�̗́j
	g_aPlayer.nAllUpScrap = 0;			//�S�ẴX�e�[�^�X�����X�N���b�v�l����
	g_aPlayer.nTotalAllUpScrap = 0;     //�S�ẴX�e�[�^�X�����X�N���b�v���v�l����
	g_aPlayer.fSpeed = 0.0f;			//���у��{�̑���
	g_aPlayer.nWeapon = 0;				//�������Ă��镐��
	g_aPlayer.fSuctionScrapRange = 0.0f;//�X�N���b�v�z���͈�

	//=================================
	// ���̃X�e�[�^�X
	//=================================
	g_aPlayer.aSwordInfo.nLevel = 0;			//���x��
	g_aPlayer.aSwordInfo.nSwordScrap = 0;		//���X�N���b�v�l����
	g_aPlayer.aSwordInfo.nTotalSwordScrap = 0;	//���X�N���b�v���v�l����
	g_aPlayer.aSwordInfo.nMaxLevelUpPoint = 0;	//���̃��x���܂łɕK�v�ȃX�N���b�v��
	g_aPlayer.aSwordInfo.nSwordPower = 0;		//���̍U����
	g_aPlayer.aSwordInfo.nSwordAttackSpeed = 0;//���̍U�����x

   //=================================
   // �u�[�������̃X�e�[�^�X
   //=================================
	g_aPlayer.aBoomerangInfo.nLevel = 0;				   //���x��
	g_aPlayer.aBoomerangInfo.nBoomerangScrap = 0;		   //�u�[�������X�N���b�v�l����
	g_aPlayer.aBoomerangInfo.nTotalBoomerangScrap = 0;	   //�u�[�������X�N���b�v���v�l����
	g_aPlayer.aBoomerangInfo.nMaxLevelUpPoint = 0;		   //���̃��x���܂łɕK�v�ȃX�N���b�v��
	g_aPlayer.aBoomerangInfo.nBoomerangPower = 0;	       //�u�[�������̍U����
	g_aPlayer.aBoomerangInfo.fBoomerangRenge = 0.0f;	   //�u�[�������̎˒�
	g_aPlayer.aBoomerangInfo.nBoomerangPiece = 0;		   //�u�[�������̌�
	g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece = 0;       //���Ă�u�[�������̍ő吔
	g_aPlayer.aBoomerangInfo.BoomerangScale = NULL_VECTOR3;//�u�[�������̑傫��
	g_aPlayer.aBoomerangInfo.bPossibleBoomerang = false;   //�u�[�������U�����\���ǂ���

   //=================================
   // �X�s�[�J�[�̃X�e�[�^�X
   //=================================
	g_aPlayer.aSpeakerInfo.nLevel = 0;				//���x��
	g_aPlayer.aSpeakerInfo.nSpeakerScrap = 0;		//�X�s�[�J�[�X�N���b�v�l����
	g_aPlayer.aSpeakerInfo.nTotalSpeakerScrap = 0;	//�X�s�[�J�[�X�N���b�v���v�l����
	g_aPlayer.aSpeakerInfo.nMaxLevelUpPoint = 0;	//���̃��x���܂łɕK�v�ȃX�N���b�v��
	g_aPlayer.aSpeakerInfo.fSpeakerPower = 0.0f;	//�X�s�[�J�[�̍U����
	g_aPlayer.aSpeakerInfo.nPowerConsumption = 0;	//�X�s�[�J�[�̏���d��

	//================================
	// �o�b�e���[
	//================================
	g_aPlayer.nMoveFrame = 0;//�������t���[�������J�E���g����
	g_aPlayer.nAttackCount = 0;//�U���������Ƃ��ɃJ�E���g���J�n����B
	g_aPlayer.nDamageCountTime = 0;//�_���[�W���󂯂Ă��鎞�Ԃ��J�E���g����
	g_aPlayer.bDamageState = false;//�_���[�W��Ԃ��ǂ���

	//================================
	//���U���g�֌W
	//================================
	g_aPlayer.nTotalGetScrap = 0;   //�X�N���b�v���v�l����
	g_aPlayer.nTotalDefeatEnemy = 0;//���v�G���j��

    //================================
	//����֌W
	//================================
	g_aPlayer.nDodgeCoolTime = 0;//������ł���悤�ɂȂ�܂ł̃N�[���^�C��
	g_aPlayer.nDodgeTime = 0;    //��������Ă��鎞��
	g_aPlayer.bCompulsionDodge = false;//�����I�ɉ����Ԃɂ���
	g_aPlayer.bPossibleDodge = false;  //�����I�ɉ����Ԃɂ���

	//================================
	//�x�N�g���ϐ�
	//================================

	g_aPlayer.fVXaim = 0.0f;
	g_aPlayer.fVZaim = 0.0f;
	g_aPlayer.fVaim = 0.0f;

	//================================
	// ���f���̍\����
	//================================
	for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
	{
		for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYERPARTS2; nCntPlayerParts++)
		{
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh = NULL;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat = NULL;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat = 0;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].mtxWorld = {};
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].nIdxModelParent = 0;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].bUse = 0;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���_�̍ŏ��l
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//���_�̍ő�l
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].bUse = false;
			g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].PartsPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�p�[�c�̈ʒu�i���[���h���W�j
		

			//==================================
			// �v���C���[�p�[�c�̏��(�O�`�P�V�j
			//==================================
			if (nCntPlayerParts < MAX_PLAYERPARTS2 - 1)
			{
				//X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(PLAYER_FILENAME[nCntPlayerParts],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat,
					NULL,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh);

				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat; nCntMat++)
				{
					g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].DiffUse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
					g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].FormarDiffUse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//���̐F����
				}

				//���_���̎擾
				int nNumVtx = g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->GetNumVertices();

				//���_�t�H�[�}�b�g�̃T�C�Y���擾
				DWORD sizeFVF = D3DXGetFVFVertexSize(g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->GetFVF());

				//���_�o�b�t�@�̃��b�N
				g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
				{
					//���_���W�̑��
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;


					//================================
					// �v���C���[�̍ő�ŏ������߂�
					//================================

					//���f���̒��_���W�̍ŏ��l�ƍő�l���擾
					if (vtx.x < g_aPlayer.vtxMin.x)
					{//X���W�̍ŏ��l
						g_aPlayer.vtxMin.x = vtx.x;
					}
					if (vtx.y < g_aPlayer.vtxMin.y)
					{//Y���W�̍ŏ��l
						g_aPlayer.vtxMin.y = vtx.y;
					}
					if (vtx.z < g_aPlayer.vtxMin.z)
					{//Z���W�̍ŏ��l
						g_aPlayer.vtxMin.z = vtx.z;
					}
					if (vtx.x > g_aPlayer.vtxMax.x)
					{//X���W�̍ő�l
						g_aPlayer.vtxMax.x = vtx.x;
					}
					if (vtx.y > g_aPlayer.vtxMax.y)
					{//Y���W�̍ő�l
						g_aPlayer.vtxMax.y = vtx.y;
					}
					if (vtx.z > g_aPlayer.vtxMax.z)
					{//Z���W�̍ő�l
						g_aPlayer.vtxMax.z = vtx.z;
					}

					//====================================
					// �p�[�c���Ƃ̍ő�ŏ������߂�
					//====================================

					if (vtx.x < g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.x)
					{//X���W�̍ŏ��l
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.x = vtx.x;
					}
					if (vtx.y < g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.y)
					{//X���W�̍ŏ��l
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.y = vtx.y;
					}
					if (vtx.z < g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.z)
					{//X���W�̍ŏ��l
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMinModel.z = vtx.z;
					}

					if (vtx.x > g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.x)
					{//X���W�̍ő�l
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.x = vtx.x;
					}
					if (vtx.y > g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.y)
					{//X���W�̍ő�l
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.y = vtx.y;
					}
					if (vtx.z > g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.z)
					{//X���W�̍ő�l
						g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].vtxMaxModel.z = vtx.z;
					}

					//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
					pVtxBuff += sizeFVF;
				}

				//��ނ��Ƃ̃T�C�Y�����߂�
				g_aPlayer.Size =
				{
					g_aPlayer.vtxMax.x - g_aPlayer.vtxMin.x,
					g_aPlayer.vtxMax.y - g_aPlayer.vtxMin.y,
					g_aPlayer.vtxMax.z - g_aPlayer.vtxMin.z
				};

				//���_�o�b�t�@�̃A�����b�N
				g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->UnlockVertexBuffer();
			}

			//=========================================================================================
			// �v���C���[�̕���̏��i�v���C���[�̃p�[�c��(18) + ����R��(�p�[�c�� + 1) * MAX_WEAPON)
			//=========================================================================================
			else if (nCntPlayerParts == MAX_PLAYERPARTS)
			{
				g_aPlayer.aWeapon[nCntWeapon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aPlayer.aWeapon[nCntWeapon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aPlayer.aWeapon[nCntWeapon].bUse = 0;
				g_aPlayer.aWeapon[nCntWeapon].nType = 0;

				//X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(WEAPON_FILENAME[nCntWeapon],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat,
					NULL,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat,
					&g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh);

				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat; nCntMat++)
				{
					g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].DiffUse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
					g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].FormarDiffUse[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;//���̐F����

				}

				//���_���̎擾
				int nNumVtx = g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->GetNumVertices();

				//���_�t�H�[�}�b�g�̃T�C�Y���擾
				DWORD sizeFVF = D3DXGetFVFVertexSize(g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->GetFVF());

				//���_�o�b�t�@�̃��b�N(���ۂɓǂݍ���ł��郂�f���̃|�C���^��aWeapon�ɓn���j
				g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
				{
					//���_���W�̑��
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

					//���f���̒��_���W�̍ŏ��l�ƍő�l���擾
					if (vtx.x < g_aPlayer.aWeapon[nCntWeapon].vtxMin.x)
					{//X���W�̍ŏ��l
						g_aPlayer.aWeapon[nCntWeapon].vtxMin.x = vtx.x;
					}
					if (vtx.y < g_aPlayer.aWeapon[nCntWeapon].vtxMin.y)
					{//Y���W�̍ŏ��l
						g_aPlayer.aWeapon[nCntWeapon].vtxMin.y = vtx.y;
					}
					if (vtx.z < g_aPlayer.aWeapon[nCntWeapon].vtxMin.z)
					{//Z���W�̍ŏ��l
						g_aPlayer.aWeapon[nCntWeapon].vtxMin.z = vtx.z;
					}
					if (vtx.x > g_aPlayer.aWeapon[nCntWeapon].vtxMax.x)
					{//X���W�̍ő�l
						g_aPlayer.aWeapon[nCntWeapon].vtxMax.x = vtx.x;
					}
					if (vtx.y > g_aPlayer.aWeapon[nCntWeapon].vtxMax.y)
					{//Y���W�̍ő�l
						g_aPlayer.aWeapon[nCntWeapon].vtxMax.y = vtx.y;
					}
					if (vtx.z > g_aPlayer.aWeapon[nCntWeapon].vtxMax.z)
					{//Z���W�̍ő�l
						g_aPlayer.aWeapon[nCntWeapon].vtxMax.z = vtx.z;
					}

					//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
					pVtxBuff += sizeFVF;
				}

				//��ނ��Ƃ̃T�C�Y�����߂�
				g_aPlayer.aWeapon[nCntWeapon].Size =
				{
					g_aPlayer.aWeapon[nCntWeapon].vtxMax.x - g_aPlayer.aWeapon[nCntWeapon].vtxMin.x,
					g_aPlayer.aWeapon[nCntWeapon].vtxMax.y - g_aPlayer.aWeapon[nCntWeapon].vtxMin.y,
					g_aPlayer.aWeapon[nCntWeapon].vtxMax.z - g_aPlayer.aWeapon[nCntWeapon].vtxMin.z
				};

				//���_�o�b�t�@�̃A�����b�N
				g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pMesh->UnlockVertexBuffer();
			}

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntPlayerParts][nCntWeapon].dwNumMat; nCntMat++)
			{
				//�e�N�X�`���t�@�C�������݂���
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_apTexturePlayer[nCntPlayerParts][nCntWeapon][nCntMat]);
				}
			}
		}

		//���[�V�������̓ǂݍ���
		LoadMotion(nCntWeapon);
	}

	//=================================
	// �e���x���A�b�v�ɕK�v�Ȍo���l��
	//=================================
	for (int nCntLevelUpPoint = 0; nCntLevelUpPoint < MAX_LEVEL; nCntLevelUpPoint++)
	{
		g_nDecideLevelUpPoint[nCntLevelUpPoint] = 0;
	}

	//=================================
	// ���x���A�b�v�K�v�o���l���菈��
	//=================================
	FILE* pFile;//�t�@�C���|�C���^
	char aString[100] = {};//�������ǂݍ��ނ��߂̕ϐ�
	int nLevelNum = 0;//�o���l�̗ʂ����߂Ă��郌�x���ԍ�

	pFile = fopen(LEVEL_TXT, "r");

	if (pFile != NULL)//�t�@�C�����J���Ă��邩�ǂ���
	{
		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (aString[0] == '#')
			{
				fgets(&aString[0], 100, pFile);
				continue;
			}
			else if (strcmp(&aString[0], "ENDSCRIPT") == 0)
			{
				fclose(pFile);
				break;//�s�̍Ō�ł��̕�����𔻒肳���邱�Ƃɂ��A���[�v���I��点��
			}
			else if (strcmp(&aString[0], "LEVELSET") == 0 && nLevelNum < MAX_LEVEL)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);
					if (aString[0] == '#')
					{
						fgets(&aString[0], 100, pFile);
						continue;
					}
					else  if (strcmp(&aString[0], "EXP") == 0)
					{
						fscanf(pFile, "%d", &g_nDecideLevelUpPoint[nLevelNum]);//�C���f�b�N�X�̎�ނ����߂�
					}
					else if (strcmp(&aString[0], "ENDLEVELSET") == 0)
					{
						nLevelNum++;
						break;
					}
				}
			}
		}
	}

	//�ϐ�
	g_nDelay = 0;//�f�B���C�J�E���g
	g_nDelay2 = 0;//�f�B���C�J�E���g
}

//=======================================================================================================================================================================================================
// �I������
//=======================================================================================================================================================================================================
void UninitPlayer(void)
{
	for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
	{
		for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
		{
			//���b�V���̔j��
			if (g_aPlayer.aModel[nCntParts][nCntWeapon].pMesh != NULL)
			{
				g_aPlayer.aModel[nCntParts][nCntWeapon].pMesh->Release();
				g_aPlayer.aModel[nCntParts][nCntWeapon].pMesh = NULL;
			}
			//�}�e���A���̔j��
			if (g_aPlayer.aModel[nCntParts][nCntWeapon].pBuffMat != NULL)
			{
				g_aPlayer.aModel[nCntParts][nCntWeapon].pBuffMat->Release();
				g_aPlayer.aModel[nCntParts][nCntWeapon].pBuffMat = NULL;
			}

			//�e�N�X�`���̔j��
			for (int nCntMat = 0; nCntMat < MAX_PLAYERMAT; nCntMat++)
			{
				if (g_apTexturePlayer[nCntParts][nCntWeapon][nCntMat] != NULL)
				{
					g_apTexturePlayer[nCntParts][nCntWeapon][nCntMat]->Release();
					g_apTexturePlayer[nCntParts][nCntWeapon][nCntMat] = NULL;
				}
			}
		}
	}

}

//=======================================================================================================================================================================================================
// �X�V����
//=======================================================================================================================================================================================================
void UpdatePlayer(void)
{
	Camera* pCamera = GetCamera();	//�J�����̏����擾
	Enemy* pEnemy = GetEnemy();		//�G�̏����擾
	Boss* pBoss = GetBoss();		//�{�X�̏����擾
	Wall* pWall = GetWall();		//�ǂ̏����擾

	if (GetMode() == MODE_GAME)
	{//�Q�[����ʂ̏ꍇ
		g_nDelay2++;//�Q�[�����[�h�̎������J�E���g������
	}

	if (GetMode() != MODE_TITLE && pCamera->nMode == CAMERAMODE_REVOLUTION)
	{//�^�C�g����ʂł͂Ȃ��ꍇ

		//============================
		// 1f�O�̏�Ԃ�ۑ�����
		//============================
		g_aPlayer.nStateOld = g_aPlayer.State;

		//========================================================================
		// �v���C���[�̃p�[�c�̈ʒu���擾���邽�߁A�}�g���b�N�X�̌v�Z���J�n����
		//========================================================================
		CalculateMatrixPlayer();

		g_nDelay++;//�f�B���C�J�E���g�i���t���[���Ɉ�񔭓������������Ȃǂ����鎞�Ɏg���Ă���B�j

		//=========================================================================================
		//�ŏ��̃t���[���̎������A�ǂ̃��f���̏�ɏ���Ă��邩������Ȃ��̂ŁA�l��-1�ɂ���
		//=========================================================================================
		if (g_nDelay == 1)
		{
			g_aPlayer.nNumLandingModel = -1;
			g_aPlayer.bPossibleDodge = true;

			//�e�̐ݒ�
			g_aPlayer.nIdxShadow = SetShadow();

			g_aPlayer.nIdxRaderDisplay = SetRaderDisplay(RADERDISPLAY00_PLAYER);
		}

		if (g_aPlayer.pos.y <= 0.0f)
		{//�n�ʂɂ�����
			g_aPlayer.pos.y = 0.0f;
			g_aPlayer.bIsLanding = true;
		}

		if (GetKeyboardTrigger(DIK_F2))
		{//F2�������ꂽ�ꍇ
			g_aPlayer.nMode++;

			if (g_aPlayer.nMode > PLAYERMODE_EDIT)
			{
				g_aPlayer.nMode = PLAYERMODE_NORMAL;
			}
		}

		//=======================================================================
		// �v���C���[�̓������̂����߂�i���ʃ��[�h�ƃG�f�B�b�g���[�h�j
		//=======================================================================
		if (g_aPlayer.nMode == PLAYERMODE_NORMAL)
		{//���ʃ��[�h�̏ꍇ
			if (g_aPlayer.bIsLanding == false)
			{//�v���C���[���n�ʂɗ����Ă��Ȃ��ꍇ
				g_aPlayer.move.y += -1.2f;
			}
			else
			{//�v���C���[���n�ʂɗ����Ă���ꍇ
				g_aPlayer.move.y = 0.0f;

				if (g_aPlayer.bIsLanding == true)
				{
					Model* pModel = GetModel();		//���f���̏����擾

					//===================================================
					//���t�g�̏�ɏ�葱����
					//===================================================
					for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
					{
						if (pModel->bUse == true && g_aPlayer.nNumLandingModel == nCntModel)
						{
							g_aPlayer.pos.y = pModel->pos.y + pModel->vtxMax.y;
						}
					}
				}
			}
		}
		else if (g_aPlayer.nMode == PLAYERMODE_EDIT)
		{//�G�f�B�b�g���[�h�̏ꍇ
			if (GetKeyboardPress(DIK_UP))
			{//���L�[�������ꂽ�ꍇ
				g_aPlayer.pos.y += 5.0f;
			}
			else if (GetKeyboardPress(DIK_DOWN))
			{//���L�[�������ꂽ�ꍇ
				g_aPlayer.pos.y += -5.0f;
			}
		}

		//===========================================================================================
		// �v���C���[�̈ړ�����
		//===========================================================================================
		if (pCamera->nMode == CAMERAMODE_REVOLUTION)
		{
			if (!g_aPlayer.bCompulsionDodge)
			{//�����Ԃ���Ȃ��Ƃ������ړ��̓��͂̎󂯕t�����s���B
				g_aPlayer.fMoveX = 0.0f;
				g_aPlayer.fMoveZ = 0.0f;
				if (GetKeyboardPress(DIK_W) || (float)(GetStick().abAnglePress[STICKTYPE_LEFT][STICKANGLE_UP]) > 0.1f)
				{
					g_aPlayer.fMoveZ = 1.0f;
				}
				else if (GetKeyboardPress(DIK_S) || (float)(GetStick().abAnglePress[STICKTYPE_LEFT][STICKANGLE_DOWN]) > 0.1f)
				{
					g_aPlayer.fMoveZ = -1.0f;
				}

				if (GetKeyboardPress(DIK_D) || (float)(GetStick().abAnglePress[STICKTYPE_LEFT][STICKANGLE_RIGHT]) > 0.1f)
				{
					g_aPlayer.fMoveX = 1.0f;
				}
				else if (GetKeyboardPress(DIK_A) || (float)(GetStick().abAnglePress[STICKTYPE_LEFT][STICKANGLE_LEFT]) > 0.1f)
				{
					g_aPlayer.fMoveX = -1.0f;
				}
			}

			//============================
			// �ړ��{�^���������Ă�����
			//============================
			if (g_aPlayer.fMoveX != 0.0f || g_aPlayer.fMoveZ != 0.0f)
			{
				g_aPlayer.bMove = true;//�ړ����
			}
			else
			{
				g_aPlayer.bMove = false;//�ҋ@���
			}

			//================================
			//�������
			//================================
			float fDodgeSpeed = 1.0f;

			if (g_aPlayer.bMove == true)
			{
				if (GetJoypadTrigger(JOYKEY_A) == true && g_aPlayer.bPossibleDodge == true && g_aPlayer.bCompulsionHealGround == false &&
					g_aPlayer.bCompulsionHealWall == false)
				{
					PlaySound(SOUND_LABEL_SE_DODGE_000);
					g_aPlayer.bCompulsionDodge = true;
					g_aPlayer.bPossibleDodge = false;
				}
			}

			if (g_aPlayer.bCompulsionDodge == true)
			{
				for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
				{
					g_aPlayer.aModel[19][nCntWeapon].bUse = false;
				}

				g_aPlayer.nDodgeTime++;
				fDodgeSpeed = 5.0f;

				if (g_aPlayer.nDodgeTime >= 10)
				{
					g_aPlayer.nDodgeTime = 0;
					g_aPlayer.bCompulsionDodge = false;
				}
			}
			else
			{
				for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
				{
					g_aPlayer.aModel[19][nCntWeapon].bUse = true;
				}
			}

			if (g_aPlayer.bCompulsionDodge == false && g_aPlayer.bPossibleDodge == false)
			{
				g_aPlayer.nDodgeCoolTime++;

				if (g_aPlayer.nDodgeCoolTime >= 80)
				{
					g_aPlayer.bPossibleDodge = true;
					g_aPlayer.nDodgeCoolTime = 0;
				}
			}

			//================================
			// �ړ������J�n
			//================================
			if (g_aPlayer.bMove == true && g_aPlayer.bDamageState == false && g_aPlayer.bCompulsionHealWall == false && g_aPlayer.bCompulsionHealGround == false)
			{//�i�_���[�W�A�Ǐ[�d��ԁA���[�d�j��Ԃ���Ȃ���Έړ����J�n����B
				g_aPlayer.State = PLAYERSTATE_MOVE;

				//�ړ��������t���[����t
				g_aPlayer.nMoveFrame++;

				g_aPlayer.move.x = sinf(atan2f(g_aPlayer.fMoveX, g_aPlayer.fMoveZ) + pCamera->rot.y) * g_aPlayer.fSpeed * fDodgeSpeed;
				g_aPlayer.move.z = cosf(atan2f(g_aPlayer.fMoveX, g_aPlayer.fMoveZ) + pCamera->rot.y) * g_aPlayer.fSpeed * fDodgeSpeed;

				g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.5f;
				g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * 0.5f;

				if (g_aPlayer.nMoveFrame % 20 == 0)
				{
					/*PlaySound(SOUND_LABEL_SE_TIBIROBOWALK);*/
				}

				g_aPlayer.targetrot.y = atan2f(g_aPlayer.fMoveX, g_aPlayer.fMoveZ) + pCamera->rot.y + D3DX_PI;

				g_aPlayer.rot.y += ((g_aPlayer.targetrot.y) - g_aPlayer.rot.y) * 0.5f;
			}
			else
			{
				g_aPlayer.State = PLAYERSTATE_NUTORAL;
				g_aPlayer.move.x = 0.0f;
				g_aPlayer.move.z = 0.0f;
			}
		}

		//============================
		//�{�X�Ƀx�N�g����������
		//============================
		g_aPlayer.fVXaim = pBoss->pos.x - g_aPlayer.pos.x;
		g_aPlayer.fVZaim = pBoss->pos.z - g_aPlayer.pos.z;
		g_aPlayer.fVaim = atan2f(g_aPlayer.fVXaim, g_aPlayer.fVZaim);

		g_aPlayer.posOld = g_aPlayer.pos;
		g_aPlayer.pos += g_aPlayer.move;

		g_aPlayer.nNumLandingModel = -1;//���̎��_�ł́A���f���̏�ɏ���Ă��邩������Ȃ��̂ŁA�ԍ���-1�ɂ���

		g_aPlayer.bIsLanding = false;//�n�ʂɂ��Ă��邩�ǂ���	

		g_aPlayer.bIsLanding = bCollisionModel(&g_aPlayer.pos, &g_aPlayer.posOld, g_aPlayer.Size, g_aPlayer.nNumLandingModel);

		if (g_aPlayer.nMoveFrame  == 20)
		{
			SubBattery(1);
			g_aPlayer.nMoveFrame = 0;
		}

		//=================================================
		//�����I�ɉ����Ԃɂ��鏈���i�ړ����D��x���j
		//=================================================
		if (g_aPlayer.bCompulsionDodge == true)
		{
			g_aPlayer.State = PLAYERSTATE_DODGE;
		}

		//=============================
		// �U���{�^�����͏���
		//=============================
		if (g_aPlayer.bDamageState == false)
		{
			if (g_aPlayer.State != PLAYERSTATE_HEALGROUND && g_aPlayer.State != PLAYERSTATE_HEALWALL
				&& g_aPlayer.bCompulsionHealWall == false && g_aPlayer.bCompulsionHealGround == false)
			{
			    //���U��
				if (GetKeyboardTrigger(DIK_K) == true || GetJoypadTrigger(JOYKEY_X) == true)
				{
					g_aPlayer.State = PLAYERSTATE_ATTACK;//�U�����
					g_aPlayer.nWeapon = 0;//��������
					g_aPlayer.nAttackCount += 1;//�U���J�E���g�{�P
					g_aPlayer.bCompulsionAttack = true;//�����I�ɍU����Ԃɂ���

					if (g_aPlayer.nAttackTime == 0)
					{
						SubBattery(1);
					}
				}
				//�u�[�������U��
				else if (GetJoypadTrigger(JOYKEY_Y) == true && g_aPlayer.aBoomerangInfo.bPossibleBoomerang == true)
				{
					g_aPlayer.State = PLAYERSTATE_ATTACK;//�U�����
					g_aPlayer.nWeapon = 1;//�u�[������������
					g_aPlayer.bCompulsionAttack = true;//�����I�ɍU����Ԃɂ���
					g_aPlayer.aModel[19][g_aPlayer.nWeapon].bUse = false;//������\��
				}
				//�X�s�[�J�[�U��
				else if (GetJoypadTrigger(JOYKEY_B) == true)
				{
					g_aPlayer.State = PLAYERSTATE_ATTACK;//�U�����
					g_aPlayer.nWeapon = 2;//�X�s�[�J�[������
					g_aPlayer.bCompulsionAttack = true;//�����I�ɍU����Ԃɂ���

					if (g_aPlayer.nAttackTime == 0)
					{
						PlaySound(SOUND_LABEL_SE_SPEAKERWAVE_000);
					}
				}
			}
		}

		//======================================
		//�U��������Ԃ�������
		//======================================
		if (g_aPlayer.bCompulsionAttack)
		{//�U����Ԃ̏ꍇ
			g_aPlayer.State = PLAYERSTATE_ATTACK;
		}
		if (g_aPlayer.State == PLAYERSTATE_ATTACK)
		{
			g_aPlayer.nAttackTime++;
		}

	//============================================================================
	//�v���C���[�񕜁i�[�d�j����
	//============================================================================
		Model* pModel = GetModel();		//���f���̏����擾

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
		{
			if (pModel->bUse == true && pModel->nType == 41)
			{//�ǃR���Z���g�̃��f�����g�p���Ă�����

				//===========================================
				//�v���C���[�ƕǃR���Z���g�Ƃ̋��������
				//===========================================
				float fLength = sqrtf((g_aPlayer.pos.x - pModel->pos.x) * (g_aPlayer.pos.x - pModel->pos.x) +
					(g_aPlayer.pos.y - pModel->pos.y) * (g_aPlayer.pos.y - pModel->pos.y) +
					(g_aPlayer.pos.z - pModel->pos.z) * (g_aPlayer.pos.z - pModel->pos.z) / 3.0f);

				if (fLength <= 100.0f && GetJoypadTrigger(JOYKEY_A) == true && g_aPlayer.bCompulsionHealWall == false)
				{//�͈͓���A�{�^���������ꂽ�ꍇ
					g_aPlayer.bCompulsionHealWall = true;
					g_aPlayer.pos = D3DXVECTOR3(pModel->pos.x + sinf(pModel->rot.y + D3DX_PI) * 37.0f, 10.0f, pModel->pos.z + cosf(pModel->rot.y + D3DX_PI) * 37.0f);
					g_aPlayer.HealWallSaveRot.y = pModel->rot.y + D3DX_PI;
					g_aPlayer.nNumUseWallOutlet = nCntModel;//�g�p���Ă���ǃR���Z���g�̃��f���ԍ����L�^����
					g_aPlayer.bChargingFlag = true;//�Ǐ[�d���̃t���O��true�ɂ���
				}
			}
			if (pModel->bUse == true && pModel->nType == 19)
			{//���R���Z���g�̃��f�����g�p���Ă�����

				//===========================================
				//�v���C���[�Ə��R���Z���g�Ƃ̋��������
				//===========================================
				float fLength = sqrtf((g_aPlayer.pos.x - pModel->pos.x) * (g_aPlayer.pos.x - pModel->pos.x) +
					(g_aPlayer.pos.y - pModel->pos.y) * (g_aPlayer.pos.y - pModel->pos.y) +
					(g_aPlayer.pos.z - pModel->pos.z) * (g_aPlayer.pos.z - pModel->pos.z) / 3);

				if (fLength <= 100.0f && GetJoypadTrigger(JOYKEY_A) == true && g_aPlayer.bCompulsionHealGround == false)
				{//�͈͓���A�{�^���������ꂽ�ꍇ
					g_aPlayer.bCompulsionHealGround = true;
					g_aPlayer.pos = D3DXVECTOR3(pModel->pos.x + sinf(pModel->rot.y + D3DX_PI) * 30.0f + 10.0f, 0.0f, pModel->pos.z + cosf(pModel->rot.y + D3DX_PI) * 30.0f);
					g_aPlayer.HealGroundSaveRot.y = pModel->rot.y + D3DX_PI;
					g_aPlayer.nNumUseGroundOutlet = nCntModel;//�g�p���Ă��鏰�R���Z���g�̃��f���ԍ����L�^����
					g_aPlayer.bChargingGroundFlag = true;//���[�d���̃t���O��true�ɂ���
				}
			}
		}

		//==================================================
		//�X�e�[�g�����t���O��true�̊Ԃ́A���̏�ԂɌŒ�
		//==================================================
		if (g_aPlayer.bCompulsionHealWall)
		{
			g_aPlayer.State = PLAYERSTATE_HEALWALL;//�ǉ񕜏��
			g_aPlayer.rot.y = g_aPlayer.HealWallSaveRot.y;
			g_aPlayer.aModel[19][g_aPlayer.nWeapon].bUse = false;//������\��
		}

		if (g_aPlayer.bCompulsionHealGround)
		{
			g_aPlayer.State = PLAYERSTATE_HEALGROUND;//���񕜏��
			g_aPlayer.rot.y = g_aPlayer.HealGroundSaveRot.y;
			g_aPlayer.aModel[19][g_aPlayer.nWeapon].bUse = false;//������\��
		}

		//=======================================================================================================================

		//===============================================================================================================================================
		// �U���̓����蔻�菈��(�U���{�^�����������̏��ł��ƁA�{�^�������Ă��鎞�����������Ȃ��̂ŁA���ݎg�p���Ă��镐�킪�ǂꂩ�Ŕ��f�������̂������B
		//===============================================================================================================================================

		//================================================
		// �v���C���[�̎g�p���Ă��镐�킪�u���v��������
		//================================================
		D3DXVECTOR3 CollisionVertex = D3DXVECTOR3(sinf(-D3DX_PI * g_aPlayer.rot.y) * 35.0f + g_aPlayer.aModel[18][0].PartsPos.x,
			g_aPlayer.aModel[18][0].PartsPos.y,
			cosf(-D3DX_PI * g_aPlayer.rot.y) * 35.0f + g_aPlayer.aModel[18][0].PartsPos.z);

		g_aPlayer.aModel[18][0].PartsPos = CollisionVertex;

		//==================================
		//�[�d��ԂȂ��΍U�������Ȃ�
		//==================================
		if (g_aPlayer.State != PLAYERSTATE_HEALGROUND && g_aPlayer.State != PLAYERSTATE_HEALWALL)
		{
			if (g_aPlayer.State == PLAYERSTATE_ATTACK && g_aPlayer.nWeapon == 0)
			{
				if (g_aPlayer.nAttackTime == 10 || g_aPlayer.nAttackTime == 26)
				{
					PlaySound(SOUND_LABEL_SE_SWORDSWING_000);
				}

				//========================
				//�G�Ƃ̓����蔻��
				//========================
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse)
					{//�G���g�p����Ă���ꍇ
						float fLength = sqrtf((g_aPlayer.aModel[18][0].PartsPos.x - pEnemy->pos.x) * (g_aPlayer.aModel[18][0].PartsPos.x - pEnemy->pos.x) +
							(g_aPlayer.aModel[18][0].PartsPos.z - pEnemy->pos.z) * (g_aPlayer.aModel[18][0].PartsPos.z - pEnemy->pos.z) +
							(g_aPlayer.aModel[18][0].PartsPos.y - pEnemy->pos.y) * (g_aPlayer.aModel[18][0].PartsPos.y - pEnemy->pos.y)) / 3;

						if (fLength <= 35.0f)
						{
							if ((g_aPlayer.nAttackTime >= 7 && g_aPlayer.nAttackTime <= 17) || (g_aPlayer.nAttackTime >= 20 && g_aPlayer.nAttackTime <= 34))
							{
								SetDamageEnemy(g_aPlayer.aSwordInfo.nSwordPower, nCntEnemy, PLAYERATTACK00_SWORD, 0);
							}
						}
					}
				}

				//==========================
				//�{�X�Ƃ̓����蔻��
				//==========================
				if (pBoss->bUse)
				{//�{�X���g�p����Ă���ꍇ
					float fLengthBoss = sqrtf((g_aPlayer.aModel[18][0].PartsPos.x - pBoss->pos.x) * (g_aPlayer.aModel[18][0].PartsPos.x - pBoss->pos.x) +
						(g_aPlayer.aModel[18][0].PartsPos.z - pBoss->pos.z) * (g_aPlayer.aModel[18][0].PartsPos.z - pBoss->pos.z) +
						(g_aPlayer.aModel[18][0].PartsPos.y - pBoss->pos.y) * (g_aPlayer.aModel[18][0].PartsPos.y - pBoss->pos.y)) / 3;

					if (fLengthBoss <= 35.0f)
					{
						if ((g_aPlayer.nAttackTime >= 7 && g_aPlayer.nAttackTime <= 17) || (g_aPlayer.nAttackTime >= 20 && g_aPlayer.nAttackTime <= 34))
						{
							SetDamageBoss(g_aPlayer.aSwordInfo.nSwordPower, PLAYERATTACK00_SWORD, 0);
						}
					}
				}

				//==============================
				//�_�~�[�Ƃ̓����蔻��
				//==============================
				Dummy* pDummy = GetDummy();	//�_�~�[�̏����擾

				if (pDummy->bUse)
				{//�_�~�[���g�p����Ă���ꍇ
					float fLengthDummy = sqrtf((g_aPlayer.aModel[18][0].PartsPos.x - pDummy->pos.x) * (g_aPlayer.aModel[18][0].PartsPos.x - pDummy->pos.x) +
						(g_aPlayer.aModel[18][0].PartsPos.z - pDummy->pos.z) * (g_aPlayer.aModel[18][0].PartsPos.z - pDummy->pos.z) +
						(g_aPlayer.aModel[18][0].PartsPos.y - pDummy->pos.y) * (g_aPlayer.aModel[18][0].PartsPos.y - pDummy->pos.y)) / 3;

					if (fLengthDummy <= 35.0f)
					{
						if ((g_aPlayer.nAttackTime >= 7 && g_aPlayer.nAttackTime <= 17) || (g_aPlayer.nAttackTime >= 20 && g_aPlayer.nAttackTime <= 34))
						{
							DamageDummy(g_aPlayer.aSwordInfo.nSwordPower, PLAYERATTACK00_SWORD);
						}
					}
				}
			}


			//========================================================
			// �v���C���[�̑������Ă��镐�킪�u�u�[�������v��������
			//========================================================
			if (g_aPlayer.aBoomerangInfo.nBoomerangPiece == g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece)
			{
				g_aPlayer.aBoomerangInfo.bPossibleBoomerang = true;
			}
			else
			{
				g_aPlayer.aBoomerangInfo.bPossibleBoomerang = false;
			}

			if (g_aPlayer.State == PLAYERSTATE_ATTACK && g_aPlayer.nWeapon == 1 && g_aPlayer.aBoomerangInfo.bPossibleBoomerang == true)
			{//�����Ă���u�[���������ő吔�Ɠ������ɂȂ�����U���ł���悤�ɂ���B�i��C�ɓ�����j
				PlaySound(SOUND_LABEL_SE_BOOMERANGTHROW_000);
				SubBattery(1);//�U�����邽�сA�d�͂��P����

				g_aPlayer.aBoomerangInfo.nBoomerangPiece = 0;//�S�Ẵu�[����������C�ɓ�����
				float fRot = 0.0f;//�����𑝂₷
				for (int nCntBoomerang = 0; nCntBoomerang < g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece; nCntBoomerang++)
				{
					if (nCntBoomerang % 2 == 0)
					{
						SetAttackModel(ATTACKMODEL00_BOOMERANG, 600, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + 50.0f, g_aPlayer.pos.z), D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI + fRot) * 20.0f, 0.0f, cosf(g_aPlayer.rot.y + D3DX_PI + fRot) * 20.0f),
							NULL_VECTOR3, g_aPlayer.aBoomerangInfo.BoomerangScale, 0.0f, g_aPlayer.rot.y + D3DX_PI, 0.15f, g_aPlayer.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					else
					{
						SetAttackModel(ATTACKMODEL00_BOOMERANG, 600, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + 50.0f, g_aPlayer.pos.z), D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI - fRot) * 20.0f, 0.0f, cosf(g_aPlayer.rot.y + D3DX_PI - fRot) * 20.0f),
							NULL_VECTOR3, g_aPlayer.aBoomerangInfo.BoomerangScale, 0.0f, g_aPlayer.rot.y + D3DX_PI, 0.15f, g_aPlayer.pos, ATTACKMODELBEHAVIOR01_ANOTHER);
					}
					if (nCntBoomerang % 2 == 0)
					{//������������
						fRot += 0.1f;
					}
				}

			}

			//===============================================================
			// �v���C���[�̑������Ă��镐�킪�u�X�s�[�J�[�v��������
			//===============================================================
			if (g_aPlayer.State == PLAYERSTATE_ATTACK && g_aPlayer.nWeapon == 2)
			{
				float fSpace = 0.5f;
				float fWide = 0.5f;

				if (g_aPlayer.nAttackTime == 2)
				{
					SubBattery(g_aPlayer.aSpeakerInfo.nPowerConsumption);//�X�s�[�J�[�̓d�͏���ʕ��A�o�b�e���[����

					for (int nCntAttack = 0; nCntAttack < 3; nCntAttack++)
					{
						SetAttackModel(ATTACKMODEL01_WAVE, 30, D3DXVECTOR3(g_aPlayer.aModel[18][2].PartsPos.x, g_aPlayer.aModel[18][2].PartsPos.y, g_aPlayer.aModel[18][2].PartsPos.z),
							D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI - fSpace + (fSpace * nCntAttack)) * 10.0f, 0.0f, cosf(g_aPlayer.rot.y + D3DX_PI - fSpace + (fSpace * nCntAttack)) * 10.0f),
							D3DXVECTOR3(0.0f, g_aPlayer.rot.y - fWide + (fWide * nCntAttack), 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.3f, NULL_VECTOR3, 0);
					}
				}

				if (g_aPlayer.nAttackTime % 10 == 0)
				{
					for (int nCntAttack = 0; nCntAttack < 3; nCntAttack++)
					{
						SetAttackModel(ATTACKMODEL01_WAVE, 30, D3DXVECTOR3(g_aPlayer.aModel[18][2].PartsPos.x, g_aPlayer.aModel[18][2].PartsPos.y, g_aPlayer.aModel[18][2].PartsPos.z),
							D3DXVECTOR3(sinf(g_aPlayer.rot.y + D3DX_PI - fSpace + (fSpace * nCntAttack)) * 10.0f, 0.0f, cosf(g_aPlayer.rot.y + D3DX_PI - fSpace + (fSpace * nCntAttack)) * 10.0f),
							D3DXVECTOR3(0.0f, g_aPlayer.rot.y - fWide + (fWide * nCntAttack), 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.3f, NULL_VECTOR3, 0);
					}
				}
			}
		}

		//============================================================================================================================================

		//============================================
		//�_���[�W���󂯂����Ƀ_���[�W��Ԃɂ��鏈��
		//============================================
		if (g_aPlayer.bDamageState == true)
		{
			g_aPlayer.State = PLAYERSTATE_DAMAGE;

			g_aPlayer.nDamageCountTime++;

			if (g_aPlayer.nDamageCountTime == 60)
			{
				g_aPlayer.nDamageCountTime = 0;
				g_aPlayer.bDamageState = false;
			}
		}

		//=================================================
		//�v���C���[�̍s���͈͐���
		//=================================================
		for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
		{
			//1����(��)
			if (g_aPlayer.pos.z > pWall->pos.z - COLLISION_WALL && nCntWall == 0)
			{
				g_aPlayer.pos.z = pWall->pos.z - COLLISION_WALL;
			}
			//2����(�E)
			if (g_aPlayer.pos.x > pWall->pos.x - COLLISION_WALL && nCntWall == 1)
			{
				g_aPlayer.pos.x = pWall->pos.x - COLLISION_WALL;
			}
			//3����(��)
			if (g_aPlayer.pos.z < pWall->pos.z + COLLISION_WALL && nCntWall == 2)
			{
				g_aPlayer.pos.z = pWall->pos.z + COLLISION_WALL;
			}
			//4����(��)
			if (g_aPlayer.pos.x < pWall->pos.x + COLLISION_WALL && nCntWall == 3)
			{
				g_aPlayer.pos.x = pWall->pos.x + COLLISION_WALL;
			}
		}

		//���x���A�b�v����
		LevelPlayer();

		//�X�e�[�^�X����
		StatusPlayer();

		//�e�̈ʒu��ݒ�
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);

		if (GetMode() != MODE_TITLE && GetMode() != MODE_TUTORIAL)
		{
			if (g_nDelay2 == 1)
			{
				g_aPlayer.nIdxRaderDisplay = SetRaderDisplay(RADERDISPLAY00_PLAYER);
			}

			//���[�_�[�f�B�X�v���C�̈ʒu��ݒ�
			SetPositionRaderDisPlay(g_aPlayer.nIdxRaderDisplay, g_aPlayer.pos, -g_aPlayer.rot.y + D3DX_PI);
		}
	}

	//============================================
	//���G��Ԃ̏���
	//===========================================
	if (g_aPlayer.bInvincible)
	{
		g_aPlayer.nInvincibleCnt++;

		if (g_aPlayer.nInvincibleCnt % 2 == 0)
		{
			for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
			{
				for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
				{
					for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nCntWeapon].dwNumMat; nCntMat++)
					{
						g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].a = 1.0f;
					}
				}
			}
		}
		else
		{
			for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
			{
				for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
				{
					for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nCntWeapon].dwNumMat; nCntMat++)
					{
						g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].a = 0.0f;
					}
				}
			}
		}

		if (g_aPlayer.nInvincibleCnt == 120)
		{
			for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
			{
				for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
				{
					for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nCntWeapon].dwNumMat; nCntMat++)
					{
						g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].a = 1.0f;
					}
				}
			}

			g_aPlayer.bInvincible = false;
			g_aPlayer.nInvincibleCnt = 0;
		}
	}


	//===============================================================
	//�v���C���[�̏�Ԃ��ς�������ɃJ�E���g�n���O�ɂ���
	//===============================================================

	//�̂̃��[�V��������
	PlayerMotion(g_aPlayer.nWeapon);

	//�r�̃��[�V��������
	PlayerMotion2(g_aPlayer.nWeapon);

	//================================================================================
	//��U���i�o���b�g�Ȃǁj�����������ɁA�v���C���[�̐F����u�ԐF�ɕς���
	//================================================================================
	if (g_aPlayer.bWeakDamageReaction == true)
	{
		g_aPlayer.nWeakDamageReactionTime++;

		for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
		{
			for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
			{
				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nCntWeapon].dwNumMat; nCntMat++)
				{
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].r = 1.0f;
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].g = 0.0f;
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].b = 0.0f;
				}
			}
		}

		if (g_aPlayer.nWeakDamageReactionTime >= 10)
		{
			g_aPlayer.bWeakDamageReaction = false;
			g_aPlayer.nWeakDamageReactionTime = 0;
		}
	}
	else
	{
		for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
		{
			for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
			{
				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nCntWeapon].dwNumMat; nCntMat++)
				{
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].r = g_aPlayer.aModel[nCntParts][nCntWeapon].FormarDiffUse[nCntMat].r;
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].g = g_aPlayer.aModel[nCntParts][nCntWeapon].FormarDiffUse[nCntMat].g;
					g_aPlayer.aModel[nCntParts][nCntWeapon].DiffUse[nCntMat].b = g_aPlayer.aModel[nCntParts][nCntWeapon].FormarDiffUse[nCntMat].b;
				}
			}
		}
	}

	//=======================================================
	//�u�[�������U�����̎��ɁA����̕`�����������
	//=======================================================
	if (g_aPlayer.State == PLAYERSTATE_ATTACK && g_aPlayer.nWeapon == PLAYERATTACK01_BOOMERANG)
	{
		g_aPlayer.aModel[19][1].bUse = false;
	}

	//�R���{����
	g_aPlayer.nCntCombo--;

	//�R���{���e�J�E���g��0�ɂȂ����ꍇ
	if (g_aPlayer.nCntCombo <= 0)
	{
		//0�ȉ��ɂȂ�Ȃ��悤�ɒ���
		g_aPlayer.nCntCombo = 0;

		//�R���{�{�������Z�b�g
		g_aPlayer.fCombo = 1.0f;
	}
}

//=======================================================================================================================================================================================================
// �`�揈��
//=======================================================================================================================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;						//�}�e���A���̃f�[�^�ւ̃|�C���^

	//�g�p���Ă���ꍇ
	if (g_aPlayer.bUse)
	{//�v���C���[���g�p����Ă���ꍇ
		//�v���C���[�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

		//�v���C���[�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

		//�v���C���[�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

		//�������Ă��镐��̔ԍ���ۑ�
		int nWeapon = g_aPlayer.nWeapon;

		for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
		{
			if (g_aPlayer.aModel[nCntParts][nWeapon].bUse)
			{//�p�[�c���g�p����Ă���ꍇ

				//�e�p�[�c���f���̃��[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld);

				//�e�p�[�c���f���̌����𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCntParts][nWeapon].rot.y, g_aPlayer.aModel[nCntParts][nWeapon].rot.x, g_aPlayer.aModel[nCntParts][nWeapon].rot.z);
				D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxRot);

				//�e�p�[�c���f���̈ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCntParts][nWeapon].pos.x, g_aPlayer.aModel[nCntParts][nWeapon].pos.y, g_aPlayer.aModel[nCntParts][nWeapon].pos.z);
				D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxTrans);

				//�e�̔ԍ���ۑ�
				int nParent = g_aPlayer.aModel[nCntParts][nWeapon].nIdxModelParent;

				//�e�̃}�g���b�N�X�𔽉f
				if (nParent != -1)
				{//�e�̔ԍ������ꍇ
					mtxParent = g_aPlayer.aModel[nParent][nWeapon].mtxWorld;//�e���f���̃}�g���b�N�X
				}
				else
				{//�e�̔ԍ��������Ȃ��ꍇ
					mtxParent = g_aPlayer.mtxWorld;	//�v���C���[�̃}�g���b�N�X
				}

				//�e�̃}�g���b�N�X���|�����킹��
				D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxParent);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld);

				//���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts][nWeapon].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts][nWeapon].dwNumMat; nCntMat++)
				{
					//===============================
					//���݂̐F�𔽉f������
					//===============================
					pMat[nCntMat].MatD3D.Diffuse = g_aPlayer.aModel[nCntParts][nWeapon].DiffUse[nCntMat];

					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					if (nWeapon == 1)
					{//�u�[�������̏ꍇ
						//�e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, g_apTexturePlayer[nCntParts][nWeapon][nCntMat]);
					}
					else
					{
						//�e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, NULL);
					}

					//���f��(�p�[�c)�̕`��
					g_aPlayer.aModel[nCntParts][nWeapon].pMesh->DrawSubset(nCntMat);
				}

				//�ۑ����Ă����}�e���A����߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//=======================================================================================================================================================================================================
// �v���C���[�̍\���̂̏����擾����B
//=======================================================================================================================================================================================================
Player* GetPlayer(void)
{
	return &g_aPlayer;
}

//=======================================================================================================================================================================================================
// �v���C���[�̃p�[�c���̓ǂݍ���
//=======================================================================================================================================================================================================
void LoadPlayer(void)
{
	int nCntParts = 0;
	char aDataSearch[MAX_TEXT];

	//�t�@�C�����J��
	FILE* pFile = fopen(PLAYER_TXT, "r");

	//�t�@�C�����J���Ȃ��ꍇ
	if (pFile == NULL)
	{
		return;
	}

	//END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		int nResult = fscanf(pFile, "%s", aDataSearch);	//����

		//END_PARTS�����������ꍇ
		if (nResult == EOF)
		{//�ǂݍ��ݏI��
			fclose(pFile);
			break;
		}

		//���f���Z�b�g�����������ꍇ
		else if (!strcmp(aDataSearch, "PARTSSET"))
		{
			//���ڂ��Ƃ̃f�[�^����
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//����

				//ENDMODEL�Z�b�g�����������ꍇ
				if (!strcmp(aDataSearch, "END_PARTSSET"))
				{//���f���f�[�^�̓ǂݍ��݂��I��
					break;
				}
				//�f�[�^�̓ǂݍ���
				else if (!strcmp(aDataSearch, "POS"))
				{//�ʒu
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].pos.x);
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].pos.y);
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].pos.z);
				}
				else if (!strcmp(aDataSearch, "ROT"))
				{//����
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].rot.x);
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].rot.y);
					fscanf(pFile, "%f", &g_aPlayerInfo[nCntParts].rot.z);
				}
				else if (!strcmp(aDataSearch, "INDEX"))
				{//���
					fscanf(pFile, "%d", &g_aPlayerInfo[nCntParts].nType);
				}
				else if (!strcmp(aDataSearch, "PARENT"))
				{//�e�̔ԍ�
					fscanf(pFile, "%d", &g_aPlayerInfo[nCntParts].nIdxModelParent);
				}
			}

			nCntParts++;				//�p�[�c�������Z
			g_nCntParts = nCntParts;	//�p�[�c����ۑ�
		}

		else
		{//�X�L�b�v���ēǂݍ��݂𑱂���
			continue;
		}
	}

	//�v���C���[�̐ݒ�
	for (int nCntSet = 0; nCntSet < g_nCntParts; nCntSet++)
	{
		SetPlayerParts(g_aPlayerInfo[nCntSet].pos, g_aPlayerInfo[nCntSet].rot, g_aPlayerInfo[nCntSet].nType, g_aPlayerInfo[nCntSet].nIdxModelParent);
	}
}

//=======================================================================================================================================================================================================
// �v���C���[�̕�����̓ǂݍ���
//=======================================================================================================================================================================================================
void LoadWeapon(void)
{
	int nCntWeapon = 0;
	char aDataSearch[MAX_TEXT];

	//�t�@�C�����J��
	FILE* pFile = fopen(WEAPON_TXT, "r");

	//�t�@�C�����J���Ȃ��ꍇ
	if (pFile == NULL)
	{
		return;
	}

	//END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	//����

		//END_PARTS�����������ꍇ
		if (!strcmp(aDataSearch, "END_PARTS"))
		{//�ǂݍ��ݏI��
			fclose(pFile);
			break;
		}

		//�R�����g�A�E�g�����������ꍇ
		if (aDataSearch[0] == '#')
		{//�X�L�b�v���ēǂݍ��݂𑱂���
			continue;
		}

		//���f���Z�b�g�����������ꍇ
		if (!strcmp(aDataSearch, "PARTSSET"))
		{
			//���ڂ��Ƃ̃f�[�^����
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//����

				//ENDMODEL�Z�b�g�����������ꍇ
				if (!strcmp(aDataSearch, "END_PARTSSET"))
				{//���f���f�[�^�̓ǂݍ��݂��I��
					break;
				}
				//�f�[�^�̓ǂݍ���
				else if (!strcmp(aDataSearch, "POSx"))
				{//�ʒu��X���W
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].pos.x);
				}
				else if (!strcmp(aDataSearch, "POSy"))
				{//�ʒu��Y���W
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].pos.y);
				}
				else if (!strcmp(aDataSearch, "POSz"))
				{//�ʒu��Z���W
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].pos.z);
				}
				else if (!strcmp(aDataSearch, "ROTx"))
				{//������X���W
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].rot.x);
				}
				else if (!strcmp(aDataSearch, "ROTy"))
				{//������Y���W
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].rot.y);
				}
				else if (!strcmp(aDataSearch, "ROTz"))
				{//������Z���W
					fscanf(pFile, "%f", &g_aWeaponInfo[nCntWeapon].rot.z);
				}
				else if (!strcmp(aDataSearch, "TYPE"))
				{//���
					fscanf(pFile, "%d", &g_aWeaponInfo[nCntWeapon].nType);
				}
				else if (!strcmp(aDataSearch, "PARENT"))
				{//�e�̔ԍ�
					fscanf(pFile, "%d", &g_aWeaponInfo[nCntWeapon].nIdxModelParent);
				}
			}

			nCntWeapon++;				//�p�[�c�������Z
			g_nCntWeapon = nCntWeapon;	//�p�[�c����ۑ�
		}
	}

	//�v���C���[�̕���̐ݒ�
	for (int nCntSet = 0; nCntSet < g_nCntWeapon; nCntSet++)
	{
		SetPlayerWeapon(g_aWeaponInfo[nCntSet].pos, g_aWeaponInfo[nCntSet].rot, g_aWeaponInfo[nCntSet].nType, g_aWeaponInfo[nCntSet].nIdxModelParent, nCntSet);
	}
}

//=======================================================================================================================================================================================================
// �v���C���[�̃p�[�c���Z�b�g����
//=======================================================================================================================================================================================================
void SetPlayerParts(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nIdx)
{
	for (int nCntWeapon = 0; nCntWeapon < MAX_PLAYER_WEAPON; nCntWeapon++)
	{
		if (g_aPlayer.aModel[nType][nCntWeapon].bUse == false)
		{//�p�[�c���g�p����Ă��Ȃ��ꍇ
			g_aPlayer.aModel[nType][nCntWeapon].bUse = true;//�g�p����
			g_aPlayer.aModel[nType][nCntWeapon].pos = pos;//�ʒu
			g_aPlayer.aModel[nType][nCntWeapon].rot = rot;//����
			g_aPlayer.aModel[nType][nCntWeapon].nIdxModelParent = nIdx;//�e�̔ԍ�
		}
	}
}

//=======================================================================================================================================================================================================
// �v���C���[�̕�����Z�b�g����
//=======================================================================================================================================================================================================
void SetPlayerWeapon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nIdx, int nCnt)
{
	if (!g_aPlayer.aModel[nType][nCnt].bUse)
	{//���킪�g�p����Ă��Ȃ��ꍇ
		g_aPlayer.aModel[nType][nCnt].bUse = true;//�g�p����
		g_aPlayer.aModel[nType][nCnt].pos = pos;//�ʒu
		g_aPlayer.aModel[nType][nCnt].rot = rot;//����
		g_aPlayer.aModel[nType][nCnt].nIdxModelParent = nIdx;//�e�̔ԍ�
	}
}

//============================================================================================================================================================================================================
// ���[�V�������̓ǂݍ��ݏ���
//============================================================================================================================================================================================================
void LoadMotion(int nWeapon)
{
	int nMotion = 0;							//���[�V�����̎�ޔԍ�
	int nCntParts = 0;							//�p�[�c��
	int nCntKey[MAX_PLAYER_WEAPON][MAX_MOTION];	//���[�V�������Ƃ̃L�[��
	char aDataSearch[MAX_TEXT];					//�f�[�^�����p

	//�L�[���̏�����
	for (int nCnt = 0; nCnt < MAX_MOTION; nCnt++)
	{
		nCntKey[nWeapon][nCnt] = 0;
	}

	//�t�@�C�����J��
	FILE* pFile = fopen(MOTION_FILENAME[nWeapon], "r");

	//�t�@�C�����J���Ȃ��ꍇ
	if (pFile == NULL)
	{
		return;
	}

	while (1)
	{
		int nResult = fscanf(pFile, "%s", aDataSearch);	//����

		//���[�V�����Z�b�g�����������ꍇ
		if (!strcmp(aDataSearch, "MOTIONSET"))
		{
			//���[�V�����̎�ނ�ǂݍ���
			fscanf(pFile, "%d", &nMotion);

			//END_MOTIONSET��������܂œǂݍ��݂��J��Ԃ�
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	//����

				//END_MOTIONSET�����������ꍇ
				if (!strcmp(aDataSearch, "END_MOTIONSET"))
				{//���[�V�����ǂݍ��ݏI��
					break;
				}

				//�R�����g�A�E�g�����������ꍇ
				if (aDataSearch[0] == '#')
				{//�X�L�b�v���ēǂݍ��݂𑱂���
					continue;
				}

				//���[�v���肪���������ꍇ
				if (!strcmp(aDataSearch, "LOOP"))
				{//���[�v���邩�ǂ������擾
					fscanf(pFile, "%d", &g_aPlayerMotionSet[nWeapon][nMotion].nMotionLoop);
				}

				//�L�[�Z�b�g�����������ꍇ
				if (!strcmp(aDataSearch, "KEYSET"))
				{
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	//����

						//�R�����g�A�E�g�����������ꍇ
						if (aDataSearch[0] == '#')
						{//�X�L�b�v���ēǂݍ��݂𑱂���
							continue;
						}

						//KEY�����������ꍇ
						if (!strcmp(aDataSearch, "KEY"))
						{
							//���ڂ��Ƃ̃f�[�^����
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch);	//����

								//FRAME�����������ꍇ
								if (!strcmp(aDataSearch, "FRAME"))
								{//�t���[������ǂݍ���
									fscanf(pFile, "%d", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].nMaxFrame);
								}

								//�f�[�^�̓ǂݍ���
								else if (!strcmp(aDataSearch, "POS"))
								{//�ʒu
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].pos.x);
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].pos.y);
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].pos.z);
								}
								else if (!strcmp(aDataSearch, "ROT"))
								{//����
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].rot.x);
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].rot.y);
									fscanf(pFile, "%f", &g_aPlayerKeySet[nWeapon][nCntKey[nWeapon][nMotion]][nMotion].key[nCntParts].rot.z);
								}

								//END_KEY�Z�b�g�����������ꍇ
								else if (!strcmp(aDataSearch, "END_KEY"))
								{//���f���f�[�^�̓ǂݍ��݂��I��
									nCntParts++;	//�f�[�^�������Z
									break;
								}
							}
						}

						//END_KEYSET�����������ꍇ
						if (!strcmp(aDataSearch, "END_KEYSET"))
						{
							nCntParts = 0;
							nCntKey[nWeapon][nMotion]++;												//�L�[�������Z
							g_aPlayerMotionSet[nWeapon][nMotion].nMaxKey = nCntKey[nWeapon][nMotion];	//���v�L�[����ۑ�
							break;
						}
					}
				}
			}
		}

		//EOF�����������ꍇ
		else if (nResult == EOF)
		{//�t�@�C�������
			fclose(pFile);
			break;
		}

		//���[�V�����Z�b�g�ȊO�����������ꍇ
		else
		{//�����𑱂���
			continue;
		}
	}
}

//============================================================================================================================================================================================================
// �̂̃��[�V��������
//============================================================================================================================================================================================================
void PlayerMotion(int nWeapon)
{
	int nState = g_aPlayer.State;																									//�v���C���[�̏��
	int nCntKey = g_aPlayer.aMotionSet[nWeapon][nState].nCntKey;																	//���݂̃L�[��ۑ�
	int nNextKey = (nCntKey + 1) % g_aPlayerMotionSet[nWeapon][nState].nMaxKey;														//���̃L�[�����߂�
	float fFrame = (float)g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame / g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame;	//���݂̃t���[���ƑS�̃t���[���̊���

	//�p�[�c���Ƃɏ����X�V
	for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
	{
		if (nCntParts < 10 || nCntParts > 13)
		{
			float PlayerPosX = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.x;
			float PlayerPosY = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.y;
			float PlayerPosZ = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.z;
			float PlayerRotX = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.x;
			float PlayerRotY = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.y;
			float PlayerRotZ = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.z;

			float NextPlayerPosX = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.x;
			float NextPlayerPosY = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.y;
			float NextPlayerPosZ = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.z;
			float NextPlayerRotX = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.x;
			float NextPlayerRotY = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.y;
			float NextPlayerRotZ = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.z;

			//���̃L�[�܂ł̍��������߂�
			float fPosx = NextPlayerPosX - PlayerPosX;	// X���W
			float fPosy = NextPlayerPosY - PlayerPosY;	// Y���W
			float fPosz = NextPlayerPosZ - PlayerPosZ;	// Z���W
			float fRotx = NextPlayerRotX - PlayerRotX;	// X���̌���
			float fRoty = NextPlayerRotY - PlayerRotY;	// Y���̌���
			float fRotz = NextPlayerRotZ - PlayerRotZ;	// Z���̌���

			//�������X�V
			g_aPlayer.aModel[nCntParts][nWeapon].rot.x = g_aPlayerInfo[nCntParts].rot.x + PlayerRotX + fRotx * fFrame;//X��
			g_aPlayer.aModel[nCntParts][nWeapon].rot.y = g_aPlayerInfo[nCntParts].rot.y + PlayerRotY + fRoty * fFrame;//Y��
			g_aPlayer.aModel[nCntParts][nWeapon].rot.z = g_aPlayerInfo[nCntParts].rot.z + PlayerRotZ + fRotz * fFrame;//Z��

			//�ʒu���X�V
			g_aPlayer.aModel[nCntParts][nWeapon].pos.x = g_aPlayerInfo[nCntParts].pos.x + PlayerPosX + fPosx * fFrame;//X���W
			g_aPlayer.aModel[nCntParts][nWeapon].pos.y = g_aPlayerInfo[nCntParts].pos.y + PlayerPosY + fPosy * fFrame;//Y���W
			g_aPlayer.aModel[nCntParts][nWeapon].pos.z = g_aPlayerInfo[nCntParts].pos.z + PlayerPosZ + fPosz * fFrame;//Z���W
		}
	}

	//�L�[���̍X�V
	if (g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame == 0 || g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame >= g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame)
	{
		if (g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame >= g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame)
		{
			//�t���[�����J�E���^�����Z�b�g
			g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame = 0;
		}

		//�L�[�������Z
		g_aPlayer.aMotionSet[nWeapon][nState].nCntKey++;

		//�L�[�����ő吔�ɂȂ���
		if (g_aPlayer.aMotionSet[nWeapon][nState].nCntKey >= g_aPlayerMotionSet[nWeapon][nState].nMaxKey)
		{
			//���[�v���Ȃ��ꍇ
			if (g_aPlayerMotionSet[nWeapon][nState].nMotionLoop == 1)
			{
				//�j���[�g�������
				g_aPlayer.State = PLAYERSTATE_NUTORAL;
				g_aPlayer.bCompulsionAttack = false;    //�����I�ɍU����Ԃɂ���
				g_aPlayer.bCompulsionHealWall = false;  //�����I�ɕǉ񕜏�Ԃɂ���
				g_aPlayer.bCompulsionHealGround = false;//�����I�ɏ��񕜏�Ԃɂ���
				g_aPlayer.bCompulsionDodge = false;     //�����I�ɉ����Ԃɂ���
				g_aPlayer.aModel[19][1].bUse = true;//�u�[��������Ԃ��I������Ƃ��Ƀu�[�������̕`��𕜊�
				g_aPlayer.nAttackTime = 0;


				if (g_aPlayer.nStateOld == PLAYERSTATE_HEALWALL)
				{//�O�̃X�e�[�g���Ǐ[�d��Ԃ�������
					g_aPlayer.bInvincible = true;//���G��Ԃɂ���

					Model* pModel = GetModel();

					for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
					{
						if (pModel->bUse == true && pModel->nType == 41 && g_aPlayer.nNumUseWallOutlet == nCntModel)
						{
							pModel->bUse = false;
							KillRaderDisplay(pModel->nIdxRaderDisplay);
						}
					}
				}
				else if (g_aPlayer.nStateOld == PLAYERSTATE_HEALGROUND)
				{
					g_aPlayer.bInvincible = true;//���G��Ԃɂ���

					Model* pModel = GetModel();

					for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
					{
						if (pModel->bUse == true && pModel->nType == 19 && g_aPlayer.nNumUseGroundOutlet == nCntModel)
						{
							pModel->bUse = false;
							KillRaderDisplay(pModel->nIdxRaderDisplay);


						}
					}
				}
			}

			//�L�[����������
			g_aPlayer.aMotionSet[nWeapon][nState].nCntKey = 0;
		}
	}

	//===========================================================================
	//�v���O���R���Z���g�̍������݌��Ɏh�������Ƃ��ɌŒ肵�A�[�d���J�n����
	//===========================================================================
	if (g_aPlayer.State == PLAYERSTATE_HEALWALL && nCntKey == 18 && g_aPlayer.bChargingFlag == true)
	{
		SubBattery(-500);
		PlaySound(SOUND_LABEL_SE_CHARGINGCOMPLETION_000);
		g_aPlayer.bChargingFlag = false;
	}
	else if (g_aPlayer.State == PLAYERSTATE_HEALGROUND && nCntKey == 18 && g_aPlayer.bChargingGroundFlag)
	{
		SubBattery(-500);
		PlaySound(SOUND_LABEL_SE_CHARGINGCOMPLETION_000);
		g_aPlayer.bChargingGroundFlag = false;
	}

	//�t���[�����X�V
	g_aPlayer.aMotionSet[nWeapon][nState].nCntFrame++;

	PrintDebugProc("���݂̃L�[�F%d\n", g_aPlayer.aMotionSet[nWeapon][nState].nCntKey);
	PrintDebugProc("���݂̃v���C���[�̏�ԁF%d\n", g_aPlayer.State);

}

//============================================================================================================================================================================================================
// �r�̃��[�V��������
//============================================================================================================================================================================================================
void PlayerMotion2(int nWeapon)
{
	int nState = 0;
	int nCntKey = 0;
	int nNextKey;
	float fFrame;

	//�p�[�c���Ƃɏ����X�V
	for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
	{
		if (nCntParts >= 10 && nCntParts <= 13)
		{
			if (g_aPlayer.State == PLAYERSTATE_ATTACK)
			{//�U����Ԃ̏ꍇ
				if (g_aPlayer.bMove)
				{//�����Ă���ꍇ
					nState = (int)PLAYERSTATE_MOVE;	//�ړ����
				}
				else
				{//�����Ă��Ȃ��ꍇ
					nState = (int)g_aPlayer.State;	//��Ԃ����킹��
				}
			}
			else
			{//�U����Ԃł͂Ȃ��ꍇ
				nState = (int)g_aPlayer.State;	//��Ԃ����킹��
			}

			nCntKey = g_aPlayer.aMotionSet2[nWeapon][nState].nCntKey;																//���݂̃L�[��ۑ�
			nNextKey = (nCntKey + 1) % g_aPlayerMotionSet[nWeapon][nState].nMaxKey;													//���̃L�[�����߂�
			fFrame = (float)g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame / g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame;	//���݂̃t���[���ƑS�̃t���[���̊���

			float PlayerPosX = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.x;
			float PlayerPosY = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.y;
			float PlayerPosZ = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].pos.z;
			float PlayerRotX = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.x;
			float PlayerRotY = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.y;
			float PlayerRotZ = g_aPlayerKeySet[nWeapon][nCntKey][nState].key[nCntParts].rot.z;

			float NextPlayerPosX = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.x;
			float NextPlayerPosY = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.y;
			float NextPlayerPosZ = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].pos.z;
			float NextPlayerRotX = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.x;
			float NextPlayerRotY = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.y;
			float NextPlayerRotZ = g_aPlayerKeySet[nWeapon][nNextKey][nState].key[nCntParts].rot.z;

			//���̃L�[�܂ł̍��������߂�
			float fPosx = NextPlayerPosX - PlayerPosX;	// X���W
			float fPosy = NextPlayerPosY - PlayerPosY;	// Y���W
			float fPosz = NextPlayerPosZ - PlayerPosZ;	// Z���W
			float fRotx = NextPlayerRotX - PlayerRotX;	// X���̌���
			float fRoty = NextPlayerRotY - PlayerRotY;	// Y���̌���
			float fRotz = NextPlayerRotZ - PlayerRotZ;	// Z���̌���

			//�������X�V
			g_aPlayer.aModel[nCntParts][nWeapon].rot.x = g_aPlayerInfo[nCntParts].rot.x + PlayerRotX + fRotx * fFrame;//X��
			g_aPlayer.aModel[nCntParts][nWeapon].rot.y = g_aPlayerInfo[nCntParts].rot.y + PlayerRotY + fRoty * fFrame;//Y��
			g_aPlayer.aModel[nCntParts][nWeapon].rot.z = g_aPlayerInfo[nCntParts].rot.z + PlayerRotZ + fRotz * fFrame;//Z��

			//�ʒu���X�V
			g_aPlayer.aModel[nCntParts][nWeapon].pos.x = g_aPlayerInfo[nCntParts].pos.x + PlayerPosX + fPosx * fFrame;//X���W
			g_aPlayer.aModel[nCntParts][nWeapon].pos.y = g_aPlayerInfo[nCntParts].pos.y + PlayerPosY + fPosy * fFrame;//Y���W
			g_aPlayer.aModel[nCntParts][nWeapon].pos.z = g_aPlayerInfo[nCntParts].pos.z + PlayerPosZ + fPosz * fFrame;//Z���W
		}
	}

	//�L�[���̍X�V
	if (g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame == 0 || g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame >= g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame)
	{
		if (g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame >= g_aPlayerKeySet[nWeapon][nCntKey][nState].nMaxFrame)
		{
			//�t���[�����J�E���^�����Z�b�g
			g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame = 0;
		}

		//�L�[�������Z
		g_aPlayer.aMotionSet2[nWeapon][nState].nCntKey++;

		//�L�[�����ő吔�ɂȂ���
		if (g_aPlayer.aMotionSet2[nWeapon][nState].nCntKey >= g_aPlayerMotionSet[nWeapon][nState].nMaxKey)
		{
			//���[�v���Ȃ��ꍇ
			if (g_aPlayerMotionSet[nWeapon][nState].nMotionLoop == 1)
			{
				//�j���[�g�������
				g_aPlayer.State = PLAYERSTATE_NUTORAL;
			}

			//�L�[����������
			g_aPlayer.aMotionSet2[nWeapon][nState].nCntKey = 0;
		}
	}

	//�t���[�����X�V
	g_aPlayer.aMotionSet2[nWeapon][nState].nCntFrame++;
}

//=======================================================================================================================================================================================================
// �_���[�W����
//=======================================================================================================================================================================================================
void SetDamagePlayer(int nDamage)
{
	g_aPlayer.nHp -= nDamage;
}

//=======================================================================================================================================================================================================
// �v���C���[�����񂾂��ǂ����̔����Ԃ�
//=======================================================================================================================================================================================================
bool bPlayerDeath(void)
{
	bool bDeath = false;

	if (g_aPlayer.nHp <= 0)
	{
		bDeath = true;
		g_aPlayer.bUse = false;
	}

	return bDeath;
}

//=======================================================================================================================================================================================================
// �X�N���b�v�������
//=======================================================================================================================================================================================================
void AddScrapPlayer(SCRAPTYPE ScrapType, int nScrap)//�X�N���b�v����������Ƃ��ɌĂԊ֐��B
{
	g_aPlayer.nTotalGetScrap++;//���v�l�������v���X

	switch (ScrapType)
	{
	case SCRAPTYPE00_TIBIROBO:

		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 50 - 25) / 10;
			float fRandumMove = float(rand() % 50 - 25) / 10;

			SetParticle(PARTICLETYPE_NORMAL, 100, 8.0f, 0.4f, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		g_aPlayer.nTibiroboScrap += nScrap;//���у��{�X�N���b�v���l��
		g_aPlayer.nTotalTibiroboScrap += nScrap;//���у��{�X�N���b�v���v�l����

		AddGauge(SCRAPTYPE00_TIBIROBO, g_aPlayer.nTibiroboScrap, g_nDecideLevelUpPoint[g_aPlayer.nLevel]);

		break;

	case SCRAPTYPE01_SWORD:

		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 50 - 25) / 10;
			float fRandumMove = float(rand() % 50 - 25) / 10;
			SetParticle(PARTICLETYPE_NORMAL, 100, 8.0f, 0.4f, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		g_aPlayer.aSwordInfo.nSwordScrap += nScrap;//���X�N���b�v���l��
		g_aPlayer.aSwordInfo.nTotalSwordScrap += nScrap;//���X�N���b�v���v�l����

		AddGauge(SCRAPTYPE01_SWORD, g_aPlayer.aSwordInfo.nSwordScrap, g_nDecideLevelUpPoint[g_aPlayer.aSwordInfo.nLevel]);

		break;

	case SCRAPTYPE02_BOOMERANG:

		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 50 - 25) / 10;
			float fRandumMove = float(rand() % 50 - 25) / 10;
			SetParticle(PARTICLETYPE_NORMAL, 100, 8.0f, 0.4f, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		g_aPlayer.aBoomerangInfo.nBoomerangScrap += nScrap;//�u�[�������X�N���b�v���l��
		g_aPlayer.aBoomerangInfo.nTotalBoomerangScrap += nScrap;//�u�[�������X�N���b�v���v�l����

		AddGauge(SCRAPTYPE02_BOOMERANG, g_aPlayer.aBoomerangInfo.nBoomerangScrap, g_nDecideLevelUpPoint[g_aPlayer.aBoomerangInfo.nLevel]);

		break;

	case SCRAPTYPE03_SPEAKER:

		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 50 - 25) / 10;
			float fRandumMove = float(rand() % 50 - 25) / 10;
			SetParticle(PARTICLETYPE_NORMAL, 100, 8.0f, 0.4f, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		g_aPlayer.aSpeakerInfo.nSpeakerScrap += nScrap;//�X�s�[�J�[�X�N���b�v���l��
		g_aPlayer.aSpeakerInfo.nTotalSpeakerScrap += nScrap;//�X�s�[�J�[�X�N���b�v���v�l����

		AddGauge(SCRAPTYPE03_SPEAKER, g_aPlayer.aSpeakerInfo.nSpeakerScrap, g_nDecideLevelUpPoint[g_aPlayer.aSpeakerInfo.nLevel]);

		break;

	case SCRAPTYPE04_ALL:

		for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		{
			float fRandumRot = float(rand() % 200 + 1) / 100;
			float fRandumSpeed = float(rand() % 50 - 25) / 10;
			float fRandumMove = float(rand() % 50 - 25) / 10;
			float fRandumColorR = float(rand() % 100 + 1) / 100;
			float fRandumColorG = float(rand() % 100 + 1) / 100;
			float fRandumColorB = float(rand() % 100 + 1) / 100;
			SetParticle(PARTICLETYPE_NORMAL, 100, 8.0f, 0.4f, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + g_aPlayer.Size.y / 2, g_aPlayer.pos.z), D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed),
				D3DXCOLOR(fRandumColorR, fRandumColorG, fRandumColorB, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		g_aPlayer.nTibiroboScrap += nScrap;//���у��{�X�N���b�v���l��
		g_aPlayer.aSwordInfo.nSwordScrap += nScrap;//���X�N���b�v���l��
		g_aPlayer.aBoomerangInfo.nBoomerangScrap += nScrap;//�u�[�������X�N���b�v���l��
		g_aPlayer.aSpeakerInfo.nSpeakerScrap += nScrap;//�X�s�[�J�[�X�N���b�v���l��
		g_aPlayer.nTotalAllUpScrap += nScrap;//�S�X�e�[�^�X�����X�N���b�v���v�l����

		AddGauge(SCRAPTYPE00_TIBIROBO, g_aPlayer.nTibiroboScrap, g_nDecideLevelUpPoint[g_aPlayer.nLevel]);
		AddGauge(SCRAPTYPE01_SWORD, g_aPlayer.aSwordInfo.nSwordScrap, g_nDecideLevelUpPoint[g_aPlayer.aSwordInfo.nLevel]);
		AddGauge(SCRAPTYPE02_BOOMERANG, g_aPlayer.aBoomerangInfo.nBoomerangScrap, g_nDecideLevelUpPoint[g_aPlayer.aBoomerangInfo.nLevel]);
		AddGauge(SCRAPTYPE03_SPEAKER, g_aPlayer.aSpeakerInfo.nSpeakerScrap, g_nDecideLevelUpPoint[g_aPlayer.aSpeakerInfo.nLevel]);

		break;
	}
}

//=======================================================================================================================================================================================================
// �v���C���[�̃��x���Ɋւ��鏈�����s���B
//=======================================================================================================================================================================================================
void LevelPlayer(void)
{
	//========================
	// �v���C���[����
	//========================
	if (g_aPlayer.nTibiroboScrap >= g_nDecideLevelUpPoint[g_aPlayer.nLevel] && g_aPlayer.nLevel < MAX_LEVEL)//�v���C���[�����X�N���b�v���A���x���A�b�v����ʂ܂ŗ��܂�����
	{
		g_aPlayer.nTibiroboScrap = 0;//���x���A�b�v�K�v�ʂɒB�����̂ŁA�X�N���b�v�l���ʂ����Z�b�g
		g_aPlayer.nLevel += 1;//���x�����P�グ��

		AddLevel(0);
	}
	
	//========================
	// ���U������
	//========================
	if (g_aPlayer.aSwordInfo.nSwordScrap >= g_nDecideLevelUpPoint[g_aPlayer.aSwordInfo.nLevel] && g_aPlayer.nLevel < MAX_LEVEL)//���U�������X�N���b�v���A���x���A�b�v����ʂ܂ŗ��܂�����
	{
		g_aPlayer.aSwordInfo.nSwordScrap = 0;//���x���A�b�v�K�v�ʂɒB�����̂ŁA�X�N���b�v�l���ʂ����Z�b�g
		g_aPlayer.aSwordInfo.nLevel += 1;//���x�����P�グ��

		AddLevel(1);
	}

	//========================
	// �u�[�������U������
	//========================
	if (g_aPlayer.aBoomerangInfo.nBoomerangScrap >= g_nDecideLevelUpPoint[g_aPlayer.aBoomerangInfo.nLevel] && g_aPlayer.nLevel < MAX_LEVEL)//�u�[�������U�������X�N���b�v���A���x���A�b�v����ʂ܂ŗ��܂�����
	{
		g_aPlayer.aBoomerangInfo.nBoomerangScrap = 0;//���x���A�b�v�K�v�ʂɒB�����̂ŁA�X�N���b�v�l���ʂ����Z�b�g
		g_aPlayer.aBoomerangInfo.nLevel += 1;//���x�����P�グ��

		AddLevel(2);
	}
	
	//========================
	// �X�s�[�J�[�U������
	//========================
	if (g_aPlayer.aSpeakerInfo.nSpeakerScrap >= g_nDecideLevelUpPoint[g_aPlayer.aSpeakerInfo.nLevel] && g_aPlayer.nLevel < MAX_LEVEL)//�X�s�[�J�[�U�������X�N���b�v���A���x���A�b�v����ʂ܂ŗ��܂�����
	{
		g_aPlayer.aSpeakerInfo.nSpeakerScrap = 0;//���x���A�b�v�K�v�ʂɒB�����̂ŁA�X�N���b�v�l���ʂ����Z�b�g
		g_aPlayer.aSpeakerInfo.nLevel += 1;//���x�����P�グ��

		AddLevel(3);
	}
}

//=======================================================================================================================================================================================================
// �v���C���[�̃X�e�[�^�X�Ɋւ��鏈�����s���B
//=======================================================================================================================================================================================================
void StatusPlayer(void)
{
	int nSaveBoomerangPiece = g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece;//1F�O�̎��Ă�u�[�������̍ő吔(����Ă�����

	//===============================
	// �v���C���[�̃X�e�[�^�X
	//===============================
	g_aPlayer.fSpeed = FIRST_PLAYERSPEED + (0.4f * g_aPlayer.nLevel);
	g_aPlayer.nBattery = 1000 + (100 * g_aPlayer.nLevel);
	g_aPlayer.fSuctionScrapRange = 20.0f + (15.0f * g_aPlayer.nLevel);

	//===============================
	// ���̃X�e�[�^�X
	//===============================
	g_aPlayer.aSwordInfo.nSwordPower = 10 + (int)(0.6f * g_aPlayer.aSwordInfo.nLevel);//���̍U����
	float fData = 1.0f;

	for (int nCnt = 0; nCnt < g_aPlayer.aSwordInfo.nLevel; nCnt++)
	{
		fData *= 1.1f;
	}

	g_aPlayer.aSwordInfo.nSwordAttackSpeed = 4 * (int)fData;//���̍U�����x

	//===============================
	// �u�[�������̃X�e�[�^�X
	//===============================
	g_aPlayer.aBoomerangInfo.nBoomerangPower = 5 + (int)(0.5f * g_aPlayer.aBoomerangInfo.nLevel);//�u�[�������̍U����
	g_aPlayer.aBoomerangInfo.fBoomerangRenge = 200.0f + (30.0f * g_aPlayer.aBoomerangInfo.nLevel);//�u�[�������̎˒�
	g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece = 1 + (g_aPlayer.aBoomerangInfo.nLevel / 3);//�u�[�������̌�
	float fBoomerangScale = 1.0f + (0.2f * g_aPlayer.aBoomerangInfo.nLevel);//�u�[�������̑傫��
	g_aPlayer.aBoomerangInfo.BoomerangScale = D3DXVECTOR3(fBoomerangScale, fBoomerangScale, fBoomerangScale);//�u�[�������̑傫�����f

	if (nSaveBoomerangPiece != g_aPlayer.aBoomerangInfo.nMaxBoomerangPiece)
	{//���x�����オ�����̂ŁA���ݎ����Ă���u�[�������̌����P���₷
		g_aPlayer.aBoomerangInfo.nBoomerangPiece += 1;
	}

	//===============================
	// �X�s�[�J�[�̃X�e�[�^�X
	//===============================
	g_aPlayer.aSpeakerInfo.fSpeakerPower = 7.0f + 7.0f * (0.3f * g_aPlayer.aSpeakerInfo.nLevel);//�X�s�[�J�[�̍U����
	g_aPlayer.aSpeakerInfo.nPowerConsumption = 100;//�X�s�[�J�[�̏���d��
}

//=======================================================================================================================================================================================================
// �v���C���[�̃}�g���b�N�X���v�Z����֐�
//=======================================================================================================================================================================================================
void CalculateMatrixPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;	//�v�Z�p�}�g���b�N�X

	//�������Ă��镐��̔ԍ���ۑ�
	int nWeapon = g_aPlayer.nWeapon;
	
	//�g�p���Ă���ꍇ
	if (g_aPlayer.bUse)
	{
		//�v���C���[�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

		//�v���C���[�̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

		//�v���C���[�̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

		for (int nCntParts = 0; nCntParts < MAX_PLAYERPARTS2; nCntParts++)
		{
			//�e�p�[�c���f���̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld);

			//�e�p�[�c���f���̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCntParts][nWeapon].rot.y, g_aPlayer.aModel[nCntParts][nWeapon].rot.x, g_aPlayer.aModel[nCntParts][nWeapon].rot.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxRot);

			//�e�p�[�c���f���̈ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCntParts][nWeapon].pos.x, g_aPlayer.aModel[nCntParts][nWeapon].pos.y, g_aPlayer.aModel[nCntParts][nWeapon].pos.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxTrans);

			//�e�̔ԍ���ۑ�
			int nParent = g_aPlayer.aModel[nCntParts][nWeapon].nIdxModelParent;

			//�e�̃}�g���b�N�X�𔽉f
			if (nParent != -1)
			{//�e�̔ԍ������ꍇ
				mtxParent = g_aPlayer.aModel[nParent][nWeapon].mtxWorld;	//�e���f���̃}�g���b�N�X
			}
			else
			{//�e�̔ԍ��������Ȃ��ꍇ
				mtxParent = g_aPlayer.mtxWorld;	//�v���C���[�̃}�g���b�N�X
			}

			//�e�̃}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld, &mtxParent);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld);

			D3DXVECTOR3 posSword;
			D3DXVECTOR3 posZero(0.0f, 0.0f, 0.0f);

			D3DXVec3TransformCoord(&posSword, &posZero, &g_aPlayer.aModel[nCntParts][nWeapon].mtxWorld);//���̈����̃��[�J�����W���^�񒆂̈����̃��[���h���W��������

			//���[���h���W����
			g_aPlayer.aModel[nCntParts][nWeapon].PartsPos = posSword;
		}
	}
}

//=======================================================================================================================================================================================================
//�v���C���[�ɍU�����q�b�g�������ɌĂԊ֐�
//=======================================================================================================================================================================================================
void AttackHitPlayer(int nSubBattery)
{
	if (!g_aPlayer.bDamageState && !g_aPlayer.bInvincible && g_aPlayer.State != PLAYERSTATE_HEALWALL && g_aPlayer.State != PLAYERSTATE_HEALGROUND &&
		g_aPlayer.State != PLAYERSTATE_DODGE)
	{
		PlaySound(SOUND_LABEL_SE_DAMAGE_000);

		SubBattery(nSubBattery);
		g_aPlayer.bDamageState = true;//�_���[�W��Ԃɂ���
		g_aPlayer.bInvincible = true;//���G��Ԃɂ���

		//�R���{�{�������Z�b�g
		g_aPlayer.fCombo = 1.0f;
	}
}

//=======================================================================================================================================================================================================
//�v���C���[�̈ʒu���Z�b�g����
//=======================================================================================================================================================================================================
void ResetPlayerPos(void)
{
	//���������Z�b�g(�O����)
	g_aPlayer.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	
	switch (GetMode())
	{
	case MODE_TUTORIAL:	//�`���[�g���A��
		g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;
	case MODE_GAME:	//�Q�[��
		g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, -2200.0f);
		break;
	default:
		break;
	}
}