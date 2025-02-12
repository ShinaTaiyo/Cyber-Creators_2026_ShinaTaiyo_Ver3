//=======================================================================================
//
//６月４日：モデルを描画する[objectX.h]
//Author:ShinaTaiyo
//
//=======================================================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//==============================================
//インクルード
//==============================================
#include "main.h"
#include "object.h"
//=======================================================================================

//=======================================================
//オブジェクトXクラス
//=======================================================
class CObjectX : public CObject
{
public:
	static const int m_nMAX_MAT = 100;                                                      //マテリアルの最大数
	typedef enum
	{
		OBJECTXTYPE_BLOCK = 0,        //ブロック
		OBJECTXTYPE_MODEL,            //モデル
		OBJECTXTYPE_PLAYER,           //プレイヤー
		OBJECTXTYPE_ENEMY,            //敵
		OBJECTXTYPE_ITEM,             //アイテム
		OBJECTXTYPE_MARKER,           //マーカー
		OBJECTXTYPE_ATTACK,           //攻撃
		OBJECTXTYPE_BOSS,             //ボス
		OBJECTXTYPE_TRAP,             //トラップ
		OBJECTXTYPE_SIGNBOARD,        //立て看板
		OBJECTXTYPE_AIMODEL,          //AIモデル
		OBJECTXTYPE_MAX
	}OBJECTXTYPE;

	typedef enum
	{
		ORIGINPOSTYPE_SENTER = 0,
		ORIGINPOSTYPE_DOWN,
	    ORIGINPOSTYPE_MAX
	}ORIGINPOSTYPE;

	typedef enum
	{
		ROTTYPE_NORMAL = 0,    //普通の向き
		ROTTYPE_Z,             //Z方向に回転させる
		ROTTYPE_MAX          
	}ROTTYPE;

	typedef enum
	{
		MOVETYPE_NONE = 0,     //移動タイプ：無し
		MOVETYPE_TRUE,         //移動タイプ：する
		MOVETYPE_MAX
	}MOVETYPE;

	typedef enum
	{
		COLORTYPE_RED = 0,
		COLORTYPE_GREEN,
		COLORTYPE_BRUE,
		COLORTYPE_ALPHA,
		COLORTYPE_MAX
	}COLORTYPE;

	//=================================
    //モデル情報構造体
    //=================================
	typedef struct
	{
		LPD3DXMESH pMesh;                           //メッシュ（頂点情報）へのポインタ（モデルの種類数分）
		LPD3DXBUFFER pBuffMat;                      //マテリアルへのポインタ
		DWORD dwNumMat;                             //マテリアルの数
		LPDIRECT3DTEXTURE9* pTexture;               //テクスチャへのポインタ
		D3DCOLORVALUE* Diffuse;          //色合い
		D3DCOLORVALUE* FormarDiffuse;    //元の色合い
	}ObjectXInfo;
	//============================================================================================================

