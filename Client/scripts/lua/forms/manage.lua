----------------
-- Guild Form --
----------------
frmManage = UI_CreateForm( "frmManage", FALSE, 455, 373, 350, 200, TRUE, FALSE )
UI_FormSetHotKey( frmManage, ALT_KEY, HOTKEY_C )
UI_ShowForm( frmManage, FALSE )
UI_AddFormToTemplete( frmManage, 1 )
UI_SetIsDrag( frmManage, TRUE )
UI_SetFormStyleEx( frmManage, FORM_BOTTOM, 0, 45)

imgTradeTop = UI_CreateCompent( frmManage, IMAGE_TYPE, "imgTradeTop", 455, 373, 0, 0 )
UI_LoadImage( imgTradeTop, "texture/ui/corsairs/guild.png", NORMAL, 455, 373, 0, 0 )

btnClose = UI_CreateCompent( frmManage, BUTTON_TYPE, "btnClose", 14, 14, 436, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

-- Title Bar (Guild Name)
labTitleBar = UI_CreateCompent( frmManage, LABELEX_TYPE, "labTitleBar", 95, 13, 6, 4 )
UI_SetCaption( labTitleBar, "Guild Interface" )
UI_SetTextColor( labTitleBar, COLOR_BLACK )
UI_SetLabelExFont( labTitleBar, DEFAULT_FONT, TRUE, COLOR_WHITE )

----------------
--guild member--
----------------
lstNum = UI_CreateListView( frmManage, "lstNum", 322, 254, 123, 10, 6, 2 )
UI_ListViewSetTitle( lstNum, 0, 103, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstNum, 1, 81, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstNum, 2, 30, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstNum, 3, 28, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstNum, 4, 28, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstNum, 6, 30, "", 0, 0, 0, 0 )
UI_SetListRowHeight( lstNum, 18 )

scrollid = UI_GetScroll( lstNum )
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

----------------------
--apply guild member--
----------------------
lstAsk = UI_CreateListView( frmManage, "lstAsk", 322, 254, 123, 10, 3, 2 )
UI_ListViewSetTitle( lstAsk, 0, 103, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstAsk, 1, 81, "", 0, 0, 0, 0 )
UI_ListViewSetTitle( lstAsk, 2, 30, "", 0, 0, 0, 0 )
UI_SetListRowHeight( lstAsk, 18 )

scrollid = UI_GetScroll( lstAsk )
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

pgePublic = UI_CreateCompent( frmManage, PAGE_TYPE, "pgePublic", 185, 275, 11, 79 )
UI_SetPageButton( pgePublic, PAGE_BUTTON_CUSTOM, 48, 16 )

-- Guild Members tabs
skillid = UI_CreatePageItem( pgePublic )
ttlNum = UI_GetPageItemObj( skillid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlNum, "texture/ui/corsairs/SysBotton4.png", PAGE_ITEM_TITLE_NORMAL, 49, 31, 152, 120 )
UI_LoadImage( ttlNum, "texture/ui/corsairs/SysBotton4.png", PAGE_ITEM_TITLE_ACTIVE, 49, 31, 103, 120 )
UI_SetPos( ttlNum, 133, -48 )
UI_SetSize( ttlNum, 49, 30 )
UI_AddCompent( skillid, lstNum )

chkSortName = UI_CreateCompent(frmManage, CHECK_TYPE, "chkSortName", 10, 10, 148, -5)
UI_LoadImage(chkSortName, "texture/ui/corsairs/sort-arrows.tga", UNCHECKED, 15, 15, 0, 0)
UI_LoadImage(chkSortName, "texture/ui/corsairs/sort-arrows.tga", CHECKED, 15, 15, 0, 15)

chkSortClass = UI_CreateCompent(frmManage, CHECK_TYPE, "chkSortClass", 10, 10, 237, -5)
UI_LoadImage(chkSortClass, "texture/ui/corsairs/sort-arrows.tga", UNCHECKED, 15, 15, 0, 0)
UI_LoadImage(chkSortClass, "texture/ui/corsairs/sort-arrows.tga", CHECKED, 15, 15, 0, 15)

chkSortLevel = UI_CreateCompent(frmManage, CHECK_TYPE, "chkSortLevel", 10, 10, 295, -5)
UI_LoadImage(chkSortLevel, "texture/ui/corsairs/sort-arrows.tga", UNCHECKED, 15, 15, 0, 15)
UI_LoadImage(chkSortLevel, "texture/ui/corsairs/sort-arrows.tga", CHECKED, 15, 15, 0, 0)

UI_AddCompent( skillid, chkSortName )
UI_AddCompent( skillid, chkSortClass )
UI_AddCompent( skillid, chkSortLevel )


-- Apply Tab - 187, -44
skillid = UI_CreatePageItem( pgePublic )
ttlLive = UI_GetPageItemObj( skillid, PAGE_ITEM_TITLE )
UI_LoadImage( ttlLive,"texture/ui/corsairs/SysBotton4.png", PAGE_ITEM_TITLE_NORMAL, 49, 31, 208, 323 ) 
UI_LoadImage( ttlLive,"texture/ui/corsairs/SysBotton4.png", PAGE_ITEM_TITLE_ACTIVE, 49, 31, 159, 323 )
UI_SetPos( ttlLive, 187, -49 )
UI_SetSize( ttlLive, 49, 31 )


--[[ 
-- (Comment out, don't need this grid)
id1 = UI_CreateCompent( frmManage, IMAGE_TYPE, "id1", 256, 256, 118, -24 )
UI_LoadImage( id1, "texture/ui/manage4.tga", NORMAL, 256, 256, 0, 0 )
UI_AddCompent( skillid, id1 )

id2 = UI_CreateCompent( frmManage, IMAGE_TYPE, "id2", 256, 23, 118, 232 )
UI_LoadImage( id2, "texture/ui/ShipD.tga", NORMAL, 256, 23, 0, 162 )
UI_AddCompent( skillid, id2 )

id3 = UI_CreateCompent( frmManage, IMAGE_TYPE, "id3", 52, 256, 374, -24 )
UI_LoadImage( id3, "texture/ui/botton2.tga", NORMAL, 52, 256, 0, 0 )
UI_AddCompent( skillid, id3 )

id4 = UI_CreateCompent( frmManage, IMAGE_TYPE, "id4", 52, 23, 374, 232 )
UI_LoadImage( id4, "texture/ui/botton2.tga", NORMAL, 52, 23, 52, 0 )

UI_AddCompent( skillid, id4 )
-- (Comment out, don't need this grid)
]]

UI_AddCompent( skillid, lstAsk )

-- Guild Bank
skillid = UI_CreatePageItem( pgePublic )
ttlNum = UI_GetPageItemObj( skillid, PAGE_ITEM_TITLE )   --159 - 219
UI_LoadImage( ttlNum, "texture/ui/corsairs/SysBotton4.png", PAGE_ITEM_TITLE_NORMAL, 49, 35, 152+56, 219 )
UI_LoadImage( ttlNum, "texture/ui/corsairs/SysBotton4.png", PAGE_ITEM_TITLE_ACTIVE, 49, 35, 159, 219 )
UI_SetPos( ttlNum, 187+50, -49 )
UI_SetSize( ttlNum, 49, 35 )

guildBank = UI_CreateCompent( frmManage, GOODS_GRID_TYPE, "guildBank", 291, 265, 130, -11 ) 

id1 = UI_CreateCompent( frmManage, IMAGE_TYPE, "id1", 291, 265, 127, -15 )
UI_LoadImage( id1, "texture/ui/Corsairs/guildbank.png", NORMAL, 291, 265, 0, 0 )
UI_AddCompent( skillid, id1 )
UI_AddCompent( skillid, id1 )

UI_SetGridSpace( guildBank, 4, 4)
UI_SetGridContent( guildBank, 4, 8 )
UI_SetGridUnitSize( guildBank, 32, 32 )
--UI_SetMargin( guildBank, 21, 4, 0, 0 )

-- Guild Deposit
btngoldput = UI_CreateCompent( frmManage, BUTTON_TYPE, "btngoldput", 41, 19, 367, 206 )
UI_LoadButtonImage( btngoldput, "texture/ui/corsairs/SysBotton4.png", 41, 19, 0, 276, TRUE )

-- Guild Withdraw
btngoldtake = UI_CreateCompent( frmManage, BUTTON_TYPE, "btngoldtake", 41, 19, 367, 226 )
UI_LoadButtonImage( btngoldtake, "texture/ui/corsairs/SysBotton4.png", 41, 19, 0, 295, TRUE )

-- Guild Money Box
imgGuildMoney = UI_CreateCompent( frmManage, IMAGE_TYPE, "imgGuildMoney", 193, 18, 161, 217 )
UI_LoadImage( imgGuildMoney, "texture/ui/ShipBuild4.tga", NORMAL, 193, 18, 4, 202 )

-- Guild Money Label
labGuildMoney = UI_CreateCompent( frmManage, LABELEX_TYPE, "labGuildMoney", 95, 13, 180, 221 )
UI_SetCaption( labGuildMoney, "0" )
UI_SetTextColor( labGuildMoney, COLOR_BLACK )
UI_SetLabelExFont( labGuildMoney, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Guild Money Box
imgMoneyLogo = UI_CreateCompent( frmManage, IMAGE_TYPE, "imgMoneyLogo", 20, 28, 136, 211 )
UI_LoadImage( imgMoneyLogo, "texture/ui/PublicC.tga", NORMAL, 20, 28, 228, 110 )

UI_AddCompent( skillid, btngoldtake )
UI_AddCompent( skillid, btngoldput )
UI_AddCompent( skillid, imgGuildMoney )
UI_AddCompent( skillid, labGuildMoney )
UI_AddCompent( skillid, imgMoneyLogo )
UI_AddCompent( skillid, guildBank )

bankLocked = UI_CreateCompent( frmManage, IMAGE_TYPE, "bankLocked", 291, 265, 127, -15 )
UI_LoadImage( bankLocked, "texture/ui/Corsairs/guildbankLocked.png", NORMAL, 291, 265, 0, 0 )
UI_AddCompent( skillid, bankLocked )

---------------------
-- Guild Bank Logs --
---------------------

skillid = UI_CreatePageItem( pgePublic)
ttlNum = UI_GetPageItemObj (skillid, PAGE_ITEM_TITLE)
UI_LoadImage( ttlNum, "texture/ui/corsairs/SysBotton4.png", PAGE_ITEM_TITLE_NORMAL, 48, 31, 111, 323 )
UI_LoadImage( ttlNum, "texture/ui/corsairs/SysBotton4.png", PAGE_ITEM_TITLE_ACTIVE, 49, 31, 62, 323 )
UI_SetPos(ttlNum, 187+108, -49)
UI_SetSize(ttlNum, 48, 31)

imgBankLog = UI_CreateCompent( frmManage, IMAGE_TYPE, "imgBankLog", 290, 264, 130, -15)
UI_LoadImage(imgBankLog, "texture/ui/corsairs/imgBankLog.png", NORMAL, 290, 264, 0, 0)
UI_AddCompent (skillid, imgBankLog)

listBankLog = UI_CreateListView( frmManage, "listBankLog", 290, 278, 122, -24, 0, 0 )
UI_SetListRowHeight( listBankLog, 19 )
UI_SetMargin(listBankLog, 10, 0, 0, 0 ) 
UI_SetListIsMouseFollow( listBankLog, TRUE)
UI_ListViewSetTitle( listBankLog, 0, 50, "", 0, 0 ,0, 0 )
--UI_ListViewSetTitle( listBankLog, 1, 81, "", 20, 20, 0, 0 )

btnNext = UI_CreateCompent(frmManage, BUTTON_TYPE, "btnNext", 13, 13, 405, 260)
UI_LoadButtonImage(btnNext, "texture/ui/SystemBotton3.tga", 13, 13, 0, 211, TRUE)

btnPrev = UI_CreateCompent(frmManage, BUTTON_TYPE, "btnPrev", 13, 13, 375, 260)
UI_LoadButtonImage(btnPrev, "texture/ui/SystemBotton3.tga", 13, 13, 0, 197, TRUE) --241, 409




UI_AddCompent(skillid, listBankLog)
UI_AddCompent(skillid, btnNext)
UI_AddCompent(skillid, btnPrev)

--UI_AddCompent(skillid, scrollid)
--UI_AddCompent(skillid, upbtn)
--UI_AddCompent(skillid, scrollbar)
--UI_AddCompent(skillid, downbtn)

-- Guild Name
labName = UI_CreateCompent( frmManage, LABELEX_TYPE, "labName", 95, 13, 20, 80 )
UI_SetCaption( labName, "Forbidden words" )
UI_SetTextColor( labName, COLOR_BLACK )
UI_SetLabelExFont( labName, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Founder
labPeople = UI_CreateCompent( frmManage, LABELEX_TYPE, "labPeople", 95, 13, 20, 95 + 19 )
UI_SetCaption( labPeople, "Forbidden words" )
UI_SetTextColor( labPeople, COLOR_BLACK )
UI_SetLabelExFont( labPeople, DEFAULT_FONT, TRUE, COLOR_WHITE )

-- Members / Maximum
labNum = UI_CreateCompent( frmManage, LABELEX_TYPE, "labNum", 50, 13, 42, 91 + (30 * 2) )
UI_SetCaption( labNum, "Pirate Guild" )
UI_SetTextColor( labNum, COLOR_BLACK )
UI_SetLabelExFont( labNum, DEFAULT_FONT, TRUE, COLOR_WHITE )


-- Recruit Button
btnYes = UI_CreateCompent( frmManage, BUTTON_TYPE, "btnYes", 41, 19, 21, 289 )
UI_LoadButtonImage( btnYes, "texture/ui/corsairs/SysBotton4.png", 41, 19, 0, 19, TRUE )

-- Reject Button
btnNo = UI_CreateCompent( frmManage, BUTTON_TYPE, "btnNo", 41, 19, 21, 316 )
UI_LoadButtonImage( btnNo, "texture/ui/corsairs/SysBotton4.png", 41, 19, 0, 57, TRUE )

-- Remove / Kick Button
btnkick = UI_CreateCompent( frmManage, BUTTON_TYPE, "btnkick", 41, 19, 71, 289 )
UI_LoadButtonImage( btnkick, "texture/ui/corsairs/SysBotton4.png", 41, 19, 0, 38, TRUE )

-- Disband Button
btnDisband = UI_CreateCompent( frmManage, BUTTON_TYPE, "btnDisband", 41, 19, 71, 316 )
UI_LoadButtonImage( btnDisband, "texture/ui/corsairs/SysBotton4.png", 41, 19, 0, 76, TRUE )

-- Leave Button
btnLeave = UI_CreateCompent( frmManage, BUTTON_TYPE, "btnLeave", 41, 19, 71, 316 )
UI_LoadButtonImage( btnLeave, "texture/ui/SystemBotton3.tga", 41, 19, 0, 152, TRUE )

-- Permissions Button
btnperm = UI_CreateCompent( frmManage, BUTTON_TYPE, "btnperm", 56, 19, 314, 343 )
UI_LoadButtonImage( btnperm, "texture/ui/corsairs/SysBotton4.png", 56, 19, 0, 256, TRUE )

-- Motto Button
btnMaxim = UI_CreateCompent( frmManage, BUTTON_TYPE, "btnMaxim", 41, 19, 270, 343 )
-- UI_LoadButtonImage( btnMaxim, "texture/ui/ShipBuildD.tga", 53, 19, 0, 159, TRUE )
UI_LoadButtonImage( btnMaxim, "texture/ui/corsairs/coButtons.png", 41, 19, 0, 95, TRUE )

-- Motto
labMaxim = UI_CreateCompent( frmManage, LABELEX_TYPE, "labMaxim", 244, 19, 15, 347 )
UI_SetCaption( labMaxim, "Forbidden words" )
UI_SetTextColor( labMaxim, COLOR_BLACK )
UI_SetLabelExFont( labMaxim, DEFAULT_FONT, TRUE, COLOR_WHITE )

-----------------------------------------------------------------------
-- Guild Motto
-----------------------------------------------------------------------
frmEditMaxim = UI_CreateForm( "frmEditMaxim",  FALSE, 191, 106, 397, 500, TRUE, FALSE )
UI_LoadFormImage( frmEditMaxim, "texture/ui/new/guildPerms.png", 191, 106, 1, 310, "", 0, 0 )
UI_ShowForm( frmEditMaxim, FALSE )
UI_AddFormToTemplete( frmEditMaxim, FORM_MAIN )
UI_SetFormStyle( frmEditMaxim, 0 )
UI_SetIsDrag( frmEditMaxim, TRUE )
--UI_FormSetHotKey( frmEditMaxim, ALT_KEY, HOTKEY_W ) 


btnClose = UI_CreateCompent( frmEditMaxim, BUTTON_TYPE, "btnClose", 25, 25, 166, 1 )
UI_LoadButtonImage( btnClose, "texture/ui/new/Quest.png", 25, 25, 1, 455, TRUE )

UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )


imgMaxim = UI_CreateCompent( frmEditMaxim, IMAGE_TYPE, "imgMaxim", 172, 21, 8, 40 )
UI_LoadImage( imgMaxim, "texture/ui/new/guildPerms.png", NORMAL, 172, 21, 1, 425 )

labMaxim = UI_CreateCompent( frmEditMaxim, LABELEX_TYPE, "labMaxim", 58, 11, 67, 9 )
UI_SetCaption( labMaxim, "Edit Motto" )
UI_SetTextColor( labMaxim, COLOR_WHITE )
UI_SetLabelExFont( labMaxim, DEFAULT_FONT, TRUE, COLOR_BLACK )

edtMaxim = UI_CreateCompent( frmEditMaxim, EDIT_TYPE, "edtMaxim", 179, 11, 15, 44 )
UI_SetTextColor( edtMaxim, COLOR_WHITE )
UI_SetEditMaxNum( edtMaxim, 26 )
UI_SetEditMaxNumVisible( edtMaxim, 26 )

btnYes = UI_CreateCompent( frmEditMaxim, BUTTON_TYPE, "btnYes", 54, 16, 40, 80 )
UI_LoadButtonImage( btnYes, "texture/ui/new/settings.png", 54, 16, 270, 478, TRUE )
UI_SetEditEnterButton( edtTradeGold, btnYes )

btnNo = UI_CreateCompent( frmEditMaxim, BUTTON_TYPE, "btnNo", 54, 16, 110, 80 )
UI_LoadButtonImage( btnNo, "texture/ui/new/settings.png", 54, 16, 270, 460, TRUE )

UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )


----------------------------------------------------------------
-- Guild Permissions | Coded by Billy , GUI by Foxseiz
----------------------------------------------------------------
frmGuildPerm = UI_CreateForm( "frmGuildPerm", FALSE, 190, 281, 805, 351, FALSE, FALSE )
UI_ShowForm( frmGuildPerm, FALSE )
UI_AddFormToTemplete( frmGuildPerm, 1 )
UI_SetIsDrag( frmGuildPerm, TRUE )

imgVideoT = UI_CreateCompent( frmGuildPerm, IMAGE_TYPE, "imgVideoT", 190, 281, 0, 0 )
UI_LoadImage( imgVideoT, "texture/ui/corsairs/guildPerms.png", NORMAL, 190, 281, 0, 0 )

btnClose = UI_CreateCompent( frmGuildPerm, BUTTON_TYPE, "btnClose", 14, 14, 165, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labGame = UI_CreateCompent( frmGuildPerm, LABELEX_TYPE, "labGame", 47, 11, 7, 3 )
UI_SetCaption( labGame, "Guild Permissions" )
UI_SetTextColor( labGame, COLOR_BLACK )
UI_SetLabelExFont( labGame, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnYesPerm = UI_CreateCompent( frmGuildPerm, BUTTON_TYPE, "btnYesPerm", 41, 19, 84, 225+29 )
UI_LoadButtonImage( btnYesPerm, "texture/ui/corsairs/coButtons.png", 41, 19, 0, 152, TRUE )
UI_SetButtonModalResult( btnYesPerm, BUTTON_CLOSE )

btnNo = UI_CreateCompent( frmGuildPerm, BUTTON_TYPE, "btnNo", 41, 19, 132, 225+29 )
UI_LoadButtonImage( btnNo, "texture/ui/corsairs/coButtons.png", 41, 19, 0, 133, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )


local perms = {
	"Speak",
	"Manage Permissions",
	"View Bank",
	"Deposit Bank",
	"Withdraw Bank",
	"Recruit",
	"Kick",
	"Change Motto",
	--"Manage Stats",	
	--"Enter Guild House",
	--"Place Object",
	--"Remove Object",
	"Disband Guild",
	"Leader",
}



trvEditor = UI_CreateCompent( frmGuildPerm, TREE_TYPE, "trvEditor", 180, 200, 0, 35 )


for i,v in ipairs(perms) do			
	itemid = UI_CreateTextItem( v, COLOR_RED )--permission name
	sndNode = UI_CreateSingleNode( trvEditor, itemid, -1 )--add to tree list
end



UI_SetIsDrag( trvEditor, TRUE )

UI_TreeLoadImage( trvEditor, enumTreeAddImage, "texture/ui/QQ2.tga", 20,16,85,222, 20, 16)
UI_TreeLoadImage( trvEditor, enumTreeSubImage, "texture/ui/QQ2.tga", 20,16,85,240, 20, 16)

scrollid = UI_GetScroll( trvEditor )
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


-- Predefined Presets

local predefX = 36
local predefY = 254

btnPredef = UI_CreateCompent( frmGuildPerm, BUTTON_TYPE, "btnPredef", 41, 19, predefX, predefY )
UI_LoadButtonImage( btnPredef, "texture/ui/corsairs/coButtons.png", 41, 19, 0, 171, TRUE )

local predefPicX = 41
local predefPicY = 19

for i = 1,6 do
	predef = UI_CreateCompent( frmGuildPerm, BUTTON_TYPE, "btnPredef"..i, predefPicX, predefPicY, predefX, predefY-(predefPicY*i) )
	UI_LoadImage( predef, "texture/ui/corsairs/coButtons.png", 0, predefPicX,predefPicY,169 + ( i-1) * predefPicX, 168 )
	UI_LoadImage( predef, "texture/ui/corsairs/coButtons.png", 2, 41,predefPicY,169 + ( i-1) * predefPicX, 168 + predefPicY )
	UI_LoadImage( predef, "texture/ui/corsairs/coButtons.png", 1, 41,predefPicY,169 + ( i-1) * predefPicX, 168 + 2*predefPicY )
	UI_SetIsShow(predef,0)
end
