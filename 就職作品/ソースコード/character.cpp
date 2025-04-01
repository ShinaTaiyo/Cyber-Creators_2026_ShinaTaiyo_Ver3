//==================================================================================================================
//
//�Q�O�Q�T�N�P���W���F���[�V�����L�����N�^�[�N���X�����[character.cpp]
//Author:ShinaTaiyo
//
//===================================================================================================================

//=============================================================
//�C���N���[�h�t�@�C��
//=============================================================
#include "character.h"
#include "manager.h"
#include "objectXInfo.h"
#include "debugtext.h"
#include "calculation.h"
//===================================================================================================================

//=============================================================
//�ÓI�����o�錾
//=============================================================
vector<CCharacter::CHARACTERMOTIONDATA> CCharacter::s_VecMotionInfo = {};//�L�����N�^�[���Ƃ̃��[�V�������
//===================================================================================================================

//=============================================================
//�R���X�g���N�^
//=============================================================
CCharacter::CCharacter(int nPri, bool bUseintPri, CObject::TYPE type, CObject::OBJECTTYPE ObjType) : CObjectX(nPri, bUseintPri, type, ObjType), 
m_nIdxCharacter(0), m_NowMotionInfo({})
{

}
//===================================================================================================================

//=============================================================
//�f�X�g���N�^
//=============================================================
CCharacter::~CCharacter()
{

}
//===================================================================================================================

//=============================================================
//����������
//=============================================================
HRESULT CCharacter::Init()
{
    CObjectX::Init();//�I�u�W�F�N�gX�̏���������
	return S_OK;
}
//===================================================================================================================

//=============================================================
//�I������
//=============================================================
void CCharacter::Uninit()
{
    CObjectX::Uninit();//�I�u�W�F�N�gX�̏I������
}
//===================================================================================================================

//=============================================================
//�X�V����
//=============================================================
void CCharacter::Update()
{
    CObjectX::Update();//�I�u�W�F�N�gX�̍X�V����

    //�f�o�b�O�\��
    CManager::GetDebugText()->PrintDebugText("�d�͂��g�p���邩�ǂ����F%d\n", GetMoveInfo().GetUseGravity());

    for (auto it : m_VecModelParts)
    {
        it->ExtraUpdate();//���ꂼ��̃��f���p�[�c�̍X�V�������Ă�
    }
}
//===================================================================================================================

//=============================================================
//�`�揈��
//=============================================================
void CCharacter::Draw()
{
    CObjectX::Draw();//�I�u�W�F�N�gX�̕`�揈��

    for (auto it : m_VecModelParts)
    {
        it->ExtraDraw();//���ꂼ��̃��f���p�[�c�̕`�揈�����ĂԁiExtraDraw�́A�I�u�W�F�N�g��Drawall����͌Ă΂�Ȃ��悤�ɂ��邽��)
    }
}
//===================================================================================================================

//=============================================================
//���S�t���O�ݒ菈��
//=============================================================
void CCharacter::SetDeath()
{
    CObjectX::SetDeath();//�I�u�W�F�N�gX�̎��S�t���O�ݒ菈��

    for (auto it : m_VecModelParts)
    {
        it->SetUseDeath(true);//���S�t���O���g�p����
        it->SetDeath();       //���S�t���O��ݒ肷��
    }

    m_VecModelParts.clear();//�S�Ẵ��f���p�[�c�̎��S�t���O�̐ݒ肪�I������̂ŁA�f�[�^���N���A����
}
//===================================================================================================================

//=============================================================
//���̃��[�V������ݒ肷��
//=============================================================
void CCharacter::SetNextMotion(int nNext)
{
    if (nNext >= s_VecMotionInfo[m_nIdxCharacter].nNumMotion)
    {//�w�肵�����[�V�������A���[�V�����ő吔�i�z��j�𒴂��Ă����珉�����[�V�������w�肷��
        nNext = 0;
    }
    m_NowMotionInfo.nNextMotion = nNext;//�ݒ肷��
}
//===================================================================================================================

