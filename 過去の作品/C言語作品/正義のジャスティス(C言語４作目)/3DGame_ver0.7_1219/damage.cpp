#include "Damage.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "shadow.h"
#include "effect.h"
#include "wall.h"
#include "particle.h"
#include "player.h"

#define MAX_DAMAGE (1024)//ダメージ表記の最大数
#define DAMAGE_DIGIT (3) //一回当たりのダメージの最大桁数
#define DAMAGE_SCALE_X (10)
#define DAMAGE_SCALE_Y (5)

#define DAMAGE_RADIUS (10.0f)

#define Damage_SCALE (10.0f)
//グローバル変数
PDIRECT3DVERTEXBUFFER9 g_pVtxBuffDamage = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureDamage = NULL;//テクスチャへのポインタ
D3DXVECTOR3 g_posDamage;//位置
//D3DXVECTOR3 g_rotDamage;//向き
D3DXMATRIX g_mtxWorldDamage;//ワールドマトリックス(float4*4)

Damage g_aDamage[MAX_DAMAGE][DAMAGE_DIGIT];


//タイトル画面の初期化処理
void InitDamage(void)
{


	//bFadeFlag = false;

	//g_nCntTime = 0;

	//g_nColorChange = 255;//カラーの初期化（白色）

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,
		"data\\number_001.png",
		&g_pTextureDamage);

	g_posDamage = D3DXVECTOR3(0.0f, 50.0f, 100.0f);//位置の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_DAMAGE * DAMAGE_DIGIT,//頂点の数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDamage,
		NULL);

	//スコア情報の初期化
	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		for (int nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{
			g_aDamage[nCntDamage][nCntDigit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aDamage[nCntDamage][nCntDigit].bUse = false;//使用していない状態にする

			g_aDamage[nCntDamage][nCntDigit].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU = 0;
			g_aDamage[nCntDamage][nCntDigit].nLife = 0;
			g_aDamage[nCntDamage][nCntDigit].fDamageWrite = 0.0f;
			g_aDamage[nCntDamage][nCntDigit].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}




	////タイトル画面BGMの設定
	//PlaySound(SOUND_LABEL_BGM000);

	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDamage->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE * DAMAGE_DIGIT; nCntDamage++)
	{
		//頂点座標の設定（初期位置）
		pVtx[0].pos = D3DXVECTOR3(-Damage_SCALE, Damage_SCALE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(Damage_SCALE, Damage_SCALE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-Damage_SCALE, -Damage_SCALE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(Damage_SCALE, -Damage_SCALE, 0.0f);

		//norの設定（初期設定）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。

		//テクスチャ座標の設定（初期設定）
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;//ポインタを進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffDamage->Unlock();
}

//==============================
//タイトル画面の終了処理
//==============================
void UninitDamage(void)
{
	//テクスチャの破棄
	if (g_pTextureDamage != NULL)
	{
		g_pTextureDamage->Release();
		g_pTextureDamage = NULL;
	}
	//}
	//頂点バッファの破棄
	if (g_pVtxBuffDamage != NULL)
	{
		g_pVtxBuffDamage->Release();
		g_pVtxBuffDamage = NULL;
	}
}
//============================
//タイトル画面の更新処理
//============================
void UpdateDamage(void)
{
	VERTEX_3D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDamage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		//頂点座標の設定

		for (int nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{
			if (g_aDamage[nCntDamage][nCntDigit].bUse == true)
			{

				g_aDamage[nCntDamage][nCntDigit].nLife -= 1;//ダメージ表示時間を減らし続ける
				g_aDamage[nCntDamage][nCntDigit].a_col -= 0.01f;//アルファ値を変え続け、どんどんダメージ表記を透明にしていく処理

				pVtx[0].pos = D3DXVECTOR3(-g_aDamage[nCntDamage][nCntDigit].fRadius - (g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit),+g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(+g_aDamage[nCntDamage][nCntDigit].fRadius - (g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit),+g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aDamage[nCntDamage][nCntDigit].fRadius - (g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit),-g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(+g_aDamage[nCntDamage][nCntDigit].fRadius - (g_aDamage[nCntDamage][nCntDigit].fRadius * nCntDigit),-g_aDamage[nCntDamage][nCntDigit].fRadius, 0.0f);

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aDamage[nCntDamage][nCntDigit].a_col);//D3DXCOLORは、0.0f～1.0fの間の数値で色を変える。


				//ダメージの動き
				g_aDamage[nCntDamage][nCntDigit].pos += g_aDamage[nCntDamage][nCntDigit].move;

				if (g_aDamage[nCntDamage][nCntDigit].nLife < 1)
				{
					g_aDamage[nCntDamage][nCntDigit].bUse = false;
				}
			}
			pVtx += 4;//頂点データのポインタを４つ分進める。
		}
	}
	

	//頂点バッファをアンロックする
	g_pVtxBuffDamage->Unlock();
}



//================================
//タイトル画面の描画処理
//================================
void DrawDamage(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

		//デバイスの取得
	pDevice = GetDevice();



	for (int nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		for (int nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{
			if (g_aDamage[nCntDamage][nCntDigit].bUse == true)
			{
				D3DXMATRIX mtxTrans;//計算用マトリックス

				D3DXMATRIX mtxView;//ビューマトリックス取得用

				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_mtxWorldDamage);

				//ビューマトリックスを取得
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				//ポリゴンをカメラに対して正面に向ける
				D3DXMatrixInverse(&g_mtxWorldDamage, NULL, &mtxView);

				//逆行列を求める
				g_mtxWorldDamage._41 = 0.0f;
				g_mtxWorldDamage._42 = 0.0f;
				g_mtxWorldDamage._43 = 0.0f;

				////向きを反映
				//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotDamage.y, g_rotDamage.x, g_rotDamage.z);

				//D3DXMatrixMultiply(&g_mtxWorldDamage, &g_mtxWorldDamage, &mtxRot);

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aDamage[nCntDamage][nCntDigit].pos.x, g_aDamage[nCntDamage][nCntDigit].pos.y, g_aDamage[nCntDamage][nCntDigit].pos.z);
				D3DXMatrixMultiply(&g_mtxWorldDamage, &g_mtxWorldDamage, &mtxTrans);

				//=============================
				//描画の調整
				//==============================================================
				//ライトを無効にする
				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

				//Zの比較方法変更。
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

				//Zバッファに書き込まない
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

				//アルファテストを有効
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

				//===============================================================

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldDamage);


				//頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_pVtxBuffDamage, 0, sizeof(VERTEX_3D));//頂点バッファへのポインタと頂点情報の構造体のサイズ

				//頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureDamage);

				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDigit * 4 + (DAMAGE_DIGIT * 4) * nCntDamage, 2);//0は、描画する最初の頂点インデックスで、2は、描画するプリミティブ

				//============================
				//描画の調整をもとに戻す
				//============================

				//ライトを有効に戻す
				pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

				//Zの比較方法変更。
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

				//Zバッファに書き込む
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

				//アルファテストを無効に戻す
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

				//=========================================================
			}
		}
	}
}

//スコアの設定処理
void SetDamage(int nDamage, int nLife, D3DXVECTOR3 pos, float fRadius, D3DXCOLOR col)//posは「発射位置」、moveは「移動量」
{

	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_3D* pVtx;

	Player* pPlayer = GetPlayer();


	int nCntDamage;
	int nCntDigit;

	if (pPlayer->bAttackUp == true)
	{
		nDamage *= 2;
	}

	int nGetDamage = nDamage;



	int nCnt;
	int nCntDamageDigit = 0;//もしダメージ表記が使用されていなかったら、ダメージ表記の桁数分のカウントを開始する。
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffDamage->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		float fRandumRot = float(rand() % 200 + 1) / 100;
		float fRandumSpeed = float(rand() % 100 + 30) / 10;
		float fRandumMove = float(rand() % 21 - 10);

		for (nCntDigit = 0; nCntDigit < DAMAGE_DIGIT; nCntDigit++)
		{
			if (g_aDamage[nCntDamage][nCntDigit].bUse == false)
			{

				g_aDamage[nCntDamage][nCntDigit].bUse = true;
				g_aDamage[nCntDamage][nCntDigit].nLife = nLife;
				g_aDamage[nCntDamage][nCntDigit].pos = pos;
				g_aDamage[nCntDamage][nCntDigit].fRadius = fRadius;
				g_aDamage[nCntDamage][nCntDigit].a_col = 0.8f;
				g_aDamage[nCntDamage][nCntDigit].col = col;

			
				g_aDamage[nCntDamage][nCntDigit].move = D3DXVECTOR3(sinf(-D3DX_PI * fRandumRot) * fRandumSpeed, fRandumMove, cosf(-D3DX_PI * fRandumRot) * fRandumSpeed);

				/*	for (nCnt = 0; nCnt < DAMAGE_DIGIT; nCnt++)
					{*/

				//g_aDamage[nCntDamage][nCntDigit].pos += (fRadius * 2) * nCntDigit;//ダメージ表記の位置を桁数ごとにずらす

				g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU = nGetDamage % 10;
				nGetDamage = nGetDamage / 10;
				//}



			/*	for (nCntDigit= 0;  nCntDigit< DAMAGE_DIGIT; nCntDigit++)
				{*/
				pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU),0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU),0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU),1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aDamage[nCntDamage][nCntDigit].nDamagePosTexU),1.0f);

				nCntDamageDigit++;
			}
			if (nCntDamageDigit >= DAMAGE_DIGIT)
			{
				break;//for文の終了
			}
			pVtx += 4;//頂点データのポインタを４つ分進める。

		}
		if (nCntDamageDigit >= DAMAGE_DIGIT)
		{
			break;//for文の終了
		}


	}
	//頂点バッファをアンロックする
	g_pVtxBuffDamage->Unlock();

	/*for (nCntDamage = 0; nCntDamage < MAX_DAMAGE; nCntDamage++)
	{
		SetScore(10);
	}*/
}

Damage* GetDamage(void)//弾構造体の情報をゲットする
{
	return &g_aDamage[0][0];
}

