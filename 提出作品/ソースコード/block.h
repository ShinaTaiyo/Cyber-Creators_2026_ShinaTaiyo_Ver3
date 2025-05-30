//====================================================
//
//�V���P�O���FC++�Ŋ�ՂÂ���[block.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _BLOCK_H_  
#define _BLOCK_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "objectX.h"
//==========================================

//===================================
//�}�b�v�̃e�L�X�g�t�@�C���̃}�N��
//===================================
#define MAP00_SENTRAL_TXT "data\\TEXTFILE\\Map\\Sentral.txt"

//==========================================

//==========================================
//�u���b�N�N���X
//==========================================
class CBlock : public CObjectX
{
public:
	//===========================
	//�u���b�N�̎��
	//===========================
    enum class BLOCKTYPE
	{
		NORMAL = 0,    //���ʃu���b�N
		WATER,         //���u���b�N
		RENGA,         //�����K�u���b�N
		BLACK,         //���u���b�N
		MAX
	};
	//======================================================================

	//===========================
	//�}�b�v�̎��
	//===========================
	typedef enum
	{
		MAP00_SENTRAL = 0,
		MAP_MAX
	}MAPTYPE;
	//======================================================================

	CBlock(int nPri = 0, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::BLOCK, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                 //�R���X�g���N�^
	~CBlock() override;       //�f�X�g���N�^
	HRESULT Init() override;  //����������
	void Uninit() override;   //�I������
	void Update() override;   //�X�V����
	void Draw() override;     //�`�揈��
	void SetDeath() override; //���S�t���O��ݒ�
	static CBlock * Create(BLOCKTYPE type, int nLife,D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 scale,bool bSwapVtxXZ);//�u���b�N�𐶐�
	void SetBlockType(BLOCKTYPE Type);                                                                                  //�u���b�N�̃^�C�v��ݒ肷��
	static int GetNumFile() { return m_nNumFile; }                                                                      //�t�@�C�������擾����
	BLOCKTYPE GetType();//�u���b�N�̎�ނ��擾����
	BLOCKTYPE GetBlockType() { return m_type; }                                                                         //�u���b�N�̃^�C�v���擾����
	bool GetbCollision() { return m_bCollision; }                                                                       //����\���ǂ������擾����     

    //==========================================================
	//�G�f�B�^�֌W
	//==========================================================
	//�֐�
	void SaveInfoTxt(fstream& WritingFile) override;                                                                    //�e�L�X�g�t�@�C���ɏ���ۑ����邽�߂̊֐�
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override;        //�e�L�X�g�t�@�C���������ǂݍ��ނ��߂̊֐�
	CObject* ManagerChengeObject(bool bAim) override;                                                                   //�X�e�[�W�}�l�[�W���[�ɕύX�����I�u�W�F�N�g��n��
	CObject* ManagerSaveObject() override;                                                                              //�X�e�[�W�}�l�[�W���[�ɍ��̃I�u�W�F�N�g��ۑ�����
	//=================================================================================================================
private:
	//======================================
	//�ÓI�����o
	//======================================
	static const char* m_BLOCK_FILENAME[static_cast<int>(BLOCKTYPE::MAX)];                                              //�u���b�N�̃t�@�C���l�[��
	static const float m_fBLOCKCORRECTIONCOLLISION;                                                                     //����ɗ]�T�����������
	static int m_nNumFile;                                                                                              //�t�@�C�������i�[����
	//======================================================================================================================

	//======================================
	//��{�n
	//======================================
	BLOCKTYPE m_type;                                                              //�u���b�N�̎��
	bool m_bCollision;                                                             //�����蔻������邩�ǂ���
	//======================================================================================================================
};

#endif
