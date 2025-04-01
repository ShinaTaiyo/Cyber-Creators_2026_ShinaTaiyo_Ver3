//====================================================
//
//�T���Q�O���FC++�Ŋ�ՂÂ���[object2d.h]
//Author:ShinaTaiyo
//
//====================================================

#ifndef _OBJECT2D_H_  
#define _OBJECT2D_H_

//======================
//�C���N���[�h
//======================
#include "main.h"
#include "object.h"
#include "renderer.h"
//==========================================

//======================
//�}�N����`
//======================
#define MAX_2DOBJECT (10)

//==========================================
//�I�u�W�F�N�g�QD�N���X
//==========================================
class CObject2D : public CObject
{
public:

	//======================================
	//�|���S���̐ݒ�^�C�v
	//======================================
	enum class POLYGONTYPE
	{
	    NORMAL = 0,   //���ʂɏo���|���S��
	    SENTERROLLING,//���S�_�����ɉ�]�ł���|���S��
		DOWNSENTERROLLING,//���S�_�����̉�]�ł���|���S��
	    DOWN,         //���S�_������
	    LEFT,         //���S�_������
	    RIGHT,        //���S�_���E��
		MAX
	};
	//======================================================================================================

	CObject2D(int nPri = 6, bool bUseintPri = false, CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_2D);     //�`�揇�ݒ�p�R���X�g���N�^
	~CObject2D() override;       //�f�X�g���N�^
	HRESULT Init() override;     //����������
	void Uninit() override;      //�I������
	void Update() override;      //�X�V����
	void Draw() override;        //�`�揈��
	void SetDeath() override;    //���S�t���O�ݒ菈��
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);  //�e�N�X�`�������蓖�Ă�

	//================================
	//��]�n
	//================================
	D3DXVECTOR3& GetRot() { return m_rot; }                                 //�������擾
	void SetRot(D3DXVECTOR3 Rot) { m_rot = Rot; }                           //������ݒ�
	void SetPolygonRotSpeed(float fSpeed) { m_fPolygonRotSpeed = fSpeed; }  //��]���x��ݒ�
	float& GetPolygonRotSpeed() { return m_fPolygonRotSpeed; }              //��]���x
	//========================================================================================================


	//===============
	//�擾�n
	//===============
	D3DXVECTOR3& GetPos() { return m_pos; }             //�ʒu�擾�p
	D3DXVECTOR3 GetSupportPos() { return m_SupportPos; }//�����ʒu���擾
	float GetWidth() { return m_fWidth; }               //�������擾
	float GetMaxWidth() { return m_fMaxWidth; }         //�ő剡�����擾
	float GetHeight() { return m_fHeight; }             //�������擾
	float GetMaxHeight() { return m_fMaxHeight; }       //�ő卂�����擾����
	D3DXCOLOR GetColor() { return m_col; }              //�F�������擾����
	//=======================================================================================================

	//===============
	//���ʌn
	//===============
	void SetUseFloating(float fFloatingRot,float fFloatingAddSpeed,float fLimitSpeed,bool bUseFloating)
	{
		m_fFloatingRot = fFloatingRot;          //��������
		m_fFloatingAddSpeed = fFloatingAddSpeed;//���Z���鑬�x
		m_fFloatingLimitSpeed = fLimitSpeed;    //���V���x�̃��~�b�g
		m_bUseFloating = bUseFloating;          //���V���g�p���邩�ǂ���
	}
	//=======================================================================================================

	//�ړ���
	D3DXVECTOR3& GetMove() { return m_Move; }
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }

	//�|���S���̎��
	POLYGONTYPE GetPolygonType() { return m_PolygonType; }
	void SetPolygonType(POLYGONTYPE Type) { m_PolygonType = Type; }

	//===============
	//�T�C�Y�n
	//===============
	void SetScale(D3DXVECTOR2 Scale) { m_Scale = Scale; }      //�g�嗦��ݒ肷��
	void SetUseScale(bool bUse) { m_bUseScale = bUse; }        //�g�嗦���g�p���邩�ǂ���

	//�g�嗦�̉��Z�ʐݒ�
	void SetUseAddScale(D3DXVECTOR2 AddScale, bool bUse) { m_AddScale = AddScale; m_bUseAddScale = bUse; m_bUseScale = true; }
	const D3DXVECTOR2& GetAddScale() const { return m_AddScale; }//�g�嗦�̉��Z�ʂ��擾
	D3DXVECTOR2& GetScale() { return m_Scale; }//�g�嗦���擾
	//=======================================================================================================

	//===============
	//���W�n
	//===============
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos;}                                 //�ʒu��ݒ�
	void SetSupportPos(D3DXVECTOR3 pos) { m_SupportPos = pos; }                  //�����ʒu��ݒ�
	void SetWidth(float fWidth) { m_fWidth = fWidth; }                           //������ݒ�
	void SetMaxWidth(float fWidth) { m_fMaxWidth = fWidth; }                     //�ő剡����ݒ�
	void SetHeight(float fHeight) { m_fHeight = fHeight; }                       //������ݒ�
	void SetMaxHeight(float fHeight) { m_fMaxHeight = fHeight; }                 //�ő卂����ݒ�
	void SetColor(D3DXCOLOR col, bool bAlphaOnly,float fAlpha);                  //�F��ݒ�
	void SetAnimInfo(int nMaxAnimationPattern, int nAnimationChange,bool bAnim); //�A�j���[�V�����֌W�̐ݒ�
	void SetAnim(int nAnim) { m_nAnimaionPattern = nAnim; }                      //�A�j���[�V�����ԍ���ݒ�
	void InitTextureOnly();                                                    //�e�N�X�`������������
	void SetTextureIndex(int nIdx) { m_nTextureIndex = nIdx; }                   //�e�N�X�`���ԍ����Z�b�g����
	int GetTextureIndex() { return m_nTextureIndex; }                            //�e�N�X�`���ԍ����擾����
    //==============================================================================================================================================================

	//==============================
	//�̗͊֌W
	//==============================
	void SetUseLife(bool bUse, int nLife, int nMaxLife)
	{
		m_bUseLife = bUse;
		m_nLife = nLife;
		m_nMaxLife = nMaxLife;
	}//�̗͂��g�p���邩�ǂ���

	int& GetLife() { return m_nLife; }//�̗͂��擾
	int& GetMaxLife() { return m_nMaxLife; }//�ő�̗͂��擾
	float& GetRatioLife() { return m_fRatioLife; }//�̗͂̊���
	//==============================================================================================================================================================

	//==============================
	//�_�Ŋ֌W
	//==============================
	void SetUseBlinking(bool bUse, int nMaxBlinkingFrame,float fLimitBlinkingRatio) {
		m_bUseBlinking = bUse;                      //�_�ł��g�p���邩�ǂ���
		m_nMaxBlinkingFrame = nMaxBlinkingFrame;    //�_�ōő�t���[��
		m_fLimitBlinkingRatio = fLimitBlinkingRatio;//�_�ł�����Z���̃��~�b�g
	}
	//==============================================================================================================================================================

	//==============================
	//�̗͊����֌W
	//==============================
	void SetUseLifeRatioColor(bool bUse) { m_bUseLifeRatioColor = bUse; }
	//==============================================================================================================================================================

	//================================
	//�`��n
	//================================
	//�`�悷�邩�ǂ����ݒ�A�擾
	void SetUseDraw(bool bUse) { m_bUseDraw = bUse; }
	const bool& GetUseDraw() const { return m_bUseDraw; }
	//==============================================================================================================================================================
