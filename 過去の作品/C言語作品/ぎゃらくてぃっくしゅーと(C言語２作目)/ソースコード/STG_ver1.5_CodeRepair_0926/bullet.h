//========================================================
//
//８月３０日：エフェクトもう一回見直す[bullet.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"


#define MAX_BULLET (4096)//弾の最大数
#define GENERALLY_BULLETLIFE (1500)//通常の弾のライフ

//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,//プレイヤーの弾
BULLETTYPE_ENEMY,
BULLETTYPE_SKILL1,
BULLETTYPE_REAFY,
BULLETTYPE_ROT,
BULLETTYPE_SKILL2_SLASH,
BULLETTYPE_ENEMY9,
BULLETTYPE_BOLBY,
BULLETTYPE_BANDY,
BULLETTYPE_ARROW,
BULLETTYPE_HORMING,//ホーミングショット
BULLETTYPE_SCATTERING,//ばらまき拡散弾
BULLETTYPE_REVERSE_CURVE,//BULLETTYPE_REAFYの逆方向のカーブの弾
BULLETTYPE_RIGHT_WIND,//右に発射されてから左にだんだん移動していく弾
BULLETTYPE_LEFT_WIND,//左に発射されてから右にだんだん移動していく弾
BULLETTYPE_REFLECTION,//壁にあたると反射する弾
BULLETTYPE_NOVA,//スキル「ノヴァブラストの弾」
BULLETTYPE_ROCKON,//この弾から更にプレイヤーへの自機狙いの弾が発射される。
BULLETTYPE_DOWNWIND,
BULLETTYPE_UPWIND,
BULLETTYPE_SATELLITE,//衛星弾
BULLETTYPE_MAX
}BULLETTYPE;

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//移動量
	int nLife;//寿命（どのくらい弾が飛んでいくか）
	BULLETTYPE type;//種類
	bool bUse;//弾を使用しているかどうか
	float fCurveBulletX;
	float fCurveBulletY;
	int nCntBullet_2;
	int nAnimationCounter_Slash;//斬撃アニメーションカウンター
	float fAnimationPattern_Slash;//斬撃アニメーションパターン
	bool bCurveFlag;//カーブの方向を表すフラグ
	float fEnemy5Curve;//エネミー５の弾をカーブさせる
	int nApperCount;//敵が出現した瞬間にカウントを開始する。
	float fBulletMove_Enemy9;//エネミー９の弾
	float fBulletMoveX_Enemy9;//エネミー９のX座標の弾
	float fBulletMoveY_Enemy9;//エネミー９のY座標の弾
	int nNumEnemy;//どのエネミーが発射している弾かを判別する
	float fXVaim;
	float fYVaim;
	float fShotVectorX;
	float fShotVectorY;
	float fVaim;
	int nNumber;	// エネミー５の番号
	bool bBulletReflectionX;//X方向に反射させるフラグ
	bool bBulletReflectionY;//Y方向に反射させるフラグ
	bool HormingFlag;
	float fRotMove;
	float fRotDest;
	float fRotDiff;
	int nHormingCount;
	int nHormingaim;//ホーミングのターゲットのエネミーを判別する。
	float fSpeed;//バレットのスピードを変える
	float fCurve;//バレットのカーブ値を変える
}Bullet;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move,int nLife,BULLETTYPE type, int nNum,int nNumEnemy,int nAimingEnemy);//posは「発射位置」、moveは「移動量」,damageは「弾の攻撃力」nNumEneyは、何番目の敵が攻撃しているか?

Bullet* GetBullet(void);//弾構造体の情報を取得する。

#endif
