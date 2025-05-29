//====================================
//
//����Q�[�����̂Q�F�u�l�[���n���^�[�vver:1.1�i�����ŁI�j
//Author:ShinaTaiyo
//
//====================================
#include<stdio.h>
#include<string.h>//��������g���Ƃ��Ɏg�p����B
#include<stdlib.h>//�����_���֐����g���Ƃ��Ɏg�p����B
#include<time.h>//�^�C���֐����g���Ƃ��Ɏg�p����B
#include<Windows.h>
#include<mmsyscom.h>
#pragma comment(lib,"Winmm.lib")


//�}�N����`
#define MAX_PLAYER (2)
#define MAX_NAME (256)
#define PlayerNumber1 (0)
#define PlayerNumber2 (1)
#define MAX_TURN (200)


//�񋓌^��`
typedef enum
{
	MODE_NOTHIING = 0,//�������Ȃ�(0)
	MODE_NORMALATTACK,//�ʏ�U��(1)
	MODE_SKILLATTACK,//�X�L���U��(2)
	MODE_USEITEM,//�A�C�e�����g��(3)
	MODE_MAX

}MODE;


//�\���̒�`
typedef struct
{
	char aName[MAX_NAME];//�v���C���[�̖��O
	char aJob[MAX_NAME];//�v���C���[�̖�E
	int nCheckJob;//��E�̔ԍ����L�q����B
	float fLife;//�v���C���[�̗̑�
	float fATK;//�v���C���[�̍U����
	float fDEF;//�v���C���[�̖h���
	float fMP;//�v���C���[�̃}�W�b�N�|�C���g
}Player;



//�v���g�^�C�v�錾
void InputData(Player* pPlayer, float* pLength);//�v���C���[�\���̂̃|�C���^��float�^�̃|�C���^��錾����B

void SetStatus(Player* pPlayer, float* pLength);

void SelectJob(Player* pPlayer, int* pSelectJob);//��E��I�ԃ��[�h��ݒ肷��B

void SetBattle(Player* pPlayer);

void Player1NormalAttack(Player* pPlayer, bool Player2CounterFlag, bool bPlayer1CriticalLineBuff);//�v���C���[�P���ʏ�U��������Ƃ��ɌĂяo���֐��B�J�E���^�[�̃t���O��true�ɂȂ��Ă���ꍇ�A�v���C���[�Q���m���ŃJ�E���^�[�𔭓�������B

void Player2NormalAttack(Player* pPlayer, bool Player1CounterFlag, bool bPlayer2CriticalLineBuff);//�v���C���[�Q���ʏ�U��������Ƃ��ɌĂяo���֐��B�J�E���^�[�̃t���O��true�ɂȂ��Ă���ꍇ�A�v���C���[�P���m���ŃJ�E���^�[�𔭓�������B

void Player1InpactEdge(Player* pPlayer);//�v���C���[�P�����m�̃X�L���i�P�j�A�u�C���p�N�g�G�b�W�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player2InpactEdge(Player* pPlayer);//�v���C���[�Q�����m�̃X�L���i�P�j�A�u�C���p�N�g�G�b�W�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player1RecoverHeal(Player* pPlayer, float fPlayer1MaxHp);//�v���C���[�P�����m�̃X�L���i�Q�j�A�u���J�o�[�q�[���v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player2RecoverHeal(Player* pPlayer, float fPlayer2MaxHp);//�v���C���[�Q�����m�̃X�L���i�Q�j�A�u���J�o�[�q�[���v�𔭓����鎞�ɌĂяo���֐��B

void Player1RainArrow(Player* pPlayer);//�v���C���[�P����l�̃X�L���i�P�j�A�u���C���A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player2RainArrow(Player* pPlayer);//�v���C���[�Q����l�̃X�L���i�P�j�A�u���C���A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player1PowerDownArrow(Player* pPlayer);//�v���C���[�P����l�̃X�L���i�Q�j�A�u�p���_�E���A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player2PowerDownArrow(Player* pPlayer);//�v���C���[2����l�̃X�L���i�Q�j�A�u�p���_�E���A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player1StarArrow(Player* pPlayer);//�v���C���[�P����l�̃X�L���i�R�j�A�u�X�^�[�A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player2StarArrow(Player* pPlayer);//�v���C���[�Q����l�̃X�L���i�R�j�A�u�X�^�[�A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player1DefenceLine(Player* pPlayer);//�v���C���[�P���ޏ��̃X�L���i�P�j�A�u���̐w�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player2DefenceLine(Player* pPlayer);//�v���C���[�Q���ޏ��̃X�L���i�P�j�A�u���̐w�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player1HolyRay(Player* pPlayer);//�v���C���[�P���ޏ��̃X�L���i�Q�j�A�u�z�[���[���C�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player2HolyRay(Player* pPlayer);//�v���C���[�Q���ޏ��̃X�L���i�Q�j�A�u�z�[���[���C�v�𔭓�����Ƃ��ɌĂяo���֐��B

//void Player1CriticalLine(Player* pPlayer);//�v���C���[�P���ޏ��̃X�L���i�R�j�A�u��S�̐w�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player1BloodyBite(Player* pPlayer);//�v���C���[�P���z���S�̃X�L���i�P�j�A�u�u���b�e�B�o�C�g�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player2BloodyBite(Player* pPlayer);//�v���C���[�Q���z���S�̃X�L���i�P�j�A�u�u���b�f�B�o�C�g�𔭓�����Ƃ��ɌĂяo���֐��B

void Player1CrazyGracity(Player* pPlayer);//�v���C���[�P���z���S�̃X�L���i�Q�j�A�u�N���C�W�[�O���r�e�B�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player2CrazyGravity(Player* pPlayer);//�v���C���[�Q���z���S�̃X�L���i�Q�j�A�u�N���C�W�[�O���r�e�B�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player1PhantomStab(Player* pPlayer);//�v���C���[�P���z���S�̃X�L���i�R�j�A�u�t�@���g���X�^�u�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player2PhantomStab(Player* pPlayer);//�v���C���[�Q���z���S�̃X�L���i�R�j�A�u�t�@���g���X�^�u�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player1PoisonMist(Player* pPlayer);//�v���C���[�P���Ȋw�҂̃X�L���i�P�j�A�u�|�C�Y���~�X�g�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player2PoisonMist(Player* pPlayer);//�v���C���[�Q���Ȋw�҂̃X�L���i�P�j�A�u�|�C�Y���~�X�g�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player1SteamBurst(Player* pPlayer);//�v���C���[�P���Ȋw�҂̃X�L���i�Q�j�A�u�X�`�[���o�[�X�g�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player2SteamBurst(Player* pPlayer);//�v���C���[�Q���Ȋw�҂̃X�L���i�Q�j�A�u�X�`�[���o�[�X�g�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player1SquidLaser(Player* pPlayer);//�v���C���[�P���Ȋw�҂̃X�L���i�R�j�A�u�C�J�T�}���[�U�[�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player2SquidLaser(Player* pPlayer);//�v���C���[�P���Ȋw�҂̃X�L���i�R�j�A�u�C�J�T�}���[�U�[�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player1FlameWave(Player* pPlayer);//�v���C���[�P�����@�g���̃X�L���i�P�j�A�u�t���C���E�F�[�u�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player2FlameWave(Player* pPlayer);//�v���C���[�Q�����@�g���̃X�L���i�P�j�A�u�t���C���E�F�[�u�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player1IceLance(Player* pPlayer);//�v���C���[1�����@�g���̃X�L���i�R�j�A�u�A�C�X�����X�v�𔭓�����Ƃ��ɌĂяo���֐��B

bool Player2IceLance(Player* pPlayer);//�v���C���[�Q�����@�g���̃X�L���i�R�j�A�u�A�C�X�����X�v�𔭓�����Ƃ��ɌĂяo���֐��B

void Player1WaveCanon(Player* pPlayer);//�v���C���[�P���A�C�e���u�g���C�v����ɓ��ꂽ�Ƃ��ɌĂяo���֐��B

void Player2WaveCanon(Player* pPlayer);//�v���C���[�Q���A�C�e���u�g���C�v����ɓ��ꂽ�Ƃ��ɌĂяo���֐��B



//�O���[�o���ϐ��錾
Player g_aPlayer[MAX_PLAYER];



//=====================================
//���C���֐�
//=====================================

int main(void)
{
	PlaySound(TEXT("bgm2.wav"), NULL, SND_ASYNC | SND_FILENAME);

	printf("�O�@�@�O�@�@�O�@�@�O�@�@�@�O�@�O�O�O\n");
	printf("�O�O�@�O�@�O�@�O�@�O�O�@�O�O�@�O\n");
	printf("�O�@�O�O�@�O�O�O�@�O�@�O�@�O�@�O�O�O\n");
	printf("�O�@�@�O�@�O�@�O�@�O�@�O�@�O�@�O\n");
	printf("�O�@�@�O�@�O�@�O�@�O�@�O�@�O�@�O�O�O\n\n");

	printf("�O�@�O�@�O�@�O�@�O�@�@�O�@�O�O�O�@�O�O�O�@�O�O�O\n");
	printf("�O�@�O�@�O�@�O�@�O�O�@�O�@�@�O�@�@�O�@�@�@�O�@�@�O\n");
	printf("�O�O�O�@�O�@�O�@�O�@�O�O�@�@�O�@�@�O�O�O�@�O�O�O\n");
	printf("�O�@�O�@�O�@�O�@�O�@�@�O�@�@�O�@�@�O�@�@�@�O�@�O\n");
	printf("�O�@�O�@�O�O�O�@�O�@�@�O�@�@�O�@�@�O�O�O�@�O�@�@�O\n\n\n\n\n\n\n\n\n");


	printf("             �`���͂������O�Ő키RPG�`\n\n");
	printf("                       PressStart�I\n");
	rewind(stdin);
	getchar();
	system("cls");
	float fLength[MAX_PLAYER];//������̒����𐔒l�Ƃ��đ�����邽�߂̕ϐ�

	int nSelectJob[MAX_PLAYER];//��E���v���C���[�̐l�������肷��ϐ���錾����B

	//int nCount;

	int nChoose;

	while (1)
	{


		InputData(&g_aPlayer[0], &fLength[0]);//�\���̂�float�^�̈�������g_aPlayer�̃A�h���X��fLength�̃A�h���X��������B


		SetStatus(&g_aPlayer[0], &fLength[0]);//�\���̂�float�̈�����g_aPlayer�̃A�h���X��fLength�̃A�h���X����������B

		SelectJob(&g_aPlayer[0], &nSelectJob[0]);//��E�����߂āA�ŏI�I�ȃX�e�[�^�X�ƃX�L���̎�ނ����߂�B
		PlaySound(NULL, 0, 0);
		SetBattle(&g_aPlayer[0]);//�o�g���̏������s���֐��B

		PlaySound(NULL, 0, 0);
		printf("�������l�[���n���^�[�����܂����H\n�P�F���������A����ȊO�F�Q�[�����I������\n");

		scanf("%d", &nChoose);
		if (nChoose == 1)
		{
			printf("�������܂����I\n");
		}
		else
		{
			printf("�킩��܂����I����ꂳ�܂ł����I�܂��̃v���C�����҂����Ă���܂�!\n");
			break;
		}
	}
	printf("�����L�[�������Ă�������\n");

	rewind(stdin);//�W�����͂�����͂����������ɖ߂��B
	getchar();
	return 0;






	//int nTurnAttack;//�U���^�[���̕ϐ�

	//bool bEndBattle;//�擪���I���������ǂ����̃t���O��ݒ肷��B�i���̌^�́Atrue�i�^)�A����false�i�U)�̒l�ƂȂ�B)
}

void InputData(Player* pPlayer, float* pLength)//�\���̂�float�̃|�C���^��錾����B
{
	//float f[MAX_PLAYER];



	int nCount;
	while (1)
	{


		for (nCount = 0; nCount < MAX_PLAYER; nCount++)
		{
			printf("%d�l�ڂ̃v���C���[�̖��O����͂��Ă��������I\n�F", nCount + 1);

			scanf("%s", &pPlayer[nCount].aName[0]);

			printf("\n\n");



			//pPlayer++;
		}



		for (nCount = 0; nCount < MAX_PLAYER; nCount++)
		{
			pLength[nCount] = (float)strlen(&pPlayer[nCount].aName[0]);//������̒����𐔒l�Ƃ��Đݒ肷��B
			//nLength[1] = (int)strlen(&pPlayer[PlayerNumber2].aName[0]);//������̒����𐔒l�Ƃ��Đݒ肷��B

			//pPlayer++;//�|�C���^���C���N�������g����B
			//pLength++;
		}
		//pLength[0];

		//for (nCount = 0; nCount < MAX_PLAYER; nCount++,pLength++)
		for (nCount = 0; nCount < MAX_PLAYER; nCount++)
		{
			if (pLength[nCount] > MAX_NAME)//fLength�̒l���ő啶�����ȏ�Ȃ�G���[�\��������B�|�C���^��̒l��\���B
			{
				printf("���͂��ꂽ���O���������܂��I\n\n");
			}
			//pLength++;
		}

		if (pLength[0] <= MAX_NAME && pLength[1] <= MAX_NAME)//��l�̃v���C���[�̓��͂������O���ő啶�����ȉ��������ꍇ�A���[�v���I������B
		{
			break;
		}

		//pPlayer = pPlayer - 2;//�v���C���[�̃|�C���^���Q�߂��A����ꏊ�����Ƃɂ��ǂ��B
	}




	//pPlayer[0];//�C���N�������g���I���������A�擪�A�h���X��������B

	//return fLength[MAX_PLAYER];
}
//===================================================
//�v���C���[�̃p�����[�^��ݒ肷��
//===================================================

