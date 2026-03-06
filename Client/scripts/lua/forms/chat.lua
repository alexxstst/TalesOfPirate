-----------------------------------------------------------------------
-- QQ
-----------------------------------------------------------------------
-- QQ
frmQQMin = UI_CreateForm( "frmQQMin", FALSE, 146, 20, 0, 160, TRUE, FALSE )
--UI_FormSetHotKey( frmQQMin, ALT_KEY, HOTKEY_N )
UI_ShowForm( frmQQMin, FALSE )
UI_AddFormToTemplete( frmQQMin, 1 )
UI_SetIsDrag( frmQQMin, TRUE )

-- 
imgQQMin = UI_CreateCompent( frmQQMin, IMAGE_TYPE, "imgQQMin", 146, 20, 0, 0 )
UI_LoadImage( imgQQMin, "texture/ui/Word.dds", NORMAL, 146, 20, 1, 157 )

--
btnQQMinMax = UI_CreateCompent( frmQQMin, BUTTON_TYPE, "btnQQMinMax", 14, 14, 115, 3 )
UI_LoadButtonImage( btnQQMinMax, "texture/ui/PublicC.tga", 14, 14, 161, 175, TRUE )

--
btnClose = UI_CreateCompent( frmQQMin, BUTTON_TYPE, "btnClose", 14, 14, 130, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )


-- title
labTitle = UI_CreateCompent( frmQQMin, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Friend List")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

---------------
--friend list--
---------------
frmQQ = UI_CreateForm( "frmQQ", FALSE, 148, 354, 652, 200, TRUE, FALSE )
UI_FormSetHotKey( frmQQ, ALT_KEY, HOTKEY_F )
UI_ShowForm( frmQQ, FALSE )
UI_AddFormToTemplete( frmQQ, 1 )
UI_SetIsDrag( frmQQ, TRUE )
UI_SetFormStyleEx( frmQQ, FORM_BOTTOM, 0, 45)

imgQQTop = UI_CreateCompent( frmQQ, IMAGE_TYPE, "imgQQTop", 148, 256, 0, 0 )
UI_LoadImage( imgQQTop, "texture/ui/Speak.dds", NORMAL, 148, 256, 0, 0 )
imgQQDown = UI_CreateCompent( frmQQ, IMAGE_TYPE, "imgQQDown", 148, 98, 0, 256 )
UI_LoadImage( imgQQDown, "texture/ui/QQ2.tga", NORMAL, 148, 98, 82, 78 )

labTitle = UI_CreateCompent( frmQQ, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Chat")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMineName = UI_CreateCompent( frmQQ, LABELEX_TYPE, "labMineName", 96, 11, 40, 29 )
UI_SetCaption( labMineName, "Child of Demon what is that" )
UI_SetTextColor( labMineName, COLOR_BLACK )
UI_SetLabelExFont( labMineName, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgHead = UI_CreateCompent( frmQQ, IMAGE_TYPE, "imgHead", 32, 32, 1, 20 )
UI_LoadImage( imgHead, "texture/ui/main800.tga", NORMAL, 32, 32, 37, 201 )

btnClose = UI_CreateCompent( frmQQ, BUTTON_TYPE, "btnClose", 14, 14, 127, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

btnAdd = UI_CreateCompent( frmQQ, BUTTON_TYPE, "btnAdd", 16, 16, 30, 331 )
UI_LoadButtonImage( btnAdd, "texture/ui/SysBotton2.dds", 16, 16, 127, 84, TRUE )
UI_SetHint( btnAdd, "Add" )

btnEdit = UI_CreateCompent( frmQQ, BUTTON_TYPE, "btnEdit", 14, 14, 50, 332 )
UI_LoadButtonImage( btnEdit, "texture/ui/SystemBotton3.tga", 14, 14, 167, 19, TRUE )
UI_SetHint( btnEdit, "Info" )


-- Block button
btnBlock = UI_CreateCompent( frmQQ, BUTTON_TYPE, "btnBlock", 14, 14, 70, 332 )
UI_LoadButtonImage( btnBlock, "texture/ui/corsairs/coButtons.png", 14, 14, 276, 51, TRUE )
UI_SetHint( btnBlock, "Block" )

trvEditor = UI_CreateCompent( frmQQ, TREE_TYPE, "trvEditor", 142, 265, 5, 62 )
itemid = UI_CreateNoteGraphItem( "texture/ui/QQ2.tga", 87,16,105,222, 0, "Chat", 5,2 )
gridid = UI_CreateGridNode( trvEditor, itemid, 1, 32, 32, -1 )
	
itemid = UI_CreateNoteGraphItem( "texture/ui/QQ2.tga", 87,16,105,222, 0, "Friend", 5,2 )
gridid = UI_CreateGridNode( trvEditor, itemid, 1, 32, 32, -1 )
	
itemid = UI_CreateNoteGraphItem( "texture/ui/QQ2.tga", 87,16,105,222, 0, "Party", 5,2 )
gridid = UI_CreateGridNode( trvEditor, itemid, 1, 32, 32, -1 )
	
itemid = UI_CreateNoteGraphItem( "texture/ui/QQ2.tga", 87,16,105,222, 0, "Guild", 5,2 )
gridid = UI_CreateGridNode( trvEditor, itemid, 1, 32, 32, -1 )

--itemid = UI_CreateNoteGraphItem( "texture/ui/QQ2.tga", 87,16,105,222, 0, "Passer-by", 5,2 )
--gridid = UI_CreateGridNode( trvEditor, itemid, 1, 32, 32, -1 )

itemid = UI_CreateNoteGraphItem( "texture/ui/QQ2.tga", 87,16,105,222, 0, "Mentor", 5,2 )
gridid = UI_CreateGridNode( trvEditor, itemid, 1, 32, 32, -1 )

itemid = UI_CreateNoteGraphItem( "texture/ui/QQ2.tga", 87,16,105,222, 0, "Disciple", 5,2 )
gridid = UI_CreateGridNode( trvEditor, itemid, 1, 32, 32, -1 )

itemid = UI_CreateNoteGraphItem( "texture/ui/QQ2.tga", 87,16,105,222, 0, "GM", 5,2 )
gridid = UI_CreateGridNode( trvEditor, itemid, 1, 32, 32, -1 )

itemid = UI_CreateNoteGraphItem( "texture/ui/QQ2.tga", 87,16,105,222, 0, "Blocked", 5,2 )
gridid = UI_CreateGridNode( trvEditor, itemid, 1, 32, 32, -1 )

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


-----------------------------------------------------------------------
-- right click
-----------------------------------------------------------------------
frndMouseRight = UI_CreateCompent( frmQQ, MENU_TYPE, "frndMouseRight", 92, 97, 240, 468 )
UI_MenuLoadImage( frndMouseRight, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( frndMouseRight, 10, 5, 5, 5 )

UI_MenuLoadSelect( frndMouseRight, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )

UI_MenuAddText( frndMouseRight, "Detailed Info" )
UI_MenuAddText( frndMouseRight, "Send message" )
UI_MenuAddText( frndMouseRight, "Delete Friend" )
UI_MenuAddText( frndMouseRight, "PM" )

groupMouseRight = UI_CreateCompent( frmQQ, MENU_TYPE, "groupMouseRight", 92, 97, 240, 468 )
UI_MenuLoadImage( groupMouseRight, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( groupMouseRight, 10, 5, 5, 5 )


UI_MenuLoadSelect( groupMouseRight, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )

UI_MenuAddText( groupMouseRight, "Add Friend" )
UI_MenuAddText( groupMouseRight, "Send message" )
UI_MenuAddText( groupMouseRight, "PM" )
UI_MenuAddText( groupMouseRight, "Kick" )

roadMouseRight = UI_CreateCompent( frmQQ, MENU_TYPE, "roadMouseRight", 92, 97, 240, 468 )
UI_MenuLoadImage( roadMouseRight, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( roadMouseRight, 10, 5, 5, 5 )

UI_MenuLoadSelect( roadMouseRight, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )

UI_MenuAddText( roadMouseRight, "Add Friend" )
UI_MenuAddText( roadMouseRight, "Party Invite" )
UI_MenuAddText( roadMouseRight, "Send message" )
UI_MenuAddText( roadMouseRight, "PM" )

ChatMouseRight = UI_CreateCompent( frmQQ, MENU_TYPE, "ChatMouseRight", 92, 97, 240, 468 )
UI_MenuLoadImage( ChatMouseRight, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( ChatMouseRight, 10, 5, 5, 5 )

UI_MenuLoadSelect( ChatMouseRight, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )

UI_MenuAddText( ChatMouseRight, "Exit conversation" )

MasterRight = UI_CreateCompent( frmQQ, MENU_TYPE, "MasterRight", 92, 97, 240, 468 )
UI_MenuLoadImage( MasterRight, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( MasterRight, 10, 5, 5, 5 )

UI_MenuLoadSelect( MasterRight, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )

UI_MenuAddText( MasterRight, "Friend" )
UI_MenuAddText( MasterRight, "Party" )
UI_MenuAddText( MasterRight, "Message" )
UI_MenuAddText( MasterRight, "PM" )
UI_MenuAddText( MasterRight, "Delete" )

StudentRight = UI_CreateCompent( frmQQ, MENU_TYPE, "StudentRight", 92, 97, 240, 468 )
UI_MenuLoadImage( StudentRight, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( StudentRight, 10, 5, 5, 5 )

UI_MenuLoadSelect( StudentRight, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )

UI_MenuAddText( StudentRight, "Friend" )
UI_MenuAddText( StudentRight, "Party" )
UI_MenuAddText( StudentRight, "Message" )
UI_MenuAddText( StudentRight, "PM" )
UI_MenuAddText( StudentRight, "Delete" )

-- Block
BlockedRight = UI_CreateCompent( frmQQ, MENU_TYPE, "BlockedRight", 92, 97, 240, 468 )
UI_MenuLoadImage( BlockedRight, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( BlockedRight, 10, 5, 5, 5 )

UI_MenuLoadSelect( BlockedRight, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )

UI_MenuAddText( BlockedRight, "Delete" )


------------------
-- send message --
------------------
frmChat = UI_CreateForm( "frmChat", FALSE, 377, 274, 232, 163, TRUE, FALSE )
--UI_FormSetHotKey( frmChat, ALT_KEY, HOTKEY_C )
UI_ShowForm( frmChat, FALSE )
UI_AddFormToTemplete( frmChat, 1 )
UI_SetIsDrag( frmChat, TRUE )
--UI_FormSetIsEscClose( frmChat, FALSE )

imgChatTL = UI_CreateCompent( frmChat, IMAGE_TYPE, "imgChatTL", 256, 256, 0, 0 )
UI_LoadImage( imgChatTL, "texture/ui/Talk1.tga", NORMAL, 256, 256, 0, 0 )

imgChatTR = UI_CreateCompent( frmChat, IMAGE_TYPE, "imgChatTR", 121, 256, 256, 0 )
UI_LoadImage( imgChatTR, "texture/ui/speak2.tga", NORMAL, 121, 256, 0, 0 )
--title
labTitle = UI_CreateCompent( frmChat, LABELEX_TYPE, "labTitle", 60, 14, 10, 4 )
UI_SetCaption( labTitle, "Dialogue")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

--labName = UI_CreateCompent( frmChat, LABELEX_TYPE, "labName", 96, 11, 7, 4 )
--UI_SetCaption( labName, "012345678901234567890123456789" )
--UI_SetTextColor( labName, COLOR_BLACK )
--UI_SetLabelExFont( labName, DEFAULT_FONT, TRUE, COLOR_WHITE )

labName1 = UI_CreateCompent( frmChat, LABELEX_TYPE, "labName1", 96, 11, 52, 27 )
UI_SetCaption( labName1, "012345678901234567890123456789" )
UI_SetTextColor( labName1, COLOR_BLACK )
UI_SetLabelExFont( labName1, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmChat, BUTTON_TYPE, "btnClose", 14, 14, 355, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )


btnMin = UI_CreateCompent( frmChat, BUTTON_TYPE, "btnMin", 14, 14, 342, 3 )
UI_LoadButtonImage( btnMin, "texture/ui/PublicC.tga", 14, 14, 2, 75, TRUE )

btnSend = UI_CreateCompent( frmChat, BUTTON_TYPE, "btnSend", 53, 19, 207, 177 )
UI_LoadButtonImage( btnSend, "texture/ui/Quest1.tga", 53, 19, 20, 218, TRUE )
UI_SetHint( btnSend, "Alt+X to send message" )

edtChat = UI_CreateCompent( frmChat, EDIT_TYPE, "edtChat", 246, 51, 14, 198 )
UI_SetTextColor( edtChat, COLOR_BLACK )
UI_SetEditMaxNum( edtChat, 64 )
UI_SetEditMaxNumVisible( edtChat, 64 )
UI_SetEditCursorColor( edtChat, COLOR_BLACK )

UI_SetEditEnterButton( edtChat,btnSend )

memChat = UI_CreateCompent( frmChat, RICHMEMO_TYPE, "memChat", 254, 120, 14, 55 )
--memList = UI_GetList( memChat )
--UI_SetListRowHeight( memList, 18 )
--UI_SetMargin( memChat, 2, 2, 2, 2 )
--UI_SetIsShow( memChat , TRUE)
--UI_SetTextColor( memChat, COLOR_BLACK )
--UI_SetCaption( memChat, "" ) 
--UI_SetMemoMaxNumPerRow( memChat, 42 )
--UI_SetMemoPageShowNum( memChat, 6 )
--UI_SetMemoRowHeight( memChat , 21 )

scrollid = UI_GetScroll( memChat )
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

lstChat = UI_CreateCompent( frmChat, LIST_TYPE, "lstChat", 105, 207, 275, 51 )
UI_SetMargin( lstChat, 1, 4, 3, 5 )
UI_SetListRowHeight( lstChat, 38 )

scrollid = UI_GetScroll( lstChat )
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

labParty = UI_CreateCompent( frmChat, LABELEX_TYPE, "labParty", 96, 11, 280, 36 )
UI_SetCaption( labParty, "Members (36)" )
UI_SetTextColor( labParty, COLOR_BLACK )
UI_SetLabelExFont( labParty, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnBrow = UI_CreateCompent( frmChat, BUTTON_TYPE, "btnBrow", 16, 16, 51, 179 )
UI_LoadButtonImage( btnBrow, "texture/ui/SysBotton2.dds", 16, 16, 119, 212, TRUE )
UI_SetHint( btnBrow, "Emoticon" )

btnClean = UI_CreateCompent( frmChat, BUTTON_TYPE, "btnClean", 16, 16, 19, 179 )
UI_LoadButtonImage( btnClean, "texture/ui/SysBotton2.dds", 16, 16, 119, 193, TRUE )
UI_SetHint( btnClean, "Clear Screen" )

btnChat = UI_CreateCompent( frmChat, BUTTON_TYPE, "btnChat", 16, 16, 84, 179 )
UI_LoadButtonImage( btnChat, "texture/ui/SystemBotton3.tga", 16, 16, 73, 197, TRUE )
UI_SetHint( btnChat, "Save message log" )

btnQuit = UI_CreateCompent( frmChat, BUTTON_TYPE, "btnQuit", 13, 15, 120, 179 )
UI_LoadButtonImage( btnQuit, "texture/ui/SystemBotton3.tga", 13, 15, 73, 212, TRUE )
UI_SetHint( btnQuit, "Exit conversation" )

grdFace = UI_CreateCompent( frmChat, GRID_TYPE, "grdFace", 142, 170, 293, -170 ) 
UI_LoadImage( grdFace,  "texture/ui/QQ.dds", NORMAL, 142, 170, 0, 0 )
UI_SetGridUnitSize( grdFace, 20, 20 )
UI_SetIsShow( grdFace, FALSE )
UI_SetMargin( grdFace, 11, 5, 5, 5 )
UI_SetGridSpace( grdFace, 4, 4 )
UI_SetGridIsDragSize( grdFace, FALSE )
UI_GridLoadSelectImage( grdFace, "texture/ui/StartF.dds", 32, 32, 123, 99 )
n=0
while 1 do
	name = "texture/ui/face/"..(n+1)..".tga"   	
    	UI_AddFaceToGrid( grdFace, name, 16, 16, 0, 0, 1, 0 )	
	UI_SetTextParse( n , name, 16, 16 , 0 , 0 ,1  )
	n = n + 1    	
    	if n>=21 then break end
end

ListMouseRight = UI_CreateCompent( frmChat, MENU_TYPE, "ListMouseRight", 92, 97, 240, 468 )
UI_MenuLoadImage( ListMouseRight, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( ListMouseRight, 10, 5, 5, 5 )

UI_MenuLoadSelect( ListMouseRight, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )

UI_MenuAddText( ListMouseRight, "Friend" )

-----------------------------------------------------------------------
-- ()
-----------------------------------------------------------------------
-- 
frmChatMd = UI_CreateForm( "frmChatMd", FALSE, 203, 135, 232, 163, TRUE, FALSE )
--UI_FormSetHotKey( frmChatMd, ALT_KEY, HOTKEY_C )
UI_ShowForm( frmChatMd, FALSE )
UI_AddFormToTemplete( frmChatMd, 1 )
UI_SetIsDrag( frmChatMd, TRUE )
UI_FormSetIsEscClose( frmChatMd, FALSE )

-- 
imgChatA = UI_CreateCompent( frmChatMd, IMAGE_TYPE, "imgChatA", 203, 135, 0, 0 )
UI_LoadImage( imgChatA, "texture/ui/TalkM.dds", NORMAL, 203, 135, 0, 0 )

--
labName = UI_CreateCompent( frmChatMd, LABELEX_TYPE, "labName", 96, 11, 7, 4 )
--UI_SetCaption( labName, "012345678901234567890123456789" )
UI_SetTextColor( labName, COLOR_BLACK )
UI_SetLabelExFont( labName, DEFAULT_FONT, TRUE, COLOR_WHITE )

--
btnClose = UI_CreateCompent( frmChatMd, BUTTON_TYPE, "btnClose", 14, 14, 182, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

--
btnMin = UI_CreateCompent( frmChatMd, BUTTON_TYPE, "btnMin", 14, 14, 152, 3 )
UI_LoadButtonImage( btnMin, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

--
btnReturn = UI_CreateCompent( frmChatMd, BUTTON_TYPE, "btnReturn", 14, 14, 167, 3 )
UI_LoadButtonImage( btnReturn, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

--
btnSend = UI_CreateCompent( frmChatMd, BUTTON_TYPE, "btnSend", 16, 13, 185, 117 )
UI_LoadButtonImage( btnSend, "texture/ui/PublicC.tga", 16, 13, 49, 89, TRUE )

--
edtTradeGold = UI_CreateCompent( frmChatMd, EDIT_TYPE, "edtTradeGold", 171, 25, 17, 112 )
UI_SetTextColor( edtTradeGold, COLOR_BLACK )
UI_SetEditMaxNum( edtTradeGold, 29 )
UI_SetEditMaxNumVisible( edtTradeGold, 29 )

--
--memCtrl = UI_CreateCompent( frmChatMd, MEMO_TYPE, "memCtrl", 183, 79, 13, 23 )
--UI_SetTextColor( memCtrl, COLOR_BLACK )
--UI_SetCaption( memCtrl, "" ) 
--UI_SetMemoMaxNumPerRow(memCtrl, 42 )
--UI_SetMemoPageShowNum(memCtrl, 6 )
--UI_SetMemoRowHeight( memCtrl , 21 )

--
memCtrl = UI_CreateCompent( frmChatMd, LIST_TYPE, "memCtrl", 183, 79, 13, 23 )
listid1 = UI_GetList( memCtrl )
UI_SetListRowHeight( listid1, 18 )
UI_SetMargin( memChat, 2, 2, 2, 2 )
UI_SetIsShow( memChat , TRUE)


UI_SetEditEnterButton( edtTradeGold,btnSend )

--
scrollid = UI_GetScroll( memCtrl )
UI_SetSize( scrollid, 11, 1 )
UI_LoadImage( scrollid, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 1, 194, 13 )

--
id = UI_GetScrollObj( scrollid, SCROLL_UP )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )

--
id = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( id, "texture/ui/PublicC.tga", COMPENT_BACK, 11, 43, 166, 10 )
UI_SetSize( id, 11, 20 )

--
id = UI_GetScrollObj( scrollid, SCROLL_DOWN )
UI_LoadButtonImage( id, "texture/ui/PublicC.tga", 11, 9, 166, 0, TRUE )
UI_SetSize( id, 11, 9 )


------------------------------
--- chat pop up ui by aLex ---
------------------------------
frmChatMin = UI_CreateForm( "frmChatMin", FALSE, 185, 20, 150, 150, TRUE, FALSE )
--UI_FormSetHotKey( frmChatMin, ALT_KEY, HOTKEY_N)
UI_ShowForm( frmChatMin, FALSE )
UI_AddFormToTemplete( frmChatMin, 1 )
UI_SetIsDrag( frmChatMin, TRUE )
UI_FormSetIsEscClose( frmChatMin, FALSE )
--UI_SetFormStyle( frmChatMin, 0 )
--UI_SetAlpha( frmChatMin, 200 )

imgItemMin = UI_CreateCompent( frmChatMin, IMAGE_TYPE, "imgItemMin", 185, 20, 0, 0 )
UI_LoadImage( imgItemMin, "texture/ui/QQ.dds", NORMAL, 185, 20, 0, 197 )

btnMax = UI_CreateCompent( frmChatMin, BUTTON_TYPE, "btnMax", 14, 14, 154, 3 )
UI_LoadButtonImage( btnMax, "texture/ui/PublicC.tga", 14, 14, 161, 175, TRUE )

btnClose = UI_CreateCompent( frmChatMin, BUTTON_TYPE, "btnClose", 14, 14, 169, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

labName = UI_CreateCompent( frmChatMin, LABELEX_TYPE, "labName", 150, 11, 7, 4 )
UI_SetCaption( labName, "012345678901234567890123456789" )
UI_SetTextColor( labName, COLOR_BLACK )
UI_SetLabelExFont( labName, DEFAULT_FONT, TRUE, COLOR_WHITE )


----------------
-- Add Friend --
----------------
frmAddFriend = UI_CreateForm( "frmAddFriend",  FALSE, 185, 101, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmAddFriend, "texture/ui/sure.dds", 185, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmAddFriend, FALSE )
UI_AddFormToTemplete( frmAddFriend, FORM_MAIN )
UI_SetFormStyle( frmAddFriend, 0 )
UI_SetIsDrag( frmAddFriend, TRUE )
--UI_SetAlpha( frmAddFriend, 200 )
--UI_FormSetHotKey( frmAddFriend, ALT_KEY, HOTKEY_K ) 

btnClose = UI_CreateCompent( frmAddFriend, BUTTON_TYPE, "btnClose", 14, 14, 164, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labTitle = UI_CreateCompent( frmAddFriend, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Add Friend")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTradeGold = UI_CreateCompent( frmAddFriend, LABELEX_TYPE, "labTradeGold", 23, 11, 50, 30 )
UI_SetCaption( labTradeGold, "Please input player name" )
UI_SetTextColor( labTradeGold, COLOR_BLACK )
UI_SetLabelExFont( labTradeGold, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgTradeGoldBak = UI_CreateCompent( frmAddFriend, IMAGE_TYPE, "imgTradeGoldBak", 109, 19, 50, 50 )
UI_LoadImage( imgTradeGoldBak, "texture/ui/PublicC.tga", NORMAL, 109, 19, 47, 68 )

edtTradeGold = UI_CreateCompent( frmAddFriend, EDIT_TYPE, "edtTradeGold", 96, 11, 58, 54 )
UI_SetTextColor( edtTradeGold, COLOR_BLACK )
UI_SetEditMaxNum( edtTradeGold, 17 )
UI_SetEditMaxNumVisible( edtTradeGold, 17 )

btnYes = UI_CreateCompent( frmAddFriend, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmAddFriend, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )

styMenu = UI_CreateCompent( frmQQ, MENU_TYPE, "styMenu", 92, 97, 240, 468 )
UI_MenuLoadImage( styMenu, TRUE, FALSE, "texture/ui/right.tga", 5, 5, 10, 10, "texture/ui/right.tga", 5, 5 )
UI_SetMargin( styMenu, 10, 5, 5, 5 )

UI_MenuLoadSelect( styMenu, "texture/ui/SystemBotton3.tga", 79, 13, 25, 227  )



---------------------
-- character motto --
---------------------
frmEdit = UI_CreateForm( "frmEdit",  FALSE, 221, 157, 397, 500, TRUE, FALSE )
UI_LoadFormImage( frmEdit, "texture/ui/date.tga", 221, 157, 0, 0, "", 0, 0 )
UI_ShowForm( frmEdit, FALSE )
UI_AddFormToTemplete( frmEdit, FORM_MAIN )
UI_SetFormStyle( frmEdit, 0 )
UI_SetIsDrag( frmEdit, TRUE )
--UI_SetAlpha( frmEdit, 200 )
--UI_FormSetHotKey( frmEdit, ALT_KEY, HOTKEY_I ) 

labTitle = UI_CreateCompent( frmEdit, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Character Info")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

btnClose = UI_CreateCompent( frmEdit, BUTTON_TYPE, "btnClose", 14, 14, 200, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )

UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

edtTradeGold = UI_CreateCompent( frmEdit, EDIT_TYPE, "edtTradeGold", 95, 11, 80, 81 )
UI_SetTextColor( edtTradeGold, COLOR_BLACK )
UI_SetEditMaxNum( edtTradeGold, 16 )
UI_SetEditMaxNumVisible( edtTradeGold, 16 )

chkChat = UI_CreateCompent( frmEdit, CHECK_TYPE, "chkChat", 10, 10, 110, 105 )
UI_LoadImage( chkChat, "texture/ui/QQ2.tga", UNCHECKED, 10, 10, 40, 231 )
UI_LoadImage( chkChat, "texture/ui/QQ2.tga", CHECKED, 10, 10, 52, 231 )

btnYes = UI_CreateCompent( frmEdit, BUTTON_TYPE, "btnYes", 41, 19, 99, 134 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )
UI_SetEditEnterButton( edtTradeGold, btnYes )

btnNo = UI_CreateCompent( frmEdit, BUTTON_TYPE, "btnNo", 41, 19, 150, 134 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )

labName = UI_CreateCompent( frmEdit, LABELEX_TYPE, "labName", 96, 11, 68, 56 )
UI_SetCaption( labName, "Child of Demon what is that" )
UI_SetTextColor( labName, COLOR_BLACK )
UI_SetLabelExFont( labName, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgMhead = UI_CreateCompent( frmEdit, IMAGE_TYPE, "imgMhead", 32, 32, 20, 35 )
UI_LoadImage( imgMhead, "texture/ui/main800.tga", NORMAL, 32, 32, 37, 201 )

-------------------------------
-- Friend details ui By aLex --
-------------------------------
frmdetails = UI_CreateForm( "frmdetails",  FALSE, 212, 229, 397, 500, TRUE, FALSE )
UI_LoadFormImage( frmdetails, "texture/ui/Friend.dds", 212, 229, 0, 0, "", 0, 0 )
UI_ShowForm( frmdetails, FALSE )
UI_AddFormToTemplete( frmdetails, FORM_MAIN )
UI_SetFormStyle( frmdetails, 0 )
UI_SetIsDrag( frmdetails, TRUE )
--UI_SetAlpha( frmdetails, 200 )
--UI_FormSetHotKey( frmdetails, ALT_KEY, HOTKEY_I ) 

btnRefurbish = UI_CreateCompent( frmdetails, BUTTON_TYPE, "btnRefurbish", 41, 19, 70, 200 )
UI_LoadButtonImage( btnRefurbish, "texture/ui/SystemBotton3.tga", 41, 19, 215, 39, FALSE )

btnNo = UI_CreateCompent( frmdetails, BUTTON_TYPE, "btnNo", 41, 19, 122, 200 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )

imgDeTitle = UI_CreateCompent( frmdetails, IMAGE_TYPE, "imgDeTitle", 49, 13, 8, 3 )
UI_LoadImage( imgDeTitle, "texture/ui/Word.dds", NORMAL, 49, 13, 120, 58 )

labDeName = UI_CreateCompent( frmdetails, LABELEX_TYPE, "labDeName", 96, 11, 40, 40 )
--UI_SetCaption( labDeName, "Child of Demon what is that" )
UI_SetTextColor( labDeName, COLOR_BLACK )
UI_SetLabelExFont( labDeName, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgDeHead = UI_CreateCompent( frmdetails, IMAGE_TYPE, "imgDeHead", 32, 32, 4, 24 )
UI_LoadImage( imgDeHead, "texture/ui/main800.tga", NORMAL, 32, 32, 37, 201 )

labJob = UI_CreateCompent( frmdetails, LABELEX_TYPE, "labJob", 96, 11, 82, 68 )
--UI_SetCaption( labJob, "Swordsman" )
UI_SetTextColor( labJob, COLOR_BLACK )
UI_SetLabelExFont( labJob, DEFAULT_FONT, TRUE, COLOR_WHITE )

labLv = UI_CreateCompent( frmdetails, LABELEX_TYPE, "labLv", 96, 11, 82, 86 )
--UI_SetCaption( labLv, "100" )
UI_SetTextColor( labLv, COLOR_BLACK )
UI_SetLabelExFont( labLv, DEFAULT_FONT, TRUE, COLOR_WHITE )

labConsortia = UI_CreateCompent( frmdetails, LABELEX_TYPE, "labConsortia", 96, 11, 82, 104 )
--UI_SetCaption( labConsortia, "AhAhAh" )
UI_SetTextColor( labConsortia, COLOR_BLACK )
UI_SetLabelExFont( labConsortia, DEFAULT_FONT, TRUE, COLOR_WHITE )

labRepute = UI_CreateCompent( frmdetails, LABELEX_TYPE, "labRepute", 96, 11, 82, 122 )
--UI_SetCaption( labRepute, "200" )
UI_SetTextColor( labRepute, COLOR_BLACK )
UI_SetLabelExFont( labRepute, DEFAULT_FONT, TRUE, COLOR_WHITE )

labName = UI_CreateCompent( frmdetails, LABELEX_TYPE, "labName", 96, 11, 82, 140 )
--UI_SetCaption( labName, "200" )
UI_SetTextColor( labName, COLOR_BLACK )
UI_SetLabelExFont( labName, DEFAULT_FONT, TRUE, COLOR_WHITE )

labShip = UI_CreateCompent( frmdetails, LABELEX_TYPE, "labShip", 96, 11, 82, 158 )
--UI_SetCaption( labShip, "" )
UI_SetTextColor( labShip, COLOR_BLACK )
UI_SetLabelExFont( labShip, DEFAULT_FONT, TRUE, COLOR_WHITE )

labShiplv = UI_CreateCompent( frmdetails, LABELEX_TYPE, "labShiplv", 96, 11, 82, 176 )
--UI_SetCaption( labShiplv, "100" )
UI_SetTextColor( labShiplv, COLOR_BLACK )
UI_SetLabelExFont( labShiplv, DEFAULT_FONT, TRUE, COLOR_WHITE )

-----------------------------------------------------------------------
-- Block
-----------------------------------------------------------------------
frmBlockPlayer = UI_CreateForm( "frmBlockPlayer",  FALSE, 185, 101, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmBlockPlayer, "texture/ui/sure.dds", 185, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmBlockPlayer, FALSE )
UI_AddFormToTemplete( frmBlockPlayer, FORM_MAIN )
UI_SetFormStyle( frmBlockPlayer, 0 )
UI_SetIsDrag( frmBlockPlayer, TRUE )
--UI_SetAlpha( frmBlockPlayer, 200 )
--UI_FormSetHotKey( frmBlockPlayer, ALT_KEY, HOTKEY_K ) 

btnClose = UI_CreateCompent( frmBlockPlayer, BUTTON_TYPE, "btnClose", 14, 14, 164, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

labTitle = UI_CreateCompent( frmBlockPlayer, LABELEX_TYPE, "labTitle", 400, 150, 10, 4 )
UI_SetCaption( labTitle, "Block Player")
UI_SetTextColor( labTitle, COLOR_BLACK )
UI_SetLabelExFont( labTitle, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTradeGold = UI_CreateCompent( frmBlockPlayer, LABELEX_TYPE, "labTradeGold", 23, 11, 50, 30 )
UI_SetCaption( labTradeGold, "Please input player name" )
UI_SetTextColor( labTradeGold, COLOR_BLACK )
UI_SetLabelExFont( labTradeGold, DEFAULT_FONT, TRUE, COLOR_WHITE )

imgTradeGoldBak = UI_CreateCompent( frmBlockPlayer, IMAGE_TYPE, "imgTradeGoldBak", 109, 19, 50, 50 )
UI_LoadImage( imgTradeGoldBak, "texture/ui/PublicC.tga", NORMAL, 109, 19, 47, 68 )

edtTradeGold = UI_CreateCompent( frmBlockPlayer, EDIT_TYPE, "edtTradeGold", 96, 11, 58, 54 )
UI_SetTextColor( edtTradeGold, COLOR_BLACK )
UI_SetEditMaxNum( edtTradeGold, 17 )
UI_SetEditMaxNumVisible( edtTradeGold, 17 )

btnYes = UI_CreateCompent( frmBlockPlayer, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmBlockPlayer, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
UI_SetButtonModalResult( btnNo, BUTTON_CLOSE )

-----------------
-- chat pop up --
-----------------
frmRemind = UI_CreateForm( "frmRemind",  TRUE, 185, 101, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmRemind, "texture/ui/sure.dds", 185, 101, 0, 0, "", 0, 0 )
UI_SetFormStyle( frmRemind, 1 )
UI_ShowForm( frmRemind, FALSE )
--UI_AddFormToTemplete( frmRemind, FORM_LOGIN )
UI_AddFormToTemplete( frmRemind, FORM_MAIN )
--UI_AddFormToTemplete( frmRemind, FORM_SELECT_CHA )
--UI_AddFormToTemplete( frmRemind, FORM_EDITOR )
--UI_AddFormToTemplete( frmRemind, FORM_SWITCH_SCENE )
--UI_FormSetHotKey( frmRemind, ALT_KEY, HOTKEY_I ) 
UI_SetIsDrag( frmRemind, TRUE )
UI_SetAlpha( frmRemind, 220 )

btnCheck = UI_CreateCompent( frmRemind, BUTTON_TYPE, "btnCheck", 121, 11, 32, 40 )
--UI_SetCaption( labCheck, "012345678901234567890123456789" )
--UI_SetTextColor( labCheck, COLOR_BLACK )
--UI_SetLabelExFont( labCheck, DEFAULT_FONT, TRUE, COLOR_WHITE )

labTitle = UI_CreateCompent( frmRemind, LABELEX_TYPE, "labTitle", 170, 18, 8, 4 )
UI_SetTextColor( labTitle, COLOR_BLACK )

btnYes = UI_CreateCompent( frmRemind, BUTTON_TYPE, "btnYes", 41, 19, 74, 78 )
UI_LoadButtonImage( btnYes, "texture/ui/SystemBotton3.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmRemind, BUTTON_TYPE, "btnNo", 41, 19, 122, 78 )
UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )

-----------------------------------------------------------------------
-- (2005.4.11-Arcol:)
-----------------------------------------------------------------------
frmChatManage = UI_CreateForm( "frmChatManage",  FALSE, 130, 200, 397, 500, FALSE, FALSE )
UI_LoadFormImage( frmChatManage, "texture/ui/check.dds", 185, 101, 0, 0, "", 0, 0 )
UI_ShowForm( frmChatManage, FALSE )
UI_AddFormToTemplete( frmChatManage, FORM_MAIN )
--UI_FormSetHotKey( frmChatManage, ALT_KEY, HOTKEY_G ) --'Z'
UI_SetFormStyle( frmChatManage, 0 )
--UI_SetAlpha( frmChatManage, 200 )
UI_FormSetIsEscClose( frmChatManage, FALSE )

trvChatManage = UI_CreateCompent( frmChatManage, TREE_TYPE, "trvChatManage", 120, 160, 5, 40 )
-- UI_LoadImage( trvChatManage,  "texture/ui/editor/frame.dds", NORMAL, 1, 1, 0, 0 )
scrollid = UI_GetScroll( trvChatManage )
UI_SetSize( scrollid, 15, 15 )
-- UI_LoadImage( scrollid, "texture/ui/editor/chat.dds", COMPENT_BACK, 9, 9, 0, 51 )
scrollid = UI_GetScrollObj( scrollid, SCROLL_SCROLL )
UI_LoadImage( scrollid, "texture/ui/editor/scroll.tga", COMPENT_BACK, 16, 32, 0, 0 )
UI_SetSize( scrollid, 10, 32 )
-- UI_SetAlpha( trvChatManage, 100 )
