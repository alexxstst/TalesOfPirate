#include "stdafx.h"
#include "GameApp.h"

#include "Scene.h"
#include "Character.h"
#include "SceneObj.h"
#include "SceneItem.h"
#include "ItemRecord.h"
#include "EffectObj.h"
#include "SceneObjSet.h"
#include "LuaInterface.h"
#include "CharacterRecord.h"
#include "MapSet.h"
#include "GameApp.h"
#include "Actor.h"
#include "EventRecord.h"
#include "cameractrl.h"
using namespace std;

void CGameScene::HandleSceneMsg(int nMsgType, int nParam1, int nParam2, int nParam3)
{
	CCharacter* pCha     = NULL;
	CEffectObj	*pEffect = NULL;
	CSceneItem* pItem    = NULL;
	CSceneObj*	pObj = NULL;

	
    switch(nMsgType)
	{
        // 
        case SCENEMSG_SCENEOBJ_UPDATEVALID:
        {
            BOOL bValid = nParam1;
            list<int> *pList = &_SceneObjIdx[nParam2];
            if(bValid)
            {
                if(find(pList->begin(), pList->end(), nParam3)!=pList->end())
                {
                    ToLogService("error", "{} {} {}", g_oLangRec.GetString(352), nParam2, nParam3);
                    break;
                }
                _SceneObjIdx[nParam2].push_back(nParam3);
                
                // begin debug by lsh
                if(nParam2 == 4)
                {
                    SceneLight* s = (SceneLight*)&_pSceneObjArray[nParam3];
                    if( s->type != SceneLight::SL_AMBIENT)
                    {
                        MessageBox(0, "y", "y", MB_OK);
                    }
                }
                // end
            }
            else
            {
                if(find(pList->begin(), pList->end(), nParam3)==pList->end())
                {
                    ToLogService("error", "{} {} {}", g_oLangRec.GetString(353), nParam2, nParam3);
                    break;
                }
                _SceneObjIdx[nParam2].remove(nParam3);
            }
            break;
        }
        case SCENEMSG_CHA_CREATE:
		{
			////nParam1 == ID, nParam2 == typeID
			CChaRecord *pInfo = GetChaRecordInfo(nParam2);
			if(!pInfo)
				return;
			pCha = GetCha(nParam1);

			//{lemon add@2004.9.2 for create binding effect
			for (int n = 0; n < pInfo->sEeffID; n++)
			{
				CMagicInfo* peffInfo = GetMagicInfo(pInfo->sFeffID[n]);
				if(!peffInfo)
				{
					continue;
				}
				for (int m = 0; m < peffInfo->nDummyNum; m++)
				{
					pEffect = GetFirstInvalidEffObj();
					if( !pEffect || !pEffect->Create( pInfo->sFeffID[n] ) )
					{
						ToLogService("ERROR", "create cha`s effect fail,ID {}", pInfo->sFeffID[n]);
						return;
					}
					if(pCha->IsBoat() && 
						(pInfo->sFeffID[n] == EFFECT_RIPPLE1 ||pInfo->sFeffID[n] == EFFECT_RIPPLE2 ))
					{
						//pEffect->setFollow();
						pEffect->setFollowObj((CSceneNode*)pCha,NODE_CHA);

					}else
					{
						//pEffect->setDummy(peffInfo->nDummy[m]);
						pEffect->setFollowObj((CSceneNode*)pCha,NODE_CHA,peffInfo->nDummy[m]);
					}
					//pEffect->setChaID(pCha->getID());
					if(pCha->IsBoat() && (pInfo->sFeffID[n] == EFFECT_RIPPLE2 ))
					{
						pEffect->SetHide(TRUE);
					}
					if(nParam2==CHA1 ||nParam2==CHA2 ||nParam2==CHA3 ||nParam2==CHA4 )
					{
						pEffect->SetHide(TRUE);
					}
					pEffect->Emission( -1, NULL, NULL); 

					pEffect->SetValid(TRUE);

					pCha->AddEffect(pEffect->getID());
				}

			}
  
			//{lemon add@2004.9.2 for create binding shade
			CShadeEff *pShade = NULL;
 /*           pShade = GetFirstInvalidShadeObj();
            if( pShade )
            {
                pShade->Create(2);
                pShade->setChaID(pCha->getID());
                pShade->Emission( 0, &pCha->GetPos(), NULL);    
                pShade->SetValid(TRUE);
                pShade->SetHide(TRUE);

                pCha->AddShade(pShade->getID());
            }*/

 			if(pInfo->sShadow != 0)
			{
				pShade = GetFirstInvalidShadeObj();
                if( pShade )
                {
                    pShade->Create(pInfo->sShadow);
                    pShade->setChaID(pCha->getID());
                    pShade->Emission( 0, &pCha->GetPos(), NULL);    
                    pShade->SetValid(TRUE);

                    pCha->AddShade(pShade->getID());
                }
			}

			//@}
			break;
		}
		case SCENEMSG_CHA_DESTROY:
		{
			pCha = GetCha(nParam1);
			int size = (int)pCha->GetEffectNum();
			int id;
			CEffectObj *pEff;
			int n;
			for( n = 0; n < size; n++)
			{
				id = pCha->GetEffectID(n);
				if(id < 0)
					continue;
				pEff = GetEffect(id);
				if(pEff && pEff->IsValid())
				{
					pEff->SetValid(FALSE);
				}
			}
			pCha->RemoveEffect();
			_cSceenSign.ChaDisappear( pCha );

			//shade
			CShadeEff *pShade;
			size = pCha->GetShadeNum();
			for(n = 0; n < size; n++)
			{
				id = pCha->GetShadeID(n);
				if(id < 0)
					continue;
				pShade = GetShadeObj(id);
				if(pShade && pShade->IsValid())
				{
					pShade->SetValid(FALSE);
				}
			}
			pCha->RemoveShade();
            // destroy scene item
            pCha->DestroyLinkItem();

			_free_chas.Push( pCha );
			break;
		}
		case SCENEMSG_CHA_BEGINMOVE:
		{
			pCha = GetCha(nParam2);
			////lemon add@2005.2.17
			if(pCha->IsMainCha())
				GetPugMgr()->BeginPug();

			if(nParam2 == g_pGameApp->GetMainCam()->m_iCha)
			{
				g_pGameApp->GetMainCam()->BeginMove();
			}
			if(pCha->IsBoat())
			{
				CEffectObj *pEff = GetEffect(pCha->GetEffectID(0));
				if(pEff)
				{
					if(pEff->getIdxID() == EFFECT_RIPPLE1)
					{
						pEff->End();
					}
				}
				pEff = GetEffect(pCha->GetEffectID(1));
				if(pEff)
				{
					if(pEff->getIdxID() == EFFECT_RIPPLE2)
					{
						pEff->SetHide(FALSE);
						pEff->Emission( -1, &pCha->GetPos(), NULL);
					}
				}
			}else
			{
				int size = (int)pCha->GetEffectNum();
				int id;
				for(int n = 0; n < size; n++)
				{
					id = pCha->GetEffectID(n);
					if(id < 0)
						continue;
					pEffect = GetEffect(id);
					if(pEffect && ((pEffect->getIdxID()==CHA_EFFECT1) ||(pEffect->getIdxID()==CHA_EFFECT2)
						||(pEffect->getIdxID()==CHA_EFFECT3) ||(pEffect->getIdxID()==CHA_EFFECT4)))
					{
						pEffect->SetHide(false);
					}
				}
			}
				//CEffectObj *pEff = GetEffect(pCha->GetEffectID(0));
				//if(pEff && ((pEff->getIdxID()==CHA_EFFECT1) ||(pEff->getIdxID()==CHA_EFFECT2)
				//	||(pEff->getIdxID()==CHA_EFFECT3) ||(pEff->getIdxID()==CHA_EFFECT4)))
				//{
				//	pEff->SetHide(false);	
				//	pEff = GetEffect(pCha->GetEffectID(1));
				//	if(pEff)
				//		pEff->SetHide(false);
				//}

			//}
			break;
		}
		case SCENEMSG_CHA_ENDMOVE:
		{
			pCha = GetCha(nParam2);
			////lemon add@2005.2.17
			if(pCha->IsMainCha())
				GetPugMgr()->EndPug();

			if(nParam2 == g_pGameApp->GetMainCam()->m_iCha)
			{
				g_pGameApp->GetMainCam()->EndMove();
			}
			if(pCha->IsBoat())
			{
				CEffectObj *pEff = GetEffect(pCha->GetEffectID(0));
				if(pEff)
				{
					if(pEff->getIdxID() == EFFECT_RIPPLE1)
					{
						pEff->Emission( -1, &pCha->GetPos(), NULL);
					}
				}
				pEff = GetEffect(pCha->GetEffectID(1));
				if(pEff)
				{
					if(pEff->getIdxID() == EFFECT_RIPPLE2)
					{
						pEff->End();
					}
				}
			}else
			{
				int size = (int)pCha->GetEffectNum();
				int id;
				for(int n = 0; n < size; n++)
				{
					id = pCha->GetEffectID(n);
					if(id < 0)
						continue;
					pEffect = GetEffect(id);
					if(pEffect && ((pEffect->getIdxID()==CHA_EFFECT1) ||(pEffect->getIdxID()==CHA_EFFECT2)
						||(pEffect->getIdxID()==CHA_EFFECT3) ||(pEffect->getIdxID()==CHA_EFFECT4)))
					{
						pEffect->SetHide(true);
					}
				}

				//CEffectObj *pEff = GetEffect(pCha->GetEffectID(0));
				//if(pEff && ((pEff->getIdxID()==CHA_EFFECT1) ||(pEff->getIdxID()==CHA_EFFECT2)
				//	||(pEff->getIdxID()==CHA_EFFECT3) ||(pEff->getIdxID()==CHA_EFFECT4)))
				//{
				//	pEff->SetHide(true);
				//	pEff = GetEffect(pCha->GetEffectID(1));
				//	if(pEff)
				//		pEff->SetHide(true);
				//}
			}
			break;
		}
		case SCENEMSG_SCENEOBJ_CREATE:
		{
			//CSceneObjInfo *pInfo = GetSceneObjInfo(nParam2);
			//if(pInfo->nType == 3)
			//{
			//	pObj = GetSceneObj(nParam1);
			//	CShadeEff *pShade;

			//	D3DXCOLOR dwColor;
			//	dwColor.a = 0.5f;
			//	dwColor.r = pInfo->btPointColor[0]/255;
			//	dwColor.g = pInfo->btPointColor[1]/255;
			//	dwColor.b = pInfo->btPointColor[2]/255;


			//	//pShade = GetFirstInvalidShadeObj();
 		//	//	pShade->CreateAttachLight(pInfo->nAttachEffectID,pInfo->nRange,dwColor);
			//	//pShade->setChaID(pObj->getID());
			//	//pShade->Emission( -1, &pObj->getPos(), NULL);    
			//	//pShade->SetValid(TRUE);
			//	//pShade->SetHide(TRUE);

			//	//pObj->AddShade(pShade->getID());


			//}
			break;
		}
		case SCENEMSG_SCENEOBJ_DESTROY:
		{
			pObj = GetSceneObj(nParam1);
			if( pObj->getObjType()==SCENEOBJ_TYPE_ENVSOUND && pObj->GetMusicID()!=-1 )
			{
				//env_snd_del( pObj->GetMusicID() );
				pObj->SetMusicID( -1 );
			}

			int size = (int)pObj->GetEffectNum();
			int id;
			CEffectObj *pEff;
			int n;
			for( n = 0; n < size; n++)
			{
				id = pObj->GetEffectID(n);
				if(id < 0)
					continue;
				pEff = GetEffect(id);
				if(pEff && pEff->IsValid())
				{
					pEff->SetValid(FALSE);
				}
			}
			pObj->RemoveEffect();
			//shade
			CShadeEff *pShade;
			size = pObj->GetShadeNum();
			for(n = 0; n < size; n++)
			{
				id = pObj->GetShadeID(n);
				if(id < 0)
					continue;
				pShade = GetShadeObj(id);
				if(pShade && pShade->IsValid())
				{
					pShade->SetValid(FALSE);
				}
			}
			pObj->RemoveShade();
			break;
		}
		case SCENEMSG_SCENEOBJ_MOVE:
		{
			break;
		}
		//
		case SCENEMSG_EFFECT_HIT:
		{
			break;
		}
		//
		case SCENEMSG_EFFECT_VALID:
		{
			CEffectObj * eff = GetEffect( nParam2 );
			if( !eff ) break;

			_free_effs.Push( eff );

			int size = (int)eff->GetEffectNum();
			int id;
			for(int n = 0; n < size; n++)
			{
				id = eff->GetEffectID(n);
				if(id < 0)
					continue;
				pObj = GetSceneObj(id);
				if(pObj && pObj->IsValid())
				{
					pObj->SetValid(FALSE);
					pObj->setChaID(-1);
					pObj->setChaType(-1);
				}
			}
			eff->RemoveEffect();
			break;
		}
		//
		case SCENEMSG_EFFECT_CREATE:
		{
			CEffectObj * eff = GetEffect( nParam2 );
			if( !eff ) return;
			if(eff->IsFlyEff())
			{
				if(eff->GetLightID()<=0)
					return;
				else
				{
					pObj = AddSceneObj(eff->GetLightID());
					if(pObj)
					{
						pObj->setChaType(3);
						pObj->setChaID(nParam2);

						eff->AddEffect(pObj->getID());
					}
				}
				break;
			}
			CMagicInfo* pInfo = GetMagicInfo(nParam1);
			if(!pInfo)
				return;
			if(pInfo->LightID<=0)
				return;
			else
			{
				pObj = AddSceneObj(pInfo->LightID);
				if(pObj)
				{
					pObj->setChaType(3);
					pObj->setChaID(nParam2);

					eff->AddEffect(pObj->getID());
				}
			}
			break;
		}
		//
		case SCENEMSG_SCENEITEM_CREATE:
		{
			pItem = GetSceneItem(nParam1);
			if( nParam2!=0 )
			{
				// 
				CItemRecord *pInfo = pItem->GetItemInfo();
				for (int n = 0; n < pInfo->sEffNum; n++)
				{
					if( pInfo->sEffect[n][0]<=0 )
						continue;

					pItem->bindEffect( pInfo->sEffect[n][1],
						pInfo->sEffect[n][0],
						true );

					//pEffect = GetFirstInvalidEffObj();
					//if( !pEffect ) continue;

					//// dummy
					//if(!pEffect->Create( pInfo->sEffect[n][0] ))
					//{
					//	LG("ERROR","create cha`s effect fail,ID %d", pInfo->sEffect[n][0]);
					//	return;
					//}
					//pEffect->setFollowObj((CSceneNode*)pItem,NODE_ITEM,pInfo->sEffect[n][1]);
					//pEffect->Emission( -1, NULL, NULL);    
					//pEffect->SetValid(TRUE);
					//pItem->AddEffect(pEffect->getID());
				}
			}
			else
			{
				// 
				CShadeEff *pShade = NULL;
				//
				pShade = GetFirstInvalidShadeObj();
				if( pShade )
				{
					pShade->Create(3);
					pShade->setChaID(pItem->getID());
					pShade->Emission( 0, &pItem->getPos(), NULL);    
					pShade->SetValid(TRUE);
					pShade->SetHide(TRUE);
					pItem->AddShade(pShade->getID());
				}

				//
				pShade = GetFirstInvalidShadeObj();
				if( pShade )
				{
					pShade->Create(5);
					pShade->setChaID(pItem->getID());
					pShade->Emission( 0, &pItem->getPos(), NULL);    
					pShade->SetValid(TRUE);
					pShade->SetHide(TRUE);

					pItem->AddShade(pShade->getID());
				}
			}
			break;
		}
		//
		case SCENEMSG_SCENEITEM_DESTROY:
		{
			pItem = GetSceneItem(nParam1);
			_free_items.Push( pItem );

			int size = (int)pItem->GetEffectNum();
			int id;
			CEffectObj *pEff;
			int n;
			for( n = 0; n < size; n++)
			{
				id = pItem->GetEffectID(n);
				if(id < 0)
					continue;
				pEff = GetEffect(id);
				if(pEff && pEff->IsValid())
				{
					pEff->SetValid(FALSE);
				}
			}
			pItem->RemoveEffect();
			//shade
			CShadeEff *pShade;
			size = pItem->GetShadeNum();
			for(n = 0; n < size; n++)
			{
				id = pItem->GetShadeID(n);
				if(id < 0)
					continue;
				pShade = GetShadeObj(id);
				if(pShade && pShade->IsValid())
				{
					pShade->SetValid(FALSE);
				}
			}
			pItem->RemoveShade();
			break;
		}
	}
}