void SetStatus(Player* pPlayer, float* pLength)//�v���C���[��float�̃|�C���^��錾����B
{
	int CheckCount = 0;

	int nCount;

	float fData[MAX_NAME]; // , nLength[MAX_PLAYER];

	float fData1[MAX_NAME];

	//int LifeParam[MAX_PLAYER];//�̗͂̐��l��\���ϐ��i��l���̔z���錾����j

	//int AtkParam[MAX_PLAYER];//�U���͂�\���ϐ��i��l���̔z���錾����j

	//float fRandum1;//�����_���Œl�����܂�ϐ���錾����B
	//float fRandum2;//     
	//int ChecknRandum;//�����_���ŏ�����ς��邽�߂̕ϐ���錾����B

	float fBonus[6][10];//�����𖞂������Ƃ��ɔ�������{�[�i�X�X�e�[�^�X�̔z����R��ނłP�O���錾����B


	//================================================
	//���݂��̃v���C���[�̃��C�t�{�[�i�X�̒l��ҏW
	//================================================
	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[0][nCount] = 1;//�P�O���̃{�[�i�X�ϐ��̒��g����U�P�ɂ���B
	}

	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[1][nCount] = 1;//�P�O���̃{�[�i�X�ϐ��̒��g����U�P�ɂ���B
	}

	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[2][nCount] = 1;//�P�O���̃{�[�i�X�ϐ��̒��g����U�P�ɂ���B
	}

	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[3][nCount] = 1;//�P�O���̃{�[�i�X�ϐ��̒��g����U�P�ɂ���B
	}

	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[4][nCount] = 1;//�P�O���̃{�[�i�X�ϐ��̒��g����U�P�ɂ���B
	}

	for (nCount = 0; nCount < 10; nCount++)
	{
		fBonus[5][nCount] = 1;//�P�O���̃{�[�i�X�ϐ��̒��g����U�P�ɂ���B
	}
	//for (nCount = 0; nCount < 10; nCount++)
	//{
	//	fBonus[6][nCount] = 1;//�P�O���̃{�[�i�X�ϐ��̒��g����U�P�ɂ���B
	//}

	//for (nCount = 0; nCount < 10; nCount++)
	//{
	//	fBonus[7][nCount] = 1;//�P�O���̃{�[�i�X�ϐ��̒��g����U�P�ɂ���B
	//}



	//�����̎��ݒ肷��
	//srand((unsigned int)time(NULL));
	//fRandum1 = rand() % 25 + 1;//�����l�i�Q�T���j��錾

	//printf("%f\n", fRandum1);


	//fRandum1 = rand() % 25 + 1;//�����l�i�Q�T���j��錾
	//ChecknRandum = rand() % 2 + 1;//�����_���ŏ�����ς������Ƃ��Ɏg�������ϐ�

	//float fRandumResult;//�ŏI�I�����_���֐��̒l���m�肳����ϐ��B


	//==================================================
	//�v�Z�Ɏg���l�̐ݒ�
	//==================================================


	//���͂��ꂽ��������v���C���[�̍ő�l�������ꂼ��ǂݎ��B

	//for (nCount = 0; nCount < MAX_PLAYER; nCount++)
	//{
	//	nLength[nCount] = (float)strlen(&g_aPlayer[nCount].aName[0]);//������̒����𐔒l�Ƃ��Đݒ肷��B
	//	//nLength[1] = (int)strlen(&pPlayer[PlayerNumber2].aName[0]);//������̒����𐔒l�Ƃ��Đݒ肷��B

	//	//pPlayer++;//�|�C���^���C���N�������g����B

	//}

	//printf("nLength[0]�̒l�F%f\n", pLength[0]);
	//printf("��l�ڂ̃v���C���[�̖̂��O�F%s", &g_aPlayer[0].aName[0]);

	//pPlayer;


	//�v���C���[�P���ǂݎ����������nData�̔z��̒l��錾����B
	for (nCount = 0; nCount < MAX_NAME; nCount++)
	{
		fData[nCount] = 80
			;//���������Ȃ��������p�ɁA���͂��ĂȂ������񕪂��f�[�^�����
		if (nCount < pLength[PlayerNumber1])
		{
			fData[nCount] = (float)g_aPlayer[PlayerNumber1].aName[nCount];//�w�肵�Ă��镶�����A�X�L�[�R�[�h�̐��l�Ƃ��ēǂݍ��ށB
		}
	}
	//�v���C���[�Q���ǂݎ����������nData�̔z��̒l��錾����B
	for (nCount = 0; nCount < MAX_NAME; nCount++)
	{
		fData1[nCount] = 80;//���������Ȃ��������p�ɁA���͂��ĂȂ������񕪂��f�[�^�����
		if (nCount < pLength[PlayerNumber2])
		{
			fData1[nCount] = (float)g_aPlayer[PlayerNumber2].aName[nCount];//�w�肵�Ă��镶�����A�X�L�[�R�[�h�̐��l�Ƃ��ēǂݍ��ށB
		}

		//nData1[nCount] = (int)g_aPlayer[nCount].aName[nCount];//�w�肵�Ă��镶�����A�X�L�[�R�[�h�̐��l�Ƃ��ēǂݍ��ށB
	}


	//================================
	//�v���C���[�P�̃��C�t�{�[�i�X
	//================================

	if (fData[0] == 107.0 || fData[0] == 115.0)//�A�X�L�[�R�[�h�̕�����"k,s"�A�ꕶ���ڂ�"k,s"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[0][0] = 1300;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData[1] == 105.0 || fData[1] == 104.0)//�A�X�L�[�R�[�h�̕�����"i,h"�A�Q�����ڂ�"i,h"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[0][1] = 1300;
	}

	if (fData[2] == 114.0 || fData[2] == 105.0)//�A�X�L�[�R�[�h�̕�����"r,i"�A3�����ڂ�"r,i"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[0][2] = 1300;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData[3] == 98.0 || fData[3] == 110.0)//�A�X�L�[�R�[�h�̕�����"b,n"�A4�����ڂ�"b,n"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[0][3] = 1300;
	}

	if (fData[4] == 121.0 || fData[4] == 97.0)//�A�X�L�[�R�[�h�̕�����"y,a"�A5�����ڂ�"y,a"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[0][4] = 1300;
	}

	//======================================
	//�v���C���[�Q�̃��C�t�{�[�i�X
	//======================================
	if (fData1[0] == 107.00 || fData1[0] == 115.0)//�A�X�L�[�R�[�h�̕�����"k,s"�A�ꕶ���ڂ�"k,s"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[3][0] = 1300;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData1[1] == 105.0 || fData1[1] == 104.0)//�A�X�L�[�R�[�h�̕�����"i,h"�A�Q�����ڂ�"i,h"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[3][1] = 1300;
	}

	if (fData1[2] == 114.0 || fData1[2] == 105.0)//�A�X�L�[�R�[�h�̕�����"r,i"�A3�����ڂ�"r,i"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[3][2] = 1300;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData1[3] == 98.0 || fData1[3] == 110.0)//�A�X�L�[�R�[�h�̕�����"b,n"�A4�����ڂ�"b.n"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[3][3] = 1300;
	}

	if (fData1[4] == 121.0 || fData1[4] == 97.0)//�A�X�L�[�R�[�h�̕�����"y,a"�A5�����ڂ�"y,a"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[3][4] = 1300;
	}

	//===================================  1:taiyo 2:fpsgm
	//�v���C���[�P�̃A�^�b�N�{�[�i�X
	//===================================
	if (fData[0] == 116.0 || fData[0] == 102.0)//�A�X�L�[�R�[�h�̕�����"t,f"�A�ꕶ���ڂ�"t,f"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[1][0] = 140;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData[1] == 97.0 || fData[1] == 112.0)//�A�X�L�[�R�[�h�̕�����"a,p"�A�Q�����ڂ�"a,p"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[1][1] = 140;
	}

	if (fData[2] == 105.0 || fData[2] == 115.0)//�A�X�L�[�R�[�h�̕�����"i,s"�A3�����ڂ�"i,s"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[1][2] = 140;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData[3] == 121.0 || fData[3] == 103.0)//�A�X�L�[�R�[�h�̕�����"y,g"�A4�����ڂ�"y,g"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[1][3] = 140;
	}

	if (fData[4] == 111.0 || fData[4] == 109.0)//�A�X�L�[�R�[�h�̕�����"o,m"�A5�����ڂ�"o,m"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[1][4] = 140;
	}

	//=======================================
	//�v���C���[�Q�̃A�^�b�N�{�[�i�X
	//=======================================
	if (fData1[0] == 116.0 || fData1[0] == 102.0)//�A�X�L�[�R�[�h�̕�����"t,f"�A�ꕶ���ڂ�"t,f"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[4][0] = 140;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData1[1] == 97.0 || fData1[1] == 112.0)//�A�X�L�[�R�[�h�̕�����"a,p"�A�Q�����ڂ�"a,p"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[4][1] = 140;
	}

	if (fData1[2] == 105.0 || fData1[2] == 115.0)//�A�X�L�[�R�[�h�̕�����"i,s"�A3�����ڂ�"i,s"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[4][2] = 140;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData1[3] == 121.0 || fData1[3] == 103.0)//�A�X�L�[�R�[�h�̕�����"y,g"�A4�����ڂ�"y,g"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[4][3] = 140;
	}

	if (fData1[4] == 111.0 || fData1[4] == 109.0)//�A�X�L�[�R�[�h�̕�����"o,m"�A5�����ڂ�"o,m"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[4][4] = 140;
	}

	//===============================================  1:haruk 2:aorin
	//�v���C���[�P�̃f�B�t�F���X�{�[�i�X
	//===============================================
	if (fData[0] == 104.0 || fData[0] == 97.0)//�A�X�L�[�R�[�h�̕�����"h,a"�A�ꕶ���ڂ�"h,a"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[2][0] = 100;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData[1] == 97.0 || fData[1] == 111.0)//�A�X�L�[�R�[�h�̕�����"a,o"�A�Q�����ڂ�"a,o"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[2][1] = 100;
	}

	if (fData[2] == 114.0)//�A�X�L�[�R�[�h�̕�����"r"�A3�����ڂ�"r"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[2][2] = 100;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData[3] == 117.0 || fData[3] == 105.0)//�A�X�L�[�R�[�h�̕�����"u,i"�A4�����ڂ�"u,i"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[2][3] = 100;
	}

	if (fData[4] == 117.0 || fData[4] == 110.0)//�A�X�L�[�R�[�h�̕�����"k,n"�A5�����ڂ�"k,n"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[2][4] = 100;
	}


	//===============================================  1:haruk 2:aorin
	//�v���C���[�Q�̃f�B�t�F���X�{�[�i�X
	//===============================================
	if (fData1[0] == 104.0 || fData1[0] == 97.0)//�A�X�L�[�R�[�h�̕�����"h,a"�A�ꕶ���ڂ�"h,a"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[5][0] = 100;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData1[1] == 97.0 || fData1[1] == 111.0)//�A�X�L�[�R�[�h�̕�����"a,o"�A�Q�����ڂ�"a,o"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[5][1] = 100;
	}

	if (fData1[2] == 114.0)//�A�X�L�[�R�[�h�̕�����"r"�A3�����ڂ�"r"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[5][2] = 100;//�{�[�i�X�X�e�[�^�X�̈�ڂ�錾����B
	}

	if (fData1[3] == 117.0 || fData1[3] == 105.0)//�A�X�L�[�R�[�h�̕�����"u,i"�A4�����ڂ�"u,i"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	{
		fBonus[5][3] = 100;
	}

	if (fData1[4] == 117.0 || fData1[4] == 110.0)//�A�X�L�[�R�[�h�̕�����"k,n"�A5�����ڂ�"k,n"�Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B 
	{
		fBonus[5][4] = 100;
	}

	//=========================================
	//�v���C���[1�̃X�e�[�^�X�{�[�i�X
	//=========================================
	//if (fData[5] == 122.0)//�A�X�L�[�R�[�h�̕����́h���h�A�U�����ڂ����Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	//{
	//	fBonus[6][5] = 120;
	//}
	//if (fData1[5] == 122.0)//�A�X�L�[�R�[�h�̕����́h���h�A�U�����ڂ����Ȃ�A�X�e�[�^�X�{�[�i�X��t�^����B
	//{
	//	fBonus[7][5] = 120;
	//}




	////�v���C���[�Q���ǂݎ����������nData�̔z��̒l��錾����B
	//for (nCount = 0; nCount < MAX_NAME; nCount++)
	//{
	//	fData1[nCount] = 80;//���������Ȃ��������p�ɁA���͂��ĂȂ������񕪂��f�[�^�����
	//	if (nCount < nLength[PlayerNumber2])
	//	{
	//		fData1[nCount] = (float)g_aPlayer[PlayerNumber2].aName[nCount];//�w�肵�Ă��镶�����A�X�L�[�R�[�h�̐��l�Ƃ��ēǂݍ��ށB
	//	}

	//	//nData1[nCount] = (int)g_aPlayer[nCount].aName[nCount];//�w�肵�Ă��镶�����A�X�L�[�R�[�h�̐��l�Ƃ��ēǂݍ��ށB
	//}


	//========================================
	//�p�����[�^�̌v�Z��
	//========================================
	char aBonusName1[20] = "tanakaakira";//���O���c���͂������Ƃ��Ƀ{�[�i�X��t�^����B
	char aBonusName2[20] = "uchiyamajunichi";//���O�����R���ꂾ�����Ƃ��Ƀ{�[�i�X��t�^����B
	char aBonusName3[20] = "azumatakashi";//���O�������u�������Ƃ��Ƀ{�[�i�X���ӂ悷��B
	char aBonusName4[20] = "kirby";//���O���J�[�r�B�������Ƃ��Ƀ{�[�i�X��t�^����B
	char aBonusName5[20] = "tadokorokoji";
	char aBonusName6[20] = "akira";
	char aBonusName7[20] = "junichi";
	char aBonusName8[20] = "takashi";

	//pLength;


	//for (nCount = 0; nCount < MAX_PLAYER; nCount++)
	//{

	pPlayer->fLife = (fData[0] + fData[1] + fData[2] + fData[3] + fData[4] + fData[5] + fData[6] + pLength[0] * 10) * 24 + fBonus[0][0] + fBonus[0][1] + fBonus[0][2] + fBonus[0][3] + fBonus[0][4];//�v���C���[�̗̑͂̐��l�̌v�Z��

	pPlayer->fATK = (fData[0] + fData[3] + fData[5] + fData[7] + fData[8]) * 2.1f - pLength[0] * 10 + fBonus[1][0] + fBonus[1][1] + fBonus[1][2] + fBonus[1][3] + fBonus[1][4];//�v���C���[�̍U���͂̐��l�̌v�Z��

	pPlayer->fDEF = (fData[1] + fData[2] + fData[4] + fData[6]) * 1.8f - pLength[0] * 10 + fBonus[2][0] + fBonus[2][1] + fBonus[2][2] + fBonus[2][3] + fBonus[2][4];//�v���C���[�̍U���͂̐��l�̌v�Z��

	if (strcmp(&pPlayer->aName[0], &aBonusName1[0]) == 0)
	{
		printf("�v���C���[�P�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 6000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName2[0]) == 0)
	{
		printf("�v���C���[�P�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 600;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName3[0]) == 0)
	{
		printf("�v���C���[�P�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 600;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName4[0]) == 0)
	{
		printf("�v���C���[�P�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 2000;
		pPlayer->fATK = pPlayer->fATK + 550;
		pPlayer->fDEF = pPlayer->fDEF + 550;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName5[0]) == 0)
	{
		printf("�v���C���[�P�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 810;
		pPlayer->fATK = pPlayer->fATK + 810;
		pPlayer->fDEF = pPlayer->fDEF + 810;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName6[0]) == 0)
	{
		printf("�v���C���[�P�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 6000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName7[0]) == 0)
	{
		printf("�v���C���[�P�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 600;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName8[0]) == 0)
	{
		printf("�v���C���[�P�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 600;
	}
	//pLength++;

	pPlayer++;//�|�C���^���C���N�������g����B
 //for (nCount = 0; nCount < MAX_PLAYER; nCount++)
 //{
	pPlayer->fLife = (fData1[0] + fData1[1] + fData1[2] + fData1[3] + fData1[4] + fData1[5] + fData1[6] + pLength[1] * 10) * 24 + fBonus[3][0] + fBonus[3][1] + fBonus[3][2] + fBonus[3][3] + fBonus[3][4];//�v���C���[�̗̑͂̐��l�̌v�Z��

	pPlayer->fATK = (fData1[0] + fData1[3] + fData1[5] + fData1[7] + fData1[8]) * 2.1f - pLength[1] * 10 + fBonus[4][0] + fBonus[4][1] + fBonus[4][2] + fBonus[4][3] + fBonus[4][4];//�v���C���[�̗̑͂̐��l�̌v�Z��

	pPlayer->fDEF = (fData1[1] + fData1[2] + fData1[4] + fData1[6]) * 1.8f - pLength[1] * 10 + fBonus[5][0] + fBonus[5][1] + fBonus[5][2] + fBonus[5][3] + fBonus[5][4];//�v���C���[�̖h��͂̐��l�̌v�Z��

	if (strcmp(&pPlayer->aName[0], &aBonusName1[0]) == 0)
	{
		printf("�v���C���[�Q�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 6000;
		pPlayer->fATK = pPlayer->fATK + 250;
		pPlayer->fDEF = pPlayer->fDEF + 250;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName2[0]) == 0)
	{
		printf("�v���C���[�Q�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 500;
		pPlayer->fDEF = pPlayer->fDEF + 250;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName3[0]) == 0)
	{
		printf("�v���C���[�Q�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 250;
		pPlayer->fDEF = pPlayer->fDEF + 500;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName4[0]) == 0)
	{
		printf("�v���C���[�Q�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 2000;
		pPlayer->fATK = pPlayer->fATK + 550;
		pPlayer->fDEF = pPlayer->fDEF + 550;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName5[0]) == 0)
	{
		printf("�v���C���[�Q�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 810;
		pPlayer->fATK = pPlayer->fATK + 810;
		pPlayer->fDEF = pPlayer->fDEF + 810;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName6[0]) == 0)
	{
		printf("�v���C���[�Q�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 6000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName7[0]) == 0)
	{
		printf("�v���C���[�Q�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 600;
		pPlayer->fDEF = pPlayer->fDEF + 300;
	}
	if (strcmp(&pPlayer->aName[0], &aBonusName8[0]) == 0)
	{
		printf("�v���C���[�Q�̉B���l�[���̓��͂��m�F�I�X�e�[�^�X�Ƀ{�[�i�X��t�^���܂��I\n");

		pPlayer->fLife = pPlayer->fLife + 3000;
		pPlayer->fATK = pPlayer->fATK + 300;
		pPlayer->fDEF = pPlayer->fDEF + 600;
	}
	pPlayer--;//�|�C���^����v���C���[�P��
//�v���C���[�̖h���
	//�v���C���[�̍U���͂̐��l�̌v�Z��
//}

	//	printf("LifeParam[0]�̒l�F%d\n", LifeParam[0]);

//�̗́A�U���͂���U��`���Ă����B

//aPlayer[0].nLife = 100;
//aPlayer[0].nATK = 15;

//aPlayer[1].nLife = 130;
//aPlayer[1].nATK = 12;



	if (pPlayer->fLife < 0)
	{
		pPlayer->fLife = pPlayer->fLife * -1;//�X�e�[�^�X���O�ȉ��������ꍇ�A�X�e�[�^�X�̒l�Ɂ[�P���|����B
	}
	if (pPlayer->fATK < 0)
	{
		pPlayer->fATK = pPlayer->fATK * -1;//�X�e�[�^�X���O�ȉ��������ꍇ�A�X�e�[�^�X�̒l�Ɂ[�P���|����B
	}
	if (pPlayer->fDEF < 0)
	{
		pPlayer->fDEF = pPlayer->fDEF * -1;//�X�e�[�^�X���O�ȉ��������ꍇ�A�X�e�[�^�X�̒l�Ɂ[�P���|����B
	}

	pPlayer++;//�|�C���^����v���C���[�Q��
	if (pPlayer->fLife < 0)
	{
		pPlayer->fLife = pPlayer->fLife * -1;//�X�e�[�^�X���O�ȉ��������ꍇ�A�X�e�[�^�X�̒l�Ɂ[�P���|����B
	}
	if (pPlayer->fATK < 0)
	{
		pPlayer->fATK = pPlayer->fATK * -1;//�X�e�[�^�X���O�ȉ��������ꍇ�A�X�e�[�^�X�̒l�Ɂ[�P���|����B
	}
	if (pPlayer->fDEF < 0)
	{
		pPlayer->fDEF = pPlayer->fDEF * -1;//�X�e�[�^�X���O�ȉ��������ꍇ�A�X�e�[�^�X�̒l�Ɂ[�P���|����B
	}
	pPlayer--;//�|�C���^����v���C���[�P��


	//pPlayer[0];//�|�C���^��擪�A�h���X��


	//�擪�A�h���X�����i�z��[�O]�ցj

//�p�����[�^���\���̂ɂ��Ă͂߂�B(��邩��A�|�C���^���g���B�j

	/*for (nCount = 0; nCount < MAX_PLAYER; nCount++, pPlayer++)
	{
		pPlayer->nLife = LifeParam[nCount];

		pPlayer->nATK = AtkParam[nCount];
	}*/

	//pPlayer;//�擪�A�h���X�����i�z��[0]�ցj
	//aPlayer[PlayerNumber2].nLife = LifeParam[PlayerNumber2];

	//aPlayer[PlayerNumber2].nATK = AtkParam[PlayerNumber2];

	/*for (nCount = 0; nCount < MAX_PLAYER; nCount++)
	{*/
	printf("              <<�v���C���[�P>>�@�@      vs�@�@     <<�v���C���[�Q>>\n");

	printf("  ���O�F%16s                     %16s\n", &g_aPlayer[0].aName[0], &g_aPlayer[1].aName[0]);

	printf("  \x1b[32m�̗́F%16.f                     %16.f\n", g_aPlayer[0].fLife, g_aPlayer[1].fLife);

	printf("\x1b[31m�U���́F%16.f                     %16.f\n", g_aPlayer[0].fATK, g_aPlayer[1].fATK);

	printf("\x1b[36m�h��́F%16.f                     %16.f\n", g_aPlayer[0].fDEF, g_aPlayer[1].fDEF);

	printf("\n\n");

	//	if (nCount == 1)
	//	{
	//		break;
	//	}

	//	printf("VS\n\n");

	//	//pPlayer++;//�|�C���^���C���N�������g
	//}




}

//============================================
//��E�̑I������
//============================================
void SelectJob(Player* pPlayer, int* pSelectJob)
{
	int nChoose;//�������I��p�ϐ�

	int nCount;

	printf("\x1b[0m+-------------------------------------------------------------+\n");
	printf("| �E�����HP���O�ɂ����珟���I                                |\n");
	printf("| \x1b[33m�E�ʏ�U���������\x1b[36mMP���T����\x1b[0m���Ă�����I\x1b[0m                |\n");
	printf("| �E�X�L�������ɂ�\x1b[36mMP���K�v����I\x1b[0mMP������Ȃ������ꍇ�́@�@�@�@|\n");
	printf("|\x1b[91m�ʏ�U���Ɉڍs����\x1b[0m�̂Œ��ӂ��悤�I          �@�@�@�@�@�@�@�@ |\n");
	printf("| \x1b[33m�E�A�C�e���̏�������\x1b[36m�R��\x1b[0m����I�Ȃ��A�A�C�e����\x1b[93m�K�`��\x1b[0m�� \x1b[0m�@ |\n");
	printf("|�Ȃ��Ă����I\x1b[0m                                               |\n");
	printf("| \x1b[0m�E�u�������Ȃ��v��I������ƁA�s����\x1b[91m�X�L�b�v\x1b[0m��������    |\n");
	printf("|\x1b[36mMP���P�O��\x1b[0m�����I                                         |\n");
	printf("| \x1b[33m�E\x1b[95m����ȃo�t\x1b[0m�͂����Ă���\x1b[96m�ʏ�U��\x1b[0m�ɏ���I\x1b[0m                  |\n");
	printf("| �E\x1b[91m����̃X�L��\x1b[0m�����̏�������\x1b[92m�R�}���h\x1b[0m���g���Ȃ��Ȃ�Ɓ@�@�@|\n");
	printf("|\x1b[91m�����I�ɒʏ�U�����s��\x1b[0m�̂Œ��ӂ��悤�I		              |\n");
	printf("|\x1b[33m �E\x1b[96m�P�O�^�[��\x1b[0m���Ƃ�\x1b[92m�A�C�e���͈��[\x1b[0m������I              \x1b[0m|\n");
	printf("+-------------------------------------------------------------+\n\n");
	printf("�����L�[�������Ă�������\n");

	rewind(stdin);
	getchar();
	system("cls");//��ʂ����Z�b�g����B

	//int nSelectJob[MAX_PLAYER];

	printf("\x1b[0m<<�����y�[�W>>\n\n");





	while (1)
	{

		//system("cls");
		printf("��E�̐����́i�P�`�U�ԁj�A��E�I���ɐi�ނȂ�i�O�ԁj��I�����Ă��������I\n\n");
		printf("�O�`�U�̔ԍ���I��ŉ������i\x1b[32m�P�F���m�A\x1b[34m�Q�F��l�A\x1b[33m�R�F�ޏ��A\x1b[31m�S�F�z���S�A\x1b[36m�T�F�Ȋw�ҁA\x1b[35m�U�C���@�g���A\x1b[0m�O�F���֐i�ށj:");
		scanf("%d", &nChoose);


		if (nChoose == 1)
		{
			printf("+----------+\n");
			printf("|�@\x1b[32m���m\x1b[0m    |\n");
			printf("+----------+\n\n");

			printf("�X�e�[�^�X�{�[�i�X\n");
			printf("+----------+\n");
			printf("| LIFE*1.1 |\n");
			printf("|  ATK*1.1 |\n");
			printf("|  DEF*1.1 |\n");
			printf("|  MP :100 |\n");
			printf("+----------+\n\n");


			printf("��E����\n");
			printf("+--------------------------------------------------------------------------+\n");
			printf("|�U��ɗD�ꂽ�o�����X�^�C�v�ŁA�l�X�ȏ󋵂ɑΉ��ł���I�[�����E���_�[���I�I| \n");
			printf("+--------------------------------------------------------------------------+\n\n");

			printf("�X�L���T�v\n");
			printf("�P�F�C���p�N�g�G�b�W\n");
			printf("�`�G�Ɏ����̍U����*2.8�̃_���[�W��^����I(MP-30)\n\n");

			printf("�Q�F���J�o�[�q�[��\n");
			printf("�`�ő�̗͂�25%%���A�̗͂��񕜂���!(MP-80)\n\n");

			printf("�R�F�J�E���^�[�X�^�C��\n");
			printf("�`3�^�[���̊ԁA�m���Œʏ�U���̃J�E���^�[���s��(50%%)(MP-40)\n\n");
			rewind(stdin);
			printf("�����L�[�������Ă�������\n");
			getchar();

			system("cls");//��ʂ����Z�b�g����B
		}


		else if (nChoose == 2)
		{
			printf("+----------+\n");
			printf("|�@\x1b[34m��l \x1b[0m   |\n");
			printf("+----------+\n\n");

			printf("�X�e�[�^�X�{�[�i�X\n");
			printf("+----------+\n");
			printf("| LIFE*0.8 |\n");
			printf("|  ATK*1.3 |\n");
			printf("|  DEF*1.0 |\n");
			printf("|  MP :100 |\n");
			printf("+----------+\n\n");


			printf("��E����\n");
			printf("+--------------------------------------------------------------------------+\n");
			printf("| ������ؗ�ȋ|���΂��Œǂ��l�߂�X�i�C�p�[�I�͂����p���[�Ƃ����l�����@�@ |\n");
			printf("+--------------------------------------------------------------------------+\n\n");

			printf("�X�L���T�v\n");
			printf("�P�F���C���A���[\n");
			printf("�`�G�Ɏ����̍U����*0.7�̂W�A���U�����s���I(������40��)(MP-25)\n\n");

			printf("�Q�F�p���_�E���A���[\n");
			printf("�U����*2�̃_���[�W��^���A�m���łQ�^�[���U���̓_�E��(ATK��50��)(MP-40)\n\n");

			printf("�R�F�X�^�[�A���[\n");
			printf("�`�G�Ɏ����̍U����*5�̖h�䖳���̍U�����s���I(MP-60)\n\n");
			printf("�����L�[�������Ă�������\n");
			rewind(stdin);
			getchar();
			system("cls");//��ʂ����Z�b�g����B
		}
		else if (nChoose == 3)
		{
			printf("+----------+\n");
			printf("|�@\x1b[33m�ޏ�\x1b[0m �@ |\n");
			printf("+----------+\n\n");

			printf("�X�e�[�^�X�{�[�i�X\n");
			printf("+----------+\n");
			printf("| LIFE*1.2 |\n");
			printf("|  ATK*1.0 |\n");
			printf("|  DEF*1.1 |\n");
			printf("|  MP :120 |\n");
			printf("+----------+\n\n");


			printf("��E����\n");
			printf("+--------------------------------------------------------------------------+\n");
			printf("| ���Ȃ�͂ő��ʂȃo�t�������ޏ����񂾁IMP�����������I�@�@�@�@�@�@�@�@ �@�@|\n");
			printf("+--------------------------------------------------------------------------+\n\n");

			printf("�X�L���T�v\n");
			printf("�P�F���̐w\n");
			printf("�`�T�^�[���̊ԁADEF���㏸����B�iDEF��40��)(MP-35�j\n\n");

			printf("�Q�F�z�[���[���C\n");
			printf("�`�����̍U���́{�h��́�1.2�̃_���[�W��^���A�����MP���P�T���炷�B(�l�o�_���[�W)(MP-45)\n\n");

			printf("�R�F��S�̋F��\n");
			printf("�`�ʏ�U�����R�^�[���̊ԁA�m���ŃN���e�B�J���ɂȂ�悤�ɂȂ�B�iCRT��35��)(MP-55)\n\n");
			printf("�����L�[�������Ă�������\n");

			rewind(stdin);
			getchar();
			system("cls");//��ʂ����Z�b�g����B
		}
		else if (nChoose == 4)
		{
			printf("+----------+\n");
			printf("|�@\x1b[31m�z���S\x1b[0m�@|\n");
			printf("+----------+\n\n");

			printf("�X�e�[�^�X�{�[�i�X\n");
			printf("+----------+\n");
			printf("| LIFE*0.9 |\n");
			printf("|  ATK*1.2 |\n");
			printf("|  DEF*0.9 |\n");
			printf("|  MP :100 |\n");
			printf("+----------+\n\n");


			printf("��E����\n");
			printf("+--------------------------------------------------------------------------------+\n");
			printf("| ����̂g�o���z���̂����ӂȖ�̉��҂��I�ϋv�͉񕜂ŕ₨���I�@�@�@�@�@�@�@�@ �@�@|\n");
			printf("+--------------------------------------------------------------------------------+\n\n");

			printf("�X�L���T�v\n");
			printf("�P�F�u���b�e�B�o�C�g\n");
			printf("�`�����̍U����*2.2�̃_���[�W��^���A�^�����_���[�W�̂V�T���g�o���񕜂���I�i�z���U��)(MP-35�j\n\n");

			printf("�Q�F�N���C�W�[�O���r�e�B\n");
			printf("�`�m���œG�̌��݂g�o���Q�O�����炷�B���͎����̌��݂g�o���P�T�O�O�񕜂���B(gravity20(30��))(MP-65)\n\n");

			printf("�R�F�t�@���g���X�^�u\n");
			printf("�`�����̍U����*3.1�̃_���[�W��^���A�m���łR�^�[���̊ԁA�X�L���s��t�^����B�iSKILL�~35��)(MP-50)\n\n");
			printf("�����L�[�������Ă�������\n");

			rewind(stdin);
			getchar();
			system("cls");//��ʂ����Z�b�g����B
		}
		else if (nChoose == 5)
		{
			printf("+----------+\n");
			printf("|�@\x1b[36m�Ȋw��\x1b[0m�@|\n");
			printf("+----------+\n\n");

			printf("�X�e�[�^�X�{�[�i�X\n");
			printf("+----------+\n");
			printf("| LIFE*1.3 |\n");
			printf("|  ATK*0.8 |\n");
			printf("|  DEF*0.7 |\n");
			printf("|  MP :110 |\n");
			printf("+----------+\n\n");


			printf("��E����\n");
			printf("+--------------------------------------------------------------------------------+\n");
			printf("| �g���b�L�[�Ȑ�@���D�ރ}�b�h�T�C�G���e�B�X�g���I�̗͂̔{�����ƂĂ��������I�@�@ |\n");
			printf("+--------------------------------------------------------------------------------+\n\n");

			printf("�X�L���T�v\n");
			printf("�P�F�|�C�Y���~�X�g\n");
			printf("�`�����̍U����*2.7�̃_���[�W��^���A�m���łR�^�[���̊ԁA�ŏ�Ԃɂ���B�i��35��)(MP-40�j\n\n");

			printf("�Q�F�X�`�[���o�[�X�g\n");
			printf("�`�m���Łi�����̍U���́{����̖h��́j��4.5�̑�_���[�W��^����B�܂��͎����ɉΏ���Ԃ�t�^�B(MP-65)\n\n");

			printf("�R�F�C�J�T�}���[�U�[\n");
			printf("�`�G�̍U���́�2.6�̃_���[�W��^���A�m���œG�̖h��͂��R�^�[���̊ԁA�S�O��������B�i�G�U���͈ˑ��U��)(MP-45)\n\n");
			printf("�����L�[�������Ă�������\n");

			rewind(stdin);
			getchar();
			system("cls");//��ʂ����Z�b�g����B
		}
		else if (nChoose == 6)
		{
			printf("+----------+\n");
			printf("|�@\x1b[35m���@�g��\x1b[0m|\n");
			printf("+----------+\n\n");

			printf("�X�e�[�^�X�{�[�i�X\n");
			printf("+----------+\n");
			printf("| LIFE*0.8 |\n");
			printf("|  ATK*1.0 |\n");
			printf("|  DEF*1.3 |\n");
			printf("|  MP:100  |\n");
			printf("+----------+\n\n");


			printf("��E����\n");
			printf("+--------------------------------------------------------------------------------+\n");
			printf("| ���@�Ő키���@�g���I�X���b�v�}�W�b�N���g�����Ȃ���Έ�l�O���I�@�@�@�@�@�@�@   |\n");
			printf("+--------------------------------------------------------------------------------+\n\n");

			printf("�X�L���T�v\n");
			printf("�P�F�t���C���E�F�[�un");
			printf("�`�����̍U����*1.1�̂S�A���U�������A�m���Ŏ����̍U���͂��Q�^�[���̊ԂQ�T���㏸������B�i�������T�O��)(�A���U��)�i�U���́��Q�T���j(MP-35)\n\n");

			printf("�Q�F�X���b�v�}�W�b�N\n");
			printf("�`�����̍U���͂Ɩh��͂̐��l�����ւ���B(MP-25)\n\n");

			printf("�R�F�A�C�X�����X\n");
			printf("�`�����̍U����*4.5�̃_���[�W��^���A�m���łT�^�[������̃A�C�e���R�}���h�𕕈󂷂�B�i�A�C�e���~50��)(MP-70)\n\n");
			printf("�����L�[�������Ă�������\n");

			rewind(stdin);
			getchar();
			system("cls");//��ʂ����Z�b�g����B
		}
		else if (nChoose == 0)
		{
			break;
		}

		else
		{
		
			rewind(stdin);
			getchar();
			system("cls");//��ʂ����Z�b�g����B
		}
		//system("cls");//��ʂ����Z�b�g����
	}

	for (nCount = 0; nCount < MAX_PLAYER; nCount++)
	{



		while (1)
		{
			printf("�v���C���[%d�̖�E��I��ł�������\n�i\x1b[32m�P�F���m�A\x1b[34m�Q�F��l�A\x1b[33m�R�F�ޏ��A\x1b[31m�S�F�z���S�A\x1b[36m�T�F�Ȋw�ҁA\x1b[35m�U�C���@�g���A\x1b[0m\n", nCount + 1);
			scanf("%d", &pSelectJob[nCount]);

			if (pSelectJob[nCount] == 1)
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 1.1;//�̗͂̒l��1.1�{����B
				pPlayer->fATK = g_aPlayer[nCount].fATK * 1.1;//�U���͂̒l��1.1�{����B
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 1.1;//�h��͂̒l��1.1�{����B
				pPlayer->fMP = 100;//�}�W�b�N�|�C���g���P�O�O�ɐݒ�B
				pPlayer->nCheckJob = 1;//��E�����m�̎��ɁA�ԍ��P��������B
				break;
			}

			else if (pSelectJob[nCount] == 2)
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 0.8;//�̗͂̒l��0.9�{����B
				pPlayer->fATK = g_aPlayer[nCount].fATK * 1.3;//�U���͂̒l��1.2�{����B
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 1.0;//�h��͂̒l��1.1�{����B
				pPlayer->fMP = 100;//�}�W�b�N�|�C���g���P�O�O�ɐݒ�B
				pPlayer->nCheckJob = 2;//��E����l�̎��ɁA�ԍ��Q��������B
				break;
			}
			else if (pSelectJob[nCount] == 3)
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 1.2;//�̗͂̒l��1.2�{����B
				pPlayer->fATK = g_aPlayer[nCount].fATK * 1.0;//�U���͂̒l��1.2�{����B
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 1.1;//�h��͂̒l��1.1�{����B
				pPlayer->fMP = 120;//�}�W�b�N�|�C���g���P�O�O�ɐݒ�B
				pPlayer->nCheckJob = 3;//��E���ޏ��̎��ɁA�ԍ��R��������B
				break;
			}
			else if (pSelectJob[nCount] == 4)//�I�񂾐E�Ƃ��u�z���S�v�������ꍇ
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 0.9;//�̗͂̒l��0.9�{����B
				pPlayer->fATK = g_aPlayer[nCount].fATK * 1.2;//�U���͂̒l��1.2�{����B
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 0.9;//�h��͂̒l��0.9�{����B
				pPlayer->fMP = 100;//�}�W�b�N�|�C���g��100�ɐݒ�B
				pPlayer->nCheckJob = 4;//��E���z���S�̎��ɁA�ԍ��S��������B
				break;
			}
			else if (pSelectJob[nCount] == 5)//�I�񂾐E�Ƃ��u�Ȋw�ҁv�������ꍇ
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 1.3;//�̗͂̒l��1.4�{����B
				pPlayer->fATK = g_aPlayer[nCount].fATK * 0.8;//�U���͂̒l��0.8�{����B
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 0.7;//�h��͂̒l��0.8�{����B
				pPlayer->fMP = 110;//�}�W�b�N�|�C���g��110�ɐݒ�B
				pPlayer->nCheckJob = 5;//��E���z���S�̎��ɁA�ԍ��S��������B
				break;
			}
			else if (pSelectJob[nCount] == 6)//�I�񂾐E�Ƃ��u���@�g���v�������ꍇ
			{
				pPlayer->fLife = g_aPlayer[nCount].fLife * 0.9;//�̗͂̒l��0.9�{����B
				pPlayer->fATK = g_aPlayer[nCount].fATK * 1.0;//�U���͂̒l��0.9�{����B
				pPlayer->fDEF = g_aPlayer[nCount].fDEF * 1.3;//�h��͂̒l��1.3�{����B
				pPlayer->fMP = 100;//�}�W�b�N�|�C���g��110�ɐݒ�B
				pPlayer->nCheckJob = 6;//��E�����@�g���̎��ɁA�ԍ��S��������B
				break;
			}
			else
			{
				while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
				printf("���͂��ꂽ�l���Ⴂ�܂��B��������͂��Ă�������\n\n");
				rewind(stdin);
				getchar();
				system("cls");
			}

		}

		pPlayer++;//�|�C���^���C���N�������g���Ċ�_�����炷�B


	}
	printf("�����͊����I�o�g���J�n�[�[�[�[�[�[�[�[�[�[�I\n\n");


	rewind(stdin);
	getchar();
	system("cls");

	

	

}

//=================================================
//�o�g���̏���
//=================================================



void SetBattle(Player* pPlayer)
{
	PlaySound(TEXT("bgm3.wav"), NULL, SND_ASYNC | SND_FILENAME);


	float fMaxLife = g_aPlayer[PlayerNumber1].fLife;//

	//pPlayer;//�|�C���^�ɐ擪�A�h���X��������B�i�z��[0]�ցj

	//int nCount;

	int nTurnAttack;//�U���^�[���̕ϐ�

	//float fDamageResult;//�_���[�W�̌��ʂ�������ϐ�

	//float fRandum1;

	//int CheckRandum;

	bool bEndBattle;//�擪���I���������ǂ����̃t���O��ݒ肷��B�i���̌^�́Atrue�i�^)�A����false�i�U)�̒l�ƂȂ�B)

	//bool bSwordSkill[MAX_PLAYER];

	int nChoose;//�R�}���h��I�ԂƂ��Ɉ����ϐ��B

	int nSkillChoose;//�X�L����I�ԂƂ��Ɉ����ϐ��B

	int nPlayer1ItemRouletteUseCount = 3;//�v���C���[�P���A�C�e�����[���b�g���g����c��̉񐔂�錾����B
	int nPlayer2ItemRouletteUseCount = 3;//�v���C���[�Q���A�C�e�����[���b�g���g����c��̉񐔂�錾����B

	int nShootingStar;//�A�C�e���u�V���[�e�B���O�X�^�[�v�̃����_���_���[�W��錾����B

	/*printf("<<�v���C���[�P>>�@�@vs�@�@<<�v���C���[�Q>>\n");

	printf(" ���O �F%s                      %s\n", &g_aPlayer[0].aName[0], &g_aPlayer[1].aName[0]);

	printf("  �̗́F%.f                    %.f\n", g_aPlayer[0].fLife, g_aPlayer[1].fLife);

	printf("�U���́F%.f                     %.f\n", g_aPlayer[0].fATK, g_aPlayer[1].fATK);

	printf("�h��́F%.f                     %.f\n", g_aPlayer[0].fDEF, g_aPlayer[1].fDEF);*/

	//===============================================================
	//�X�L���̓�����ʃt���O
	//================================================================
	bool Player1CounterFlag;//�v���C���[�P�̃J�E���^�[�����̃t���O��錾����B
	Player1CounterFlag = false;//�v���C���[�P�̃J�E���^�[�t���O��false�ɂ��Ă����B

	bool Player2CounterFlag;//�v���C���[�Q�̃J�E���^�[�����̃t���O��錾����B
	Player2CounterFlag = false;//�v���C���[�Q�̃J�E���^�[�t���O��false�ɂ��Ă����B

	int nPlayer1CounterCount;//�v���C���[�P�̃J�E���^�[�t���O��true�ɂȂ��Ă���Ƃ��ɁAtrue�ɂȂ��Ă���^�[�����J�E���g����B
	nPlayer1CounterCount = 0;//�J�E���^�[�J�E���g�Ɂu�O�v��������B

	int nPlayer2CounterCount;//�v���C���[�Q�̃J�E���^�[�t���O��true�ɂȂ��Ă���Ƃ��ɁAtrue�ɂȂ��Ă���^�[�����J�E���g����B
	nPlayer2CounterCount = 0;//�J�E���^�[�J�E���g�Ɂu�O�v��������B

	bool Player2PowerDownArrowDebuff;//�v���C���[�Q���v���C���[�P�̃p���_�E���A���[�̃f�o�t�i�U���͂R�O���ቺ�j���󂯂Ă��邱�Ƃ̃t���O��錾����B
	Player2PowerDownArrowDebuff = false;//�t���O��false�ɂ��Ă����B

	int nPlayer2PowerDownArrowDebuffCount;//�v���C���[�Q���p���_�E���A���[�̃f�o�t���󂯂Ă���^�[�������J�E���g����ϐ��B
	nPlayer2PowerDownArrowDebuffCount = 0;


	bool Player1PowerDownArrowDebuff;//�v���C���[�P���p���_�E���A���[�̃f�o�t�i�U���͂R�O���ቺ�j���󂯂Ă��邱�Ƃ̃t���O��錾����B
	Player1PowerDownArrowDebuff = false;//�t���O��false�ɂ��Ă����B

	int nPlayer1PowerDownArrowDebuffCount;//�v���C���[�P���p���_�E���A���[�̃f�o�t���󂯂Ă���^�[�������J�E���g����ϐ��B
	nPlayer1PowerDownArrowDebuffCount = 0;//�^�[�����̃J�E���g���O�ɂ��Ă����B


	bool bPlayer1DefenceLineBuff;//�v���C���[�P���X�L���u���̐w�v�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾����B
	bPlayer1DefenceLineBuff = false;//�t���O��false�ɂ��Ă����B

	int nPlayer1DefenceLineBuffCount;//�v���C���[�P���X�L���u���̐w�v�̃o�t���󂯂Ă���^�[�������J�E���g����ϐ��B
	nPlayer1DefenceLineBuffCount = 0;//�J�E���g���O�ɂ��Ă����B

	bool bPlayer2DefenceLineBuff;//�v���C���[�Q���X�L���u���̐w�v�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾����B
	bPlayer2DefenceLineBuff = false;//�t���O��false�ɂ��Ă����B

	int nPlayer2DefenceLineBuffCount;//�v���C���[�Q���X�L���u���̐w�v�̃o�t���󂯂Ă���^�[�������J�E���g����ϐ��B
	nPlayer2DefenceLineBuffCount = 0;//�J�E���g���O�ɂ��Ă����B


	bool bPlayer1CriticalLineBuff;//�v���C���[�P���X�L���u��S�̐w�v�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾����B
	bPlayer1CriticalLineBuff = false;//�t���O��false�ɂ��Ă����B

	int nPlayer1CriticalLineBuffCount;//�v���C���[�P���X�L���u��S�̐w�v�̃o�t���󂯂Ă���^�[�������J�E���g����t���O��錾����B
	nPlayer1CriticalLineBuffCount = 0;//�J�E���g���O�ɂ��Ă����B

	bool bPlayer2CriticalLineBuff;//�v���C���[�Q���X�L���u��S�̐w�v�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾����B
	bPlayer2CriticalLineBuff = false;//�t���O��false�ɂ��Ă����B

	int nPlayer2CriticalLineBuffCount;//�v���C���[�Q���X�L���u��S�̐w�v�̃o�t���󂯂Ă���^�[�������J�E���g����t���O��錾����B
	nPlayer2CriticalLineBuffCount = 0;//�J�E���g���O�ɂ��Ă����B


	bool bPlayer2PhantomStabDebuff;//�v���C���[�Q�Ɂu�t�@���g���X�^�u�v�̃f�o�t��^����t���O��錾����B
	bPlayer2PhantomStabDebuff = false;//�t���O��false�ɂ��Ă����B

	int nPlayer2PhantomStabDebuffCount;//�v���C���[�Q���X�L���u�t�@���g���X�^�u�v�̃f�o�t���󂯂Ă���^�[�������J�E���g����ϐ���錾����B
	nPlayer2PhantomStabDebuffCount = 0;//�J�E���g���O�ɂ��Ă����B

	bool bPlayer1PhantomStabDebuff;//�v���C���[�P�Ɂu�t�@���g���X�^�u�v�̃f�o�t��^����t���O��錾����B
	bPlayer1PhantomStabDebuff = false;//�t���O��false�ɂ��Ă����B

	int nPlayer1PhantomStabDebuffCount;//�v���C���[�P���X�L���u�t�@���g���X�^�u�v�̃f�o�t���󂯂Ă���^�[�������J�E���g����ϐ���錾����B
	nPlayer1PhantomStabDebuffCount = 0;//�J�E���g���O�ɂ��Ă����B


	bool bPlayer2PoisonMistState;//�v���C���[�Q�ɃX�L���u�|�C�Y���~�X�g�v�̓ŏ�Ԃ�^����t���O��錾����B
	bPlayer2PoisonMistState = false;//�t���O��false�ɂ��Ă����B

	int nPlayer2PoisonMistStateCount;//�v���C���[�Q���X�L���u�|�C�Y���~�X�g�v�̓ŏ�Ԃ��󂯂Ă���^�[�������J�E���g����ϐ���錾����B
	nPlayer2PoisonMistStateCount = 0;//�J�E���g���O�ɂ��Ă����B

	bool bPlayer1PoisonMistState;//�v���C���[�P�ɃX�L���u�|�C�Y���~�X�g�v�̓ŏ�Ԃ�^����t���O��錾����B
	bPlayer1PoisonMistState = false;//�t���O��false�ɂ��Ă����B

	int nPlayer1PoisonMistStateCount;//�v���C���[�P���X�L���u�|�C�Y���~�X�g�v�̓ŏ�Ԃ��󂯂Ă���^�[�������J�E���g����ϐ���錾����B
	nPlayer1PoisonMistStateCount = 0;//�J�E���g���O�ɂ��Ă����B


	bool bPlayer1SteamBurstBurn;//�v���C���[1���X�L���u�X�`�[���o�[�X�g�v�̃f�����b�g���ʁu�Ώ��v���󂯂Ă��邱�Ƃ�\���t���O��錾����B
	bPlayer1SteamBurstBurn = false;//�t���O��false�ɂ��Ă����B

	int nPlayr1SteamBurstBurnCount;//�v���C���[�P���X�L���u�X�`�[���o�[�X�g�v�̃f�����b�g���ʁu�Ώ��v���󂯂Ă���^�[�������J�E���g����ϐ���錾����B
	nPlayr1SteamBurstBurnCount = 0;//�J�E���g���O�ɂ��Ă����B

	bool bPlayer2SteamBurstBurn;//�v���C���[�Q���X�L���u�X�`�[���o�[�X�g�v�̃f�����b�g���ʁu�Ώ��v���󂯂Ă��邱�Ƃ�\���t���O��錾����B
	bPlayer2SteamBurstBurn = false;//�t���O��false�ɂ��Ă����B

	int nPlayr2SteamBurstBurnCount;//�v���C���[�Q���X�L���u�X�`�[���o�[�X�g�v�̃f�����b�g���ʁu�Ώ��v���󂯂Ă���^�[�������J�E���g����ϐ���錾����B
	nPlayr2SteamBurstBurnCount = 0;//�J�E���g���O�ɂ��Ă����B


	bool bPlayer2SquitLaserDebuff;//�v���C���[�Q���X�L���u�C�J�T�}���[�U�[�v�̃f�o�t���󂯂Ă��邩�ǂ�����\���t���O��錾����B
	bPlayer2SquitLaserDebuff = false;//�t���O��false�ɂ��Ă����B

	int nPlayer2SquitLaserDebuffCount;//�v���C���[�Q���X�L���u�C�J�T�}���[�U�[�v�̃f�o�t���󂯂Ă���^�[�������J�E���g����t���O��錾����B
	nPlayer2SquitLaserDebuffCount = 0;//�J�E���g���O�ɂ��Ă����B

	bool bPlayer1SquitLaserDebuff;//�v���C���[�P���X�L���u�C�J�T�}���[�U�[�v�̃f�o�t���󂯂Ă��邩�ǂ�����\���t���O��錾����B
	bPlayer1SquitLaserDebuff = false;//�t���O��false�ɂ��Ă����B

	int nPlayer1SquitLaserDebuffCount;//�v���C���[�P���X�L���u�C�J�T�}���[�U�[�v�̃f�o�t���󂯂Ă���^�[�������J�E���g����t���O��錾����B
	nPlayer1SquitLaserDebuffCount = 0;//�J�E���g���O�ɂ��Ă����B


	bool bPlayer1FlameWaveBuff = false;//�v���C���[�P���X�L���u�t���C���E�F�[�u�v�̃o�t���󂯂Ă��邩�ǂ�����\���t���O��錾����B
	int nPlayer1FlameWaveBuffCount = 0;//�Ղꂢ��[�P���X�L���u�t���C���E�F�[�u�v�̃o�t���󂯂Ă���^�[�������J�E���g����ϐ���錾����B

	bool bPlayer2FlameWaveBuff = false;//�v���C���[2���X�L���u�t���C���E�F�[�u�v�̃o�t���󂯂Ă��邩�ǂ�����\���t���O��錾����B
	int nPlayer2FlameWaveBuffCount = 0;//�Ղꂢ��[2���X�L���u�t���C���E�F�[�u�v�̃o�t���󂯂Ă���^�[�������J�E���g����ϐ���錾����B


	bool bPlayer2IceLanceDebuff = false;//�v���C���[�Q���X�L���u�A�C�X�����X�v�̃f�o�t���󂯂Ă��邩�ǂ����̃t���O��錾���A�t���O��false�ɂ���B
	int nPlayer2IceLanceDebuffCount = 0;//�v���C���[�Q���X�L���u�A�C�X�����X�v�̃f�o�t���󂯂Ă���^�[�������J�E���g����ϐ���錾���A�J�E���g���O�ɂ���B

	bool bPlayer1IceLanceDebuff = false;//�v���C���[�P���X�L���u�A�C�X�����X�v�̃f�o�t���󂯂Ă��邩�ǂ����̃t���O��錾���A�t���O��false�ɂ���B
	int nPlayer1IceLanceDebuffCount = 0;//�v���C���[�P���X�L���u�A�C�X�����X�v�̃f�o�t���󂯂Ă���^�[�������J�E���g����ϐ���錾���A�J�E���g���O�ɂ���B

	//============================================================================================================================================================================
	int nTurn=0;
	int nHalfTurn = 0;


	//�ŏ��̍U���^�[����ݒ肷��B
	nTurnAttack = 0;

	//�퓬�I���t���O��false�ɂ���B
	bEndBattle = false;


	float fPlayer1MaxHp;//�v���C���[�P�̍ő�̗͂�������ϐ��B
	fPlayer1MaxHp = g_aPlayer[0].fLife;

	float fPlayer2MaxHp;//�v���C���[�Q�̍ő�̗͂�������ϐ��B
	fPlayer2MaxHp = g_aPlayer[1].fLife;

	float fPlayer1MaxMp;//�v���C���[�P�̍ő�MP��������ϐ��B
	fPlayer1MaxMp = g_aPlayer[0].fMP;

	float fPlayer2MaxMp;//�v���C���[�Q�̍ő�MP��������ϐ��B
	fPlayer2MaxMp = g_aPlayer[1].fMP;

	float fPlayer1MaxATK;//�v���C���[�P�̍ő�U���͂�������ϐ��B
	fPlayer1MaxATK = g_aPlayer[PlayerNumber1].fATK;

	float fPlayer2MaxATK;//�v���C���[�Q�̍ő�U���͂�������ϐ��B
	fPlayer2MaxATK = g_aPlayer[PlayerNumber2].fATK;

	float fPlayer1MaxDEF;//�v���C���[�P�̍ő�h��͂�������ϐ��B
	fPlayer1MaxDEF = g_aPlayer[PlayerNumber1].fDEF;

	float fPlayer2MaxDEF;//�v���C���[�P�̍ő�h��͂�������ϐ��B
	fPlayer2MaxDEF = g_aPlayer[PlayerNumber2].fDEF;

	int nPlayer1ItemRouletteRandum;//�v���C���[�P�̃A�C�e�����[���b�g�̃����_���֐���錾����B
	int nPlayer2ItemRouletteRandum;//�v���C���[�Q�̃A�C�e�����[���b�g�̃����_���֐���錾����B


	//================================================
	//�A�C�e�����ʃt���O
	//================================================
	bool bPlayer1CurryRiceBuffLV1 = false;//�J���[���C�X�̃o�t���󂯂Ă��錾��\���t���O��錾���Afalse�ɂ���B

	int nPlayer1CurryRiceBuffLV1Count = 0;//�J���[���C�X�̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B

	bool bPlayer2CurryRiceBuffLV1 = false;//�J���[���C�X�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾���Afalse�ɂ���B

	int nPlayer2CurryRiceBuffLV1Count = 0;//�J���[���C�X�̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B


	bool bPlayer1CurryRiceBuffLV2 = false;//�v���C���[�P���V�[�t�[�h�J���[�̃o�t���󂯂Ă��錾��\���t���O��錾���Afalse�ɂ���B

	int nPlayer1CurryRiceBuffLV2Count = 0;//�v���C���[�P���V�[�t�[�h�J���[�̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B

	bool bPlayer2CurryRiceBuffLV2 = false;//�v���C���[�Q���V�[�t�[�h�J���[�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾���Afalse�ɂ���B

	int nPlayer2CurryRiceBuffLV2Count = 0;//�v���C���[�Q���V�[�t�[�h�J���[�̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B


	bool bPlayer1CurryRiceBuffLV3 = false;//�v���C���[�P���|�[�N�J���[�̃o�t���󂯂Ă��錾��\���t���O��錾���Afalse�ɂ���B

	int nPlayer1CurryRiceBuffLV3Count = 0;//�v���C���[�P���|�[�N�J���[�̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B

	bool bPlayer2CurryRiceBuffLV3 = false;//�v���C���[�Q���|�[�N�J���[�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾���Afalse�ɂ���B

	int nPlayer2CurryRiceBuffLV3Count = 0;//�v���C���[�Q���|�[�N�J���[�̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B



	bool bPlayer1RiceBallBuffLV1 = false;//�v���C���[�P�������ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾���A�t���O��false�ɂ���B

	int nPlayer1RiceBallBuffLV1Count = 0;//�v���C���[�P�������ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B

	bool bPlayer2RiceBallBuffLV1 = false;//�v���C���[�Q�������ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾���A�t���O��false�ɂ���B

	int nPlayer2RiceBallBuffLV1Count = 0;//�v���C���[�Q�������ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B


	bool bPlayer1RiceBallBuffLV2 = false;//�v���C���[�P�������ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾���A�t���O��false�ɂ���B

	int nPlayer1RiceBallBuffLV2Count = 0;//�v���C���[�P�������ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B

	bool bPlayer2RiceBallBuffLV2 = false;//�v���C���[�Q�������ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾���A�t���O��false�ɂ���B

	int nPlayer2RiceBallBuffLV2Count = 0;//�v���C���[�Q�������ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B


	bool bPlayer1RiceBallBuffLV3 = false;//�v���C���[�P�������炨�ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾���A�t���O��false�ɂ���B

	int nPlayer1RiceBallBuffLV3Count = 0;//�v���C���[�P�������炨�ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B

	bool bPlayer2RiceBallBuffLV3 = false;//�v���C���[�Q�������炨�ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��錾���A�t���O��false�ɂ���B

	int nPlayer2RiceBallBuffLV3Count = 0;//�v���C���[�Q�������炨�ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B


	bool bPlayer2WeakBallDeBuff = false;//�v���C���[�Q������ʂ̃f�o�t���󂯂Ă��邱�Ƃ�\���t���O��錾���A�t���O��false�ɂ���B
	int nPlayer2WeakBallDeBuffCount = 0;//�v���C���[�Q������ʂ̃f�o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B

	bool bPlayer1WeakBallDeBuff = false;//�v���C���[�P������ʂ̃f�o�t���󂯂Ă��邱�Ƃ�\���t���O��錾���A�t���O��false�ɂ���B
	int nPlayer1WeakBallDeBuffCount = 0;//�v���C���[�P������ʂ̃f�o�t���󂯂Ă���^�[�����̃J�E���g��錾���A�J�E���g���O�ɂ���B



	do
	{
		//===============================================
		//�v���C���[�P�̃^�[��
		//===============================================
		if (nTurnAttack == 0)
		{

			while (1)//�v���C���[�P�̃^�[������
			{
				printf("                                   %d�^�[����\n", nTurn/2+1);

				printf("            <<�v���C���[�P>>�@�@       vs�@�@      <<�v���C���[�Q>>\n");

				printf("  ���O�F%16s                      %16s\n", &g_aPlayer[0].aName[0], &g_aPlayer[1].aName[0]);

				printf("  \x1b[32m�̗́F%16.f                     %16.f\n", g_aPlayer[0].fLife, g_aPlayer[1].fLife);

				printf("\x1b[31m�U���́F%16.f                     %16.f\n", g_aPlayer[0].fATK, g_aPlayer[1].fATK);

				printf("\x1b[36m�h��́F%16.f                     %16.f\n", g_aPlayer[0].fDEF, g_aPlayer[1].fDEF);

				printf("    \x1b[33mMP�F%16.f                     %16.f\x1b[0m\n", g_aPlayer[0].fMP, g_aPlayer[1].fMP);

				printf("+<<�v���C���[�P�̃^�[��>>\n");
				printf("  %s�͂ǂ�����H                       \n", &g_aPlayer[0].aName[0]);
				printf("+-------------------------------------+\n");
				printf("|  �P�F�U��\x1b[0m�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n");
				printf("|  �Q�F�X�L���A�^�b�N\x1b[0m                 |\n");
				printf("|  �R�F�A�C�e�����g��(�c��%d��)\x1b[0m        |\n", nPlayer1ItemRouletteUseCount);
				printf("|  �S�F�������Ȃ�\x1b[0m                     |\n");
				printf("+-------------------------------------+\n");
				printf("���F");
				nChoose = 0;//������
				scanf("%d", &nChoose);

				switch (nChoose)
				{
				case 4:
					pPlayer->fMP = pPlayer->fMP + 10;//�u�s�����Ȃ��v��I�������ꍇ�A�v���C���[�P�̂l�o���P�O�񕜂���
					printf("%s�͗l�q�����Ă���I", &pPlayer->aName[0]);
					rewind(stdin);
					getchar();
					system("cls");//��ʂ����Z�b�g����B
					break;

				case 1:
					pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
					Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);
					rewind(stdin);
					getchar();
					system("cls");//��ʂ����Z�b�g����
					break;

				case 2://�X�L���R�}���h
					if (bPlayer1PhantomStabDebuff == false)//�t�@���g���X�^�u�́u�X�L���s�v�̃f�o�t���������Ă��Ȃ��ꍇ�ɏ��������s����B
					{

						if (g_aPlayer[0].nCheckJob == 1)//�v���C���[�P�̑I�������E�Ƃ��u���m�v�Ȃ珈�������s����B================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("�P�F�C���p�N�g�G�b�W�u�U����*3�̂̃_���[�W��^����B�v(MP-30)\n");
							printf("�Q�F���J�o�[�q�[���u�ő�g�o�̂Q�T�����̂g�o���񕜂���v(MP-80)\n");
							printf("�R�F�J�E���^�[�X�^�C���u�R�^�[���̊ԁA�m���Œʏ�U�����J�E���^�[������B�v(MP-40)\n���F");
							nSkillChoose = 0;//������
							while (1)
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 30)//�v���C���[�P��MP���R�O�ȏ�Ȃ珈�������s����B
									{
										Player1InpactEdge(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�C���p�N�g�G�b�W�v�����s����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 80)//�v���C���[�P��MP��80�ȏ�Ȃ珈�������s����B
									{
										Player1RecoverHeal(&g_aPlayer[0], fPlayer1MaxHp);//�v���C���[�P�̃X�L���u���J�o�[�q�[���v�����s����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 40)
									{
										pPlayer->fMP = pPlayer->fMP - 40;
										printf("%s�̓J�E���^�[�̎p�����Ƃ����I\n", &g_aPlayer[PlayerNumber1].aName[0]);//�v���C���[�P�̋Z�F�u�J�E���^�[�X�^�C���v
										Player1CounterFlag = true;//�v���C���[�P�̃J�E���^�[�t���O��true�ɂ���i�ʏ�U�����󂯂��ꍇ�A�m���ŃJ�E���^�[�i�T�O���j
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}

									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[0].nCheckJob == 2)//�v���C���[�P�̑I�������E�Ƃ��u��l�v�Ȃ珈�������s����B======================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("�P�F���C���A���[�u�U���́��O�D�T�̂W�A��j�U���I�v�i�������S�O���j(MP-25)\n");
							printf("�Q�F�p���_�E���A���[�u�U����*2�̃_���[�W��^���A�m���łQ�^�[���U���̓_�E���v(ATK��50%)(�f�o�t�m���S�O���j(MP-40)\n");
							printf("�R�F�X�^�[�A���[�u�G�Ɏ����̍U����*5�̖h�䖳���̍U�����s���I�v(MP-60)\n���F");
							nSkillChoose = 0;//������
							while (1)
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 25)//�v���C���[�P��MP��25�ȏ�Ȃ珈�������s����B
									{
										Player1RainArrow(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�C���p�N�g�G�b�W�v�����s����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 40)//�v���C���[�P��MP���S�O�ȏ�Ȃ珈�������s����B
									{
										Player2PowerDownArrowDebuff = Player1PowerDownArrow(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�p���_�E���A���[�v�����s����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 60)//�v���C���[�P��MP���U�O�ȏ�Ȃ珈�������s����B
									{
										Player1StarArrow(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�X�^�[�A���[�v�����s����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}

									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber1].nCheckJob == 3)//�v���C���[�P�̑I�������E�Ƃ��u�ޏ��v�Ȃ珈�������s����B======================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("�P�F���̐w�u�T�^�[���̊ԁA�h��͂��㏸������B�v�iDEF��40���j(MP-35)\n");
							printf("�Q�F�z�[���[���C�u�U����+�h��́�1.2�̃_���[�W��^���A����̂l�o���P�T���炷�B�v(�l�o�_���[�W)(MP-45)\n");
							printf("�R�F��S�̐w�u�R�^�[���̊ԁA�ʏ�U�����m���ŃN���e�B�J���ɂȂ�B�v�iCRT��35���j(MP-55)\n���F");
							nSkillChoose = 0;//������
							while (1)
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 35 && bPlayer1DefenceLineBuff == false)//�v���C���[�P��MP��35�ȏォ�A���̐w���������Ă��Ȃ��ꍇ�ɏ��������s����B
									{
										Player1DefenceLine(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u���̐w�v�����s����B
										bPlayer1DefenceLineBuff = true;//�X�L���̃o�t�����������̂Ńt���O��true�ɂ���B
									}
									else if (bPlayer1DefenceLineBuff == true)
									{
										printf("���̐w�͔��������I�@��ނ𓾂��ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}

									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 45)//�v���C���[�P��MP���S�T�ȏ�Ȃ珈�������s����B
									{
										Player1HolyRay(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�z�[���[���C�v�����s����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 55)//�v���C���[�P��MP���T�T�ȏ�Ȃ珈�������s����B
									{
										pPlayer->fMP = pPlayer->fMP - 55;//�l�o���T�T���炷�B
										printf("%s�̉�S�̐w�I\n", &pPlayer[PlayerNumber1].aName[0]);//�v���C���[�P�̃X�L���u��S�̐w�v�𔭓�����B 
										bPlayer1CriticalLineBuff = true;//�o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂ���B
										printf("�R�^�[���̊ԁA%s�̒ʏ�U���̃N���e�B�J�������㏸����I\n", &g_aPlayer[PlayerNumber1].aName[0]);
									}
									else if (bPlayer1CriticalLineBuff == true)
									{
										printf("��S�̐w�͔��������I�@��ނ𓾂��ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}

									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber1].nCheckJob == 4)//�v���C���[�P�̑I�������E�Ƃ��u�z���S�v�Ȃ珈�������s����B================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("�P�F�u���b�e�B�o�C�g�u�U����*2.2�̃_���[�W��^���A�^�����_���[�W�̂V�T���g�o���񕜂���I�v(�z���U���j(MP-35)\n");
							printf("�Q�F�N���C�W�[�O���r�e�B�u�m���œG�̌��݂g�o���Q�O�����炷�B���͎����̂g�o���P�T�O�O�񕜂���B�v(gravity20��)(30%)(MP-65)\n");
							printf("�R�F�t�@���g���X�^�u�u�U���́�3.1�̃_���[�W��^���A�m���łR�^�[���̊ԁA�X�L���s��t�^����B�v(SKILL�~35��)(MP-50)\n���F");
							nSkillChoose = 0;//������
							while (1)
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 35)//�v���C���[�P��MP���R�T�ȏ�Ȃ珈�������s����B
									{
										Player1BloodyBite(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�u���b�f�B�o�C�g�v�����s����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 65)//�v���C���[�P��MP��65�ȏ�Ȃ珈�������s����B
									{
										Player1CrazyGracity(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�N���C�W�[�O���r�e�B�v�����s����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 50)//�v���C���[�P�̂l�o���T�O�ȏ�Ȃ珈�������s����B
									{
										bPlayer2PhantomStabDebuff = Player1PhantomStab(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�t�@���g���X�^�u�v�𔭓�����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}

									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber1].nCheckJob == 5)//�v���C���[�P�̑I�������E�Ƃ��u�Ȋw�ҁv�Ȃ珈�������s����B================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("�P�F�|�C�Y���~�X�g�u�U����*2.7�̃_���[�W��^���A�m���łR�^�[���̊ԁA�ŏ�Ԃɂ���B�v(��35���j(MP-40)\n");
							printf("�Q�F�X�`�[���o�[�X�g�u�m���Łi�����̍U���́{����̖h��́j��4.5�̑�_���[�W��^����B���͎����ɉΏ���Ԃ�t�^����B(MP-60)\n");
							printf("�R�F�C�J�T�}���[�U�[�u�G�̍U����*2.6�̃_���[�W��^���A�m���œG�̖h��͂��R�^�[���̊ԁA�S�O��������B(DEF��40��)(MP-45)\n���F");
							nSkillChoose = 0;//������
							while (1)
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 40)//�v���C���[�P��MP���S�O�ȏ�Ȃ珈�������s����B
									{
										bPlayer2PoisonMistState = Player1PoisonMist(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�|�C�Y���~�X�g�v�����s����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 60)//�v���C���[�P��MP��60�ȏ�Ȃ珈�������s����B
									{
										bPlayer1SteamBurstBurn = Player1SteamBurst(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�X�`�[���o�[�X�g�v�����s����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 45)//�v���C���[�P�̂l�o��45�ȏ�Ȃ珈�������s����B
									{
										bPlayer2SquitLaserDebuff = Player1SquidLaser(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�C�J�T�}���[�U�[�v�𔭓�����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}

									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber1].nCheckJob == 6)//�v���C���[�P�̑I�������E�Ƃ��u���@�g���v�Ȃ珈�������s����B================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber1].fMP);
							printf("�P�F�t���C���E�F�[�u�u�U����*1.1�̂S�A���U�������A�m���łQ�^�[���̊ԁA�U���͂��Q�T���㏸������B�v(MP-35)\n");
							printf("�Q�F�X���b�v�}�W�b�N�u�����̍U���͂Ɩh��͂̐��l�����ւ���B�v(MP-25)\n");
							printf("�R�F�A�C�X�����X�u�����̍U����*4.5�̃_���[�W��^���A�m���łT�^�[���̊ԁA����̃A�C�e���R�}���h�𕕈󂷂�B�v(MP-70)\n���F");
							nSkillChoose = 0;//������
							while (1)
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber1].fMP >= 35)//�v���C���[�P��MP���R�T�ȏ�Ȃ珈�������s����B
									{
										bPlayer1FlameWaveBuff = Player1FlameWave(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�t���C���E�F�[�u�v�����s����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber1].fMP >= 25)//�v���C���[�P��MP��25�ȏ�Ȃ珈�������s����B
									{
										printf("%s�̃X���b�v�}�W�b�N�I", &pPlayer->aName[0]);
										pPlayer->fMP = pPlayer->fMP - 25;//�v���C���[�P��MP���Q�T���炷�B
										//�U���͂Ɩh��͂̒l�����ւ���B
										pPlayer->fATK = fPlayer1MaxDEF;
										pPlayer->fDEF = fPlayer1MaxATK;

										fPlayer1MaxATK = pPlayer->fATK;
										fPlayer1MaxDEF = pPlayer->fDEF;

										printf("%s�̍U���͂Ɩh��͂̐��l������ւ�����I\n", &pPlayer->aName[0]);
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber1].fMP >= 70)//�v���C���[�P�̂l�o��70�ȏ�Ȃ珈�������s����B
									{
										bPlayer2IceLanceDebuff = Player1IceLance(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�A�C�X�����X�v�𔭓�����B
									}
									else
									{
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}

									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
					}
					else if (bPlayer1PhantomStabDebuff == true)
					{
						pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
						printf("�X�L���͌��ݎg�p�ł��Ȃ��I(�L�E�ցE�M)��ނ������ʏ�U���I\n");
						getchar();
						Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
					}
					rewind(stdin);
					getchar();
					system("cls");//��ʂ����Z�b�g����
					break;

				case 3://�A�C�e���K�`���̃R�}���h
					if (bPlayer1IceLanceDebuff == false)//�v���C���[�P���X�L���u�A�C�X�����X�v�̓�����ʁu�A�C�e������v���󂯂Ă��Ȃ������珈�������s����B
					{
						if (nPlayer1ItemRouletteUseCount != 0)//�A�C�e���g�p�񐔂̃J�E���g���O����Ȃ�������
						{
							nPlayer1ItemRouletteUseCount--;//�v���C���[�P�̃A�C�e���g�p�񐔂��P���炷�B
							printf("�A�C�e���K�`���X�^�[�g�I\n");
							srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B
							nPlayer1ItemRouletteRandum = rand() % 16 + 1;//�P�T���ڂ̒������𒊑I����B
							getchar();//�L�[���͑҂�

							if (nPlayer1ItemRouletteRandum == 1)
							{
								pPlayer->fLife = pPlayer->fLife + 1200;
								printf("�}�b�V�����[�����Q�b�g�����I�̗͂��P�Q�O�O�񕜂���I\n");
							}
							if (nPlayer1ItemRouletteRandum == 2)
							{
								pPlayer->fLife = pPlayer->fLife + 2500;
								printf("�X�[�p�[�}�b�V�����[�����Q�b�g�����I�̗͂��Q�T�O�O�񕜂���I\n");
							}
							if (nPlayer1ItemRouletteRandum == 3)
							{
								pPlayer->fLife = pPlayer->fLife + 4000;
								printf("�E���g���}�b�V�����[�����Q�b�g�����I�i���A�A�C�e�����j�̗͂��S�O�O�O�񕜂���I\n");
							}
							if (nPlayer1ItemRouletteRandum == 4)
							{
								pPlayer->fMP = pPlayer->fMP + 30;
								printf("�}�W�b�N�T�C�_�[�r���Q�b�g�����I�l�o���R�O�񕜂���I\n");
							}
							if (nPlayer1ItemRouletteRandum == 5)
							{
								pPlayer->fMP = pPlayer->fMP + 60;
								printf("�}�W�b�N�T�C�_�[�l���Q�b�g�����I�l�o���U�O�񕜂���I\n");
							}
							if (nPlayer1ItemRouletteRandum == 6)
							{
								pPlayer->fMP = pPlayer->fMP + 100;
								printf("�}�W�b�N�T�C�_�[�k���Q�b�g�����I�i���A�A�C�e�����j�l�o���P�O�O�񕜂���I\n");
							}
							if (nPlayer1ItemRouletteRandum == 7)
							{
								pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.1);
								printf("�J���[���C�X���Q�b�g�����I�S�^�[���̊ԁA�U���͂��P�O���㏸����I\n");
								bPlayer1CurryRiceBuffLV1 = true;
							}
							if (nPlayer1ItemRouletteRandum == 8)
							{
								pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.3);
								printf("�V�[�t�[�h�J���[���Q�b�g�����I�S�^�[���̊ԁA�U���͂��R�O���㏸����I\n");
								bPlayer1CurryRiceBuffLV2 = true;
							}
							if (nPlayer1ItemRouletteRandum == 9)
							{
								pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.5);
								printf("�|�[�N�J���[���Q�b�g�����I�i���A�A�C�e�����j�S�^�[���̊ԁA�U���͂��T�O���㏸����I\n");
								bPlayer1CurryRiceBuffLV3 = true;
							}
							if (nPlayer1ItemRouletteRandum == 10)
							{
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.1);
								printf("�����ނ��т��Q�b�g�����I�S�^�[���̊ԁA�h��͂��P�O���㏸����I\n");
								bPlayer1RiceBallBuffLV1 = true;
							}
							if (nPlayer1ItemRouletteRandum == 11)
							{
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.3);
								printf("�����ނ��т��Q�b�g�����I�S�^�[���̊ԁA�h��͂��R�O���㏸����I\n");
								bPlayer1RiceBallBuffLV2 = true;
							}
							if (nPlayer1ItemRouletteRandum == 12)
							{
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.5);
								printf("�����炨�ނ��т��Q�b�g�����I�i���A�A�C�e�����j�S�^�[���̊ԁA�h��͂��T�O���㏸����I\n");
								bPlayer1RiceBallBuffLV3 = true;
							}
							if (nPlayer1ItemRouletteRandum == 13)
							{
								srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B
								nShootingStar = rand() % 2500 + 500;//�T�O�O�`�R�O�O�O�̌Œ�_���[�W��^����B
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fLife = pPlayer->fLife - nShootingStar;
								printf("����ɗ����Q���~�蒍���I%s��%d�̃_���[�W�I\n", &pPlayer->aName[0], nShootingStar);
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer1ItemRouletteRandum == 14)
							{
								Player1WaveCanon(&g_aPlayer[0]);

							}
							if (nPlayer1ItemRouletteRandum == 15)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								printf("����ʂ���ɓ��ꂽ�I�R�^�[���̊ԁA����̍U���͂Ɩh��͂��S�O����������I\n");
								pPlayer->fATK = pPlayer->fATK - (fPlayer2MaxATK * 0.4);
								pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.4);
								pPlayer--;//�|�C���^����v���C���[�P��
								bPlayer2WeakBallDeBuff = true;//�f�o�t�����������̂ŁA�t���O��true�ɂ���B
							}
							if (nPlayer1ItemRouletteRandum == 16)
							{
								printf("������o�b�W����ɓ��ꂽ�I�����U���͂��P�O�O�O�ȉ��A���͖h��͂��V�O�O�ȉ��̏ꍇ�A�i�v�I�ɍU���͂Ɩh��͂̐��l���{�R�O�O����B\n����ȊO�Ȃ�{�P�O�O����B\n");
								if (pPlayer->fATK <= 1000 || pPlayer->fDEF <= 700)
								{
									pPlayer->fATK = pPlayer->fATK + 300;
									fPlayer1MaxATK = pPlayer->fATK;
									pPlayer->fDEF = pPlayer->fDEF + 300;
									fPlayer1MaxDEF = pPlayer->fDEF;
								}
								else
								{
									pPlayer->fATK = pPlayer->fATK + 100;
									fPlayer1MaxATK = pPlayer->fATK;
									pPlayer->fDEF = pPlayer->fDEF + 100;
									fPlayer1MaxDEF = pPlayer->fDEF;
								}

							}
						}
						else
						{
							printf("�A�C�e���͂����g�p�ł��Ȃ��I��ނ𓾂��ʏ�U���I\n");
							pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
							getchar();
							Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
						}
					}
					else
					{
						printf("�A�C�e���R�}���h�͕��󂳂�Ă���I��ނ𓾂��ʏ�U���I\n");
						pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
						getchar();
						Player1NormalAttack(&g_aPlayer[0], Player2CounterFlag, bPlayer1CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
					}

					rewind(stdin);
					getchar();
					system("cls");//��ʂ����Z�b�g����
					break;

				default:
					printf("���͂��ꂽ�l���Ⴄ��I\n\n");
					rewind(stdin);
					getchar();
					system("cls");//��ʂ����Z�b�g����
					break;
				}

				if (nChoose == 1 || nChoose == 2 || nChoose == 3 || nChoose == 4)
				{
					nTurnAttack = 1;
					break;
				}
			}



		}
		//==========================================================
		//�v���C���[�Q�̃^�[��
		//==========================================================
		else
		{

			while (1)//�v���C���[�Q�̃^�[������
			{
				printf("                                   %d�^�[����\n", nTurn/2 + 1);

				printf("            <<�v���C���[�P>>�@�@       vs�@�@      <<�v���C���[�Q>>\n");

				printf("  ���O�F%16s                      %16s\n", &g_aPlayer[0].aName[0], &g_aPlayer[1].aName[0]);

				printf("  \x1b[32m�̗́F%16.f                     %16.f\n", g_aPlayer[0].fLife, g_aPlayer[1].fLife);

				printf("\x1b[31m�U���́F%16.f                     %16.f\n", g_aPlayer[0].fATK, g_aPlayer[1].fATK);

				printf("\x1b[36m�h��́F%16.f                     %16.f\n", g_aPlayer[0].fDEF, g_aPlayer[1].fDEF);

				printf("    \x1b[33mMP�F%16.f                     %16.f\x1b[0m\n", g_aPlayer[0].fMP, g_aPlayer[1].fMP);

				printf("+<<�v���C���[�Q�̃^�[��>>\n");
				printf("  %s�͂ǂ�����H                       \n", &g_aPlayer[1].aName[0]);
				printf("+-------------------------------------+\n");
				printf("|  �P�F�U��\x1b[0m�@�@�@�@�@�@�@�@�@�@�@�@�@ |\n");
				printf("|  �Q�F�X�L���A�^�b�N\x1b[0m                 |\n");
				printf("|  �R�F�A�C�e�����g��(�c��%d��)\x1b[0m        |\n", nPlayer2ItemRouletteUseCount);
				printf("|  �S�F�������Ȃ�\x1b[0m                     |\n");
				printf("+-------------------------------------+\n");
				printf("���F");
				nChoose = 0;//������
				scanf("%d", &nChoose);

				switch (nChoose)
				{
				case 4://�������Ȃ�
					pPlayer++;//�|�C���^���v���C���[�Q��
					pPlayer->fMP = pPlayer->fMP + 10;
					printf("%s�͗l�q�����Ă���I", &pPlayer->aName[0]);
					pPlayer--;//�|�C���^���v���C���[�P��

					rewind(stdin);
					getchar();
					system("cls");//��ʂ����Z�b�g����B
					break;

				case 1://�ʏ�U��
					pPlayer++;//�|�C���^���v���C���[�Q��
					pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U���������Ƀv���C���[�Q�̂l�o���T�񕜂���B

					Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);
					pPlayer--;//�|�C���^���v���C���[�P��
					rewind(stdin);
					getchar();
					system("cls");//��ʂ����Z�b�g����B
					break;

				case 2://�X�L���U��
					if (bPlayer2PhantomStabDebuff == false)//��Ԉُ�u�X�L���s�v���������Ă��Ȃ������ꍇ�A���������s����B
					{
						if (g_aPlayer[1].nCheckJob == 1)//�v���C���[�Q�̑I�������E�Ƃ��u���m�v�Ȃ珈�������s����B==============================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("�P�F�C���p�N�g�G�b�W�u�U����*3�̂̃_���[�W��^����B�v(MP-30)\n");
							printf("�Q�F���J�o�[�q�[���u�ő�g�o�̂Q�T�����̂g�o���񕜂���v(MP-80)\n");
							printf("�R�F�J�E���^�[�X�^�C���u�R�^�[���̊ԁA�m���Œʏ�U�����J�E���^�[������B�v(MP-40)\n���F");
							nSkillChoose = 0;//������
							while (1)//�u���m�v�̋Z�I��
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 30)
									{
										Player2InpactEdge(&g_aPlayer[0]);//�v���C���[�P�̃X�L���u�C���p�N�g�G�b�W�v�����s����B
									}
									else
									{
										pPlayer++;//�|�C���^���Ղꂢ��[�Q�ց@
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U���������Ƀv���C���[�Q�̂l�o���T�񕜂���B
										printf("MP������Ȃ�(�L�E�ցE`)��ނ𓾂��ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^���v���C���[�P��
									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 65)//�v���C���[�Q��MP��65�ȏ�Ȃ珈�������s��	��B
									{
										Player2RecoverHeal(&g_aPlayer[0], fPlayer2MaxHp);//�v���C���[2�̃X�L���u�N���C�W�[�O���r�e�B�v�����s����B
									}
									else
									{
										pPlayer++;//�|�C���^���v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U���������Ƀv���C���[�Q�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^���v���C���[�P��
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 40)
									{
										pPlayer++;//�|�C���^���v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP - 40;
										printf("%s�̓J�E���^�[�̎p�����Ƃ����I\n", &g_aPlayer[PlayerNumber2].aName[0]);//�v���C���[�Q�̋Z�F�u�J�E���^�[�X�^�C���v
										Player2CounterFlag = true;
										pPlayer--;
									}
									else
									{
										pPlayer++;//�|�C���^���v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U���������Ƀv���C���[�Q�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^���v���C���[�P��
									}
									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber2].nCheckJob == 2)//�v���C���[2�̑I�������E�Ƃ��u��l�v�Ȃ珈�������s����B======================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("�P�F���C���A���[�u�U���́��O�D�T�̂W�A��j�U���I�v�i�������S�O���j(MP-25)\n");
							printf("�Q�F�p���_�E���A���[�u�U����*2�̃_���[�W��^���A�m���łQ�^�[���U���̓_�E���v(ATK��50%)(�f�o�t�m���S�O���j(MP-40)\n");
							printf("�R�F�X�^�[�A���[�u�G�Ɏ����̍U����*5�̖h�䖳���̍U�����s���I�v(MP-60)\n���F");
							nSkillChoose = 0;//������
							while (1)
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 25)//�v���C���[2��MP��25�ȏ�Ȃ珈�������s����B
									{
										Player2RainArrow(&g_aPlayer[0]);//�v���C���[2�̃X�L���u���C���A���[�v�����s����B
									}
									else
									{
										pPlayer++;//�|�C���^���v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[2�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 40)//�v���C���[2��MP���S�O�ȏ�Ȃ珈�������s����B
									{
										Player1PowerDownArrowDebuff = Player2PowerDownArrow(&g_aPlayer[0]);//�v���C���[2�̃X�L���u�p���_�E���A���[�v�����s����B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[2�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 60)//�v���C���[�Q��MP���U�O�ȏ�Ȃ珈�������s����B
									{
										Player2StarArrow(&g_aPlayer[0]);//�v���C���[�Q�̃X�L���u�X�^�[�A���[�v�����s����B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��
									}

									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber2].nCheckJob == 3)//�v���C���[�Q�̑I�������E�Ƃ��u�ޏ��v�Ȃ珈�������s����B======================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("�P�F���̐w�u�T�^�[���̊ԁA�h��͂��㏸������B�v�iDEF��40���j(MP-35)\n");
							printf("�Q�F�z�[���[���C�u�U����+�h��́�1.2�̃_���[�W��^���A����̂l�o���P�T���炷�B�v(�l�o�_���[�W)(MP-45)\n");
							printf("�R�F��S�̐w�u�R�^�[���̊ԁA�ʏ�U�����m���ŃN���e�B�J���ɂȂ�B�v�iCRT��35���j(MP-55)\n���F");
							nSkillChoose = 0;//������
							while (1)
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 35 && bPlayer2DefenceLineBuff == false)//�v���C���[�Q��MP��35�ȏォ�A���̐w���������Ă��Ȃ��ꍇ�ɏ��������s����B
									{
										Player2DefenceLine(&g_aPlayer[0]);//�v���C���[�Q�̃X�L���u���̐w�v�����s����B
										bPlayer2DefenceLineBuff = true;//�X�L���̃o�t�����������̂Ńt���O��true�ɂ���B
									}
									else if (bPlayer2DefenceLineBuff == true)
									{
										printf("���̐w�͔��������I�@��ނ𓾂��ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 45)//�v���C���[�Q��MP��45�ȏ�Ȃ珈�������s����B
									{
										Player2HolyRay(&g_aPlayer[0]);//�v���C���[�Q�̃X�L���u�z�[���[���C�v�����s����B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 55)//�v���C���[�Q��MP���T�T�ȏ�Ȃ珈�������s����B
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP - 55;//�l�o���T�T���炷�B
										printf("%s�̉�S�̐w�I\n", &g_aPlayer[PlayerNumber2].aName[0]);//�v���C���[�Q�̃X�L���u��S�̐w�v�𔭓�����B 
										bPlayer2CriticalLineBuff = true;//�o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂ���B
										printf("�R�^�[���̊ԁA%s�̒ʏ�U���̃N���e�B�J�������㏸����I\n", &g_aPlayer[PlayerNumber2].aName[0]);
										pPlayer--;//�|�C���^����v���C���[�P��
									}
									else if (bPlayer1CriticalLineBuff == true)
									{
										printf("��S�̐w�͔��������I�@��ނ𓾂��ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��
									}
									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber2].nCheckJob == 4)//�v���C���[�Q�̑I�������E�Ƃ��u�z���S�v�Ȃ珈�������s����B================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("�P�F�u���b�e�B�o�C�g�u�U����*2.2�̃_���[�W��^���A�^�����_���[�W�̂V�T���g�o���񕜂���I�v(�z���U���j(MP-35)\n");
							printf("�Q�F�N���C�W�[�O���r�e�B�u�m���œG�̌��݂g�o���Q�O�����炷�B���͎����̂g�o���P�T�O�O�񕜂���B�v(MP-65)\n");
							printf("�R�F�t�@���g���X�^�u�u�U���́�3.1�̃_���[�W��^���A�m���łR�^�[���̊ԁA�X�L���s��t�^����B�v(SKILL�~35��)(MP-50)\n���F");
							nSkillChoose = 0;//������
							while (1)
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 35)//�v���C���[�Q��MP���R�T�ȏ�Ȃ珈�������s����B
									{
										Player2BloodyBite(&g_aPlayer[0]);//�v���C���[�Q�̃X�L���u�u���b�f�B�o�C�g�v�����s����B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�Q�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��
									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 65)//�v���C���[�Q��MP��65�ȏ�Ȃ珈�������s����B
									{
										Player2CrazyGravity(&g_aPlayer[0]);//�v���C���[�Q�̃X�L���u�N���C�W�[�O���r�e�B�v�����s����B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�Q�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 50)//�v���C���[�Q�̂l�o���T�O�ȏ�Ȃ珈�������s����B
									{
										bPlayer1PhantomStabDebuff = Player2PhantomStab(&g_aPlayer[0]);//�v���C���[�Q�̃X�L���u�t�@���g���X�^�u�v�𔭓�����B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�Q�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��
									}

									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber2].nCheckJob == 5)//�v���C���[�Q�̑I�������E�Ƃ��u�Ȋw�ҁv�Ȃ珈�������s����B================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("�P�F�|�C�Y���~�X�g�u�U����*2.7�̃_���[�W��^���A�m���łR�^�[���̊ԁA�ŏ�Ԃɂ���B�v(��35���j(MP-40)\n");
							printf("�Q�F�X�`�[���o�[�X�g�u�m���Łi�����̍U���́{����̖h��́j��4.5�̑�_���[�W��^����B���͎����ɉΏ���Ԃ�t�^����B(MP-60)\n");
							printf("�R�F�C�J�T�}���[�U�[�u�G�̍U����*2.6�̃_���[�W��^���A�m���œG�̖h��͂��R�^�[���̊ԁA�S�O��������B(DEF��40��)(MP-45)\n���F");
							nSkillChoose = 0;//������
							while (1)
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 40)//�v���C���[�Q��MP���S�O�ȏ�Ȃ珈�������s����B
									{
										bPlayer1PoisonMistState = Player2PoisonMist(&g_aPlayer[0]);//�v���C���[�Q�̃X�L���u�|�C�Y���~�X�g�v�����s����B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�Q�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 60)//�v���C���[�Q��MP��60�ȏ�Ȃ珈�������s����B
									{
										bPlayer2SteamBurstBurn = Player2SteamBurst(&g_aPlayer[0]);//�v���C���[�Q�̃X�L���u�X�`�[���o�[�X�g�v�����s����B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�Q�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 45)//�v���C���[�Q�̂l�o���S�T�ȏ�Ȃ珈�������s����B
									{
										bPlayer1SquitLaserDebuff = Player2SquidLaser(&g_aPlayer[0]);//�v���C���[�Q�̃X�L���u�C�J�T�}���[�U�[�v�𔭓�����B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�Q�̂l�o���T�񕜂���B
										pPlayer--;//�|�C���^����v���C���[�P��
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
									}

									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
						else if (g_aPlayer[PlayerNumber2].nCheckJob == 6)//�v���C���[�Q�̑I�������E�Ƃ��u���@�g���v�Ȃ珈�������s����B================================================================
						{
							printf("+---------------------------------------------------------------------------+\n");
							printf("<<�g�������X�L����I��ł�������>>\n");
							printf("�c��MP:%.f\n", g_aPlayer[PlayerNumber2].fMP);
							printf("�P�F�t���C���E�F�[�u�u�U����*1.1�̂S�A���U�������A�m���łQ�^�[���̊ԁA�U���͂��Q�T���㏸������B�v(MP-35)\n");
							printf("�Q�F�X���b�v�}�W�b�N�u�����̍U���͂Ɩh��͂̐��l�����ւ���B�v(MP-25)\n");
							printf("�R�F�A�C�X�����X�u�����̍U����*4.5�̃_���[�W��^���A�m���łT�^�[���̊ԁA����̃A�C�e���R�}���h�𕕈󂷂�B�v(MP-70)\n���F");
							nSkillChoose = 0;//������
							while (1)
							{
								scanf("%d", &nSkillChoose);//�X�L���I��������B



								switch (nSkillChoose)
								{
								case 1:
									if (g_aPlayer[PlayerNumber2].fMP >= 35)//�v���C���[�Q��MP���R�T�ȏ�Ȃ珈�������s����B
									{
										bPlayer2FlameWaveBuff = Player2FlameWave(&g_aPlayer[0]);//�v���C���[�Q�̃X�L���u�t���C���E�F�[�u�v�����s����B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�Q�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��

									}
									break;

								case 2:
									if (g_aPlayer[PlayerNumber2].fMP >= 25)//�v���C���[�Q��MP��25�ȏ�Ȃ珈�������s����B
									{
										//�|�C���^����v���C���[�Q��
										pPlayer++;
										printf("%s�̃X���b�v�}�W�b�N�I", &pPlayer->aName[0]);
										pPlayer->fMP = pPlayer->fMP - 25;//�v���C���[�P��MP���Q�T���炷�B
										//�U���͂Ɩh��͂̒l�����ւ���B
										pPlayer->fATK = fPlayer2MaxDEF;
										pPlayer->fDEF = fPlayer2MaxATK;

										fPlayer2MaxATK = pPlayer->fATK;
										fPlayer2MaxDEF = pPlayer->fDEF;

										printf("%s�̍U���͂Ɩh��͂̐��l������ւ�����I\n", &pPlayer->aName[0]);
										pPlayer--;//�|�C���^����v���C���[�P��
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�Q�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��
									}
									break;

								case 3:
									if (g_aPlayer[PlayerNumber2].fMP >= 70)//�v���C���[�Q�̂l�o��70�ȏ�Ȃ珈�������s����B
									{
										bPlayer1IceLanceDebuff = Player2IceLance(&g_aPlayer[0]);//�v���C���[�Q�̃X�L���u�A�C�X�����X�v�𔭓�����B
									}
									else
									{
										pPlayer++;//�|�C���^����v���C���[�Q��
										pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�P�̂l�o���T�񕜂���B
										printf("�l�o������Ȃ�(�L�E�ցE�M)��ނ������ʏ�U���I\n");
										getchar();
										Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
										pPlayer--;//�|�C���^����v���C���[�P��
									}

									break;

								default:
									while (getchar() != '\n');//�����ȊO����͂����Ƃ��ɃG���[���b�Z�[�W���o���B
									printf("���͂��ꂽ�l���Ⴄ��I\n���F");
									break;
								}

								if (nSkillChoose == 1 || nSkillChoose == 2 || nSkillChoose == 3)
								{
									break;
								}
							}

						}
					}
					else if (bPlayer2PhantomStabDebuff == true)//�t�@���g���X�^�u�́u�X�L���s�v���������Ă����ꍇ
					{
						pPlayer++;//�|�C���^����v���C���[�Q��
						pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�Q�̂l�o���T�񕜂���B
						printf("�X�L���͌��ݎg�p�ł��Ȃ��I(�L�E�ցE�M)��ނ������ʏ�U���I\n");
						getchar();
						Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
						pPlayer--;//�|�C���^����v���C���[�P��
					}

					rewind(stdin);
					getchar();
					system("cls");//��ʂ����Z�b�g����B
					break;

				case 3://�A�C�e�����g��

					if (bPlayer2IceLanceDebuff == false)///�v���C���[�Q���X�L���u�A�C�X�����X�v�̃A�C�e������̃f�o�t���󂯂Ă��Ȃ��ꍇ�ɏ���������B
					{

						if (nPlayer2ItemRouletteUseCount != 0)//�A�C�e���g�p�񐔂̃J�E���g���O����Ȃ�������
						{
							nPlayer2ItemRouletteUseCount--;//�A�C�e���g�p�񐔂��P���炷�B
							printf("�A�C�e���K�`���X�^�[�g�I\n");
							srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B
							nPlayer2ItemRouletteRandum = rand() % 16 + 1;//�P�T���ڂ̒������𒊑I����B
							getchar();//�L�[���͑҂�

							if (nPlayer2ItemRouletteRandum == 1)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fLife = pPlayer->fLife + 1200;
								printf("�}�b�V�����[�����Q�b�g�����I�̗͂��P�Q�O�O�񕜂���I\n");
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 2)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fLife = pPlayer->fLife + 2500;
								printf("�X�[�p�[�}�b�V�����[�����Q�b�g�����I�̗͂��Q�T�O�O�񕜂���I\n");
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 3)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fLife = pPlayer->fLife + 4000;
								printf("�E���g���}�b�V�����[�����Q�b�g�����I�i���A�A�C�e�����j�̗͂��S�O�O�O�񕜂���I\n");
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 4)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fMP = pPlayer->fMP + 30;
								printf("�}�W�b�N�T�C�_�[�r���Q�b�g�����I�l�o���R�O�񕜂���I\n");
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 5)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fMP = pPlayer->fMP + 60;
								printf("�}�W�b�N�T�C�_�[�l���Q�b�g�����I�l�o���U�O�񕜂���I\n");
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 6)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fMP = pPlayer->fMP + 100;
								printf("�}�W�b�N�T�C�_�[�k���Q�b�g�����I�i���A�A�C�e�����j�l�o���P�O�O�񕜂���I\n");
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 7)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.1);
								printf("�J���[���C�X���Q�b�g�����I�S�^�[���̊ԁA�U���͂��P�O���㏸����I\n");
								bPlayer2CurryRiceBuffLV1 = true;
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 8)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.3);
								printf("�V�[�t�[�h�J���[���Q�b�g�����I�S�^�[���̊ԁA�U���͂��R�O���㏸����I\n");
								bPlayer2CurryRiceBuffLV2 = true;
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 9)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.5);
								printf("�|�[�N�J���[���Q�b�g�����I�i���A�A�C�e�����j�S�^�[���̊ԁA�U���͂��T�O���㏸����I\n");
								bPlayer2CurryRiceBuffLV3 = true;
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 10)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.1);
								printf("�����ނ��т��Q�b�g�����I�S�^�[���̊ԁA�h��͂��P�O���㏸����I\n");
								bPlayer2RiceBallBuffLV1 = true;
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 11)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.3);
								printf("�����ނ��т��Q�b�g�����I�S�^�[���̊ԁA�h��͂��R�O���㏸����I\n");
								bPlayer2RiceBallBuffLV2 = true;
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 12)
							{
								pPlayer++;//�|�C���^����v���C���[�Q��
								pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.5);
								printf("�����炨�ނ��т��Q�b�g�����I�i���A�A�C�e�����j�S�^�[���̊ԁA�h��͂��T�O���㏸����I\n");
								bPlayer2RiceBallBuffLV3 = true;
								pPlayer--;//�|�C���^����v���C���[�P��
							}
							if (nPlayer2ItemRouletteRandum == 13)
							{
								srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B
								nShootingStar = rand() % 2500 + 500;//�T�O�O�`�R�O�O�O�̌Œ�_���[�W��^����B
								pPlayer->fLife = pPlayer->fLife - nShootingStar;
								printf("����ɗ����Q���~�蒍���I%s��%d�̃_���[�W�I\n", &pPlayer->aName[0], nShootingStar);

							}
							if (nPlayer2ItemRouletteRandum == 14)
							{
								Player2WaveCanon(&g_aPlayer[0]);//�v���C���[�Q���g���C�𔭓��I
							}
							if (nPlayer2ItemRouletteRandum == 15)
							{
								printf("����ʂ���ɓ��ꂽ�I�i���A�A�C�e�����j�R�^�[���̊ԁA����̍U���͂Ɩh��͂��S�O����������I\n");
								pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.4);
								pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.4);
								bPlayer1WeakBallDeBuff = true;//�f�o�t�����������̂ŁA�t���O��true�ɂ���B
							}
							if (nPlayer2ItemRouletteRandum == 16)
							{
								printf("������o�b�W����ɓ��ꂽ�I�����U���͂��P�O�O�O�ȉ��A���͖h��͂��V�O�O�ȉ��̏ꍇ�A�i�v�I�ɍU���͂Ɩh��͂̐��l���{�R�O�O����B\n����ȊO�Ȃ�{�P�O�O����B\n");
								pPlayer++;//�|�C���^����v���C���[�Q��
								if (pPlayer->fATK <= 1000 || pPlayer->fDEF <=700)
								{
									pPlayer->fATK = pPlayer->fATK + 300;
									fPlayer2MaxATK = pPlayer->fATK;
									pPlayer->fDEF = pPlayer->fDEF + 300;
									fPlayer2MaxDEF = pPlayer->fDEF;
								}
								else
								{
									pPlayer->fATK = pPlayer->fATK + 100;
									fPlayer2MaxATK = pPlayer->fATK;
									pPlayer->fDEF = pPlayer->fDEF + 100;
									fPlayer2MaxDEF = pPlayer->fDEF;
								}
								pPlayer--;//�|�C���^����v���C���[�P��
							}
						}
						else
						{
							printf("�A�C�e���͂����g�p�ł��Ȃ��I��ނ𓾂��ʏ�U���I");
							pPlayer++;//�|�C���^����v���C���[�Q��
							pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�Q�̂l�o���T�񕜂���B
							pPlayer--;//�|�C���^����v���C���[�P��
							getchar();
							Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
						}
					}
					else
					{
						printf("�A�C�e���R�}���h�����󂳂�Ă���I��ނ𓾂��ʏ�U���I\n");
						pPlayer++;//�|�C���^����v���C���[�Q��
						pPlayer->fMP = pPlayer->fMP + 5;//�ʏ�U����I�������ꍇ�A�v���C���[�Q�̂l�o���T�񕜂���B
						pPlayer--;//�|�C���^����v���C���[�P��
						getchar();
						Player2NormalAttack(&g_aPlayer[0], Player1CounterFlag, bPlayer2CriticalLineBuff);//�X�L���������ł��Ȃ������ꍇ�ɒʏ�U���𔭓��B
					}

					rewind(stdin);
					getchar();
					system("cls");//��ʂ����Z�b�g����B
					break;

				default:
					printf("���͂��ꂽ�l���Ⴄ��I\n\n");

					rewind(stdin);
					getchar();
					system("cls");//��ʂ����Z�b�g����B
					break;
				}

				if (nChoose == 1 || nChoose == 2 || nChoose == 3 || nChoose == 4)
				{
					nTurnAttack = 0;
					break;
				}

			}


		}


		//========================================================
		//�ő�l��Œ�l����������ꍇ�̏���
		//========================================================
		if (g_aPlayer[PlayerNumber1].fLife >= fPlayer1MaxHp)
		{
			g_aPlayer[PlayerNumber1].fLife = fPlayer1MaxHp;//�ő�HP���񕜂Ȃǂŏ������ꍇ�A�̗͂��ő�̗͂ɌŒ�B
		}

		if (g_aPlayer[PlayerNumber2].fLife >= fPlayer2MaxHp)
		{
			g_aPlayer[PlayerNumber2].fLife = fPlayer2MaxHp;//�ő�HP���񕜂Ȃǂŏ������ꍇ�A�̗͂��ő�̗͂ɌŒ�B
		}

		if (g_aPlayer[PlayerNumber1].fMP >= fPlayer1MaxMp)//�ő�MP���������ꍇ�ɁA����ȏ㑝���Ȃ��悤�ɌŒ�B
		{
			g_aPlayer[PlayerNumber1].fMP = fPlayer1MaxMp;
		}

		if (g_aPlayer[PlayerNumber2].fMP >= fPlayer2MaxMp)//�ő�MP���������ꍇ�A����ȏ㑝���Ȃ��悤�ɌŒ�B
		{
			g_aPlayer[PlayerNumber2].fMP = fPlayer2MaxMp;
		}


		if (g_aPlayer[PlayerNumber1].fMP <= 0)//�v���C���[�P�̂l�o���O����������ꍇ�A����ȏ㉺����Ȃ��悤�ɌŒ�B
		{
			g_aPlayer[PlayerNumber1].fMP = 0;
		}

		if (g_aPlayer[PlayerNumber2].fMP <= 0)//�v���C���[�P�̂l�o���O����������ꍇ�A����ȏ㉺����Ȃ��悤�ɌŒ�B
		{
			g_aPlayer[PlayerNumber2].fMP = 0;
		}
		//================================================================================================================

		//===============================================
		//�J�E���^�[��ԂɂȂ��Ă���̏���
		//===============================================

		if (Player1CounterFlag == true)
		{
			nPlayer1CounterCount++;//�J�E���^�[��ԂɂȂ��Ă���o�߂����^�[�����J�E���g����B

			if (nPlayer1CounterCount == 6)//�v���C���[�P�ƃv���C���[�Q���s������̂ŁA�P�^�[���ŃJ�E���g�͂Q�i�ށB�J�E���^�[�͂R�^�[���̊Ԕ�������̂ŁA�J�E���^�[�J�E���g�́u�U�v�ő��v�B
			{
				Player1CounterFlag = false;//�J�E���^�[��ԂɂȂ��Ă���R�^�[����������A
				nPlayer1CounterCount = 0;//�J�E���^�[��ԂɂȂ��Ă���̃J�E���g���O�ɂ���B
			}
		}

		if (Player2CounterFlag == true)
		{
			nPlayer2CounterCount++;//�J�E���^�[��ԂɂȂ��Ă���o�߂����^�[�����J�E���g����B

			if (nPlayer2CounterCount == 7)//�v���C���[�P�ƃv���C���[�Q���s������̂ŁA�P�^�[���ŃJ�E���g�͂Q�i�ށB�J�E���^�[�͂R�^�[���̊Ԕ�������̂ŁA�J�E���^�[�J�E���g�́u�U�v�ő��v�B
			{
				Player2CounterFlag = false;//�J�E���^�[��ԂɂȂ��Ă���R�^�[����������A
				nPlayer2CounterCount = 0;//�J�E���^�[��ԂɂȂ��Ă���̃J�E���g���O�ɂ���B
			}
		}
		//===================================================================================================================


		//=======================================================
		//�p���_�E���A���[�̃f�o�t���������Ă���Ƃ��̏���
		//=======================================================

		if (Player2PowerDownArrowDebuff == true)
		{
			nPlayer2PowerDownArrowDebuffCount++;//�v���C���[�Q���p���_�E���A���[�̃f�o�t���󂯂Ă���^�[�����̃J�E���g���{�P����B

			if (nPlayer2PowerDownArrowDebuffCount == 5)
			{
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.5);//�v���C���[�Q�̍U���͂��{�O�D�R�{����B
				pPlayer--;//�|�C���^����v���C���[�P��
				nPlayer2PowerDownArrowDebuffCount = 0;//�J�E���g���O�ɖ߂��B
				Player2PowerDownArrowDebuff = false;//�f�o�t���I������̂ŁA�����t���O��false�ɂ��Ă����B
			}
		}

		if (Player1PowerDownArrowDebuff == true)
		{
			nPlayer1PowerDownArrowDebuffCount++;//�v���C���[1���p���_�E���A���[�̃f�o�t���󂯂Ă���^�[�����̃J�E���g���{�P����B

			if (nPlayer1PowerDownArrowDebuffCount == 5)
			{

				pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.5);//�v���C���[�P�̍U���͂��{�O�D�R�{����B
				nPlayer1PowerDownArrowDebuffCount = 0;
				Player1PowerDownArrowDebuff = false;//�f�o�t���I������̂ŁA�����t���O��false�ɂ��Ă����B
			}
		}
		//===========================================================================================================================


		//=============================================
		//���̐w�̃o�t���������Ă���Ƃ��̏���
		//=============================================

		if (bPlayer1DefenceLineBuff == true)
		{
			nPlayer1DefenceLineBuffCount++;//�v���C���[�P�����̐w�̃o�t���󂯂Ă���^�[�����̃J�E���g���{�P����B

			if (nPlayer1DefenceLineBuffCount == 12)//�o�t���󂯂Ă���R�^�[����������
			{
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.4);//�v���C���[�P�̖h��͂��[�O�D�S�{����B
				bPlayer1DefenceLineBuff = false;
				nPlayer1DefenceLineBuffCount = 0;//�o�t���I������̂ŁA�o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
			}
		}

		if (bPlayer2DefenceLineBuff == true)
		{
			nPlayer2DefenceLineBuffCount++;//�v���C���[�Q�����̐w�̃o�t���󂯂Ă���^�[�����̃J�E���g���{�P����B

			if (nPlayer2DefenceLineBuffCount == 12)//�o�t���󂯂Ă���R�^�[����������
			{
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.4);//�v���C���[�Q�̖h��͂��[�O�D�S�{����B
				bPlayer2DefenceLineBuff = false;
				nPlayer2DefenceLineBuffCount = 0;//�o�t���I������̂ŁA�o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer--;//�|�C���^����v���C���[�P��
			}
		}
		//================================================================================================================================

		//==========================================================
		//��S�̐w�̃o�t���������Ă���Ƃ��̏���
		//==========================================================

		if (bPlayer1CriticalLineBuff == true)
		{
			nPlayer1CriticalLineBuffCount++;//�v���C���[�P����S�̐w�̃o�t���󂯂Ă���^�[�����̃J�E���g���{�P����B

			if (nPlayer1CriticalLineBuffCount == 8)//�o�t���󂯂����̃^�[������R�^�[���o�����Ƃ�
			{
				bPlayer1CriticalLineBuff = false;//�o�t�̃^�[�����I������̂ŁA�t���O��false�ɂ���B
				nPlayer1CriticalLineBuffCount = 0;//�o�t���I������̂ŁA�o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
			}
		}

		if (bPlayer2CriticalLineBuff == true)
		{
			nPlayer2CriticalLineBuffCount++;//�v���C���[�Q����S�̐w�̃o�t���󂯂Ă���^�[�����̃J�E���g���{�P����B

			if (nPlayer2CriticalLineBuffCount == 8)
			{
				bPlayer2CriticalLineBuff = false;//�o�t�̃^�[�����I������̂ŁA�t���O��false�ɂ���B
				nPlayer2CriticalLineBuffCount = 0;//�o�t���������̂ŁA�o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
			}
		}
		//======================================================================================================================================================

		//=================================================================================
		//�X�L���F�u�t�@���g���X�^�u�v�̃X�L���s�̃f�o�t���󂯂Ă���Ƃ��̏���
		//=================================================================================

		if (bPlayer2PhantomStabDebuff == true)
		{
			nPlayer2PhantomStabDebuffCount++;//�v���C���[�Q���t�@���g���X�^�u�̃f�o�t���󂯂Ă���^�[�����̃J�E���g���{�P����B

			if (nPlayer2PhantomStabDebuffCount == 7)
			{
				bPlayer2PhantomStabDebuff = false;//�f�o�t�̃^�[�����I������̂ŁA�t���O��false�ɂ���B
				nPlayer2PhantomStabDebuffCount = 0;//�f�o�t���������̂ŁA�f�o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
			}
		}

		if (bPlayer1PhantomStabDebuff == true)
		{
			nPlayer1PhantomStabDebuffCount++;//�v���C���[�P���t�@���g���X�^�u�̃f�o�t���󂯂Ă���^�[�����̃J�E���g���{�P����B

			if (nPlayer1PhantomStabDebuffCount == 7)
			{
				bPlayer1PhantomStabDebuff = false;//�f�o�t�̃^�[�����I������̂ŁA�t���O��false�ɂ���B
				nPlayer1PhantomStabDebuffCount = 0;//�f�o�t���������̂ŁA�f�o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
			}
		}

		if (g_aPlayer[PlayerNumber1].fLife <= 0 || g_aPlayer[PlayerNumber2].fLife <= 0)//�ǂ��炩�̃v���C���[�̗̑͂��O�ɂȂ�����bEndBattle��true�ɂ���B
		{
			bEndBattle = true;//�l��^�ɕς���B
		}

		//==================================================================================================================================================================

		//====================================================================
		//�X�L���u�|�C�Y���~�X�g�v�̓ŏ�Ԃ��󂯂Ă���Ƃ��̏���
		//====================================================================

		if (bPlayer2PoisonMistState == true)
		{
			nPlayer2PoisonMistStateCount++;//�v���C���[�Q���X�L���u�|�C�Y���~�X�g�v�̓ŏ�Ԃ��󂯂Ă���^�[�������J�E���g����ϐ��B

			pPlayer++;//�|�C���^����v���C���[�Q��
			pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.03);//�ŏ�Ԃ̎��A���^�[���g�o�����݂g�o��3���������Ă����B

			if (nPlayer2PoisonMistStateCount == 1)//�ŏ�ԂɂȂ�������ɏ��������s����B
			{
				pPlayer->fATK = pPlayer->fATK - (fPlayer2MaxATK * 0.2);//�ŏ�Ԃ̎��A�U���͂��Q�O����������B
			}

			if (nPlayer2PoisonMistStateCount == 7)//�v���C���[�Q���ŏ�ԂɂȂ��Ă���R�^�[����������
			{
				bPlayer2PoisonMistState = false;//�^�[�����Ȃ��Ȃ����̂ŁA�ŏ�Ԃ̃t���O��false�ɂ���B
				nPlayer2PoisonMistStateCount = 0;//�ŏ�ԂɂȂ��Ă���̃^�[�����̃J�E���g���O�ɂ���B

				pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.2);//�v���C���[�Q�̍U���͂��Q�O���㏸������B
			}
			pPlayer--;//�|�C���^����v���C���[�P��

		}

		if (bPlayer1PoisonMistState == true)
		{
			nPlayer1PoisonMistStateCount++;//�v���C���[�P���X�L���u�|�C�Y���~�X�g�v�̓ŏ�Ԃ��󂯂Ă���^�[�������J�E���g����ϐ��B

			pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.03);//�ŏ�Ԃ̎��A���^�[���g�o�����݂g�o��3���������Ă����B

			if (nPlayer1PoisonMistStateCount == 1)//�ŏ�ԂɂȂ�������ɏ��������s����B
			{
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.2);//�ŏ�Ԃ̎��A�U���͂��Q�O����������B
			}

			if (nPlayer1PoisonMistStateCount == 7)//�v���C���[�P���ŏ�ԂɂȂ��Ă���R�^�[����������
			{
				bPlayer1PoisonMistState = false;//�^�[�����Ȃ��Ȃ����̂ŁA�ŏ�Ԃ̃t���O��false�ɂ���B
				nPlayer1PoisonMistStateCount = 0;//�ŏ�ԂɂȂ��Ă���̃^�[�����̃J�E���g���O�ɂ���B

				pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.2);//�v���C���[�P�̍U���͂��Q�O���㏸������B
			}
		}
		//==============================================================================================================================================

		//==============================================================================
		//�X�L���u�X�`�[���o�[�X�g�̃f�����b�g���ʁu�Ώ��v���������Ă���Ƃ��̏���
		//==============================================================================

		if (bPlayer1SteamBurstBurn == true)//�v���C���[�P���f�����b�g���ʂ��󂯂Ă���Ƃ�
		{
			nPlayr1SteamBurstBurnCount++;//�Ώ����󂯂Ă���^�[�������J�E���g����ϐ��B

			pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.03);//�Ώ���Ԃ̎��A���^�[���g�o�����݂g�o��3���������Ă����B

			if (nPlayr1SteamBurstBurnCount == 1)//�Ώ������������u��
			{
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.2);//�Ώ���Ԃ̎��A�h��͂��Q�O����������B	
			}

			if (nPlayr1SteamBurstBurnCount == 7)//�₯�ǂ��󂯂Ă���R�^�[������������
			{
				bPlayer1SteamBurstBurn = false;//�Ώ���Ԃ��I������̂ŁA�Ώ���Ԃ̃t���O��false�ɂ���B
				nPlayr1SteamBurstBurnCount = 0;//�^�[�����̃J�E���g���O�ɂ���B

				pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.2);//�v���C���[�P�̖h��͂��{�Q�O��
			}
		}

		if (bPlayer2SteamBurstBurn == true)//�v���C���[�Q���f�����b�g���ʂ��󂯂Ă���Ƃ�
		{
			nPlayr2SteamBurstBurnCount++;//�Ώ����󂯂Ă���^�[�������J�E���g����ϐ��B

			pPlayer++;//�|�C���^����v���C���[�Q��
			pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.03);//�Ώ���Ԃ̎��A���^�[���g�o�����݂g�o��3���������Ă����B

			if (nPlayr2SteamBurstBurnCount == 1)//�Ώ������������u��
			{
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.2);//�Ώ���Ԃ̎��A�h��͂��Q�O����������B	
			}

			if (nPlayr2SteamBurstBurnCount == 7)//�₯�ǂ��󂯂Ă���R�^�[������������
			{
				bPlayer2SteamBurstBurn = false;//�Ώ���Ԃ��I������̂ŁA�Ώ���Ԃ̃t���O��false�ɂ���B
				nPlayr2SteamBurstBurnCount = 0;//�^�[�����̃J�E���g���O�ɂ���B

				pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.2);//�v���C���[�Q�̖h��͂��{�Q�O��
			}
			pPlayer--;//�|�C���^����v���C���[�P��
		}
		//==========================================================================================================================================================

		//=============================================================================
		//�X�L���u�C�J�T�}���[�U�[�v�̃f�o�t���������Ă���Ƃ��̏���
		//=============================================================================

		if (bPlayer2SquitLaserDebuff == true)//�v���C���[�Q���f�o�t���󂯂Ă���Ƃ�
		{
			nPlayer2SquitLaserDebuffCount++;//�f�o�t���󂯂Ă���^�[�������J�E���g����ϐ��B

			if (nPlayer2SquitLaserDebuffCount == 1)
			{
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.4);
				pPlayer--;//�|�C���^����v���C���[�P��
			}

			if (nPlayer2SquitLaserDebuffCount == 7)
			{
				bPlayer2SquitLaserDebuff = false;//�f�o�t�̃^�[�����I������̂ŁA�t���O��false�ɂ���B
				nPlayer2SquitLaserDebuffCount = 0;//�^�[�����̃J�E���g���O�ɂ���B
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.4);//�h��͂��{�S�O��
				pPlayer--;//�|�C���^����v���C���[�P��
			}
		}

		if (bPlayer1SquitLaserDebuff == true)//�v���C���[�P���f�o�t���󂯂Ă���Ƃ�
		{
			nPlayer1SquitLaserDebuffCount++;//�f�o�t���󂯂Ă���^�[�������J�E���g����ϐ��B

			if (nPlayer1SquitLaserDebuffCount == 1)
			{

				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.4);

			}

			if (nPlayer1SquitLaserDebuffCount == 7)
			{
				bPlayer1SquitLaserDebuff = false;//�f�o�t�̃^�[�����I������̂ŁA�t���O��false�ɂ���B
				nPlayer1SquitLaserDebuffCount = 0;//�^�[�����̃J�E���g���O�ɂ���B
				pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.4);//�h��͂��{�S�O��
			}
		}
		//=========================================================================================================================================

		//================================================
		//�J���[���C�X�̃o�t���󂯂Ă���Ƃ��̏���
		//================================================

		if (bPlayer1CurryRiceBuffLV1 == true)//�J���[���C�X�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer1CurryRiceBuffLV1Count++;//�J���[���C�X�̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer1CurryRiceBuffLV1Count == 10)
			{
				bPlayer1CurryRiceBuffLV1 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer1CurryRiceBuffLV1Count = 0;//�J���[���C�X�̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.1);//�v���C���[�P�̍U���͂����ɖ߂��B
			}

		}
		if (bPlayer2CurryRiceBuffLV1 == true)//�J���[���C�X�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer2CurryRiceBuffLV1Count++;//�J���[���C�X�̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer2CurryRiceBuffLV1Count == 10)
			{
				bPlayer2CurryRiceBuffLV1 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer2CurryRiceBuffLV1Count = 0;//�J���[���C�X�̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fATK = pPlayer->fATK - (fPlayer2MaxATK * 0.1);//�v���C���[�P�̍U���͂����ɖ߂��B
				pPlayer--;//�|�C���^����v���C���[�P��
			}

		}
		//================================================================================================================================================

		//=====================================================
		//�V�[�t�[�h�J���[�̃o�t���󂯂Ă��鎞�̏���
		//=====================================================

		if (bPlayer1CurryRiceBuffLV2 == true)//�V�[�t�[�h�J���[�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer1CurryRiceBuffLV2Count++;//�V�[�t�[�h�J���[�̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer1CurryRiceBuffLV2Count == 10)
			{
				bPlayer1CurryRiceBuffLV2 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer1CurryRiceBuffLV2Count = 0;//�V�[�t�[�h�J���[�̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.3);//�v���C���[�P�̍U���͂����ɖ߂��B
			}

		}
		if (bPlayer2CurryRiceBuffLV2 == true)//�V�[�t�[�h�J���[�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer2CurryRiceBuffLV2Count++;//�V�[�t�[�h�J���[�̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer2CurryRiceBuffLV2Count == 10)
			{
				bPlayer2CurryRiceBuffLV2 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer2CurryRiceBuffLV2Count = 0;//�V�[�t�[�h�J���[�̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fATK = pPlayer->fATK - (fPlayer2MaxATK * 0.3);//�v���C���[�Q�̍U���͂����ɖ߂��B
				pPlayer--;//�|�C���^����v���C���[�P��
			}

		}
		//================================================================================================================================================

		//=====================================================
		//�|�[�N�J���[�̃o�t���󂯂Ă��鎞�̏���
		//=====================================================

		if (bPlayer1CurryRiceBuffLV3 == true)//�|�[�N�J���[�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer1CurryRiceBuffLV3Count++;//�|�[�N�J���[�̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer1CurryRiceBuffLV3Count == 10)
			{
				bPlayer1CurryRiceBuffLV3 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer1CurryRiceBuffLV3Count = 0;//�|�[�N�J���[�̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.5);//�v���C���[�P�̍U���͂����ɖ߂��B
			}

		}
		if (bPlayer2CurryRiceBuffLV3 == true)//�|�[�N�J���[�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer2CurryRiceBuffLV3Count++;//�|�[�N�J���[�̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer2CurryRiceBuffLV3Count == 10)
			{
				bPlayer2CurryRiceBuffLV3 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer2CurryRiceBuffLV3Count = 0;//�|�[�N�J���[�̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fATK = pPlayer->fATK - (fPlayer2MaxATK * 0.5);//�v���C���[�Q�̍U���͂����ɖ߂��B
				pPlayer--;//�|�C���^����v���C���[�P��
			}

		}
		//========================================================================================================================================================

		//====================================================
		//�����ނ��т̃o�t���󂯂Ă��鎞�̏���
		//====================================================

		if (bPlayer1RiceBallBuffLV1 == true)//�v���C���[�P�������ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer1RiceBallBuffLV1Count++;//�����ނ��т̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer1RiceBallBuffLV1Count == 10)
			{
				bPlayer1RiceBallBuffLV1 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer1RiceBallBuffLV1Count = 0;//�����ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.1);//�v���C���[�P�̖h��͂����ɖ߂��B
			}
		}


		if (bPlayer2RiceBallBuffLV1 == true)//�v���C���[�Q�������ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer2RiceBallBuffLV1Count++;//�����ނ��т̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer2RiceBallBuffLV1Count == 10)
			{
				bPlayer2RiceBallBuffLV1 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer2RiceBallBuffLV1Count = 0;//�����ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.1);//�v���C���[�Q�̖h��͂����ɖ߂��B
				pPlayer--;//�|�C���^����v���C���[�P��
			}
		}
		//========================================================================================================================================================

		//====================================================
		//�����ނ��т̃o�t���󂯂Ă��鎞�̏���
		//====================================================

		if (bPlayer1RiceBallBuffLV2 == true)//�v���C���[�P�������ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer1RiceBallBuffLV2Count++;//�����ނ��т̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer1RiceBallBuffLV2Count == 10)
			{
				bPlayer1RiceBallBuffLV2 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer1RiceBallBuffLV2Count = 0;//�����ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.3);//�v���C���[�P�̖h��͂����ɖ߂��B
			}
		}

		if (bPlayer2RiceBallBuffLV2 == true)//�v���C���[�Q�������ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer2RiceBallBuffLV2Count++;//�����ނ��т̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer2RiceBallBuffLV2Count == 10)
			{
				bPlayer2RiceBallBuffLV2 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer2RiceBallBuffLV2Count = 0;//�����ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.3);//�v���C���[�Q�̖h��͂����ɖ߂��B
				pPlayer--;//�|�C���^����v���C���[�P��
			}
		}

		if (g_aPlayer[PlayerNumber1].fLife <= 0 || g_aPlayer[PlayerNumber2].fLife <= 0)//�ǂ��炩�̃v���C���[�̗̑͂��O�ɂȂ�����bEndBattle��true�ɂ���B
		{
			bEndBattle = true;//�l��^�ɕς���B
		}
		//========================================================================================================================================================

		//====================================================
		//�����炨�ނ��т̃o�t���󂯂Ă��鎞�̏���
		//====================================================

		if (bPlayer1RiceBallBuffLV3 == true)//�v���C���[�P�������炨�ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer1RiceBallBuffLV3Count++;//�����炨�ނ��т̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer1RiceBallBuffLV3Count == 10)
			{
				bPlayer1RiceBallBuffLV3 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer1RiceBallBuffLV3Count = 0;//�����炨�ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer1MaxDEF * 0.5);//�v���C���[�P�̖h��͂����ɖ߂��B
			}
		}

		if (bPlayer2RiceBallBuffLV3 == true)//�v���C���[�Q�������炨�ނ��т̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer2RiceBallBuffLV3Count++;//�����炨�ނ��т̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer2RiceBallBuffLV3Count == 10)
			{
				bPlayer2RiceBallBuffLV3 = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer2RiceBallBuffLV3Count = 0;//�����炨�ނ��т̃o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fDEF = pPlayer->fDEF - (fPlayer2MaxDEF * 0.5);//�v���C���[�Q�̖h��͂����ɖ߂��B
				pPlayer--;//�|�C���^����v���C���[�P��
			}
		}
		//==========================================================================================================================================

		//=====================================================================
		//�X�L���u�t���C���E�F�[�u�v�̃o�t���������Ă��鎞�̏���
		//=====================================================================

		if (bPlayer1FlameWaveBuff == true)//�v���C���[�P���X�L���u�t���C���E�F�[�u�v�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer1FlameWaveBuffCount++;//�t���C���E�F�[�u�̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer1FlameWaveBuffCount == 1)
			{
				pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.25);//�U���͂��{�Q�T��
			}

			if (nPlayer1FlameWaveBuffCount == 6)//�o�t���󂯂Ă���Q�^�[����������
			{
				bPlayer1FlameWaveBuff = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer1FlameWaveBuffCount = 0;//�o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.25);//�U���͂��[�Q�T��
			}
		}

		if (bPlayer2FlameWaveBuff == true)//�v���C���[�Q���X�L���u�t���C���E�F�[�u�v�̃o�t���󂯂Ă��邱�Ƃ�\���t���O��true�ɂȂ����Ƃ�
		{
			nPlayer2FlameWaveBuffCount++;//�t���C���E�F�[�u�̃o�t���󂯂Ă���̃^�[�������J�E���g����B

			if (nPlayer2FlameWaveBuffCount == 1)
			{
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.25);//�U���͂��{�Q�T��
				pPlayer--;//�|�C���^����v���C���[�P��
			}

			if (nPlayer2FlameWaveBuffCount == 6)//�o�t���󂯂Ă���Q�^�[����������
			{
				bPlayer2FlameWaveBuff = false;//�o�t�̃^�[�����I������̂Ńt���O��false�ɂ���B
				nPlayer2FlameWaveBuffCount = 0;//�o�t���󂯂Ă���^�[�����̃J�E���g���O�ɂ���B
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fATK = pPlayer->fATK - (fPlayer1MaxATK * 0.25);//�U���͂��[�Q�T��
				pPlayer--;//�|�C���^����v���C���[�P��
			}
		}
		//========================================================================================================================================================

		//===============================================================================
		//�X�L���u�A�C�X�����X�v�̓�����ʁu�A�C�e������v���������Ă���Ƃ��̏���
		//===============================================================================

		if (bPlayer2IceLanceDebuff == true)
		{
			nPlayer2IceLanceDebuffCount++;//�v���C���[�Q���X�L���u�A�C�X�����X�v�̃A�C�e��������󂯂Ă���^�[�������J�E���g����B

			if (nPlayer2IceLanceDebuffCount == 11)//������ʂ��󂯂Ă���R�^�[�����o������
			{
				bPlayer2IceLanceDebuff = false;//������ʂ��󂯂�^�[�����I������̂Ńt���O��false�ɂ���B
			}
		}

		if (bPlayer1IceLanceDebuff == true)
		{
			nPlayer1IceLanceDebuffCount++;//�v���C���[�P���X�L���u�A�C�X�����X�v�̃A�C�e��������󂯂Ă���^�[�������J�E���g����B

			if (nPlayer1IceLanceDebuffCount == 11)//������ʂ��󂯂Ă���R�^�[�����o������
			{
				bPlayer1IceLanceDebuff = false;//������ʂ��󂯂�^�[�����I������̂Ńt���O��false�ɂ���B
			}
		}
		//==============================================================================================================================================

		//====================================================
		//�A�C�e���F����ʂ̃f�o�t���󂯂Ă���Ƃ��̏���
		//====================================================

		if (bPlayer2WeakBallDeBuff == true)
		{
			nPlayer2WeakBallDeBuffCount++;//�v���C���[�Q���A�C�e���u����ʁv�̃f�o�t���󂯂Ă���Ƃ��̏���

			if (nPlayer2WeakBallDeBuffCount == 7)//�f�o�t���󂯂Ă���R�^�[�����o������
			{
				bPlayer2WeakBallDeBuff = false;//�f�o�t���󂯂�^�[�����I������̂Ńt���O��false�ɂ���B
				pPlayer++;//�|�C���^����v���C���[�Q��
				pPlayer->fATK = pPlayer->fATK + (fPlayer2MaxATK * 0.4);//�U���͂����ɖ߂��B
				pPlayer->fDEF = pPlayer->fDEF + (fPlayer2MaxDEF * 0.4);//�h��͂����ɖ߂��B
				pPlayer--;//�|�C���^����v���C���[�P��
			}
		}
		if (bPlayer1WeakBallDeBuff == true)
		{
			nPlayer1WeakBallDeBuffCount++;//�v���C���[�P���A�C�e���u����ʁv�̃f�o�t���󂯂Ă���Ƃ��̏���

			if (nPlayer1WeakBallDeBuffCount == 7)//�f�o�t���󂯂Ă���R�^�[�����o������
			{
				bPlayer1WeakBallDeBuff = false;//�f�o�t���󂯂�^�[�����I������̂Ńt���O��false�ɂ���B
				pPlayer->fATK = pPlayer->fATK + (fPlayer1MaxATK * 0.4);//�U���͂����ɖ߂��B
				pPlayer->fDEF = pPlayer->fDEF + (fPlayer1MaxDEF * 0.4);//�h��͂����ɖ߂��B
			}
		}
		//==================================================================================================================================

		//==========================================
		//�^�[���̏���
		//==========================================
		
		/*nHalfTurn++;
		if (nHalfTurn % 2 == 0)
		{
			nTurn++;
		}*/
		nTurn++;
		if (nTurn != 0)
		{
			if (nTurn % 20 == 0)//�Q�O�Ŋ����ė]�肪�O�̂Ƃ�
			{
				nPlayer1ItemRouletteUseCount++;
				nPlayer2ItemRouletteUseCount++;
				printf("�A�C�e������[���ꂽ�I\n");
				rewind(stdin);
				getchar();
				system("cls");
			}
		}

		if (g_aPlayer[PlayerNumber1].fLife <= 0 || g_aPlayer[PlayerNumber2].fLife <= 0)//�ǂ��炩�̃v���C���[�̗̑͂��O�ɂȂ�����bEndBattle��true�ɂ���B
		{
			bEndBattle = true;//�l��^�ɕς���B
		}

	} while (bEndBattle == false);


	if (g_aPlayer[PlayerNumber1].fLife >= 1)//�v���C���[�P�̗̑͂��P�ȏ�Ȃ珈�������s����B
	{
		printf("%s�̏����I\n\n", &g_aPlayer[PlayerNumber1].aName[0]);
	}

	else
	{

		printf("%s�̏���!\n\n", &g_aPlayer[PlayerNumber2].aName[0]);
	}


}




