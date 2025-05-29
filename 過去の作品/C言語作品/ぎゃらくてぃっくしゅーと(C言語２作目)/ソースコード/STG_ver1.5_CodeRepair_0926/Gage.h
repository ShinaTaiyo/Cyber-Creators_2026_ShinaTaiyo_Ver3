//========================================================
//
//８月３０日：エフェクトもう一回見直す[Gage.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _GAGE_H_
#define _GAGE_H



typedef enum
{
	GAGETYPE_BOSS1HP=0,
	GAGETYPE_BOSSHP_FRAME,//ボスのHPゲージの枠組み
	GAGETYPE_MAX
}GAGETYPE;

//構造体定義
typedef struct
{
	int nGageCount;
	float fGage;//ゲージの長さを変える変数
	int nType;//ゲージの種類
	bool bUse;//使用しているかどうか
	int nSubjectEnemy;//HPを表示する敵
	int nCountBoss1;//ボス１が出現してからの時間を計測する。
	float fBoss1MaxHp;//ボス１が出現してからボス１のHpを保存する。


}Gage;


//プロトタイプ宣言
void InitGage(void);
void UninitGage(void);
void UpdateGage(void);
void DrawGage(void);

void SetGage(int nCntEnemy, int nType);

Gage* GetGage(void);//ゲージ構造体の情報をゲットする。
#endif

