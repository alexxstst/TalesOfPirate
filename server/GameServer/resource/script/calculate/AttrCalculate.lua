print("-- [Loading] AttrCalculate")

attr = {}
attr[1] = {0, 0}
attr[2] = {0, 0}
attr[3] = {0, 0}
attr[4] = {0, 0}
attr[5] = {0, 0}
attr[6] = {0, 0}
attr[7] = {0, 0}
attr[8] = {0, 0}
attr[9] = {0, 0}
attr[10] = {0, 0}

item_add = {cnt = 0, attr = attr}

function Reset_item_add()
    item_add.cnt = 0
    item_add.attr[1] = {0, 0}
    item_add.attr[2] = {0, 0}
    item_add.attr[3] = {0, 0}
    item_add.attr[4] = {0, 0}
    item_add.attr[5] = {0, 0}
    item_add.attr[6] = {0, 0}
    item_add.attr[7] = {0, 0}
    item_add.attr[8] = {0, 0}
    item_add.attr[9] = {0, 0}
    item_add.attr[10] = {0, 0}
end

function Add_Item_Attr(attr_idx, radio)
    item_add.cnt = item_add.cnt + 1
    item_add.attr[item_add.cnt] = {attr_idx, radio}
end

Mxhp_con_rad1 = {}
Mxhp_con_rad2 = {}
Mxhp_lv_rad = {}
Mxhp_bs = {}
Mxsp_sta_rad1 = {}
Mxsp_sta_rad2 = {}
Mxsp_lv_rad = {}
Mnatk_str_rad1 = {}
Mnatk_str_rad2 = {}
Mnatk_dex_rad1 = {}
Mnatk_dex_rad2 = {}
Mxatk_str_rad1 = {}
Mxatk_str_rad2 = {}
Mxatk_dex_rad1 = {}
Mxatk_dex_rad2 = {}
Def_con_rad1 = {}
Def_con_rad2 = {}
Hit_dex_rad1 = {}
Hit_dex_rad2 = {}
Hit_lv_rad = {}
Hit_min = {}
Flee_agi_rad1 = {}
Flee_agi_rad2 = {}
Flee_lv_rad = {}
Flee_min = {}
Mf_luk_rad = {}
Crt_luk_rad = {}
Crt_min = {}
Crt_max = {}
Hrec_bsmxhp_rad = {}
Hrec_con_rad = {}
Hrec_min = {}
Srec_bsmxsp_rad = {}
Srec_sta_rad = {}
Srec_min = {}
Aspd_bsrad = {}
Aspd_agi_rad = {}
Aspd_min = {}
Str_updata = {}
Dex_updata = {}
Con_updata = {}
Agi_updata = {}
Sta_updata = {}
Luk_updata = {}


Mxhp_con_rad1[JOB_TYPE_XINSHOU], Mxhp_con_rad2[JOB_TYPE_XINSHOU], Mxhp_lv_rad[JOB_TYPE_XINSHOU]	= 3, 2, 15							
Mxsp_sta_rad1[JOB_TYPE_XINSHOU], Mxsp_sta_rad2[JOB_TYPE_XINSHOU], Mxsp_lv_rad[JOB_TYPE_XINSHOU]	= 1, 0, 3 							
Mnatk_str_rad1[JOB_TYPE_XINSHOU], Mnatk_str_rad2[JOB_TYPE_XINSHOU], Mnatk_dex_rad1[JOB_TYPE_XINSHOU], Mnatk_dex_rad2[JOB_TYPE_XINSHOU]	= 1.5, 0.4, 0, 0	
Mxatk_str_rad1[JOB_TYPE_XINSHOU], Mxatk_str_rad2[JOB_TYPE_XINSHOU], Mxatk_dex_rad1[JOB_TYPE_XINSHOU], Mxatk_dex_rad2[JOB_TYPE_XINSHOU]	= 1.5, 0.4, 0, 0	
Def_con_rad1[JOB_TYPE_XINSHOU], Def_con_rad2[JOB_TYPE_XINSHOU]		= 0.1, 0.1										
Hit_dex_rad1[JOB_TYPE_XINSHOU], Hit_dex_rad2[JOB_TYPE_XINSHOU]		= 0.6, 0										
Flee_agi_rad1[JOB_TYPE_XINSHOU], Flee_agi_rad2[JOB_TYPE_XINSHOU]	= 0.6, 0										
Mf_luk_rad[JOB_TYPE_XINSHOU]	= 0.39																
Crt_luk_rad[JOB_TYPE_XINSHOU]	= 0.31																
Hrec_bsmxhp_rad[JOB_TYPE_XINSHOU], Hrec_con_rad[JOB_TYPE_XINSHOU]	= 1/200, 1/8										
Srec_bsmxsp_rad[JOB_TYPE_XINSHOU], Srec_sta_rad[JOB_TYPE_XINSHOU]	= 1/100, 1/12										
Aspd_agi_rad[JOB_TYPE_XINSHOU]	= 1.1																
Str_updata[JOB_TYPE_XINSHOU]	= 0.2 
Dex_updata[JOB_TYPE_XINSHOU]	= 0.1 
Con_updata[JOB_TYPE_XINSHOU]	= 0.6 
Agi_updata[JOB_TYPE_XINSHOU]	= 0.1 
Sta_updata[JOB_TYPE_XINSHOU]	= 0.1 
Luk_updata[JOB_TYPE_XINSHOU]	= 0.1 

Mxhp_con_rad1[JOB_TYPE_JIANSHI], Mxhp_con_rad2[JOB_TYPE_JIANSHI], Mxhp_lv_rad[JOB_TYPE_JIANSHI]	= 5, 7,	25							
Mxsp_sta_rad1[JOB_TYPE_JIANSHI], Mxsp_sta_rad2[JOB_TYPE_JIANSHI], Mxsp_lv_rad[JOB_TYPE_JIANSHI]	= 1, 0,	3							
Mnatk_str_rad1[JOB_TYPE_JIANSHI], Mnatk_str_rad2[JOB_TYPE_JIANSHI], Mnatk_dex_rad1[JOB_TYPE_JIANSHI], Mnatk_dex_rad2[JOB_TYPE_JIANSHI] = 1.5, 0.4, 0, 0		
Mxatk_str_rad1[JOB_TYPE_JIANSHI], Mxatk_str_rad2[JOB_TYPE_JIANSHI], Mxatk_dex_rad1[JOB_TYPE_JIANSHI], Mxatk_dex_rad2[JOB_TYPE_JIANSHI] = 1.5, 0.4, 0, 0		
Def_con_rad1[JOB_TYPE_JIANSHI], Def_con_rad2[JOB_TYPE_JIANSHI]		= 0.2, 0.2	 									
Hit_dex_rad1[JOB_TYPE_JIANSHI], Hit_dex_rad2[JOB_TYPE_JIANSHI]		= 0.6, 0										
Flee_agi_rad1[JOB_TYPE_JIANSHI], Flee_agi_rad2[JOB_TYPE_JIANSHI]	= 0.6, 0										
Mf_luk_rad[JOB_TYPE_JIANSHI]	= 0.39 																
Crt_luk_rad[JOB_TYPE_JIANSHI]	= 0.31																
Hrec_bsmxhp_rad[JOB_TYPE_JIANSHI], Hrec_con_rad[JOB_TYPE_JIANSHI]	= 1/180, 1/8										
Srec_bsmxsp_rad[JOB_TYPE_JIANSHI], Srec_sta_rad[JOB_TYPE_JIANSHI]	= 1/100, 1/12 										
Aspd_agi_rad[JOB_TYPE_JIANSHI]	= 1.1   															
Str_updata[JOB_TYPE_JIANSHI]	= 0.5 
Dex_updata[JOB_TYPE_JIANSHI]	= 0.1 
Con_updata[JOB_TYPE_JIANSHI]	= 0.5 
Agi_updata[JOB_TYPE_JIANSHI]	= 0.1 
Sta_updata[JOB_TYPE_JIANSHI]	= 0.1 
Luk_updata[JOB_TYPE_JIANSHI]	= 0.1 

Mxhp_con_rad1[JOB_TYPE_LIEREN], Mxhp_con_rad2[JOB_TYPE_LIEREN], Mxhp_lv_rad[JOB_TYPE_LIEREN]	= 3, 3, 25 							
Mxsp_sta_rad1[JOB_TYPE_LIEREN], Mxsp_sta_rad2[JOB_TYPE_LIEREN], Mxsp_lv_rad[JOB_TYPE_LIEREN]	= 1, 0, 3							
Mnatk_str_rad1[JOB_TYPE_LIEREN], Mnatk_str_rad2[JOB_TYPE_LIEREN], Mnatk_dex_rad1[JOB_TYPE_LIEREN], Mnatk_dex_rad2[JOB_TYPE_LIEREN]	= 0, 0, 1.7, 0.4	
Mxatk_str_rad1[JOB_TYPE_LIEREN], Mxatk_str_rad2[JOB_TYPE_LIEREN], Mxatk_dex_rad1[JOB_TYPE_LIEREN], Mxatk_dex_rad2[JOB_TYPE_LIEREN]	= 0, 0, 1.7, 0.4	
Def_con_rad1[JOB_TYPE_LIEREN], Def_con_rad2[JOB_TYPE_LIEREN]		= 0.14, 0.1 										
Hit_dex_rad1[JOB_TYPE_LIEREN], Hit_dex_rad2[JOB_TYPE_LIEREN]		= 0.7, 0										
Flee_agi_rad1[JOB_TYPE_LIEREN], Flee_agi_rad2[JOB_TYPE_LIEREN]		= 0.7, 0										
Mf_luk_rad[JOB_TYPE_LIEREN]	= 0.39 																
Crt_luk_rad[JOB_TYPE_LIEREN]	= 0.25																
Hrec_bsmxhp_rad[JOB_TYPE_LIEREN], Hrec_con_rad[JOB_TYPE_LIEREN]		= 1/180, 1/8 										
Srec_bsmxsp_rad[JOB_TYPE_LIEREN], Srec_sta_rad[JOB_TYPE_LIEREN]		= 1/100, 1/12 										
Aspd_agi_rad[JOB_TYPE_LIEREN]	= 1.2   												 			
Str_updata[JOB_TYPE_LIEREN]	= 0.1 
Dex_updata[JOB_TYPE_LIEREN]	= 0.5 
Con_updata[JOB_TYPE_LIEREN]	= 0.1 
Agi_updata[JOB_TYPE_LIEREN]	= 0.5 
Sta_updata[JOB_TYPE_LIEREN]	= 0.1 
Luk_updata[JOB_TYPE_LIEREN]	= 0.1 

Mxhp_con_rad1[JOB_TYPE_SHUISHOU], Mxhp_con_rad2[JOB_TYPE_SHUISHOU], Mxhp_lv_rad[JOB_TYPE_SHUISHOU]	= 3, 2, 15							
Mxsp_sta_rad1[JOB_TYPE_SHUISHOU], Mxsp_sta_rad2[JOB_TYPE_SHUISHOU], Mxsp_lv_rad[JOB_TYPE_SHUISHOU]	= 0.5, 0.5, 1							
Mnatk_str_rad1[JOB_TYPE_SHUISHOU], Mnatk_str_rad2[JOB_TYPE_SHUISHOU], Mnatk_dex_rad1[JOB_TYPE_SHUISHOU], Mnatk_dex_rad2[JOB_TYPE_SHUISHOU]	= 0.9, 0.9, 0, 0	
Mxatk_str_rad1[JOB_TYPE_SHUISHOU], Mxatk_str_rad2[JOB_TYPE_SHUISHOU], Mxatk_dex_rad1[JOB_TYPE_SHUISHOU], Mxatk_dex_rad2[JOB_TYPE_SHUISHOU]	= 0.9, 0.9, 0, 0 	
Def_con_rad1[JOB_TYPE_SHUISHOU], Def_con_rad2[JOB_TYPE_SHUISHOU]	= 0.45, 0.45 											
Hit_dex_rad1[JOB_TYPE_SHUISHOU], Hit_dex_rad2[JOB_TYPE_SHUISHOU]	= 0.31, 0.15 											
Flee_agi_rad1[JOB_TYPE_SHUISHOU], Flee_agi_rad2[JOB_TYPE_SHUISHOU]	= 0.31, 0.15											
Mf_luk_rad[JOB_TYPE_SHUISHOU]	= 0.39 																	
Crt_luk_rad[JOB_TYPE_SHUISHOU]	= 0.31																	
Hrec_bsmxhp_rad[JOB_TYPE_SHUISHOU], Hrec_con_rad[JOB_TYPE_SHUISHOU]	= 1/200, 1/100											
Srec_bsmxsp_rad[JOB_TYPE_SHUISHOU], Srec_sta_rad[JOB_TYPE_SHUISHOU]	= 1/200, 1/120											
Aspd_agi_rad[JOB_TYPE_SHUISHOU] = 1.1   																
Str_updata[JOB_TYPE_SHUISHOU]	= 0.2 
Dex_updata[JOB_TYPE_SHUISHOU]	= 0.1 
Con_updata[JOB_TYPE_SHUISHOU]	= 0.6 
Agi_updata[JOB_TYPE_SHUISHOU]	= 0.1 
Sta_updata[JOB_TYPE_SHUISHOU]	= 0.1 
Luk_updata[JOB_TYPE_SHUISHOU]	= 0.1 