//===============================================================================
//�v���C���[�P�̒ʏ�U��
//===============================================================================
void  Player1NormalAttack(Player* pPlayer, bool Player2CounterFlag, bool bPlayer1CriticalLineBuff)//�v���C���[�P���ʏ�U��������Ƃ��ɌĂяo���֐��B�v���C���[�Q���J�E���^�[��ԂɂȂ����Ƃ��ɂS�O���̊m���ŃJ�E���^�[�𔭓�������B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B
	//int nCount;
	//int nTurnAttack;//�U���^�[���̕ϐ�
	float fDamageResult;//�_���[�W�̌��ʂ�������ϐ�
	float fRandum1;
	int CheckRandum;
	int nCounterOn;//�J�E���^�[���m���Ŕ���������ϐ��B]
	int CriticalLineBuffRandum;//�X�L���u��S�̐w�v���������ɔ������郉���_���֐��B


	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 20 + 1, CriticalLineBuffRandum = rand() % 21 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B

	nCounterOn = rand() % 2 + 1;//�Q���̈�̊m���ŃJ�E���^�[�𔭓�������B
	   //if (nTurnAttack == 0)	   //{
	printf("%s�̍U���I\n\n", &g_aPlayer[PlayerNumber1].aName[0]);

	getchar();


	if (bPlayer1CriticalLineBuff == false)//��S�̐w�̃o�t���������Ă��Ȃ��Ƃ��Ɉ��������B
	{
		//�_���[�W�v�Z
		if (CheckRandum >= 1 && CheckRandum <= 9)//�����{�����{�̕�
		{
			//pPlayer++;//�|�C���^����v���C���[�Q��
			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�P�̍U����-�v���C���[2�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j

			fDamageResult = (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));////�v���C���[�P�̍U����-�v���C���[2�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j


			//pPlayer->fLife=g_aPlayer[1].fLife - fDamageResult;//�v���C���[2�̗̑͂����B

			printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);

			if (Player2CounterFlag == true && nCounterOn == 1)//�v���C���[�Q�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�Q���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�v���C���[�P�ɃJ�E���^�[�̃_���[�W��^����B
			}


		}

		else if (CheckRandum >= 10 && CheckRandum <= 18)//�����{����-�̕�
		{

			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�P�̍U����-�v���C���[�Q�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j
			fDamageResult = (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�P�̍U����-�v���C���[�Q�̖h���/2���i0.75�`0.99�j
			//pPlayer++;//�|�C���^����v���C���[�Q��

			//pPlayer->fLife - fDamageResult;//�v���C���[�Q�̗̑͂����B

			printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W���ʂ̕\��

			if (Player2CounterFlag == true && nCounterOn == 1)//�v���C���[�Q�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�Q���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);
			}

		}
		else if (CheckRandum == 19)//�N���e�B�J��
		{
			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK) * 2 * (1 + (fRandum1 / 100));//�v���C���[1�̍U����*2���i�P�D�O�P�`�P�D�Q�T�j

			fDamageResult = (g_aPlayer[PlayerNumber1].fATK) * 2 * (1 + (fRandum1 / 100));//�v���C���[�P�̍U����*2���i�P�D�O�P�`�P�D�Q�T�j

			printf("\x1b[33m�N���e�B�J�����I\x1b[0m\n");

			printf("%s��%.f�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);

			if (Player2CounterFlag == true && nCounterOn == 1)//�v���C���[�Q�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�Q���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�v���C���[�P�ɃJ�E���^�[�̃_���[�W��^����B
			}
		}

		else if (CheckRandum == 20)//���
		{
			printf("%s�͍U������������I\n", &g_aPlayer[PlayerNumber2].aName[0]);
		}
	}
	else if (bPlayer1CriticalLineBuff == true)//�u��S�̐w�v�̃o�t���������Ă���Ƃ��Ɉ�������
	{
		if (CriticalLineBuffRandum >= 1 && CriticalLineBuffRandum <= 6)//�����{�����{�̕�
		{
			//pPlayer++;//�|�C���^����v���C���[�Q��
			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�P�̍U����-�v���C���[2�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j

			fDamageResult = (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));////�v���C���[�P�̍U����-�v���C���[2�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j


			//pPlayer->fLife=g_aPlayer[1].fLife - fDamageResult;//�v���C���[2�̗̑͂����B

			printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);

			if (Player2CounterFlag == true && nCounterOn == 1)//�v���C���[�Q�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�Q���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�v���C���[�P�ɃJ�E���^�[�̃_���[�W��^����B
			}


		}

		else if (CriticalLineBuffRandum >= 7 && CriticalLineBuffRandum <= 12)//�����{����-�̕�
		{

			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�P�̍U����-�v���C���[�Q�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j
			fDamageResult = (g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�P�̍U����-�v���C���[�Q�̖h���/2���i0.75�`0.99�j
			//pPlayer++;//�|�C���^����v���C���[�Q��

			//pPlayer->fLife - fDamageResult;//�v���C���[�Q�̗̑͂����B

			printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W���ʂ̕\��

			if (Player2CounterFlag == true && nCounterOn == 1)//�v���C���[�Q�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�Q���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);
			}

		}
		else if (CriticalLineBuffRandum >= 13 && CriticalLineBuffRandum <= 20)//�N���e�B�J��
		{
			g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK) * 2 * (1 + (fRandum1 / 100));//�v���C���[1�̍U����*2���i�P�D�O�P�`�P�D�Q�T�j

			fDamageResult = (g_aPlayer[PlayerNumber1].fATK) * 2 * (1 + (fRandum1 / 100));//�v���C���[�P�̍U����*2���i�P�D�O�P�`�P�D�Q�T�j

			printf("\x1b[33m�N���e�B�J�����I\x1b[0m\n");

			printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);

			if (Player2CounterFlag == true && nCounterOn == 1)//�v���C���[�Q�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�Q���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber2].aName[0]);
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�v���C���[�P�ɃJ�E���^�[�̃_���[�W��^����B
			}
		}

		else if (CriticalLineBuffRandum == 21)//���
		{
			printf("%s�͍U����\x1b[36m���\x1b[0m�����I\n", &g_aPlayer[PlayerNumber2].aName[0]);
		}
	}

}


//nTurnAttack = 1;//���̃^�[���ɉ񂷂��߂ɕϐ��̒l��+1����B

	//else

//===============================================================
//�v���C���[�Q�̒ʏ�U��
//===============================================================

void Player2NormalAttack(Player* pPlayer, bool Player1CounterFlag, bool bPlayer2CriticalLineBuff)//�v���C���[�Q���ʏ�U��������Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	//int nCount;

	//int nTurnAttack;//�U���^�[���̕ϐ�

	float fDamageResult;//�_���[�W�̌��ʂ�������ϐ�

	float fRandum1;

	int CheckRandum;

	int nCounterOn;

	int nCriticalLineBuffRandum;//�u��S�̐w�̃o�t���������Ă���Ƃ��ɔ�������ϐ��B


	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 20 + 1, nCriticalLineBuffRandum = rand() % 21 + 1;//�����l�i�Q�T���j��錾)�B�����n�i�P���Q�j��錾

	nCounterOn = rand() % 2 + 1;//�Q���̈�̊m���ŃJ�E���^�[�𔭓�������B

	printf("%s�̍U���I\n\n", &g_aPlayer[PlayerNumber2].aName[0]);//�v���C���[�Q�̍U��

	getchar();


	//�_���[�W�v�Z
	if (bPlayer2CriticalLineBuff == false)//��S�̐w�̃o�t���������Ă��Ȃ��Ƃ��ɏ���������ϐ��B
	{
		if (CheckRandum >= 1 && CheckRandum <= 9)//�����{�����{�̕�
		{
			g_aPlayer[0].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�Q�̍U����-�v���C���[�P�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�Q�̍U����-�v���C���[�P�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j

			//pPlayer;//�|�C���^����v���C���[�P��


			//pPlayer->fLife - fDamageResult;//�v���C���[�P�̗̑͂����B

			printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);

			if (Player1CounterFlag == true && nCounterOn == 1)//�v���C���[�P�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�P���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�v���C���[�Q�ɃJ�E���^�[�̃_���[�W��^����B
			}

		}

		else if (CheckRandum >= 10 && CheckRandum <= 18)//�����{����-�̕�
		{
			g_aPlayer[0].fLife = g_aPlayer[0].fLife - (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�Q�̍U����-�v���C���[�P�̖h���/2���i�O�D�V�T�`�O�D�X�X�j

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�Q�̍U����-�v���C���[�P�̖h���/2���i�O�D�V�T�`�O�D�X�X�j
			//pPlayer;//�|�C���^����v���C���[��

			//pPlayer->fLife - fDamageResult;//�v���C���[�P�̗̑͂����B


			printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W���ʂ̕\��

			if (Player1CounterFlag == true && nCounterOn == 1)//�v���C���[�P�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�P���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�v���C���[�Q�ɃJ�E���^�[�̃_���[�W��^����B
			}
		}
		else if (CheckRandum == 19)//�N���e�B�J��
		{
			g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK) * 2 * (1 + (fRandum1 / 100));//�v���C���[1�̍U����*2���i�P�D�O�P�`�P�D�Q�T�j

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK) * 2 * (1 + (fRandum1 / 100));//�v���C���[�P�̍U����*2���i�P�D�O�P�`�P�D�Q�T�j

			printf("\x1b[33m�N���e�B�J��\x1b[0m���I\n");

			printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);

			if (Player1CounterFlag == true && nCounterOn == 1)//�v���C���[�Q�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�Q���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�v���C���[�Q�ɃJ�E���^�[�̃_���[�W��^����B
			}
		}
		else if (CheckRandum == 20)//���
		{
			printf("%s�͍U����\x1b[36m���\x1b[0m�����I\n", &g_aPlayer[PlayerNumber1].aName[0]);
		}
	}
	else if (bPlayer2CriticalLineBuff == true)//��S�̐w�̃o�t���������Ă���Ƃ��ɏ���������ϐ��B
	{
		if (nCriticalLineBuffRandum >= 1 && nCriticalLineBuffRandum <= 6)//�����{�����{�̕�
		{
			g_aPlayer[0].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�Q�̍U����-�v���C���[�P�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�Q�̍U����-�v���C���[�P�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j

			//pPlayer;//�|�C���^����v���C���[�P��


			//pPlayer->fLife - fDamageResult;//�v���C���[�P�̗̑͂����B

			printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);

			if (Player1CounterFlag == true && nCounterOn == 1)//�v���C���[�P�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�P���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�v���C���[�Q�ɃJ�E���^�[�̃_���[�W��^����B
			}

		}

		else if (nCriticalLineBuffRandum >= 7 && nCriticalLineBuffRandum <= 12)//�����{����-�̕�
		{
			g_aPlayer[0].fLife = g_aPlayer[0].fLife - (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�Q�̍U����-�v���C���[�P�̖h���/2���i�O�D�V�T�`�O�D�X�X�j

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�Q�̍U����-�v���C���[�P�̖h���/2���i�O�D�V�T�`�O�D�X�X�j
			//pPlayer;//�|�C���^����v���C���[��

			//pPlayer->fLife - fDamageResult;//�v���C���[�P�̗̑͂����B


			printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W���ʂ̕\��

			if (Player1CounterFlag == true && nCounterOn == 1)//�v���C���[�P�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�P���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�v���C���[�Q�ɃJ�E���^�[�̃_���[�W��^����B
			}
		}
		else if (nCriticalLineBuffRandum >= 13 && nCriticalLineBuffRandum <= 20)//�N���e�B�J��
		{
			g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK) * 2 * (1 + (fRandum1 / 100));//�v���C���[1�̍U����*2���i�P�D�O�P�`�P�D�Q�T�j

			fDamageResult = (g_aPlayer[PlayerNumber2].fATK) * 2 * (1 + (fRandum1 / 100));//�v���C���[�P�̍U����*2���i�P�D�O�P�`�P�D�Q�T�j

			printf("\x1b[33m�N���e�B�J��\x1b[0m���I\n");

			printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);

			if (Player1CounterFlag == true && nCounterOn == 1)//�v���C���[�Q�̃J�E���^�[�t���O��true�ł��邩�A�T�O�����������Ă��ꍇ�A�v���C���[�Q���J�E���^�[�𔭓��B
			{
				printf("%s���J�E���^�[�𔭓��I\n", &g_aPlayer[PlayerNumber1].aName[0]);
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);//
				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK - (g_aPlayer[PlayerNumber2].fDEF / 2)) * 2.2);
				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�v���C���[�Q�ɃJ�E���^�[�̃_���[�W��^����B
			}
		}
		else if (nCriticalLineBuffRandum == 21)//���
		{
			printf("%s�͍U����\x1b[36m���\x1b[0m�����I\n", &g_aPlayer[PlayerNumber1].aName[0]);
		}
	}
	//nTurnAttack = 0;
}

