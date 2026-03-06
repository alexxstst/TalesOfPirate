---------------
-- Inventory --
---------------
function addEquip(x,y,name,form,skillid)
	_G["img"..name] = UI_CreateCompent( form, IMAGE_TYPE, "img"..name, 36,36, x-2 ,y -2 )
	UI_LoadImage( _G["img"..name], "texture/ui/corsairs/eqform/"..name..".png", NORMAL, 36,36, 0, 0 )
	_G[name] = UI_CreateCompent( form, COMMAND_ONE_TYPE, name, 32, 32, x ,y )
	if skillid then
		UI_AddCompent( skillid, _G["img"..name] )
		UI_AddCompent( skillid, _G[name] )
	end
end

function addEquipGroup(X,Y,Xoff,Yoff,names,form,skillid)
	for i,v in pairs(names) do
		addEquip(X + (i-1)* Xoff ,Y + (i-1)* Yoff,v,form,skillid)
	end
end


frmInv = UI_CreateForm( "frmInv", FALSE, 440, 400, 198, 160, TRUE, FALSE )
UI_FormSetHotKey( frmInv, ALT_KEY, HOTKEY_E ) --'E'
UI_ShowForm( frmInv, FALSE )
UI_AddFormToTemplete( frmInv, FORM_MAIN )
UI_SetIsDrag( frmInv, TRUE )

imgfrmInv = UI_CreateCompent( frmInv, IMAGE_TYPE, "imgfrmInv", 440, 400, 0, 0 )
UI_LoadImage( imgfrmInv, "texture/ui/corsairs/INV/invform.png", NORMAL, 440, 400, 0, 0 )