//=============================================================
//�_���[�W��^����
//=============================================================
void CCharacter::SetDamage(int nDamage, int nHitStopTime)
{
    for (auto ModelPartsIt : m_VecModelParts)
    {//�͈̓x�[�Xfor��
        ModelPartsIt->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), nHitStopTime, false, false, true);//�S�Ẵ��f���p�[�c�̐F������ς���
    }

    CObjectX::SetDamage(nDamage, nHitStopTime);//�I�u�W�F�N�gX�̃_���[�W����
}
//===================================================================================================================

//=============================================================
//���[�V�����������蓖�Ă�
//=============================================================
int CCharacter::RegistMotion(string String, CCharacter* pCharacter)
{
	int nIdx = 0;//���[�V�����ԍ�
	for (auto it = s_VecMotionInfo.begin(); it != s_VecMotionInfo.end(); it++)
	{
		if (it->FileName == String)
		{//�w�肵���t�@�C���p�X�����[�V�������̓��I�z��ɑ��݂�����
            LoadModelParts(String, pCharacter);//���ɑ��݂���t�@�C���Ȃ̂ŁA���f���p�[�c��񂾂��ǂݍ���
            pCharacter->m_nIdxCharacter = nIdx;//�L�����N�^�[�ԍ���ݒ�
			return nIdx;
		}
		nIdx++;
	}

	SaveMotion(String);//�܂����[�V���������݂��Ă��Ȃ��̂ŁA�V�������[�V��������ۑ�����
    LoadModelParts(String, pCharacter);//���f���p�[�c����ǂݍ���
    pCharacter->m_nIdxCharacter = nIdx;//�L�����N�^�[�ԍ���ݒ�
	return nIdx;
}
//===================================================================================================================

//=============================================================
//�L�����N�^�[�̃T�C�Y��ݒ肷��
//=============================================================
void CCharacter::SetSize()
{
    CObjectX::SetSize();//�T�C�Y��ݒ� 
}
//===================================================================================================================

