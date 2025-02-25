//========================================================
//
//８月３０日：エフェクトもう一回見直す[Block.h]
//Author:ShinaTaiyo
//
//=========================================================
#ifndef _BLOCK_H_
#define _BLOCK_H

#include "main.h"

#define MAX_BLOCK (5000)
#define NORMAL_BLOCKSIZE (20.0f)//普通のブロックのサイズ



//bool CollisionBlock(void);

//構造体定義
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;//1f前の位置
	D3DXVECTOR3 move;
	int nCollisionUse;//bCollisionBlockを呼んでいる物を判別する
	bool bUse;//演出を使っているかどうか
	int nType;//演出の種類
	int nLife;//演出が消えるまでの時間
	float fWingMove;//ブロックをゆらゆら動かす
	float fXsize;//X方向の大きさ
	float fYsize;//Y方向の大きさ
	float fRadius;//半径で大きさを決めている場合の大きさ
	bool bDraw;//描画するかどうかをbUse以外でさらに判定するフラグ
	bool bMoveWay;//移動方向を変えるフラグ
}Block;



//列挙型定義
typedef enum
{
	BLOCKTYPE_NORMAL = 0,//0,普通のブロック（効果なし）
	BLOCKTYPE_GRASS,//1,草ブロック（効果なし）
	BLOCKTYPE_RENGA,//2,レンガブロック（効果なし）
	BLOCKTYPE_FRAME,//3,炎ブロック（スリップダメージ）
	BLOCKTYPE_WATER,//4,水ブロック（水の中にいる間は無限ジャンプ開放）
	BLOCKTYPE_SAND,//5,砂ブロック（少しずつ沈んでいく）
	BLOCKTYPE_WING,//6,羽ブロック（ふわふわしている）
	BLOCKTYPE_STONE,//7,岩ブロック（上から落ちてきて、当たると１５ダメージを受ける。）
	BLOCKTYPE_LINE,//8,線ブロック（ラストダンジョンメインブロック）
	BLOCKTYPE_ICE,//9,氷ブロック（超滑る）
	BLOCKTYPE_ARROW,//10,横に移動するブロック
	BLOCKTYPE_AUTOMOVE_RIGHT,//11,乗っている間、自動的にプレイヤーを右にずらしていくブロック
	BLOCKTYPE_AUTOMOVE_LEFT,//12,乗っている間、自動的にプレイヤーを左にずらしていくブロック
	BLOCKTYPE_BLINKING,//13,点滅ブロック（消えたり現れたりするブロック）
	BLOCKTYPE_MAX
}BLOCKTYPE;

typedef enum
{
	COLLISIONUSE_PLAYER = 0,//プレイヤーがbCollisionBlockを呼んでいる
	COLLISIONUSE_ENEMY,//敵がbCollisionBlockを呼んでいる
	COLLISIONUSE_ATTACK,//攻撃がbCollisionBlockを呼んでいる
	COLLISIONUSE_MAX
}COLLISIONUSE;

typedef enum
{
	COLLISIONTYPE_RADIUS = 0,//半径で判定している
	COLLISIONTYPE_LENGTH,//対角線で判定している
	COLLISIONTYPE_MAX
}COLLISIONTYPE;

//プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(int ntype, D3DXVECTOR3 pos, int nLife);
bool bCollisionBlock(D3DXVECTOR3* pPos,
	D3DXVECTOR3* pPosOld,
	D3DXVECTOR3* pMove,
	float fWidth,
	float fHeight,
	float *fGravity,
	COLLISIONTYPE CollisionType,
	bool *bMoveWay,
	COLLISIONUSE C_use,
	bool* bIsWalling);
Block* GetBlock(void);
//Block* GetBlock(void);
#endif


