---------------------
-- npc cargo trade --
---------------------
frmSeaTrade = UI_CreateForm( "frmSeaTrade", FALSE, 382, 384, 200, 200, TRUE, FALSE )
--UI_FormSetHotKey(frmSeaTrade, ALT_KEY, HOTKEY_W )
UI_ShowForm( frmSeaTrade, FALSE )
UI_AddFormToTemplete( frmSeaTrade, 1 )
UI_SetIsDrag(frmSeaTrade, TRUE )

imgTradeTop = UI_CreateCompent( frmSeaTrade, IMAGE_TYPE, "imgTradeTop", 256, 216, 0, 0 )
UI_LoadImage( imgTradeTop, "texture/ui/trade1.tga", NORMAL, 256, 216, 0, 0 )

imgTradeDown = UI_CreateCompent( frmSeaTrade, IMAGE_TYPE, "imgTradeDown", 126, 216, 256, 0 )
UI_LoadImage( imgTradeDown, "texture/ui/trade2.tga", NORMAL, 126, 216, 0, 0 )

imgTradePL = UI_CreateCompent( frmSeaTrade, IMAGE_TYPE, "imgTradePL", 256, 168, 0, 216 )
--UI_LoadImage( imgTradePL, "texture/ui/shipR2.tga", NORMAL, 256, 168, 0, 0 )

imgTradePR = UI_CreateCompent( frmSeaTrade, IMAGE_TYPE, "imgTradePR", 126, 168, 256, 216 )
--UI_LoadImage( imgTradePR, "texture/ui/shipR.tga", NORMAL, 126, 168, 0, 0 )

btnRight = UI_CreateCompent( frmSeaTrade, BUTTON_TYPE, "btnRight", 15, 39, 350, 59 )
--UI_LoadButtonImage( btnRight, "texture/ui/SysBotton4.tga", 15, 39, 165, 0, TRUE )

btnLeft = UI_CreateCompent( frmSeaTrade, BUTTON_TYPE, "btnLeft", 15, 39, 15, 59 )
--UI_LoadButtonImage( btnLeft, "texture/ui/SysBotton4.tga", 15, 39, 165, 39, TRUE )

btnClose = UI_CreateCompent( frmSeaTrade, BUTTON_TYPE, "btnClose", 14, 14, 362, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )

--cmdItem1 = UI_CreateCompent( frmSeaTrade, COMMAND_ONE_TYPE, "cmdItem1", 32, 32, 67, 47 )
--cmdItem2 = UI_CreateCompent( frmSeaTrade, COMMAND_ONE_TYPE, "cmdItem2", 32, 32, 174, 47 )
--cmdItem3 = UI_CreateCompent( frmSeaTrade, COMMAND_ONE_TYPE, "cmdItem3", 32, 32, 281, 47 )

