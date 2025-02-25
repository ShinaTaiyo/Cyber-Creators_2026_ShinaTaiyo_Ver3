//========================================================
//
//８月３０日：エフェクトもう一回見直す[score.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _WAVE_H_
#define _WAVE_H_

#include "main.h"


//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	//D3DXVECTOR3 move;//移動量
	////int nLife;//寿命（どのくらい弾が飛んでいくか）
	bool bUse;//弾を使用しているかどうか

	int nBonusCountStart;//ウェーブごとに早くクリアするほどスコアが上がるカウントを作る。

	bool bWaveFlag;//各ウェーブが開始したかどうかを判定するフラグ

	bool bBonusCountFlag;//ウェーブ開始時にボーナスカウントをスタートさせるフラグ
}Wave;



//プロトタイプ宣言
void InitWave(void);
void UninitWave(void);
void UpdateWave(void);
void DrawWave(void);
bool GetWaveFlag(void);//現在が何ウェーブ目かを読み込む。
//void SetWave(int nScore);//posは「発射位置」、moveは「移動量」


#endif

