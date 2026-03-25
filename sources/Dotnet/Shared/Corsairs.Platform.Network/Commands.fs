namespace Corsairs.Platform.Network.Protocol

/// Константы команд сетевого протокола.
/// Перенесены из Packet.h (C++).
/// Формат именования: CMD_{отправитель}{получатель}_{имя}
///   C = Client, M = GameServer, T = GateServer, P = GroupServer, A = AccountServer, O = Monitor, S = Server
[<RequireQualifiedAccess>]
module Commands =

    // ── Базовые смещения (каждый диапазон — 500 команд) ──
    [<Literal>] let CMD_INVALID         = 0us

    [<Literal>] let CMD_CM_BASE         = 0us       // Client       → GameServer
    [<Literal>] let CMD_MC_BASE         = 500us     // GameServer   → Client
    [<Literal>] let CMD_TM_BASE         = 1000us    // GateServer   → GameServer
    [<Literal>] let CMD_MT_BASE         = 1500us    // GameServer   → GateServer
    [<Literal>] let CMD_TP_BASE         = 2000us    // GateServer   → GroupServer
    [<Literal>] let CMD_PT_BASE         = 2500us    // GroupServer  → GateServer
    [<Literal>] let CMD_PA_BASE         = 3000us    // GroupServer  → AccountServer
    [<Literal>] let CMD_AP_BASE         = 3500us    // AccountServer→ GroupServer
    [<Literal>] let CMD_MM_BASE         = 4000us    // GameServer   → GameServer
    [<Literal>] let CMD_PM_BASE         = 4500us    // GroupServer  → GameServer
    [<Literal>] let CMD_PC_BASE         = 5000us    // GroupServer  → Client
    [<Literal>] let CMD_MP_BASE         = 5500us    // GameServer   → GroupServer
    [<Literal>] let CMD_CP_BASE         = 6000us    // Client       → GroupServer
    [<Literal>] let CMD_OS_BASE         = 6500us    // Monitor      → Server
    [<Literal>] let CMD_SO_BASE         = 7000us    // Server       → Monitor
    [<Literal>] let CMD_TC_BASE         = 7500us    // GateServer   → Client

    // ── Суб-базы ──
    [<Literal>] let CMD_CM_ROLEBASE     = 300us     // CMD_CM_BASE + 300
    [<Literal>] let CMD_CM_GULDBASE     = 400us     // CMD_CM_BASE + 400
    [<Literal>] let CMD_CM_CHARBASE     = 430us     // CMD_CM_BASE + 430

    [<Literal>] let CMD_MC_ROLEBASE     = 800us     // CMD_MC_BASE + 300
    [<Literal>] let CMD_MC_GULDBASE     = 900us     // CMD_MC_BASE + 400
    [<Literal>] let CMD_MC_CHARBASE     = 930us     // CMD_MC_BASE + 430

    // ════════════════════════════════════════════════════════════
    //  Client → GameServer  (CM)
    // ════════════════════════════════════════════════════════════
    [<Literal>] let CMD_CM_SAY              = 1us   // Сообщение в чат (локальный)
    [<Literal>] let CMD_CM_BEGINACTION      = 6us   // Начало действия (атака, скилл)
    [<Literal>] let CMD_CM_ENDACTION        = 7us   // Конец действия
    [<Literal>] let CMD_CM_SYNATTR          = 8us   // Запрос синхронизации атрибутов
    [<Literal>] let CMD_CM_SYNSKILLBAG      = 9us   // Запрос синхронизации скиллов
    [<Literal>] let CMD_CM_DIE_RETURN       = 10us  // Воскрешение после смерти
    [<Literal>] let CMD_CM_SKILLUPGRADE     = 11us  // Прокачка скилла
    [<Literal>] let CMD_CM_PING             = 15us  // Пинг к GameServer
    [<Literal>] let CMD_CM_REFRESH_DATA     = 16us  // Запрос обновления данных
    [<Literal>] let CMD_CM_CHECK_PING       = 17us  // Проверка пинга (античит)
    [<Literal>] let CMD_CM_MAP_MASK         = 18us  // Запрос маски карты (туман войны)
    [<Literal>] let CMD_CM_UPDATEHAIR       = 20us  // Смена причёски
    [<Literal>] let CMD_CM_TEAM_FIGHT_ASK   = 21us  // Запрос PvP-дуэли
    [<Literal>] let CMD_CM_TEAM_FIGHT_ASR   = 22us  // Ответ на PvP-дуэль
    [<Literal>] let CMD_CM_ITEM_REPAIR_ASK  = 23us  // Запрос ремонта предмета
    [<Literal>] let CMD_CM_ITEM_REPAIR_ASR  = 24us  // Подтверждение ремонта
    [<Literal>] let CMD_CM_ITEM_FORGE_ASK   = 25us  // Запрос ковки предмета
    [<Literal>] let CMD_CM_ITEM_FORGE_ASR   = 26us  // Подтверждение ковки
    [<Literal>] let CMD_CM_TIGER_START      = 27us  // Начало мини-игры "тигр" (слот-машина)
    [<Literal>] let CMD_CM_TIGER_STOP       = 28us  // Остановка слот-машины
    [<Literal>] let CMD_CM_ITEM_FORGE_CANACTION = 29us // Проверка возможности ковки
    [<Literal>] let CMD_CM_KITBAG_LOCK      = 31us  // Блокировка ячейки инвентаря
    [<Literal>] let CMD_CM_KITBAG_UNLOCK    = 32us  // Разблокировка ячейки инвентаря
    [<Literal>] let CMD_CM_KITBAG_CHECK     = 33us  // Проверка состояния инвентаря
    [<Literal>] let CMD_CM_KITBAG_AUTOLOCK  = 34us  // Автоблокировка инвентаря
    [<Literal>] let CMD_CM_KITBAGTEMP_SYNC  = 35us  // Синхронизация временного инвентаря
    [<Literal>] let CMD_CM_KITBAGTEMPlocks  = 36us  // Блокировки временного инвентаря
    [<Literal>] let CMD_CM_STORE_OPEN_ASK   = 41us  // Открытие магазина (ItemMall)
    [<Literal>] let CMD_CM_STORE_LIST_ASK   = 42us  // Запрос списка товаров
    [<Literal>] let CMD_CM_STORE_BUY_ASK    = 43us  // Покупка в магазине
    [<Literal>] let CMD_CM_STORE_CHANGE_ASK = 44us  // Обмен в магазине
    [<Literal>] let CMD_CM_STORE_QUERY      = 45us  // Запрос информации о товаре
    [<Literal>] let CMD_CM_STORE_VIP        = 46us  // VIP-магазин
    [<Literal>] let CMD_CM_STORE_AFFICHE    = 47us  // Объявление магазина
    [<Literal>] let CMD_CM_STORE_CLOSE      = 48us  // Закрытие магазина
    [<Literal>] let CMD_CM_BLACKMARKET_EXCHANGE_REQ = 51us // Обмен на чёрном рынке
    [<Literal>] let CMD_CM_CHEAT_CHECK      = 52us  // Античит-проверка
    [<Literal>] let CMD_CM_VOLUNTER_LIST    = 61us  // Список волонтёров
    [<Literal>] let CMD_CM_VOLUNTER_ADD     = 62us  // Добавить волонтёра
    [<Literal>] let CMD_CM_VOLUNTER_DEL     = 63us  // Удалить волонтёра
    [<Literal>] let CMD_CM_VOLUNTER_SEL     = 64us  // Выбрать волонтёра
    [<Literal>] let CMD_CM_VOLUNTER_OPEN    = 65us  // Открыть панель волонтёров
    [<Literal>] let CMD_CM_VOLUNTER_ASR     = 66us  // Ответ на приглашение волонтёра
    [<Literal>] let CMD_CM_MASTER_INVITE    = 71us  // Приглашение в ученики (от мастера)
    [<Literal>] let CMD_CM_MASTER_ASR       = 72us  // Ответ на приглашение мастера
    [<Literal>] let CMD_CM_MASTER_DEL       = 73us  // Удалить мастера
    [<Literal>] let CMD_CM_PRENTICE_DEL     = 74us  // Удалить ученика
    [<Literal>] let CMD_CM_PRENTICE_INVITE  = 75us  // Приглашение от ученика
    [<Literal>] let CMD_CM_PRENTICE_ASR     = 76us  // Ответ от ученика
    [<Literal>] let CMD_CM_LIFESKILL_ASR    = 80us  // Ответ по жизненному навыку
    [<Literal>] let CMD_CM_LIFESKILL_ASK    = 81us  // Запрос жизненного навыка (крафт)
    [<Literal>] let CMD_CM_BIDUP            = 86us  // Ставка на аукционе
    [<Literal>] let CMD_CM_STALLSEARCH      = 87us  // Поиск по лавкам игроков
    [<Literal>] let CMD_CM_SAY2CAMP         = 91us  // Чат лагеря (фракции)
    [<Literal>] let CMD_CM_GM_SEND          = 92us  // Отправка GM-почты
    [<Literal>] let CMD_CM_GM_RECV          = 93us  // Получение GM-почты
    [<Literal>] let CMD_CM_PK_CTRL          = 94us  // Переключение PK-режима
    [<Literal>] let CMD_CM_ITEM_LOTTERY_ASK = 95us  // Запрос лотереи
    [<Literal>] let CMD_CM_ITEM_LOTTERY_ASR = 96us  // Ответ лотереи
    [<Literal>] let CMD_CM_RANK             = 97us  // Запрос рейтинга
    [<Literal>] let CMD_CM_CAPTAIN_CONFIRM_ASR = 97us // Подтверждение капитана (дубликат CMD_CM_RANK)
    [<Literal>] let CMD_CM_ITEM_AMPHITHEATER_ASK = 98us // Запрос амфитеатра (PvP-арена)
    [<Literal>] let CMD_CM_ITEM_LOCK_ASK    = 99us  // Запрос блокировки предмета
    [<Literal>] let CMD_CM_ITEM_UNLOCK_ASK  = 100us // Запрос разблокировки предмета
    [<Literal>] let CMD_CM_GAME_REQUEST_PIN = 101us // Запрос PIN-кода
    [<Literal>] let CMD_CM_DailyBuffRequest = 110us // Запрос ежедневного баффа

    // CM — гильдии (GULDBASE = 400)
    [<Literal>] let CMD_CM_GUILD_PUTNAME        = 401us // Задать имя гильдии
    [<Literal>] let CMD_CM_GUILD_TRYFOR         = 402us // Заявка на вступление
    [<Literal>] let CMD_CM_GUILD_TRYFORCFM      = 403us // Подтверждение заявки
    [<Literal>] let CMD_CM_GUILD_LISTTRYPLAYER  = 404us // Список заявок в гильдию
    [<Literal>] let CMD_CM_GUILD_APPROVE        = 405us // Одобрить заявку
    [<Literal>] let CMD_CM_GUILD_REJECT         = 406us // Отклонить заявку
    [<Literal>] let CMD_CM_GUILD_KICK           = 407us // Исключить из гильдии
    [<Literal>] let CMD_CM_GUILD_LEAVE          = 408us // Покинуть гильдию
    [<Literal>] let CMD_CM_GUILD_DISBAND        = 409us // Распустить гильдию
    [<Literal>] let CMD_CM_GUILD_MOTTO          = 410us // Изменить девиз гильдии
    [<Literal>] let CMD_CM_GUILD_CHALLENGE      = 411us // Вызов на войну гильдий
    [<Literal>] let CMD_CM_GUILD_LEIZHU         = 412us // Назначить лидера (Lei Zhu)
    [<Literal>] let CMD_CM_GUILD_PERM           = 413us // Управление правами участников

    // CM — персонаж/логин (CHARBASE = 430)
    [<Literal>] let CMD_CM_LOGIN        = 431us // Авторизация (SyncCall → GroupServer)
    [<Literal>] let CMD_CM_LOGOUT       = 432us // Выход из игры
    [<Literal>] let CMD_CM_BGNPLAY      = 433us // Начать игру (выбор персонажа → карта)
    [<Literal>] let CMD_CM_ENDPLAY      = 434us // Вернуться к выбору персонажа
    [<Literal>] let CMD_CM_NEWCHA       = 435us // Создать персонажа
    [<Literal>] let CMD_CM_DELCHA       = 436us // Удалить персонажа
    [<Literal>] let CMD_CM_CANCELEXIT   = 437us // Отменить выход из игры
    [<Literal>] let CMD_CM_REGISTER     = 438us // Регистрация аккаунта

    // CM — ролевая система (ROLEBASE = 300)
    [<Literal>] let CMD_CM_REQUESTNPC       = 301us // Запрос диалога с NPC
    [<Literal>] let CMD_CM_REQUESTTALK      = 301us // = REQUESTNPC (алиас)
    [<Literal>] let CMD_CM_TALKPAGE         = 302us // Выбор страницы диалога
    [<Literal>] let CMD_CM_FUNCITEM         = 303us // Использование функц. предмета
    [<Literal>] let CMD_CM_REQUESTTRADE     = 308us // Запрос торговли с NPC
    [<Literal>] let CMD_CM_TRADEITEM        = 309us // Выбор предмета для торговли
    [<Literal>] let CMD_CM_REQUESTAGENCY    = 310us // Запрос агентства (авто-торговля)
    [<Literal>] let CMD_CM_CHARTRADE_REQUEST        = 312us // Запрос обмена между игроками
    [<Literal>] let CMD_CM_CHARTRADE_ACCEPT         = 313us // Принять обмен
    [<Literal>] let CMD_CM_CHARTRADE_REJECT         = 314us // Отклонить обмен
    [<Literal>] let CMD_CM_CHARTRADE_CANCEL         = 315us // Отменить обмен
    [<Literal>] let CMD_CM_CHARTRADE_ITEM           = 316us // Положить предмет в обмен
    [<Literal>] let CMD_CM_CHARTRADE_VALIDATEDATA   = 317us // Валидация данных обмена
    [<Literal>] let CMD_CM_CHARTRADE_VALIDATE       = 318us // Подтвердить обмен (lock)
    [<Literal>] let CMD_CM_CHARTRADE_MONEY          = 319us // Установить золото в обмене
    [<Literal>] let CMD_CM_MISSION          = 322us // Взять/сдать квест
    [<Literal>] let CMD_CM_MISSIONLIST      = 323us // Запрос списка квестов
    [<Literal>] let CMD_CM_MISSIONTALK      = 324us // Диалог по квесту
    [<Literal>] let CMD_CM_MISLOG           = 325us // Журнал квестов
    [<Literal>] let CMD_CM_MISLOGINFO       = 326us // Детали записи журнала
    [<Literal>] let CMD_CM_MISLOG_CLEAR     = 327us // Очистка журнала квестов
    [<Literal>] let CMD_CM_STALL_ALLDATA    = 330us // Запрос данных лавки
    [<Literal>] let CMD_CM_STALL_OPEN       = 331us // Открыть лавку
    [<Literal>] let CMD_CM_STALL_BUY        = 332us // Купить из лавки
    [<Literal>] let CMD_CM_STALL_CLOSE      = 333us // Закрыть лавку
    [<Literal>] let CMD_CM_FORGE            = 335us // Ковка/улучшение предмета
    [<Literal>] let CMD_CM_CREATE_BOAT      = 338us // Создание корабля
    [<Literal>] let CMD_CM_UPDATEBOAT_PART  = 339us // Обновить часть корабля
    [<Literal>] let CMD_CM_BOAT_CANCEL      = 340us // Отменить строительство корабля
    [<Literal>] let CMD_CM_BOAT_LUANCH      = 341us // Спустить корабль на воду
    [<Literal>] let CMD_CM_BOAT_BAGSEL      = 342us // Выбор груза корабля
    [<Literal>] let CMD_CM_BOAT_SELECT      = 343us // Выбрать корабль
    [<Literal>] let CMD_CM_BOAT_GETINFO     = 344us // Информация о корабле
    [<Literal>] let CMD_CM_ENTITY_EVENT     = 345us // Событие сущности (мина, ловушка)
    [<Literal>] let CMD_CM_CREATE_PASSWORD2 = 346us // Создать второй пароль
    [<Literal>] let CMD_CM_UPDATE_PASSWORD2 = 347us // Обновить второй пароль
    [<Literal>] let CMD_CM_READBOOK_START   = 348us // Начать чтение книги
    [<Literal>] let CMD_CM_READBOOK_CLOSE   = 349us // Закрыть книгу
    [<Literal>] let CMD_CM_SEND_PRIVATE_KEY     = 355us // AES-ключ клиента (RSA handshake)
    [<Literal>] let CMD_CM_REQUEST_DROP_RATE    = 356us // Запрос текущего дропрейта
    [<Literal>] let CMD_CM_REQUEST_EXP_RATE     = 357us // Запрос текущего экспрейта
    [<Literal>] let CMD_CM_OFFLINE_MODE         = 358us // Включить офлайн-стойло
    [<Literal>] let CMD_CM_GARNER2_REORDER      = 371us // Переупорядочить Garner (склад)
    [<Literal>] let CMD_CM_ANTIINDULGENCE       = 372us // Антизависимость (таймер онлайна)

    // ════════════════════════════════════════════════════════════
    //  GameServer → Client  (MC)
    // ════════════════════════════════════════════════════════════
    [<Literal>] let CMD_MC_SAY              = 501us // Сообщение чата
    [<Literal>] let CMD_MC_MAPCRASH         = 503us // Крэш карты (текст ошибки)
    [<Literal>] let CMD_MC_CHABEGINSEE      = 504us // Персонаж появился в зоне видимости
    [<Literal>] let CMD_MC_CHAENDSEE        = 505us // Персонаж покинул зону видимости
    [<Literal>] let CMD_MC_ITEMBEGINSEE     = 506us // Предмет появился на земле
    [<Literal>] let CMD_MC_ITEMENDSEE       = 507us // Предмет исчез с земли
    [<Literal>] let CMD_MC_NOTIACTION       = 508us // Уведомление о действии (скилл, атака)
    [<Literal>] let CMD_MC_SYNATTR          = 509us // Синхронизация атрибутов
    [<Literal>] let CMD_MC_SYNSKILLBAG      = 510us // Синхронизация сумки скиллов
    [<Literal>] let CMD_MC_SYNASKILLSTATE   = 511us // Синхронизация активных скиллов
    [<Literal>] let CMD_MC_PING             = 515us // Ответ на пинг
    [<Literal>] let CMD_MC_ENTERMAP         = 516us // Подтверждение входа на карту
    [<Literal>] let CMD_MC_SYSINFO          = 517us // Системное сообщение (текст)
    [<Literal>] let CMD_MC_ALARM            = 518us // Оповещение (popup)
    [<Literal>] let CMD_MC_TEAM             = 519us // Обновление команды (пати)
    [<Literal>] let CMD_MC_FAILEDACTION     = 520us // Действие не удалось
    [<Literal>] let CMD_MC_MESSAGE          = 521us // Сообщение от сервера
    [<Literal>] let CMD_MC_ASTATEBEGINSEE   = 522us // Состояние (бафф) появилось
    [<Literal>] let CMD_MC_ASTATEENDSEE     = 523us // Состояние (бафф) исчезло
    [<Literal>] let CMD_MC_TLEADER_ID       = 524us // ID лидера команды
    [<Literal>] let CMD_MC_CHA_EMOTION      = 525us // Эмоция персонажа
    [<Literal>] let CMD_MC_QUERY_CHA        = 526us // Ответ: информация о персонаже
    [<Literal>] let CMD_MC_QUERY_CHAITEM    = 527us // Ответ: экипировка персонажа
    [<Literal>] let CMD_MC_CALL_CHA         = 528us // GM: телепорт персонажа к себе
    [<Literal>] let CMD_MC_GOTO_CHA         = 529us // GM: телепорт к персонажу
    [<Literal>] let CMD_MC_KICK_CHA         = 530us // GM: кик персонажа
    [<Literal>] let CMD_MC_SYNDEFAULTSKILL  = 531us // Синхронизация скилла по умолчанию
    [<Literal>] let CMD_MC_ADD_ITEM_CHA     = 532us // GM: добавить предмет
    [<Literal>] let CMD_MC_DEL_ITEM_CHA     = 533us // GM: удалить предмет
    [<Literal>] let CMD_MC_QUERY_CHAPING    = 534us // Ответ: пинг персонажа
    [<Literal>] let CMD_MC_QUERY_RELIVE     = 535us // Запрос на воскрешение
    [<Literal>] let CMD_MC_PREMOVE_TIME     = 536us // Время до принудительного перемещения
    [<Literal>] let CMD_MC_CHECK_PING       = 537us // Ответ проверки пинга (античит)
    [<Literal>] let CMD_MC_MAP_MASK         = 538us // Маска карты (туман войны)
    [<Literal>] let CMD_MC_OPENHAIR         = 539us // Открыть салон причёсок
    [<Literal>] let CMD_MC_UPDATEHAIR_RES   = 540us // Результат смены причёски
    [<Literal>] let CMD_MC_EVENT_INFO       = 541us // Информация о событии
    [<Literal>] let CMD_MC_SIDE_INFO        = 542us // Информация о фракции
    [<Literal>] let CMD_MC_TEAM_FIGHT_ASK   = 543us // Запрос PvP от другого игрока
    [<Literal>] let CMD_MC_ITEM_REPAIR_ASK  = 544us // Предложение ремонта
    [<Literal>] let CMD_MC_ITEM_REPAIR_ASR  = 545us // Результат ремонта
    [<Literal>] let CMD_MC_BEGIN_ITEM_REPAIR = 546us // Открыть окно ремонта
    [<Literal>] let CMD_MC_APPEND_LOOK      = 547us // Обновить внешний вид
    [<Literal>] let CMD_MC_ITEM_FORGE_ASK   = 548us // Предложение ковки
    [<Literal>] let CMD_MC_ITEM_FORGE_ASR   = 549us // Результат ковки
    [<Literal>] let CMD_MC_ITEM_USE_SUC     = 550us // Предмет успешно использован
    [<Literal>] let CMD_MC_KITBAG_CAPACITY  = 551us // Ёмкость инвентаря
    [<Literal>] let CMD_MC_ESPE_ITEM        = 552us // Специальный предмет
    [<Literal>] let CMD_MC_KITBAG_CHECK_ASR = 553us // Результат проверки инвентаря
    [<Literal>] let CMD_MC_KITBAGTEMP_SYNC  = 554us // Синхронизация временного инвентаря
    [<Literal>] let CMD_MC_STORE_OPEN_ASR   = 561us // Магазин открыт
    [<Literal>] let CMD_MC_STORE_LIST_ASR   = 562us // Список товаров магазина
    [<Literal>] let CMD_MC_STORE_BUY_ASR    = 563us // Результат покупки
    [<Literal>] let CMD_MC_STORE_CHANGE_ASR = 564us // Результат обмена
    [<Literal>] let CMD_MC_STORE_QUERY      = 565us // Ответ запроса магазина
    [<Literal>] let CMD_MC_STORE_VIP        = 566us // VIP-данные магазина
    [<Literal>] let CMD_MC_STORE_AFFICHE    = 567us // Объявление магазина
    [<Literal>] let CMD_MC_POPUP_NOTICE     = 568us // Всплывающее уведомление
    [<Literal>] let CMD_MC_BLACKMARKET_EXCHANGEDATA     = 571us // Данные чёрного рынка
    [<Literal>] let CMD_MC_BLACKMARKET_EXCHANGE_ASR     = 572us // Результат обмена чёрного рынка
    [<Literal>] let CMD_MC_BLACKMARKET_EXCHANGEUPDATE   = 573us // Обновление чёрного рынка
    [<Literal>] let CMD_MC_BLACKMARKET_TRADEUPDATE      = 574us // Обновление торгов чёрного рынка
    [<Literal>] let CMD_MC_EXCHANGEDATA                 = 575us // Данные обмена
    [<Literal>] let CMD_MC_VOLUNTER_LIST    = 581us // Список волонтёров
    [<Literal>] let CMD_MC_VOLUNTER_STATE   = 582us // Статус волонтёра
    [<Literal>] let CMD_MC_VOLUNTER_SEL     = 583us // Выбор волонтёра
    [<Literal>] let CMD_MC_VOLUNTER_OPEN    = 584us // Открыть панель волонтёров
    [<Literal>] let CMD_MC_VOLUNTER_ASK     = 585us // Приглашение волонтёра
    [<Literal>] let CMD_MC_LISTAUCTION      = 586us // Список аукциона
    [<Literal>] let CMD_MC_RANK             = 587us // Данные рейтинга
    [<Literal>] let CMD_MC_STALLSEARCH      = 588us // Результаты поиска лавок
    [<Literal>] let CMD_MC_MASTER_ASK       = 591us // Приглашение от мастера
    [<Literal>] let CMD_MC_PRENTICE_ASK     = 592us // Приглашение от ученика
    [<Literal>] let CMD_MC_CHAPLAYEFFECT    = 593us // Визуальный эффект персонажа
    [<Literal>] let CMD_MC_ITEM_UNLOCK_ASR  = 595us // Результат разблокировки предмета
    [<Literal>] let CMD_MC_SAY2CAMP         = 596us // Сообщение в чат фракции
    [<Literal>] let CMD_MC_GM_MAIL          = 597us // GM-почта
    [<Literal>] let CMD_MC_CHEAT_CHECK      = 598us // Античит-ответ
    [<Literal>] let CMD_MC_LIFESKILL_BGING  = 600us // Крафт: процесс идёт
    [<Literal>] let CMD_MC_LIFESKILL_ASR    = 601us // Результат крафта
    [<Literal>] let CMD_MC_LIFESKILL_ASK    = 602us // Окно крафта
    [<Literal>] let CMD_MC_ITEM_LOTTERY_ASK = 603us // Окно лотереи
    [<Literal>] let CMD_MC_ITEM_LOTTERY_ASR = 604us // Результат лотереи
    [<Literal>] let CMD_MC_CAPTAIN_ASK      = 605us // Приглашение капитана (корабль)
    [<Literal>] let CMD_MC_ITEM_AMPHITHEATER_ASR = 606us // Результат арены
    [<Literal>] let CMD_MC_UPDATEIMP        = 611us // Обновление импланта/перса
    [<Literal>] let CMD_MC_GUILDNOTICE      = 612us // Уведомление гильдии
    [<Literal>] let CMD_MC_REQUESTPIN       = 613us // Запрос PIN-кода
    [<Literal>] let CMD_MC_RecDailyBuffInfo = 615us // Информация ежедневного баффа
    [<Literal>] let CMD_MC_REQUEST_DROP_RATE = 623us // Текущий дропрейт
    [<Literal>] let CMD_MC_REQUEST_EXP_RATE  = 624us // Текущий экспрейт

    // MC — персонаж/логин (CHARBASE = 930)
    [<Literal>] let CMD_MC_LOGIN            = 931us // Ответ на логин (список персонажей)
    [<Literal>] let CMD_MC_LOGOUT           = 932us // Подтверждение выхода
    [<Literal>] let CMD_MC_BGNPLAY          = 933us // Ответ на начало игры
    [<Literal>] let CMD_MC_ENDPLAY          = 934us // Ответ на выход к выбору персонажа
    [<Literal>] let CMD_MC_NEWCHA           = 935us // Результат создания персонажа
    [<Literal>] let CMD_MC_DELCHA           = 936us // Результат удаления персонажа
    [<Literal>] let CMD_MC_STARTEXIT        = 937us // Начало таймера выхода (10 сек)
    [<Literal>] let CMD_MC_CANCELEXIT       = 938us // Отмена таймера выхода
    [<Literal>] let CMD_MC_CREATE_PASSWORD2 = 941us // Результат создания 2-го пароля
    [<Literal>] let CMD_MC_UPDATE_PASSWORD2 = 942us // Результат обновления 2-го пароля
    [<Literal>] let CMD_MC_SEND_SERVER_PUBLIC_KEY = 943us // RSA публичный ключ сервера
    [<Literal>] let CMD_MC_SEND_HANDSHAKE   = 944us // Подтверждение RSA/AES handshake

    // MC — ролевая система (ROLEBASE = 800)
    [<Literal>] let CMD_MC_TALKPAGE         = 801us // Страница диалога NPC
    [<Literal>] let CMD_MC_FUNCPAGE         = 802us // Функциональная страница NPC
    [<Literal>] let CMD_MC_CLOSETALK        = 803us // Закрыть диалог NPC
    [<Literal>] let CMD_MC_HELPINFO         = 804us // Подсказка
    [<Literal>] let CMD_MC_TRADEPAGE        = 805us // Страница торговли NPC
    [<Literal>] let CMD_MC_TRADERESULT      = 806us // Результат торговли
    [<Literal>] let CMD_MC_TRADE_DATA       = 807us // Данные торговли
    [<Literal>] let CMD_MC_TRADE_ALLDATA    = 808us // Полные данные торговли
    [<Literal>] let CMD_MC_TRADE_UPDATE     = 809us // Обновление торговли
    [<Literal>] let CMD_MC_EXCHANGE_UPDATE  = 810us // Обновление обмена
    [<Literal>] let CMD_MC_CHARTRADE                = 811us // Обмен между игроками
    [<Literal>] let CMD_MC_CHARTRADE_REQUEST        = 812us // Входящий запрос обмена
    [<Literal>] let CMD_MC_CHARTRADE_CANCEL         = 813us // Обмен отменён
    [<Literal>] let CMD_MC_CHARTRADE_ITEM           = 814us // Предмет в обмене обновлён
    [<Literal>] let CMD_MC_CHARTRADE_PAGE           = 815us // Страница обмена
    [<Literal>] let CMD_MC_CHARTRADE_VALIDATEDATA   = 816us // Данные валидации обмена
    [<Literal>] let CMD_MC_CHARTRADE_VALIDATE       = 817us // Обмен подтверждён (locked)
    [<Literal>] let CMD_MC_CHARTRADE_RESULT         = 818us // Результат обмена
    [<Literal>] let CMD_MC_CHARTRADE_MONEY          = 819us // Золото в обмене обновлено
    [<Literal>] let CMD_MC_MISSION          = 822us // Данные квеста
    [<Literal>] let CMD_MC_MISSIONLIST      = 823us // Список квестов
    [<Literal>] let CMD_MC_MISSIONTALK      = 824us // Диалог квеста
    [<Literal>] let CMD_MC_NPCSTATECHG      = 825us // Изменение состояния NPC
    [<Literal>] let CMD_MC_TRIGGER_ACTION   = 826us // Триггер скриптового действия
    [<Literal>] let CMD_MC_MISPAGE          = 827us // Страница квеста
    [<Literal>] let CMD_MC_MISLOG           = 828us // Журнал квестов
    [<Literal>] let CMD_MC_MISLOGINFO       = 829us // Детали журнала квестов
    [<Literal>] let CMD_MC_MISLOG_CHANGE    = 830us // Обновление записи журнала
    [<Literal>] let CMD_MC_MISLOG_CLEAR     = 831us // Очистка журнала
    [<Literal>] let CMD_MC_MISLOG_ADD       = 832us // Новая запись в журнале
    [<Literal>] let CMD_MC_BEGIN_ITEM_FUSION     = 833us // Открыть окно слияния
    [<Literal>] let CMD_MC_BEGIN_ITEM_UPGRADE    = 834us // Открыть окно улучшения
    [<Literal>] let CMD_MC_BEGIN_ITEM_FORGE      = 835us // Открыть окно ковки
    [<Literal>] let CMD_MC_BEGIN_ITEM_UNITE      = 836us // Открыть окно объединения
    [<Literal>] let CMD_MC_BEGIN_ITEM_MILLING    = 837us // Открыть окно разборки
    [<Literal>] let CMD_MC_CREATEBOAT       = 838us // Корабль создан
    [<Literal>] let CMD_MC_UPDATEBOAT       = 839us // Корабль обновлён
    [<Literal>] let CMD_MC_UPDATEBOAT_PART  = 840us // Часть корабля обновлена
    [<Literal>] let CMD_MC_BERTH_LIST       = 841us // Список причалов
    [<Literal>] let CMD_MC_BOAT_LIST        = 842us // Список кораблей
    [<Literal>] let CMD_MC_BOAT_ADD         = 843us // Корабль добавлен
    [<Literal>] let CMD_MC_BOAT_CLEAR       = 844us // Корабль удалён
    [<Literal>] let CMD_MC_BOATINFO         = 845us // Информация о корабле
    [<Literal>] let CMD_MC_BOAT_BAGLIST     = 846us // Содержимое трюма
    [<Literal>] let CMD_MC_BEGIN_ITEM_EIDOLON_METEMPSYCHOSIS = 847us // Окно перерождения эйдолона
    [<Literal>] let CMD_MC_BEGIN_ITEM_EIDOLON_FUSION = 848us // Окно слияния эйдолонов
    [<Literal>] let CMD_MC_BEGIN_ITEM_PURIFY = 849us // Открыть окно очистки
    [<Literal>] let CMD_MC_ENTITY_BEGINESEE = 850us // Сущность появилась (мина, ловушка)
    [<Literal>] let CMD_MC_ENTITY_ENDSEE    = 851us // Сущность исчезла
    [<Literal>] let CMD_MC_ENTITY_CHGSTATE  = 852us // Состояние сущности изменилось
    [<Literal>] let CMD_MC_STALL_ALLDATA    = 854us // Данные лавки
    [<Literal>] let CMD_MC_STALL_UPDATE     = 855us // Обновление лавки
    [<Literal>] let CMD_MC_STALL_DELGOODS   = 856us // Товар убран из лавки
    [<Literal>] let CMD_MC_STALL_CLOSE      = 857us // Лавка закрыта
    [<Literal>] let CMD_MC_STALL_START      = 858us // Лавка открыта
    [<Literal>] let CMD_MC_STALL_NAME       = 859us // Название лавки
    [<Literal>] let CMD_MC_BICKER_NOTICE    = 860us // Уведомление о ссоре/PK
    [<Literal>] let CMD_MC_BEGIN_ITEM_ENERGY     = 871us // Открыть окно энергии предмета
    [<Literal>] let CMD_MC_BEGIN_GET_STONE       = 872us // Открыть окно извлечения камня
    [<Literal>] let CMD_MC_BEGIN_TIGER           = 873us // Открыть слот-машину
    [<Literal>] let CMD_MC_TIGER_ITEM_ID         = 874us // ID предмета в слот-машине
    [<Literal>] let CMD_MC_TIGER_STOP            = 875us // Слот-машина остановлена
    [<Literal>] let CMD_MC_BEGIN_ITEM_FIX        = 876us // Открыть окно починки
    [<Literal>] let CMD_MC_BEGIN_GM_SEND         = 877us // Открыть окно GM-отправки
    [<Literal>] let CMD_MC_BEGIN_GM_RECV         = 878us // Открыть окно GM-получения
    [<Literal>] let CMD_MC_BEGIN_ITEM_LOTTERY     = 879us // Открыть окно лотереи
    [<Literal>] let CMD_MC_BEGIN_ITEM_AMPHITHEATER = 880us // Открыть окно арены
    [<Literal>] let CMD_MC_COLOUR_NOTICE    = 892us // Цветное уведомление
    [<Literal>] let CMD_MC_UPDATEGUILDBANKGOLD = 893us // Обновление золота банка гильдии

    // MC — гильдии (GULDBASE = 900)
    [<Literal>] let CMD_MC_GUILD_GETNAME        = 901us // Название гильдии
    [<Literal>] let CMD_MC_LISTGUILD            = 902us // Список гильдий
    [<Literal>] let CMD_MC_GUILD_TRYFORCFM      = 903us // Подтверждение заявки в гильдию
    [<Literal>] let CMD_MC_GUILD_LISTTRYPLAYER  = 904us // Список заявок
    [<Literal>] let CMD_MC_GUILD_LISTCHALL      = 905us // Список вызовов на войну
    [<Literal>] let CMD_MC_GUILD_MOTTO          = 910us // Девиз гильдии
    [<Literal>] let CMD_MC_GUILD_LEAVE          = 911us // Подтверждение выхода из гильдии
    [<Literal>] let CMD_MC_GUILD_KICK           = 912us // Подтверждение исключения
    [<Literal>] let CMD_MC_GUILD_INFO           = 913us // Информация о гильдии

    // ════════════════════════════════════════════════════════════
    //  GateServer ↔ GameServer  (TM / MT)
    // ════════════════════════════════════════════════════════════
    [<Literal>] let CMD_TM_ENTERMAP         = 1003us // Вход игрока на карту
    [<Literal>] let CMD_TM_GOOUTMAP         = 1004us // Выход игрока с карты
    [<Literal>] let CMD_TM_LOGIN_ACK        = 1005us // Ответ на регистрацию GameServer
    [<Literal>] let CMD_TM_CHANGE_PERSONINFO = 1006us // Обновить личные данные на GameServer
    [<Literal>] let CMD_TM_MAPENTRY         = 1007us // Маршрутизация входа на карту
    [<Literal>] let CMD_TM_MAPENTRY_NOMAP   = 1008us // Карта не найдена на Gate
    [<Literal>] let CMD_TM_KICKCHA          = 1009us // Кикнуть персонажа (fire-and-forget)
    [<Literal>] let CMD_TM_OFFLINE_MODE     = 1011us // Переключение офлайн-режима

    [<Literal>] let CMD_MT_LOGIN            = 1501us // Регистрация GameServer (имя + карты)
    [<Literal>] let CMD_MT_SWITCHMAP        = 1504us // Перемещение игрока между картами
    [<Literal>] let CMD_MT_KICKUSER         = 1505us // Запрос кика от GameServer
    [<Literal>] let CMD_MT_MAPENTRY         = 1506us // Маршрутизация карты между GameServer
    [<Literal>] let CMD_MT_PALYEREXIT       = 1508us // Игрок завершает выход (по таймеру)

    // ════════════════════════════════════════════════════════════
    //  GateServer ↔ GroupServer  (TP / PT)
    // ════════════════════════════════════════════════════════════
    [<Literal>] let CMD_TP_LOGIN            = 2001us // Регистрация Gate на GroupServer
    [<Literal>] let CMD_TP_LOGOUT           = 2002us // Отключение Gate от GroupServer
    [<Literal>] let CMD_TP_USER_LOGIN       = 2003us // Авторизация игрока (SyncCall)
    [<Literal>] let CMD_TP_USER_LOGOUT      = 2004us // Выход игрока (SyncCall)
    [<Literal>] let CMD_TP_BGNPLAY          = 2005us // Начало игры / выбор персонажа (SyncCall)
    [<Literal>] let CMD_TP_ENDPLAY          = 2006us // Возврат к выбору персонажа (SyncCall)
    [<Literal>] let CMD_TP_NEWCHA           = 2007us // Создание персонажа (SyncCall)
    [<Literal>] let CMD_TP_DELCHA           = 2008us // Удаление персонажа (SyncCall)
    [<Literal>] let CMD_TP_PLAYEREXIT       = 2009us // Уведомление о выходе игрока
    [<Literal>] let CMD_TP_REQPLYLST        = 2010us // Запрос списка игроков
    [<Literal>] let CMD_TP_DISC             = 2011us // Уведомление о дисконнекте (actId, IP, причина)
    [<Literal>] let CMD_TP_ESTOPUSER_CHECK  = 2012us // Проверка estop (мут) по actId
    [<Literal>] let CMD_TP_CREATE_PASSWORD2 = 2013us // Создание 2-го пароля (SyncCall)
    [<Literal>] let CMD_TP_UPDATE_PASSWORD2 = 2014us // Обновление 2-го пароля (SyncCall)
    [<Literal>] let CMD_TP_SYNC_PLYLST      = 2015us // Синхронизация списка игроков
    [<Literal>] let CMD_TP_REGISTER         = 2016us // Регистрация аккаунта (SyncCall)
    [<Literal>] let CMD_TP_CHANGEPASS       = 2017us // Смена пароля (SyncCall)

    [<Literal>] let CMD_PT_KICKUSER         = 2511us // Кик игрока по gpAddr
    [<Literal>] let CMD_PT_ESTOPUSER        = 2512us // Включить мут (estop)
    [<Literal>] let CMD_PT_DEL_ESTOPUSER    = 2513us // Снять мут (estop)
    [<Literal>] let CMD_PT_REGISTER         = 2515us // Ответ на регистрацию
    [<Literal>] let CMD_PT_KICKPLAYINGPLAYER = 2516us // Кик играющего (RPC, SyncCall)

    // ════════════════════════════════════════════════════════════
    //  GroupServer ↔ AccountServer  (PA / AP)
    // ════════════════════════════════════════════════════════════
    [<Literal>] let CMD_PA_LOGIN            = 3002us // Регистрация Group на Account
    [<Literal>] let CMD_PA_USER_LOGIN       = 3004us // Авторизация пользователя
    [<Literal>] let CMD_PA_USER_LOGOUT      = 3005us // Выход пользователя
    [<Literal>] let CMD_PA_USER_DISABLE     = 3006us // Отключить пользователя
    [<Literal>] let CMD_PA_USER_LOGIN1      = 3013us // Авторизация ч.1 (двухэтапная)
    [<Literal>] let CMD_PA_USER_LOGIN2      = 3014us // Авторизация ч.2 (двухэтапная)
    [<Literal>] let CMD_PA_USER_BILLBGN     = 3020us // Начало биллинга
    [<Literal>] let CMD_PA_USER_BILLEND     = 3021us // Конец биллинга
    [<Literal>] let CMD_PA_GROUP_BILLEND_AND_LOGOUT = 3022us // Биллинг + выход
    [<Literal>] let CMD_PA_LOGOUT           = 3023us // Отключение Group от Account
    [<Literal>] let CMD_PA_GMBANACCOUNT     = 3024us // GM: заблокировать аккаунт
    [<Literal>] let CMD_PA_GMUNBANACCOUNT   = 3025us // GM: разблокировать аккаунт
    [<Literal>] let CMD_PA_REGISTER         = 3026us // Регистрация аккаунта
    [<Literal>] let CMD_PA_CHANGEPASS       = 3027us // Смена пароля

    [<Literal>] let CMD_AP_LOGIN            = 3502us // Ответ на регистрацию Group
    [<Literal>] let CMD_AP_USER_LOGIN       = 3503us // Результат авторизации
    [<Literal>] let CMD_AP_RELOGIN          = 3504us // Повторный вход (кик предыдущей сессии)
    [<Literal>] let CMD_AP_KICKUSER         = 3511us // Кик от AccountServer
    [<Literal>] let CMD_AP_USER_LOGIN1      = 3512us // Результат авторизации ч.1
    [<Literal>] let CMD_AP_USER_LOGIN2      = 3513us // Результат авторизации ч.2
    [<Literal>] let CMD_AP_EXPSCALE         = 3514us // Множитель опыта от Account
    [<Literal>] let CMD_AP_REGISTER         = 3515us // Результат регистрации

    // ════════════════════════════════════════════════════════════
    //  GameServer ↔ GameServer  (MM)
    // ════════════════════════════════════════════════════════════
    [<Literal>] let CMD_MM_GATE_RELEASE     = 4001us // Gate освобождает GameServer
    [<Literal>] let CMD_MM_GATE_CONNECT     = 4002us // Gate подключает GameServer
    [<Literal>] let CMD_MM_QUERY_CHA        = 4003us // GM: запрос персонажа (кросс-сервер)
    [<Literal>] let CMD_MM_QUERY_CHAITEM    = 4004us // GM: запрос экипировки (кросс-сервер)
    [<Literal>] let CMD_MM_CALL_CHA         = 4005us // GM: телепорт персонажа (кросс-сервер)
    [<Literal>] let CMD_MM_GOTO_CHA         = 4006us // GM: телепорт к персонажу (кросс-сервер)
    [<Literal>] let CMD_MM_KICK_CHA         = 4007us // GM: кик (кросс-сервер)
    [<Literal>] let CMD_MM_GUILD_REJECT     = 4008us // Отклонение гильдии (кросс-сервер)
    [<Literal>] let CMD_MM_GUILD_APPROVE    = 4009us // Одобрение гильдии (кросс-сервер)
    [<Literal>] let CMD_MM_GUILD_KICK       = 4010us // Исключение из гильдии (кросс-сервер)
    [<Literal>] let CMD_MM_GUILD_DISBAND    = 4011us // Роспуск гильдии (кросс-сервер)
    [<Literal>] let CMD_MM_QUERY_CHAPING    = 4012us // Пинг персонажа (кросс-сервер)
    [<Literal>] let CMD_MM_NOTICE           = 4013us // Общее уведомление (broadcast)
    [<Literal>] let CMD_MM_GUILD_MOTTO      = 4014us // Девиз гильдии (кросс-сервер)
    [<Literal>] let CMD_MM_DO_STRING        = 4015us // Выполнить Lua-строку (GM-команда)
    [<Literal>] let CMD_MM_CHA_NOTICE       = 4016us // Уведомление персонажу
    [<Literal>] let CMD_MM_LOGIN            = 4017us // GameServer → GameServer handshake
    [<Literal>] let CMD_MM_GUILD_CHALL_PRIZEMONEY = 4018us // Призовые войны гильдий
    [<Literal>] let CMD_MM_ADDCREDIT        = 4019us // Начислить кредиты (кросс-сервер)
    [<Literal>] let CMD_MM_STORE_BUY        = 4020us // Покупка в магазине (кросс-сервер)
    [<Literal>] let CMD_MM_ADDMONEY         = 4021us // Начислить золото (кросс-сервер)
    [<Literal>] let CMD_MM_AUCTION          = 4022us // Аукцион (кросс-сервер)
    [<Literal>] let CMD_MM_UPDATEGUILDBANK  = 4023us // Обновить банк гильдии
    [<Literal>] let CMD_MM_UPDATEGUILDBANKGOLD = 4024us // Обновить золото банка гильдии

    // ════════════════════════════════════════════════════════════
    //  GroupServer ↔ GameServer  (PM / MP)
    // ════════════════════════════════════════════════════════════
    [<Literal>] let CMD_PM_TEAM             = 4501us // Обновление команды (broadcast)
    [<Literal>] let CMD_PM_GUILD_DISBAND    = 4502us // Роспуск гильдии → все GameServer
    [<Literal>] let CMD_PM_GUILDINFO        = 4504us // Информация о гильдии → GameServer
    [<Literal>] let CMD_PM_GUILD_CHALLMONEY = 4505us // Деньги за войну гильдий
    [<Literal>] let CMD_PM_GUILD_CHALL_PRIZEMONEY = 4506us // Призовой фонд войны гильдий
    [<Literal>] let CMD_PM_GARNER2_UPDATE   = 4507us // Обновление склада Garner
    [<Literal>] let CMD_PM_TEAM_CREATE      = 4508us // Создание команды → GameServer
    [<Literal>] let CMD_PM_SAY2ALL          = 4509us // Глобальный чат → все GameServer
    [<Literal>] let CMD_PM_SAY2TRADE        = 4510us // Торговый чат → все GameServer
    [<Literal>] let CMD_PM_EXPSCALE         = 4511us // Множитель опыта → GameServer
    [<Literal>] let CMD_PM_GUILDBANK        = 4515us // Банк гильдии → GameServer
    [<Literal>] let CMD_PM_PUSHTOGUILDBANK  = 4518us // Пополнить банк гильдии → GameServer

    [<Literal>] let CMD_MP_ENTERMAP         = 5501us // Уведомление: игрок вошёл на карту
    [<Literal>] let CMD_MP_GUILD_APPROVE    = 5502us // Одобрение заявки → Group
    [<Literal>] let CMD_MP_GUILD_CREATE     = 5503us // Создание гильдии → Group
    [<Literal>] let CMD_MP_GUILD_KICK       = 5504us // Исключение → Group
    [<Literal>] let CMD_MP_GUILD_LEAVE      = 5505us // Выход из гильдии → Group
    [<Literal>] let CMD_MP_GUILD_DISBAND    = 5506us // Роспуск → Group
    [<Literal>] let CMD_MP_GUILD_MOTTO      = 5510us // Девиз → Group
    [<Literal>] let CMD_MP_GUILD_CHALLMONEY = 5513us // Деньги за войну → Group
    [<Literal>] let CMD_MP_GUILD_CHALL_PRIZEMONEY = 5514us // Призовой фонд → Group
    [<Literal>] let CMD_MP_GARNER2_UPDATE   = 5507us // Обновление склада → Group
    [<Literal>] let CMD_MP_GARNER2_CGETORDER = 5515us // Запрос порядка склада → Group
    [<Literal>] let CMD_MP_TEAM_CREATE      = 5516us // Создание команды → Group
    [<Literal>] let CMD_MP_MASTER_CREATE    = 5517us // Создание связи мастер-ученик → Group
    [<Literal>] let CMD_MP_MASTER_DEL       = 5518us // Удаление мастера → Group
    [<Literal>] let CMD_MP_MASTER_FINISH    = 5519us // Завершение обучения → Group
    [<Literal>] let CMD_MP_SAY2ALL          = 5520us // Глобальный чат → Group (broadcast)
    [<Literal>] let CMD_MP_SAY2TRADE        = 5521us // Торговый чат → Group (broadcast)
    [<Literal>] let CMD_MP_GM1SAY1          = 5522us // GM-сообщение (системное) → Group
    [<Literal>] let CMD_MP_GM1SAY           = 5523us // GM-команда → Group
    [<Literal>] let CMD_MP_GMBANACCOUNT     = 5524us // GM: бан аккаунта → Group
    [<Literal>] let CMD_MP_GMUNBANACCOUNT   = 5525us // GM: разбан аккаунта → Group
    [<Literal>] let CMD_MP_GUILD_PERM       = 5527us // Права гильдии → Group
    [<Literal>] let CMD_MP_GUILDBANK        = 5528us // Банк гильдии → Group
    [<Literal>] let CMD_MP_PUSHTOGUILDBANK  = 5531us // Пополнить банк → Group
    [<Literal>] let CMD_MP_GUILDNOTICE      = 5532us // Уведомление гильдии → Group
    [<Literal>] let CMD_MP_CANRECEIVEREQUESTS = 5533us // Приём заявок в гильдию → Group
    [<Literal>] let CMD_MP_MUTE_PLAYER      = 5534us // GM: замутить игрока → Group

    // ════════════════════════════════════════════════════════════
    //  GroupServer ↔ Client  (CP / PC)
    // ════════════════════════════════════════════════════════════
    [<Literal>] let CMD_CP_TEAM_INVITE      = 6001us // Приглашение в команду
    [<Literal>] let CMD_CP_TEAM_ACCEPT      = 6002us // Принять приглашение в команду
    [<Literal>] let CMD_CP_TEAM_REFUSE      = 6003us // Отклонить приглашение
    [<Literal>] let CMD_CP_TEAM_LEAVE       = 6004us // Покинуть команду
    [<Literal>] let CMD_CP_TEAM_KICK        = 6005us // Исключить из команды
    [<Literal>] let CMD_CP_FRND_INVITE      = 6011us // Приглашение в друзья
    [<Literal>] let CMD_CP_FRND_ACCEPT      = 6012us // Принять в друзья
    [<Literal>] let CMD_CP_FRND_REFUSE      = 6013us // Отклонить дружбу
    [<Literal>] let CMD_CP_FRND_DELETE      = 6014us // Удалить из друзей
    [<Literal>] let CMD_CP_FRND_CHANGE_GROUP    = 6015us // Изменить группу друга
    [<Literal>] let CMD_CP_FRND_REFRESH_INFO    = 6016us // Обновить данные друга
    [<Literal>] let CMD_CP_CHANGE_PERSONINFO    = 6017us // Изменить персональные данные
    [<Literal>] let CMD_CP_FRND_DEL_GROUP       = 6018us // Удалить группу друзей
    [<Literal>] let CMD_CP_FRND_ADD_GROUP       = 6019us // Создать группу друзей
    [<Literal>] let CMD_CP_FRND_MOVE_GROUP      = 6020us // Переместить друга в группу
    [<Literal>] let CMD_CP_QUERY_PERSONINFO     = 6021us // Запрос данных игрока
    [<Literal>] let CMD_CP_PING             = 6022us // Пинг к GroupServer
    [<Literal>] let CMD_CP_REPORT_WG        = 6023us // Отчёт о читере (WG = WallGuard)
    [<Literal>] let CMD_CP_MASTER_REFRESH_INFO      = 6031us // Обновить данные мастера
    [<Literal>] let CMD_CP_PRENTICE_REFRESH_INFO    = 6032us // Обновить данные ученика
    [<Literal>] let CMD_CP_GM1SAY           = 6400us // GM-команда от клиента
    [<Literal>] let CMD_CP_SAY2TRADE        = 6401us // Торговый чат
    [<Literal>] let CMD_CP_SAY2ALL          = 6402us // Глобальный чат
    [<Literal>] let CMD_CP_SAY2YOU          = 6403us // Личное сообщение (whisper)
    [<Literal>] let CMD_CP_SAY2TEM          = 6404us // Чат команды
    [<Literal>] let CMD_CP_SAY2GUD          = 6405us // Чат гильдии
    [<Literal>] let CMD_CP_SESS_CREATE      = 6406us // Создать сессию чата
    [<Literal>] let CMD_CP_SESS_SAY         = 6407us // Сообщение в сессию чата
    [<Literal>] let CMD_CP_SESS_ADD         = 6408us // Добавить в сессию чата
    [<Literal>] let CMD_CP_SESS_LEAVE       = 6409us // Покинуть сессию чата
    [<Literal>] let CMD_CP_REFUSETOME       = 6410us // Блокировка (ignore) игрока
    [<Literal>] let CMD_CP_GM1SAY1          = 6411us // GM: системное сообщение
    [<Literal>] let CMD_CP_GUILDBANK        = 6414us // Операции с банком гильдии
    [<Literal>] let CMD_CP_CHANGEPASS       = 6415us // Смена пароля (SyncCall)

    [<Literal>] let CMD_PC_TEAM_INVITE      = 5001us // Входящее приглашение в команду
    [<Literal>] let CMD_PC_TEAM_REFRESH     = 5002us // Обновление состава команды
    [<Literal>] let CMD_PC_TEAM_CANCEL      = 5003us // Приглашение отменено/отклонено
    [<Literal>] let CMD_PC_FRND_INVITE      = 5011us // Входящее приглашение в друзья
    [<Literal>] let CMD_PC_FRND_REFRESH     = 5012us // Обновление списка друзей
    [<Literal>] let CMD_PC_FRND_CANCEL      = 5013us // Приглашение в друзья отменено
    [<Literal>] let CMD_PC_FRND_CHANGE_GROUP    = 5015us // Группа друга изменена
    [<Literal>] let CMD_PC_FRND_REFRESH_INFO    = 5016us // Данные друга обновлены
    [<Literal>] let CMD_PC_CHANGE_PERSONINFO    = 5017us // Персональные данные обновлены
    [<Literal>] let CMD_PC_FRND_DEL_GROUP       = 5018us // Группа друзей удалена
    [<Literal>] let CMD_PC_FRND_ADD_GROUP       = 5019us // Группа друзей создана
    [<Literal>] let CMD_PC_FRND_MOVE_GROUP      = 5020us // Друг перемещён в группу
    [<Literal>] let CMD_PC_QUERY_PERSONINFO     = 5021us // Ответ: данные игрока
    [<Literal>] let CMD_PC_PING             = 5022us // Ответ на пинг GroupServer
    [<Literal>] let CMD_PC_GUILD            = 5030us // Данные гильдии
    [<Literal>] let CMD_PC_GUILD_PERM       = 5031us // Права в гильдии
    [<Literal>] let CMD_PC_GM_INFO          = 5032us // GM-информация
    [<Literal>] let CMD_PC_MASTER_REFRESH   = 5041us // Обновление списка мастеров
    [<Literal>] let CMD_PC_MASTER_CANCEL    = 5042us // Приглашение мастера отменено
    [<Literal>] let CMD_PC_MASTER_REFRESH_INFO      = 5043us // Данные мастера обновлены
    [<Literal>] let CMD_PC_PRENTICE_REFRESH_INFO    = 5044us // Данные ученика обновлены
    [<Literal>] let CMD_PC_REGISTER         = 5045us // Результат регистрации аккаунта
    [<Literal>] let CMD_PC_GM1SAY           = 5400us // GM-команда → клиенту
    [<Literal>] let CMD_PC_SAY2TRADE        = 5401us // Торговый чат → клиенту
    [<Literal>] let CMD_PC_SAY2ALL          = 5402us // Глобальный чат → клиенту
    [<Literal>] let CMD_PC_SAY2YOU          = 5403us // Личное сообщение → клиенту
    [<Literal>] let CMD_PC_SAY2TEM          = 5404us // Чат команды → клиенту
    [<Literal>] let CMD_PC_SAY2GUD          = 5405us // Чат гильдии → клиенту
    [<Literal>] let CMD_PC_SESS_CREATE      = 5406us // Сессия чата создана
    [<Literal>] let CMD_PC_SESS_SAY         = 5407us // Сообщение сессии чата
    [<Literal>] let CMD_PC_SESS_ADD         = 5408us // Добавлен в сессию чата
    [<Literal>] let CMD_PC_SESS_LEAVE       = 5409us // Покинул сессию чата
    [<Literal>] let CMD_PC_GM1SAY1          = 5411us // GM: системное → клиенту
    [<Literal>] let CMD_PC_ERRMSG           = 5414us // Сообщение об ошибке
    [<Literal>] let CMD_PC_GUILDNOTICE      = 5417us // Уведомление гильдии → клиенту
    [<Literal>] let CMD_PC_REFRESH_SELECT   = 5418us // Обновить экран выбора персонажа
    [<Literal>] let CMD_PC_GARNER2_ORDER    = 5101us // Порядок склада Garner

    // ════════════════════════════════════════════════════════════
    //  Monitor ↔ Server  (OS / SO)
    // ════════════════════════════════════════════════════════════
    [<Literal>] let CMD_OS_LOGIN            = 6501us // Авторизация монитора
    [<Literal>] let CMD_OS_PING             = 6502us // Пинг монитора
    [<Literal>] let CMD_SO_LOGIN            = 7001us // Ответ на авторизацию монитора
    [<Literal>] let CMD_SO_PING             = 7002us // Ответ на пинг монитора
    [<Literal>] let CMD_SO_WARING           = 7003us // Предупреждение → монитору
    [<Literal>] let CMD_SO_EXCEPTION        = 7004us // Исключение → монитору
    [<Literal>] let CMD_SO_ON_LINE          = 7005us // Игрок онлайн → монитору
    [<Literal>] let CMD_SO_OFF_LINE         = 7006us // Игрок офлайн → монитору
    [<Literal>] let CMD_SO_ENTER_MAP        = 7007us // Вход на карту → монитору
    [<Literal>] let CMD_SO_LEAVE_MAP        = 7008us // Выход с карты → монитору

    // ════════════════════════════════════════════════════════════
    //  GateServer → Client  (TC)
    // ════════════════════════════════════════════════════════════
    [<Literal>] let CMD_TC_DISCONNECT       = 7502us // Принудительное отключение клиента

    // ════════════════════════════════════════════════════════════
    //  Коды ошибок (перенесены из NetRetCode.h)
    // ════════════════════════════════════════════════════════════

    [<Literal>] let ERR_SUCCESS         = 0s   // Успешно

    // GameServer/GateServer → Client (MC, база 0)
    [<Literal>] let ERR_MC_BASE         = 0s   // Базовый код MC (0..500)
    [<Literal>] let ERR_MC_NETEXCP      = 1s   // Сетевая ошибка GateServer
    [<Literal>] let ERR_MC_NOTSELCHA    = 2s   // Не в состоянии выбора персонажа
    [<Literal>] let ERR_MC_NOTPLAY      = 3s   // Не в состоянии игры
    [<Literal>] let ERR_MC_NOTARRIVE    = 4s   // Карта недоступна
    [<Literal>] let ERR_MC_TOOMANYPLY   = 5s   // Слишком много игроков на сервере
    [<Literal>] let ERR_MC_NOTLOGIN     = 6s   // Не залогинен
    [<Literal>] let ERR_MC_VER_ERROR    = 7s   // Неверная версия клиента
    [<Literal>] let ERR_MC_ENTER_ERROR  = 8s   // Ошибка входа на карту
    [<Literal>] let ERR_MC_ENTER_POS    = 9s   // Некорректная позиция на карте
    [<Literal>] let ERR_MC_BANUSER      = 10s  // Аккаунт заблокирован (официально)
    [<Literal>] let ERR_MC_PBANUSER     = 11s  // Аккаунт заблокирован (персональная защита)

    // GroupServer → GateServer (PT, база 500)
    [<Literal>] let ERR_PT_BASE         = 500s  // Базовый код PT (500..1000)
    [<Literal>] let ERR_PT_LOGFAIL      = 501s  // Регистрация Gate на Group провалена
    [<Literal>] let ERR_PT_SAMEGATENAME = 502s  // Имя GateServer уже зарегистрировано
    [<Literal>] let ERR_PT_INVALIDDAT   = 520s  // Невалидный формат данных
    [<Literal>] let ERR_PT_INERR        = 521s  // Внутренняя ошибка GroupServer
    [<Literal>] let ERR_PT_NETEXCP      = 522s  // Ошибка связи Group→AccountServer
    [<Literal>] let ERR_PT_DBEXCP       = 523s  // Ошибка БД на GroupServer
    [<Literal>] let ERR_PT_INVALIDCHA   = 524s  // Аккаунт не владеет персонажем
    [<Literal>] let ERR_PT_TOMAXCHA     = 525s  // Достигнут лимит персонажей
    [<Literal>] let ERR_PT_SAMECHANAME  = 526s  // Имя персонажа уже занято
    [<Literal>] let ERR_PT_INVALIDBIRTH = 527s  // Некорректный класс/профессия
    [<Literal>] let ERR_PT_TOOBIGCHANM  = 528s  // Имя персонажа слишком длинное
    [<Literal>] let ERR_PT_KICKUSER     = 529s  // Кик от GroupServer
    [<Literal>] let ERR_PT_ISGLDLEADER  = 530s  // Нельзя удалить — лидер гильдии
    [<Literal>] let ERR_PT_ERRCHANAME   = 531s  // Недопустимое имя персонажа
    [<Literal>] let ERR_PT_SERVERBUSY   = 532s  // Сервер перегружен
    [<Literal>] let ERR_PT_TOOBIGPW2    = 533s  // Второй пароль слишком длинный
    [<Literal>] let ERR_PT_INVALID_PW2  = 534s  // Невалидный второй пароль
    [<Literal>] let ERR_PT_BANUSER      = 535s  // Аккаунт заблокирован (официально)
    [<Literal>] let ERR_PT_PBANUSER     = 536s  // Аккаунт заблокирован (персональная защита)
    [<Literal>] let ERR_PT_GMISLOG      = 537s  // GM уже залогинен
    [<Literal>] let ERR_PT_MULTICHA     = 538s  // Мульти-персонаж запрещён
    [<Literal>] let ERR_PT_BONUSCHARS   = 539s  // Бонусные слоты персонажей
    [<Literal>] let ERR_PT_BADBOY       = 550s  // Негативная репутация (PK)

    // AccountServer → GroupServer (AP, база 1000)
    [<Literal>] let ERR_AP_BASE         = 1000s // Базовый код AP (1000..1500)
    [<Literal>] let ERR_AP_INVALIDUSER  = 1001s // Неверное имя пользователя
    [<Literal>] let ERR_AP_INVALIDPWD   = 1002s // Неверный пароль
    [<Literal>] let ERR_AP_ACTIVEUSER   = 1003s // Ошибка активации аккаунта
    [<Literal>] let ERR_AP_LOGGED       = 1004s // Пользователь уже залогинен
    [<Literal>] let ERR_AP_DISABLELOGIN = 1005s // Логин запрещён для аккаунта
    [<Literal>] let ERR_AP_BANUSER      = 1006s // Аккаунт заблокирован (официально)
    [<Literal>] let ERR_AP_PBANUSER     = 1007s // Аккаунт заблокирован (персональная защита)
    [<Literal>] let ERR_AP_SBANUSER     = 1008s // Аккаунт заблокирован (доп.)
    [<Literal>] let ERR_AP_GPSLOGGED    = 1011s // Уже залогинен на GroupServer
    [<Literal>] let ERR_AP_GPSAUTHFAIL  = 1012s // Ошибка авторизации на GroupServer
    [<Literal>] let ERR_AP_UNKNOWN      = 1100s // Неизвестная ошибка
    [<Literal>] let ERR_AP_OFFLINE      = 1101s // Аккаунт в офлайн-состоянии
    [<Literal>] let ERR_AP_LOGIN1       = 1102s // Аккаунт в фазе авторизации 1
    [<Literal>] let ERR_AP_LOGIN2       = 1103s // Аккаунт в фазе авторизации 2
    [<Literal>] let ERR_AP_ONLINE       = 1104s // Аккаунт уже онлайн
    [<Literal>] let ERR_AP_SAVING       = 1105s // Аккаунт сохраняется
    [<Literal>] let ERR_AP_LOGINTWICE   = 1106s // Повторный логин запрещён
    [<Literal>] let ERR_AP_DISCONN      = 1107s // GroupServer отключён
    [<Literal>] let ERR_AP_UNKNOWNCMD   = 1108s // Неизвестная команда
    [<Literal>] let ERR_AP_TLSWRONG     = 1109s // Ошибка TLS-хранилища потока
    [<Literal>] let ERR_AP_NOBILL       = 1110s // Требуется пополнение аккаунта
    [<Literal>] let ERR_AP_NOALT        = 1111s // Нет альтов
    [<Literal>] let ERR_AP_3ACCS        = 1112s // Лимит 3 аккаунта

    // GateServer → GameServer (TM, база 2000)
    [<Literal>] let ERR_TM_BASE         = 2000s // Базовый код TM (2000..2500)
    [<Literal>] let ERR_TM_OVERNAME     = 2001s // Дублирующееся имя GameServer
    [<Literal>] let ERR_TM_OVERMAP      = 2002s // Дублирующаяся карта
    [<Literal>] let ERR_TM_MAPERR       = 2003s // Ошибка парсинга строки карт

    // MonitorServer → Server (OS, база 2500)
    [<Literal>] let ERR_OS_BASE              = 2500s // Базовый код OS (2500..3000)
    [<Literal>] let ERR_OS_NOTMATCH_VERSION  = 2501s // Несовпадение версий
    [<Literal>] let ERR_OS_RELOGIN           = 2502s // Двойной логин монитора

    // ════════════════════════════════════════════════════════════
    //  Суб-сообщения (MSG_*)
    // ════════════════════════════════════════════════════════════

    module TeamMsg =
        [<Literal>] let ADD         = 1uy  // Участник добавлен
        [<Literal>] let LEAVE       = 2uy  // Участник вышел
        [<Literal>] let UPDATE      = 3uy  // Данные обновлены
        [<Literal>] let GROUP_ADD   = 4uy  // Группа добавлена
        [<Literal>] let GROUP_BREAK = 5uy  // Группа распущена
        [<Literal>] let KICK        = 6uy  // Участник исключён

    module TeamCancelMsg =
        [<Literal>] let BUSY        = 1uy  // Цель занята
        [<Literal>] let TIMEOUT     = 2uy  // Таймаут приглашения
        [<Literal>] let OFFLINE     = 3uy  // Цель офлайн
        [<Literal>] let ISFULL      = 4uy  // Команда полная
        [<Literal>] let CANCEL      = 5uy  // Отменено вручную

    module FriendRefreshMsg =
        [<Literal>] let START       = 1uy  // Начало списка друзей
        [<Literal>] let ADD         = 2uy  // Друг добавлен
        [<Literal>] let DEL         = 3uy  // Друг удалён
        [<Literal>] let ONLINE      = 4uy  // Друг вошёл в игру
        [<Literal>] let OFFLINE     = 5uy  // Друг вышел из игры

    module FriendCancelMsg =
        [<Literal>] let BUSY            = 1uy  // Цель занята
        [<Literal>] let TIMEOUT         = 2uy  // Таймаут
        [<Literal>] let OFFLINE         = 3uy  // Цель офлайн
        [<Literal>] let INVITER_ISFULL  = 4uy  // Список инициатора полон
        [<Literal>] let SELF_ISFULL     = 5uy  // Свой список полон
        [<Literal>] let CANCEL          = 6uy  // Отменено вручную

    module GuildMsg =
        [<Literal>] let START       = 1uy  // Начало списка гильдии
        [<Literal>] let ADD         = 2uy  // Участник добавлен
        [<Literal>] let DEL         = 3uy  // Участник удалён
        [<Literal>] let ONLINE      = 4uy  // Участник вошёл в игру
        [<Literal>] let OFFLINE     = 5uy  // Участник вышел из игры
        [<Literal>] let STOP        = 6uy  // Конец списка гильдии

    module MasterRefreshMsg =
        [<Literal>] let START               = 1uy  // Начало списка мастеров
        [<Literal>] let ADD                 = 2uy  // Мастер добавлен
        [<Literal>] let DEL                 = 3uy  // Мастер удалён
        [<Literal>] let ONLINE              = 4uy  // Мастер онлайн
        [<Literal>] let OFFLINE             = 5uy  // Мастер офлайн
        [<Literal>] let PRENTICE_START      = 6uy  // Начало списка учеников
        [<Literal>] let PRENTICE_ADD        = 7uy  // Ученик добавлен
        [<Literal>] let PRENTICE_DEL        = 8uy  // Ученик удалён
        [<Literal>] let PRENTICE_ONLINE     = 9uy  // Ученик онлайн
        [<Literal>] let PRENTICE_OFFLINE    = 10uy // Ученик офлайн

