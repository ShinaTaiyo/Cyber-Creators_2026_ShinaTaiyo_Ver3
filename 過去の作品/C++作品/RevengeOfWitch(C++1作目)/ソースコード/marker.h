//====================================================
//
//�U���Q�T���F���[�v�}�[�J�[������[marker.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _MARKER_H_  
#define _MARKER_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
#include "number3d.h"
//==========================================

//==========================================

//==========================================
//�}�[�J�[�N���X
//==========================================
class CMarker : public CObjectX
{
public:
	//===========================
	//�}�[�J�[�̎��
	//===========================
	typedef enum
	{
		MARKERTYPE00_WARP = 0,//���[�v�}�[�J�[
		MARKERTYPE_MAX
	}MARKERTYPE;
	//======================================================================
	CMarker();                   //�R���X�g���N�^
	~CMarker() override;         //�f�X�g���N�^
	HRESULT Init() override;    //����������
	void Uninit() override;     //�I������
	void Update() override;     //�X�V����
	void Draw() override;       //�`�揈��
	void SetDeath() override;   //���S�t���O��ݒ�
	static int GetNumFile() { return m_nNumFile; }//�t�@�C���̐����擾����
	static CMarker* Create(MARKERTYPE type,D3DXVECTOR3 pos,D3DXVECTOR3 Scale,D3DXVECTOR3 WarpPos,int nMapNum);//�G�𐶐�
	D3DXVECTOR3 GetWarpPos() { return m_WarpPos; }//���[�v����ʒu���擾����
	int GetWarpMap() { return m_nWarpMapNum; }    //���[�v��̃}�b�v���擾����
	void DeleteNum();       //�ԍ�������
private:
	static const int m_nWARPMAPDIVISION = 2;
	void Collision();       //�����蔻������
	MARKERTYPE m_type;      //�G�̎��
	D3DXVECTOR3 m_Aim;      //�x�N�g��
	D3DXVECTOR3 m_WarpPos;  //���[�v����ʒu�����߂�
	CNumber3D* m_pWarpMapDispNum[m_nWARPMAPDIVISION];//���[�v��}�b�v�̔ԍ���\��
	int m_nWarpMapNum;      //���[�v����}�b�v�̔ԍ����i�[
	float m_fTotalAim;      //���������x�N�g��
	int m_nCntTime;         //�o�����Ă���̃t���[�����𑪂�
	static const char* m_MARKER_FILENAME[MARKERTYPE_MAX];   //�}�[�J�[�̃t�@�C���l�[��
	static const int m_nMAXENEMY01BULLET;
	static int m_nNumFile;                                  //�ǂݍ��񂾃t�@�C���̐�
};

#endif