Mxhp_con_rad1[JOB_TYPE_MAOXIANZHE], Mxhp_con_rad2[JOB_TYPE_MAOXIANZHE], Mxhp_lv_rad[JOB_TYPE_MAOXIANZHE]	= 5, 5, 25							
Mxsp_sta_rad1[JOB_TYPE_MAOXIANZHE], Mxsp_sta_rad2[JOB_TYPE_MAOXIANZHE], Mxsp_lv_rad[JOB_TYPE_MAOXIANZHE]	= 2, 1.5, 5							
Mnatk_str_rad1[JOB_TYPE_MAOXIANZHE], Mnatk_str_rad2[JOB_TYPE_MAOXIANZHE], Mnatk_dex_rad1[JOB_TYPE_MAOXIANZHE], Mnatk_dex_rad2[JOB_TYPE_MAOXIANZHE]	= 1.5, 0.4, 0, 0	
Mxatk_str_rad1[JOB_TYPE_MAOXIANZHE], Mxatk_str_rad2[JOB_TYPE_MAOXIANZHE], Mxatk_dex_rad1[JOB_TYPE_MAOXIANZHE], Mxatk_dex_rad2[JOB_TYPE_MAOXIANZHE]	= 1.5, 0.4, 0, 0 	
Def_con_rad1[JOB_TYPE_MAOXIANZHE], Def_con_rad2[JOB_TYPE_MAOXIANZHE]	= 0.13, 0.1 												
Hit_dex_rad1[JOB_TYPE_MAOXIANZHE], Hit_dex_rad2[JOB_TYPE_MAOXIANZHE]	= 0.6, 0 												
Flee_agi_rad1[JOB_TYPE_MAOXIANZHE], Flee_agi_rad2[JOB_TYPE_MAOXIANZHE]	= 0.6, 0												
Mf_luk_rad[JOB_TYPE_MAOXIANZHE]		= 0.39 																	
Crt_luk_rad[JOB_TYPE_MAOXIANZHE]	= 0.31																	
Hrec_bsmxhp_rad[JOB_TYPE_MAOXIANZHE], Hrec_con_rad[JOB_TYPE_MAOXIANZHE]	= 1/180, 1/8												
Srec_bsmxsp_rad[JOB_TYPE_MAOXIANZHE], Srec_sta_rad[JOB_TYPE_MAOXIANZHE]	= 1/100, 1/12												
Aspd_agi_rad[JOB_TYPE_MAOXIANZHE]	= 1.1   												 				
Str_updata[JOB_TYPE_MAOXIANZHE]		= 0.1 
Dex_updata[JOB_TYPE_MAOXIANZHE]		= 0.1 
Con_updata[JOB_TYPE_MAOXIANZHE]		= 0.3 
Agi_updata[JOB_TYPE_MAOXIANZHE]		= 0.1 
Sta_updata[JOB_TYPE_MAOXIANZHE]		= 0.5 
Luk_updata[JOB_TYPE_MAOXIANZHE]		= 0.1 

Mxhp_con_rad1[JOB_TYPE_QIYUANSHI], Mxhp_con_rad2[JOB_TYPE_QIYUANSHI], Mxhp_lv_rad[JOB_TYPE_QIYUANSHI]	= 5, 5, 25							
Mxsp_sta_rad1[JOB_TYPE_QIYUANSHI], Mxsp_sta_rad2[JOB_TYPE_QIYUANSHI], Mxsp_lv_rad[JOB_TYPE_QIYUANSHI]	= 2, 1.5 , 5							
Mnatk_str_rad1[JOB_TYPE_QIYUANSHI], Mnatk_str_rad2[JOB_TYPE_QIYUANSHI], Mnatk_dex_rad1[JOB_TYPE_QIYUANSHI], Mnatk_dex_rad2[JOB_TYPE_QIYUANSHI]	= 1.5, 0.4, 0, 0	
Mxatk_str_rad1[JOB_TYPE_QIYUANSHI], Mxatk_str_rad2[JOB_TYPE_QIYUANSHI], Mxatk_dex_rad1[JOB_TYPE_QIYUANSHI], Mxatk_dex_rad2[JOB_TYPE_QIYUANSHI]	= 1.5, 0.4, 0, 0 	
Def_con_rad1[JOB_TYPE_QIYUANSHI], Def_con_rad2[JOB_TYPE_QIYUANSHI]	= 0.13, 0.1 											
Hit_dex_rad1[JOB_TYPE_QIYUANSHI], Hit_dex_rad2[JOB_TYPE_QIYUANSHI]	= 0.6, 0 											
Flee_agi_rad1[JOB_TYPE_QIYUANSHI], Flee_agi_rad2[JOB_TYPE_QIYUANSHI]	= 0.6, 0											
Mf_luk_rad[JOB_TYPE_QIYUANSHI]		= 0.39 																
Crt_luk_rad[JOB_TYPE_QIYUANSHI] 	= 0.31																
Hrec_bsmxhp_rad[JOB_TYPE_QIYUANSHI], Hrec_con_rad[JOB_TYPE_QIYUANSHI]	= 1/180, 1/8											
Srec_bsmxsp_rad[JOB_TYPE_QIYUANSHI], Srec_sta_rad[JOB_TYPE_QIYUANSHI]	= 1/100, 1/12											
Aspd_agi_rad[JOB_TYPE_QIYUANSHI]	= 1.1   												 			
Str_updata[JOB_TYPE_QIYUANSHI]		= 0.1 
Dex_updata[JOB_TYPE_QIYUANSHI]		= 0.1 
Con_updata[JOB_TYPE_QIYUANSHI]		= 0.3 
Agi_updata[JOB_TYPE_QIYUANSHI]		= 0.1 
Sta_updata[JOB_TYPE_QIYUANSHI]		= 0.5 
Luk_updata[JOB_TYPE_QIYUANSHI]		= 0.1 

Mxhp_con_rad1[JOB_TYPE_JISHI], Mxhp_con_rad2[JOB_TYPE_JISHI], Mxhp_lv_rad[JOB_TYPE_JISHI]	= 2, 2, 10 						
Mxsp_sta_rad1[JOB_TYPE_JISHI], Mxsp_sta_rad2[JOB_TYPE_JISHI], Mxsp_lv_rad[JOB_TYPE_JISHI]	= 0.5, 0.5, 1						
Mnatk_str_rad1[JOB_TYPE_JISHI], Mnatk_str_rad2[JOB_TYPE_JISHI], Mnatk_dex_rad1[JOB_TYPE_JISHI], Mnatk_dex_rad2[JOB_TYPE_JISHI]	= 0.8, 0.8, 0, 0	
Mxatk_str_rad1[JOB_TYPE_JISHI], Mxatk_str_rad2[JOB_TYPE_JISHI], Mxatk_dex_rad1[JOB_TYPE_JISHI], Mxatk_dex_rad2[JOB_TYPE_JISHI]	= 0.8, 0.8, 0, 0 	
Def_con_rad1[JOB_TYPE_JISHI], Def_con_rad2[JOB_TYPE_JISHI]	= 0.5, 0.5 										
Hit_dex_rad1[JOB_TYPE_JISHI], Hit_dex_rad2[JOB_TYPE_JISHI]	= 0.31, 0.15 										
Flee_agi_rad1[JOB_TYPE_JISHI], Flee_agi_rad2[JOB_TYPE_JISHI]	= 0.31, 0.15										
Mf_luk_rad[JOB_TYPE_JISHI]	= 0.39 															
Crt_luk_rad[JOB_TYPE_JISHI]	= 0.31															
Hrec_bsmxhp_rad[JOB_TYPE_JISHI], Hrec_con_rad[JOB_TYPE_JISHI]	= 1/200, 1/100										
Srec_bsmxsp_rad[JOB_TYPE_JISHI],Srec_sta_rad[JOB_TYPE_JISHI]	= 1/200, 1/120										
Aspd_agi_rad[JOB_TYPE_JISHI]	= 1.1   												 		
Str_updata[JOB_TYPE_JISHI]	= 0.4 
Dex_updata[JOB_TYPE_JISHI]	= 0.1 
Con_updata[JOB_TYPE_JISHI]	= 0.5 
Agi_updata[JOB_TYPE_JISHI]	= 0.1 
Sta_updata[JOB_TYPE_JISHI]	= 0.1 
Luk_updata[JOB_TYPE_JISHI]	= 0.2 

Mxhp_con_rad1[JOB_TYPE_SHANGREN], Mxhp_con_rad2[JOB_TYPE_SHANGREN], Mxhp_lv_rad[JOB_TYPE_SHANGREN]	= 2, 2, 10 							
Mxsp_sta_rad1[JOB_TYPE_SHANGREN], Mxsp_sta_rad2[JOB_TYPE_SHANGREN], Mxsp_lv_rad[JOB_TYPE_SHANGREN]	= 0.5, 0.5, 1							
Mnatk_str_rad1[JOB_TYPE_SHANGREN], Mnatk_str_rad2[JOB_TYPE_SHANGREN], Mnatk_dex_rad1[JOB_TYPE_SHANGREN], Mnatk_dex_rad2[JOB_TYPE_SHANGREN]	= 0.8, 0.8, 0, 0	
Mxatk_str_rad1[JOB_TYPE_SHANGREN], Mxatk_str_rad2[JOB_TYPE_SHANGREN], Mxatk_dex_rad1[JOB_TYPE_SHANGREN], Mxatk_dex_rad2[JOB_TYPE_SHANGREN]	= 0.8, 0.8, 0, 0	
Def_con_rad1[JOB_TYPE_SHANGREN], Def_con_rad2[JOB_TYPE_SHANGREN]	= 0.5, 0.5 											
Hit_dex_rad1[JOB_TYPE_SHANGREN], Hit_dex_rad2[JOB_TYPE_SHANGREN]	= 0.31, 0.15 											
Flee_agi_rad1[JOB_TYPE_SHANGREN], Flee_agi_rad2[JOB_TYPE_SHANGREN]	= 0.31, 0.15											
Mf_luk_rad[JOB_TYPE_SHANGREN]	= 0.39 																	
Crt_luk_rad[JOB_TYPE_SHANGREN]	= 0.31																	
Hrec_bsmxhp_rad[JOB_TYPE_SHANGREN], Hrec_con_rad[JOB_TYPE_SHANGREN]	= 1/200, 1/100											
Srec_bsmxsp_rad[JOB_TYPE_SHANGREN], Srec_sta_rad[JOB_TYPE_SHANGREN]	= 1/200, 1/120											
Aspd_agi_rad[JOB_TYPE_SHANGREN] = 1.1   												 				
Str_updata[JOB_TYPE_SHANGREN]	= 0.3 
Dex_updata[JOB_TYPE_SHANGREN]	= 0.1 
Con_updata[JOB_TYPE_SHANGREN]	= 0.5 
Agi_updata[JOB_TYPE_SHANGREN]	= 0.1 
Sta_updata[JOB_TYPE_SHANGREN]	= 0.1 
Luk_updata[JOB_TYPE_SHANGREN]	= 0.1 

Mxhp_con_rad1[JOB_TYPE_JUJS], Mxhp_con_rad2[JOB_TYPE_JUJS], Mxhp_lv_rad[JOB_TYPE_JUJS]	= 6, 7, 40 							
Mxsp_sta_rad1[JOB_TYPE_JUJS], Mxsp_sta_rad2[JOB_TYPE_JUJS], Mxsp_lv_rad[JOB_TYPE_JUJS]	= 1, 0, 3 							
Mnatk_str_rad1[JOB_TYPE_JUJS], Mnatk_str_rad2[JOB_TYPE_JUJS], Mnatk_dex_rad1[JOB_TYPE_JUJS], Mnatk_dex_rad2[JOB_TYPE_JUJS]	= 1, 0.4, 0, 0		
Mxatk_str_rad1[JOB_TYPE_JUJS], Mxatk_str_rad2[JOB_TYPE_JUJS], Mxatk_dex_rad1[JOB_TYPE_JUJS], Mxatk_dex_rad2[JOB_TYPE_JUJS]	= 1, 0.4, 0, 0		
Def_con_rad1[JOB_TYPE_JUJS], Def_con_rad2[JOB_TYPE_JUJS]	= 0.2, 0.7 										
Hit_dex_rad1[JOB_TYPE_JUJS], Hit_dex_rad2[JOB_TYPE_JUJS]	= 0.7, 0.4 										
Flee_agi_rad1[JOB_TYPE_JUJS], Flee_agi_rad2[JOB_TYPE_JUJS]	= 0.4, 0										
Mf_luk_rad[JOB_TYPE_JUJS]	= 0.39 															
Crt_luk_rad[JOB_TYPE_JUJS]	= 0.31															
Hrec_bsmxhp_rad[JOB_TYPE_JUJS], Hrec_con_rad[JOB_TYPE_JUJS] = 1/180, 1/8										
Srec_bsmxsp_rad[JOB_TYPE_JUJS], Srec_sta_rad[JOB_TYPE_JUJS] = 1/100, 1/12										
Aspd_agi_rad[JOB_TYPE_JUJS]	= 1.0    												 		
Str_updata[JOB_TYPE_JUJS]	= 0.3 
Dex_updata[JOB_TYPE_JUJS]	= 0.1 
Con_updata[JOB_TYPE_JUJS]	= 0.5 
Agi_updata[JOB_TYPE_JUJS]	= 0.1 
Sta_updata[JOB_TYPE_JUJS]	= 0.1 
Luk_updata[JOB_TYPE_JUJS]	= 0.1 

