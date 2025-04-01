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

	//=================================
	//描画情報構造体
	//=================================
	struct DrawInfo
	{
		//影
		bool bUseShadow = true;//影を使用するかどうか!

		//描画を使用するかどうか
		bool bUseDraw = true;//!

		//カリングするかどうか
		bool bUseCulling = false;

		//子マトリックスにするかどうか
		bool bUseMatrixChild = false;

		//色
		int nChengeColorTime = 0;                    //色を変える時間!
		bool bColorChenge = false;                   //色を変えているかどうか!
		bool bBlinkingColor = false;                 //色を点滅させるかどうか

		//ワールド変換行列
		D3DXMATRIX mtxWorld;                     //マトリックスワールド!
	    D3DXMATRIX * pMtxParent = nullptr;            //親マトリックス 
		D3DXCOLOR Color = { 1.0f,1.0f,1.0f,1.0f };    //現在の統一の色合い

		//影
		void SetUseShadow(bool bUse) { bUseShadow = bUse; }
		const bool& GetUseDraw() const { return bUseDraw; }

		//描画するかどうか
		void SetUseDraw(bool bUse) { bUseDraw = bUse; }

		//子マトリックスにするかどうか
		void SetUseMatrixChild(bool bUse,D3DXMATRIX * CopyMatrixParent) { bUseMatrixChild = bUse; pMtxParent = CopyMatrixParent; }

		//カリングするかどうか
		void SetUseCulling(bool bUse) { bUseCulling = bUse; }

		//色を指定時間変え続ける処理
		void ChengeColorProcess(CObjectX * pObjX);

        //ワールド変換行列
		void SetMtxParent(D3DXMATRIX* pCopyMtxParent) {pMtxParent = pCopyMtxParent; }              //親マトリックスを取得                   
		D3DXMATRIX& GetMatrixWorld() { return mtxWorld; }                     //ワールド変換行列の取得
		//================================================================================================================================================
	};
	//============================================================================================================

	struct PosInfo
	{
		//==========================================================
        //位置
        //==========================================================
		D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };                              //位置
		D3DXVECTOR3 PosOld = { 0.0f,0.0f,0.0f };                           //1f前の位置
		D3DXVECTOR3 PosFuture = { 0.0f,0.0f,0.0f };                        //1f後の位置
		D3DXVECTOR3 SupportPos = { 0.0f,0.0f,0.0f };                       //最初に設置された位置
		D3DXVECTOR3 WorldPos = { 0.0f,0.0f,0.0f };                         //ワールド座標を求める
		D3DXVECTOR3 SenterPos = { 0.0f,0.0f,0.0f };                        //中心点の位置
		//=================================================================================================================
    
	    //位置
		void SetPos(D3DXVECTOR3 CopyPos) { Pos = CopyPos; }                                   //位置の設定
		const D3DXVECTOR3& GetPos() const { return Pos; }                                     //位置の取得

		//1f前の位置
		void SetPosOld(D3DXVECTOR3 CopyPosOld) { PosOld = CopyPosOld; }                       //1f前の位置の設定
		const D3DXVECTOR3& GetPosOld() const { return PosOld; }                               //1f前の位置を設定

		//初期位置
		void SetSupportPos(D3DXVECTOR3 CopySupportPos) { SupportPos = CopySupportPos; }       //設置位置を設定する
		D3DXVECTOR3& GetSupportPos() { return SupportPos; }                                   //設置位置を取得する

		//中心点の位置
		D3DXVECTOR3& GetSenterPos() { return SenterPos; }                                     //中心点を取得する

		//1f後の位置
		void SetPosFuture(D3DXVECTOR3 CopyPosFuture) { PosFuture = CopyPosFuture; }           //1f後の位置を設定する
		const D3DXVECTOR3& GetPosFuture() const { return PosFuture; }                         //1f後の位置を取得する

		//ワールド座標
		const D3DXVECTOR3& GetWorldPos() const { return WorldPos; }                           //ワールド座標を取得する（左のcosntは呼び出し先で値を変更出来なくし、
		                                                                                      //右のconstは、関数内で値を変更出来なくする

		//============================================================================================================
	};

	struct RotInfo
	{
		//変数
		D3DXVECTOR3 Rot = {0.0f,0.0f,0.0f};                              //向き!
		D3DXVECTOR3 AddRot = { 0.0f,0.0f,0.0f};                          //加算する向き!
		D3DXVECTOR3 RotAim = { 0.0f,0.0f,0.0f};                          //目的の向き 
		bool bUseAddRot = false;   //向きの加算を使用するかどうか!

		//関数
		void SetRot(D3DXVECTOR3 CopyRot) { Rot = CopyRot; }                                           //向きの設定
		D3DXVECTOR3& GetRot() { return Rot; }                                                         //向きの取得

		void SetUseAddRot(bool bUse, D3DXVECTOR3 CopyRot) { bUseAddRot = bUse; AddRot = CopyRot; }    //向きの加算を設定
		bool& GetUseAddRot() { return bUseAddRot; }                                                   //向きの加算を使用しているかどうか
		D3DXVECTOR3& GetAddRot() { return AddRot; }                                                   //向きの加算量を取得

		void SetRotAim(D3DXVECTOR3 CopyRotAim) { RotAim = CopyRotAim; }                               //目的の向きを設定
		const D3DXVECTOR3& GetRotAim() const { return RotAim; }                                       //目的の向きを取得
	};

	struct SizeInfo
	{
		//*変数

		//頂点の最小値
		D3DXVECTOR3 VtxMin = { 0.0f,0.0f,0.0f };                           //それぞれの座標のモデルの位置の最小値!
		D3DXVECTOR3 OriginVtxMin = { 0.0f,0.0f,0.0f };                     //元々の最小値!

		//頂点の最大値
		D3DXVECTOR3 VtxMax = { 0.0f,0.0f,0.0f };                           //それぞれの座標のモデルの位置の最大値!
		D3DXVECTOR3 OriginVtxMax = { 0.0f,0.0f,0.0f };                     //元々の最大値!

		//サイズ
		D3DXVECTOR3 Size = { 0.0f,0.0f,0.0f };                             //サイズ!

		//XZのサイズを変える
		bool bSwapVtxXZ = false;

		//変数
		D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f };                            //大きさ!
		D3DXVECTOR3 FormarScale = { 1.0f,1.0f,1.0f };                      //元の拡大率!
		D3DXVECTOR3 MultiScale = { 0.0f,0.0f,0.0f };                       //乗算拡大率
		D3DXVECTOR3 AddScale = { 0.0f,0.0f,0.0f };                         //加算する拡大率を設定!
		bool bUseMultiScale = false;                                       //拡大率を乗算するかどうか
		bool bUseAddScaling = false;                                       //拡大率を使用するかどうか!


		//*関数
		D3DXVECTOR3& GetVtxMax() { return VtxMax; }                                          //各頂点最大値の取得
		void SetVtxMax(D3DXVECTOR3 CopyVtxMax) { VtxMax = CopyVtxMax; }
		D3DXVECTOR3& GetOriginVtxMax() { return OriginVtxMax; }                              //元の各頂点最大値の取得
		void SetOriginVtxMax(D3DXVECTOR3 CopyVtxMax) { OriginVtxMax = CopyVtxMax; }                   //元の最大頂点の設定

		//最小頂点
		D3DXVECTOR3& GetVtxMin() { return VtxMin; }                                          //各頂点最小値の取得
		void SetVtxMin(D3DXVECTOR3 CopyVtxMin) { VtxMin = CopyVtxMin; }
		D3DXVECTOR3& GetOriginVtxMin() { return OriginVtxMin; }                              //元の各頂点最小値の取得
		void SetOriginVtxMin(D3DXVECTOR3 CopyVtxMin) { OriginVtxMin = CopyVtxMin; }                   //元の最小頂点の設定

		D3DXVECTOR3 GetSize() { return Size; }                                                //Xオブジェクトのサイズを取得

		//最大頂点と最小頂点をスワップ
		void ActiveSwapVtxMaxMin()
		{
			if (bSwapVtxXZ == true)
			{
				swap(OriginVtxMax.x, OriginVtxMax.z);
				swap(OriginVtxMin.x, OriginVtxMin.z);
			}
		}
		//関数
		void SetScale(D3DXVECTOR3 CopyScale) { Scale = CopyScale; }                                  //拡大率の設定
		D3DXVECTOR3& GetScale() { return Scale; }                                            //拡大率の取得

		//元の拡大率
		void SetFormarScale(D3DXVECTOR3 CopyScale) { FormarScale = CopyScale; }                      //元の拡大率を設定
		D3DXVECTOR3& GetFormarScale() { return FormarScale; }                                //元の拡大率を取得

		//加算拡大率
		void SetUseAddScale(D3DXVECTOR3 CopyAddScale, bool bUse);                                  //拡大率の加算を使用するかどうか

		//乗算拡大率
		void SetUseMultiScale(bool bUse, D3DXVECTOR3 CopyScale) { bUseMultiScale = bUse; MultiScale = CopyScale; }

		void SetUseSwapVtxXZ(bool bUse) { bSwapVtxXZ = bUse; }
		const bool& GetUseSwapVtxXZ()const { return bSwapVtxXZ; }

		//処理
		void AddScaleProcess();//拡大率加算処理
		void MultiScaleProcess();//拡大率乗算処理
		void DecideVtxMaxMinProcess();//拡大率を参照して頂点の最大最小を決める処理
	};

	struct MoveInfo
	{
		//*変数

		//移動量
		D3DXVECTOR3 Move = { 0.0f,0.0f,0.0f };                             //移動量!
		D3DXVECTOR3 AddMove = { 0.0f,0.0f,0.0f };                          //加算移動量
		//慣性
		float fInertia = m_fNORMAL_INERTIA;                               //慣性!
		bool bUseInteria = false;                             //慣性をかけるかどうか!
		//重力
		bool bUseGravity = false;                             //重力を使用するかどうか!
		float fGravityPower = m_fNORMAL_GRAVITY;                          //重力の大きさ!
		//乗算加速
		bool bUseMultiSpeed = false;                                  //乗算加速をするかどうか!
		D3DXVECTOR3 MultiSpeed = { 0.0f,0.0f,0.0f };                               //乗算加速度!
		//加速
		bool bUseAddSpeed = false;                                    //加速を使用するかどうか!
		D3DXVECTOR3 AddSpeed = { 0.0f,0.0f,0.0f };                                 //加速度

		//位置の更新をするかどうか
		bool bUseUpdatePos = true;                           //位置の更新を使用するかどうか!

		//*関数
		
		//移動量
		void SetMove(D3DXVECTOR3 CopyMove) { Move = CopyMove; }//移動量を設定する
		const D3DXVECTOR3& GetMove() const { return Move; }//移動量を取得する

		void SetAddMove(D3DXVECTOR3 CopyMove) { AddMove = CopyMove; }//加算移動量を設定
		const D3DXVECTOR3& GetAddMove() const { return AddMove; }//加算移動量を取得

		//慣性
		void SetUseInteria(bool bUse, float fCopyInertia) { bUseInteria = bUse; fInertia = fCopyInertia; }//慣性をかけるかどうか
		float& GetInertia() { return fInertia; }//慣性を取得する
		void SetUndoInertia() { fInertia = m_fNORMAL_INERTIA; }//慣性の大きさを元に戻す
		bool& GetUseInteria() { return bUseInteria; }//慣性を掛けるかどうかを取得

		//重力
		void SetUseGravity(bool bUse, float fCopyGravityPower) { bUseGravity = bUse; fGravityPower = fCopyGravityPower; }//重力を設定する
		const bool& GetUseGravity() const { return bUseGravity; }//重力を使用するかどうかを設定
		const float& GetGravity() const { return fGravityPower; }//重力を取得する
		//乗算加速
		void SetUseMultiSpeed(bool bUse, D3DXVECTOR3 CopyMultiSpeed) { MultiSpeed = CopyMultiSpeed; bUseMultiSpeed = bUse; }//乗算加速を使用するかどうか

		//加算速度
		void SetUseAddSpeed(bool bUse, D3DXVECTOR3 CopyAddSpeed) { bUseAddSpeed = bUse; AddSpeed = CopyAddSpeed; }//加速を使用するかどうか
		//============================================================================================================

		//===================================
		//位置更新関係
		//===================================
		void SetUseUpdatePos(bool bUse) { bUseUpdatePos = bUse; }//位置の更新を使用するかどうか
		void GravityProcess();                                   //重力の処理
		void MultiSpeedProcess();                                //乗算加速処理
		void AddSpeedProcess();                                  //加速処理
		//=================================================================================================================
	};

	//体力構造体
	struct LifeInfo
	{
		//*変数
		bool bHitStop = false;                        //ヒットストップ!
		int nHitStopTime = 0;                             //ヒットストップ時間!
		void HitStopProcess();                        //ヒットストップの処理

		//自動的に体力を減らすかどうか
		bool bAutoSubLife = false;                            //自動的に体力を減らし続けるかどうか!

		//体力
		int nLife = 1;                                    //体力!
		int nMaxLife = 1;                                 //最大体力!

		//色
		bool bUseRatioLifeAlpha = false;                      //体力の割合で透明度を変えるかどうか!

		//死亡
		bool bAutoDeath = false;                             //体力０で自動的に破棄されるかどうか
		//============================================================================================================

		//体力
		void SetLife(int nCopyLife) { nLife = nCopyLife; }                    //体力を設定する
		int& GetLife() { return nLife; }                              //体力を取得する

		//最大体力
		void SetMaxLife(int nCopyMaxLife) { nMaxLife = nCopyMaxLife; }        //最大体力を設定する
		int& GetMaxLife() { return nMaxLife; }                        //最大体力を取得する

		//ヒットストップ
		void SetHitStopTime(int nCopyHitStopTime) { nHitStopTime = nCopyHitStopTime; }//ヒットストップ時間を設定する
		bool& GetHitStop() { return bHitStop; }                       //ヒットストップ状態かどうかを取得する

		//自動的に体力を減らす
		void SetAutoSubLife(bool bUse) { bAutoSubLife = bUse; }       //体力を自動的に減らすかどうかを設定
		int& GetHitStopTime() { return nHitStopTime; }                //ヒットストップ時間を取得する

		//体力によって透明度を変えるかどうか
		void SetUseRatioLifeAlpha(bool bUse) { bUseRatioLifeAlpha = bUse; }

		//体力０で自動的に破棄するかどうか
		void SetAutoDeath(bool bUse) { bAutoDeath = bUse; }
		const bool& GetAutoDeath() const { return bAutoDeath; }

		//体力を自動的に減らす処理
		void AutoSubLifeProcess();

		//体力の割合に応じて色合いを変える処理
		void RatioLifeAlphaColorProcess(CObjectX * pObjX);

		//体力が０になったら自動的に死亡フラグを発動する処理
		void AutoDeathProcess(CObjectX* pObjX);
	};

	//当たり判定構造体
	struct CollisionInfo
	{
		//正方形
		struct Square
		{
			//変数
			bool bPushOutFirst[static_cast<int>(AXIS::MAX)] = {};                  

			//関数
			void ResetPushOutFirstFlag() //それぞれの軸の判定を優先するフラグをリセットする
			{
				for (int nCnt = 0; nCnt < static_cast<int>(AXIS::MAX); nCnt++)
				{
					bPushOutFirst[nCnt] = false;
				}
			}
			void SetPushOutFirstFlag(AXIS Axis,bool bFlag)//それぞれの軸の判定を優先するかどうかを設定
			{
				bPushOutFirst[static_cast<int>(Axis)] = bFlag;
			}
			const bool& GetPushOutFlag(AXIS Axis)//それぞれの軸の判定を優先するかどうかを取得
			{ 
				return bPushOutFirst[static_cast<int>(Axis)];
			}
		};

		//状態
		struct State
		{
		private:
			bool bIsLanding = false;                                      //地面にいるかどうか
			bool bIsLandingOld = false;                                   //1f前に地面にいるかどうか
			bool bIsWalling = false;                                      //壁にいるかどうか
			bool bIsWallingOld = false;                                   //1f前に壁にいるかどうか
		public:
			const bool& GetLanding() const { return bIsLanding; }         //地面にいるかどうかを取得
			const bool& GetLandingOld() const { return bIsLandingOld; }   //1f前に地面にいるかどうかを取得する
			const bool& GetWalling() const { return bIsWalling; }         //壁にいるかどうかを取得
			const bool& GetWallingOld() const { return bIsWallingOld; }   //1f前に壁にいるかどうかを取得
			void SetLanding(bool bFlag) { bIsLanding = bFlag; }           //地面にいるかどうかを設定
			void SetLandingOld(bool bFlag) { bIsLandingOld = bFlag; }     //1f前に地面にいるかどうかを取得する
			void SetWalling(bool bFlag) { bIsWalling = bFlag; }           //壁にいるかどうかを設定
			void SetWallingOld(bool bFlag) { bIsWallingOld = bFlag; }     //1f前に壁にいるかどうかを取得する
			void ResetState() { bIsLanding = false; bIsWalling = false; } //フラグをリセットする
		};

		//変数
		State StateInfo = {};                            //判定状態
		Square SquareInfo = {};                          //正方形の当たり判定

		//関数
		State & GetState() { return StateInfo; }         //判定状態の取得
		Square & GetSquareInfo() { return SquareInfo; }  //正方形の判定情報の取得
	};


	CObjectX(int nPri = 0, bool bUseintPri = false,CObject::TYPE type = CObject::TYPE::NONE, CObject::OBJECTTYPE ObjType = CObject::OBJECTTYPE::OBJECTTYPE_X);                                                           //コンストラクタ
	~CObjectX() override;                                                 //デストラクタ
	HRESULT Init() override;                                              //初期化処理
	void Uninit() override;                                               //終了処理
	void Update() override;                                               //更新処理
	void Draw() override;                                                 //描画処理
	void SetDeath() override;                                             //死亡フラグ設定処理
	void BindObjectXInfo(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPDIRECT3DTEXTURE9* pTexture,D3DCOLORVALUE * pDiffuse);//モデル情報を割り当てる

	void UpdatePos();                                                     //位置の更新処理
	//==========================================================
	//静的メンバ宣言
	//==========================================================
	static void SetCommonDraw(bool bDraw) { s_bCOMMON_DRAWSHADOW = bDraw; }    //影を描画するかどうか
	
	//クォータニオン
	void SetAxis(float fAxis) { m_fAxis = fAxis; }                             //回転角を設定
	const float& GetAxis() const { return m_fAxis; }                           //回転角を取得 
	void SetVecAxis(D3DXVECTOR3 VecAxis) { m_VecAxis = VecAxis; }              //回転軸を設定
	const D3DXVECTOR3& GetVecAxis() const { return m_VecAxis; }                //回転軸を取得

	//============================================================================================================

	//==========================================================
	//拡大率
	//==========================================================
	//============================================================================================================
	
	//==========================================================
	//体力関係
	//==========================================================
	virtual void SetDamage(int nDamage, int nHitStopTime);                                   //ダメージを与える
	//============================================================================================================

	//==========================================================
	//サイズ
	//==========================================================
	void CalculateSenterPos();
	virtual void SetSize();                                                                  //サイズを設定する 
	//============================================================================================================

	//==========================================================
	//エディタ
	//==========================================================

	//オブジェクトXの種類
	void SetObjXType(OBJECTXTYPE ObjXType) { m_nObjXType = ObjXType; }                                           //オブジェクトXの分類を設定
	OBJECTXTYPE GetObjXType() { return m_nObjXType; }                                                            //オブジェクトXのタイプを取得する
																							                     
	//タイプ番号																			                        
	void SetTypeNum(int nTypeNum) { m_nTypeNum = nTypeNum; }                                                     //オブジェクトごとのタイプ番号を設定
	int GetTypeNum() { return m_nTypeNum;}                                                                       //オブジェクトごとのタイプ番号を設定
																							                     
	//情報表示処理＆操作																	                        
	void ManagerChooseControlInfo() override;                                                                    //ステージマネージャーが情報を操作する
																							                     
	void SaveInfoTxt(fstream& WritingFile) override;                                                             //テキストファイルに情報を保存するための関数
	void LoadInfoTxt(fstream& LoadingFile, list<CObject*>& listSaveManager, string& Buff, CObject* pObj) override; //テキストファイルから情報を読み込むための関数
	//================================================================================================================================================

	//==========================================================
	//モデル情報
	//==========================================================
	CObjectX::ObjectXInfo& GetObjectXInfo() { return m_ObjectXInfo; }
	//================================================================================================================================================

	//===================================
	//描画
	//===================================
	//狙ったマテリアルにテクスチャ割り当て（ポインタのポインタになってるから大丈夫）
	void SetLockOnMatBindTexture(int nNumMat, LPDIRECT3DTEXTURE9 pTexture);

	//色合い
	void SetColor(D3DXCOLOR col, int nColChengeTime, bool bChoose, bool bSetAlpha,bool bBlinking);

	//=================================================================================================================

	//===================================
	//各構造体
	//===================================

	//描画情報の取得
	DrawInfo& GetDrawInfo() { return m_DrawInfo; }

	//位置情報の取得
	PosInfo& GetPosInfo() { return m_PosInfo; }

	//向き情報の取得
	RotInfo& GetRotInfo() { return m_RotInfo; }

	//サイズ情報の取得
	SizeInfo& GetSizeInfo() { return m_SizeInfo; }

	//移動量情報の取得
	MoveInfo& GetMoveInfo() { return m_MoveInfo; }

	//体力情報の取得
	LifeInfo& GetLifeInfo() { return m_LifeInfo; }

	//当たり判定情報の取得
	CollisionInfo& GetCollisionInfo() { return m_CollisionInfo; }

	//=================================================================================================================

	//==========================================================
	//判定関係
	//==========================================================

	//正方形の押し出し判定

	//X
	void SetExtrusionCollisionSquareX(bool bSuccess) { m_bExtrusionCollisionSquareX = bSuccess; }
	const bool& GetExtrusionCollisionSquareX() const { return m_bExtrusionCollisionSquareX; }

	//Y
	void SetExtrusionCollisionSquareY(bool bSuccess) { m_bExtrusionCollisionSquareY = bSuccess; }
	const bool& GetExtrusionCollisionSquareY() const { return m_bExtrusionCollisionSquareY; }

	//Z
	void SetExtrusionCollisionSquareZ(bool bSuccess) { m_bExtrusionCollisionSquareZ = bSuccess; }
	const bool& GetExtrusionCollisionSquareZ() const { return m_bExtrusionCollisionSquareZ; }

	//============================================================================================================


    //==========================================================
	//静的メンバ取得
	//==========================================================
	static const float GetNormalGravity() { return m_fNORMAL_GRAVITY; }//通常重力を取得する
	static const float GetNormalInertia() { return m_fNORMAL_INERTIA; }//慣性を取得
	//=================================================================================================================
