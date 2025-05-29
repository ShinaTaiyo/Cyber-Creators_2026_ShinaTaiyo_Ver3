//========================================================
//
//８月３０日：エフェクトもう一回見直す[explosion.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "main.h"
#include "explosion.h"
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "health.h"
#include "sound.h"


//マクロ定義
#define MAX_EXPLOSION (10000)//爆発の最大数
#define EXPLOSION_TEXTURE "data\\TEXTURE\\explosion_002.png"
#define EXPLOSION_SCALE (20)
#define EXPLOSION_SCALE_NOVA (300.0f)//スキル「ノヴァブラスト」の爆発範囲
#define EXPLOSION_SCALE_E6 (100.0f)//敵６の攻撃の爆発の大きさ

//爆発構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXCOLOR col;//色
	int nCounterAnim;//アニメーションカウンター
	int nPatternAnim;//アニメーションパターンNo
	float fPatternAnimY;//テクスチャのY座標をずらす。
	bool bUse;//使用しているかどうか
	int nType;//爆発のタイプ（種類）
	float fWidth;//幅
	float fHeight;//高さ
	int nCntTime;//時間
	
}Explosion;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;//頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];//爆発の情報

//爆発の初期化処理
void InitExplosion(void)
{
	
	LPDIRECT3DDEVICE9 pDevice;
	int	nCntExplosion;


	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み（弾のテクスチャを読み込む）
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\explosion_002.png",
		&g_pTextureExplosion);

	//爆発情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		/*g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(600.0f, 300.0f, 0.0f); */
		g_aExplosion[nCntExplosion].bUse = false;//使用していない状態にする
		g_aExplosion[nCntExplosion].fWidth = 0.0f;//幅
		g_aExplosion[nCntExplosion].fHeight = 0.0f;//高さ
		g_aExplosion[nCntExplosion].nCntTime = 0;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,//４は必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);



	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定(弾の大きさ）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(40.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(40.0f, 40.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 0.5f);

		pVtx += 4;//頂点データのポインタを４つ分進める。
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//爆発の終了処理
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//爆発の更新処理
void UpdateExplosion(void)
{
	int nCntExplosion;
	int nCntEnemy;
	int nCntBullet;
	
	Bullet* pBullet = GetBullet();
	
	Player* pPlayer = GetPlayer();

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION;nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている（true)の間は、ずっと処理される
   			g_aExplosion[nCntExplosion].nCounterAnim++;//カウンタを進めてパターンNoを更新

			if (g_aExplosion[nCntExplosion].nCounterAnim%5==0)
			{
				g_aExplosion[nCntExplosion].nPatternAnim++;//１０分の１の速度でアニメーションを進める
			}


			if (g_aExplosion[nCntExplosion].nType == EXPLOSIONTYPE_NOVA)
			{

	/*			for (nCntBullet = 0; nCntBullet < MAX_ENEMY; nCntBullet++, pBullet++)
				{
					
					if(pBullet->bUse==true)
					{*/
							if (pPlayer->pos.x >= g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SCALE_NOVA && pPlayer->pos.x <= g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SCALE_NOVA &&
								pPlayer->pos.y >= g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SCALE_NOVA && pPlayer->pos.y <= g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SCALE_NOVA)
							{//敵と弾が当たった
								pBullet->bUse = false;//爆発範囲内の弾を消す。
								DamageHealth(10);
							}
						
						
				/*	}
				}*/
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SCALE_NOVA, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SCALE_NOVA, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SCALE_NOVA, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SCALE_NOVA, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SCALE_NOVA, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SCALE_NOVA, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SCALE_NOVA, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SCALE_NOVA, 0.0f);

			}
            else if (g_aExplosion[nCntExplosion].nType == EXPLOSIONTYPE_E6)
			{
				Player* pPlayer = GetPlayer();
				g_aExplosion[nCntExplosion].nCntTime++;

				if (g_aExplosion[nCntExplosion].nCntTime == 1)
				{
					PlaySound(SOUND_LABEL_SE_EXPLOSION_ENEMY_000);
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].fWidth, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].fWidth, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].fWidth, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].fWidth, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].fHeight, 0.0f);

				if (pPlayer->pos.x > g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].fWidth &&
					pPlayer->pos.x < g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].fWidth &&
					pPlayer->pos.y - pPlayer->fHeight / 2 > g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].fHeight &&
					pPlayer->pos.y - pPlayer->fHeight / 2 < g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].fHeight)
				{//プレイヤーの中心がどの位置にいるかで計算

					DamageHealth(1);

				}
			}

			pVtx[0].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim/ 8.0f, 0.0f+g_aExplosion[nCntExplosion].fPatternAnimY);
			pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim + 1.0f) / 8.0f, 0.0f + g_aExplosion[nCntExplosion].fPatternAnimY);
			pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim / 8.0f, 0.5f + g_aExplosion[nCntExplosion].fPatternAnimY);
			pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim+ 1.0f) / 8.0f, 0.5f + g_aExplosion[nCntExplosion].fPatternAnimY);


			
			
			if (g_aExplosion[nCntExplosion].nPatternAnim>8)//
			{
				g_aExplosion[nCntExplosion].fPatternAnimY = 0.5f;
			}
			if (g_aExplosion[nCntExplosion].nPatternAnim > 8&&g_aExplosion[nCntExplosion].fPatternAnimY==0.5f)//
			{
				g_aExplosion[nCntExplosion].bUse = false;//使用していない状態にする
			}

			

		}
		pVtx += 4;//
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//弾の描画処理
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	int nCntExplosion;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);



	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//弾が使用されている
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ数
				nCntExplosion * 4,
				2);
		}

	}


}

//爆発の設定処理
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int nType, float fWidth, float fHeight)
 {
	VERTEX_2D* pVtx;
 	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	int nCntExplosion;
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//爆発が終わったとき
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;//アニメーションカウンターのリセット
			g_aExplosion[nCntExplosion].nPatternAnim = 0;//アニメーションパターン（アニメーションの遅延用変数）のリセット
			g_aExplosion[nCntExplosion].fPatternAnimY = 0.0f;
			g_aExplosion[nCntExplosion].bUse = true;//爆発は、trueでかつ、発射された弾が消えると、爆発するので、trueに初期化しておくことが必要
			g_aExplosion[nCntExplosion].nType = nType;//爆発の種類
			g_aExplosion[nCntExplosion].fWidth = fWidth;//爆発の幅
			g_aExplosion[nCntExplosion].fHeight = fHeight;//爆発の高さ
			g_aExplosion[nCntExplosion].nCntTime = 0;
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SCALE, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SCALE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SCALE, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SCALE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SCALE, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SCALE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SCALE, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SCALE, 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;


			//頂点カラーの設定
			pVtx[0].col = g_aExplosion[nCntExplosion].col;
			pVtx[1].col = g_aExplosion[nCntExplosion].col;
			pVtx[2].col = g_aExplosion[nCntExplosion].col;
			pVtx[3].col = g_aExplosion[nCntExplosion].col;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.125f, 0.5f);

			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}