Mxhp_con_rad1[JOB_TYPE_SHUANGJS], Mxhp_con_rad2[JOB_TYPE_SHUANGJS], Mxhp_lv_rad[JOB_TYPE_SHUANGJS]	= 3.5, 3.5, 30 							
Mxsp_sta_rad1[JOB_TYPE_SHUANGJS], Mxsp_sta_rad2[JOB_TYPE_SHUANGJS], Mxsp_lv_rad[JOB_TYPE_SHUANGJS]	= 1, 0, 3 							
Mnatk_str_rad1[JOB_TYPE_SHUANGJS], Mnatk_str_rad2[JOB_TYPE_SHUANGJS], Mnatk_dex_rad1[JOB_TYPE_SHUANGJS], Mnatk_dex_rad2[JOB_TYPE_SHUANGJS]	= 1.5, 0.45, 0, 0	
Mxatk_str_rad1[JOB_TYPE_SHUANGJS], Mxatk_str_rad2[JOB_TYPE_SHUANGJS], Mxatk_dex_rad1[JOB_TYPE_SHUANGJS], Mxatk_dex_rad2[JOB_TYPE_SHUANGJS]	= 1.5, 0.45, 0, 0	
Def_con_rad1[JOB_TYPE_SHUANGJS], Def_con_rad2[JOB_TYPE_SHUANGJS]	= 0.2, 0.2 											
Hit_dex_rad1[JOB_TYPE_SHUANGJS], Hit_dex_rad2[JOB_TYPE_SHUANGJS]	= 0.6, 0.2 											
Flee_agi_rad1[JOB_TYPE_SHUANGJS], Flee_agi_rad2[JOB_TYPE_SHUANGJS]	= 0.75, 0											
Mf_luk_rad[JOB_TYPE_SHUANGJS]	= 0.39 																	
Crt_luk_rad[JOB_TYPE_SHUANGJS]	= 0.31																	
Hrec_bsmxhp_rad[JOB_TYPE_SHUANGJS], Hrec_con_rad[JOB_TYPE_SHUANGJS] = 1/180, 1/8											
Srec_bsmxsp_rad[JOB_TYPE_SHUANGJS], Srec_sta_rad[JOB_TYPE_SHUANGJS] = 1/100, 1/12											
Aspd_agi_rad[JOB_TYPE_SHUANGJS]	= 1.25    												 				
Str_updata[JOB_TYPE_SHUANGJS]	= 0.3 
Dex_updata[JOB_TYPE_SHUANGJS]	= 0.1 
Con_updata[JOB_TYPE_SHUANGJS]	= 0.5 
Agi_updata[JOB_TYPE_SHUANGJS]	= 0.1 
Sta_updata[JOB_TYPE_SHUANGJS]	= 0.1 
Luk_updata[JOB_TYPE_SHUANGJS]	= 0.1 

Mxhp_con_rad1[JOB_TYPE_JIANDUNSHI], Mxhp_con_rad2[JOB_TYPE_JIANDUNSHI], Mxhp_lv_rad[JOB_TYPE_JIANDUNSHI]	= 5, 5, 30 							
Mxsp_sta_rad1[JOB_TYPE_JIANDUNSHI], Mxsp_sta_rad2[JOB_TYPE_JIANDUNSHI], Mxsp_lv_rad[JOB_TYPE_JIANDUNSHI]	= 3, 1.5, 5							
Mnatk_str_rad1[JOB_TYPE_JIANDUNSHI], Mnatk_str_rad2[JOB_TYPE_JIANDUNSHI], Mnatk_dex_rad1[JOB_TYPE_JIANDUNSHI], Mnatk_dex_rad2[JOB_TYPE_JIANDUNSHI]	= 1.5, 0.4, 0, 0	
Mxatk_str_rad1[JOB_TYPE_JIANDUNSHI], Mxatk_str_rad2[JOB_TYPE_JIANDUNSHI], Mxatk_dex_rad1[JOB_TYPE_JIANDUNSHI], Mxatk_dex_rad2[JOB_TYPE_JIANDUNSHI]	= 1.5, 0.4, 0, 0	
Def_con_rad1[JOB_TYPE_JIANDUNSHI], Def_con_rad2[JOB_TYPE_JIANDUNSHI]	= 0.15,	0.1 												
Hit_dex_rad1[JOB_TYPE_JIANDUNSHI], Hit_dex_rad2[JOB_TYPE_JIANDUNSHI]	= 0.6, 0 												
Flee_agi_rad1[JOB_TYPE_JIANDUNSHI], Flee_agi_rad2[JOB_TYPE_JIANDUNSHI]	= 0.6, 0											
Mf_luk_rad[JOB_TYPE_JIANDUNSHI]		= 0.39 																	
Crt_luk_rad[JOB_TYPE_JIANDUNSHI]	= 0.31																	
Hrec_bsmxhp_rad[JOB_TYPE_JIANDUNSHI], Hrec_con_rad[JOB_TYPE_JIANDUNSHI]	= 1/180, 1/8											
Srec_bsmxsp_rad[JOB_TYPE_JIANDUNSHI], Srec_sta_rad[JOB_TYPE_JIANDUNSHI]	= 1/100, 1/12												
Aspd_agi_rad[JOB_TYPE_JIANDUNSHI]	= 1.1   												 				
Str_updata[JOB_TYPE_JIANDUNSHI]		= 0.3 
Dex_updata[JOB_TYPE_JIANDUNSHI]		= 0.1 
Con_updata[JOB_TYPE_JIANDUNSHI]		= 0.5 
Agi_updata[JOB_TYPE_JIANDUNSHI]		= 0.1 
Sta_updata[JOB_TYPE_JIANDUNSHI]		= 0.1 
Luk_updata[JOB_TYPE_JIANDUNSHI]		= 0.1 

Mxhp_con_rad1[JOB_TYPE_XUNSHOUSHI], Mxhp_con_rad2[JOB_TYPE_XUNSHOUSHI], Mxhp_lv_rad[JOB_TYPE_XUNSHOUSHI]	= 2, 2, 10 							
Mxsp_sta_rad1[JOB_TYPE_XUNSHOUSHI], Mxsp_sta_rad2[JOB_TYPE_XUNSHOUSHI], Mxsp_lv_rad[JOB_TYPE_XUNSHOUSHI]	= 0.5, 0.5, 1							
Mnatk_str_rad1[JOB_TYPE_XUNSHOUSHI], Mnatk_str_rad2[JOB_TYPE_XUNSHOUSHI], Mnatk_dex_rad1[JOB_TYPE_XUNSHOUSHI], Mnatk_dex_rad2[JOB_TYPE_XUNSHOUSHI]	= 0.8, 0.8, 0, 0	
Mxatk_str_rad1[JOB_TYPE_XUNSHOUSHI], Mxatk_str_rad2[JOB_TYPE_XUNSHOUSHI], Mxatk_dex_rad1[JOB_TYPE_XUNSHOUSHI], Mxatk_dex_rad2[JOB_TYPE_XUNSHOUSHI]	= 0.8, 0.8, 0, 0	
Def_con_rad1[JOB_TYPE_XUNSHOUSHI], Def_con_rad2[JOB_TYPE_XUNSHOUSHI]	= 0.5, 0.5 												
Hit_dex_rad1[JOB_TYPE_XUNSHOUSHI], Hit_dex_rad2[JOB_TYPE_XUNSHOUSHI]	= 0.31, 0.15 												
Flee_agi_rad1[JOB_TYPE_XUNSHOUSHI], Flee_agi_rad2[JOB_TYPE_XUNSHOUSHI]	= 0.5, 0.15												
Mf_luk_rad[JOB_TYPE_XUNSHOUSHI]		= 0.39 																	
Crt_luk_rad[JOB_TYPE_XUNSHOUSHI]	= 0.31																	
Hrec_bsmxhp_rad[JOB_TYPE_XUNSHOUSHI], Hrec_con_rad[JOB_TYPE_XUNSHOUSHI]	= 1/200, 1/100												
Srec_bsmxsp_rad[JOB_TYPE_XUNSHOUSHI], Srec_sta_rad[JOB_TYPE_XUNSHOUSHI]	= 1/200, 1/120												
Aspd_agi_rad[JOB_TYPE_XUNSHOUSHI]	= 1.2   												 				
Str_updata[JOB_TYPE_XUNSHOUSHI]		= 0.3 
Dex_updata[JOB_TYPE_XUNSHOUSHI]		= 0.1 
Con_updata[JOB_TYPE_XUNSHOUSHI]		= 0.5 
Agi_updata[JOB_TYPE_XUNSHOUSHI]		= 0.1 
Sta_updata[JOB_TYPE_XUNSHOUSHI]		= 0.1 
Luk_updata[JOB_TYPE_XUNSHOUSHI]		= 0.1 

Mxhp_con_rad1[JOB_TYPE_JUJISHOU], Mxhp_con_rad2[JOB_TYPE_JUJISHOU], Mxhp_lv_rad[JOB_TYPE_JUJISHOU]	= 5.5, 5.5, 30 						
Mxsp_sta_rad1[JOB_TYPE_JUJISHOU], Mxsp_sta_rad2[JOB_TYPE_JUJISHOU], Mxsp_lv_rad[JOB_TYPE_JUJISHOU]	= 1,0, 3						
Mnatk_str_rad1[JOB_TYPE_JUJISHOU], Mnatk_str_rad2[JOB_TYPE_JUJISHOU], Mnatk_dex_rad1[JOB_TYPE_JUJISHOU], Mnatk_dex_rad2[JOB_TYPE_JUJISHOU]	= 0,0,2, 0.5	
Mxatk_str_rad1[JOB_TYPE_JUJISHOU], Mxatk_str_rad2[JOB_TYPE_JUJISHOU], Mxatk_dex_rad1[JOB_TYPE_JUJISHOU], Mxatk_dex_rad2[JOB_TYPE_JUJISHOU]	= 0,0,2, 0.5	
Def_con_rad1[JOB_TYPE_JUJISHOU], Def_con_rad2[JOB_TYPE_JUJISHOU]	= 0.2, 0.2 										
Hit_dex_rad1[JOB_TYPE_JUJISHOU], Hit_dex_rad2[JOB_TYPE_JUJISHOU]	= 0.6, 0 										
Flee_agi_rad1[JOB_TYPE_JUJISHOU], Flee_agi_rad2[JOB_TYPE_JUJISHOU]	= 0.85, 0										
Mf_luk_rad[JOB_TYPE_JUJISHOU]	= 0.39 																
Crt_luk_rad[JOB_TYPE_JUJISHOU]	= 0.31																
Hrec_bsmxhp_rad[JOB_TYPE_JUJISHOU], Hrec_con_rad[JOB_TYPE_JUJISHOU]	= 1/180, 1/8										
Srec_bsmxsp_rad[JOB_TYPE_JUJISHOU], Srec_sta_rad[JOB_TYPE_JUJISHOU]	= 1/100, 1/12										
Aspd_agi_rad[JOB_TYPE_JUJISHOU]	= 1.2    												 			
Str_updata[JOB_TYPE_JUJISHOU]	= 0.3 
Dex_updata[JOB_TYPE_JUJISHOU]	= 0.1 
Con_updata[JOB_TYPE_JUJISHOU]	= 0.5 
Agi_updata[JOB_TYPE_JUJISHOU]	= 0.1 
Sta_updata[JOB_TYPE_JUJISHOU]	= 0.1 
Luk_updata[JOB_TYPE_JUJISHOU]	= 0.1 

Mxhp_con_rad1[JOB_TYPE_SHENGZHIZHE], Mxhp_con_rad2[JOB_TYPE_SHENGZHIZHE], Mxhp_lv_rad[JOB_TYPE_SHENGZHIZHE]	= 3.5, 3.5, 30							
Mxsp_sta_rad1[JOB_TYPE_SHENGZHIZHE], Mxsp_sta_rad2[JOB_TYPE_SHENGZHIZHE], Mxsp_lv_rad[JOB_TYPE_SHENGZHIZHE]	= 3, 1.5, 5							
Mnatk_str_rad1[JOB_TYPE_SHENGZHIZHE], Mnatk_str_rad2[JOB_TYPE_SHENGZHIZHE], Mnatk_dex_rad1[JOB_TYPE_SHENGZHIZHE], Mnatk_dex_rad2[JOB_TYPE_SHENGZHIZHE] = 1.5, 0.4, 0, 0		
Mxatk_str_rad1[JOB_TYPE_SHENGZHIZHE], Mxatk_str_rad2[JOB_TYPE_SHENGZHIZHE], Mxatk_dex_rad1[JOB_TYPE_SHENGZHIZHE], Mxatk_dex_rad2[JOB_TYPE_SHENGZHIZHE] = 1.5, 0.4, 0, 0		
Def_con_rad1[JOB_TYPE_SHENGZHIZHE], Def_con_rad2[JOB_TYPE_SHENGZHIZHE]		= 0.15,	0.15 											
Hit_dex_rad1[JOB_TYPE_SHENGZHIZHE], Hit_dex_rad2[JOB_TYPE_SHENGZHIZHE]		= 0.6, 0 											
Flee_agi_rad1[JOB_TYPE_SHENGZHIZHE], Flee_agi_rad2[JOB_TYPE_SHENGZHIZHE]	= 0.85, 0											
Mf_luk_rad[JOB_TYPE_SHENGZHIZHE]	= 0.39 																	
Crt_luk_rad[JOB_TYPE_SHENGZHIZHE]	= 0.31																	
Hrec_bsmxhp_rad[JOB_TYPE_SHENGZHIZHE], Hrec_con_rad[JOB_TYPE_SHENGZHIZHE] = 1/180, 1/8												
Srec_bsmxsp_rad[JOB_TYPE_SHENGZHIZHE], Srec_sta_rad[JOB_TYPE_SHENGZHIZHE] = 1/100, 1/12												
Aspd_agi_rad[JOB_TYPE_SHENGZHIZHE]	= 1.1   												 				
Str_updata[JOB_TYPE_SHENGZHIZHE]	= 0.3 
Dex_updata[JOB_TYPE_SHENGZHIZHE]	= 0.1 
Con_updata[JOB_TYPE_SHENGZHIZHE]	= 0.5 
Agi_updata[JOB_TYPE_SHENGZHIZHE]	= 0.1 
Sta_updata[JOB_TYPE_SHENGZHIZHE]	= 0.1 
Luk_updata[JOB_TYPE_SHENGZHIZHE]	= 0.1 

