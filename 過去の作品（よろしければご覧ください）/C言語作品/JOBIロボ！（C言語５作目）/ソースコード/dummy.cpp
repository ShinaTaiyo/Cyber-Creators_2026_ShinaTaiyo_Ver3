//================================================================================================================================================================================================================================
//
// ダミー人形の処理
// Author : Atsumu Kuboichi
//
//================================================================================================================================================================================================================================
#include "dummy.h"
#include "player.h"
#include "particle.h"
#include "sound.h"
//================================================================================================================================================================================================================================
// グローバル変数
//================================================================================================================================================================================================================================
Dummy g_aDummy;	//構造体情報

//================================================================================================================================================================================================================================
// 初期化処理
//================================================================================================================================================================================================================================
void InitDummy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	BYTE* pVtxBuff;							//頂点バッファへのポインタ

	//構造体の情報
	g_aDummy.pos = D3DXVECTOR3(-120.0f, 0.0f, 120.0f);	//位置
	g_aDummy.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	g_aDummy.nLife = DUMMY_LIFE;						//体力
	g_aDummy.nRespawn = (int)(60.0f * RESPAWN_TIME);	//リスポーン時間
	g_aDummy.bUse = true;								//使用状態
	g_aDummy.nRespawnTime = 0;                          //リスポーン時間をカウントする

	for (int nCntAttack = 0; nCntAttack < PLAYERATTACK_MAX; nCntAttack++)
	{
		g_aDummy.bHitStop[nCntAttack] = false;
		g_aDummy.nHitStopTime[nCntAttack] = 0;
	}

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\OBJECT\\dummy000\\dummy000.x", D3DXMESH_SYSTEMMEM, pDevice, NULL,
		&g_aDummy.pBuffMat, NULL, &g_aDummy.dwNumMat, &g_aDummy.pMesh);

	//頂点数の取得
	int nNumVtx = g_aDummy.pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	DWORD sizeFVF = D3DXGetFVFVertexSize(g_aDummy.pMesh->GetFVF());

	//頂点バッファのロック
	g_aDummy.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//モデルの最小値と最大値を取得
		if (vtx.x < g_aDummy.vtxMin.x)
		{//X座標の最小値
			g_aDummy.vtxMin.x = vtx.x;
		}
		if (vtx.y < g_aDummy.vtxMin.y)
		{//Y座標の最小値
			g_aDummy.vtxMin.y = vtx.y;
		}
		if (vtx.z < g_aDummy.vtxMin.z)
		{//Z座標の最小値
			g_aDummy.vtxMin.z = vtx.z;
		}
		if (vtx.x > g_aDummy.vtxMax.x)
		{//X座標の最大値
			g_aDummy.vtxMax.x = vtx.x;
		}
		if (vtx.y > g_aDummy.vtxMax.y)
		{//Y座標の最大値
			g_aDummy.vtxMax.y = vtx.y;
		}
		if (vtx.z > g_aDummy.vtxMax.z)
		{//Z座標の最大値
			g_aDummy.vtxMax.z = vtx.z;
		}

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	g_aDummy.Size = g_aDummy.vtxMax - g_aDummy.vtxMin;

	//頂点バッファのアンロック
	g_aDummy.pMesh->UnlockVertexBuffer();
}

//================================================================================================================================================================================================================================
// 終了処理
//================================================================================================================================================================================================================================
void UninitDummy(void)
{
	//メッシュの破棄
	if (g_aDummy.pMesh != NULL)
	{	
		g_aDummy.pMesh->Release();
		g_aDummy.pMesh = NULL;
	}
	//マテリアルの破棄
	if (g_aDummy.pBuffMat != NULL)
	{
		g_aDummy.pBuffMat->Release();
		g_aDummy.pBuffMat = NULL;
	}
}

//================================================================================================================================================================================================================================
// 更新処理
//================================================================================================================================================================================================================================
void UpdateDummy(void)
{
	if (g_aDummy.bUse == true)
	{
		HitStopDummy();
	}

	if (g_aDummy.bUse == false)
	{
		g_aDummy.nRespawnTime++;

		if (g_aDummy.nRespawnTime >= g_aDummy.nRespawn)
		{
			g_aDummy.bUse = true;
			g_aDummy.nRespawnTime = 0;
			g_aDummy.nLife = DUMMY_LIFE;
		}
	}
}

