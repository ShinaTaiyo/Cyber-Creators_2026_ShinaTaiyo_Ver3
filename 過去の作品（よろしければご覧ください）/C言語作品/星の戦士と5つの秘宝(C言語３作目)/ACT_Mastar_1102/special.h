//========================================================
//
//８月３０日：エフェクトもう一回見直す[Special.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _SPECIAL_H_
#define _SPECIAL_H

#include "main.h"
#include "Attack.h"
//プロトタイプ宣言
void InitSpecial(void);
void UninitSpecial(void);
void UpdateSpecial(void);
void DrawSpecial(void);

//構造体定義
typedef struct
{
	int nSpecialType;//スキルのタイプを決める変数
	D3DXVECTOR3 pos;
	int nAtackSummon;//演出を発動させた場所に攻撃技を召喚する
	bool bUse;//演出を使っているかどうか
	float fScale;//大きさ
	int nA_Life;//召喚した攻撃のライフ
	int type;//演出の種類
	int nLife;//演出が消えるまでの時間
	int nSpecialCount;//演出が始まってからのカウントを開始する。
	float fSpecialPattern;//アニメーションの種類
	float fSpecialPatternCountY;//アニメーションのY座標をずらすカウント
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
}Special;



//列挙型定義
typedef enum
{
	SPECIALTYPE_BOSS1_SPECIAL_ATTACK,//ボス１の必殺技演出
	SPECIALTYPE_BOSS2_SPECIAL_ATTACK,//ボス２の必殺技演出
	SPECIALTYPE_BOSS3_SPECIAL_ATTACK,//ボス３の必殺技演出
	SPECIALTYPE_BOSS4_SPECIAL_ATTACK,//ボス４必殺技処理
	SPECIALTYPE_FINALBOSS_SPECIAL_ATTACK,//ラスボスの必殺技演出
	SPECIALTYPE_MAX
}SpecialTYPE;

void SetSpecial(SpecialTYPE type, D3DXVECTOR3 pos, int nLife);

//Special* GetSpecial(void);
#endif