-- Close Button
btnClose = UI_CreateCompent( frmInv, BUTTON_TYPE, "btnClose", 14, 14, 415, 1 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-- Title Bar
labTitle = UI_CreateCompent( frmInv, LABELEX_TYPE, "labTitle", 400, 150, 10, 2 )
UI_SetCaption( labTitle, "Inventory")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

pgePublic = UI_CreateCompent( frmInv, PAGE_TYPE, "pgePublic", 192, 325, 8, 16 )
UI_SetPageButton( pgePublic, PAGE_BUTTON_CUSTOM, 48, 16 )

local tabX = 8
local tabXoff= 90
local tabY = 2

local tabsX = 80
local tabsY = 35

--Equip Tab
skillid = UI_CreatePageItem( pgePublic )
ttlNum = UI_GetPageItemObj( skillid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlNum, "texture/ui/corsairs/INV/ivntab.png", PAGE_ITEM_TITLE_ACTIVE, 80, 35, 0, 0 )
UI_LoadImage( ttlNum, "texture/ui/corsairs/INV/ivntab.png", PAGE_ITEM_TITLE_NORMAL, 80, 35, 80, 0 )
UI_SetPos( ttlNum, tabX, tabY )
UI_SetSize( ttlNum, tabsX, tabsY )


--left col
addEquipGroup(8,40,0,49,{"cmdArmet","cmdWing","cmdNecklace","cmdRightHand","cmdJewelry4","cmdCloak"},frmInv,skillid)
--2nd col
addEquipGroup(56,187,0,49,{"cmdCirclet1","cmdJewelry1","cmdRearPet"},frmInv,skillid)
--3rd col
addEquipGroup(104,187,0,49,{"cmdCirclet2","cmdJewelry2","cmdPet"},frmInv,skillid)
--right col
addEquipGroup(152,40,0,49,{"cmdBody","cmdGlove","cmdShoes","cmdLeftHand","cmdJewelry3","cmdMount"},frmInv,skillid)



ui3dCha = UI_CreateCompent( frmInv, UI3D_COMPENT, "ui3dCha", 0, 0, 96, 180 )



btnChaLeft3d = UI_CreateCompent( frmInv, BUTTON_TYPE, "btnChaLeft3d", 32, 38, 55,335 )
UI_LoadButtonImage( btnChaLeft3d, "texture/ui/corsairs/coButtons.png", 32, 32, 217, 87, TRUE )

btnChaRight3d = UI_CreateCompent( frmInv, BUTTON_TYPE, "btnChaRight3d", 32, 38, 109,335 )
UI_LoadButtonImage( btnChaRight3d, "texture/ui/corsairs/coButtons.png",  32, 32, 217, 119, TRUE )

UI_AddCompent( skillid, btnChaLeft3d )
UI_AddCompent( skillid, btnChaRight3d )
UI_AddCompent( skillid, ui3dCha )



--Apparel Tab
skillid = UI_CreatePageItem( pgePublic )
ttlNum = UI_GetPageItemObj( skillid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlNum, "texture/ui/corsairs/INV/ivntab.png", PAGE_ITEM_TITLE_ACTIVE, 80, 35, 0, 35 )
UI_LoadImage( ttlNum, "texture/ui/corsairs/INV/ivntab.png", PAGE_ITEM_TITLE_NORMAL, 80, 35, 80, 35 )

UI_SetPos( ttlNum, tabX+tabXoff , tabY )
UI_SetSize( ttlNum, tabsX, tabsY )
-- UI_AddCompent( skillid, lstNum )
UI_AddCompent( skillid, ttlNum )



addEquipGroup(22,48,0,55,{"cmdPetApp","cmdDaggerApp","cmdGunApp","cmdSword1App","cmdGreatSwordApp"},frmInv,skillid)
addEquipGroup(82,48,0,55,{"cmdArmetApp","cmdFaceApp","cmdBodyApp","cmdGloveApp","cmdShoesApp",},frmInv,skillid)
addEquipGroup(142,48,0,55,{"cmdGlowApp","cmdStaffApp","cmdBowApp","cmdSword2App","cmdShieldApp",},frmInv,skillid)

grdItem = UI_CreateCompent( frmInv, GOODS_GRID_TYPE, "grdItem", 208, 280, 215,55 ) 
UI_SetGridSpace( grdItem, 3, 3 )
UI_SetGridContent( grdItem, 8, 6 )
UI_SetGridUnitSize( grdItem, 32, 32 )
UI_SetMargin( grdItem, 0, 0, 0, 0 )

local impgoldplacementX = 200
local impgoldplacementY = 342

imgimpgold = UI_CreateCompent( frmInv, IMAGE_TYPE, "imgimpgold", 150,53, impgoldplacementX, impgoldplacementY )
UI_LoadImage( imgimpgold, "texture/ui/corsairs/INV/goldimp.png", NORMAL, 150,53, 0, 0 )


labItemgoldnumber = UI_CreateCompent( frmInv, LABELEX_TYPE, "labItemgoldnumber", 91, 8, impgoldplacementX+24, impgoldplacementY+8 )
UI_SetTextColor( labItemgoldnumber, COLOR_BLACK )
UI_SetLabelExFont( labItemgoldnumber, DEFAULT_FONT, TRUE, 4289901234 )

labItemIMPnumber = UI_CreateCompent( frmInv, LABELEX_TYPE, "labItemIMPnumber", 91, 8, impgoldplacementX+24, impgoldplacementY+34 )
UI_SetTextColor( labItemIMPnumber, COLOR_BLACK )
UI_SetLabelExFont( labItemIMPnumber, DEFAULT_FONT, TRUE, 4289901234 )

btnOpenTempBag = UI_CreateCompent( frmInv, BUTTON_TYPE, "btnOpenTempBag", 37, 22, 394, 20 )
UI_LoadButtonImage( btnOpenTempBag, "texture/ui/new6.tga", 37, 22, 0, 220,TRUE )
UI_SetHint( btnOpenTempBag , "Temporary Bag" )

btnLock = UI_CreateCompent( frmInv, BUTTON_TYPE, "btnLock", 12, 15, 418, 375 )
UI_FormSetHotKey( btnLock, ALT_KEY, HOTKEY_B )
imgLock = UI_CreateCompent( frmInv, IMAGE_TYPE, "imgLock", 12, 15, 418, 375 )
UI_LoadImage( imgLock, "texture/ui/a001.tga",NORMAL, 12, 15, 72, 165, TRUE )
imgUnLock = UI_CreateCompent( frmInv, IMAGE_TYPE, "imgUnLock", 12, 15, 418, 375)
UI_LoadImage( imgUnLock, "texture/ui/a001.tga",NORMAL, 12, 15, 84, 165, TRUE )
UI_SetHint( imgUnLock, "Lock Inventory" )

UI_SetHint( cmdArmet, "Helm" )
UI_SetHint( cmdBody, "Armor" )
UI_SetHint( cmdNecklace, "Necklace" )
UI_SetHint( cmdRightHand, "Weapon" )
UI_SetHint( cmdLeftHand, "Shield" )
UI_SetHint( cmdGlove, "Glove" )
UI_SetHint( cmdCirclet1, "Ring 1" )
UI_SetHint( cmdCirclet2, "Ring 2" )
UI_SetHint( cmdShoes, "Shoes" )
UI_SetHint( cmdJewelry1, "Bracelet 1" )
UI_SetHint( cmdJewelry2, "Bracelet 2" )
UI_SetHint( cmdJewelry3, "Belt" )
UI_SetHint( cmdJewelry4, "Handguard" )
UI_SetHint( cmdWing, "Wing" )
UI_SetHint( cmdMount, "Mount" )
UI_SetHint( cmdCloak, "Cloak" )
UI_SetHint( cmdRearPet, "Rear Fairy" )
UI_SetHint( cmdPet, "Fairy" )
UI_SetTag( cmdArmet, enumEQUIP_HEAD )
UI_SetTag( cmdBody, enumEQUIP_BODY )
UI_SetTag( cmdNecklace, enumEQUIP_NECK )
UI_SetTag( cmdRightHand, enumEQUIP_RHAND )
UI_SetTag( cmdLeftHand, enumEQUIP_LHAND )
UI_SetTag( cmdGlove, enumEQUIP_GLOVE )
UI_SetTag( cmdCirclet1, enumEQUIP_HAND1 )
UI_SetTag( cmdCirclet2, enumEQUIP_HAND2 )
UI_SetTag( cmdShoes, enumEQUIP_SHOES )
UI_SetTag( cmdJewelry1, enumEQUIP_Jewelry1 )
UI_SetTag( cmdJewelry2, enumEQUIP_Jewelry2 )
UI_SetTag( cmdJewelry3, enumEQUIP_Jewelry3 )
UI_SetTag( cmdJewelry4, enumEQUIP_Jewelry4 )
UI_SetTag( cmdWing, enumEQUIP_WING )
UI_SetTag( cmdCloak, 15 )
UI_SetTag( cmdPet, 16 )
UI_SetTag( cmdRearPet, 17 )
UI_SetTag( cmdMount, 18 )
UI_SetHint( cmdPetApp, "Pet Apparel" )
UI_SetTag( cmdPetApp, 24 ) 
UI_SetHint( cmdDaggerApp, "Dagger Apparel" )
UI_SetTag( cmdDaggerApp, 26 ) 
UI_SetHint( cmdGunApp, "Gun Apparel" )
UI_SetTag( cmdGunApp, 27 ) 
UI_SetHint( cmdSword1App, "Sword Apparel" )
UI_SetTag( cmdSword1App, 28 ) 
UI_SetHint( cmdGreatSwordApp, "Greatsword Apparel" )
UI_SetTag( cmdGreatSwordApp, 29 ) 
UI_SetHint( cmdArmetApp, "Helm Apparel" )
UI_SetTag( cmdArmetApp, 19 ) 
UI_SetHint( cmdFaceApp, "Face Apparel" )
UI_SetTag( cmdFaceApp, 20 ) 
UI_SetHint( cmdBodyApp, "Armor Apparel" )
UI_SetTag( cmdBodyApp, 21 ) 
UI_SetHint( cmdGloveApp, "Glove Apparel" )
UI_SetTag( cmdGloveApp, 22 )
UI_SetHint( cmdShoesApp, "Shoes Apparel" )
UI_SetTag( cmdShoesApp, 23 )
UI_SetHint( cmdGlowApp, "Glow Apparel" )
UI_SetTag( cmdGlowApp, 25 ) 
UI_SetHint( cmdStaffApp, "Staff Apparel" )
UI_SetTag( cmdStaffApp, 30 )
UI_SetHint( cmdBowApp, "Bow Apparel" )
UI_SetTag( cmdBowApp, 31 ) 
UI_SetHint( cmdSword2App, "Sword Apparel" )
UI_SetTag( cmdSword2App, 32 ) 
UI_SetHint( cmdShieldApp, "Shield Apparel" )
UI_SetTag( cmdShieldApp, 33 ) 




------------
-- skills --
------------
frmSkill = UI_CreateForm( "frmSkill", FALSE, 198, 394, 396, 160, FALSE, FALSE )
UI_FormSetHotKey( frmSkill, ALT_KEY, HOTKEY_S ) 
UI_ShowForm( frmSkill, TRUE )
UI_AddFormToTemplete( frmSkill, FORM_MAIN )
UI_SetIsDrag( frmSkill, TRUE )
--UI_SetFormStyle( frmSkill, 0 )
--UI_SetAlpha( frmSkill, 200 )

imgSkillBTop = UI_CreateCompent( frmSkill, IMAGE_TYPE, "imgSkillBTop", 198, 256, 0, 0 )
UI_LoadImage( imgSkillBTop, "texture/ui/PublicD.png", NORMAL, 198, 256, 0, 0 )
imgSkillBDown = UI_CreateCompent( frmSkill, IMAGE_TYPE, "imgSkillBDown", 198, 138, 0, 256 )
UI_LoadImage( imgSkillBDown, "texture/ui/PublicD2.png", NORMAL, 198, 138, 0, 0 )

imgSkillTop = UI_CreateCompent( frmSkill, IMAGE_TYPE, "imgSkillTop", 177, 256, 7, 54 )
UI_LoadImage( imgSkillTop, "texture/ui/Skill.dds", NORMAL, 175, 256, 0, 0 )
imgSkillDown = UI_CreateCompent( frmSkill, IMAGE_TYPE, "imgSkillDown", 177, 19, 7, 310 )
UI_LoadImage( imgSkillDown, "texture/ui/ShipBuild2.tga", NORMAL, 175, 18, 80, 233 )

imgSkillD = UI_CreateCompent( frmSkill, IMAGE_TYPE, "imgSkillD", 184, 43, 7, 330 )
UI_LoadImage( imgSkillD, "texture/ui/TalkM.dds", NORMAL, 184, 43, 0, 213 )

imgSkillDL = UI_CreateCompent( frmSkill, IMAGE_TYPE, "imgSkillDL", 36, 36, 15, 334 )
UI_LoadImage( imgSkillDL, "texture/ui/PublicC.tga", NORMAL, 36, 36, 167, 59 )

imgSkillP = UI_CreateCompent( frmSkill, IMAGE_TYPE, "imgSkillP", 39, 18, 132, 296 )
UI_LoadImage( imgSkillP, "texture/ui/SysBotton4.tga", NORMAL, 39, 18, 106, 99 )

labFront = UI_CreateCompent( frmSkill, LABELEX_TYPE, "labFront", 59, 11, 69, 299 )
UI_SetCaption( labFront, "Skill Pts" )
UI_SetTextColor( labFront, COLOR_BLACK )
UI_SetLabelExFont( labFront, DEFAULT_FONT, TRUE, COLOR_WHITE )

labPoint = UI_CreateCompent( frmSkill, LABELEX_TYPE, "labPoint", 19, 11, 143, 299 )
UI_SetCaption( labPoint, "100" )
UI_SetTextColor( labPoint, COLOR_BLACK )
UI_SetLabelExFont( labPoint, DEFAULT_FONT, TRUE, COLOR_WHITE )

labPoint1 = UI_CreateCompent( frmSkill, LABELEX_TYPE, "labPoint1", 19, 11, 143, 299 )
UI_SetCaption( labPoint1, "100" )
UI_SetTextColor( labPoint1, COLOR_BLACK )
UI_SetLabelExFont( labPoint1, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmSkill, BUTTON_TYPE, "btnClose", 14, 14, 177, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labTitle = UI_CreateCompent( frmSkill, LABELEX_TYPE, "labTitle", 400, 150, 10, 3 )
UI_SetCaption( labTitle, "Skill")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

lstSkill = UI_CreateCompent( frmSkill, SKILL_LIST_TYPE, "lstSkill", 183, 275, 5, -20 )
UI_SetMargin( lstSkill, 1, 4, 3, 5 )
UI_SetListRowHeight( lstSkill, 38 )
--UI_ListLoadSelectImage( lstSkill, "texture/ui/button.dds", 148, 32, 38, 175 )

UI_LoadSkillListButtonImage( lstSkill, "texture/ui/Quest1.tga", 17, 17, 4, 198, 17, 17 )

scrollid = UI_GetScroll( lstSkill)
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

lstSkillW = UI_CreateCompent( frmSkill, SKILL_LIST_TYPE, "lstSkillW", 183, 275, 5, -20 )
UI_SetMargin( lstSkillW, 1, 4, 3, 5 )
UI_SetListRowHeight( lstSkillW, 38 )
--UI_ListLoadSelectImage( lstSkillW, "texture/ui/button.dds", 148, 32, 38, 175 )
UI_LoadSkillListButtonImage( lstSkillW, "texture/ui/Quest1.tga", 17, 17, 4, 198, 17, 17 )

scrollid = UI_GetScroll( lstSkillW )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

lstSkillS = UI_CreateCompent( frmSkill, SKILL_LIST_TYPE, "lstSkillS", 183, 275, 5, -20 )
UI_SetMargin( lstSkillS, 1, 4, 3, 5 )
UI_SetListRowHeight( lstSkillS, 38 )
--UI_ListLoadSelectImage( lstSkillS, "texture/ui/button.dds", 148, 32, 38, 175 )

UI_LoadSkillListButtonImage( lstSkillS, "texture/ui/Quest1.tga", 17, 17, 4, 198, 17, 17 )

scrollid = UI_GetScroll( lstSkillS )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

pgeSkill = UI_CreateCompent( frmSkill, PAGE_TYPE, "pgeSkill", 211, 233, 11, 79 )
UI_SetPageButton( pgeSkill, PAGE_BUTTON_CUSTOM, 48, 16 )

skillid = UI_CreatePageItem( pgeSkill )
ttlBattle = UI_GetPageItemObj( skillid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlBattle, "texture/ui/player2.tga", PAGE_ITEM_TITLE_NORMAL, 49, 35, 103, 113 )
UI_LoadImage( ttlBattle, "texture/ui/player2.tga", PAGE_ITEM_TITLE_ACTIVE, 49, 35, 48, 113 )
UI_SetPos( ttlBattle, 12, -54 )
UI_SetSize( ttlBattle, 49, 35 )
UI_AddCompent( skillid, lstSkill )
UI_AddCompent( skillid, imgSkillP )
UI_AddCompent( skillid, labFront )
UI_AddCompent( skillid, labPoint )

skillid = UI_CreatePageItem( pgeSkill )
ttlLive = UI_GetPageItemObj( skillid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlLive, "texture/ui/player2.tga", PAGE_ITEM_TITLE_NORMAL, 49, 35, 100, 154 )
UI_LoadImage( ttlLive, "texture/ui/player2.tga", PAGE_ITEM_TITLE_ACTIVE, 49, 35, 45, 154 )
UI_SetPos( ttlLive, 61, -54 )
UI_SetSize( ttlLive, 49, 35 )
UI_AddCompent( skillid, lstSkillW )
UI_AddCompent( skillid, imgSkillP )
UI_AddCompent( skillid, labFront )
UI_AddCompent( skillid, labPoint1 )

skillid = UI_CreatePageItem( pgeSkill )
ttlSail = UI_GetPageItemObj( skillid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlSail, "texture/ui/player2.tga", PAGE_ITEM_TITLE_NORMAL, 49, 35, 103, 70 )
UI_LoadImage( ttlSail, "texture/ui/player2.tga", PAGE_ITEM_TITLE_ACTIVE, 49, 35, 48, 70 )
UI_SetPos( ttlSail, 110, -54 )
UI_SetSize( ttlSail, 49, 35 )
UI_AddCompent( skillid, lstSkillS )

inventoryItemRightClick = UI_CreateCompent( frmInv, MENU_TYPE, "inventoryItemRightClick", 90, 97, 240, 468 )
UI_MenuLoadImage( inventoryItemRightClick, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( inventoryItemRightClick, 10, 5, 5, 5 )
UI_MenuLoadSelect( inventoryItemRightClick, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )
UI_MenuAddText(inventoryItemRightClick, "Lock Item")
UI_MenuAddText(inventoryItemRightClick, "Unlock Item")
UI_MenuAddText(inventoryItemRightClick, "Throw Item")
UI_MenuAddText(inventoryItemRightClick, "Delete Item")
UI_MenuAddText(inventoryItemRightClick, "Sell Item")


-------------------------------------------------
-- Equipment Spy | Coded by Billy, GUI by Foxseiz
-------------------------------------------------
frmEquipX = 196
frmEquipY = 385
frmItemSpy = UI_CreateForm( "frmItemSpy", FALSE, frmEquipX,frmEquipY, 198+242, 160, TRUE, FALSE )
--UI_FormSetHotKey( frmItemSpy, ALT_KEY, HOTKEY_Z ) --'F'
UI_ShowForm( frmItemSpy, FALSE )
UI_AddFormToTemplete( frmItemSpy, FORM_MAIN )
UI_SetIsDrag( frmItemSpy, TRUE )

imgItemTop = UI_CreateCompent( frmItemSpy, IMAGE_TYPE, "imgItemTop", frmEquipX,frmEquipY, 0, 0 )
UI_LoadImage( imgItemTop, "texture/ui/corsairs/eqform/eqform.png", NORMAL, frmEquipX,frmEquipY, 0, 0 )

btnClose = UI_CreateCompent( frmItemSpy, BUTTON_TYPE, "btnClose", 14, 14, 173, 0 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-- Form Label
labTitle = UI_CreateCompent( frmItemSpy, LABELEX_TYPE, "labTitle", 400, 150, 10, 1 )
UI_SetCaption( labTitle, "Equip")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

--backdrop = UI_CreateCompent( frmItemSpy, IMAGE_TYPE, "backdrop", 108,200, 44,16 )
--UI_LoadImage( backdrop, "texture/ui/corsairs/eqform2.png",NORMAL, 108,200, 44, 16, TRUE )


--left col
addEquipGroup(10,84,0,49,{"cmdArmet","cmdWing","cmdNecklace","cmdRightHand","cmdJewelry4","cmdCloak"},frmItemSpy)
--2nd col
addEquipGroup(58,231,0,49,{"cmdCirclet1","cmdJewelry1","cmdRearPet"},frmItemSpy)
--3rd col
addEquipGroup(106,231,0,49,{"cmdCirclet2","cmdJewelry2","cmdPet"},frmItemSpy)
--right col
addEquipGroup(154,84,0,49,{"cmdBody","cmdGlove","cmdShoes","cmdLeftHand","cmdJewelry3","cmdMount"},frmItemSpy)


UI_SetHint( cmdArmet, "Helm" )
UI_SetHint( cmdBody, "Armor" )
UI_SetHint( cmdNecklace, "Necklace" )
UI_SetHint( cmdRightHand, "Weapon" )
UI_SetHint( cmdLeftHand, "Shield" )
UI_SetHint( cmdGlove, "Glove" )
UI_SetHint( cmdCirclet1, "Ring 1" )
UI_SetHint( cmdCirclet2, "Ring 2" )
UI_SetHint( cmdShoes, "Shoes" )
UI_SetHint( cmdJewelry1, "Bracelet 1" )
UI_SetHint( cmdJewelry2, "Bracelet 2" )
UI_SetHint( cmdJewelry3, "Belt" )
UI_SetHint( cmdJewelry4, "Handguard" )
UI_SetHint( cmdWing, "Wing" )
UI_SetHint( cmdMount, "Mount" )
UI_SetHint( cmdCloak, "Cloak" )
UI_SetHint( cmdRearPet, "Rear Fairy" )
UI_SetHint( cmdPet, "Fairy" )


-- PLAYER MODEL
ui3dEqSpyCha = UI_CreateCompent( frmItemSpy, UI3D_COMPENT, "ui3dEqSpyCha", 0, 0, 96, 210 )

btnLeft3d = UI_CreateCompent( frmItemSpy, BUTTON_TYPE, "btnLeft3d", 32, 38, 11,182-49-49-49 )
-- UI_LoadButtonImage( btnLeft3d, "texture/ui/select4.tga", 32, 38, 0, 38, TRUE )
UI_LoadButtonImage( btnLeft3d, "texture/ui/corsairs/coButtons.png", 32, 32, 217, 87, TRUE )

btnRight3d = UI_CreateCompent( frmItemSpy, BUTTON_TYPE, "btnRight3d", 32, 38, 153,182-49-49-49 )
-- UI_LoadButtonImage( btnRight3d, "texture/ui/select4.tga", 32, 38, 0, 0, TRUE )
UI_LoadButtonImage( btnRight3d, "texture/ui/corsairs/coButtons.png",  32, 32, 217, 119, TRUE )

---------------------------------------
--  Npc/ Monster Helper from Top II  --
----------------------------------------
--  Lee NPC   20080715  --
----------------------------------------
frmNpcShow= UI_CreateForm( "frmNpcShow", FALSE, 420, 544, 0, 0, FALSE, FALSE )
UI_ShowForm( frmNpcShow, TRUE )
UI_AddFormToTemplete( frmNpcShow, FORM_MAIN )
UI_SetIsDrag( frmNpcShow, TRUE )
UI_SetFormStyleEx( frmNpcShow, 1, 0, 45)

UI_FormSetHotKey( frmNpcShow, ALT_KEY, HOTKEY_Z )
imgSkilld1 = UI_CreateCompent( frmNpcShow, IMAGE_TYPE, "imgSkilld1", 416,544, 0, 0 )
UI_LoadImage( imgSkilld1, "texture/ui/corsairs/npclist.png", NORMAL, 420, 544, 0, 0 )

labTitle = UI_CreateCompent( frmNpcShow, LABELEX_TYPE, "labTitle", 400, 150, 150, 5 )
UI_SetCaption( labTitle, "Npc and Monster Index")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmNpcShow, BUTTON_TYPE, "btnClose", 14, 14, 400, 5 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

chkID = UI_CreateCompent( frmNpcShow, CHECK_TYPE, "chkID", 10, 10, 17, 400 )
UI_LoadImage( chkID, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40,231 )
UI_LoadImage( chkID, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )
UI_SetHint( chkID, "Walks automatically towards the target" )

labID= UI_CreateCompent( frmNpcShow, LABELEX_TYPE, "labID", 12, 12, 32, 400 )
UI_SetCaption( labID, "Use auto-pathing" )
UI_SetTextColor( labID, COLOR_BLACK )
UI_SetLabelExFont( labID, DEFAULT_FONT, FALSE, COLOR_WHITE )

labNPC= UI_CreateCompent( frmNpcShow, LABELEX_TYPE, "labNPC", 128, 11,33,5 )
UI_SetCaption( labNPC, "Name" )
UI_SetTextColor( labNPC, COLOR_BLACK )
UI_SetLabelExFont( labNPC, DEFAULT_FONT, FALSE, COLOR_WHITE )

labNpcPos= UI_CreateCompent( frmNpcShow, LABELEX_TYPE, "labNpcPos", 219, 11, 210, 5)
UI_SetCaption( labNpcPos, "Region" )
UI_SetTextColor( labNpcPos, COLOR_BLACK )
UI_SetLabelExFont( labNpcPos, DEFAULT_FONT, FALSE, COLOR_WHITE )

labPOS= UI_CreateCompent( frmNpcShow, LABELEX_TYPE, "labPOS", 219, 11, 300,5)
UI_SetCaption( labPOS, "Coordinates" )
UI_SetTextColor( labPOS, COLOR_BLACK )
UI_SetLabelExFont( labPOS, DEFAULT_FONT, FALSE, COLOR_WHITE )

lstNpcList = UI_CreateCompent( frmNpcShow, LIST_TYPE, "lstNpcList", 392, 297, 10, 17 )
lstHelpButton = UI_GetList(lstNpcList)
UI_SetListRowHeight(lstNpcList,15)
UI_SetListIsMouseFollow( lstNpcList, TRUE )


scrollid = UI_GetScroll( lstNpcList )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/forsaken/item1.tga", COMPENT_BACK, 11, 1, 232, 190 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, FALSE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, FALSE )
UI_SetSize( id, 8, 7 )

------------------------------------------------------------
labMonster= UI_CreateCompent( frmNpcShow, LABELEX_TYPE, "labMonster", 61, 11, 33,5 )
UI_SetCaption( labMonster, "Name" )
UI_SetTextColor( labMonster, COLOR_BLACK )
UI_SetLabelExFont( labMonster, DEFAULT_FONT, FALSE, COLOR_WHITE )

labMonsterLV= UI_CreateCompent( frmNpcShow, LABELEX_TYPE, "labMonsterLV", 61, 11,210,5 )
UI_SetCaption( labMonsterLV, "Level" )
UI_SetTextColor( labMonsterLV, COLOR_BLACK )
UI_SetLabelExFont( labMonsterLV, DEFAULT_FONT, FALSE, COLOR_WHITE )

labMPOS= UI_CreateCompent( frmNpcShow, LABELEX_TYPE, "labMPOS",216, 11, 300,5 )
UI_SetCaption( labMPOS, "Coordinates" )
UI_SetTextColor( labMPOS, COLOR_BLACK )
UI_SetLabelExFont( labMPOS, DEFAULT_FONT, FALSE, COLOR_WHITE )

lstMonsterList = UI_CreateCompent( frmNpcShow, LIST_TYPE, "lstMonsterList", 392, 297, 10, 17 )
lstHelpButton = UI_GetList(lstMonsterList)
UI_SetListRowHeight(lstMonsterList,15)
UI_SetListIsMouseFollow( lstMonsterList, TRUE )

scrollid = UI_GetScroll( lstMonsterList )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/forsaken/item1.tga", COMPENT_BACK, 11, 1, 232, 190 )

id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, FALSE )
UI_SetSize( id, 11, 9 )

id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 43 )

id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, FALSE )
UI_SetSize( id, 11, 9 )

pgeSkill = UI_CreateCompent( frmNpcShow, PAGE_TYPE, "pgeSkill", 211, 233, 9, 82 )
UI_SetPageButton( pgeSkill, PAGE_BUTTON_CUSTOM, 48, 16 )

skillid = UI_CreatePageItem( pgeSkill )
ttlBattle = UI_GetPageItemObj( skillid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlBattle, "texture/ui/corsairs/npclist.tga", PAGE_ITEM_TITLE_NORMAL, 67, 34, 428, 136 )
UI_LoadImage( ttlBattle, "texture/ui/corsairs/npclist.tga", PAGE_ITEM_TITLE_ACTIVE, 67, 34, 428, 96 )
UI_SetPos( ttlBattle, 20, -32 )
UI_SetSize( ttlBattle, 67,34 )
UI_AddCompent( skillid, lstNpcList )
UI_AddCompent( skillid, labNPC )
UI_AddCompent( skillid, labNpcPos )
UI_AddCompent( skillid, labPOS )

skillid = UI_CreatePageItem( pgeSkill )
ttlBattle = UI_GetPageItemObj( skillid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlBattle, "texture/ui/corsairs/npclist.tga", PAGE_ITEM_TITLE_NORMAL, 67, 34, 428, 45 )
UI_LoadImage( ttlBattle, "texture/ui/corsairs/npclist.tga", PAGE_ITEM_TITLE_ACTIVE, 67, 34, 428, 5 )
UI_SetPos( ttlBattle, 85, -32 )
UI_SetSize( ttlBattle, 67, 34 )
UI_AddCompent( skillid, lstMonsterList )
UI_AddCompent( skillid, labMonster )
UI_AddCompent( skillid, labMonsterLV )
UI_AddCompent( skillid, labMPOS )

stateDrags = UI_CreateForm( "stateDrags", FALSE, 250, 120, 0, 160, TRUE, FALSE )	--198, 394
UI_ShowForm( stateDrags, FALSE )
UI_AddFormToTemplete( stateDrags, 1 )
UI_SetIsDrag( stateDrags, TRUE )
UI_FormSetIsEscClose( stateDrags, FALSE )