//if (g_aPlayer[PlayerNumber1].fLife <= 0 || g_aPlayer[PlayerNumber2].fLife <= 0)//�ǂ��炩�̃v���C���[�̗̑͂��O�ɂȂ�����bEndBattle��true�ɂ���B
//{
//	bEndBattle = true;//�l��^�ɕς���B
//}



//pPlayer;//�擪�A�h���X����(�z��[0]�ցj
//} while (bEndBattle == false);


//if (g_aPlayer[PlayerNumber1].fLife >= 1)//�v���C���[�P�̗̑͂��P�ȏ�Ȃ珈�������s����B
//{
//	printf("%s�̏����I\n\n", &g_aPlayer[PlayerNumber1].aName[0]);
//}

//else
//{

//	printf("%s�̏���!\n\n", &g_aPlayer[PlayerNumber2].aName[0]);
//}


//=====================================================
//�v���C���[�P�̃X�L���F�C���p�N�g�G�b�W
//=====================================================
void Player1InpactEdge(Player* pPlayer)//�v���C���[�P�̃X�L���F�C���p�N�g�G�b�W
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 30;//�X�L���������Ƀv���C���[�P��MP��30���炷�B

	printf("%s�̃C���p�N�g�G�b�W�I\n", &g_aPlayer[0].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 3 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*2.5-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 3 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*2.5-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 3 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.5-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 3 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.5-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}

	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L



}