protected:
	LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer() { return m_pVtxBuff; }
private:
	int m_nAnimaionPattern;              //�A�j���[�V�����p�^�[��!
	int m_nAnimationCnt;                 //�A�j���[�V�����J�E���g!
	int m_nMaxAnimationPattern;          //�A�j���[�V�����p�^�[���̍ő吔!
	int m_nAnimationChange;              //�A�j���[�V������ς���J�E���g��!
	bool m_bAnimFlag;                    //�A�j���[�V���������邩�ǂ���!
	float m_fLength;                     //�S���_�Ƃ̋���!
	float m_fAngle;                       //!
	float m_fAnimationSplit;             //�A�j���[�V�����P����������̒l!

	int m_nTextureIndex;                 //�e�N�X�`���ԍ��i�S�ẴI�u�W�F�N�g2D�h���N���X�������Ă���j!
	D3DXVECTOR3 m_pos;                   //�ʒu!
	D3DXVECTOR3 m_Move;                  //�ړ���!
	D3DXVECTOR3 m_SupportPos;            //�����ʒu!
	D3DXCOLOR m_col;                     //�F����!
	float m_fWidth;                      //����!
	float m_fHeight;                     //����!
	float m_fMaxWidth;                   //�ő剡��!
	float m_fMaxHeight;                  //�ő卂��!
	bool m_bUse;                         //�g�p���!
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;                //���_�o�b�t�@!
	LPDIRECT3DTEXTURE9 m_pTexture;                     //�e�N�X�`��!
	POLYGONTYPE m_PolygonType;                         //�|���S���̎��!

	//================================
	//�T�C�Y�n
	//================================
	D3DXVECTOR2 m_Scale;     //�g�嗦!
	D3DXVECTOR2 m_AddScale;  //���Z����g�嗦
	bool m_bUseScale;        //�g�嗦���g�p���邩�ǂ���!
	bool m_bUseAddScale;     //�g�嗦�̉��Z�����邩�ǂ���
	void ScaleProcess();     //�g�嗦�̏���
	//==============================================================================================================================================================

	//================================
	//���V�n
	//================================
	float m_fFloatingRot;   //�������������������!
	float m_fFloatingSpeed; //���������̑��x!
	float m_fFloatingAddSpeed;//���V�̋�����������Ƃ��̉��Z���x!
	float m_fFloatingLimitSpeed;//���V���x�̃��~�b�g!
	bool m_bUseFloating;    //�����������g�p���邩�ǂ���!
	void FloatingProcess(); //���������̏���
	//==============================================================================================================================================================

	//================================
	//�_�ŏ���
	//================================
	int m_nCntBlinkingFrame;   //�_�ŗp�̃J�E���g!
	int m_nMaxBlinkingFrame;   //�_�ŗp�̍ő�J�E���g��!
	bool m_bBlinkingAim;       //�J�E���g�𑝂₷�����炷�������߂�!
	bool m_bUseBlinking;       //�_�ł����邩�ǂ���!
	float m_fLimitBlinkingRatio; //�_�ł�����F�̔Z���̃��~�b�g!
	void BlinkingProcess();//�_�ł����鏈��
	//==============================================================================================================================================================

	//================================
	//�̗͌n
	//================================
	int m_nLife;//�̗�!
	int m_nMaxLife;//�ő�̗�!
	bool m_bUseLife;//�̗͂��g�p���邩�ǂ���!
	float m_fRatioLife;//�̗͂̊���!
	void SubLifeProcess();//�̗͂��ւ炷����

	bool m_bUseLifeRatioColor;//�̗͊����ɂ���ĐF������ς��邩�ǂ���!
	void LifeRatioColorProcess();//�̗͊����ɂ���ĐF������ς��鏈��
	//==============================================================================================================================================================

	//================================
	//��]�n
	//================================
	D3DXVECTOR3 m_rot;                   //����!
	float m_fPolygonRotSpeed;                   //��]���x!
	//==============================================================================================================================================================

	//================================
	//�`��n
	//================================
	bool m_bUseDraw;//�`�悷�邩�ǂ���
	//==============================================================================================================================================================

};

#endif
