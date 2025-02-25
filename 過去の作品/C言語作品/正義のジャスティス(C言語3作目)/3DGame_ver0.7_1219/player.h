//========================================================
//
//１１月７日：Xファイルを読み込む[Player.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _Player_H_
#define _Player_H_

#include "main.h"

#define MAX_PLAYERPARTS (100)//プレイヤーのパーツの最大数
#define NUM_PLAYERPARTS (100)//プレイヤーのパーツの種類
typedef struct
{
	//LPD3DXMESH pMesh;//メッシュ（頂点情報）へのポインタ
	//LPD3DXBUFFER pBuffMat;//マテリアルへのポインタ
	//DWORD dwNumMatPlayer;//マテリアルの数
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 vtxMinModel;//位置の最小値
	D3DXVECTOR3 vtxMaxModel;//位置の最大値
	D3DXVECTOR3 SaveRot;//向きを保存する
	D3DXVECTOR3 SavePos;//初期位置を保存する
	int nNumVtx;//頂点数
	BYTE* pVtxBuff;//頂点バッファへのポインタ
	DWORD sizeFVF;//頂点フォーマットのサイズ
	D3DXMATRIX mtxWorld;//ワールドマトリックス
	int nIdxModelParent;//親モデルのインデックス
	int nType;//読み込むモデルの種類
	bool bUse;//使用しているかどうか
	LPD3DXMESH pMesh;//メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER pBuffMat;//マテリアルへのポインタ
	DWORD dwNumMatPlayer;//マテリアルの数
}ModelParts;//パーツごとの動き


typedef enum
{
	PLAYERSTATE_NUTORAL = 0,//ニュートラル状態
    PLAYERSTATE_MOVE,//動いている状態
    PLAYERSTATE_ATTACK,//攻撃状態
	PLAYERSTATE_JUMP,//ジャンプする状態
	PLAYERSTATE_LANDING,//着地した
}PLAYERSTATE;

typedef struct
{

}MatPlayer;

typedef enum
{
	PLAYERMODE_NORMAL = 0,//普通モード
	PLAYERMODE_EDIT//エディットモード
}PLAYERMODE;


typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 posOld;//前の位置
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 targetrot;//目的の向き
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 vtxMax;//プレイヤー全体の最大頂点値
	D3DXVECTOR3 vtxMin;//プレイヤー全体の最小頂点値
	bool bMotionFlag;//モーション発動フラグ
	bool bLandingFlag;//着地モーションとニュートラルモーションを使い分ける
	int nMode;//モードの種類
	ModelParts aModel[MAX_PLAYERPARTS];
	MatPlayer aModelMat[NUM_PLAYERPARTS];
	bool bUse;//使用しているかどうか
	int nIdxShadow;//影のインデックス
	float fWidth;//プレイヤーの幅
	float fHeight;//プレイヤーの高さ
	bool bIsLanding;//地面にいるかどうか
	int nState;//プレイヤーの状態を表す
	int nCntMotionTime;//モーションを使用している時間を宣言する
	D3DXVECTOR3 Size;//プレイヤー全体の大きさ
	int nHp;//プレイヤーの体力

	bool bSpecialAttackFlag;//スペシャルアタック発動処理
	int nSpecialAttackCount;//スペシャルアタック発動時間をカウントする処理

	bool bAttackUp;//攻撃力上昇状態を付与するフラグ
	int nAttackUpStateCnt;//攻撃力上昇状態の時間をカウントする

	int nBarrierCount;//このカウントの数だけ被弾時にダメージを無効にする。

}Player;//パーツ全体（プレイヤーの動き）


//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
void SetDamagePlayer(int nDamage);

void SetPlayerParts(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdx,int nType);

bool bPlayerDeath(void);//プレイヤーが死んだかどうかの判定を返す

#endif