//=====================================================================
//�v���C���[�Q�̃X�L���F�C���p�N�g�G�b�W
//=====================================================================
void Player2InpactEdge(Player* pPlayer)//�v���C���[�Q�����m�̃X�L���i�P�j�A�u�C���p�N�g�G�b�W�v����������Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer++;//�C���N�������g���Ċ�_���P���炷�B
	pPlayer->fMP = pPlayer->fMP - 30;//�X�L���������ɂl�o���R�O���炷�B

	printf("%s�̃C���p�N�g�G�b�W�I\n", &g_aPlayer[1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[0].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 3 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*2.5-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 3 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*2.5-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[0].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 3 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.5-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 3 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.5-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}

	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L
}



//======================================================
//�v���C���[�P�̃X�L���F���J�o�[�q�[��
//======================================================
void Player1RecoverHeal(Player* pPlayer, float fPlayer1MaxHp)//�v���C���[�P�����m�̃X�L���i�Q�j�A�u���J�o�[�q�[���v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	float fRandum1;
	int CheckRandum;
	//float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 80;//�X�L���������Ƀv���C���[�P�̂l�o���T�O���炷�B

	printf("%s�̃��J�o�[�q�[���I\n", &g_aPlayer[0].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B

	pPlayer->fLife = pPlayer->fLife + (fPlayer1MaxHp * 0.25);//�v���C���[�P�̗̑͂����������g�o�̂Q�T���񕜂���B


	printf("%s�̍ő�̗͂�\x1b[36m�Q�T����\x1b[0m�̗͂��񕜂���I", &pPlayer->aName[0]);
}


