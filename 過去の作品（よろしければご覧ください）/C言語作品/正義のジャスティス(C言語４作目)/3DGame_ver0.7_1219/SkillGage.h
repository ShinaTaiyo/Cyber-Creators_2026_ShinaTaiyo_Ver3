//========================================================
//
//８月３０日：エフェクトもう一回見直す[SkillGage.cpp]
//Author:ShinaTaiyo
//
//=========================================================

#ifndef _SKILLSkillGage_H_
#define _SKILLSkillGage_H





//構造体定義
typedef struct
{
	int nSkillGageCount;
	float fSkillGage;//ゲージの長さを変える変数
	int nType;//ゲージの種類
	bool bUse;//使用しているかどうか
	bool bBrinkingFlag;//点滅させるフラグ
	int nSubjectEnemy;//HPを表示する敵
	int nCountBoss1;//ボス１が出現してからの時間を計測する。
	float fBrinkingPoint;//点滅させるため、色を変える
	float fBoss1MaxHp;//ボス１が出現してからボス１のHpを保存する。
	D3DXVECTOR3 pos;//スキルゲージを設定する場所

}SkillGage;

typedef enum
{
	SKILLGAGETYPE_NORMAL=0,
	SKILLGAGETYPE_ENERGY,
	SKILLGAGETYPE_MAX
}SKILLGAGETYPE;

//プロトタイプ宣言
void InitSkillGage(void);
void UninitSkillGage(void);
void UpdateSkillGage(void);
void DrawSkillGage(void);
void AddSkillGage(float fSkillPoint);//スキルゲージを増やす処理



SkillGage* GetSkillGage(void);//ゲージ構造体の情報をゲットする。
#endif


