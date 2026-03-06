--                ID   angle distance   height


function SetClientAttr(IDs,Camera)
	for i,v in pairs(IDs) do
		CHA_SetClientAttr(v,   unpack(Camera))
	end
end

--Default camera profile, incase mobID profile is not defined.
CHA_SetClientAttr(0,   30,   3,   0.6) 

--Camera profile for lance/carsise/phyliss/ami
CHA_SetClientAttr(1,   30,   1.5,   0.5)
CHA_SetClientAttr(2,   30,   1.8,   0.4)
CHA_SetClientAttr(3,   30,   1.5,   0.3)
CHA_SetClientAttr(4,   30,   1.4,   0.5)

--Black Dragon camera config. (hardin etc)
local BDID = {789,987,952}
local BDCamera = { 357,   15,  3}
SetClientAttr(BDID,BDCamera)

--abyss supreme kara
local karaID = {262,857,988,1001,1027,1182}
local karaCamera = {15,   6,  3.6}
SetClientAttr(karaID,karaCamera)

--Baby Black Dragons camera config. 
local BBDID = {791,793,794,1136,1139}
local BBDCamera = { 357,   6,  1.1}
SetClientAttr(BBDID,BBDCamera)

--dwb1 camera config. 
local dwb1id = {289,679,963,1183}
local dwb1cam = { 30,   4,  1.4}
SetClientAttr(dwb1id,dwb1cam)

--dwb2 camera config. 
local dwb2id = {289,678,976,1181}
local dwb2cam = { 14,   7.5,  2.4}
SetClientAttr(dwb2id,dwb2cam)

--little squirt, and variants.
local littlesquirt = {69,70,230,231,232,233,234,235,287,288,841,991,1135}
local littlesquirtcam = { 30,   2.1,   0.3}
SetClientAttr(littlesquirt,littlesquirtcam)

--piglets and variants
local piglet = {237, 239,296}
local pigletcam = { 30,   2.1,   1.2}
SetClientAttr(piglet,pigletcam)

--yeti mobs
local yeti = {98}
local yeticam = { 14,   7.5,  0.8}
SetClientAttr(yeti,yeticam)

--yeti mobs
local yeti2 = {194,516,544,725}
local yeticam2 = { 14,   7.5,  1.3}
SetClientAttr(yeti2,yeticam2)

--yeti mobs
local yeti3 = {195,517,545,726}
local yeticam3 = { 14,   7.5,  1.3}
SetClientAttr(yeti3,yeticam3)

--treant (small)
local tree1 = {107,547}
local treecam1 = { 14,   7.5,  2.6}
SetClientAttr(tree1,treecam1)

--treant (big)
local tree2 = {205,511}
local treecam2 = { 14,   7.5,  2}
SetClientAttr(tree2,treecam2)

--wolf
local wolf = {100,101,136,137,500,540,851,}
local wolfcam = {19,   3,   0.6}
SetClientAttr(wolf,wolfcam)