Mxhp_con_rad1[JOB_TYPE_FENGYINSHI], Mxhp_con_rad2[JOB_TYPE_FENGYINSHI], Mxhp_lv_rad[JOB_TYPE_FENGYINSHI]	= 5.5, 5.5, 30							
Mxsp_sta_rad1[JOB_TYPE_FENGYINSHI], Mxsp_sta_rad2[JOB_TYPE_FENGYINSHI], Mxsp_lv_rad[JOB_TYPE_FENGYINSHI]	= 3, 1.5, 5							
Mnatk_str_rad1[JOB_TYPE_FENGYINSHI], Mnatk_str_rad2[JOB_TYPE_FENGYINSHI], Mnatk_dex_rad1[JOB_TYPE_FENGYINSHI], Mnatk_dex_rad2[JOB_TYPE_FENGYINSHI]	= 1.5, 0.4, 0, 0	
Mxatk_str_rad1[JOB_TYPE_FENGYINSHI], Mxatk_str_rad2[JOB_TYPE_FENGYINSHI], Mxatk_dex_rad1[JOB_TYPE_FENGYINSHI], Mxatk_dex_rad2[JOB_TYPE_FENGYINSHI]	= 1.5, 0.4, 0, 0	
Def_con_rad1[JOB_TYPE_FENGYINSHI], Def_con_rad2[JOB_TYPE_FENGYINSHI]	= 0.2, 0.3 												
Hit_dex_rad1[JOB_TYPE_FENGYINSHI], Hit_dex_rad2[JOB_TYPE_FENGYINSHI]	= 0.6, 0 												
Flee_agi_rad1[JOB_TYPE_FENGYINSHI], Flee_agi_rad2[JOB_TYPE_FENGYINSHI]	= 0.75, 0												
Mf_luk_rad[JOB_TYPE_FENGYINSHI]		= 0.39 																	
Crt_luk_rad[JOB_TYPE_FENGYINSHI]	= 0.31																	
Hrec_bsmxhp_rad[JOB_TYPE_FENGYINSHI], Hrec_con_rad[JOB_TYPE_FENGYINSHI]	= 1/180, 1/8												
Srec_bsmxsp_rad[JOB_TYPE_FENGYINSHI], Srec_sta_rad[JOB_TYPE_FENGYINSHI]	= 1/100, 1/12												
Aspd_agi_rad[JOB_TYPE_FENGYINSHI]	= 1.1 												 					
Str_updata[JOB_TYPE_FENGYINSHI]		= 0.3 
Dex_updata[JOB_TYPE_FENGYINSHI]		= 0.1 
Con_updata[JOB_TYPE_FENGYINSHI]		= 0.5 
Agi_updata[JOB_TYPE_FENGYINSHI]		= 0.1 
Sta_updata[JOB_TYPE_FENGYINSHI]		= 0.1 
Luk_updata[JOB_TYPE_FENGYINSHI]		= 0.1 

Mxhp_con_rad1[JOB_TYPE_CHUANZHANG], Mxhp_con_rad2[JOB_TYPE_CHUANZHANG], Mxhp_lv_rad[JOB_TYPE_CHUANZHANG]	= 2, 2, 10 							
Mxsp_sta_rad1[JOB_TYPE_CHUANZHANG], Mxsp_sta_rad2[JOB_TYPE_CHUANZHANG], Mxsp_lv_rad[JOB_TYPE_CHUANZHANG]	= 0.5, 0.5, 1							
Mnatk_str_rad1[JOB_TYPE_CHUANZHANG], Mnatk_str_rad2[JOB_TYPE_CHUANZHANG], Mnatk_dex_rad1[JOB_TYPE_CHUANZHANG], Mnatk_dex_rad2[JOB_TYPE_CHUANZHANG]	= 0.8, 0.8, 0, 0	
Mxatk_str_rad1[JOB_TYPE_CHUANZHANG], Mxatk_str_rad2[JOB_TYPE_CHUANZHANG], Mxatk_dex_rad1[JOB_TYPE_CHUANZHANG], Mxatk_dex_rad2[JOB_TYPE_CHUANZHANG]	= 0.8, 0.8, 0, 0	
Def_con_rad1[JOB_TYPE_CHUANZHANG], Def_con_rad2[JOB_TYPE_CHUANZHANG]	= 0.5, 0.5 												
Hit_dex_rad1[JOB_TYPE_CHUANZHANG], Hit_dex_rad2[JOB_TYPE_CHUANZHANG]	= 0.31, 0.15 												
Flee_agi_rad1[JOB_TYPE_CHUANZHANG], Flee_agi_rad2[JOB_TYPE_CHUANZHANG]	= 0.31, 0.15												
Mf_luk_rad[JOB_TYPE_CHUANZHANG]		= 0.39 																	
Crt_luk_rad[JOB_TYPE_CHUANZHANG]	= 0.31																	
Hrec_bsmxhp_rad[JOB_TYPE_CHUANZHANG], Hrec_con_rad[JOB_TYPE_CHUANZHANG]	= 1/200, 1/100												
Srec_bsmxsp_rad[JOB_TYPE_CHUANZHANG], Srec_sta_rad[JOB_TYPE_CHUANZHANG] = 1/200, 1/120												
Aspd_agi_rad[JOB_TYPE_CHUANZHANG]	= 1.1   												 				
Str_updata[JOB_TYPE_CHUANZHANG]		= 0.3 
Dex_updata[JOB_TYPE_CHUANZHANG]		= 0.1 
Con_updata[JOB_TYPE_CHUANZHANG]		= 0.5 
Agi_updata[JOB_TYPE_CHUANZHANG]		= 0.1 
Sta_updata[JOB_TYPE_CHUANZHANG]		= 0.1 
Luk_updata[JOB_TYPE_CHUANZHANG]		= 0.1 

Mxhp_con_rad1[JOB_TYPE_HANGHAISHI], Mxhp_con_rad2[JOB_TYPE_HANGHAISHI], Mxhp_lv_rad[JOB_TYPE_HANGHAISHI]	= 6, 6, 35						
Mxsp_sta_rad1[JOB_TYPE_HANGHAISHI], Mxsp_sta_rad2[JOB_TYPE_HANGHAISHI], Mxsp_lv_rad[JOB_TYPE_HANGHAISHI]	= 3, 1.5, 5						
Mnatk_str_rad1[JOB_TYPE_HANGHAISHI], Mnatk_str_rad2[JOB_TYPE_HANGHAISHI], Mnatk_dex_rad1[JOB_TYPE_HANGHAISHI], Mnatk_dex_rad2[JOB_TYPE_HANGHAISHI] = 1.5, 0.4, 0, 0	
Mxatk_str_rad1[JOB_TYPE_HANGHAISHI], Mxatk_str_rad2[JOB_TYPE_HANGHAISHI], Mxatk_dex_rad1[JOB_TYPE_HANGHAISHI], Mxatk_dex_rad2[JOB_TYPE_HANGHAISHI] = 1.5, 0.4, 0, 0	
Def_con_rad1[JOB_TYPE_HANGHAISHI], Def_con_rad2[JOB_TYPE_HANGHAISHI]	= 0.2, 0.4 											
Hit_dex_rad1[JOB_TYPE_HANGHAISHI], Hit_dex_rad2[JOB_TYPE_HANGHAISHI]	= 0.6, 0 											
Flee_agi_rad1[JOB_TYPE_HANGHAISHI], Flee_agi_rad2[JOB_TYPE_HANGHAISHI]	= 0.7, 0											
Mf_luk_rad[JOB_TYPE_HANGHAISHI]		= 0.39 																
Crt_luk_rad[JOB_TYPE_HANGHAISHI]	= 0.31																
Hrec_bsmxhp_rad[JOB_TYPE_HANGHAISHI], Hrec_con_rad[JOB_TYPE_HANGHAISHI]	= 1/180, 1/8											
Srec_bsmxsp_rad[JOB_TYPE_HANGHAISHI], Srec_sta_rad[JOB_TYPE_HANGHAISHI]	= 1/100, 1/12											
Aspd_agi_rad[JOB_TYPE_HANGHAISHI]	= 1.1   												 			
Str_updata[JOB_TYPE_HANGHAISHI]		= 0.3 
Dex_updata[JOB_TYPE_HANGHAISHI]		= 0.1 
Con_updata[JOB_TYPE_HANGHAISHI]		= 0.5 
Agi_updata[JOB_TYPE_HANGHAISHI]		= 0.1 
Sta_updata[JOB_TYPE_HANGHAISHI]		= 0.1 
Luk_updata[JOB_TYPE_HANGHAISHI]		= 0.1 

Mxhp_con_rad1[JOB_TYPE_BAOFAHU], Mxhp_con_rad2[JOB_TYPE_BAOFAHU], Mxhp_lv_rad[JOB_TYPE_BAOFAHU]	= 2, 2, 10 							
Mxsp_sta_rad1[JOB_TYPE_BAOFAHU], Mxsp_sta_rad2[JOB_TYPE_BAOFAHU], Mxsp_lv_rad[JOB_TYPE_BAOFAHU]	= 0.5, 0.5, 1							
Mnatk_str_rad1[JOB_TYPE_BAOFAHU], Mnatk_str_rad2[JOB_TYPE_BAOFAHU], Mnatk_dex_rad1[JOB_TYPE_BAOFAHU], Mnatk_dex_rad2[JOB_TYPE_BAOFAHU]	= 0.8, 0.8, 0, 0	
Mxatk_str_rad1[JOB_TYPE_BAOFAHU], Mxatk_str_rad2[JOB_TYPE_BAOFAHU], Mxatk_dex_rad1[JOB_TYPE_BAOFAHU], Mxatk_dex_rad2[JOB_TYPE_BAOFAHU]	= 0.8, 0.8, 0, 0	
Def_con_rad1[JOB_TYPE_BAOFAHU], Def_con_rad2[JOB_TYPE_BAOFAHU]		= 0.5, 0.5 										
Hit_dex_rad1[JOB_TYPE_BAOFAHU], Hit_dex_rad2[JOB_TYPE_BAOFAHU]		= 0.31, 0.15 										
Flee_agi_rad1[JOB_TYPE_BAOFAHU], Flee_agi_rad2[JOB_TYPE_BAOFAHU]	= 0.31, 0.15										
Mf_luk_rad[JOB_TYPE_BAOFAHU]	= 0.39 																
Crt_luk_rad[JOB_TYPE_BAOFAHU]	= 0.31																
Hrec_bsmxhp_rad[JOB_TYPE_BAOFAHU], Hrec_con_rad[JOB_TYPE_BAOFAHU]	= 1/200, 1/100										
Srec_bsmxsp_rad[JOB_TYPE_BAOFAHU], Srec_sta_rad[JOB_TYPE_BAOFAHU]	= 1/200, 1/120										
Aspd_agi_rad[JOB_TYPE_BAOFAHU]	= 1.1   												 			
Str_updata[JOB_TYPE_BAOFAHU]	= 0.3 
Dex_updata[JOB_TYPE_BAOFAHU]	= 0.1 
Con_updata[JOB_TYPE_BAOFAHU]	= 0.5 
Agi_updata[JOB_TYPE_BAOFAHU]	= 0.1 
Sta_updata[JOB_TYPE_BAOFAHU]	= 0.1 
Luk_updata[JOB_TYPE_BAOFAHU]	= 0.1 

Mxhp_con_rad1[JOB_TYPE_GONGCHENGSHI], Mxhp_con_rad2[JOB_TYPE_GONGCHENGSHI], Mxhp_lv_rad[JOB_TYPE_GONGCHENGSHI]	= 2, 2, 10 								
Mxsp_sta_rad1[JOB_TYPE_GONGCHENGSHI], Mxsp_sta_rad2[JOB_TYPE_GONGCHENGSHI], Mxsp_lv_rad[JOB_TYPE_GONGCHENGSHI]	= 0.5, 0.5, 1								
Mnatk_str_rad1[JOB_TYPE_GONGCHENGSHI], Mnatk_str_rad2[JOB_TYPE_GONGCHENGSHI], Mnatk_dex_rad1[JOB_TYPE_GONGCHENGSHI], Mnatk_dex_rad2[JOB_TYPE_GONGCHENGSHI]	= 0.8, 0.8, 0, 0	
Mxatk_str_rad1[JOB_TYPE_GONGCHENGSHI], Mxatk_str_rad2[JOB_TYPE_GONGCHENGSHI], Mxatk_dex_rad1[JOB_TYPE_GONGCHENGSHI], Mxatk_dex_rad2[JOB_TYPE_GONGCHENGSHI]	= 0.8, 0.8, 0, 0	
Def_con_rad1[JOB_TYPE_GONGCHENGSHI], Def_con_rad2[JOB_TYPE_GONGCHENGSHI]	= 0.5, 0.5 												
Hit_dex_rad1[JOB_TYPE_GONGCHENGSHI], Hit_dex_rad2[JOB_TYPE_GONGCHENGSHI]	= 0.31, 0.15 												
Flee_agi_rad1[JOB_TYPE_GONGCHENGSHI], Flee_agi_rad2[JOB_TYPE_GONGCHENGSHI]	= 0.31, 0.15												
Mf_luk_rad[JOB_TYPE_GONGCHENGSHI]	= 0.39 																		
Crt_luk_rad[JOB_TYPE_GONGCHENGSHI]	= 0.31																		
Hrec_bsmxhp_rad[JOB_TYPE_GONGCHENGSHI], Hrec_con_rad[JOB_TYPE_GONGCHENGSHI]	= 1/200, 1/100												
Srec_bsmxsp_rad[JOB_TYPE_GONGCHENGSHI], Srec_sta_rad[JOB_TYPE_GONGCHENGSHI]	= 1/200, 1/120												
Aspd_agi_rad[JOB_TYPE_GONGCHENGSHI]	= 1.1   												 					
Str_updata[JOB_TYPE_GONGCHENGSHI]	= 0.3 
Dex_updata[JOB_TYPE_GONGCHENGSHI]	= 0.1 
Con_updata[JOB_TYPE_GONGCHENGSHI]	= 0.5 
Agi_updata[JOB_TYPE_GONGCHENGSHI]	= 0.1 
Sta_updata[JOB_TYPE_GONGCHENGSHI]	= 0.1 
Luk_updata[JOB_TYPE_GONGCHENGSHI]	= 0.1 