private:
	//===============================================
    //静的メンバ
    //===============================================
	static constexpr float m_fNORMAL_INERTIA = 0.5f;//普通の慣性
	static constexpr float m_fNORMAL_GRAVITY = 1.0f;//普通の重力
	static bool s_bCOMMON_DRAWSHADOW;               //描画
	//=================================================================================================================

	void DrawShadow();                                                       //影を描画する

	//==========================================================
	//クォータニオン
	//==========================================================
    float m_fAxis;//クォータニオンの回転角
	D3DXVECTOR3 m_VecAxis;//クォータニオンの回転軸
	//==========================================================================================================================================================
	
	//===============================================
	//モデル情報
	//===============================================
	ObjectXInfo m_ObjectXInfo;                      //モデル情報の構造体!
	//===========================================================================================================================================================
	
	//===================================
	//描画関係
	//===================================
	void SetFormarColor();                     //元の色合いに戻す
	void SetOnlyColor(D3DXCOLOR Col);          //色だけを設定する
	void SetOnlyFormarColor();                 //データは変えずにアルファ値だけ設定する
	//=================================================================================================================

	//===================================
	//モデルインデックス
	//===================================
	int m_nIndexObjectX;
	//==================================================================================================================
	
	//===================================
	//各構造体
	//===================================
	DrawInfo m_DrawInfo;           //描画情報
	PosInfo m_PosInfo;             //位置情報
	RotInfo m_RotInfo;             //向き情報
	SizeInfo m_SizeInfo;           //サイズ情報
	MoveInfo m_MoveInfo;           //移動量情報
	LifeInfo m_LifeInfo;           //体力情報
	CollisionInfo m_CollisionInfo; //当たり判定情報
	//==================================================================================================================

	//===================================
	//判定関係
	//===================================
	
	//正方形の押し出し判定
	bool m_bExtrusionCollisionSquareX;//X
	bool m_bExtrusionCollisionSquareY;//Y
	bool m_bExtrusionCollisionSquareZ;//Z

	//地面にいるかどうか
	bool m_bIsLanding;

	//============================================================================================================


	//===================================
	//エディタ関係
	//===================================
	int m_nManagerType;                             //マネージャーで呼び出される時のタイプ番号!
	int m_nTypeNum;                                 //モデルごとのタイプ番号!
	OBJECTXTYPE m_nObjXType;                        //オブジェクトXの分類!

	void ChengeEditScale();                         //拡大率を変更する
	void ChengeEditScaleX();                        //拡大率Xを変更する
	void ChengeEditScaleY();                        //拡大率Yを変更する
	void ChengeEditScaleZ();                        //拡大率Zを変更する
	void ChengeEditPos();                           //位置を変更する
	void EditLife();                                //体力を変更する
	void ChengeEditSwapVtxXZ();                     //最大頂点と最小頂点を変えるかどうか
	//=================================================================================================================

};
//============================================================================================================
#endif