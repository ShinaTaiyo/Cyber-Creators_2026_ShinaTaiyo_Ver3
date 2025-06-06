//========================================================
//
//１１月１３日：３Ｄ空間でエミッターを発射する[Emitter.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _EMITTER_H_
#define _EMITTER_H_

#include "main.h"

#define MAX_EMITTER (50000)//エミッターの最大数
#define NUM_EMITTER (3)//エミッターの種類数

typedef enum
{
	EMITTERTYPE_EFFECT = 0,//エミッターでエフェクトを呼び出す
	EMITTERTYPE_PARTICLE,//エミッターでパーティクルを呼び出す
	EMITTERTYPE_CURVE,//カーブさせるエミッターを呼び出す
	EMITTERTYPE_TORNADO,//トルネードエミッター
	EMITTERTYPE_ROTATION,//決めた位置の周りを回り続けるエミッター
	EMITTERTYPE_GATHERING,//決めた位置に集まるエミッター
	EMITTERTYPE_ACCELEHORMING,//加速型のホーミング
	EMITTERTYPE_MAX
}EMITTERTYPE;

typedef enum
{
	EMITTERTARGET_ATTACKMODEL = 0,//攻撃モデルを中心に動かす
	EMITTERTARGET_ENEMY,          //敵を中心に動かす
	EMITTERTARGET_BOSS,           //ボスを中心に動かす
	EMITTERTARGET_MAX
}EMITTERTARGET;

typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 SupportPos;//決めた位置で何かしたいときに使う
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 AddMove;//移動量を追加する
	D3DXVECTOR3 rot;//向き
	D3DXCOLOR col;//色
	EMITTERTYPE EmitterType;//エミッターで呼び出す物の種類
	int nType;//読みだすエフェクトやパーティクルの種類
	int nTargetType;//どのモデルを中心にエミッターを出すか
	int nLife;//体力
	int nSummonLife;//呼び出す処理の体力を設定する
	int nNumAttack;//エミッターを呼び出している攻撃の番号を格納する
	D3DXVECTOR3 SummonMove;//呼び出す処理の移動量を設定する
	int nIdxShadow;//影のインデックス
	float fRWidth;//幅（半径）
	float fRHeight;//高さ（半径）
	float fScale;//エミッターの大きさ
	float fRedution;//エミッターの大きさを小さくするスピード
	float fRotMove;//進行方向を決める
	bool bUse;//使用しているかどうか
	float fLength;//距離
	int nCntTime;//エミッターが出現してからの時間をカウントする

	//======================================
	//３次元自機狙い弾の実装に使う変数
	//======================================
	float fVXaim3D;
	float fVYaim3D;
	float fVZaim3D;
	float fVLaim3D;
}Emitter;

//プロトタイプ宣言
void InitEmitter(void);
void UninitEmitter(void);
void UpdateEmitter(void);
void DrawEmitter(void);
void SetEmitterEffect(int nType,int nTargetType, int nLife, int nSummonLife, float fScale, float fRedution, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRotMove,int nNumAttack,float fLength,D3DXVECTOR3 SupportPos);

#endif