//===========================================================
//�v���C���[�Q�̃X�L���F���J�o�[�q�[��
//===========================================================
void Player2RecoverHeal(Player* pPlayer, float fPlayer2MaxHp)//�v���C���[�Q�����m�̃X�L���i�Q�j�A�u���J�o�[�q�[���v�𔭓����鎞�ɌĂяo���֐��B
{
	float fRandum1;
	int CheckRandum;
	//float fDamageResult;

	pPlayer++;//�C���N�������g���Ċ�_���Â炷�B

	pPlayer->fMP = pPlayer->fMP - 80;//�X�L���������Ƀv���C���[�P�̂l�o���T�O���炷�B

	printf("%s�̃��J�o�[�q�[���I\n", &g_aPlayer[0].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B

	pPlayer->fLife = pPlayer->fLife + (fPlayer2MaxHp * 0.25);//�v���C���[�P�̗̑͂����������g�o�̂Q�T���񕜂���B


	printf("%s�̍ő�̗͂�\x1b[36m�Q�T����\x1b[0m�̗͂��񕜂���I", &pPlayer->aName[0]);
}

//===========================================================
//�v���C���[�P�̃X�L���F���C���A���[
//===========================================================
void Player1RainArrow(Player* pPlayer)//�v���C���[�P����l�̃X�L���i�P�j�A�u���C���A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;

	pPlayer->fMP = pPlayer->fMP - 25;//�v���C���[�P�̂l�o���Q�T���炷�B

	printf("%s�̃��C���A���[�I\n", &pPlayer->aName[0]);

	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 8; nCount++, nAttackSucces = rand() % 10 + 1, fRandum1 = rand() % 25 + 1)//�W�A���U�����s���B�i�������S�O���j
		{
			if (nAttackSucces >= 1 && nAttackSucces <= 4)//�U�������m���̃����_���֐����P�`�S��������U���𐬌�������B
			{
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK * 0.75) - (g_aPlayer[PlayerNumber2].fDEF / 4)) * (1 + (fRandum1 / 100));//�v���C���[�P�̍U���́��R-�v���C���[�Q�̖h���/4���i�P�D�O�P�`�P�D�Q�T�j

				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK * 0.75) - (g_aPlayer[PlayerNumber2].fDEF / 4)) * (1 + (fRandum1 / 100));//�v���C���[�P�̍U���́��R-�v���C���[�Q�̖h���/4���i�P�D�O�P�`�P�D�Q�T�j


				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L
				getchar();
			}

			else
			{
				printf("\x1b[34m�U�����O�ꂽ�I\x1b[0m\n");//�U�O���̊m���ōU�����O������B
				getchar();
			}
		}

	}
	else if (CheckRandum == 2)
	{
		for (nCount = 0; nCount < 8; nCount++, nAttackSucces = rand() % 10 + 1, fRandum1 = rand() % 25 + 1)//�W�A���U�����s���B�i�������S�O���j
		{
			if (nAttackSucces >= 1 && nAttackSucces <= 4)//�U�������m���̃����_���֐����P�`�S��������U���𐬌�������B
			{
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK * 0.75) - (g_aPlayer[PlayerNumber2].fDEF / 4)) * (1 - (fRandum1 / 100));//�v���C���[�P�̍U���́��R-�v���C���[�Q�̖h���/4���i�P�D�O�P�`�P�D�Q�T�j

				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK * 0.75) - (g_aPlayer[PlayerNumber2].fDEF / 4)) * (1 - (fRandum1 / 100));//�v���C���[�P�̍U���́��R-�v���C���[�Q�̖h���/4���i�P�D�O�P�`�P�D�Q�T�j


				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L
				getchar();
			}

			else
			{
				printf("\x1b[34m�U�����O�ꂽ�I\x1b[0m\n");//�U�O���̊m���ōU�����O������B
				getchar();
			}
		}

	}





}