	CObjectX(int nPriority = 3);                                                           //コンストラクタ
	~CObjectX() override;                                                 //デストラクタ
	HRESULT Init() override;                                              //初期化処理
	void Uninit() override;                                               //終了処理
	void Update() override;                                               //更新処理
	void Draw() override;                                                 //描画処理
	void BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture,D3DCOLORVALUE * pDiffuse);//モデル情報を割り当てる

	void SetColor(COLORTYPE type,int nColChengeTime);                                       //色合いを設定する
	void SetColorAlphaOnly(float fAlpha);                                                   //色の透明度だけを設定する
	void SetOriginalColor(D3DXCOLOR col, int nColChengeTime);                               //オリジナルの色合いを設定する
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; }                                           //位置の設定
	void SetPosOld(D3DXVECTOR3 PosOld) { m_PosOld = PosOld; }                               //1f前の位置の設定
	void SetSupportPos(D3DXVECTOR3 SupportPos) { m_SupportPos = SupportPos; }               //設置位置を保存する
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; }                                           //向きの設定
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }                                   //拡大率の設定
	void SetFormarScale(D3DXVECTOR3 Scale) { m_FormarScale = Scale; }                       //元の拡大率を設定
	D3DXVECTOR3& GetFormarScale() { return m_FormarScale; }                                 //元の拡大率を取得
	D3DXVECTOR3 &GetPos() { return m_Pos; }                                                 //位置の取得
	D3DXVECTOR3* GetPosAddress() { return &m_Pos; }                                         //位置のアドレスを取得
	D3DXVECTOR3 &GetPosOld() { return m_PosOld;}                                            //1f前の位置を設定
	D3DXVECTOR3 & GetSupportPos() { return m_SupportPos; }                                  //設置位置を取得する
	D3DXVECTOR3 &GetRot() { return m_Rot; }                                                 //向きの取得
	D3DXVECTOR3 & GetScale() { return m_Scale; }                                            //拡大率の取得
	D3DXVECTOR3 & GetVtxMax() { return m_VtxMax; }                                          //各頂点最大値の取得
	D3DXVECTOR3 & GetVtxMin() { return m_VtxMin; }                                          //各頂点最小値の取得
	D3DXVECTOR3 & GetOriginVtxMax() { return m_OriginVtxMax; }                              //元の各頂点最大値の取得
	D3DXVECTOR3 & GetOriginVtxMin() { return m_OriginVtxMin; }                              //元の各頂点最小値の取得
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }                                       //移動量を設定する
	D3DXVECTOR3& GetMove() { return m_Move; }                                               //移動量を取得する
	D3DXVECTOR3 GetSize() { return m_Size; }                                                //Xオブジェクトのサイズを取得
	void SetMoveType(MOVETYPE movetype) { m_MoveType = movetype; }                          //移動タイプを設定する
	MOVETYPE GetMoveType() { return m_MoveType; }                                           //移動タイプを取得する
	void SetSize();                                                                         //サイズを設定する 
	void SetSizeAnother(D3DXVECTOR3 Size) { m_Size = Size;}                                 //直値でにサイズを設定する
	int GetManagerType() { return m_nManagerType; }//マネージャーで呼び出される際のタイプ番号
	void SetManagerType(int nManagerType) { m_nManagerType = nManagerType; }//マネージャーで呼び出される際のタイプ番号を設定する
	CObjectX::ObjectXInfo & GetObjectXInfo(){return m_ObjectXInfo;}
	//============================================
	//エディタ判定用
	//============================================
	void SetObjXType(OBJECTXTYPE ObjXType) { m_nObjXType = ObjXType; }                      //オブジェクトXの分類を設定
	OBJECTXTYPE GetObjXType() { return m_nObjXType; }                                       //オブジェクトXのタイプを取得する

	void SetTypeNum(int nTypeNum) { m_nTypeNum = nTypeNum; }                                //オブジェクトごとのタイプ番号を設定
	int GetTypeNum() { return m_nTypeNum;}                                                  //オブジェクトごとのタイプ番号を設定

	void SetRotType(ROTTYPE RecvRotType);                                                   //向きの種類        
	ROTTYPE& GetRotType() { return m_RotType; }                                             //向きの種類を取得

	virtual void DispInfo();                        //情報表示処理
	//================================================================================================================================================

	D3DXMATRIX &GetMatrixWorld() { return m_mtxWorld; }

	//============================================
	//調整用
	//============================================
	void SetUseInteria(bool bUseInteria) { m_bUseInteria = bUseInteria;}    //慣性をかけるかどうか
	float& GetInertia() { return m_fInertia; }                                              //慣性を取得する
	void SetInertia(float fInertia) { m_fInertia = fInertia; }                              //慣性を設定する
	void SetUndoInertia() { m_fInertia = m_fNORMAL_INERTIA; }                               //慣性の大きさを元に戻す
	bool& GetUseInteria() { return m_bUseInteria; }                                         //慣性を掛けるかどうかを取得
	//================================================================================================================================================

	void SetLanding(bool bLanding) { m_bIsLanding = bLanding; }                                                    //地面にいるかどうかを設定
	bool& GetLanding() { return m_bIsLanding; }                                                                    //地面にいるかどうかを取得

	void SetWalling(bool bWalling) { m_bIsWalling = bWalling; }                             //壁にくっついているかどうかを設定
	bool& GetWalling() { return m_bIsWalling; }                                             //壁にくっついているかどうかを取得

	void SetJumping(bool bJumping) { m_bIsJumping = bJumping; }                             //ジャンプしているかどうかを設定
	bool& GetJumping() { return m_bIsJumping; }                                             //ジャンプしているかどうかを取得

	void SetLiftLanding(bool bLiftLanding) { m_bIsLiftLanding = bLiftLanding; }             //リフトに乗っているかどうか
	bool& GetLiftLanding() { return m_bIsLiftLanding; }                                     //リフトに乗っているかどうかを取得
	CObject*& GetLandingObj() { return m_pLiftLandingObj; }                                  
	void SetLandingObj(CObject* pObj) { m_pLiftLandingObj = pObj; }                         //乗っているオブジェクトを設定

	//=============================
	//ペアレント関係
	//=============================
	void SetParentInfo(D3DXVECTOR3 ParentPos, D3DXVECTOR3 ParentRot, D3DXVECTOR3 ParentScale, bool bUse)//オフセット情報を設定
	{
		m_ParentSetPos = ParentPos; m_ParentSetRot = ParentRot; m_ParentSetScale = ParentScale; m_bUseParent = bUse;
	}
	//void SetUseParent(D3DXMATRIX * mtxWorld){ m_ParentMtxWorld = mtxWorld;}
	D3DXVECTOR3& GetParentPos() { return m_ParentSetPos; }         //親の位置を取得
	D3DXVECTOR3& GetParentRot() { return m_ParentSetRot; }         //親の向きを取得
	D3DXVECTOR3& GetParentScale() { return m_ParentSetScale; }     //親の拡大率を取得
	bool GetUseParent() { return m_bUseParent; }                   //親を使用するかどうか

	D3DXVECTOR3& GetOffSetPos() { return m_OffSetPos; }            //オフセット位置を取得
	D3DXVECTOR3& GetOffSetRot() { return m_OffSetRot; }            //オフセット向きを取得
	D3DXVECTOR3& GetOffSetScale() { return m_OffSetScale; }        //オフセット拡大率を取得


	//=================================================================================================================

	//======================================================
	//向き加算関係
	//======================================================
	void SetUseAddRot(bool bUse, D3DXVECTOR3 Rot) { m_bUseAddRot = bUse; m_AddRot = Rot;}
	bool& GetUseAddRot() { return m_bUseAddRot; }//向きの加算を使用しているかどうか
	D3DXVECTOR3& GetAddRot() { return m_AddRot; }//向きの加算量を取得
	//======================================================
	//体力関係
	//======================================================
	void SetLife(int nLife) { m_nLife = nLife; }                    //体力を設定する
	void SetMaxLife(int nMaxLife) { m_nMaxLife = nMaxLife; }        //最大体力を設定する
	int& GetLife() { return m_nLife; }                              //体力を取得する
	int& GetMaxLife() { return m_nMaxLife; }                        //最大体力を取得する
	bool& GetHitStop() { return m_bHitStop; }                       //ヒットストップ状態かどうかを取得する
	void SetAutoSubLife(bool bUse) { m_bAutoSubLife = bUse; }       //体力を自動的に減らすかどうかを設定
	void SetHitStopTime(int nHitStopTime) { m_nHitStopTime = nHitStopTime; }//ヒットストップ時間を設定する
	int& GetHitStopTime() { return m_nHitStopTime; }                //ヒットストップ時間を取得する
	virtual void SetDamage(int nDamage, int nHitStopTime);          //ダメージを与える
	virtual void SetHeal(int nHeal,D3DXCOLOR col,float fWidth,float fHeight);                  //回復量を与える
	//=================================================================================================================

    //===================================
	//重力関係
	//===================================
	void SetNotUseGravityTime(int nTime) { m_nNotUseGravityTime = nTime; m_bTimeChengeGravity = true; }//重力を使わない時間を設定
	void SetUseGravity(bool bUse) { m_bUseGravity = bUse; }               //重力を使用するかどうかを設定
	bool& GetUseGravity() { return m_bUseGravity; }                       //重力を使用するかどうかを設定
	//=================================================================================================================

	//===================================
	//ブロック当たり判定系
	//===================================
	void SetUseBlockCollision(bool bUse) { m_bUseBlockCollision = bUse;}                    //ブロックとの当たり判定を使用するかどうか
	bool& GetUseBlockCollision() { return m_bUseBlockCollision; }                           //ブロックとの当たり判定を使用するかどうか
	bool& GetSuccessBlockCollisionX() { return m_bSuccessBlockCollisionX; }                 //ブロックとのX方向の当たり判定が成功したかどうか
	bool& GetSuccessBlockCollisionY() { return m_bSuccessBlockCollisionY; }                 //ブロックとのY方向の当たり判定が成功したかどうか
	bool& GetSuccessBlockCollisionZ() { return m_bSuccessBlockCollisionY; }                 //ブロックとのZ方向の当たり判定が成功したかどうか
	//=================================================================================================================

	//===================================
	//テクスチャ関係
	//===================================
	void SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture);//狙ったマテリアルのテクスチャを変える
	//=================================================================================================================

	//===================================
	//召喚関係
	//===================================
	void SetSummonObjectDeath(bool bDeath) { m_bSummonObjectDeath = bDeath; }               //召喚したオブジェクトを消すフラグ
	bool& GetSummonObjectDeath() { return m_bSummonObjectDeath; }                           //召喚したオブジェクトを消すかどうかを取得
	//=================================================================================================================

	//===================================
	//影関係
	//===================================
	void SetUseShadow(bool bUse) { m_bUseShadow = bUse; }
	//=================================================================================================================

	void SetUseGravity(float fGravityPower);                //重力を使用するかどうか

	void SetUseAddScale(D3DXVECTOR3 AddScale, bool bUse);              //拡大率の加算を使用するかどうか

	//===================================
	//中心点関係
	//===================================
	D3DXVECTOR3& GetSenterPos() { return m_SenterPos;}//中心点を取得する
	//=================================================================================================================

	//===================================
	//色合い関係
	//===================================
	void SetUseRatioLifeAlpha(bool bUse) { m_bUseRatioLifeAlpha = bUse; }//体力の割合でアルファ値を変えるかどうか
	//=================================================================================================================

	//===================================
	//回避関係
	//===================================
	bool& GetDodgeState() { return m_bDodge; }//回避状態かどうか
	//=================================================================================================================


	void SetUseMultiSpeed(bool bUse, D3DXVECTOR3 MultiSpeed) { m_MultiSpeed = MultiSpeed; m_bUseMultiSpeed = bUse; }//乗算加速を使用するかどうか

	void SetUseAddSpeed(bool bUse, D3DXVECTOR3 AddSpeed) { m_bUseAddSpeed = bUse; m_AddSpeed = AddSpeed; }//加速を使用するかどうか

	//===================================
	//位置更新関係
	//===================================
	void SetUseUpdatePos(bool bUse) { m_bUseUpdatePos = bUse; }//位置の更新を使用するかどうか
	void SetUseAdjustOriginPos(bool bUse) { m_bUseAdjustOriginPos = bUse; }//原点に到達したときに位置の補正を行うかどうか
	//=================================================================================================================

	//===================================
	//描画関係
	//===================================
	void SetUseDraw(bool bUse) { m_bUseDraw = bUse; }
	//=================================================================================================================

