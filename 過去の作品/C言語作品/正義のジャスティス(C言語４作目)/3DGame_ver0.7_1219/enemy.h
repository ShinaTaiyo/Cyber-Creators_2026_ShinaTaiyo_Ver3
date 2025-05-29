//========================================================
//
//１１月７日：Xファイルを読み込む[Enemy.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

#define NUM_Enemy (100)//敵の種類
#define MAX_ENEMY (1024)//読み込む敵の最大数
#define ENEMYSET "data\\TEXTFILE\\Enemy.txt"

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 posOld;//1F前の位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 vtxMinEnemy;//それぞれの座標の敵の位置の最小値
	D3DXVECTOR3 vtxMaxEnemy;//それぞれの座標の敵の位置の最大値
	
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ
	int nType;//種類
	int nIdxShadow;//影のインデックス
	bool bUse;//使用しているかどうか
	bool bIsLanding;//地面にいるかどうか

	int nHp;//体力
	int nAttack;//攻撃力
	int nDefeatScore;//倒したときにもらえるスコア
	float fSpeed;//速さ

	float fVXaim;//X方向のベクトル
	float fVYaim;//Y方向のベクトル
	float fVZaim;//Z方向のベクトル
	float fVaim;//総合したベクトル
	float fVaimY1;//総合したY方向のベクトル
	float fVaimY2;
	float fVaim3;

	float fPenduram;//振り子のように弾を発射させる変数
	bool bPenduramFlag;

	D3DXVECTOR3 Size;//敵の大きさ
}Enemy;

typedef enum
{
	ENEMY00_NOOBSLIME = 0,//弱いスライム
	ENEMY01_ANGRYSLIME,//怒りスライム
	ENEMY02_SWORDSLIME,//剣スライム
	ENEMY03_HAMMERSLIME,//ハンマースライム
	ENEMY04_ORANGESLIME,//オレンジスライム
	ENEMY05_GREENSLIME,//グリーンスライム
	ENEMY06_WHITESLIME,//ホワイトスライム
	ENEMY07_TURQUOISSLIME,//ターコイズスライム
	ENEMY08_ENGELSLIME,//エンジェルスライム
	ENEMY09_DARKSLIME,//ダークスライム
	ENEMY10_RAINBOWSLIME,//レインボースライム
	ENEMY11_TREASURESLIME,//トレジャースライム
	ENEMY_MAX
}ENEMYTYPE;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
int GetNumEnemy(void);//読み込む敵の最大数を取得する
Enemy* GetEnemy(void);//敵構造体の情報を取得する

void SetDamageEnemy(int nDamage,int nNumEnemy);

#endif


