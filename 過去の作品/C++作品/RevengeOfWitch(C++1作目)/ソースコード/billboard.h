//==================================================================================
//
//�U���T���F�r���{�[�h���쐬����[billboard.h]
//Author:ShinaTaiyo
//
//===================================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//===================================================================================
//�C���N���[�h
//===================================================================================
#include "main.h"
#include "object.h"

//===================================================================================

//===================================================================================
//�O���錾
//===================================================================================
class CEnemy;
class CObjectX;

//===================================================================================
//�r���{�[�h�N���X
//===================================================================================
class CBillboard : public CObject
{
public:
	CBillboard(int nPriority = 1);                                              //�R���X�g���N�^
	~CBillboard();                                                              //�f�X�g���N�^
	HRESULT Init(void) override;                                                //����������
	void Uninit(void) override;                                                 //�I������
	void Update(void) override;                                                 //�X�V����
	void Draw(void) override;                                                   //�`�揈��
	void bindTexture(LPDIRECT3DTEXTURE9 pTexture);                              //�e�N�X�`�������蓖�Ă�

	bool CollisionSquare(D3DXVECTOR3 pos,D3DXVECTOR3 VtxMax,D3DXVECTOR3 VtxMin);//�����`�̓����蔻������

	D3DXVECTOR3 & GetPos() { return m_Pos; }                                    //�ʒu���擾����
	D3DXVECTOR3& GetPosOld() { return m_PosOld; }                               //1f�O�̈ʒu���擾
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }                               //�ʒu��ݒ肷��
	void SetSupportPos(D3DXVECTOR3 Pos) { m_SupportPos = Pos; }                 //�����ʒu��ݒ肷��
	D3DXVECTOR3& GetSupportPos() { return m_SupportPos; }                       //�����ʒu���擾����
	void SetMove(D3DXVECTOR3 Move) { m_Move = Move; }                           //�ړ��ʂ�ݒ肷��
	D3DXVECTOR3& GetMove() { return m_Move; }                                   //�ړ��ʂ��擾����
	float &GetWidth() { return m_fWidth; }                                      //�������擾����
	float &GetHeight() { return m_fHeight; }                                    //�������擾����
	float& GetFormarWidth() { return m_fFormarWidth; }                          //���̉������擾����
	float& GetFormarHeight() { return m_fFormarHeight; }                        //���̍������擾����
	void SetSize(float fWidth, float fHeight);                                  //�T�C�Y��ݒ肷��
	void SetFormarSize(float fFormarWidth, float fFormarHeight);                //���̑傫����ݒ肷��
	void SetColor(D3DXCOLOR col);                                               //�F������ݒ肷��
	D3DXCOLOR GetColor() { return m_Col; }                                      //�F���擾����
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }//�e�N�X�`���ԍ����Z�b�g����
	int GetTextureIndex() { return m_nTextureIndex; }         //�e�N�X�`���ԍ����擾����
	void SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange,D3DXCOLOR col,bool bAnim);//�A�j���[�V�����֌W�̐ݒ�
	void SetAnim(int nAnim) { m_nAnimaionPattern = nAnim; }   //�A�j���[�V�����ԍ���ݒ�
	D3DXMATRIX& GetMtxWorld() { return m_mtxWorld; }          //���[���h�}�g���b�N�X���擾����
	LPDIRECT3DVERTEXBUFFER9& GetVtxBuff() { return m_pVtxBuff; }//���_�o�b�t�@���擾����
	LPDIRECT3DTEXTURE9& GetTexture() { return m_pTexture; }     //�e�N�X�`�����擾����

	void SetScaling(float fAddScale);//�g������邩�ǂ���
	void SetUseEffect(int nSetEffectLife, D3DXVECTOR2 Size, D3DXCOLOR Col);//�G�t�F�N�g���g�p���邩�ǂ���
	void SetUseMtxChild(D3DXMATRIX* pMtxParent,bool bUse);//�q�}�g���b�N�X��ݒ�
	void SetUseGravity(float fGravity); //�d�͂�ݒ�
	void SetAddGravity(float fAddGravity) { m_fAddGravity = fAddGravity; };//���Z����d�͂�ݒ�
	void SetUseHorming(float fSpeed);   //�z�[�~���O��ݒ�
	void SetUseAddSpeed(bool bUseAddSpeed, bool bMultiplication, float fAddSpeed);//������ݒ�

	D3DXVECTOR3& GetMtxTransPos() {return m_TransPos; }                                //�q�}�g���b�N�X�̃��[���h���W���擾����
	bool GetUseMtxChild() { return m_bMtxChild; }                               //�q�}�g���b�N�X���g���Ă��邩�ǂ������擾����    

	//====================================================
	//�J�[�u�֌W
	//====================================================
	void SetUseCurve(float fRot, float fSpeed, float fCurveSpeed);//�J�[�u�����邩�ǂ���
	void SetUseLengthCurve(bool bUse, float fStartRot, float fCurveSpeed, float fAddCurveLength);
    //====================================================================================================================

	//====================================================
	//��]�֌W
	//====================================================
	void SetUsePolygonRot(bool bUse, float fRotPower) { m_bUsePolygonRot = bUse; m_fPolygonRotPower = fRotPower; }//�|���S������]�����邩�ǂ�����ݒ�
	//====================================================================================================================

	//====================================================
	//�`��֌W
	//====================================================
	void SetUseDraw(bool bDraw) { m_bDraw = bDraw; }                            //�`�悷�邩�ǂ�����ݒ肷��
	bool& GetUseDraw() { return m_bDraw; }                                      //�`�悷�邩�ǂ������擾����
	//====================================================================================================================

	void SetLife(int nLife) { m_nLife = nLife; }                                //�̗͂�ݒ�
	void SetMaxLife(int nLife) { m_nMaxLife = nLife; }                          //�ő�̗͂�ݒ�

	int& GetLife() { return m_nLife; }                                          //�̗͂��擾
	int& GetMaxLife() { return m_nMaxLife; }                                    //�ő�̗͂��擾