function Creat_Item(item, item_type, item_lv, item_event)
    item_event = item_event
    item_type = item_type
    item_lv = item_lv
    Reset_item_add()

    local i = 0
    local Num = 0

	if GetItemID(item) >= 0128 and GetItemID(item) <= 0140 then
		SetItemPrefix(item, 95)
	end
	
    if GetItemID(item) == Server.Cloak.Item.ID then
        Add_Item_Attr(ITEMATTR_VAL_STR, 1)
        Add_Item_Attr(ITEMATTR_VAL_CON, 1)
        Add_Item_Attr(ITEMATTR_VAL_AGI, 1)
        Add_Item_Attr(ITEMATTR_VAL_STA, 1)
        Add_Item_Attr(ITEMATTR_VAL_DEX, 1)
		SetItemAttr(item, 55, 1)
		SetItemLv(item, 1)
    end

    if item_event == NPC_SALE then
        Npc_Sale(item_type, item_lv, item_event)
        SetItemForgeParam_Npc_Sale(item, Num)
    elseif item_event == MONSTER_BAOLIAO then
        Monster_Baoliao(item_type, item_lv, item_event)
        SetItemForgeParam_MonsterBaoliao(item, Num)
    elseif item_event == PLAYER_HECHENG then
        Player_Hecheng(item_type, item_lv, item_event)
        SetItemForgeParam_PlayerHecheng(item, Num)
    elseif item_event == PLAYER_XSBOX then
        Player_XSBox(item_type, item_lv, item_event)
        local Num = GetItemForgeParam(item, 1)
        local Part1 = GetNum_Part1(Num)
        local Part2 = GetNum_Part2(Num)
        local Part3 = GetNum_Part3(Num)
        local Part4 = GetNum_Part4(Num)
        local Part5 = GetNum_Part5(Num)
        local Part6 = GetNum_Part6(Num)
        local Part7 = GetNum_Part7(Num)
        if item_type == 1 then
            Part1 = 1
            Part2 = 1
            Part3 = 1
        end
        if item_type == 2 then
            Part1 = 1
            Part2 = 2
            Part3 = 1
        end
        if item_type == 3 then
            Part1 = 1
            Part2 = 2
            Part3 = 1
        end
        if item_type == 4 then
            Part1 = 1
            Part2 = 3
            Part3 = 1
        end
        if item_type == 7 then
            Part1 = 1
            Part2 = 8
            Part3 = 1
        end
        if item_type == 9 then
            Part1 = 1
            Part2 = 8
            Part3 = 1
        end
        Num = SetNum_Part3(Num, Part3)
        Num = SetNum_Part2(Num, Part2)
        Num = SetNum_Part1(Num, Part1)
        SetItemForgeParam(item, 1, Num)
    elseif item_event == PLAYER_CCFSBOXA then
        Player_CCFSBoxA(item_type, item_lv, item_event)
    elseif item_event == PLAYER_CCFSBOXB then
        Player_CCFSBoxB(item_type, item_lv, item_event)
    elseif item_event == PLAYER_CCFSBOXC then
        Player_CCFSBoxC(item_type, item_lv, item_event)
    elseif item_event == PLAYER_CCFSBOXD then
        Player_CCFSBoxD(item_type, item_lv, item_event)
    elseif item_event == PLAYER_CCFSBOXE then
        Player_CCFSBoxE(item_type, item_lv, item_event)
    elseif item_event == PLAYER_CCFSBOXF then
        Player_CCFSBoxF(item_type, item_lv, item_event)
    elseif item_event == PLAYER_CCFSBOXG then
        Player_CCFSBoxG(item_type, item_lv, item_event)
    elseif item_event == PLAYER_CCFSBOXH then
        Player_CCFSBoxH(item_type, item_lv, item_event)
    elseif item_event == PLAYER_CCFSBOXI then
        Player_CCFSBoxI(item_type, item_lv, item_event)
    elseif item_event == PLAYER_ZSITEM then
        Player_ZSitem(item_type, item_lv, item_event)
    elseif item_event == PLAYER_HSSR then
        Player_HSSR(item_type, item_lv, item_event)
    elseif item_event == PLAYER_HSSRA then
        Player_HSSRA(item_type, item_lv, item_event)
    elseif item_event >= QUEST_AWARD_1 then
        Quest_Award(item_type, item_lv, item_event)
        SetItemForgeParam_QuestAward(item, Num, item_event)
    else
        item_add.cnt = 0
    end

    return item_add.cnt, 
	item_add.attr[1][1], 
	item_add.attr[1][2], 
	item_add.attr[2][1], 
	item_add.attr[2][2], 
	item_add.attr[3][1], 
	item_add.attr[3][2], 
	item_add.attr[4][1], 
	item_add.attr[4][2], 
	item_add.attr[5][1], 
	item_add.attr[5][2], 
	item_add.attr[6][1], 
	item_add.attr[6][2], 
	item_add.attr[7][1], 
	item_add.attr[7][2]
end