//========================================================
//�v���C���[�Q�̃X�L���F���C���A���[
//========================================================
void Player2RainArrow(Player* pPlayer)//�v���C���[�Q����l�̃X�L���i�P�j�A�u���C���A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;

	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 25;//�v���C���[�P�̂l�o���Q�T���炷�B

	printf("%s�̃��C���A���[�I\n", &pPlayer->aName[0]);

	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 8; nCount++, nAttackSucces = rand() % 10 + 1, fRandum1 = rand() % 25 + 1)//�W�A���U�����s���B�i�������S�O���j
		{
			if (nAttackSucces >= 1 && nAttackSucces <= 4)//�U�������m���̃����_���֐����P�`�S��������U���𐬌�������B
			{
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK * 0.75) - (g_aPlayer[PlayerNumber1].fDEF / 4)) * (1 + (fRandum1 / 100));//�v���C���[�Q�̍U���́��R-�v���C���[�Q�̖h���/4���i�P�D�O�P�`�P�D�Q�T�j

				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK * 0.75) - (g_aPlayer[PlayerNumber1].fDEF / 4)) * (1 + (fRandum1 / 100));//�v���C���[�P�̍U���́��R-�v���C���[�Q�̖h���/4���i�P�D�O�P�`�P�D�Q�T�j


				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L
				getchar();
			}

			else
			{
				printf("\x1b[34m�U�����O�ꂽ�I\x1b[0m\n");//�U�O���̊m���ōU�����O������B
				getchar();
			}
		}

	}
	else if (CheckRandum == 2)
	{
		for (nCount = 0; nCount < 8; nCount++, nAttackSucces = rand() % 10 + 1, fRandum1 = rand() % 25 + 1)//�W�A���U�����s���B�i�������S�O���j
		{
			if (nAttackSucces >= 1 && nAttackSucces <= 4)//�U�������m���̃����_���֐����P�`�S��������U���𐬌�������B
			{
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK * 0.75) - (g_aPlayer[PlayerNumber1].fDEF / 4)) * (1 - (fRandum1 / 100));//�v���C���[2�̍U���́��R-�v���C���[�Q�̖h���/4���i�P�D�O�P�`�P�D�Q�T�j

				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK * 0.75) - (g_aPlayer[PlayerNumber1].fDEF / 4)) * (1 - (fRandum1 / 100));//�v���C���[�P�̍U���́��R-�v���C���[�Q�̖h���/4���i�P�D�O�P�`�P�D�Q�T�j


				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L
				getchar();
			}

			else
			{
				printf("\x1b[34m�U�����O�ꂽ�I\x1b[0m\n");//�U�O���̊m���ōU�����O������B
				getchar();
			}
		}

	}

}
//=====================================================================
//�v���C���[�P�̃X�L���F�p���_�E���A���[
//=====================================================================

bool Player1PowerDownArrow(Player* pPlayer)//�v���C���[�P����l�̃X�L���i�Q�j�A�u�p���_�E���A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	//int nCount;

	//int nTurnAttack;//�U���^�[���̕ϐ�

	float fDamageResult;//�_���[�W�̌��ʂ�������ϐ�

	float fRandum1;

	int CheckRandum;

	int PowerDownSuccess;//�U���͒ቺ���������邩�𔻒肷��ϐ��B

	bool Player2PowerDownArrowDebuff;//�v���C���[�P���p���_�E���A���[�̃f�o�t�i�U���͂R�O���ቺ�j�̃t���O��錾����B

	Player2PowerDownArrowDebuff = false;

	pPlayer->fMP = pPlayer->fMP - 40;//�X�L���������Ƀv���C���[�P��MP��40���炷�B

	printf("%s�̃p���_�E���A���[�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*2-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*2-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[1].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.5-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.5-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}

	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L

	PowerDownSuccess = rand() % 5 + 1;//�U���͒ቺ�̐����𔻒肷��B

	if (PowerDownSuccess >= 1 && PowerDownSuccess <= 2)
	{
		pPlayer++;//�|�C���^����v���C���[�Q��
		pPlayer->fATK = pPlayer->fATK - (pPlayer->fATK * 0.5);//�U���͒ቺ�̃f�o�t�����������ꍇ�ɁA�v���C���[�Q�̍U���͂��R�O���ቺ������B
		pPlayer--;//�|�C���^����v���C���[�P��

		printf("%s��\x1b[36m�U���͂��ቺ�����I\x1b[0m�i�T�O���j�i�Q�s�j\n", &pPlayer->aName);


		Player2PowerDownArrowDebuff = true;//�f�o�t���������Ă��邱�Ƃ�\���t���O��true�ɂ���B
	}

	return Player2PowerDownArrowDebuff;//�v���C���[�Q�փp���_�E���A���[�̃f�o�t���������Ă��邩�ǂ�����Ԃ��B
}

//=================================================================
//�v���C���[�Q�̃X�L���F�p���_�E���A���[
//=================================================================
bool Player2PowerDownArrow(Player* pPlayer)//�v���C���[2����l�̃X�L���i�Q�j�A�u�p���_�E���A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	int nCount;

	int nTurnAttack;//�U���^�[���̕ϐ�

	float fDamageResult;//�_���[�W�̌��ʂ�������ϐ�

	float fRandom1;

	int CheckRandum;

	int PowerDownSuccess;//�U���͒ቺ���������邩�𔻒肷��ϐ��B

	bool Player1PowerDownArrowDebuff;//�v���C���[�Q���v���C���[�P�փp���_�E���A���[�̃f�o�t�i�U���͂T�O���ቺ�j�̃t���O��錾����B

	Player1PowerDownArrowDebuff = false;

	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 40;//�X�L���������Ƀv���C���[�P��MP��40���炷�B
	pPlayer--;//�|�C���^����v���C���[�P��

	printf("%s�̃p���_�E���A���[�I\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandom1 = rand() % 25 + 1;
	CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B

	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandom1 / 100));//(�v���C���[2�̍U����*2-�v���C���[1�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandom1 / 100));//(�v���C���[2�̍U����*2-�v���C���[1�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandom1 / 100));//(�v���C���[2�̍U����*2-�v���C���[1�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandom1 / 100));//(�v���C���[2�̍U����*2-�v���C���[1�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}

	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L

	PowerDownSuccess = rand() % 5 + 1;//�U���͒ቺ�̐����𔻒肷��B

	if (PowerDownSuccess >= 1 && PowerDownSuccess <= 2)
	{
		pPlayer->fATK = pPlayer->fATK - (pPlayer->fATK * 0.5);//�U���͒ቺ�̃f�o�t�����������ꍇ�ɁA�v���C���[1�̍U���͂��T�O���ቺ������B

		printf("%s��\x1b[36m�U���͂��ቺ�����I\x1b[0m�i�T�O���j�i�Q�s�j\n", &pPlayer->aName);


		Player1PowerDownArrowDebuff = true;//�f�o�t���������Ă��邱�Ƃ�\���t���O��true�ɂ���B
	}

	return Player1PowerDownArrowDebuff;//�v���C���[1�փp���_�E���A���[�̃f�o�t���������Ă��邩�ǂ�����Ԃ��B
}

//======================================================
//�v���C���[�P�̃X�L���F�X�^�[�A���[
//======================================================
void Player1StarArrow(Player* pPlayer)//�v���C���[�P����l�̃X�L���i�R�j�A�u�X�^�[�A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 60;//�X�L���������Ƀv���C���[�P��MP��60���炷�B

	printf("%s�̃X�^�[�A���[�I\n", &g_aPlayer[0].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 5) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*4.5���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 5) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*5���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 5) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*4.5���i�O�D�V�T�`�O�D�X�X�j

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 5) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*4.5���i�P�D�O�P�`�P�D�Q�T�j
	}

	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L
}

//============================================================================
//�v���C���[�Q�̃X�L���u�X�^�[�A���[�v
//============================================================================
void Player2StarArrow(Player* pPlayer)//�v���C���[�Q����l�̃X�L���i�R�j�A�u�X�^�[�A���[�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 60;//�X�L���������Ƀv���C���[�P��MP��60���炷�B
	pPlayer--;//�|�C���^����v���C���[�P��

	printf("%s�̃X�^�[�A���[�I\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 5) * (1 + (fRandum1 / 100));//(�v���C���[2�̍U����*5���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 5) * (1 + (fRandum1 / 100));//(�v���C���[2�̍U����*5���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 5) * (1 - (fRandum1 / 100));//(�v���C���[2�̍U����*5���i�O�D�V�T�`�O�D�X�X�j

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 5) * (1 - (fRandum1 / 100));//(�v���C���[2�̍U����*5���i�O�D�V�T�`�O�D�X�X�j
	}

	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L
}

//============================================================================
//�v���C���[�P�̃X�L���u���̐w�v
//============================================================================
void Player1DefenceLine(Player* pPlayer)//�v���C���[�P���ޏ��̃X�L���i�P�j�A�u���̐w�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 35;//�X�L���������Ƀv���C���[�P��MP��60���炷�B


	printf("%s�̎��̐w�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	pPlayer->fDEF = pPlayer->fDEF + (pPlayer->fDEF * 0.4);//�h��͂��{�S�O��

	printf("%s��\x1b[33m�h��͂��S�O���㏸�����I\x1b[0m\n", &pPlayer->aName[0]);//�o�t���󂯂����Ƃ̕\�L
}

//=================================================================================
//�v���C���[�Q�̃X�L���u���̐w�v
//=================================================================================
void Player2DefenceLine(Player* pPlayer)//�v���C���[�Q���ޏ��̃X�L���i�P�j�A�u���̐w�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 35;//�X�L���������Ƀv���C���[�Q��MP��60���炷�B


	printf("%s�̎��̐w�I\n", &pPlayer->aName[0]);

	pPlayer->fDEF = pPlayer->fDEF + (pPlayer->fDEF * 0.4);//�h��͂��{�S�O��

	printf("%s��\x1b[33m�h��͂��S�O���㏸�����I\x1b[0m\n", &pPlayer->aName[0]);//�o�t���󂯂����Ƃ̕\�L
	pPlayer--;//�|�C���^����v���C���[�P��
}

//==============================================================================
//�v���C���[�P�̃X�L���u�z�[���[���C�v
//==============================================================================
void Player1HolyRay(Player* pPlayer)//�v���C���[�P���ޏ��̃X�L���i�Q�j�A�u�z�[���[���C�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 45;//�X�L���������Ƀv���C���[�P��MP���T�O���炷�B

	printf("%s�̃z�[���[���C�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber1].fDEF) * 1.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//((�v���C���[�P�̍U���́{�v���C���[�P�̖h���-�v���C���[2�̖h���/2))���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber1].fDEF) * 1.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//((�v���C���[�P�̍U���́{�v���C���[�P�̖h���)*1.2-�v���C���[2�̖h���/2))���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber1].fDEF) * 1.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//((�v���C���[�P�̍U���́{�v���C���[�P�̖h���-�v���C���[2�̖h���/2))���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber1].fDEF) * 1.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//((�v���C���[�P�̍U���́{�v���C���[�P�̖h���-�v���C���[2�̖h���/2))���i�P�D�O�P�`�P�D�Q�T�j
	}

	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L


	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 15;//�v���C���[�Q�̂l�o���P�T���炷�B
	printf("%s��\x1b[32mMP���P�T�������I\x1b[0m\n", &g_aPlayer[PlayerNumber2].aName[0]);
}

//=======================================================================================
//�v���C���[�Q�̃X�L���u�z�[���[���C�v
//=======================================================================================
void Player2HolyRay(Player* pPlayer)//�v���C���[�Q���ޏ��̃X�L���i�Q�j�A�u�z�[���[���C�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 45;//�X�L���������Ƀv���C���[�Q��MP���T�O���炷�B
	pPlayer--;//�|�C���^����v���C���[�P��

	printf("%s�̃z�[���[���C�I\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber2].fDEF) * 1.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//((�v���C���[�Q�̍U���́{�v���C���[�Q�̖h���-�v���C���[2�̖h���/2))���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber2].fDEF) * 1.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//((�v���C���[�Q�̍U���́{�v���C���[�Q�̖h���)*1.2-�v���C���[2�̖h���/2))���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber2].fDEF) * 1.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//((�v���C���[�P�̍U���́{�v���C���[�P�̖h���-�v���C���[2�̖h���/2))���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber2].fDEF) * 1.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//((�v���C���[�P�̍U���́{�v���C���[�P�̖h���-�v���C���[2�̖h���/2))���i�P�D�O�P�`�P�D�Q�T�j
	}

	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L



	pPlayer->fMP = pPlayer->fMP - 15;//�v���C���[�P�̂l�o���P�T���炷�B
	printf("%s��\x1b[32mMP���P�T�������I\x1b[0m\n", &g_aPlayer[PlayerNumber1].aName[0]);
}

//===========================================================================
//�v���C���[�P�̃X�L���F�u���b�f�B�o�C�g
//===========================================================================
void Player1BloodyBite(Player* pPlayer)//�v���C���[�P���z���S�̃X�L���i�P�j�A�u�u���b�e�B�o�C�g�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 35;//�X�L���������Ƀv���C���[�P��MP��35���炷�B

	printf("%s�̃u���b�f�B�o�C�g�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*2.2-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*2.2-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.2-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.2 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.2-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}
	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L

	printf("�^�����_���[�W��\x1b[32m�V�T��\x1b[0m�g�o���񕜂���I\n");
	pPlayer->fLife = pPlayer->fLife + (fDamageResult * 0.75);//�v���C���[�P���^�����_���[�W�̂O�D�V�T�{�g�o���񕜂���B
}

//====================================================================================
//�v���C���[�Q�̃X�L���F�u���b�f�B�o�C�g
//====================================================================================
void Player2BloodyBite(Player* pPlayer)//�v���C���[�Q���z���S�̃X�L���i�P�j�A�u�u���b�f�B�o�C�g�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 35;//�X�L���������Ƀv���C���[�Q��MP��35���炷�B
	pPlayer--;//�|�C���^����v���C���[�P��

	printf("%s�̃u���b�f�B�o�C�g�I\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.2-�v���C���[�P�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.2-�v���C���[�P�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.2-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.2 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.2-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}
	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L

	printf("�^�����_���[�W��\x1b[32m�V�T��\x1b[0m�g�o���񕜂���I\n");
	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fLife = pPlayer->fLife + (fDamageResult * 0.75);//�v���C���[�Q���^�����_���[�W�̂O�D�V�T�{�g�o���񕜂���B
	pPlayer--;//�|�C���^����v���C���[�P��
}

//===================================================================
//�v���C���[�P�̃X�L���F�N���C�W�[�O���r�e�B
//===================================================================
void Player1CrazyGracity(Player* pPlayer)//�v���C���[�P���z���S�̃X�L���i�Q�j�A�u�N���C�W�[�O���r�e�B�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	float fRandum1;
	//int CheckRandum;
	//float fDamageResult;

	pPlayer->fMP = pPlayer->fMP - 65;//�X�L���������Ƀv���C���[1��MP��70���炷�B

	fRandum1 = rand() % 10 + 1;//�Q��ނ̌��ʂ��m���łǂ��炩����������B

	printf("%s�̃N���C�W�[�O���r�e�B�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	if (fRandum1 >= 1 && fRandum1 <= 3)//���ʂ��R�O���̊m���Ŕ�������B
	{
		pPlayer++;//�|�C���^����v���C���[�Q��
		pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.2);//�v���C���[�Q�̌��݂g�o���Q�O�����炷�A
		printf("%s�̗̑͂�\x1b[35m���݂g�o�Q�O�������������I\x1b[0m\n", &pPlayer->aName);
		pPlayer--;//�|�C���^����v���C���[�P��
	}
	else
	{
		pPlayer->fLife = pPlayer->fLife + 1500;//�v���C���[�P�̗̑͂��P�T�O�O�񕜂���B
		printf("%s�̗̑͂�\x1b[32m�P�T�O�O\x1b[0m�񕜂����I\n", &pPlayer->aName);
	}

}

//===============================================================================
//�v���C���[�Q�̃X�L���F�N���C�W�[�O���r�e�B
//===============================================================================
void Player2CrazyGravity(Player* pPlayer)//�v���C���[�Q���z���S�̃X�L���i�Q�j�A�u�N���C�W�[�O���r�e�B�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	float fRandum1;
	//int CheckRandum;
	//float fDamageResult;

	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 65;//�X�L���������Ƀv���C���[1��MP��70���炷�B
	pPlayer--;//�|�C���^����v���C���[�P��

	fRandum1 = rand() % 10 + 1;//�Q��ނ̌��ʂ��m���łǂ��炩�𔭓������邽�߂ɌĂяo���ϐ��B

	printf("%s�̃N���C�W�[�O���r�e�B�I\n", &g_aPlayer[PlayerNumber2].aName[0]);

	if (fRandum1 >= 1 && fRandum1 <= 3)//���ʂ��R�O���̊m���Ŕ�������B
	{
		pPlayer->fLife = pPlayer->fLife - (pPlayer->fLife * 0.2);//�v���C���[�P�̌��݂g�o���Q�O�����炷�A
		printf("%s�̗̑͂�\x1b[35m���݂g�o�Q�O�������������I\x1b[0m\n", &pPlayer->aName);
	}
	else
	{
		pPlayer++;//�|�C���^����v���C���[�Q��
		pPlayer->fLife = pPlayer->fLife + 1500;//�v���C���[�P�̗̑͂��P�T�O�O�񕜂���B
		printf("%s�̗̑͂�\x1b[32m�P�T�O�O\x1b[0m�񕜂����I\n", &pPlayer->aName);
		pPlayer--;//�|�C���^����v���C���[�P��
	}

}

//======================================================================================
//�v���C���[�P�̃X�L���F�t�@���g���X�^�u
//======================================================================================
bool Player1PhantomStab(Player* pPlayer)//�v���C���[�P���z���S�̃X�L���i�R�j�A�u�t�@���g���X�^�u�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	int SkillDebuffRandum;//�X�L���s��^����m���������֐���錾����B
	bool bPlayre2PhantomStabDebuff = false;//�v���C���[�Q�ɃX�L���s��^����t���O��false�ɂ���B

	pPlayer->fMP = pPlayer->fMP - 50;//�X�L���������Ƀv���C���[�P��MP��50���炷�B

	printf("%s�̃t�@���g���X�^�u�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1;
	CheckRandum = rand() % 2 + 1;
	SkillDebuffRandum = rand() % 20 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B

	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 3.1 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*3.1-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 3.1 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*3.1-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 3.1 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*3.1-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 3.1 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*3.1-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}
	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L

	if (SkillDebuffRandum >= 1 && SkillDebuffRandum <= 7)//�R�T���̊m���ŃX�L���s�i�R�s�j�𔭓�����B
	{
		bPlayre2PhantomStabDebuff = true;//�v���C���[�Q�ɃX�L���s��^����t���O��true�ɂ���B
		printf("%s�͂R�^�[���̊ԁA\x1b[35m�X�L�����g�p�ł��Ȃ��Ȃ����I\x1b[0m\n", &g_aPlayer[PlayerNumber2].aName[0]);
	}
	return bPlayre2PhantomStabDebuff;//�v���C���[�Q���X�L���u�t�@���g���X�^�u�v�̃X�L���s�̃f�o�t���󂯂Ă��邩�ǂ����̃t���O��Ԃ��B
}

