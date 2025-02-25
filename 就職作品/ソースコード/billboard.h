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
	CBillboard(int nPri = 6, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_BILLBOARD);                                              //�R���X�g���N�^
	~CBillboard();                                                              //�f�X�g���N�^
	HRESULT Init(void) override;                                                //����������
	void Uninit(void) override;                                                 //�I������
	void Update(void) override;                                                 //�X�V����
	void Draw(void) override;                                                   //�`�揈��
	void SetDeath() override;                                                   //���S�t���O�ݒ菈��
	void bindTexture(LPDIRECT3DTEXTURE9 pTexture);                              //�e�N�X�`�������蓖�Ă�

	bool CollisionSquare(D3DXVECTOR3 pos,D3DXVECTOR3 VtxMax,D3DXVECTOR3 VtxMin);//�����`�̓����蔻������

	//================================================
	//���W�֌W
	//================================================
	D3DXVECTOR3 & GetPos() { return m_Pos; }                                    //�ʒu���擾����
	D3DXVECTOR3& GetPosOld() { return m_PosOld; }                               //1f�O�̈ʒu���擾
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }                               //�ʒu��ݒ肷��
	void SetSupportPos(D3DXVECTOR3 Pos) { m_SupportPos = Pos; }                 //�����ʒu��ݒ肷��
	D3DXVECTOR3& GetSupportPos() { return m_SupportPos; }                       //�����ʒu���擾����
	void SetMove(D3DXVECTOR3 Move) { m_Move = Move; }                           //�ړ��ʂ�ݒ肷��
	D3DXVECTOR3& GetMove() { return m_Move; }                                   //�ړ��ʂ��擾����
	//=======================================================================================================

	//================================================
	//�T�C�Y�֌W
	//================================================
	float &GetWidth() { return m_fWidth; }                                      //�������擾����
	float &GetHeight() { return m_fHeight; }                                    //�������擾����
	float& GetFormarWidth() { return m_fFormarWidth; }                          //���̉������擾����
	float& GetFormarHeight() { return m_fFormarHeight; }                        //���̍������擾����
	void SetSize(float fWidth, float fHeight);                                  //�T�C�Y��ݒ肷��
	void SetFormarSize(float fFormarWidth, float fFormarHeight);                //���̑傫����ݒ肷��
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }                       //�g�嗦��ݒ�
	const D3DXVECTOR3& GetScale() const { return m_Scale; }                     //�g�嗦���擾
	void SetUseAddScale(bool bUse, D3DXVECTOR3 AddScale) { m_AddScale = AddScale; m_bUseAddScale = bUse; }            //���Z�g�嗦
	const D3DXVECTOR3& GetAddScale() const { return m_AddScale; }               //���Z�g�嗦���擾
	//=======================================================================================================


	void SetColor(D3DXCOLOR col);                                                             //�F������ݒ肷��
	D3DXCOLOR GetColor() { return m_Col; }                                                    //�F���擾����
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }                                //�e�N�X�`���ԍ����Z�b�g����
	int GetTextureIndex() { return m_nTextureIndex; }                                         //�e�N�X�`���ԍ����擾����
	void SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange,D3DXCOLOR col,bool bAnim);//�A�j���[�V�����֌W�̐ݒ�
	void SetAnim(int nAnim) { m_nAnimaionPattern = nAnim; }                                   //�A�j���[�V�����ԍ���ݒ�
	D3DXMATRIX& GetMtxWorld() { return m_mtxWorld; }                                          //���[���h�}�g���b�N�X���擾����
	LPDIRECT3DVERTEXBUFFER9& GetVtxBuff() { return m_pVtxBuff; }                              //���_�o�b�t�@���擾����
	LPDIRECT3DTEXTURE9& GetTexture() { return m_pTexture; }                                   //�e�N�X�`�����擾����

	void SetUseGravity(float fGravity);                                                       //�d�͂�ݒ�
	void SetAddGravity(float fAddGravity) { m_fAddGravity = fAddGravity; };                   //���Z����d�͂�ݒ�
	void SetUseHorming(float fSpeed);                                                         //�z�[�~���O���邩�ǂ�����ݒ�
	void SetUseAddSpeed(bool bUseAddSpeed, bool bMultiplication, float fAddSpeed);            //�������邩�ǂ�����ݒ�

	//====================================================
	//�J�[�u�֌W
	//====================================================
	void SetUseLengthCurve(bool bUse, float fStartRot, float fCurveSpeed, float fAddCurveLength,float fSupportLength);
    //====================================================================================================================

	//====================================================
	//��]�֌W
	//====================================================
	void SetUsePolygonRot(bool bUse, float fRotPower) { m_bUsePolygonRot = bUse; m_fPolygonRotPower = fRotPower; }//�|���S������]�����邩�ǂ�����ݒ�
	//====================================================================================================================

	//==============================
	//�_�Ŋ֌W
	//==============================
	void SetUseBlinking(bool bUse, int nMaxBlinkingFrame, float fLimitBlinkingRatio) {
		m_bUseBlinking = bUse;                                   //�_�ł��g�p���邩�ǂ���
		m_nMaxBlinkingFrame = nMaxBlinkingFrame;                 //�_�ōő�t���[��
		m_fLimitBlinkingRatio = fLimitBlinkingRatio;             //�_�ł�����Z���̃��~�b�g
	}
	//==============================================================================================================================================================

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
	static const int m_nMAX_MAT = 50;                                           //�}�e���A���̍ő吔
	void HormingProcess();                                                      //�z�[�~���O�̏���

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;                                         //���_�o�b�t�@�ւ̃|�C���^!
	LPDIRECT3DTEXTURE9 m_pTexture;                                              //�e�N�X�`���ւ̃|�C���^!

	//=======================================================
	//���W�n
	//=======================================================
	D3DXVECTOR3 m_Pos;                                                          //�ʒu!
	D3DXVECTOR3 m_PosOld;                                                       //1f�O�̈ʒu!
	D3DXVECTOR3 m_SupportPos;                                                   //�����ʒu��ݒ�!
	D3DXVECTOR3 m_Move;                                                         //�ړ���!
	D3DXVECTOR3 m_Rot;                                                          //����!
	float m_fSpeed;
    //==========================================================================================

	D3DXMATRIX m_mtxWorld;                                                      //���[���h�ϊ��s��!
	D3DXCOLOR m_Col;                                                            //�F����!

	//=======================================================
	//�T�C�Y�֌W
	//=======================================================
	float m_fWidth;                                                             //����!
	float m_fFormarWidth;                                                       //���̉���!
	float m_fHeight;                                                            //����!
	float m_fFormarHeight;                                                      //���̍���!
	int m_nLife;                                                                //�̗�!
	int m_nMaxLife;                                                             //�ő�̗�!
	D3DXVECTOR3 m_Scale;                                                        //�g�嗦

	D3DXVECTOR3 m_AddScale;                                                     //���Z�g�嗦
	bool m_bUseAddScale;                                                        //���Z�g�嗦���g�p���邩�ǂ���
	//==========================================================================================

   
	//=======================================================
	//�A�j���[�V�����֌W
	//=======================================================
	float m_fAnimationSplit;                                                    //�A�j���[�V�����P����������̒l!
	int m_nAnimaionPattern;                                                     //�A�j���[�V�����p�^�[��!
	int m_nAnimationCnt;                                                        //�A�j���[�V�����J�E���g!
	int m_nMaxAnimationPattern;                                                 //�A�j���[�V�����p�^�[���̍ő吔!
	int m_nAnimationChange;                                                     //�A�j���[�V������ς���J�E���g��!
	bool m_bAnimFlag;                                                           //�A�j���[�V���������邩�ǂ���!
	int m_nTextureIndex;                                                        //�e�N�X�`���ԍ�!
	int m_nCntTime;          //�o�����Ă���̎��Ԃ��J�E���g����!
	//==========================================================================================

	//========================================
	//�J�[�u�n
	//========================================
	bool m_bUseCurve;           //�J�[�u�e�ɂ��邩�ǂ���!
	bool m_bUseLengthCurve;     //���_�����ɃJ�[�u���邩�ǂ���!
	float m_fAddCurveLength;    //�x�_����̋�����L�΂��l!
	float m_fCurveSpeed;        //�J�[�u����X�s�[�h!
	float m_fSupportCurveLength;//���_�����ɉ�]���鋗�������߂�!
	float m_fAddRot;            //�����̕␳!
	float m_fStartRot;          //�ŏ��̌���!
	//=======================================================================================================

	//========================================
	//��]�֌W
	//========================================
	bool m_bUsePolygonRot;   //�|���S������]�����邩�ǂ���!
	float m_fPolygonRotPower;//�|���S���̉�]��!
	//=======================================================================================================


	//========================================
	//�`��֌W
	//========================================
	bool m_bDraw;               //�`�悷�邩�ǂ���!
	//=======================================================================================================

	//====================================================
	//�_�Ŋ֌W
	//====================================================
	int m_nCntBlinkingFrame;     //�_�ŗp�̃J�E���g!
	int m_nMaxBlinkingFrame;     //�_�ŗp�̍ő�J�E���g��!
	bool m_bBlinkingAim;         //�J�E���g�𑝂₷�����炷�������߂�!
	bool m_bUseBlinking;         //�_�ł����邩�ǂ���!
	float m_fLimitBlinkingRatio; //�_�ł�����F�̔Z���̃��~�b�g!
	void BlinkingProcess();      //�_�ł����鏈��
	//====================================================================================================================

	//===============================
	//�d�͌n
	//===============================
	bool m_bUseGravity;      //�d�͂��g�p���邩�ǂ���!
	float m_fGravityPower;   //�d�͂̑傫��!
	float m_fAddGravity;     //���Z����d��!
	float m_fGravity;        //�d��!
	//================================================================================

	//===============================
	//�z�[�~���O�n
	//===============================
	bool m_bUseHorming;      //�z�[�~���O���g�p���邩�ǂ���!
	float m_fRotMove;        //�����ւ̈ړ���!
	//================================================================================

	//===============================
	//�����n
	//===============================
	bool m_bUseAddSpeed;     //�������g�p���邩�ǂ���!
	float m_fAddSpeed;       //�����x!
	bool m_bMultiplication;  //���x����Z�����邩�ǂ���!
	//================================================================================

};
//===================================================================================


#endif