protected:
	static const float GetNormalGravity() { return m_fNORMAL_GRAVITY; }//基本の重力を取得する
	virtual void GravityProcess();                  //重力の処理
	void AdjusOriginPosProcess();                   //原点に到達したときに位置の補正を行うかどうか
	void CommonBlockCollision();//共通のブロックとの当たり判定

private:
	//===============================================
	//静的メンバ
	//===============================================
	static constexpr float m_fNORMAL_INERTIA = 0.5f;//普通の慣性
	static constexpr float m_fNORMAL_GRAVITY = 1.0f;//普通の重力
	//=================================================================================================================

	void DrawShadow(D3DXVECTOR3 Pos);                                                       //影を描画する
	D3DXVECTOR3 m_Pos;                              //位置
	D3DXVECTOR3 m_SupportPos;                       //最初に設置された位置
	D3DXVECTOR3 m_PosOld;                           //1f前の位置
	D3DXVECTOR3 m_Rot;                              //向き
	D3DXVECTOR3 m_Scale;                            //大きさ
	D3DXVECTOR3 m_FormarScale;                      //元の拡大率
	D3DXVECTOR3 m_Size;                             //サイズ
	D3DXVECTOR3 m_VtxMin;                           //それぞれの座標のモデルの位置の最小値
	D3DXVECTOR3 m_OriginVtxMin;                     //元々の最小値
	D3DXVECTOR3 m_VtxMax;                           //それぞれの座標のモデルの位置の最大値
	D3DXVECTOR3 m_OriginVtxMax;                     //元々の最大値
	D3DXVECTOR3 m_Move;                             //移動量
	D3DXMATRIX m_mtxWorld;                          //マトリックスワールド
	MOVETYPE m_MoveType;                            //移動タイプ
	DWORD m_SizeFVF;                                //頂点フォーマットのサイズ
	BYTE* m_pVtxBuff;                               //頂点バッファへのポインタ
	ObjectXInfo m_ObjectXInfo;                      //モデル情報の構造体

	int m_nNumVtx;                                  //頂点数
	int m_nNumModel;                                //何番目のモデルか？
	int m_nIdxShadow;                               //影のインデックス
	int m_nManagerType;                             //マネージャーで呼び出される時のタイプ番号

	//===================================
	//位置更新関係
	//===================================
	bool m_bUseUpdatePos;                           //位置の更新を使用するかどうか
	void UpdatePos();                               //位置を更新させる
	bool m_bUseAdjustOriginPos;                     //原点に到達下ときに位置の補正を行うかどうか

	//===================================
	//体力関係
	//===================================
	int m_nLife;                                    //体力
	int m_nMaxLife;                                 //最大体力

	bool m_bHitStop;                                //ヒットストップ
	int m_nHitStopTime;                             //ヒットストップ時間

	bool m_bAutoSubLife;                            //自動的に体力を減らし続けるかどうか

	//============================================================================================================

	//===================================
	//影関係
	//===================================
	bool m_bUseShadow;//影を使用するかどうか
	//=================================================================================================================


	//===================================
	//重力関係
	//===================================
	bool m_bUseGravity;                             //重力を使用するかどうか
	int m_nNotUseGravityTime;                       //重力を使わない時間
	bool m_bTimeChengeGravity;                      //重力のON、OFFを時間で変えるかどうか
	//============================================================================================================

	//===================================
	//召喚関係
	//===================================
	bool m_bSummonObjectDeath;                       //オブジェクトXが召喚したオブジェクトを消すフラグ
	//============================================================================================================

	//===================================
	//描画関係
	//===================================
	bool m_bUseDraw;
	//============================================================================================================

	//===================================
	//回転関係
	//===================================
	D3DXVECTOR3 m_AddRot;//加算する向き
	bool m_bUseAddRot;   //向きの加算を使用するかどうか
	//=================================================================================================================

	float m_fInertia;                               //慣性
	int m_nChengeColorTime;                         //色を変える時間
	bool m_bColorChenge;                            //色を変えているかどうか
	bool m_bUse;                                    //使用しているかどうか
	bool m_bUseInteria;                             //慣性をかけるかどうか

	bool m_bIsLanding;                              //地面にいるかどうか
	bool m_bIsWalling;                              //壁にくっついているかどうか
	bool m_bIsLiftLanding;                          //リフトの上に乗っているかどうか
	bool m_bIsJumping;                              //ジャンプしているかどうか

	CObject* m_pLiftLandingObj;                       //乗っているリフトを格納する

	//===================================
	//ペアレント系
	//===================================
	D3DXVECTOR3 m_ParentSetPos;                          //親の位置
	D3DXVECTOR3 m_ParentSetRot;                          //親の向き
	D3DXVECTOR3 m_ParentSetScale;                        //親の拡大率

	D3DXVECTOR3 m_OffSetPos;                             //オフセットの位置
	D3DXVECTOR3 m_OffSetRot;                             //オフセットの向き
	D3DXVECTOR3 m_OffSetScale;                           //オフセットの拡大率

	D3DXMATRIX m_ParentMtxWorld;                         //親のマトリックスワールド
	bool m_bUseParent;                                   //親を使用するかどうか
	//============================================================================================================

	//===================================
	//書き出しの時に使う必須情報
	//===================================
	int m_nTypeNum;                                 //モデルごとのタイプ番号
	ROTTYPE m_RotType;                              //向き
	OBJECTXTYPE m_nObjXType;                        //オブジェクトXの分類
	//============================================================================================================

	//===================================
	//ブロック当たり判定系
	//===================================
	bool m_bUseBlockCollision;                                            //ブロックとの当たり判定を使用するかどうか
	bool m_bSuccessBlockCollisionX;                                       //X方向の当たり判定が成功したかどうか
	bool m_bSuccessBlockCollisionY;                                       //Y方向の当たり判定が成功したかどうか
	bool m_bSuccessBlockCollisionZ;                                       //Z方向の当たり判定が成功したかどうか
	//=================================================================================================================

	//===================================
	//中心点関係
	//===================================
	D3DXVECTOR3 m_SenterPos;
	void CalculateSenterPos();
	//=================================================================================================================

	//===================================
	//回避関係
	//===================================
	bool m_bDodge;//回避状態かどうか
	//=================================================================================================================

	//===================================
	//色合い関係
	//===================================
	bool m_bUseRatioLifeAlpha;//体力の割合で透明度を変えるかどうか
	//=================================================================================================================

	//===================================
	//関数
	//===================================
	void SetFormarColor();            //元の色合いに戻す 
	void OffSetUpdate();              //オフセットの情報を更新する
	void ChengeGravityProcess();      //重力を変える処理
	void HitStopProcess();            //ヒットストップの処理
    //=================================================================================================================

	bool m_bUseMultiSpeed;                                  //乗算加速をするかどうか
	D3DXVECTOR3 m_MultiSpeed;                               //乗算加速度

	bool m_bUseAddSpeed;                                    //加速を使用するかどうか
	D3DXVECTOR3 m_AddSpeed;                                 //加速度

	float m_fGravityPower;                                  //重力の大きさ

	bool m_bUseAddScaling;                                  //拡大率を使用するかどうか
	D3DXVECTOR3 m_AddScale;                                 //加算する拡大率を設定

	//===================================
	//モデル情報関係
	//===================================
	int m_nModelInfoIdx;//モデル情報のアドレス

	//===================================
	//エディタ系
	//===================================
	void ChengeEditScale();             //拡大率を変更する
	void ChengeEditScaleX();
	void ChengeEditScaleY();
	void ChengeEditScaleZ();

	void ChengeEditPos();               //位置を移動する
};
//============================================================================================================
#endif