//=============================================================
//���[�V�����������s��
//=============================================================
void CCharacter::MotionProcess()
{
    m_NowMotionInfo.nMotionOld = m_NowMotionInfo.nNowMotion;    //1f�O�̃��[�V������ݒ肷��
    m_NowMotionInfo.nNowMotion = m_NowMotionInfo.nNextMotion;   //���݂̃��[�V�������X�V����

    if (m_NowMotionInfo.nNowMotion != m_NowMotionInfo.nMotionOld)
    {//1f�O�̃��[�V�����ƈقȂ�ꍇ�A�t���[�����ƃL�[�J�E���g�����Z�b�g���A�u�����h�J�n
        m_NowMotionInfo.nCntFrame = 0;
        m_NowMotionInfo.nCntKey = 0;
        m_NowMotionInfo.bNowBrending = true;
        CManager::GetDebugText()->PrintDebugText("�t���[�����A�L�[�J�E���g���Z�b�g�I\n");
    }

    //�Q�Ƃ���l��ϐ��Ɋi�[���Ă���
    int nNowMotion = m_NowMotionInfo.nNowMotion;                                                      //���݂̃��[�V�����ԍ����i�[����
    int nNowKey = m_NowMotionInfo.nCntKey;                                                            //���݂̃L�[���i�[����
    int nNextKey = (nNowKey + 1) % s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].nNumKey;    //���̃L�[�ԍ����i�[����
    int nNowFrame = m_NowMotionInfo.nCntFrame;                                                        //�t���[�������i�[
    int nMaxFrame = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNowKey].nFrame; //���݂̃L�[�Z�b�g�̍ő�t���[�������i�[
    float fRatioFrame = static_cast<float>(nNowFrame) / static_cast<float>(nMaxFrame);                //���݂̃t���[���̍ő�t���[���ɑ΂��Ă̊������i�[


    int nSize = m_VecModelParts.size();        //�z��̑傫�����i�[
    int nBrendCheck = 0;                       //���[�V�����u�����h���������Ă���ꍇ�A���[�V�����u�����h�������������f���p�[�c�̐����J�E���g����
    bool bOverArray = false;                   //�z��O�A�N�Z�X���������ǂ���
    for (int nCntParts = 0; nCntParts < nSize; nCntParts++)
    {
        D3DXVECTOR3 NowPos = { 0.0f,0.0f,0.0f };//���݂̃L�[�̈ʒu
        D3DXVECTOR3 NextPos = { 0.0f,0.0f,0.0f };//���̃L�[�̈ʒu
        D3DXVECTOR3 NowRot = { 0.0f,0.0f,0.0f };//���݂̃L�[�̌���
        D3DXVECTOR3 NextRot = { 0.0f,0.0f,0.0f };//���̃L�[�̌���

        if (m_nIdxCharacter >= 0 && static_cast<unsigned int>(m_nIdxCharacter) < s_VecMotionInfo.size())
        {
            //�z��O�`�F�b�N
            if (nNowMotion >= 0 && static_cast<unsigned int>(nNowMotion) < s_VecMotionInfo[m_nIdxCharacter].VecMotion.size())
            {
                if (nNowKey >= 0 && static_cast<unsigned int>(nNowKey) < s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet.size())
                {
                    if (nCntParts >= 0 && static_cast<unsigned int>(nCntParts) < s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNowKey].VecKey.size())
                    {

                    }
                    else
                    {
                        assert("���f���p�[�c���z��O�A�N�Z�X");
                    }
                }
                else
                {
                    assert("�L�[���z��O�A�N�Z�X");
                }
            }
            else
            {
                assert("���[�V�������z��O�A�N�Z�X");
            }
        }
        else
        {
            assert("�L�����N�^�[���z��O�A�N�Z�X");
        }

        //���[�V�����u�����h�����邩���Ȃ����ɂ���ĎQ�Ƃ���ʒu��������ς��
        if (m_NowMotionInfo.bNowBrending == false)
        {//���[�V�����u�����h��Ԃ���Ȃ����
            NowPos = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNowKey].VecKey[nCntParts].Pos;   //���݂̃��[�V�����̃L�[�Z�b�g�̃L�[���̈ʒu
            NextPos = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNextKey].VecKey[nCntParts].Pos; //���̃��[�V�����̃L�[�Z�b�g�̃L�[���̈ʒu

            NowRot = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNowKey].VecKey[nCntParts].Rot;   //���݂̃��[�V�����̃L�[�Z�b�g�̃L�[���̌���
            NextRot = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNextKey].VecKey[nCntParts].Rot; //���̃��[�V�����̃L�[�Z�b�g�̃L�[���̌���
        }
        else
        {//���[�V�����u�����h��ԂȂ��

            //���f���p�[�c�̌��݂̈ʒu���珙�X�Ƀu�����h���郂�[�V�����̃L�[�̈ʒu�ɋ߂Â��Ă���
            NowPos = m_VecModelParts[nCntParts]->GetPosInfo().GetPos();
            NextPos = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[0].VecKey[nCntParts].Pos + m_VecModelParts[nCntParts]->GetPosInfo().GetSupportPos();

            //���f���p�[�c�̌��݂̌������珙�X�Ƀu�����h���郂�[�V�����̃L�[�̌����ɋ߂Â��Ă���
            NowRot = m_VecModelParts[nCntParts]->GetRotInfo().GetRot();
            NextRot = s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[0].VecKey[nCntParts].Rot;
        }

        D3DXVECTOR3 DifferencePos = NextPos - NowPos;//���̈ʒu�ƌ��݂̈ʒu�̍��������߂�
        D3DXVECTOR3 DifferenceRot = NextRot - NowRot;//���̌����ƌ��݂̌����̍��������߂�

        if (m_NowMotionInfo.bNowBrending == true)
        {//���[�V�����u�����h�������������ǂ����𔻒�
            if (DifferencePos.x < s_fMotionBrend_CheckDefference && 
                DifferencePos.y < s_fMotionBrend_CheckDefference && 
                DifferencePos.z < s_fMotionBrend_CheckDefference &&
                DifferenceRot.x < s_fMotionBrend_CheckDefference && 
                DifferenceRot.y < s_fMotionBrend_CheckDefference && 
                DifferenceRot.z < s_fMotionBrend_CheckDefference)
            {//�S�Ă̎��̌����̍������w�肵���l�ȉ��ɂȂ�����u�����h����������
                nBrendCheck++;
            }
        }

        //�W���o�����b�N�����
        CCalculation::CorrectionRot(DifferenceRot.x);
        CCalculation::CorrectionRot(DifferenceRot.y);
        CCalculation::CorrectionRot(DifferenceRot.z);

        //��L�ŋ��߂��f�[�^���g�p���A���[�V���������ʒu�ƌ��������߂�
        D3DXVECTOR3 DecisionPos = { 0.0f,0.0f,0.0f };
        D3DXVECTOR3 DecisionRot = { 0.0f,0.0f,0.0f };

        if (m_NowMotionInfo.bNowBrending == false)
        {//���݂̃L�[�̒l�Ǝ��̃L�[�̒l�Ɍ����ē�����
            DecisionPos = (DifferencePos * fRatioFrame) + m_VecModelParts[nCntParts]->GetPosInfo().GetSupportPos() + NowPos;//�e�p�[�c�̏����ʒu����Ɉʒu������
            DecisionRot = (DifferenceRot * fRatioFrame) + NowRot;//�e�p�[�c�̍ŏ��̌�����Rot0.0f�Ȃ̂ŁA�ʒu�݂����ȕ␳�͂���Ȃ�
        }
        else
        {//�u�����h����
            DecisionPos = (DifferencePos * s_fMotionBrend_Speed) + NowPos;
            DecisionRot = (DifferenceRot * s_fMotionBrend_Speed) + NowRot;
        }

        //�ʒu�ƌ�����ݒ�
        m_VecModelParts[nCntParts]->GetPosInfo().SetPos(DecisionPos);
        m_VecModelParts[nCntParts]->GetRotInfo().SetRot(DecisionRot);
    }

    if (m_NowMotionInfo.bNowBrending == false)
    {//���[�V�����u�����h���I���܂ł̓t���[�������J�E���g���Ȃ�
        m_NowMotionInfo.nCntFrame++;//�t���[�������{�P
        if (m_NowMotionInfo.nCntFrame >= s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].VecKeySet[nNowKey].nFrame)
        {//�t���[���������݂̃��[�V�����̍ő�ɒB������A�L�[�����ɐi�߁A�t���[�������O�ɂ���
            m_NowMotionInfo.nCntKey++;
            m_NowMotionInfo.nCntFrame = 0;

            if (m_NowMotionInfo.nCntKey >= s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].nNumKey)
            {//�L�[�����݂̃��[�V�����̍ő�ɒB������L�[���O�ɂ���
                m_NowMotionInfo.nCntKey = 0;

                if (s_VecMotionInfo[m_nIdxCharacter].VecMotion[nNowMotion].bLoop == false)
                {//���[�v���Ȃ��Ȃ珉�����[�V�����ɖ߂�
                    m_NowMotionInfo.nNextMotion = 0;
                }
            }
        }
    }
    else
    {//���[�V�����u�����h���Ȃ�
        if (nBrendCheck == nSize)
        {//�S�Ẵp�[�c�̃��[�V�����u�����h�����������ꍇ�A���[�V�����u�����h���I������
            m_NowMotionInfo.bNowBrending = false;
        }
    }
}
//===================================================================================================================