//================================================================================================================================================================================================================================
// 描画処理
//================================================================================================================================================================================================================================
void DrawDummy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9 matDef;					//現在のマテリアル保存用
	D3DXMATERIAL* pMat;						//マテリアルのデータへのポインタ

	//使用されている場合
	if (g_aDummy.bUse == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aDummy.mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aDummy.rot.y, g_aDummy.rot.x, g_aDummy.rot.z);
		D3DXMatrixMultiply(&g_aDummy.mtxWorld, &g_aDummy.mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aDummy.pos.x, g_aDummy.pos.y, g_aDummy.pos.z);
		D3DXMatrixMultiply(&g_aDummy.mtxWorld, &g_aDummy.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aDummy.mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aDummy.pBuffMat->GetBufferPointer();

		for (int nCntDummy2 = 0; nCntDummy2 < (int)g_aDummy.dwNumMat; nCntDummy2++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntDummy2].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			//モデル(パーツ)の描画
			g_aDummy.pMesh->DrawSubset(nCntDummy2);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//===================================
//構造体の情報を取得
//===================================
Dummy* GetDummy(void)
{
	return &g_aDummy;
}


//===================================
//ダミーにダメージを与える
//===================================
void DamageDummy(int nDamage, PLAYERATTACK PlayerAttack)
{
	if (g_aDummy.bHitStop[PlayerAttack] == false)//ヒットストップ状態じゃなければ
	{
		g_aDummy.nLife -= nDamage;//体力を減らす
		g_aDummy.bHitStop[PlayerAttack] = true;  //ダメージを与えた攻撃に対してヒットストップ状態にする

		for (int nCntParticle = 0; nCntParticle < 100; nCntParticle++)
		{
			float fRot = float(rand() % 200 + 1) / 100;
			float fYmove = float(rand() % 200 - 100) / 10;
			float fSpeed = float(rand() % 200 - 100) / 10;

			SetParticle(PARTICLETYPE_NORMAL, 300, 10.0f, 0.6f, g_aDummy.pos,
				D3DXVECTOR3(sinf(-D3DX_PI * fRot) * fSpeed, fYmove, cosf(-D3DX_PI * fRot) * fSpeed),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, PARTICLETEX_NORMAL,0);
		}

		//========================
		//効果音処理
		//========================
		if (PlayerAttack == PLAYERATTACK00_SWORD)
		{
			PlaySound(SOUND_LABEL_SE_SWORDSLASH_000);
		}
		if (PlayerAttack == PLAYERATTACK01_BOOMERANG)
		{
			PlaySound(SOUND_LABEL_SE_HITBOOMERANG_000);
		}
		if (PlayerAttack == PLAYERATTACK02_SPEAKER)
		{
			PlaySound(SOUND_LABEL_SE_SPEAKERWAVEHIT_000);
		}

		if (g_aDummy.nLife <= 0)
		{
			g_aDummy.bUse = false;
		}
	}
}

//================================
//ヒットストップを行う
//================================
void HitStopDummy(void)
{
	if (g_aDummy.bUse == true)
	{
		for (int nCntAttack = 0; nCntAttack < PLAYERATTACK_MAX; nCntAttack++)
		{
			if (g_aDummy.bHitStop[nCntAttack] == true)
			{
				g_aDummy.nHitStopTime[nCntAttack]++;
				switch (nCntAttack)
				{
				case PLAYERATTACK00_SWORD:

					if (g_aDummy.nHitStopTime[nCntAttack] == 20)
					{
						g_aDummy.bHitStop[nCntAttack] = false;
						g_aDummy.nHitStopTime[nCntAttack] = 0;
					}

					break;
				case PLAYERATTACK01_BOOMERANG:
					if (g_aDummy.nHitStopTime[nCntAttack] == 10)
					{
						g_aDummy.bHitStop[nCntAttack] = false;
						g_aDummy.nHitStopTime[nCntAttack] = 0;

					}
					break;
				case PLAYERATTACK02_SPEAKER:
					if (g_aDummy.nHitStopTime[nCntAttack] == 1)
					{
						g_aDummy.bHitStop[nCntAttack] = false;
						g_aDummy.nHitStopTime[nCntAttack] = 0;

					}
					break;
				}
			}
		}
	}
}