function Npc_Sale(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end

function Monster_Baoliao(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end

function Player_Hecheng(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end

function Quest_Award(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end

function Player_XSBox(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end

function Player_CCFSBoxA(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end

function Player_CCFSBoxB(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end
function Player_CCFSBoxC(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end
function Player_CCFSBoxD(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end
function Player_CCFSBoxE(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end
function Player_CCFSBoxF(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end
function Player_CCFSBoxG(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end
function Player_CCFSBoxH(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end
function Player_CCFSBoxI(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end

function Player_ZSitem(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end

function quest_award_godbox(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end

function Player_HSSR(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end
function Player_HSSRA(item_type, item_lv, item_event)
    Creat_Item_Tattr(item_type, item_lv, item_event)
end
function Creat_Item_Battr(item_type, item_lv, item_event)
    if item_type >= 1 and item_type <= 10 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_VAL_MNATK, 0)
        Add_Item_Attr(ITEMATTR_VAL_MXATK, 0)

        if item_type == 1 then
            Add_Item_Attr(ITEMATTR_VAL_HIT, 0)
            Add_Item_Attr(ITEMATTR_COE_ASPD, 0)
        elseif item_type == 2 then
            Add_Item_Attr(ITEMATTR_VAL_DEF, 0)
            Add_Item_Attr(ITEMATTR_VAL_MXHP, 0)
        elseif item_type == 3 then
            Add_Item_Attr(ITEMATTR_COE_ASPD, 0)
            Add_Item_Attr(ITEMATTR_VAL_HIT, 0)
        elseif item_type == 4 then
            Add_Item_Attr(ITEMATTR_VAL_HIT, 0)
            Add_Item_Attr(ITEMATTR_COE_ASPD, 0)
        elseif item_type == 7 then
            Add_Item_Attr(ITEMATTR_VAL_STA, 0)
            Add_Item_Attr(ITEMATTR_COE_MXSP, 0)
            Add_Item_Attr(ITEMATTR_COE_MSPD, 0)
        elseif item_type == 9 then
            Add_Item_Attr(ITEMATTR_VAL_STA, 0)
            Add_Item_Attr(ITEMATTR_COE_MXSP, 0)
            Add_Item_Attr(ITEMATTR_COE_MXHP, 0)
        end
    elseif item_type == 11 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_VAL_DEF, 0)
        Add_Item_Attr(ITEMATTR_VAL_PDEF, 0)
    elseif item_type == 20 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_VAL_DEF, 0)
    elseif item_type == 22 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_VAL_DEF, 0)
        Add_Item_Attr(ITEMATTR_VAL_PDEF, 0)
        Add_Item_Attr(ITEMATTR_VAL_MXSP, 0)
        Add_Item_Attr(ITEMATTR_VAL_AGI, 0)
    elseif item_type == 27 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_VAL_DEF, 0)
        Add_Item_Attr(ITEMATTR_VAL_PDEF, 0)
    elseif item_type == 23 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_VAL_HIT, 0)
        Add_Item_Attr(ITEMATTR_VAL_DEF, 0)
        Add_Item_Attr(ITEMATTR_VAL_DEX, 0)
        Add_Item_Attr(ITEMATTR_VAL_SREC, 0)
    elseif item_type == 24 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_VAL_FLEE, 0)
        Add_Item_Attr(ITEMATTR_VAL_DEF, 0)
    elseif item_type == 29 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_MAXENERGY, 0)
    elseif item_type == 26 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_VAL_MXATK, 0)
        Add_Item_Attr(ITEMATTR_VAL_DEF, 0)
        Add_Item_Attr(ITEMATTR_VAL_FLEE, 0)
        Add_Item_Attr(ITEMATTR_VAL_HIT, 0)
        Add_Item_Attr(ITEMATTR_VAL_CRT, 0)
    elseif item_type == 25 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_VAL_MXHP, 0)
        Add_Item_Attr(ITEMATTR_VAL_MXSP, 0)
        Add_Item_Attr(ITEMATTR_VAL_SREC, 0)
        Add_Item_Attr(ITEMATTR_VAL_HREC, 0)
        Add_Item_Attr(ITEMATTR_VAL_PDEF, 0)
    elseif item_type == 46 then
    elseif item_type == 59 then
    else
    end
end

function Creat_Item_Tattr(item_type, item_lv, item_event)
    local quality = SetItemQua(item_event)

    if item_event == 101 then
        Add_Item_Attr(ITEMATTR_VAL_BaoshiLV, 10)
        return
    end
    if item_event == 102 then
        Add_Item_Attr(ITEMATTR_VAL_BaoshiLV, 20)
        return
    end
    if item_event == 103 then
        Add_Item_Attr(ITEMATTR_VAL_BaoshiLV, 30)
        return
    end
    if item_event == 104 then
        Add_Item_Attr(ITEMATTR_VAL_BaoshiLV, 40)
        return
    end
    if item_event == 105 then
        Add_Item_Attr(ITEMATTR_VAL_BaoshiLV, 50)
        return
    end
    if item_event == 106 then
        Add_Item_Attr(ITEMATTR_VAL_BaoshiLV, 60)
        return
    end
    if item_event == 107 then
        Add_Item_Attr(ITEMATTR_VAL_BaoshiLV, 70)
        return
    end
    if item_event == 108 then
        Add_Item_Attr(ITEMATTR_VAL_BaoshiLV, 80)
        return
    end
    if item_event == 109 then
        Add_Item_Attr(ITEMATTR_VAL_BaoshiLV, 90)
        return
    end
    if item_type >= 1 and item_type <= 10 then
        Add_Item_Attr(ITEMATTR_MAXURE, SetItemQua(item_event))
        Add_Item_Attr(ITEMATTR_VAL_MNATK, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_MXATK, quality * 10)

		if item_type == 1 then
			CreatItemAttr(item_type, item_lv, item_event, quality)
		elseif item_type == 2 then
			CreatItemAttr(item_type, item_lv, item_event, quality)
		elseif item_type == 3 then
			CreatItemAttr(item_type, item_lv, item_event, quality)
		elseif item_type == 4 then
			CreatItemAttr(item_type, item_lv, item_event, quality)
		elseif item_type == 7 then
			CreatItemAttr(item_type, item_lv, item_event, quality)
		elseif item_type == 9 then
			CreatItemAttr(item_type, item_lv, item_event, quality)
		end
    elseif item_type == 11 then
        Add_Item_Attr(ITEMATTR_MAXURE, SetItemQua(item_event))
        Add_Item_Attr(ITEMATTR_VAL_DEF, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_PDEF, quality)
        CreatItemAttr(item_type, item_lv, item_event, quality)
    elseif item_type == 20 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_VAL_DEF, quality * 10)
        CreatItemAttr(item_type, item_lv, item_event, quality)
    elseif item_type == 22 then
        Add_Item_Attr(ITEMATTR_MAXURE, SetItemQua(item_event))
        Add_Item_Attr(ITEMATTR_VAL_DEF, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_PDEF, quality * 10)

        CreatItemAttr(item_type, item_lv, item_event, quality)
    elseif item_type == 27 then
        Add_Item_Attr(ITEMATTR_MAXURE, SetItemQua(item_event))
        Add_Item_Attr(ITEMATTR_VAL_DEF, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_PDEF, SetItemQua(item_event))
        CreatItemAttr(item_type, item_lv, item_event, quality)
    elseif item_type == 23 then
        Add_Item_Attr(ITEMATTR_MAXURE, SetItemQua(item_event))
        Add_Item_Attr(ITEMATTR_VAL_HIT, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_DEF, quality * 10)

        CreatItemAttr(item_type, item_lv, item_event, quality)
    elseif item_type == 24 then
        Add_Item_Attr(ITEMATTR_MAXURE, SetItemQua(item_event))
        Add_Item_Attr(ITEMATTR_VAL_FLEE, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_DEF, quality * 10)
        CreatItemAttr(item_type, item_lv, item_event, quality)
    elseif item_type == 29 then
        Add_Item_Attr(ITEMATTR_MAXURE, 0)
        Add_Item_Attr(ITEMATTR_MAXENERGY, 0)
    elseif item_type == 26 then
        Add_Item_Attr(ITEMATTR_MAXURE, SetItemQua(item_event))
        Add_Item_Attr(ITEMATTR_VAL_MXATK, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_DEF, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_FLEE, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_HIT, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_CRT, quality * 10)
        CreatItemAttr(item_type, item_lv, item_event, quality)
    elseif item_type == 25 then
        Add_Item_Attr(ITEMATTR_MAXURE, SetItemQua(item_event))
        Add_Item_Attr(ITEMATTR_VAL_MXHP, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_MXSP, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_SREC, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_HREC, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_PDEF, quality * 10)
        CreatItemAttr(item_type, item_lv, item_event, quality)
    elseif item_type == 46 then
        CreatItemAttr(item_type, item_lv, item_event, quality)
    elseif item_type == 49 then
        Add_Item_Attr(ITEMATTR_VAL_BaoshiLV, 10)
    elseif item_type == 50 then
        Add_Item_Attr(ITEMATTR_VAL_BaoshiLV, 10)
    elseif item_type == 59 then
        Add_Item_Attr(ITEMATTR_VAL_STR, 10)
        Add_Item_Attr(ITEMATTR_VAL_DEX, 10)
        Add_Item_Attr(ITEMATTR_VAL_CON, 10)
        Add_Item_Attr(ITEMATTR_VAL_AGI, 10)
        Add_Item_Attr(ITEMATTR_VAL_STA, 10)
    elseif item_type == 65 then
        Add_Item_Attr(ITEMATTR_VAL_STR, 10)
        Add_Item_Attr(ITEMATTR_VAL_DEX, 10)
        Add_Item_Attr(ITEMATTR_VAL_CON, 10)
        Add_Item_Attr(ITEMATTR_VAL_AGI, 10)
        Add_Item_Attr(ITEMATTR_VAL_STA, 10)
    elseif item_type == 68 then
        Add_Item_Attr(ITEMATTR_VAL_STR, 10)
        Add_Item_Attr(ITEMATTR_VAL_DEX, 10)
        Add_Item_Attr(ITEMATTR_VAL_CON, 10)
        Add_Item_Attr(ITEMATTR_VAL_AGI, 10)
        Add_Item_Attr(ITEMATTR_VAL_STA, 10)
    elseif item_type == 69 then
        Add_Item_Attr(ITEMATTR_VAL_STR, 10)
        Add_Item_Attr(ITEMATTR_VAL_DEX, 10)
        Add_Item_Attr(ITEMATTR_VAL_CON, 10)
        Add_Item_Attr(ITEMATTR_VAL_AGI, 10)
        Add_Item_Attr(ITEMATTR_VAL_STA, 10)
    elseif item_type == 70 then
        Add_Item_Attr(ITEMATTR_VAL_STR, 10)
    elseif item_type == 71 then
        Add_Item_Attr(ITEMATTR_VAL_STR, 10)
    else
    end
end

function SetItemQua(item_event)
    local qua = 0
    if item_event == NPC_SALE then
        qua = 0
    elseif item_event == MONSTER_BAOLIAO then
        qua = Item_Quality_Ran(Item_Baoliao)
    elseif item_event == QUEST_AWARD_1 then
        qua = Item_Quality_Ran(Item_Mission_1)
    elseif item_event == QUEST_AWARD_2 then
        qua = Item_Quality_Ran(Item_Mission_2)
    elseif item_event == QUEST_AWARD_3 then
        qua = Item_Quality_Ran(Item_Mission_3)
    elseif item_event == QUEST_AWARD_4 then
        qua = Item_Quality_Ran(Item_Mission_4)
    elseif item_event == QUEST_AWARD_5 then
        qua = Item_Quality_Ran(Item_Mission_5)
    elseif item_event == PLAYER_XSBOX then
        qua = Item_Quality_Ran(Item_Mission_11)
    elseif item_event == PLAYER_CCFSBOXA then
        qua = Item_Quality_Ran(Item_Mission_12)
    elseif item_event == PLAYER_CCFSBOXB then
        qua = Item_Quality_Ran(Item_Mission_13)
    elseif item_event == PLAYER_CCFSBOXC then
        qua = Item_Quality_Ran(Item_Mission_14)
    elseif item_event == PLAYER_CCFSBOXD then
        qua = Item_Quality_Ran(Item_Mission_15)
    elseif item_event == PLAYER_CCFSBOXE then
        qua = Item_Quality_Ran(Item_Mission_16)
    elseif item_event == PLAYER_CCFSBOXF then
        qua = Item_Quality_Ran(Item_Mission_17)
    elseif item_event == PLAYER_CCFSBOXG then
        qua = Item_Quality_Ran(Item_Mission_18)
    elseif item_event == PLAYER_CCFSBOXH then
        qua = Item_Quality_Ran(Item_Mission_19)
    elseif item_event == PLAYER_CCFSBOXI then
        qua = Item_Quality_Ran(Item_Mission_20)
    elseif item_event == PLAYER_ZSITEM then
        qua = Item_Quality_Ran(Item_Mission_22)
    elseif item_event == PLAYER_HSSR then
        qua = Item_Quality_Ran(Item_Mission_23)
    elseif item_event == PLAYER_HSSRA then
        qua = Item_Quality_Ran(Item_Mission_24)
    elseif item_event == QUEST_AWARD_GODBOX then
        qua = Item_Quality_Ran(Item_Mission_94)
    elseif item_event == QUEST_AWARD_SCBOX then
        qua = Item_Quality_Ran(Item_Mission_95)
    elseif item_event == QUEST_AWARD_SDJ then
        qua = Item_Quality_Ran(Item_Mission_96)
    elseif item_event == QUEST_AWARD_RYZ then
        qua = Item_Quality_Ran(Item_Mission_97)
    elseif item_event == QUEST_AWARD_WZX then
        qua = Item_Quality_Ran(Item_Mission_98)
    elseif item_event == QUEST_AWARD_RAND then
        qua = Item_Quality_Ran(Item_Mission_99)
    end

    return qua
end

function Item_Quality_Ran(item_type_ran)
    local a = math.random(1, 100)

    local b = 0
    for i = 0, 9, 1 do
        if a <= item_type_ran[i] then
            b = (10 - i)
            return b
        end
    end

    return b
end

function CreateItemAttrCount(item_type, item_lv, item_event, quality, item_attrcount_ran)
    local a = math.random(1, 100)

    if
        item_event == PLAYER_CCFSBOXA or item_event == PLAYER_CCFSBOXB or item_event == PLAYER_CCFSBOXC or
            item_event == PLAYER_CCFSBOXD or
            item_event == PLAYER_CCFSBOXE or
            item_event == PLAYER_CCFSBOXF or
            item_event == PLAYER_CCFSBOXG or
            item_event == PLAYER_CCFSBOXH or
            item_event == PLAYER_CCFSBOXI
     then
        return 6
    end
    for i = 0, 4, 1 do
        if a <= item_attrcount_ran[i] then
            return 5 - i
        end
    end
    return 0
end

function CreatItemAttr(item_type, item_lv, item_event, quality)
    local count = 0
    local energy = 0
    local eleven = 0
    if item_lv <= 10 and item_type ~= 46 then
        Add_Item_Attr(ITEMATTR_MAXENERGY, 0)
        return
    end
    if item_event == NPC_SALE then
        Add_Item_Attr(ITEMATTR_MAXENERGY, 0)
        return
    elseif item_event == MONSTER_BAOLIAO then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_0)
    elseif item_event == QUEST_AWARD_1 then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_1)
    elseif item_event == QUEST_AWARD_2 then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_2)
    elseif item_event == QUEST_AWARD_3 then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_3)
    elseif item_event == QUEST_AWARD_4 then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_4)
    elseif item_event == QUEST_AWARD_5 then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_5)
    elseif item_event == PLAYER_XSBOX then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_11)
    elseif item_event == PLAYER_CCFSBOXA then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_12)
    elseif item_event == PLAYER_CCFSBOXB then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_13)
    elseif item_event == PLAYER_CCFSBOXC then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_14)
    elseif item_event == PLAYER_CCFSBOXD then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_15)
    elseif item_event == PLAYER_CCFSBOXE then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_16)
    elseif item_event == PLAYER_CCFSBOXF then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_17)
    elseif item_event == PLAYER_CCFSBOXG then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_18)
    elseif item_event == PLAYER_CCFSBOXH then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_19)
    elseif item_event == PLAYER_CCFSBOXI then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_20)
    elseif item_event == PLAYER_ZSITEM then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_22)
    elseif item_event == PLAYER_HSSR then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_23)
    elseif item_event == PLAYER_HSSRA then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_24)
    elseif item_event == QUEST_AWARD_GODBOX then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_94)
    elseif item_event == QUEST_AWARD_SCBOX then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_95)
    elseif item_event == QUEST_AWARD_SDJ then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_96)
    elseif item_event == QUEST_AWARD_RYZ then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_97)
    elseif item_event == QUEST_AWARD_WZX then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_98)
    elseif item_event == QUEST_AWARD_RAND then
        count = CreateItemAttrCount(item_type, item_lv, item_event, quality, Item_Attr_99)
    end
    if count >= 2 and item_type ~= 46 then
        count = math.max(1, math.min(quality - 1, count))
    end
    if item_event == 25 or item_event == 26 then
        count = 0
    end
    if
        item_event == PLAYER_CCFSBOXA or item_event == PLAYER_CCFSBOXB or item_event == PLAYER_CCFSBOXC or
            item_event == PLAYER_CCFSBOXD or
            item_event == PLAYER_CCFSBOXE or
            item_event == PLAYER_CCFSBOXF or
            item_event == PLAYER_CCFSBOXG or
            item_event == PLAYER_CCFSBOXH or
            item_event == PLAYER_CCFSBOXI
     then
        eleven = 1
    end
    if eleven == 1 then
        count = 6
    end

    if count == 1 then
        energy = SetItemOneAttr(item_type, item_lv, item_event, quality)
    elseif count == 2 then
        energy = SetItemTwoAttr(item_type, item_lv, item_event, quality)
    elseif count == 3 then
        energy = SetItemThreeAttr(item_type, item_lv, item_event, quality)
    elseif count == 4 then
        energy = SetItemFourAttr(item_type, item_lv, item_event, quality)
    elseif count == 5 then
        energy = SetItemFiveAttr(item_type, item_lv, item_event, quality)
    elseif count == 6 then
        if item_type == 20 then
            energy = SetItemOneAttr(item_type, item_lv, item_event, quality)
        elseif item_type == 22 then
            energy = SetItemTwoAttr(item_type, item_lv, item_event, quality)
        elseif item_type == 23 or item_type == 24 then
            energy = SetItemThreeAttr(item_type, item_lv, item_event, quality)
        end
    end
    energy = quality * 100 + energy

    Add_Item_Attr(ITEMATTR_MAXENERGY, energy)
end

function SetItemOneAttr(item_type, item_lv, item_event, quality)
    local firstattr = 1
    local lastattr = 5
    local General = 0
    for i = firstattr, lastattr, 1 do
        General = ItemAttr_Rad[i] + General
    end

    local a = math.random(1, General)

    local b = 0
    local d = 0
    local c = -1
    local star = 0
    for k = firstattr, lastattr, 1 do
        d = ItemAttr_Rad[k] + b

        if a <= d and a > b then
            c = k
        end
        b = d
    end

    if
        item_event == PLAYER_CCFSBOXA or item_event == PLAYER_CCFSBOXB or item_event == PLAYER_CCFSBOXC or
            item_event == PLAYER_CCFSBOXD or
            item_event == PLAYER_CCFSBOXE or
            item_event == PLAYER_CCFSBOXF or
            item_event == PLAYER_CCFSBOXG or
            item_event == PLAYER_CCFSBOXH or
            item_event == PLAYER_CCFSBOXI
     then
        c = 6
        star = 1
    end
    if c == 1 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 2 then
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 3 then
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 4 then
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 5 then
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end
    if c == -1 then
        LG("setitemattr_err", "instantiation 1 attribute parameter sending error, C = -1 ")
        c = 0
    end
    if c == 6 and star == 1 then
        Add_Item_Attr(ITEMATTR_VAL_MXSP, quality * 10)
    end
    return c
end

function SetItemTwoAttr(item_type, item_lv, item_event, quality)
    local firstattr = 11
    local lastattr = 20
    local General = 0
    for i = firstattr, lastattr, 1 do
        General = ItemAttr_Rad[i] + General
    end
    local a = math.random(1, General)
    local b = 0
    local d = 0
    local c = -1
    local star = 0
    for k = firstattr, lastattr, 1 do
        d = ItemAttr_Rad[k] + b

        if a <= d and a > b then
            c = k
        end
        b = d
    end
    if
        item_event == PLAYER_CCFSBOXA or item_event == PLAYER_CCFSBOXB or item_event == PLAYER_CCFSBOXC or
            item_event == PLAYER_CCFSBOXD or
            item_event == PLAYER_CCFSBOXE or
            item_event == PLAYER_CCFSBOXF or
            item_event == PLAYER_CCFSBOXG or
            item_event == PLAYER_CCFSBOXH or
            item_event == PLAYER_CCFSBOXI
     then
        c = 21
        star = 1
    end
    if c == 11 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 12 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 13 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 14 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 15 then
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 16 then
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 17 then
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 18 then
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 19 then
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 20 then
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end
    if c == 21 and star == 1 then
        Add_Item_Attr(ITEMATTR_VAL_MXHP, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_HREC, quality * 10)
    end
    if c == -1 then
        LG("setitemattr_err", "instantiation 2 attribute parameter sending error, C = -1 ")
        c = 0
    end

    return c
