print("-- [Loading] Char Born Script")

function PCBorn()
    InitTrigger()
    TriggerCondition(1, IsSpawnPos, "Argent City")
    TriggerAction(1, ObligeAcceptMission, 1)
    TriggerCondition(2, IsSpawnPos, "Shaitan City")
    TriggerAction(2, ObligeAcceptMission, 2)
    TriggerCondition(3, IsSpawnPos, "Icicle Castle")
    TriggerAction(3, ObligeAcceptMission, 3)
    local triggerlist = GetMultiTrigger()

    InitTrigger()
    TriggerAction(1, HelpInfo, MIS_HELP_DESP, 'Welcome to Pirate King Online. _I will introduce you to the basic operation function. A single click on the "Left Mouse Button" to move, single click on the "Left Mouse Button" outside of town to attack monsters, holding on to "Left Mouse Button" to continue walking. During a dialogue with any NPC, you can use the "Left Mouse Button" to confirm. Single click on the "Right Mouse Button" on a player will open a selection table, you can "Party, Add Friend, Trade, PM" etc. Hold on to the "Right Mouse Button" to rotate camera angle. Scrolling of "Mouse Wheel" will enable zooming in or out and double click on "Right Mouse Button" will restore default camera angle._')
    TriggerAction(1, AddTrigger, 60000, TE_LEVELUP, 2, 1)
    TriggerAction(1, AddTrigger, 60001, TE_LEVELUP, 5, 1)
    TriggerAction(1, AddTrigger, 60002, TE_LEVELUP, 9, 1)
    TriggerAction(1, AddTrigger, 60003, TE_LEVELUP, 10, 1)

    TriggerAction(1, MultiTrigger, triggerlist, 3)
    TriggerAction(1, SaveMissionData)

    RegTrigger(88888, 1)

    InitTrigger()
    TriggerAction(1, HelpInfo, MIS_HELP_DESP, "Congratulations! Now I will teach you how to use the Radar. _To use it, click on the arrow button found in the mini-map on the right of your screen. _It will open up your radar interface. _Select the map you wish to go to and enter the X and Y coordinates. _An arrow will appear and point towards the  _direction of your destination.")
    RegTrigger(60000, 1)

    InitTrigger()
    TriggerAction(1, HelpInfo, MIS_HELP_DESP, 'Real battle will start soon but first, familiarize with the basic shortcut: _Press the "Insert" key to sit or stand, as sitting down will increase HP/SP recovery rate; _Items dropped by monsters killed by you will have a 30 secs countdown; _Press "Ctrl+A" key to pick all items in your surrounding; _Click "Alt+Left Click" on player to follow _them.')
    RegTrigger(60001, 1)

    InitTrigger()
    TriggerAction(1, HelpInfo, MIS_HELP_DESP, " I believe that you have understand the basic of the battle system. Now it is time for you to select a class. _Newbie Guide will recommend you to the respective trainer. _Go to the Newbie Guide now and select the class you wanted. _You will need to undergo some test to complete your advancement. _If you succeeded, you can advance at Lv 10.")
    RegTrigger(60002, 1)

    InitTrigger()
    TriggerAction(1, HelpInfo, MIS_HELP_DESP,"Congratulations! You are now level 10. You have received your first skill point. Each level from now on will earn you one skill point. You can start to learn skills after you completed your basic class advancement. Different skill books are sold by different Grocer in any city. Good luck!")
    RegTrigger(60003, 1)
end
PCBorn()
