//========================================================
//
//８月３０日：エフェクトもう一回見直す[staging.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _STAGING_H_
#define _STAGING_H

#include "main.h"
#include "Attack.h"
//プロトタイプ宣言
void InitStaging(void);
void UninitStaging(void);
void UpdateStaging(void);
void DrawStaging(void);

//構造体定義
typedef struct
{
	int nStagingType;//スキルのタイプを決める変数
	D3DXVECTOR3 pos;
	int nAtackSummon;//演出を発動させた場所に攻撃技を召喚する
	bool bUse;//演出を使っているかどうか
	float fScale;//大きさ
	int nA_Life;//召喚した攻撃のライフ
	int type;//演出の種類
	int nLife;//演出が消えるまでの時間
	int nStagingCount;//演出が始まってからのカウントを開始する。
	float fStagingPattern;//アニメーションの種類
	float fStagingPatternCountY;//アニメーションのY座標をずらすカウント
	bool bCountStartFlag;//演出が始まってからのカウントを開始するフラグを宣言する。
	int nAnimationRepeat;//アニメーションをリピートする変数
	bool bDraw;//描画するかどうかをbUse以外でさらに判定するフラグ
	int nCntEnemy;//n番目の敵のいる位置を判定する
	float fColor_a;//色のアルファ値を操作する変数
	bool bColorChange_a;//色のアルファ値をインクリメントするか、デクリメントするかを決めるフラグ
	float rot;//回転用
	float fAeffective_Scale;//召喚した攻撃の効果範囲
	float fA_Width;//召喚した攻撃の大きさ
	float fA_Height;//召喚した攻撃の高さ
	float fA_rotSpeed;//召喚する攻撃の回転速度
	int nCntTime;//時間を計測する
}Staging;



//列挙型定義
typedef enum
{
	STAGINGTYPE_WARNING = 0,//ルールが書かれた画面を呼び出す。
	STAGINGTYPE_APPER,//敵出現位置を予告する演出
    STAGINGTYPE_BULLETWARNING,//特定の弾を出現を予告する
	STAGINGTYPE_PRESSSTART,//「PressStart」の文字を表示する。
	STAGINGTYPE_PRESSENTER,//「PressEnter」の文字を表示する。
	STAGINGTYPE_SHIELD,//「スキル「イージスシールド」の代わりに使う列挙型定義
	STAGINGTYPE_NORMAL_CLEARBONUS,//ノーマルモードのクリアボーナス表記
	STAGINGTYPE_HARD_CLEARBONUS,//ハードモードのクリアボーナス表記
	STAGINGTYPE_VERYHARD_CLEARBONUS,//ベリーハードのクリアボーナス表記
	STAGINGTYPE_RANKING_BG,//ランキングの背景をここで一旦表示させる。
	STAGINGTYPE_BOSS1_SPECIAL_ATTACK,//ボス１の必殺技演出
	STAGINGTYPE_WARNING_ATTACK,//演出場所に攻撃を召喚する処理
	STAGINGTYPE_NORMAL_WARNINGS,//普通の危険演出。追加効果はない
	STAGINGTYPE_NORMAL_WARNINGS_RELATED,//追加効果なしの危険演出を、召喚している敵と関連させて動かす
	STAGINGTYPE_BOSS2_SPECIAL_ATTACK,//ボス２の必殺技演出
	STAGINGTYPE_EXPLOSIONWARNING,//爆発警告
	STAGINGTYPE_BOSS3_SPECIAL_ATTACK,//ボス３の必殺技演出
	STAGINGTYPE_BOSS4_SPECIAL_ATTACK,//ボス４必殺技処理
	STAGINGTYPE_FINALBOSS_SPECIAL_ATTACK,//ラスボスの必殺技演出
	STAGINGTYPE_SAFE,//安全なエリアの表示用
	STAGINGTYPE_MAX
}STAGINGTYPE;

void SetStaging(STAGINGTYPE type,int nCntEnemy,D3DXVECTOR3 pos,int nLife,ATTACKTYPE Atype,float Arot,float fScale,float fA_rotSpeed,float fA_Width,
	float fA_Height,int nA_Life);

//Staging* GetStaging(void);
#endif

