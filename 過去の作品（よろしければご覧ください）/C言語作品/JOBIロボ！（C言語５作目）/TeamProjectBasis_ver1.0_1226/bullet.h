//========================================================
//
//１１月１３日：３Ｄ空間で弾を発射する[bullet.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

#define MAX_BULLET (5000)//弾の最大数
#define NUM_BULLET (13)//弾の種類数

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	D3DXVECTOR3 Size;//大きさ
	int nType;//種類
	int nLife;//体力
	int nIdxShadow;//影のインデックス
	int nNumEnemy;//何番の敵が撃ってきているか
	float fRWidth;//幅（半径）
	float fRHeight;//高さ（半径)
	float fLength;//距離
	float fRotY;//Y回転
	bool bUse;//使用しているかどうか
	bool bOneFlag;//一回だけ発動させる処理ようフラグ
	bool bChangeFlag;//値を上下させる時に使うフラグ
	int nMaxLife;//最大体力

	float fVXaim;//X方向ベクトル
	float fVYaim;//Y方向ベクトル
	float fVZaim;//Z方向ベクトル
	float fVaim;//総合したベクトル

	int nAnimationPattern;//アニメーションパターン
	int nAnimationCnt;    //アニメーションカウント
}Bullet;

typedef enum
{
	BULLETTYPE_NORMAL = 0,//普通のバレット
	BULLETTYPE_ENEMY,//敵の弾
	BULLETTYPE_SWING,//スイング弾
	BULLETTYPE_RANDUM,//乱数弾
	BULLETTYPE_CURVE,//カーブ弾
	BULLETTYPE_WALL,//壁弾
	BULLETTYPE_BOUND,//バウンド弾
	BULLETTYPE_RING,//リング弾
	BULLETTYPE_CHAINRING,//リングに関連して移動する弾
	BULLETTYPE_CHAOS,//カオスボール
	BULLETTYPE_FIRE,//ファイアボール
	BULLETTYPE_WAVE,//ウェーブボール
	BULLETTYPE_THUNDER,//サンダーボール
	BULLETTYPE_MAX
}BULLETTYPE;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(int nType, int nLife, float fRWidth, float fRHeight, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col , int nNumEnemy, float fLength);

Bullet* GetBullet(void);//弾構造体の情報をゲットする
void DamageBullet(int nSubBattery);//バレットでダメージを与えられた時に呼び出す
#endif