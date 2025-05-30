//========================================================
//
//１１月７日：Xファイルを読み込む[Summon.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SUMMON_H_
#define _SUMMON_H_

#include "main.h"
#include "enemy.h"

#define NUM_SUMMON (100)//敵の種類
#define MAX_SUMMON (1024)//読み込む敵の最大数
#define SUMMONSET "data\\TEXTFILE\\Summon.txt"
#define MAX_SUMMONMAT (100)
typedef struct
{
	LPD3DXMESH pMesh;//メッシュ（頂点情報）へのポインタ（モデルの種類数分）
	LPD3DXBUFFER pBuffMat;//マテリアルへのポインタ
	DWORD dwNumMat;//マテリアルの数

}SummonModelInfo;

typedef struct
{
	int nType;//種類
	int nLayoutType;//配置される方法の種類
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 move;//移動量
	D3DXMATRIX mtxWorld;//マトリックスワールド
	bool bUse;//使用しているかどうか
	int nSummonTime;//召喚時間
	int nNumSetEdit;//エディターで配置された敵の番号
	bool bRemote;//遠隔からの変更を受けるかどうか
	D3DCOLORVALUE Diffuse[NUM_ENEMY][MAX_SUMMONMAT];//色合い
	D3DCOLORVALUE FormarDiffuse[NUM_ENEMY][MAX_SUMMONMAT];//色合い

}Summon;//敵召喚構造体

static const char* SUMMON_FILENAME[NUM_ENEMY] =
{
	"data\\MODEL\\OBJECT\\Cylinder\\Cylinder_000.x",
	"data\\MODEL\\OBJECT\\Cylinder\\Cylinder_001.x",
	"data\\MODEL\\OBJECT\\Cylinder\\Cylinder_002.x",
	"data\\MODEL\\OBJECT\\Cylinder\\Cylinder_003.x",
	"data\\MODEL\\OBJECT\\Cylinder\\Cylinder_004.x",
};


//プロトタイプ宣言
void InitSummon(void);
void UninitSummon(void);
void UpdateSummon(void);
void DrawSummon(void);
void SetSummon(int nType, int nTime, int nNumSetEdit,int nLayoutType, bool bRemote, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

Summon* GetSummon(void);//構造体の情報を取得
#endif