//=============================================================
//���[�V��������ۑ�����
//=============================================================
void CCharacter::SaveMotion(string MotionFileName)
{
    //=========================================
    //���[�V�������̓ǂݍ��݂Ŏg���ϐ�
    //=========================================
    int nNumMotion = 0;      //���[�V�����̐�
    int nNumKeySet = 0;      //�L�[�Z�b�g�̐�
    int nNumKey = 0;         //�L�[�̐�
    bool bLoop = false;      //���[�v���邩�ǂ����𐔒l�œǂݎ��
    //====================================================================

    CHARACTERMOTIONDATA CharacterMotionData = {};
    CharacterMotionData.FileName = MotionFileName;//�V�������[�V�������̃t�@�C���p�X��ۑ�����
    fstream ReadingFile;//�ǂݎ��p�t�@�C��
    string Reading_Buff;//�ǂݎ��p�t�@�C���̕�����

    //�t�@�C�����J��
    ReadingFile.open(MotionFileName, ios::in);//�ǂݎ�胂�[�h�ŊJ��

    while (!ReadingFile.eof())//EOF�ɓ��B����܂ŌJ��Ԃ�
    {
        ReadingFile >> Reading_Buff;
    
        if (Reading_Buff == "#")
        {//���̍s���X�L�b�v
            getline(ReadingFile, Reading_Buff);
        }
        else if (Reading_Buff == "MOTIONSET")
        {//�t�@�C���ǂݍ��݂��I��点��
            CharacterMotionData.VecMotion.push_back({});//���[�V�������̐ݒ������̂ŁA�����l��ǉ��iKEYSET��vector�Ȃ̂Ŏ����I�ɏ����������)
            auto MotionIt = CharacterMotionData.VecMotion.begin();//�ݒ肷�郂�[�V�����̐擪�C�e���[�^���擾
            if (nNumMotion != 0)
            {
                advance(MotionIt,nNumMotion);//���݂̃��[�V�����̑����@�[�@�P�A�C�e���[�^��i�߂�i�ŏ��͂O�Ȃ̂ŁA���[�V�����������P�̎��͂O���w����������j
            }
            while (Reading_Buff != "END_MOTIONSET")
            {//���[�V�����ݒ肪�I���܂ŌJ��Ԃ�
                ReadingFile >> Reading_Buff;//�P���ǂݎ��

                if (Reading_Buff == "#")
                {//���̍s��ǂݍ���ŃX�L�b�v
                    getline(ReadingFile, Reading_Buff);
                }
                else if (Reading_Buff == "LOOP")
                {//���[�v���邩�ǂ���
                    ReadingFile >> Reading_Buff;//�C�R�[��
                    ReadingFile >> MotionIt->bLoop;//���[�v���邩�ǂ�����ݒ�
                }
                else if (Reading_Buff == "NUM_KEY")
                {//�L�[�̐�
                    ReadingFile >> Reading_Buff;//�C�R�[��
                    ReadingFile >> MotionIt->nNumKey;//�L�[������ݒ�
                }
                else if (Reading_Buff == "KEYSET")
                {//�L�[���ݒ�
                    MotionIt->VecKeySet.push_back({});//���݂̃��[�V�����̃L�[�Z�b�g����ݒ肷��̂ŁA�L�[�Z�b�g���̏����l����
                    auto KeySetIt = MotionIt->VecKeySet.begin();//�L�[�Z�b�g���̐擪���C�e���[�^�Ŏw��
                    if (nNumKeySet != 0)
                    {
                        advance(KeySetIt, nNumKeySet);//���݂̃L�[�Z�b�g���̑������C�e���[�^��i�߂�
                    }
                    while (Reading_Buff != "END_KEYSET")
                    {//�L�[�ݒ肪�I���܂ŌJ��Ԃ�
                        ReadingFile >> Reading_Buff;//�P���ǂݎ��
                        if (Reading_Buff == "#")
                        {//���̍s��ǂݍ���ŃX�L�b�v
                            getline(ReadingFile, Reading_Buff);
                        }
                        else if (Reading_Buff == "FRAME")
                        {//�t���[����
                            ReadingFile >> Reading_Buff;//�C�R�[��
                            ReadingFile >> KeySetIt->nFrame;//�t���[������ݒ�
                        }
                        else if (Reading_Buff == "KEY")
                        {//�L�[�f�[�^�ݒ�
                            KeySetIt->VecKey.push_back({});//���݂̃L�[�Z�b�g���̃L�[�f�[�^��ݒ肷��
                            auto KeyIt = KeySetIt->VecKey.begin();//�L�[�f�[�^���̐擪���C�e���[�^�Ŏw��
                            if (nNumKey != 0)
                            {
                                advance(KeyIt, nNumKey);//���݂̃L�[�f�[�^���̑����C�e���[�^��i�߂�
                            }
                            while (Reading_Buff != "END_KEY")
                            {//�L�[�f�[�^�̐ݒ肪�I���܂ŌJ��Ԃ�
                                ReadingFile >> Reading_Buff;//�P���ǂݎ��
                                if (Reading_Buff == "#")
                                {//���̍s��ǂݍ���ŃX�L�b�v
                                    getline(ReadingFile, Reading_Buff);
                                }
                                else if(Reading_Buff == "POS")
                                {//�ʒu��ǂݍ���
                                    ReadingFile >> Reading_Buff;//�C�R�[��
                                    ReadingFile >> KeyIt->Pos.x; //�ʒuX
                                    ReadingFile >> KeyIt->Pos.y; //�ʒuY
                                    ReadingFile >> KeyIt->Pos.z; //�ʒuZ
                                }
                                else if (Reading_Buff == "ROT")
                                {//������ǂݍ���
                                    ReadingFile >> Reading_Buff; //�C�R�[��
                                    ReadingFile >> KeyIt->Rot.x; //����X
                                    ReadingFile >> KeyIt->Rot.y; //����Y
                                    ReadingFile >> KeyIt->Rot.z; //����Z
                                }
                                else if (Reading_Buff == "END_KEY")
                                {//�L�[�f�[�^�ݒ�I��
                                    nNumKey++;
                                }
                            }
                        }
                        else if (Reading_Buff == "END_KEYSET")
                        {//�L�[�Z�b�g�ݒ�I��
                            nNumKeySet++;
                            nNumKey = 0;
                        }
                    }
                }
                else if (Reading_Buff == "END_MOTIONSET")
                {//���[�V�����ݒ�I��
                    MotionIt->nNumKey = nNumKeySet;//���ݐݒ肵�Ă������[�V�����̃L�[������ۑ�
                    nNumMotion++;
                    nNumKey = 0;
                    nNumKeySet = 0;
                }
            }
        }
    }

    CharacterMotionData.nNumMotion = nNumMotion;//���[�V�����������i�[����

    s_VecMotionInfo.push_back(CharacterMotionData);//��L�̏����ŐV�������[�V��������ݒ肵���̂ŃL�����N�^�[���[�V�����̃x�N�^�[�ɕۑ�

    ReadingFile.close();//�t�@�C�������
}
//===================================================================================================================