labItem1 = UI_CreateCompent( frmSeaTrade, LABELEX_TYPE, "labItem1", 96, 11, 35, 86 )
--UI_SetCaption( labItem1, "" )
UI_SetTextColor( labItem1, COLOR_BLACK )
UI_SetLabelExFont( labItem1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labItem2 = UI_CreateCompent( frmSeaTrade, LABELEX_TYPE, "labItem2", 96, 11, 142, 86 )
--UI_SetCaption( labItem2, "" )
UI_SetTextColor( labItem2, COLOR_BLACK )
UI_SetLabelExFont( labItem2, DEFAULT_FONT, TRUE, COLOR_WHITE )

labItem3 = UI_CreateCompent( frmSeaTrade, LABELEX_TYPE, "labItem3", 96, 11, 249, 86 )
--UI_SetCaption( labItem3, "" )
UI_SetTextColor( labItem3, COLOR_BLACK )
UI_SetLabelExFont( labItem3, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMen1 = UI_CreateCompent( frmSeaTrade, LABELEX_TYPE, "labMen1", 46, 8, 59, 103 )
--UI_SetCaption( labMen1, "11111111" )
UI_SetTextColor( labMen1, COLOR_BLACK )
UI_SetLabelExFont( labMen1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMen2 = UI_CreateCompent( frmSeaTrade, LABELEX_TYPE, "labMen2", 46, 8, 166, 103 )
--UI_SetCaption( labMen2, "11111111" )
UI_SetTextColor( labMen2, COLOR_BLACK )
UI_SetLabelExFont( labMen2, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMen3 = UI_CreateCompent( frmSeaTrade, LABELEX_TYPE, "labMen3", 46, 8, 273, 103 )
--UI_SetCaption( labMen3, "11111111" )
UI_SetTextColor( labMen3, COLOR_BLACK )
UI_SetLabelExFont( labMen3, DEFAULT_FONT, TRUE, COLOR_WHITE )

grdItemSale = UI_CreateCompent( frmSeaTrade, GOODS_GRID_TYPE, "grdItemSale", 364, 148, 6, 10 ) 
UI_SetGridSpace( grdItemSale, 20, 5 )
UI_SetGridContent( grdItemSale, 6, 4 )
UI_SetGridUnitSize( grdItemSale, 68, 68 )
UI_SetMargin( grdItemSale, 10, 0, 0, 0 )

scrollid = UI_GetScroll( grdItemSale )
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

grdItemBuy = UI_CreateCompent( frmSeaTrade, GOODS_GRID_TYPE, "grdItemBuy", 364, 148, 6, 10 ) 
UI_SetGridSpace( grdItemBuy, 20, 5 )
UI_SetGridContent( grdItemBuy, 6, 4 )
UI_SetGridUnitSize( grdItemBuy, 68, 68 )
UI_SetMargin( grdItemBuy, 10, 0, 0, 0 )

scrollid = UI_GetScroll( grdItemBuy )
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

pgeTrade = UI_CreateCompent( frmSeaTrade, PAGE_TYPE, "pgeTrade", 359, 152, 11, 58 )
UI_SetPageButton( pgeTrade, PAGE_BUTTON_CUSTOM, 69, 41 )

Itemsale = UI_CreatePageItem( pgeTrade )
ttlSale = UI_GetPageItemObj( Itemsale, PAGE_ITEM_TITLE )
UI_LoadImage( ttlSale, "texture/ui/trade3.tga", PAGE_ITEM_TITLE_NORMAL, 69, 35, 69, 0 )
UI_LoadImage( ttlSale, "texture/ui/trade3.tga", PAGE_ITEM_TITLE_ACTIVE, 69, 35, 0, 41 )
UI_SetPos( ttlSale, 16, -25 )
UI_SetSize( ttlSale, 69, 35 )
UI_AddCompent( Itemsale, grdItemSale )

Itembuy = UI_CreatePageItem( pgeTrade )
ttlBuy = UI_GetPageItemObj( Itembuy, PAGE_ITEM_TITLE )
UI_LoadImage( ttlBuy, "texture/ui/trade3.tga", PAGE_ITEM_TITLE_NORMAL, 69, 35, 69, 41 )
UI_LoadImage( ttlBuy, "texture/ui/trade3.tga", PAGE_ITEM_TITLE_ACTIVE, 69, 35, 0, 0 )
UI_SetPos( ttlBuy, 85, -25 )
UI_SetSize( ttlBuy, 69, 35 )
UI_AddCompent( Itembuy, grdItemBuy )

btnYes = UI_CreateCompent( frmSeaTrade, BUTTON_TYPE, "btnYes", 41, 19, 239, 350 )
--UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmSeaTrade, BUTTON_TYPE, "btnNo", 41, 19, 287, 350 )
--UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
--UI_SetButtonModalResult( btnNo, BUTTON_NO )


----------------------------------
--  black market / exchange npc --
----------------------------------
frmBlackTrade = UI_CreateForm( "frmBlackTrade", FALSE, 382, 384, 200, 200, TRUE, FALSE )
--UI_FormSetHotKey(frmBlackTrade, ALT_KEY, HOTKEY_D )
UI_ShowForm( frmBlackTrade, FALSE )
UI_AddFormToTemplete( frmBlackTrade, 1 )
UI_SetIsDrag(frmBlackTrade, TRUE )

imgTradeTop = UI_CreateCompent( frmBlackTrade, IMAGE_TYPE, "imgTradeTop", 256, 216, 0, 0 )
UI_LoadImage( imgTradeTop, "texture/ui/trade1.tga", NORMAL, 256, 216, 0, 0 )

imgTradeDown = UI_CreateCompent( frmBlackTrade, IMAGE_TYPE, "imgTradeDown", 126, 216, 256, 0 )
UI_LoadImage( imgTradeDown, "texture/ui/trade2.tga", NORMAL, 126, 216, 0, 0 )

imgTradePL = UI_CreateCompent( frmBlackTrade, IMAGE_TYPE, "imgTradePL", 256, 168, 0, 216 )
UI_LoadImage( imgTradePL, "texture/ui/shipR2.tga", NORMAL, 256, 168, 0, 0 )

imgTradePR = UI_CreateCompent( frmBlackTrade, IMAGE_TYPE, "imgTradePR", 126, 168, 256, 216 )
UI_LoadImage( imgTradePR, "texture/ui/shipR.tga", NORMAL, 126, 168, 0, 0 )

btnRight = UI_CreateCompent( frmBlackTrade, BUTTON_TYPE, "btnRight", 15, 39, 350, 59 )
--UI_LoadButtonImage( btnRight, "texture/ui/SysBotton4.tga", 15, 39, 165, 0, TRUE )

btnLeft = UI_CreateCompent( frmBlackTrade, BUTTON_TYPE, "btnLeft", 15, 39, 15, 59 )
--UI_LoadButtonImage( btnLeft, "texture/ui/SysBotton4.tga", 15, 39, 165, 39, TRUE )

btnClose = UI_CreateCompent( frmBlackTrade, BUTTON_TYPE, "btnClose", 14, 14, 362, 3 )
UI_LoadButtonImage( btnClose, "texture/ui/PublicC.tga", 14, 14, 116, 175, TRUE )
UI_SetButtonModalResult( btnClose, BUTTON_CLOSE )


--cmdItem1 = UI_CreateCompent( frmBlackTrade, COMMAND_ONE_TYPE, "cmdItem1", 32, 32, 67, 47 )
--cmdItem2 = UI_CreateCompent( frmBlackTrade, COMMAND_ONE_TYPE, "cmdItem2", 32, 32, 174, 47 )
--cmdItem3 = UI_CreateCompent( frmBlackTrade, COMMAND_ONE_TYPE, "cmdItem3", 32, 32, 281, 47 )

labItem1 = UI_CreateCompent( frmBlackTrade, LABELEX_TYPE, "labItem1", 96, 11, 35, 86 )
--UI_SetCaption( labItem1, "" )
UI_SetTextColor( labItem1, COLOR_BLACK )
UI_SetLabelExFont( labItem1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labItem2 = UI_CreateCompent( frmBlackTrade, LABELEX_TYPE, "labItem2", 96, 11, 142, 86 )
--UI_SetCaption( labItem2, "" )
UI_SetTextColor( labItem2, COLOR_BLACK )
UI_SetLabelExFont( labItem2, DEFAULT_FONT, TRUE, COLOR_WHITE )

labItem3 = UI_CreateCompent( frmBlackTrade, LABELEX_TYPE, "labItem3", 96, 11, 249, 86 )
--UI_SetCaption( labItem3, "" )
UI_SetTextColor( labItem3, COLOR_BLACK )
UI_SetLabelExFont( labItem3, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMen1 = UI_CreateCompent( frmBlackTrade, LABELEX_TYPE, "labMen1", 46, 8, 59, 103 )
--UI_SetCaption( labMen1, "11111111" )
UI_SetTextColor( labMen1, COLOR_BLACK )
UI_SetLabelExFont( labMen1, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMen2 = UI_CreateCompent( frmBlackTrade, LABELEX_TYPE, "labMen2", 46, 8, 166, 103 )
--UI_SetCaption( labMen2, "11111111" )
UI_SetTextColor( labMen2, COLOR_BLACK )
UI_SetLabelExFont( labMen2, DEFAULT_FONT, TRUE, COLOR_WHITE )

labMen3 = UI_CreateCompent( frmBlackTrade, LABELEX_TYPE, "labMen3", 46, 8, 273, 103 )
--UI_SetCaption( labMen3, "11111111" )
UI_SetTextColor( labMen3, COLOR_BLACK )
UI_SetLabelExFont( labMen3, DEFAULT_FONT, TRUE, COLOR_WHITE )

grdItemSale = UI_CreateCompent( frmBlackTrade, GOODS_GRID_TYPE, "grdItemSale", 364, 148, 20, 69 ) 
UI_SetGridSpace( grdItemSale, 23, 5 )
UI_SetGridContent( grdItemSale, 14, 3 )
UI_SetGridUnitSize( grdItemSale, 92,68 )
UI_SetMargin( grdItemSale, 10, 0, 0, 0 )

scrollid = UI_GetScroll( grdItemSale )
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

grdItemBuy = UI_CreateCompent( frmBlackTrade, GOODS_GRID_TYPE, "grdItemBuy", 364, 148, 36, 245 ) 
UI_SetGridSpace( grdItemBuy, 4, 3 )
UI_SetGridContent( grdItemBuy, 3, 8 )
UI_SetGridUnitSize( grdItemBuy, 32, 32 )
UI_SetMargin( grdItemBuy, 10, 0, 0, 0 )

btnYes = UI_CreateCompent( frmBlackTrade, BUTTON_TYPE, "btnYes", 41, 19, 239, 350 )
--UI_LoadButtonImage( btnYes, "texture/ui/PublicC.tga", 41, 19, 0, 0, TRUE )

btnNo = UI_CreateCompent( frmBlackTrade, BUTTON_TYPE, "btnNo", 41, 19, 287, 350 )
--UI_LoadButtonImage( btnNo, "texture/ui/PublicC.tga", 41, 19, 0, 20, TRUE )
--UI_SetButtonModalResult( btnNo, BUTTON_NO )
