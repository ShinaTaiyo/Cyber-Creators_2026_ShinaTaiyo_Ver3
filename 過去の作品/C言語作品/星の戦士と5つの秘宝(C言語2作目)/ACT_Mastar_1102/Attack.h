//========================================================
//
//１０月１２日：プレイヤーにコマンド技をつけたい[Attack.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _ATTACK_H_
#define _ATTACK_H_

#include "main.h"


#define MAX_ATTACK (4096)//攻撃の数の最大数
#define GENERALLY_ATTACK_LIFE (1500)//特に変更を加えないときの、標準の攻撃表示時間
//弾の種類
typedef enum
{
	ATTACKTYPE_PUNCH=0,//アビリティ「ノーマル」の時のプレイヤーの攻撃技その１
	ATTACKTYPE_SLASH_E,//斬撃攻撃(敵）
	ATTACKTYPE_ICE_EXPLOSION,//氷爆発攻撃（敵）
	ATTACKTYPE_THUNDER_E4,//雷攻撃（敵４）
	ATTACKTYPE_BOMU,//爆弾攻撃（ボムスライム）
	ATTACKTYPE_NEEDLE,//とげ攻撃（花スライム）
	ATTACKTYPE_CLAW,//爪攻撃（猫スライム）
	ATTACKTYPE_VACUUMBLADE,//真空刃（猫スライム）
	ATTACKTYPE_FALLINGBLOCK,//ブロック落とし（石スライム）
	ATTACKTYPE_BEAM,//ビーム攻撃（魔法使いスライム）
	ATTACKTYPE_ICICLECLAW,//アイシクルクロー（ボス２）
	ATTACKTYPE_ABSOLUTE_BLIZZARD,//アブソリュートブリザード（ボス２）
	ATTACKTYPE_FIRE,//炎攻撃（ボス３）
	ATTACKTYPE_SHAKINGSWORD,//剣攻撃（ボス３）
	ATTACKTYPE_FLASHBLADE,//瞬刃閃 （ボス３）
	ATTACKTYPE_THUNDER_2,//雷攻撃２（ボス４）
	ATTACKTYPE_FINALBEAM,//ビーム攻撃（ラスボス）
	ATTACKTYPE_MAX
}ATTACKTYPE;

//攻撃の移動タイプ
typedef enum
{
	MOVETYPE_NORMAL = 0,//普通に動く
	MOVETYPE_GRAVITY,//攻撃に重力がつく
	MOVETYPE_MAX
}MOVETYPE;


//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 posOld;//前の位置
	D3DXVECTOR3 move;//移動量
	D3DXVECTOR3 rot;//向き
	ATTACKTYPE type;//攻撃技の種類
	MOVETYPE moveType;//攻撃の動きの種類
	float fLength;//対角線の長さ
	float fAngle;//対角線の角度
	float fWidth;//幅
	float fHeight;//高さ
	float fGravity;//重力補正値
	float fXVaim;//自機狙いをするときの処理
	float fYVaim;//自機狙いをするときの処理
	float fVaim;//自機狙いをするときの処理
	float fEffectiveScale;//攻撃の効果範囲を決める（必要に応じて使用）
	float fRotSpeed;//回転させる攻撃の回転速度を決める
	int nLife;//寿命（どのくらい弾が飛んでいくか
	int nCntUseTime;//攻撃を使っている時間をカウントする
	int nColorChange;//色を変える変数
	int nTime;//攻撃が始まってからの時間を計測する
	int nCntAnim;//アニメーションカウンター
	int nPatternAnim;//アニメーションパターン
	int nPatternAnimY;//アニメーションが二段以上の時に、弾を移す処理
	int nHitCount;//一回の攻撃で攻撃がヒットする回数を決める
	int nNumEnemy;//どの敵が攻撃をしているかを判定する
	bool bUse;//攻撃をしているかどうか
	bool bIsLanding;//地面についているフラグ
	bool bIsWalling;//壁に当たっているかどうかのフラグ
}Attack;

//プロトタイプ宣言
void InitAttack(void);
void UninitAttack(void);
void UpdateAttack(void);
void DrawAttack(void);
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, ATTACKTYPE type, int nNum, int nNumEnemy, int nAimingEnemy,MOVETYPE moveType,
	float Width,float Height,int nLife,float rot,float fRotSpeed);//posは「発射位置」、moveは「移動量」,damageは「弾の攻撃力」nNumEneyは、何番目の敵が攻撃しているか?

Attack* GetAttack(void);//弾構造体の情報を取得する。

#endif