//=============================================================
//���[�V�����t�@�C�����烂�f���p�[�c��ǂݍ���
//=============================================================
void CCharacter::LoadModelParts(string MotionFileName, CCharacter* pCharacter)
{
    fstream ReadingFile;//�ǂݎ��p�t�@�C��
    string Reading_Buff;//�ǂݎ��p�t�@�C���̕�����
    CObjectXInfo* pObjectXInfo = CManager::GetObjectXInfo();

    int nIdxParent = 0;//�e�}�g���b�N�X�̔ԍ�
    int nPartsIdx = 0;//�e�p�[�c�̃C���f�b�N�X
    int nNumSetParts = 0;//�ݒ肵���p�[�c��
    D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };//�ʒu
    D3DXVECTOR3 Rot = { 0.0f,0.0f,0.0f };//����
    //�t�@�C�����J��
    ReadingFile.open(MotionFileName, ios::in);//�ǂݎ�胂�[�h�ŊJ��
    while (!ReadingFile.eof())//EOF�ɓ��B����܂ŌJ��Ԃ�
    {
        ReadingFile >> Reading_Buff;//�P���ǂݍ���
        if (Reading_Buff == "#")
        {//���̍s���X�L�b�v
            getline(ReadingFile, Reading_Buff);
        }
        else if (Reading_Buff == "MODEL_FILENAME")
        {//���f���p�[�c��ǂݍ���
            ReadingFile >> Reading_Buff;//�C�R�[��
            ReadingFile >> Reading_Buff;//���f���p�[�c�̃p�X
            pCharacter->m_VecModelParts.push_back(CModelParts::Create(Reading_Buff,pCharacter->GetType()));//���f���p�[�c����ǉ�
        }
        else if (Reading_Buff == "CHARACTERSET")
        {//�e���f���p�[�c�̏���ݒ肷��
            while (Reading_Buff != "END_CHARACTERSET")
            {//�e���f���p�[�c�̐ݒ肪�I���܂ŌJ��Ԃ�
                ReadingFile >> Reading_Buff;//�P���ǂݍ���
                if (Reading_Buff == "#")
                {//���̍s���X�L�b�v
                    getline(ReadingFile, Reading_Buff);
                }
                else if (Reading_Buff == "PARTSSET")
                {//�P�̃p�[�c�̐ݒ������
                    auto PartsIt = pCharacter->m_VecModelParts.begin();//�p�[�c�̃x�N�^�[�̐擪���w��
                    if (nNumSetParts != 0)
                    {
                        advance(PartsIt, nNumSetParts);//�ŏ��̃C�e���[�^���w���̂͂O�ԂȂ̂ŁA�ݒ肵���p�[�c���[�P���A�C�e���[�^��i�߂�
                    }
                    while (Reading_Buff != "END_PARTSSET")
                    {//���f���p�[�c�̐ݒ肪�I���܂ŌJ��Ԃ�
                        ReadingFile >> Reading_Buff;//�P���ǂݍ���
                        if (Reading_Buff == "#")
                        {//���̍s���X�L�b�v
                            getline(ReadingFile, Reading_Buff);
                        }
                        else if (Reading_Buff == "INDEX")
                        {//�C���f�b�N�X
                            ReadingFile >> Reading_Buff;//�C�R�[��
                            ReadingFile >> nPartsIdx;   //���f���p�[�c�̃C���f�b�N�X
                            (*PartsIt)->SetPartsIdx(nPartsIdx);//���f���p�[�c���Ƃ̃C���f�b�N�X��ݒ�
                        }
                        else if (Reading_Buff == "PARENT")
                        {//�e���f���p�[�c�ԍ�
                            ReadingFile >> Reading_Buff;//�C�R�[��
                            ReadingFile >> nIdxParent;   //���f���p�[�c�̃C���f�b�N�X
                            //�e���f���p�[�c��ݒ�
                            if (nIdxParent >= 0)
                            {//���f���p�[�c�ɐe�����݂���
                                if (static_cast<unsigned int>(nIdxParent) < pCharacter->m_VecModelParts.size())
                                {
                                    (*PartsIt)->GetDrawInfo().SetUseMatrixChild(true, &pCharacter->m_VecModelParts[nIdxParent]->GetDrawInfo().GetMatrixWorld());//���f���p�[�c���Ƃ̃C���f�b�N�X��ݒ�
                                }
                                else
                                {
                                    assert("���f���p�[�c�̃C���f�b�N�X���z��O�A�N�Z�X");
                                }
                            }
                            else
                            {//���f���p�[�c�ɐe�����݂��Ȃ��̂ŁA�L�����N�^�[��e�ɂ���
                                (*PartsIt)->GetDrawInfo().SetUseMatrixChild(true, &pCharacter->GetDrawInfo().GetMatrixWorld());//���f���p�[�c���Ƃ̃C���f�b�N�X��ݒ�
                            }
                        }
                        else if (Reading_Buff == "POS")
                        {//�ʒu
                            ReadingFile >> Reading_Buff;//�C�R�[��
                            ReadingFile >> Pos.x;       //�ʒuX
                            ReadingFile >> Pos.y;       //�ʒuY
                            ReadingFile >> Pos.z;       //�ʒuZ

                            (*PartsIt)->GetPosInfo().SetSupportPos(Pos); //�����ʒu��ݒ�
                            (*PartsIt)->GetPosInfo().SetPos(Pos);        //�ʒu��ݒ�
                        }
                        else if (Reading_Buff == "ROT")
                        {//����
                            ReadingFile >> Reading_Buff; //�C�R�[��
                            ReadingFile >> Rot.x;        //����X
                            ReadingFile >> Rot.y;        //����Y
                            ReadingFile >> Rot.z;        //����Z

                            (*PartsIt)->GetPosInfo().SetSupportPos(Pos); //����������ݒ�
                            (*PartsIt)->GetPosInfo().SetPos(Pos);        //������ݒ�
                        }
                        else if (Reading_Buff == "END_PARTSSET")
                        {//�p�[�c�ݒ���I������
                            nNumSetParts++;//�ݒ肪���������p�[�c���{�P
                        }
                    }
                }
            }
        }
    }

    ReadingFile.close();//�t�@�C�������
}
//===================================================================================================================