end

function SetItemThreeAttr(item_type, item_lv, item_event, quality)
    local firstattr = 50
    local lastattr = 59
    local General = 0
    for i = firstattr, lastattr, 1 do
        General = ItemAttr_Rad[i] + General
    end
    local a = math.random(1, General)
    local b = 0
    local d = 0
    local c = -1
    local star = 0
    for k = firstattr, lastattr, 1 do
        d = ItemAttr_Rad[k] + b

        if a <= d and a > b then
            c = k
        end
        b = d
    end
    if
        item_event == PLAYER_CCFSBOXA or item_event == PLAYER_CCFSBOXB or item_event == PLAYER_CCFSBOXC or
            item_event == PLAYER_CCFSBOXD or
            item_event == PLAYER_CCFSBOXE or
            item_event == PLAYER_CCFSBOXF or
            item_event == PLAYER_CCFSBOXG or
            item_event == PLAYER_CCFSBOXH or
            item_event == PLAYER_CCFSBOXI
     then
        if item_type == 23 then
            c = 60
        elseif item_type == 24 then
            c = 61
        end
        star = 1
    end
    if c == 50 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 51 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 52 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 53 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 54 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 55 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 56 then
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 57 then
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 58 then
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 59 then
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end
    if c == 60 then
        Add_Item_Attr(ITEMATTR_VAL_MXATK, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_MNATK, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_CRT, quality * 10)
    end
    if c == 61 then
        Add_Item_Attr(ITEMATTR_VAL_MSPD, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_MXSP, quality * 10)
        Add_Item_Attr(ITEMATTR_VAL_SREC, quality * 10)
    end
    if c == -1 then
        LG("setitemattr_err", "instantiation 3 attribute parameter sending error, C = -1 ")
        c = 0
    end

    return c
end

function SetItemFourAttr(item_type, item_lv, item_event, quality)
    local firstattr = 90
    local lastattr = 94
    local General = 0
    for i = firstattr, lastattr, 1 do
        General = ItemAttr_Rad[i] + General
    end
    local a = math.random(1, General)
    local b = 0
    local d = 0
    local c = -1
    for k = firstattr, lastattr, 1 do
        d = ItemAttr_Rad[k] + b

        if a <= d and a > b then
            c = k
        end
        b = d
    end

    if c == 90 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 91 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 92 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 93 then
        Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == 94 then
        Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
        Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))
    end

    if c == -1 then
        LG("setitemattr_err", "instantiation 4 attribute parameter sending error, C = -1 ")
        c = 0
    end

    return c
end

function SetItemFiveAttr(item_type, item_lv, item_event, quality)
    local c = 0
    Add_Item_Attr(ITEMATTR_VAL_STR, math.max(10, SetItemQua(item_event) * 10))
    Add_Item_Attr(ITEMATTR_VAL_DEX, math.max(10, SetItemQua(item_event) * 10))
    Add_Item_Attr(ITEMATTR_VAL_CON, math.max(10, SetItemQua(item_event) * 10))
    Add_Item_Attr(ITEMATTR_VAL_AGI, math.max(10, SetItemQua(item_event) * 10))
    Add_Item_Attr(ITEMATTR_VAL_STA, math.max(10, SetItemQua(item_event) * 10))

    return c
end

function MentorSystem(Player, Level)
    if Level == 41 then
        AddMoney(Player, 0, 200000)
    end
    if Level < 45 then
        GiveItemX(Player, 0, 1128, 1, 4)
    end
    if MentorSys[Level] ~= nil then
        if MentorSys[Level].Reputation.Disciple ~= 0 then
            AddCreditX(Player, MentorSys[Level].Reputation.Disciple)
        end
        if MentorSys[Level].Reputation.Mentor ~= 0 then
            AddMasterCredit(Player, MentorSys[Level].Reputation.Mentor)
        end
        if MentorSys[Level].Message ~= nil then
            SystemNotice(Player, MentorSys[Level].Message)
        end
    end
end

function IsMentorInParty(Player, Id)
	local Team = {true, true, true, true, true}
	Team[1] = Player
	Team[2] = GetTeamCha(Player, 0)  
	Team[3] = GetTeamCha(Player, 1)   
	Team[4] = GetTeamCha(Player, 2)    
	Team[5] = GetTeamCha(Player, 3)
	for i = 1, #Team do
		if Team[i] == GetRoleByID(Id) then
			return true
		end
	end
	return false
end

function Shengji_Shuxingchengzhang(Player)
	local Level = Lv(Player)
	BsAttrUpgrade(Player)
	AttrRecheck(Player)
	HP = Mxhp_final(Player)
	SP = Mxsp_final(Player)
	SetCharaAttr(HP, Player, ATTR_HP)
	SetCharaAttr(SP, Player, ATTR_SP)
	if HasMaster(Player) == LUA_TRUE then
		MentorSystem(Player, Level)
	end
end

function CreatChaSkill(Player)
end

function BornCheater(Player)
    if Lv(Player) == 1 then
        local Name = GetChaDefaultName(Player)
        local PHY = Resist_final(Player)
        local STR = Str_final(Player)
        local DEX = Dex_final(Player)
        local AGI = Agi_final(Player)
        local CON = Con_final(Player)
        local SPR = Sta_final(Player)
        if STR > 5 or DEX > 5 or AGI > 5 or CON > 5 or SPR > 5 or PHY > 9 then
            PlayerReset(Player)
            BanActRole(Player)
            KickCha(Player)
            LG("Player Cheating", "Player [" .. Name .. "] detected trying to cheat when creating a character!")
            print("Player Cheating", "Player [" .. Name .. "] detected trying to cheat when creating a character!")
        end
    end
end

function FaceBugDetection(Player)
    local Slot = GetChaItem(Player, 1, 1)
    if (Slot == nil) then
        return
    end
    local Attr = {}
    Attr[1] = GetItemAttr(Slot, ITEMATTR_VAL_STR)
    Attr[2] = GetItemAttr(Slot, ITEMATTR_VAL_AGI)
    Attr[3] = GetItemAttr(Slot, ITEMATTR_VAL_DEX)
    Attr[4] = GetItemAttr(Slot, ITEMATTR_VAL_CON)
    Attr[5] = GetItemAttr(Slot, ITEMATTR_VAL_STA)
    Attr[6] = GetItemAttr(Slot, ITEMATTR_VAL_LUK)
    Attr[7] = GetItemAttr(Slot, ITEMATTR_VAL_ASPD)
    Attr[8] = GetItemAttr(Slot, ITEMATTR_VAL_ADIS)
    Attr[9] = GetItemAttr(Slot, ITEMATTR_VAL_MNATK)
    Attr[10] = GetItemAttr(Slot, ITEMATTR_VAL_MXATK)
    Attr[11] = GetItemAttr(Slot, ITEMATTR_VAL_DEF)
    Attr[12] = GetItemAttr(Slot, ITEMATTR_VAL_MXHP)
    Attr[13] = GetItemAttr(Slot, ITEMATTR_VAL_MXSP)
    Attr[14] = GetItemAttr(Slot, ITEMATTR_VAL_FLEE)
    Attr[15] = GetItemAttr(Slot, ITEMATTR_VAL_HIT)
    Attr[16] = GetItemAttr(Slot, ITEMATTR_VAL_CRT)
    Attr[17] = GetItemAttr(Slot, ITEMATTR_VAL_MF)
    Attr[18] = GetItemAttr(Slot, ITEMATTR_VAL_HREC)
    Attr[19] = GetItemAttr(Slot, ITEMATTR_VAL_SREC)
    Attr[20] = GetItemAttr(Slot, ITEMATTR_VAL_MSPD)
    Attr[21] = GetItemAttr(Slot, ITEMATTR_VAL_COL)
    Attr[22] = GetItemAttr(Slot, ITEMATTR_VAL_PDEF)
    local Error = 0
    for num = 1, 22 do
        if Attr[num] > 0 then
            Error = Error + 1
        end
    end
    if Error > 0 then
        local ChaID = GetCharID(Player)
        local Name = GetChaDefaultName(Player)
        LG("Face Bug", "Player [" .. Name .. "] detected trying to cheat when creating a character!")
        BanActRole(Player)
        KickCha(Player)
    end
end

function HairBugDetection(Player)
    local Slot = GetChaItem(Player, 1, 0)
    if (Slot == nil) then
        return
    end
    local ItemType = GetItemType(Slot)
    local Attr = {}
    Attr[1] = GetItemAttr(Slot, ITEMATTR_VAL_STR)
    Attr[2] = GetItemAttr(Slot, ITEMATTR_VAL_AGI)
    Attr[3] = GetItemAttr(Slot, ITEMATTR_VAL_DEX)
    Attr[4] = GetItemAttr(Slot, ITEMATTR_VAL_CON)
    Attr[5] = GetItemAttr(Slot, ITEMATTR_VAL_STA)
    Attr[6] = GetItemAttr(Slot, ITEMATTR_VAL_LUK)
    Attr[7] = GetItemAttr(Slot, ITEMATTR_VAL_ASPD)
    Attr[8] = GetItemAttr(Slot, ITEMATTR_VAL_ADIS)
    Attr[9] = GetItemAttr(Slot, ITEMATTR_VAL_MNATK)
    Attr[10] = GetItemAttr(Slot, ITEMATTR_VAL_MXATK)
    Attr[11] = GetItemAttr(Slot, ITEMATTR_VAL_DEF)
    Attr[12] = GetItemAttr(Slot, ITEMATTR_VAL_MXHP)
    Attr[13] = GetItemAttr(Slot, ITEMATTR_VAL_MXSP)
    Attr[14] = GetItemAttr(Slot, ITEMATTR_VAL_FLEE)
    Attr[15] = GetItemAttr(Slot, ITEMATTR_VAL_HIT)
    Attr[16] = GetItemAttr(Slot, ITEMATTR_VAL_CRT)
    Attr[17] = GetItemAttr(Slot, ITEMATTR_VAL_MF)
    Attr[18] = GetItemAttr(Slot, ITEMATTR_VAL_HREC)
    Attr[19] = GetItemAttr(Slot, ITEMATTR_VAL_SREC)
    Attr[20] = GetItemAttr(Slot, ITEMATTR_VAL_MSPD)
    Attr[21] = GetItemAttr(Slot, ITEMATTR_VAL_COL)
    Attr[22] = GetItemAttr(Slot, ITEMATTR_VAL_PDEF)
    local Error = 0
    for num = 1, 22 do
        if Attr[num] ~= nil then
            if Attr[num] > 0 then
                if ItemType == 28 then
                    Error = Error + 1
                end
            end
        end
    end
    if Error > 0 then
        local Name = GetChaDefaultName(Player)
        LG("Hair Bug", "Player [" .. Name .. "] detected trying to cheat when creating a character!")
        BanActRole(Player)
        KickCha(Player)
    end
end

function CreatCha(Player)
    local AP = Attr_ap(Player) + 4
    SetCharaAttr(AP, Player, ATTR_AP)
    AttrRecheck(Player)
    HP = GetChaAttr(Player, ATTR_MXHP)
    SP = GetChaAttr(Player, ATTR_MXSP)
    SetCharaAttr(HP, Player, ATTR_HP)
    SetCharaAttr(SP, Player, ATTR_SP)
    BornCheater(Player)
    FaceBugDetection(Player)
    HairBugDetection(Player)
end

function AttrRecheck(Player)
    local k = ChaIsBoat(Player)
    if k == 1 then
        local mPlayer = GetMainCha(Player)
        ShipAttrRecheck(mPlayer, Player)
        return
    end
    BsAttrSet(Player)
    ExAttrCheck(Player)
    ExAttrSet(Player)
end

function BsAttrUpgrade(Player)
    local Class = GetChaAttr(Player, ATTR_JOB)
    if CheckJobLegal(Class) == 0 then
        return
    end
    local Point_A = 0
    local Point_S = 0
    local STR = BSStr(Player) + 0
    SetCharaAttr(STR, Player, ATTR_BSTR)
    local ACC = BSDex(Player) + 0
    SetCharaAttr(ACC, Player, ATTR_BDEX)
    local CON = BSCon(Player) + 0
    SetCharaAttr(CON, Player, ATTR_BCON)
    local AGI = BSAgi(Player) + 0
    SetCharaAttr(AGI, Player, ATTR_BAGI)
    local SPR = BSSta(Player) + 0
    SetCharaAttr(SPR, Player, ATTR_BSTA)
    local LUK = BSLuk(Player) + 0
    SetCharaAttr(LUK, Player, ATTR_BLUK)
    local Level = Lv(Player)
    if (math.floor((Level) / 10) - math.floor((Level - 1) / 10)) == 1 then
        Point_A = 5
    else
        Point_A = 1
    end
    if Level >= 60 then
        Point_A = Point_A + 1
    end
    if Level > 9 then
        Point_S = 1
    end
    if Level >= 65 then
        if (math.floor((Level) / 5) - math.floor((Level - 1) / 5)) == 1 then
            Point_S = 2
        else
            Point_S = 1
        end
    end
    Point_S = Attr_tp(Player) + Point_S
    Point_A = Attr_ap(Player) + Point_A
    SetCharaAttr(Point_S, Player, ATTR_TP)
    SetCharaAttr(Point_A, Player, ATTR_AP)
end