//==========================================================
//�v���C���[�Q�̃X�L���F�t�@���g���X�^�u
//==========================================================
bool Player2PhantomStab(Player* pPlayer)//�v���C���[�Q���z���S�̃X�L���i�R�j�A�u�t�@���g���X�^�u�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	int SkillDebuffRandum;//�X�L���s��^����m���������֐���錾����B
	bool bPlayre1PhantomStabDebuff = false;//�v���C���[�P�ɃX�L���s��^����t���O��false�ɂ���B

	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 50;//�X�L���������Ƀv���C���[�Q��MP��50���炷�B
	pPlayer--;//�|�C���^����v���C���[�P��

	printf("%s�̃t�@���g���X�^�u�I\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, SkillDebuffRandum = rand() % 20 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 3.1 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U����*3.1-�v���C���[�P�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 3.1 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U����*3.1-�v���C���[�P�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 3.1 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U����*3.1-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 3.1 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U����*3.1-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}
	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L

	if (SkillDebuffRandum >= 1 && SkillDebuffRandum <= 7)//�R�T���̊m���ŃX�L���s�i�R�s�j�𔭓�����B
	{
		bPlayre1PhantomStabDebuff = true;//�v���C���[�P�ɃX�L���s��^����t���O��true�ɂ���B
		printf("%s�͂R�^�[���̊ԁA\x1b[35m�X�L�����g�p�ł��Ȃ��Ȃ����I\x1b[0m\n", &g_aPlayer[PlayerNumber1].aName[0]);
	}
	return bPlayre1PhantomStabDebuff;//�v���C���[�P���X�L���u�t�@���g���X�^�u�v�̃X�L���s�̃f�o�t���󂯂Ă��邩�ǂ����̃t���O��Ԃ��B
}

//============================================================
//�v���C���[�P�̃X�L���F�|�C�Y���~�X�g
//============================================================
bool Player1PoisonMist(Player* pPlayer)//�v���C���[�P���Ȋw�҂̃X�L���i�P�j�A�u�|�C�Y���~�X�g�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;
	bool bPlayer2PoisonMistState;//�v���C���[�Q�֓ŏ�Ԃ�^����t���O��錾����B
	bPlayer2PoisonMistState = false;//�t���O��false�ɂ���B
	int nPoisonStateRandum;//�ŏ�Ԃɂ���m���𑀍삷��ϐ��B

	pPlayer->fMP = pPlayer->fMP - 40;//�X�L���������Ƀv���C���[�P��MP��40���炷�B

	printf("%s�̃|�C�Y���~�X�g�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nPoisonStateRandum = rand() % 20 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.7 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*2.7-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.7 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*2.7-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.7 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.7-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.7 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.7-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}
	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L

	if (nPoisonStateRandum >= 1 && nPoisonStateRandum <= 7)
	{
		bPlayer2PoisonMistState = true;//�v���C���[�Q��ŏ�Ԃɂ���t���O��true�ɂ���B
		printf("%s�͂R�^�[���̊ԁA\x1b[35n�ŏ�ԂɂȂ����I\x1b[0m�i���݂��̃v���C���[���s������x�A����HP�̂R���_���[�W���U���͂Q�O��DOWN�j", &g_aPlayer[PlayerNumber2].aName[0]);
	}
	return bPlayer2PoisonMistState;//�v���C���[�Q���X�L���u�|�C�Y���~�X�g�v�̓ŏ�Ԃ��󂯂Ă��邩�ǂ����̃t���O��Ԃ��B
}

//=================================================
//�v���C���[�Q�̃X�L���F�|�C�Y���~�X�g
//=================================================
bool Player2PoisonMist(Player* pPlayer)//�v���C���[�Q���Ȋw�҂̃X�L���i�P�j�A�u�|�C�Y���~�X�g�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;
	bool bPlayer1PoisonMistState;//�v���C���[�P�֓ŏ�Ԃ�^����t���O��錾����B
	bPlayer1PoisonMistState = false;//�t���O��false�ɂ���B
	int nPoisonStateRandum;//�ŏ�Ԃɂ���m���𑀍삷��ϐ��B

	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 40;//�X�L���������Ƀv���C���[�Q��MP��40���炷�B
	pPlayer--;//�|�C���^����v���C���[�P��

	printf("%s�̃|�C�Y���~�X�g�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nPoisonStateRandum = rand() % 20 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.7 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.7-�v���C���[�P�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.7 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.7-�v���C���[�P�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.7 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.7-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.7 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.7-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}
	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L

	if (nPoisonStateRandum >= 1 && nPoisonStateRandum <= 7)
	{
		bPlayer1PoisonMistState = true;//�v���C���[�P��ŏ�Ԃɂ���t���O��true�ɂ���B
		printf("%s�͂R�^�[���̊ԁA\x1b[35m�ŏ�ԂɂȂ����I\x1b[0m�i���݂��̃v���C���[���s������x�A����HP�̂R���̃_���[�W���U���͂Q�O��DOWN�j\n", &g_aPlayer[PlayerNumber1].aName[0]);
	}
	return bPlayer1PoisonMistState;//�v���C���[�P���X�L���u�|�C�Y���~�X�g�v�̓ŏ�Ԃ��󂯂Ă��邩�ǂ����̃t���O��Ԃ��B
}

//================================================
//�v���C���[�P�̃X�L���F�X�`�[���o�[�X�g
//================================================
bool Player1SteamBurst(Player* pPlayer)//�v���C���[�P���Ȋw�҂̃X�L���i�Q�j�A�u�X�`�[���o�[�X�g�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	bool bPlayer1SteamBurstBurn = false;//�v���C���[�P���X�L���u�X�`�[���o�[�X�g�v�̃f�����b�g���ʁu�Ώ��v���󂯂Ă��邱�Ƃ�\���t���O��錾����B
	pPlayer->fMP = pPlayer->fMP - 60;//�X�L���������Ƀv���C���[�P��MP��60���炷�B

	int nSteamBurstBurnRandum;//�X�L���u�X�`�[���o�[�X�g�v�̃f�����b�g���ʂ��N�����m���𑀍삷��ϐ���錾����B

	printf("%s�̃X�`�[���o�[�X�g�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nSteamBurstBurnRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (nSteamBurstBurnRandum == 1)
	{
		if (CheckRandum == 1)
		{
			g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber2].fDEF) * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U���́{�v���C���[�Q�̖h���)*4-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

			fDamageResult = ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber2].fDEF) * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U���́{�v���C���[�Q�̖h���)*4-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


		}
		else if (CheckRandum == 2)
		{
			g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber2].fDEF) * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U���́{�v���C���[�Q�̖h���)*4-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j

			fDamageResult = ((g_aPlayer[PlayerNumber1].fATK + g_aPlayer[PlayerNumber2].fDEF) * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U���́{�v���C���[�Q�̖h���)*4-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j

		}
		printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L
	}

	else if (nSteamBurstBurnRandum == 2)
	{
		bPlayer1SteamBurstBurn = true;//�v���C���[�P�̃X�L���u�X�`�[���o�[�X�g�v�����s���f�����b�g���ʁu�Ώ��R�^�[���v��t�^������̂ŁA�t���O��true�ɂ���B
		printf("%s�̃X�`�[���o�[�X�g���\�������I�U�������s���R�^�[���̊�\x1b[31m�Ώ����\x1b[0m�ɂȂ����I�i���݂��̃v���C���[���s������x�A����HP�̂R���_���[�W���h��͂Q�O��DOWN�j", &g_aPlayer[PlayerNumber1].aName[0]);
	}

	return bPlayer1SteamBurstBurn;
}

//==============================================================
//�v���C���[�Q�̃X�L���F�X�`�[���o�[�X�g
//==============================================================
bool Player2SteamBurst(Player* pPlayer)//�v���C���[�Q���Ȋw�҂̃X�L���i�Q�j�A�u�X�`�[���o�[�X�g�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	bool bPlayer2SteamBurstBurn = false;//�v���C���[�Q���X�L���u�X�`�[���o�[�X�g�v�̃f�����b�g���ʁu�Ώ��v���󂯂Ă��邱�Ƃ�\���t���O��錾����B

	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 60;//�X�L���������Ƀv���C���[�Q��MP��60���炷�B
	pPlayer--;//�|�C���^����v���C���[�P��

	int nSteamBurstBurnRandum;//�X�L���u�X�`�[���o�[�X�g�v�̃f�����b�g���ʂ��N�����m���𑀍삷��ϐ���錾����B

	printf("%s�̃X�`�[���o�[�X�g�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nSteamBurstBurnRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (nSteamBurstBurnRandum == 1)
	{
		if (CheckRandum == 1)
		{
			g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber1].fDEF) * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U���́{�v���C���[�P�̖h���)*4-�v���C���[�P�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

			fDamageResult = ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber1].fDEF) * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U���́{�v���C���[�P�̖h���)*4-�v���C���[�P�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


		}
		else if (CheckRandum == 2)
		{
			g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber1].fDEF) * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U���́{�v���C���[�P�̖h���)*4-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j

			fDamageResult = ((g_aPlayer[PlayerNumber2].fATK + g_aPlayer[PlayerNumber1].fDEF) * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U���́{�v���C���[�P�̖h���)*4-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j

		}
		printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L
	}

	else if (nSteamBurstBurnRandum == 2)
	{
		bPlayer2SteamBurstBurn = true;//�v���C���[�Q�̃X�L���u�X�`�[���o�[�X�g�v�����s���f�����b�g���ʁu�Ώ��R�^�[���v��t�^������̂ŁA�t���O��true�ɂ���B
		printf("%s�̃X�`�[���o�[�X�g���\�������I�U�������s���R�^�[���̊�\x1b[31m�Ώ����\x1b[0m�ɂȂ����I", &g_aPlayer[PlayerNumber2].aName[0]);
	}

	return bPlayer2SteamBurstBurn;
}

//=======================================================
//�v���C���[�P�̃X�L���F�C�J�T�}���[�U�[
//=======================================================
bool Player1SquidLaser(Player* pPlayer)//�v���C���[�P���Ȋw�҂̃X�L���i�R�j�A�u�C�J�T�}���[�U�[�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;
	int nSquitLaserDebuffRandum;//�f�o�t����������m���𑀍삷��ϐ���錾����B
	bool bPlayer2SquitLaserDebuff = false;//�C�J�T�}���[�U�[�̃f�o�t���������Ă��邩�ǂ����𔻒肷��t���O��錾����B


	pPlayer->fMP = pPlayer->fMP - 45;//�X�L���������Ƀv���C���[�P��MP��45���炷�B

	printf("%s�̃C�J�T�}���[�U�[�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nSquitLaserDebuffRandum = rand() % 10 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.6 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.6-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.6 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.6-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber2].fATK * 2.6 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.6-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j�j

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 2.6 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.6-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

	}

	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L

	if (nSquitLaserDebuffRandum >= 1 && nSquitLaserDebuffRandum <= 10)
	{
		bPlayer2SquitLaserDebuff = true;//�v���C���[�Q�ɃC�J�T�}���[�U�[�̃f�o�t�u�h��̓_�E���v��^����t���O��true�ɂ���B
		printf("%s��\x1b[34m�h��͂��S�O�����������I\x1b[0m\n", &g_aPlayer[PlayerNumber2].aName[0]);
	}

	return bPlayer2SquitLaserDebuff;
}

//=======================================================
//�v���C���[�Q�̃X�L���F�C�J�T�}���[�U�[
//=======================================================
bool Player2SquidLaser(Player* pPlayer)//�v���C���[�P���Ȋw�҂̃X�L���i�R�j�A�u�C�J�T�}���[�U�[�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;
	int nSquitLaserDebuffRandum;//�f�o�t����������m���𑀍삷��ϐ���錾����B
	bool bPlayer1SquitLaserDebuff = false;//�C�J�T�}���[�U�[�̃f�o�t���������Ă��邩�ǂ����𔻒肷��t���O��錾����B

	pPlayer++;//1�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 45;//�X�L���������Ƀv���C���[�P��MP��45���炷�B
	pPlayer--;//�|�C���^����v���C���[�P��

	printf("%s�̃C�J�T�}���[�U�[�I\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, nSquitLaserDebuffRandum = rand() % 10 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.6 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U����*2.6-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.6 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*2.6-�v���C���[�P�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber1].fATK * 2.6 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.6-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j�j

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 2.6 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*2.6-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j

	}

	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L

	if (nSquitLaserDebuffRandum >= 1 && nSquitLaserDebuffRandum <= 10)
	{
		bPlayer1SquitLaserDebuff = true;//�v���C���[�P�ɃC�J�T�}���[�U�[�̃f�o�t�u�h��̓_�E���v��^����t���O��true�ɂ���B
		printf("%s��\x1b[34m�h��͂��S�O�����������I\x1b[0m\n", &g_aPlayer[PlayerNumber1].aName[0]);
	}

	return bPlayer1SquitLaserDebuff;
}

//======================================================
//�v���C���[�P�̃X�L���F�t���C���E�F�[�u
//======================================================
bool Player1FlameWave(Player* pPlayer)//�v���C���[�P�����@�g���̃X�L���i�P�j�A�u�t���C���E�F�[�u�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;
	bool bPlayer1FlameWaveBuff = false;//�v���C���[�P���X�L���u�t���C���E�F�[�u�v�̃o�t���󂯂Ă��邩�ǂ����̃t���O��錾����B
	int FlameWaveBuffRandum;//�X�L���̃o�t����������m���𑀍삷��ϐ��B
	FlameWaveBuffRandum = rand() % 3 + 1;

	pPlayer->fMP = pPlayer->fMP - 35;//�v���C���[�P�̂l�o���R�T���炷�B

	printf("%s�̃t���C���E�F�[�u�I\n", &pPlayer->aName[0]);

	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 4; nCount++, nAttackSucces = rand() % 2 + 1, fRandum1 = rand() % 25 + 1)//�S�A���U�����s���B�i�������T�O���j
		{
			if (nAttackSucces == 1)//�U�������m���̃����_���֐����P��������U���𐬌�������B
			{
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK * 1.1) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�P�̍U���́��P�D�P-�v���C���[�Q�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j

				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK * 1.1) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�P�̍U���́�1.1-�v���C���[�Q�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j


				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L
				getchar();
			}

			else
			{
				printf("\x1b[34m�U�����O�ꂽ�I\x1b[0m\n");//�U�O���̊m���ōU�����O������B
				getchar();
			}
		}

	}
	else if (CheckRandum == 2)
	{
		for (nCount = 0; nCount < 4; nCount++, nAttackSucces = rand() % 2 + 1, fRandum1 = rand() % 25 + 1)//�S�A���U�����s���B�i�������T�O���j
		{
			if (nAttackSucces == 1)//�U�������m���̃����_���֐����P��������U���𐬌�������B
			{
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK * 1.1) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�P�̍U���́�1.1-�v���C���[�Q�̖h���/2���i�O�D�V�T�`�O�D�X�X�j

				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK * 1.1) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�P�̍U���́�1.1-�v���C���[�Q�̖h���/4���i�O�D�V�T�`�O�D�X�X�j


				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L
				getchar();
			}

			else
			{
				printf("\x1b[34m�U�����O�ꂽ�I\x1b[0m\n");//�T�O���̊m���ōU�����O������B
				getchar();
			}
		}

	}

	if (FlameWaveBuffRandum == 1)
	{
		bPlayer1FlameWaveBuff = true;//�o�t�𔭓�������̂Ńt���O��true�ɂ���B
		printf("%s�̍U���͂��Q�^�[���̊ԁA\x1b[31m�Q�T��\x1b[0m�㏸�I", &pPlayer->aName[0]);
	}
	return bPlayer1FlameWaveBuff;
}

//========================================================================================
//�v���C���[�Q�̃X�L���F�t���C���E�F�[�u
//========================================================================================
bool Player2FlameWave(Player* pPlayer)//�v���C���[�Q�����@�g���̃X�L���i�P�j�A�u�t���C���E�F�[�u�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;
	bool bPlayer2FlameWaveBuff = false;//�v���C���[�Q���X�L���u�t���C���E�F�[�u�v�̃o�t���󂯂Ă��邩�ǂ����̃t���O��錾����B
	int FlameWaveBuffRandum;//�X�L���̃o�t����������m���𑀍삷��ϐ��B
	FlameWaveBuffRandum = rand() % 3 + 1;

	pPlayer++;//�|�C���^��v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 35;//�v���C���[�Q�̂l�o���R�T���炷�B
	printf("%s�̃t���C���E�F�[�u�I\n", &pPlayer->aName[0]);

	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 2 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 4; nCount++, nAttackSucces = rand() % 2 + 1, fRandum1 = rand() % 25 + 1)//�S�A���U�����s���B�i�������T�O���j
		{
			if (nAttackSucces == 1)//�U�������m���̃����_���֐����P��������U���𐬌�������B
			{
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK * 1.1) - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�Q�̍U���́��P�D�P-�v���C���[�P�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j

				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK * 1.1) - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�Q�̍U���́�1.1-�v���C���[�P�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j


				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L
				getchar();
			}

			else
			{
				printf("\x1b[34m�U�����O�ꂽ�I\x1b[0m\n");//�T�O���̊m���ōU�����O������B
				getchar();
			}
		}

	}
	else if (CheckRandum == 2)
	{
		for (nCount = 0; nCount < 4; nCount++, nAttackSucces = rand() % 2 + 1, fRandum1 = rand() % 25 + 1)//�S�A���U�����s���B�i�������T�O���j
		{
			if (nAttackSucces == 1)//�U�������m���̃����_���֐����P��������U���𐬌�������B
			{
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK * 1.1) - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�Q�̍U���́�1.1-�v���C���[�P�̖h���/2���i�O�D�V�T�`�O�D�X�X�j

				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK * 1.1) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//�v���C���[�P�̍U���́�1.1-�v���C���[�Q�̖h���/4���i�O�D�V�T�`�O�D�X�X�j


				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L
				getchar();
			}

			else
			{
				printf("\x1b[34m�U�����O�ꂽ�I\x1b[0m\n");//�U�O���̊m���ōU�����O������B
				getchar();
			}
		}

	}

	if (FlameWaveBuffRandum == 1)
	{
		bPlayer2FlameWaveBuff = true;//�o�t�𔭓�������̂Ńt���O��true�ɂ���B
		printf("%s�̍U���͂��Q�^�[���̊ԁA\x1b[31m�Q�T��\x1b[0m�㏸�I", &pPlayer->aName[0]);
	}
	return bPlayer2FlameWaveBuff;
}

//===============================================================================
//�v���C���[�P�̃X�L���F�A�C�X�����X
//===============================================================================
bool Player1IceLance(Player* pPlayer)//�v���C���[1�����@�g���̃X�L���i�R�j�A�u�A�C�X�����X�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	int ItemDebuffRandum;//�X�L���s��^����m���������֐���錾����B
	bool bPlayer2IceLanceDebuff = false;//�v���C���[�Q�ɃA�C�e���R�}���h�����^����t���O��false�ɂ���B

	pPlayer->fMP = pPlayer->fMP - 70;//�X�L���������Ƀv���C���[�P��MP��70���炷�B

	printf("%s�̃A�C�X�����X�I\n", &g_aPlayer[PlayerNumber1].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, ItemDebuffRandum = rand() % 20 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*4.5-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�P�̍U����*4.5-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - (g_aPlayer[PlayerNumber1].fATK * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*4.5-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber1].fATK * 4.5 - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�P�̍U����*4.5-�v���C���[2�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}
	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L

	if (ItemDebuffRandum >= 1 && ItemDebuffRandum <= 20)//�S�T���̊m���ŃA�C�e������i�R�s�j�𔭓�����B
	{
		bPlayer2IceLanceDebuff = true;//�v���C���[�Q�ɃA�C�e�������^����t���O��true�ɂ���B
		printf("%s�͂T�^�[���̊ԁA\x1b[35m�A�C�e���R�}���h\x1b[0m���g�p�ł��Ȃ��Ȃ����I\n", &g_aPlayer[PlayerNumber2].aName[0]);
	}
	return bPlayer2IceLanceDebuff;//�v���C���[�Q���X�L���u�A�C�X�����X�v�̃A�C�e������̃f�o�t���󂯂Ă��邩�ǂ����̃t���O��Ԃ��B
}

//=================================================
//�v���C���[�Q�̃X�L���F�A�C�X�����X
//=================================================
bool Player2IceLance(Player* pPlayer)//�v���C���[�Q�����@�g���̃X�L���i�R�j�A�u�A�C�X�����X�v�𔭓�����Ƃ��ɌĂяo���֐��B
{
	srand((unsigned int)time(NULL));//�����̎�����ݎ�������Ƀ����_�������B

	float fRandum1;
	int CheckRandum;
	float fDamageResult;

	int ItemDebuffRandum;//�X�L���s��^����m���������֐���錾����B
	bool bPlayer1IceLanceDebuff = false;//�v���C���[�P�ɃA�C�e���R�}���h�����^����t���O��false�ɂ���B

	pPlayer++;//�|�C���^����v���C���[�Q��
	pPlayer->fMP = pPlayer->fMP - 70;//�X�L���������Ƀv���C���[�Q��MP��70���炷�B
	pPlayer--;//�|�C���^����v���C���[�P��

	printf("%s�̃A�C�X�����X�I\n", &g_aPlayer[PlayerNumber2].aName[0]);

	fRandum1 = rand() % 25 + 1, CheckRandum = rand() % 2 + 1, ItemDebuffRandum = rand() % 20 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U����*4.5-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j

		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//(�v���C���[�Q�̍U����*4.5-�v���C���[2�̖h���/2)���i�P�D�O�P�`�P�D�Q�T�j


	}
	else if (CheckRandum == 2)
	{
		g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - (g_aPlayer[PlayerNumber2].fATK * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U����*4.5-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
		fDamageResult = (g_aPlayer[PlayerNumber2].fATK * 4.5 - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 - (fRandum1 / 100));//(�v���C���[�Q�̍U����*4.5-�v���C���[�P�̖h���/2)���i�O�D�V�T�`�O�D�X�X�j
	}
	printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L

	if (ItemDebuffRandum >= 1 && ItemDebuffRandum <= 20)//�S�T���̊m���ŃA�C�e������i�R�s�j�𔭓�����B
	{
		bPlayer1IceLanceDebuff = true;//�v���C���[�P�ɃA�C�e�������^����t���O��true�ɂ���B
		printf("%s�͂T�^�[���̊ԁA\x1b[35m�A�C�e���R�}���h\x1b[0m���g�p�ł��Ȃ��Ȃ����I\n", &g_aPlayer[PlayerNumber1].aName[0]);
	}
	return bPlayer1IceLanceDebuff;//�v���C���[�P���X�L���u�A�C�X�����X�v�̃A�C�e������̃f�o�t���󂯂Ă��邩�ǂ����̃t���O��Ԃ��B
}

//========================================================
//�v���C���[�P���A�C�e���u�g���C�v���g�p�������̏���
//========================================================
void Player1WaveCanon(Player* pPlayer)//�v���C���[�P���A�C�e���u�g���C�v����ɓ��ꂽ�Ƃ��ɌĂяo���֐��B
{
	printf("%s�͔g���C����ɓ��ꂽ�I\x1b[33m�U���́��P�D�T�{�̂P�O�A���U�����s���I\x1b[0m\n", &pPlayer->aName[0]);
	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;




	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 1 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 10; nCount++, nAttackSucces = rand() % 3 + 1, fRandum1 = rand() % 25 + 1)//�P�O�A���U�����s���B�i��������R�R���j
		{
			if (nAttackSucces == 1)//�U�������m���̃����_���֐����P��������U���𐬌�������B
			{
				g_aPlayer[PlayerNumber2].fLife = g_aPlayer[PlayerNumber2].fLife - ((g_aPlayer[PlayerNumber1].fATK * 1.5) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�P�̍U���́�1.5-�v���C���[�Q�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j

				fDamageResult = ((g_aPlayer[PlayerNumber1].fATK * 1.5) - (g_aPlayer[PlayerNumber2].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�P�̍U���́�1.5-�v���C���[�Q�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j


				printf("%s��\x1b[31m%.f�̃_���[�W�I\x1b[0m\n", &g_aPlayer[PlayerNumber2].aName[0], fDamageResult);//�_���[�W�\�L
				getchar();
			}

			else
			{
				printf("\x1b[34m�U�����O�ꂽ�I\x1b[0m\n");//��U�U���̊m���ōU�����O������B
				getchar();
			}
		}

	}

}

void Player2WaveCanon(Player* pPlayer)//�v���C���[�Q���A�C�e���u�g���C�v����ɓ��ꂽ�Ƃ��ɌĂяo���֐��B
{
	pPlayer++;//�|�C���^����v���C���[�Q��
	printf("%s�͔g���C����ɓ��ꂽ�I\x1b[33m�U���́��P�D�T�{�̂P�O�A���U�����s���I\x1b[0m\n", &pPlayer->aName[0]);
	pPlayer--;//�|�C���^����v���C���[�P��
	float fRandum1;
	int nAttackSucces;
	nAttackSucces = 0;
	int CheckRandum;
	float fDamageResult;
	int nCount;




	/*fRandum1 = rand() % 25 + 1*/ CheckRandum = rand() % 1 + 1;//�ŏI�I�ȃ_���[�W���Q�T���͈̔͂ŏ㉺������B�_���[�W���オ�邩�����邩�̔��f�́ACheckRandum�ϐ��Ŕ��f����B


	if (CheckRandum == 1)
	{
		for (nCount = 0; nCount < 10; nCount++, nAttackSucces = rand() % 3 + 1, fRandum1 = rand() % 25 + 1)//�P�O�A���U�����s���B�i��������R�R���j
		{
			if (nAttackSucces == 1)//�U�������m���̃����_���֐����P��������U���𐬌�������B
			{
				g_aPlayer[PlayerNumber1].fLife = g_aPlayer[PlayerNumber1].fLife - ((g_aPlayer[PlayerNumber2].fATK * 1.5) - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�Q�̍U���́�1.5-�v���C���[�P�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j

				fDamageResult = ((g_aPlayer[PlayerNumber2].fATK * 1.5) - (g_aPlayer[PlayerNumber1].fDEF / 2)) * (1 + (fRandum1 / 100));//�v���C���[�Q�̍U���́�1.5-�v���C���[�P�̖h���/2���i�P�D�O�P�`�P�D�Q�T�j


				printf("%s��\x1b[31m%.f\x1b[0m�̃_���[�W�I\n", &g_aPlayer[PlayerNumber1].aName[0], fDamageResult);//�_���[�W�\�L
				getchar();
			}

			else
			{
				printf("\x1b[34m�U�����O�ꂽ�I\x1b[0m\n");//��U�U���̊m���ōU�����O������B
				getchar();
			}
		}

	}

}