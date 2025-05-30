//========================================================
//
//９月２６日：ハッカソンの基盤作り[EnemyGaugeFrame.cpp]
//Author:ShinaTaiyo
//
//=========================================================
#include "enemy.h"
#include "enemygauge.h"
#include "boss.h"
//=====================================================================================
//グローバル変数
//=====================================================================================
LPDIRECT3DTEXTURE9 g_pTextureEnemyHP = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemyHP = NULL;	//頂点バッファへのポインタ
EnemyHP g_aEnemyHP[MAX_ENEMY];	//構造体の情報

LPDIRECT3DTEXTURE9 g_pTextureEnemyWeakDisp[NUM_ENEMYWEAKDISP];//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemyWeakDisp = NULL;	//頂点バッファへのポインタ
EnemyWeakDisp g_aEnemyWeakDisp[MAX_ENEMYWEAKDISP];      //構造体の情報

//=====================================================================================
// 初期化処理
//=====================================================================================
void InitEnemyGauge(void)
{
	VERTEX_3D* pVtx;							//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"",
		&g_pTextureEnemyHP);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemyHP,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemyHP->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemyHP = 0; nCntEnemyHP < MAX_ENEMY; nCntEnemyHP++)
	{
		//構造体の情報
		g_aEnemyHP[nCntEnemyHP].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aEnemyHP[nCntEnemyHP].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	//色
		g_aEnemyHP[nCntEnemyHP].bUse = false;								//使用状態

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-ENEMYHP_WIDTH / 2, ENEMYHP_HEIGHT / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(ENEMYHP_WIDTH / 2, ENEMYHP_HEIGHT / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-ENEMYHP_WIDTH / 2, -ENEMYHP_HEIGHT / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(ENEMYHP_WIDTH / 2, -ENEMYHP_HEIGHT / 2.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = g_aEnemyHP[nCntEnemyHP].col;
		pVtx[1].col = g_aEnemyHP[nCntEnemyHP].col;
		pVtx[2].col = g_aEnemyHP[nCntEnemyHP].col;
		pVtx[3].col = g_aEnemyHP[nCntEnemyHP].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemyHP->Unlock();
}


//===================================
//敵弱点表示の初期化
//===================================
void InitEnemyWeakDisp(void)
{
	VERTEX_3D* pVtx;							//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WeakDisplay_Sword_000.png",
		&g_pTextureEnemyWeakDisp[ENEMYWEAKDISP00_SWORD]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WeakDisplay_Boomerang_000.png",
		&g_pTextureEnemyWeakDisp[ENEMYWEAKDISP01_BOOMERANG]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\WeakDisplay_Speaker_000.png",
		&g_pTextureEnemyWeakDisp[ENEMYWEAKDISP02_SPEAKER]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ENEMYWEAKDISP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemyWeakDisp,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemyWeakDisp->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemyWeakDisp = 0; nCntEnemyWeakDisp < MAX_ENEMYWEAKDISP; nCntEnemyWeakDisp++)
	{
		//構造体の情報
		g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos = NULL_VECTOR3;
		g_aEnemyWeakDisp[nCntEnemyWeakDisp].bUse = false;
		g_aEnemyWeakDisp[nCntEnemyWeakDisp].nType = 0;
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-ENEMYWEAKDISP_WIDTH / 2, ENEMYWEAKDISP_HEIGHT / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(ENEMYWEAKDISP_WIDTH / 2, ENEMYWEAKDISP_HEIGHT / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-ENEMYWEAKDISP_WIDTH / 2, -ENEMYWEAKDISP_HEIGHT / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(ENEMYWEAKDISP_WIDTH / 2, -ENEMYWEAKDISP_HEIGHT / 2.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemyWeakDisp->Unlock();
}

//==============================
// 終了処理
//==============================
void UninitEnemyGauge(void)
{
	//テクスチャの破棄
	if (g_pTextureEnemyHP != NULL)
	{
		g_pTextureEnemyHP->Release();
		g_pTextureEnemyHP = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEnemyHP != NULL)
	{
		g_pVtxBuffEnemyHP->Release();
		g_pVtxBuffEnemyHP = NULL;
	}
}

void UninitEnemyWeakDisp(void)
{
	//テクスチャの破棄
	for (int nCntEnemyWeakDisp = 0; nCntEnemyWeakDisp < NUM_ENEMYWEAKDISP; nCntEnemyWeakDisp++)
	{
		if (g_pTextureEnemyWeakDisp[nCntEnemyWeakDisp] != NULL)
		{
			g_pTextureEnemyWeakDisp[nCntEnemyWeakDisp]->Release();
			g_pTextureEnemyWeakDisp[nCntEnemyWeakDisp] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEnemyWeakDisp != NULL)
	{
		g_pVtxBuffEnemyWeakDisp->Release();
		g_pVtxBuffEnemyWeakDisp = NULL;
	}
}

//============================
// 更新処理
//============================
void UpdateEnemyGauge(void)
{
	VERTEX_3D* pVtx;			//頂点情報へのポインタ
	Enemy* pEnemy = GetEnemy();	//敵の情報の取得
	Boss* pBoss = GetBoss();    //ボスの情報を取得

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemyHP->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemyHP = 0; nCntEnemyHP < MAX_ENEMY; nCntEnemyHP++, pEnemy++)
	{
		//使用されている場合
		if (g_aEnemyHP[nCntEnemyHP].bUse == true && pEnemy->bUse == true)
		{
			//位置を更新
			g_aEnemyHP[nCntEnemyHP].pos.x = pEnemy->pos.x;
			g_aEnemyHP[nCntEnemyHP].pos.y = pEnemy->pos.y + pEnemy->Size.y;
			g_aEnemyHP[nCntEnemyHP].pos.z = pEnemy->pos.z;

			//該当する敵の体力の最大値を取得
			int nMaxHP = pEnemy->nMaxHp;

			//現在の敵の体力から割合を計算
			float fWidth = (ENEMYHP_WIDTH / 2) * ((float)pEnemy->nHp / (float)nMaxHP) * 2.0f;

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(-ENEMYHP_WIDTH / 2.0f, ENEMYHP_HEIGHT / 2.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fWidth - ENEMYHP_WIDTH / 2.0f, ENEMYHP_HEIGHT / 2.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-ENEMYHP_WIDTH / 2.0f, -ENEMYHP_HEIGHT / 2.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth - ENEMYHP_WIDTH / 2.0f, -ENEMYHP_HEIGHT / 2.0f, 0.0f);

			//残り体力に応じて色を変化
			if (fWidth > ENEMYHP_WIDTH / 2.0f)
			{//51%～
				g_aEnemyHP[nCntEnemyHP].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	//緑色
			}
			else if (fWidth > ENEMYHP_WIDTH / 4.0f && fWidth <= ENEMYHP_WIDTH / 2.0f)
			{//26%～50%
				g_aEnemyHP[nCntEnemyHP].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	//黄色
			}
			else
			{//～25%
				g_aEnemyHP[nCntEnemyHP].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	//赤色
			}

			//頂点カラーの更新
			pVtx[0].col = g_aEnemyHP[nCntEnemyHP].col;
			pVtx[1].col = g_aEnemyHP[nCntEnemyHP].col;
			pVtx[2].col = g_aEnemyHP[nCntEnemyHP].col;
			pVtx[3].col = g_aEnemyHP[nCntEnemyHP].col;
		}

		//敵の体力が0の場合
		if (pEnemy->nHp <= 0 || pBoss->bUse == true)
		{
			g_aEnemyHP[nCntEnemyHP].bUse = false;	//使用しない
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemyHP->Unlock();
}

void UpdateEnemyWeakDisp(void)
{

	VERTEX_3D* pVtx;			//頂点情報へのポインタ
	Enemy* pEnemy = GetEnemy();	//敵の情報の取得

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemyWeakDisp->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemyWeakDisp = 0; nCntEnemyWeakDisp < MAX_ENEMYWEAKDISP; nCntEnemyWeakDisp++, pEnemy++)
	{
		//使用されている場合
		if (g_aEnemyWeakDisp[nCntEnemyWeakDisp].bUse == true && pEnemy->bUse == true)
		{
			//位置を更新
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.x = pEnemy->pos.x;
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.y = pEnemy->pos.y + pEnemy->Size.y + 20.0f;
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.z = pEnemy->pos.z;
		
		}

		if (pEnemy->bUse == false)
		{
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].bUse = false;
		}
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemyWeakDisp->Unlock();
}

//================================
// 描画処理
//================================
void DrawEnemyGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用

	for (int nCntEnemyHP = 0; nCntEnemyHP < MAX_ENEMY; nCntEnemyHP++)
	{
		//使用している場合
		if (g_aEnemyHP[nCntEnemyHP].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemyHP[nCntEnemyHP].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEnemyHP[nCntEnemyHP].mtxWorld, NULL, &mtxView);	//逆行列を求める
			g_aEnemyHP[nCntEnemyHP].mtxWorld._41 = 0.0f;
			g_aEnemyHP[nCntEnemyHP].mtxWorld._42 = 0.0f;
			g_aEnemyHP[nCntEnemyHP].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemyHP[nCntEnemyHP].pos.x, g_aEnemyHP[nCntEnemyHP].pos.y, g_aEnemyHP[nCntEnemyHP].pos.z);
			D3DXMatrixMultiply(&g_aEnemyHP[nCntEnemyHP].mtxWorld, &g_aEnemyHP[nCntEnemyHP].mtxWorld, &mtxTrans);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			////Zの比較方法変更。
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			////Zバッファに書き込まない
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//アルファテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemyHP[nCntEnemyHP].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEnemyHP, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEnemyHP);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemyHP * 4, 2);

			//============================
			//描画の調整をもとに戻す
			//============================

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			////Zの比較方法変更。
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			////Zバッファに書き込む
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//アルファテストを無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
}

void DrawEnemyWeakDisp(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用

	for (int nCntEnemyWeakDisp = 0; nCntEnemyWeakDisp < MAX_ENEMYWEAKDISP; nCntEnemyWeakDisp++)
	{
		//使用している場合
		if (g_aEnemyWeakDisp[nCntEnemyWeakDisp].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld, NULL, &mtxView);	//逆行列を求める
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld._41 = 0.0f;
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld._42 = 0.0f;
			g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.x, g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.y, g_aEnemyWeakDisp[nCntEnemyWeakDisp].pos.z);
			D3DXMatrixMultiply(&g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld, &g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld, &mtxTrans);

			//===============================================================
			//描画の調整
			//==============================================================
			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			////Zの比較方法変更。
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			////Zバッファに書き込まない
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//アルファテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//===============================================================

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemyWeakDisp[nCntEnemyWeakDisp].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEnemyWeakDisp, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEnemyWeakDisp[g_aEnemyWeakDisp[nCntEnemyWeakDisp].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemyWeakDisp * 4, 2);

			//============================
			//描画の調整をもとに戻す
			//============================

			 //ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			////Zの比較方法変更。
			//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			////Zバッファに書き込む
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//アルファテストを無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		}
	}
}

//======================================================
// 敵のゲージをセットする
//======================================================
void SetEnemyGauge(int nNumEnemy, D3DXVECTOR3 pos)
{
	//使用されていない場合
	if (g_aEnemyHP[nNumEnemy].bUse == false)
	{
		g_aEnemyHP[nNumEnemy].pos = pos;	//位置
		g_aEnemyHP[nNumEnemy].bUse = true;	//使用する
	}
}

//=======================================================
// 敵の弱点表記をセットする
void SetEnemyWeakDisp(int nType, int nNumEnemy, D3DXVECTOR3 pos)
{
	if (g_aEnemyWeakDisp[nNumEnemy].bUse == false)
	{
		g_aEnemyWeakDisp[nNumEnemy].bUse = true;
		g_aEnemyWeakDisp[nNumEnemy].pos = pos;
		g_aEnemyWeakDisp[nNumEnemy].nType = nType;
	}
}

//======================================================
// 構造体の情報の取得
//======================================================
EnemyHP* GetEnemyGauge(void)
{
	return &g_aEnemyHP[0];
}

EnemyWeakDisp* GetEnemyWeakDisp(void)
{
	return &g_aEnemyWeakDisp[0];
}