function BsAttrSet(Player)
    local STR = Str_final(Player)
    local ACC = Dex_final(Player)
    local AGI = Agi_final(Player)
    local CON = Con_final(Player)
    local SPR = Sta_final(Player)
    local LUK = Luk_final(Player)
    SetCharaAttr(STR, Player, ATTR_STR)
    SetCharaAttr(ACC, Player, ATTR_DEX)
    SetCharaAttr(AGI, Player, ATTR_AGI)
    SetCharaAttr(CON, Player, ATTR_CON)
    SetCharaAttr(SPR, Player, ATTR_STA)
    SetCharaAttr(LUK, Player, ATTR_LUK)
end

function ExAttrCheck(Player)
    local Class = GetChaAttr(Player, ATTR_JOB)
    if CheckJobLegal(Class) == 0 then
        return
    end
    local MxHP = math.floor(Con(Player) * 3 * Mxhp_con_rad1[Class] + Mxhp_con_rad2[Class] * math.pow(math.floor(Con(Player) * 3 / 20), 2) + Lv(Player) * Mxhp_lv_rad[Class] + 40)
    local MxSP = math.floor(Sta(Player) * 3 * Mxsp_sta_rad1[Class] + Mxsp_sta_rad2[Class] * math.pow(math.floor(Sta(Player) * 3 / 20), 2) + Lv(Player) * Mxsp_lv_rad[Class] + 5)
    local mnatk = math.floor(0 + Str(Player) * Mnatk_str_rad1[Class] + Dex(Player) * Mnatk_dex_rad1[Class] + Mnatk_str_rad2[Class] * math.pow(math.floor(Str(Player) * 4 / 20), 2) + Mnatk_dex_rad2[Class] * math.pow(math.floor(Dex(Player) * 4 / 20), 2))
    local mxatk = math.floor(0 + Str(Player) * Mxatk_str_rad1[Class] + Dex(Player) * Mxatk_dex_rad1[Class] + Mxatk_str_rad2[Class] * math.pow(math.floor(Str(Player) * 4 / 20), 2) + Mxatk_dex_rad2[Class] * math.pow(math.floor(Dex(Player) * 4 / 20), 2))
    local def = math.floor(Con(Player) * 5 * Def_con_rad1[Class] + Def_con_rad2[Class] * math.floor(math.pow(Con(Player) * 3 / 20, 2)))
    local hit = math.floor(Dex(Player) * Hit_dex_rad1[Class]) + Lv(Player) * 2 + 5
    local flee = math.floor(Agi(Player) * Flee_agi_rad1[Class]) + Lv(Player) * 2 + 5
    local mf = 100 + math.floor(Luk(Player) * 3 * Mf_luk_rad[Class])
    local crt = 11 + math.floor(Luk(Player) * 3 * Crt_luk_rad[Class])
    local HP_REC = math.max(math.max(2 * MxHP * Hrec_bsmxhp_rad[Class] + Con(Player) * 3 * Hrec_con_rad[Class], 1), 0)
    local SP_REC = math.max((MxSP * Srec_bsmxsp_rad[Class] + Sta(Player) * 3 * Srec_sta_rad[Class]) / 2, 1)
    local aspd = math.floor(100000 / (math.min(math.floor(65 + Agi(Player) * Aspd_agi_rad[Class]), 300)))
    SetCharaAttr(MxHP, Player, ATTR_BMXHP)
    if MxHP <= 0 then
        LG("ChaAttr_err", "Character", GetChaName(Player), "Base Max HP error")
        LG("ChaAttr_err", "mxhp = ", MxHP, "    con = ", Con(Player), "	lv = ", Lv(Player))
    end
    SetCharaAttr(MxSP, Player, ATTR_BMXSP)
    SetCharaAttr(mnatk, Player, ATTR_BMNATK)
    SetCharaAttr(mxatk, Player, ATTR_BMXATK)
    SetCharaAttr(def, Player, ATTR_BDEF)
    SetCharaAttr(hit, Player, ATTR_BHIT)
    SetCharaAttr(flee, Player, ATTR_BFLEE)
    SetCharaAttr(mf, Player, ATTR_BMF)
    SetCharaAttr(crt, Player, ATTR_BCRT)
    SetCharaAttr(HP_REC, Player, ATTR_BHREC)
    if HP_REC <= 0 then
        LG("ChaAttr_err", "Character", GetChaName(Player), "Base HP recovery rate error")
        LG("ChaAttr_err", "hrec = ", HP_REC, "    mxhp = ", MxHP, "	con = ", Con(Player))
    end
    SetCharaAttr(SP_REC, Player, ATTR_BSREC)
    if SP_REC <= 0 then
        LG("ChaAttr_err", "Character", GetChaName(Player), "Base SP recovery rate error")
        LG("ChaAttr_err", "srec = ", SP_REC, "    mxsp = ", mxsp, "	sta = ", Sta(role))
    end
    SetCharaAttr(aspd, Player, ATTR_BASPD)
end
function ExAttrSet(Player)
	local mxhp_final = Mxhp_final(Player)
	local mxsp_final = Mxsp_final(Player)
	local mnatk_final = Mnatk_final(Player)
	local mxatk_final = Mxatk_final(Player)
	local def_final = Def_final(Player)
	local resist_final = Resist_final(Player)
	local hit_final = Hit_final(Player)
	local flee_final = Flee_final(Player)
	local mf_final = Mf_final(Player)
	local crt_final = Crt_final(Player)
	local hrec_final = Hrec_final(Player)
	local srec_final = Srec_final(Player)
	local aspd_final = math.floor(100000/(Aspd_final(Player)))
	local adis_final = Adis_final(Player)
	local mspd_final = Mspd_final(Player)

	SetCharaAttr(mxhp_final, Player, ATTR_MXHP)
	SetCharaAttr(mxsp_final, Player, ATTR_MXSP)
	SetCharaAttr(mnatk_final, Player, ATTR_MNATK)
	SetCharaAttr(mxatk_final, Player, ATTR_MXATK)
	SetCharaAttr(def_final, Player, ATTR_DEF)
	SetCharaAttr(resist_final, Player, ATTR_PDEF) 
	SetCharaAttr(hit_final, Player, ATTR_HIT)
	SetCharaAttr(flee_final, Player, ATTR_FLEE)
	SetCharaAttr(mf_final, Player, ATTR_MF)
	SetCharaAttr(crt_final, Player, ATTR_CRT)
	SetCharaAttr(hrec_final, Player, ATTR_HREC)
	SetCharaAttr(srec_final, Player, ATTR_SREC)
	SetCharaAttr(aspd_final, Player, ATTR_ASPD)
	SetCharaAttr(adis_final, Player, ATTR_ADIS)
	SetCharaAttr(mspd_final, Player, ATTR_MSPD)
end
function ShipAttrRecheck(mPlayer, sPlayer)
    Ship_ExAttrCheck(mPlayer, sPlayer)
    Ship_ExAttrSet(mPlayer, sPlayer)
end
function Ship_ExAttrCheck(mPlayer, sPlayer)
    if sPlayer == nil then
        LG("Script Error", "Ship is null.")
        return 0
    end
    if mPlayer == nil then
        LG("Script Error", "Player is null.")
        return
    else
        lv = GetChaAttr(sPlayer, ATTR_LV)
        Class = GetChaAttr(mPlayer, ATTR_JOB)
        SPR = GetChaAttr(mPlayer, ATTR_STA)
        ship_mnatk_final = Boat_plus_MNATk(lv, Ship_Mnatk_final(mPlayer, sPlayer))
        ship_mxatk_final = Boat_plus_MXATk(lv, Ship_Mxatk_final(mPlayer, sPlayer))
        ship_adis_final = Ship_Adis_final(mPlayer, sPlayer)
        ship_cspd_final = Ship_Cspd_final(mPlayer, sPlayer)
        ship_aspd_final = math.floor(100000 / Ship_Aspd_final(mPlayer, sPlayer))
        ship_crange_final = Ship_Crange_final(mPlayer, sPlayer)
        ship_def_final = Boat_plus_def(lv, Ship_Def_final(mPlayer, sPlayer))
        ship_resist_final = Ship_Resist_final(mPlayer, sPlayer)
        ship_mxhp_final = Boat_plus_Mxhp(lv, Ship_Mxhp_final(mPlayer, sPlayer))
        ship_hrec_final = Ship_Hrec_final(mPlayer, sPlayer)
        ship_srec_final = Ship_Srec_final(mPlayer, sPlayer)
        ship_mspd_final = Boat_plus_Mspd(lv, Ship_Mspd_final(mPlayer, sPlayer))
        ship_mxsp_final = Ship_Mxsp_final(mPlayer, sPlayer)
    end
    SetCharaAttr(Class, sPlayer, ATTR_JOB)
    SetCharaAttr(SPR, sPlayer, ATTR_STA)
    SetCharaAttr(ship_mnatk_final, sPlayer, ATTR_MNATK)
    SetCharaAttr(ship_mxatk_final, sPlayer, ATTR_MXATK)
    SetCharaAttr(ship_adis_final, sPlayer, ATTR_ADIS)
    SetCharaAttr(ship_cspd_final, sPlayer, ATTR_BOAT_CSPD)
    SetCharaAttr(ship_aspd_final, sPlayer, ATTR_ASPD)
    SetCharaAttr(ship_crange_final, sPlayer, ATTR_BOAT_CRANGE)
    SetCharaAttr(ship_def_final, sPlayer, ATTR_DEF)
    SetCharaAttr(ship_resist_final, sPlayer, ATTR_PDEF)
    SetCharaAttr(ship_mxhp_final, sPlayer, ATTR_MXHP)
    SetCharaAttr(ship_hrec_final, sPlayer, ATTR_HREC)
    SetCharaAttr(ship_srec_final, sPlayer, ATTR_SREC)
    SetCharaAttr(ship_mspd_final, sPlayer, ATTR_MSPD)
    SetCharaAttr(ship_mxsp_final, sPlayer, ATTR_MXSP)
    SetCharaAttr(1, sPlayer, ATTR_FLEE)
end
function Ship_ExAttrSet(mPlayer, sPlayer)
end
function Lifelv_Up(Player)
    local Point_L = GetChaAttr(Player, ATTR_LIFETP) + 1
    SetCharaAttr(Point_L, Player, ATTR_LIFETP)
end
function Saillv_Up(Player)
end
function Resume(Player)
    local SP_REC = GetChaAttr(Player, ATTR_SREC)
    local SP = GetChaAttr(Player, ATTR_SP)
    local MxSP = GetChaAttr(Player, ATTR_MXSP)
    local HP_REC = GetChaAttr(Player, ATTR_HREC)
    if HP_REC < 0 then
        LG("Script Error", "Function Resume", GetChaName(Player), "Character's HP Recovery rate is less than 0.")
        LG("Script Error", "Function Resume", "Player HREC_STATEC", GetChaAttr(Player, ATTR_STATEC_HREC), "Player HREC_STATEV", GetChaAttr(Player, ATTR_STATEV_HREC))
        LG("Script Error", "Function Resume", "Character's HP Recovery rate is less than 0.")
        return
    end
    local HP = GetChaAttr(Player, ATTR_HP)
    local MxHP = GetChaAttr(Player, ATTR_MXHP)
    if ChaIsBoat(Player) == 1 then
        if HP <= 0 then
            LG("Script Error", "Function Resume", "Character is dead.")
            return
        end
        mPlayer = GetMainCha(Player)
        if SP <= 0 then
            BickerNotice(Player, "No more fuel! The ship is being damaged every moment! Get to the nearest Harbour now!")
            HP_REC = HP_REC - 0.025 * MxHP
            SP_REC = 0
        end
        SP = math.max(0, SP - SP_REC)
        HP = math.min(MxHP, HP + HP_REC)
        local ShipLv = GetChaAttr(Player, ATTR_LV)
        local ShipEXP = GetChaAttr(Player, ATTR_CEXP)
        local cShipEXP = GetBoatCtrlTick(Player)
        if (cShipEXP - math.floor(cShipEXP / 5) * 5) == 4 then
            a = 1
        else
            a = 0
        end
        cShipEXP = cShipEXP + 1
        if cShipEXP >= 500 then
            cShipEXP = 0
        end
        SetBoatCtrlTick(Player, cShipEXP)
        if ShipLv <= 30 and ShipEXP <= 1000 then
            if a == 1 then
                local aShipEXP = math.floor(math.random(1, 3) + math.max(0, (2 - ShipLv / 10)))
                ShipEXP = ShipEXP + aShipEXP
                SystemNotice(Player, "Ship EXP gained: " .. aShipEXP)
                SetCharaAttr(ShipEXP, Player, ATTR_CEXP)
            end
        end
        ChaSP = math.min(Mxsp(mPlayer), Sp(mPlayer) + Srec(mPlayer))
        SetCharaAttr(SP, Player, ATTR_SP)
        SetCharaAttr(HP, Player, ATTR_HP)
        SetCharaAttr(ChaSP, mPlayer, ATTR_SP)
    else
        if HP <= 0 then
            LG("Script Error", "Function Resume", "Character is dead.")
            return
        end
        local FairyRes_HP = 0
        local FairyRes_SP = 0
        if MxHP ~= HP then
            FairyRes_HP = ElfSkill_HpResume(Player)
        end
        if MxSP ~= SP then
            FairyRes_SP = ElfSkill_SpResume(Player)
        end
        HP_REC = HP_REC + FairyRes_HP
        SP_REC = SP_REC + FairyRes_SP
        if GetChaMapName(Player) == "Rush" then
            HP_REC, SP_REC = Rush.RecoveryStat(Player, HP_REC, SP_REC)
        end
        SP = math.min(MxSP, SP + SP_REC)
        HP = math.min(MxHP, HP + HP_REC)
        SetCharaAttr(HP, Player, ATTR_HP)
        SetCharaAttr(SP, Player, ATTR_SP)
    end
end