private:			
	void HormingProcess();                                                      //�z�[�~���O�̏���
	static const int m_nMAX_MAT = 50;                                           //�}�e���A���̍ő吔
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;                                         //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTexture;                                              //�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_Pos;                                                          //�ʒu
	D3DXVECTOR3 m_PosOld;                                                       //1f�O�̈ʒu
	D3DXVECTOR3 m_SupportPos;                                                   //�����ʒu��ݒ�
	D3DXVECTOR3 m_Move;                                                         //�ړ���
	D3DXVECTOR3 m_Rot;                                                          //����
	D3DXMATRIX m_mtxWorld;                                                      //���[���h�ϊ��s��
	D3DXCOLOR m_Col;                                                            //�F����
   
	float m_fWidth;                                                             //����
	float m_fFormarWidth;                                                       //���̉���
	float m_fHeight;                                                            //����
	float m_fFormarHeight;                                                      //���̍���

	int m_nLife;                                                                //�̗�
	int m_nMaxLife;                                                             //�ő�̗�

	float m_fAnimationSplit;                                                    //�A�j���[�V�����P����������̒l
	int m_nAnimaionPattern;                                                     //�A�j���[�V�����p�^�[��
	int m_nAnimationCnt;                                                        //�A�j���[�V�����J�E���g
	int m_nMaxAnimationPattern;                                                 //�A�j���[�V�����p�^�[���̍ő吔
	int m_nAnimationChange;                                                     //�A�j���[�V������ς���J�E���g��
	bool m_bAnimFlag;                                                           //�A�j���[�V���������邩�ǂ���

	int m_nTextureIndex;                                                        //�e�N�X�`���ԍ�

	bool m_bScaling;        //�g�k�����邩�ǂ���  
	float m_fAddScale;      //�g��ʂ�ݒ�
	float m_fScale;         //�g�嗦

	bool m_bUse;            //�g�p���

	float m_fSpeed;          //����

	int m_nCntTime;          //�o�����Ă���̎��Ԃ��J�E���g����
	//========================================
	//�J�[�u�n
	//========================================
	bool m_bUseCurve;           //�J�[�u�e�ɂ��邩�ǂ���
	bool m_bUseLengthCurve;     //���_�����ɃJ�[�u���邩�ǂ���
	float m_fAddCurveLength;    //�x�_����̋�����L�΂��l
	float m_fCurveSpeed;        //�J�[�u����X�s�[�h
	float m_fSupportCurveLength;//���_�����ɉ�]���鋗�������߂�
	float m_fAddRot;            //�����̕␳
	float m_fStartRot;          //�ŏ��̌���
	//=======================================================================================================

	//========================================
	//��]�֌W
	//========================================
	bool m_bUsePolygonRot;   //�|���S������]�����邩�ǂ���
	float m_fPolygonRotPower;//�|���S���̉�]��
	//=======================================================================================================


	//========================================
	//�`��֌W
	//========================================
	bool m_bDraw;               //�`�悷�邩�ǂ���
	//=======================================================================================================

	bool m_bSetEffect;          //�G�t�F�N�g���g�p���邩�ǂ���
	D3DXCOLOR m_SetEffectCol;   //�ݒ肷��G�t�F�N�g�̐F����
	int m_nSetEffectLife;       //�ݒ肷��G�t�F�N�g�̗̑�
	D3DXVECTOR2 m_SetEffectSize;//�ݒ肷��G�t�F�N�g�̃T�C�Y

	//===============================
	//�e�}�g���b�N�X�n
	//===============================
	D3DXMATRIX* m_pMtxParent; //�e�}�g���b�N�X�ւ̃|�C���^
	bool m_bMtxChild;        //�q�}�g���b�N�X�ɂ��邩�ǂ���
	D3DXVECTOR3 m_TransPos;  //�q�}�g���b�N�X�ɂ��Ă���ꍇ�̈ʒu
	//================================================================================

	//===============================
	//�d�͌n
	//===============================
	bool m_bUseGravity;      //�d�͂��g�p���邩�ǂ���
	float m_fGravityPower;   //�d�͂̑傫��
	float m_fAddGravity;     //���Z����d��
	float m_fGravity;        //�d��
	//================================================================================

	//===============================
	//�z�[�~���O�n
	//===============================
	bool m_bUseHorming;      //�z�[�~���O���g�p���邩�ǂ���
	float m_fRotMove;        //�����ւ̈ړ���
	//================================================================================

	//===============================
	//�����n
	//===============================
	bool m_bUseAddSpeed;     //�������g�p���邩�ǂ���
	float m_fAddSpeed;       //�����x
	bool m_bMultiplication;    //���x����Z�����邩�ǂ���
	//================================================================================

};
//===================================================================================


#endif