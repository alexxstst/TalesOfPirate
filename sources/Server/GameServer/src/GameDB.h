#include "Util.h"
#include "Database.h"
#include "GameAppNet.h"
#include "Player.h"

#define defCHA_TABLE_VER		110
#define defCHA_TABLE_NEW_VER	111

enum ESaveType {
	enumSAVE_TYPE_OFFLINE, //
	enumSAVE_TYPE_SWITCH, //
	enumSAVE_TYPE_TIMER, //
	enumSAVE_TYPE_TRADE, //
};

class CPlayer;

// ============================================================================
// Row structures — маппинг 1:1 на строки SQL-таблиц
// ============================================================================

struct AccountRow {
	std::int32_t ato_id;
	std::string ato_nome;
	std::int32_t jmes;
	std::string ator_ids;
	std::string last_ip;
	std::string disc_reason;
	std::string last_leave;
	std::string password;
	std::int32_t merge_state;
	std::int32_t IMP;
	std::int32_t total_votes;
	std::int32_t credit;
};

struct AmphitheaterSettingRow {
	std::int32_t section;
	std::int32_t season;
	std::int32_t round;
	std::int32_t state;
	std::string createdate;
	std::string updatetime;
};

struct AmphitheaterTeamRow {
	std::int32_t id;
	std::int32_t captain;
	std::string member;
	std::int32_t matchno;
	std::int32_t state;
	std::int32_t map;
	std::int32_t mapflag;
	std::int32_t winnum;
	std::int32_t losenum;
	std::int32_t relivenum;
	std::string createdate;
	std::string updatetime;
};

struct BoatRow {
	std::int32_t boat_id;
	std::int32_t boat_berth;
	std::string boat_name;
	std::int32_t boat_boatid;
	std::int32_t boat_header;
	std::int32_t boat_body;
	std::int32_t boat_engine;
	std::int32_t boat_cannon;
	std::int32_t boat_equipment;
	std::int32_t boat_bagsize;
	std::string boat_bag;
	std::int32_t boat_diecount;
	std::string boat_isdead;
	std::int32_t cur_endure;
	std::int32_t mx_endure;
	std::int32_t cur_supply;
	std::int32_t mx_supply;
	std::string skill_state;
	std::int32_t boat_ownerid;
	std::string boat_createtime;
	std::string boat_isdeleted;
	std::string map;
	std::int32_t map_x;
	std::int32_t map_y;
	std::int32_t angle;
	std::int32_t degree;
	std::int32_t exp;
	std::int32_t version;
};

struct CharacterRow {
	std::int32_t atorID;
	std::string atorNome;
	std::string motto;
	std::int32_t icon;
	std::int32_t version;
	std::int32_t pk_ctrl;
	std::int32_t endeMem;
	std::int32_t ato_id;
	std::int32_t guild_id;
	std::int32_t guild_stat;
	std::int64_t guild_permission;
	std::string job;
	std::int32_t degree;
	std::int64_t exp;
	std::int32_t hp;
	std::int32_t sp;
	std::int32_t ap;
	std::int32_t tp;
	std::int32_t bomd;
	std::int32_t str, dex, agi, con, sta, luk;
	std::int32_t sail_lv;
	std::int32_t sail_exp;
	std::int32_t sail_left_exp;
	std::int32_t live_lv;
	std::int32_t live_exp;
	std::string map;
	std::int32_t map_x;
	std::int32_t map_y;
	std::int32_t radius;
	std::int32_t angle;
	std::string olhe;
	std::int32_t kb_capacity;
	std::string kitbag;
	std::string skillbag;
	std::string shortcut;
	std::string mission;
	std::string misrecord;
	std::string mistrigger;
	std::string miscount;
	std::string birth;
	std::string login_cha;
	std::int32_t live_tp;
	std::string map_mask;
	std::int32_t delflag;
	std::string operdate;
	std::string deldate;
	std::string main_map;
	std::string skill_state;
	std::string bank;
	std::string estop;
	std::int32_t estoptime;
	std::int32_t kb_locked;
	std::int32_t kitbag_tmp;
	std::int32_t credit;
	std::int32_t store_item;
	std::string extend;
	std::int32_t chatColour;
	std::int32_t IMP;
};

struct CharacterLogRow {
	std::int32_t atorID;
	std::string atorNome;
	std::int32_t ato_id;
	std::int32_t guild_id;
	std::string job;
	std::int32_t degree;
	std::int32_t exp;
	std::int32_t hp, sp, ap, tp, bomd;
	std::int32_t str, dex, agi, con, sta, luk;
	std::string map;
	std::int32_t map_x, map_y, radius;
	std::string olhe;
	std::string del_date;
};

struct FriendsRow {
	std::int32_t cha_id1;
	std::int32_t cha_id2;
	std::string relation;
	std::string createtime;
};

struct GuildRow {
	std::int32_t guild_id;
	std::string guild_name;
	std::string motto;
	std::string passwd;
	std::int32_t leader_id;
	std::int64_t exp;
	std::int64_t gold;
	std::string bank;
	std::int32_t level;
	std::int32_t member_total;
	std::int32_t try_total;
	std::string disband_date;
	std::int32_t challlevel;
	std::int32_t challid;
	std::int64_t challmoney;
	std::int32_t challstart;
};

struct LotterySettingRow {
	std::int32_t section;
	std::int32_t issue;
	std::int32_t state;
	std::string createdate;
	std::string updatetime;
	std::string itemno;
};

struct MapMaskRow {
	std::int32_t id;
	std::int32_t atorID;
	std::string content1;
	std::string content2;
	std::string content3;
	std::string content4;
	std::string content5;
};

struct MasterRow {
	std::int32_t cha_id1;
	std::int32_t cha_id2;
	std::int32_t finish;
	std::string relation;
};

struct ParamRow {
	std::int32_t id;
	std::int32_t param1, param2, param3, param4, param5;
	std::int32_t param6, param7, param8, param9, param10;
};

struct PersonAvatarRow {
	std::int32_t atorID;
	std::vector<uint8_t> avatar;
};

struct PersonInfoRow {
	std::int32_t atorID;
	std::string motto;
	std::int32_t showmotto;
	std::string sex;
	std::int32_t age;
	std::string name;
	std::string animal_zodiac;
	std::string blood_type;
	std::int32_t birthday;
	std::string state;
	std::string city;
	std::string constellation;
	std::string career;
	std::int32_t avatarsize;
	std::int32_t prevent;
	std::int32_t support;
	std::int32_t oppose;
};

struct PropertyRow {
	std::int64_t id;
	std::int64_t atorID;
	std::string context;
	std::int64_t sum;
	std::string time;
};

struct ResourceRow {
	std::int32_t id;
	std::int32_t atorID;
	std::int32_t type_id;
	std::string content;
};

struct StatLogRow {
	std::string track_date;
	std::int32_t login_num;
	std::int32_t play_num;
	std::int32_t wgplay_num;
};

struct StatDegreeRow {
	std::string statDate;
	std::int32_t degree;
	std::int64_t characterCount;
};

struct StatGenderRow {
	std::string statDate;
	std::string gender;
	std::int64_t genderCount;
};

struct StatJobRow {
	std::string statDate;
	std::string job;
	std::int64_t characterCount;
};

struct StatLoginRow {
	std::string statDate;
	std::int64_t loginCount;
};

struct StatMapRow {
	std::string statDate;
	std::string map;
	std::int64_t playCount;
};

struct TicketRow {
	std::int32_t id;
	std::int32_t atorID;
	std::int32_t issue;
	std::string itemno;
	std::int32_t real;
	std::string buydate;
};

struct TradeLogRow {
	std::int32_t ID;
	std::string ExecuteTime;
	std::string GameServer;
	std::string Action;
	std::string From;
	std::string To;
	std::string Memo;
};

struct WeekReportRow {
	std::string ato_nome;
	std::string atorNome;
	std::int32_t degree;
	std::string ip;
	std::string createdate;
	std::string logouttime;
	std::int32_t playtime;
	std::string Guild_Name;
};

struct WinTicketRow {
	std::int32_t issue;
	std::string itemno;
	std::int32_t grade;
	std::string createdate;
	std::int32_t num;
};

// ============================================================================
// Типизированные таблицы — наследники OdbcTable<T> (Column DSL)
// ============================================================================

class TableAccount : public OdbcTable<AccountRow> {
public:
	explicit TableAccount(OdbcDatabase& db)
		: OdbcTable(db, "account", {
			MakeColumn("ato_id",       &AccountRow::ato_id, PrimaryKey),
			MakeColumn("ato_nome",     &AccountRow::ato_nome),
			MakeColumn("jmes",         &AccountRow::jmes),
			MakeColumn("ator_ids",     &AccountRow::ator_ids),
			MakeColumn("last_ip",      &AccountRow::last_ip),
			MakeColumn("disc_reason",  &AccountRow::disc_reason),
			MakeColumn("last_leave",   &AccountRow::last_leave, Timestamp),
			MakeColumn("password",     &AccountRow::password),
			MakeColumn("merge_state",  &AccountRow::merge_state),
			MakeColumn("IMP",          &AccountRow::IMP),
			MakeColumn("total_votes",  &AccountRow::total_votes),
			MakeColumn("credit",       &AccountRow::credit),
		}) {}
};

class TableAmphitheaterSetting : public OdbcTable<AmphitheaterSettingRow> {
public:
	explicit TableAmphitheaterSetting(OdbcDatabase& db)
		: OdbcTable(db, "AmphitheaterSetting", {
			MakeColumn("section",    &AmphitheaterSettingRow::section),
			MakeColumn("season",     &AmphitheaterSettingRow::season),
			MakeColumn("round",      &AmphitheaterSettingRow::round),
			MakeColumn("state",      &AmphitheaterSettingRow::state),
			MakeColumn("createdate", &AmphitheaterSettingRow::createdate, Timestamp),
			MakeColumn("updatetime", &AmphitheaterSettingRow::updatetime, Nullable | Timestamp),
		}) {}
};

class TableAmphitheaterTeam : public OdbcTable<AmphitheaterTeamRow> {
public:
	explicit TableAmphitheaterTeam(OdbcDatabase& db)
		: OdbcTable(db, "AmphitheaterTeam", {
			MakeColumn("id",         &AmphitheaterTeamRow::id, PrimaryKey),
			MakeColumn("captain",    &AmphitheaterTeamRow::captain),
			MakeColumn("member",     &AmphitheaterTeamRow::member),
			MakeColumn("matchno",    &AmphitheaterTeamRow::matchno),
			MakeColumn("state",      &AmphitheaterTeamRow::state),
			MakeColumn("map",        &AmphitheaterTeamRow::map),
			MakeColumn("mapflag",    &AmphitheaterTeamRow::mapflag),
			MakeColumn("winnum",     &AmphitheaterTeamRow::winnum),
			MakeColumn("losenum",    &AmphitheaterTeamRow::losenum),
			MakeColumn("relivenum",  &AmphitheaterTeamRow::relivenum),
			MakeColumn("createdate", &AmphitheaterTeamRow::createdate, Nullable | Timestamp),
			MakeColumn("updatetime", &AmphitheaterTeamRow::updatetime, Timestamp),
		}) {}
};

class TableBoat : public OdbcTable<BoatRow> {
public:
	explicit TableBoat(OdbcDatabase& db)
		: OdbcTable(db, "boat", {
			MakeColumn("boat_id",         &BoatRow::boat_id, PrimaryKey),
			MakeColumn("boat_berth",      &BoatRow::boat_berth),
			MakeColumn("boat_name",       &BoatRow::boat_name),
			MakeColumn("boat_boatid",     &BoatRow::boat_boatid),
			MakeColumn("boat_header",     &BoatRow::boat_header),
			MakeColumn("boat_body",       &BoatRow::boat_body),
			MakeColumn("boat_engine",     &BoatRow::boat_engine),
			MakeColumn("boat_cannon",     &BoatRow::boat_cannon),
			MakeColumn("boat_equipment",  &BoatRow::boat_equipment),
			MakeColumn("boat_bagsize",    &BoatRow::boat_bagsize),
			MakeColumn("boat_bag",        &BoatRow::boat_bag),
			MakeColumn("boat_diecount",   &BoatRow::boat_diecount),
			MakeColumn("boat_isdead",     &BoatRow::boat_isdead),
			MakeColumn("cur_endure",      &BoatRow::cur_endure),
			MakeColumn("mx_endure",       &BoatRow::mx_endure),
			MakeColumn("cur_supply",      &BoatRow::cur_supply),
			MakeColumn("mx_supply",       &BoatRow::mx_supply),
			MakeColumn("skill_state",     &BoatRow::skill_state),
			MakeColumn("boat_ownerid",    &BoatRow::boat_ownerid),
			MakeColumn("boat_createtime", &BoatRow::boat_createtime),
			MakeColumn("boat_isdeleted",  &BoatRow::boat_isdeleted),
			MakeColumn("map",             &BoatRow::map),
			MakeColumn("map_x",           &BoatRow::map_x),
			MakeColumn("map_y",           &BoatRow::map_y),
			MakeColumn("angle",           &BoatRow::angle),
			MakeColumn("degree",          &BoatRow::degree),
			MakeColumn("exp",             &BoatRow::exp),
			MakeColumn("version",         &BoatRow::version),
		}) {}
};

class TableCharacter : public OdbcTable<CharacterRow> {
public:
	explicit TableCharacter(OdbcDatabase& db)
		: OdbcTable(db, "character", {
			MakeColumn("atorID",           &CharacterRow::atorID, PrimaryKey),
			MakeColumn("atorNome",         &CharacterRow::atorNome),
			MakeColumn("motto",            &CharacterRow::motto),
			MakeColumn("icon",             &CharacterRow::icon),
			MakeColumn("version",          &CharacterRow::version),
			MakeColumn("pk_ctrl",          &CharacterRow::pk_ctrl),
			MakeColumn("endeMem",          &CharacterRow::endeMem),
			MakeColumn("ato_id",           &CharacterRow::ato_id),
			MakeColumn("guild_id",         &CharacterRow::guild_id),
			MakeColumn("guild_stat",       &CharacterRow::guild_stat),
			MakeColumn("guild_permission", &CharacterRow::guild_permission),
			MakeColumn("job",              &CharacterRow::job),
			MakeColumn("degree",           &CharacterRow::degree),
			MakeColumn("exp",              &CharacterRow::exp),
			MakeColumn("hp",               &CharacterRow::hp),
			MakeColumn("sp",               &CharacterRow::sp),
			MakeColumn("ap",               &CharacterRow::ap),
			MakeColumn("tp",               &CharacterRow::tp),
			MakeColumn("bomd",             &CharacterRow::bomd),
			MakeColumn("str",              &CharacterRow::str),
			MakeColumn("dex",              &CharacterRow::dex),
			MakeColumn("agi",              &CharacterRow::agi),
			MakeColumn("con",              &CharacterRow::con),
			MakeColumn("sta",              &CharacterRow::sta),
			MakeColumn("luk",              &CharacterRow::luk),
			MakeColumn("sail_lv",          &CharacterRow::sail_lv),
			MakeColumn("sail_exp",         &CharacterRow::sail_exp),
			MakeColumn("sail_left_exp",    &CharacterRow::sail_left_exp),
			MakeColumn("live_lv",          &CharacterRow::live_lv),
			MakeColumn("live_exp",         &CharacterRow::live_exp),
			MakeColumn("map",              &CharacterRow::map),
			MakeColumn("map_x",            &CharacterRow::map_x),
			MakeColumn("map_y",            &CharacterRow::map_y),
			MakeColumn("radius",           &CharacterRow::radius),
			MakeColumn("angle",            &CharacterRow::angle),
			MakeColumn("olhe",             &CharacterRow::olhe),
			MakeColumn("kb_capacity",      &CharacterRow::kb_capacity),
			MakeColumn("kitbag",           &CharacterRow::kitbag),
			MakeColumn("skillbag",         &CharacterRow::skillbag),
			MakeColumn("shortcut",         &CharacterRow::shortcut),
			MakeColumn("mission",          &CharacterRow::mission),
			MakeColumn("misrecord",        &CharacterRow::misrecord),
			MakeColumn("mistrigger",       &CharacterRow::mistrigger),
			MakeColumn("miscount",         &CharacterRow::miscount),
			MakeColumn("birth",            &CharacterRow::birth),
			MakeColumn("login_cha",        &CharacterRow::login_cha),
			MakeColumn("live_tp",          &CharacterRow::live_tp),
			MakeColumn("map_mask",         &CharacterRow::map_mask),
			MakeColumn("delflag",          &CharacterRow::delflag),
			MakeColumn("operdate",         &CharacterRow::operdate, Timestamp),
			MakeColumn("deldate",          &CharacterRow::deldate, Nullable | Timestamp),
			MakeColumn("main_map",         &CharacterRow::main_map),
			MakeColumn("skill_state",      &CharacterRow::skill_state),
			MakeColumn("bank",             &CharacterRow::bank),
			MakeColumn("estop",            &CharacterRow::estop, Timestamp),
			MakeColumn("estoptime",        &CharacterRow::estoptime),
			MakeColumn("kb_locked",        &CharacterRow::kb_locked),
			MakeColumn("kitbag_tmp",       &CharacterRow::kitbag_tmp),
			MakeColumn("credit",           &CharacterRow::credit),
			MakeColumn("store_item",       &CharacterRow::store_item),
			MakeColumn("extend",           &CharacterRow::extend),
			MakeColumn("chatColour",       &CharacterRow::chatColour),
			MakeColumn("IMP",              &CharacterRow::IMP),
		}) {}
};

class TableCharacterLog : public OdbcTable<CharacterLogRow> {
public:
	explicit TableCharacterLog(OdbcDatabase& db)
		: OdbcTable(db, "character_log", {
			MakeColumn("atorID",   &CharacterLogRow::atorID),
			MakeColumn("atorNome", &CharacterLogRow::atorNome),
			MakeColumn("ato_id",   &CharacterLogRow::ato_id),
			MakeColumn("guild_id", &CharacterLogRow::guild_id),
			MakeColumn("job",      &CharacterLogRow::job),
			MakeColumn("degree",   &CharacterLogRow::degree),
			MakeColumn("exp",      &CharacterLogRow::exp),
			MakeColumn("hp",       &CharacterLogRow::hp),
			MakeColumn("sp",       &CharacterLogRow::sp),
			MakeColumn("ap",       &CharacterLogRow::ap),
			MakeColumn("tp",       &CharacterLogRow::tp),
			MakeColumn("bomd",     &CharacterLogRow::bomd),
			MakeColumn("str",      &CharacterLogRow::str),
			MakeColumn("dex",      &CharacterLogRow::dex),
			MakeColumn("agi",      &CharacterLogRow::agi),
			MakeColumn("con",      &CharacterLogRow::con),
			MakeColumn("sta",      &CharacterLogRow::sta),
			MakeColumn("luk",      &CharacterLogRow::luk),
			MakeColumn("map",      &CharacterLogRow::map),
			MakeColumn("map_x",    &CharacterLogRow::map_x),
			MakeColumn("map_y",    &CharacterLogRow::map_y),
			MakeColumn("radius",   &CharacterLogRow::radius),
			MakeColumn("olhe",     &CharacterLogRow::olhe),
			MakeColumn("del_date", &CharacterLogRow::del_date, Timestamp),
		}) {}
};

class TableFriends : public OdbcTable<FriendsRow> {
public:
	explicit TableFriends(OdbcDatabase& db)
		: OdbcTable(db, "friends", {
			MakeColumn("cha_id1",    &FriendsRow::cha_id1),
			MakeColumn("cha_id2",    &FriendsRow::cha_id2),
			MakeColumn("relation",   &FriendsRow::relation),
			MakeColumn("createtime", &FriendsRow::createtime, Nullable | Timestamp),
		}) {}
};

class TableGuildTyped : public OdbcTable<GuildRow> {
public:
	explicit TableGuildTyped(OdbcDatabase& db)
		: OdbcTable(db, "guild", {
			MakeColumn("guild_id",     &GuildRow::guild_id, PrimaryKey),
			MakeColumn("guild_name",   &GuildRow::guild_name),
			MakeColumn("motto",        &GuildRow::motto),
			MakeColumn("passwd",       &GuildRow::passwd),
			MakeColumn("leader_id",    &GuildRow::leader_id),
			MakeColumn("exp",          &GuildRow::exp),
			MakeColumn("gold",         &GuildRow::gold),
			MakeColumn("bank",         &GuildRow::bank),
			MakeColumn("level",        &GuildRow::level),
			MakeColumn("member_total", &GuildRow::member_total),
			MakeColumn("try_total",    &GuildRow::try_total),
			MakeColumn("disband_date", &GuildRow::disband_date, Timestamp),
			MakeColumn("challlevel",   &GuildRow::challlevel),
			MakeColumn("challid",      &GuildRow::challid),
			MakeColumn("challmoney",   &GuildRow::challmoney),
			MakeColumn("challstart",   &GuildRow::challstart),
		}) {}
};

class TableLotterySetting : public OdbcTable<LotterySettingRow> {
public:
	explicit TableLotterySetting(OdbcDatabase& db)
		: OdbcTable(db, "LotterySetting", {
			MakeColumn("section",    &LotterySettingRow::section),
			MakeColumn("issue",      &LotterySettingRow::issue),
			MakeColumn("state",      &LotterySettingRow::state),
			MakeColumn("createdate", &LotterySettingRow::createdate, Timestamp),
			MakeColumn("updatetime", &LotterySettingRow::updatetime, Nullable | Timestamp),
			MakeColumn("itemno",     &LotterySettingRow::itemno),
		}) {}
};

class TableMapMaskTyped : public OdbcTable<MapMaskRow> {
public:
	explicit TableMapMaskTyped(OdbcDatabase& db)
		: OdbcTable(db, "map_mask", {
			MakeColumn("id",       &MapMaskRow::id, PrimaryKey),
			MakeColumn("atorID",   &MapMaskRow::atorID),
			MakeColumn("content1", &MapMaskRow::content1),
			MakeColumn("content2", &MapMaskRow::content2),
			MakeColumn("content3", &MapMaskRow::content3),
			MakeColumn("content4", &MapMaskRow::content4),
			MakeColumn("content5", &MapMaskRow::content5),
		}) {}
};

class TableMaster : public OdbcTable<MasterRow> {
public:
	explicit TableMaster(OdbcDatabase& db)
		: OdbcTable(db, "master", {
			MakeColumn("cha_id1",  &MasterRow::cha_id1),
			MakeColumn("cha_id2",  &MasterRow::cha_id2),
			MakeColumn("finish",   &MasterRow::finish),
			MakeColumn("relation", &MasterRow::relation),
		}) {}
};

class TableParam : public OdbcTable<ParamRow> {
public:
	explicit TableParam(OdbcDatabase& db)
		: OdbcTable(db, "param", {
			MakeColumn("id",      &ParamRow::id, PrimaryKey),
			MakeColumn("param1",  &ParamRow::param1),
			MakeColumn("param2",  &ParamRow::param2),
			MakeColumn("param3",  &ParamRow::param3),
			MakeColumn("param4",  &ParamRow::param4),
			MakeColumn("param5",  &ParamRow::param5),
			MakeColumn("param6",  &ParamRow::param6),
			MakeColumn("param7",  &ParamRow::param7),
			MakeColumn("param8",  &ParamRow::param8),
			MakeColumn("param9",  &ParamRow::param9),
			MakeColumn("param10", &ParamRow::param10),
		}) {}
};

class TablePersonAvatar : public OdbcTable<PersonAvatarRow> {
public:
	explicit TablePersonAvatar(OdbcDatabase& db)
		: OdbcTable(db, "personavatar", {
			MakeColumn("atorID", &PersonAvatarRow::atorID, PrimaryKey),
			MakeColumn("avatar", &PersonAvatarRow::avatar),
		}) {}
};

class TablePersonInfo : public OdbcTable<PersonInfoRow> {
public:
	explicit TablePersonInfo(OdbcDatabase& db)
		: OdbcTable(db, "personinfo", {
			MakeColumn("atorID",        &PersonInfoRow::atorID, PrimaryKey),
			MakeColumn("motto",         &PersonInfoRow::motto),
			MakeColumn("showmotto",     &PersonInfoRow::showmotto),
			MakeColumn("sex",           &PersonInfoRow::sex),
			MakeColumn("age",           &PersonInfoRow::age),
			MakeColumn("name",          &PersonInfoRow::name),
			MakeColumn("animal_zodiac", &PersonInfoRow::animal_zodiac),
			MakeColumn("blood_type",    &PersonInfoRow::blood_type),
			MakeColumn("birthday",      &PersonInfoRow::birthday),
			MakeColumn("state",         &PersonInfoRow::state),
			MakeColumn("city",          &PersonInfoRow::city),
			MakeColumn("constellation", &PersonInfoRow::constellation),
			MakeColumn("career",        &PersonInfoRow::career),
			MakeColumn("avatarsize",    &PersonInfoRow::avatarsize),
			MakeColumn("prevent",       &PersonInfoRow::prevent),
			MakeColumn("support",       &PersonInfoRow::support),
			MakeColumn("oppose",        &PersonInfoRow::oppose),
		}) {}
};

class TableProperty : public OdbcTable<PropertyRow> {
public:
	explicit TableProperty(OdbcDatabase& db)
		: OdbcTable(db, "property", {
			MakeColumn("id",      &PropertyRow::id, PrimaryKey),
			MakeColumn("atorID",  &PropertyRow::atorID),
			MakeColumn("context", &PropertyRow::context),
			MakeColumn("sum",     &PropertyRow::sum),
			MakeColumn("time",    &PropertyRow::time, Timestamp),
		}) {}
};

class TableResourceTyped : public OdbcTable<ResourceRow> {
public:
	explicit TableResourceTyped(OdbcDatabase& db)
		: OdbcTable(db, "Resource", {
			MakeColumn("id",      &ResourceRow::id, PrimaryKey),
			MakeColumn("atorID",  &ResourceRow::atorID),
			MakeColumn("type_id", &ResourceRow::type_id),
			MakeColumn("content", &ResourceRow::content),
		}) {}
};

class TableStatLog : public OdbcTable<StatLogRow> {
public:
	explicit TableStatLog(OdbcDatabase& db)
		: OdbcTable(db, "stat_log", {
			MakeColumn("track_date",  &StatLogRow::track_date, PrimaryKey | Timestamp),
			MakeColumn("login_num",   &StatLogRow::login_num),
			MakeColumn("play_num",    &StatLogRow::play_num),
			MakeColumn("wgplay_num",  &StatLogRow::wgplay_num),
		}) {}
};

class TableStatDegree : public OdbcTable<StatDegreeRow> {
public:
	explicit TableStatDegree(OdbcDatabase& db)
		: OdbcTable(db, "StatDegree", {
			MakeColumn("statDate",       &StatDegreeRow::statDate, PrimaryKey),
			MakeColumn("degree",         &StatDegreeRow::degree, PrimaryKey),
			MakeColumn("characterCount", &StatDegreeRow::characterCount),
		}) {}
};

class TableStatGender : public OdbcTable<StatGenderRow> {
public:
	explicit TableStatGender(OdbcDatabase& db)
		: OdbcTable(db, "StatGender", {
			MakeColumn("statDate",    &StatGenderRow::statDate, PrimaryKey),
			MakeColumn("gender",      &StatGenderRow::gender, PrimaryKey),
			MakeColumn("genderCount", &StatGenderRow::genderCount),
		}) {}
};

class TableStatJob : public OdbcTable<StatJobRow> {
public:
	explicit TableStatJob(OdbcDatabase& db)
		: OdbcTable(db, "StatJob", {
			MakeColumn("statDate",       &StatJobRow::statDate, PrimaryKey),
			MakeColumn("job",            &StatJobRow::job, PrimaryKey),
			MakeColumn("characterCount", &StatJobRow::characterCount),
		}) {}
};

class TableStatLogin : public OdbcTable<StatLoginRow> {
public:
	explicit TableStatLogin(OdbcDatabase& db)
		: OdbcTable(db, "StatLogin", {
			MakeColumn("statDate",   &StatLoginRow::statDate, PrimaryKey),
			MakeColumn("loginCount", &StatLoginRow::loginCount),
		}) {}
};

class TableStatMap : public OdbcTable<StatMapRow> {
public:
	explicit TableStatMap(OdbcDatabase& db)
		: OdbcTable(db, "StatMap", {
			MakeColumn("statDate",  &StatMapRow::statDate, PrimaryKey),
			MakeColumn("map",       &StatMapRow::map, PrimaryKey),
			MakeColumn("playCount", &StatMapRow::playCount),
		}) {}
};

class TableTicket : public OdbcTable<TicketRow> {
public:
	explicit TableTicket(OdbcDatabase& db)
		: OdbcTable(db, "Ticket", {
			MakeColumn("id",      &TicketRow::id, PrimaryKey),
			MakeColumn("atorID",  &TicketRow::atorID),
			MakeColumn("issue",   &TicketRow::issue),
			MakeColumn("itemno",  &TicketRow::itemno),
			MakeColumn("real",    &TicketRow::real),
			MakeColumn("buydate", &TicketRow::buydate, Timestamp),
		}) {}
};

class TableTradeLog : public OdbcTable<TradeLogRow> {
public:
	explicit TableTradeLog(OdbcDatabase& db)
		: OdbcTable(db, "Trade_Log", {
			MakeColumn("ID",          &TradeLogRow::ID, PrimaryKey),
			MakeColumn("ExecuteTime", &TradeLogRow::ExecuteTime),
			MakeColumn("GameServer",  &TradeLogRow::GameServer),
			MakeColumn("Action",      &TradeLogRow::Action),
			MakeColumn("From",        &TradeLogRow::From),
			MakeColumn("To",          &TradeLogRow::To),
			MakeColumn("Memo",        &TradeLogRow::Memo),
		}) {}
};

class TableWeekReport : public OdbcTable<WeekReportRow> {
public:
	explicit TableWeekReport(OdbcDatabase& db)
		: OdbcTable(db, "weekreport", {
			MakeColumn("ato_nome",   &WeekReportRow::ato_nome),
			MakeColumn("atorNome",   &WeekReportRow::atorNome),
			MakeColumn("degree",     &WeekReportRow::degree),
			MakeColumn("ip",         &WeekReportRow::ip),
			MakeColumn("createdate", &WeekReportRow::createdate, Nullable | Timestamp),
			MakeColumn("logouttime", &WeekReportRow::logouttime, Nullable | Timestamp),
			MakeColumn("playtime",   &WeekReportRow::playtime),
			MakeColumn("Guild_Name", &WeekReportRow::Guild_Name),
		}) {}
};

class TableWinTicket : public OdbcTable<WinTicketRow> {
public:
	explicit TableWinTicket(OdbcDatabase& db)
		: OdbcTable(db, "WinTicket", {
			MakeColumn("issue",      &WinTicketRow::issue),
			MakeColumn("itemno",     &WinTicketRow::itemno),
			MakeColumn("grade",      &WinTicketRow::grade),
			MakeColumn("createdate", &WinTicketRow::createdate, Timestamp),
			MakeColumn("num",        &WinTicketRow::num),
		}) {}
};

// ============================================================================
// Legacy table classes
// ============================================================================

class PlayerStorage {
	OdbcDatabase& _db;
	TableCharacter& _characters;

public:
	PlayerStorage(OdbcDatabase& db, TableCharacter& characters)
		: _db(db), _characters(characters) {
	}

	bool ShowExpRank(CCharacter& pCha, std::int32_t count);
	bool Init(void);
	bool ReadAllData(CPlayer& player, std::uint32_t atorID);
	// bForceWithPos=true — сохранять map/main_map/map_x/map_y даже если
	// pCCtrlCha->GetSubMap() уже nullptr. Нужно для offline-save в
	// CGameApp::ReleaseGamePlayer, где мы снимаем персонажа с карты
	// (GoOut) ДО сохранения, чтобы избежать dangling в eyeshot.
	bool SaveAllData(CPlayer& pPlayer, char chSaveType, bool bForceWithPos = false); //
	bool SavePos(CPlayer& pPlayer); //
	bool SaveMoney(CPlayer& pPlayer);
	bool SaveKBagDBID(CPlayer& pPlayer);
	bool SaveKBagTmpDBID(CPlayer& pPlayer); // ID
	bool SaveKBState(CPlayer& pPlayer); //
	bool SaveMMaskDBID(CPlayer& pPlayer);
	bool SaveBankDBID(CPlayer& pPlayer);
	bool SaveTableVer(std::uint32_t atorID); //
	bool SaveMissionData(CPlayer& pPlayer, std::uint32_t atorID); //
	bool VerifyName(const std::string& pszName); //
	std::string GetName(std::int32_t cha_id);

	bool AddCreditByDBID(std::uint32_t atorID, std::int32_t lCredit);
	bool IsChaOnline(std::uint32_t atorID, bool& bOnline);
	Long GetChaAddr(std::uint32_t atorID);
	bool SetChaAddr(std::uint32_t atorID, Long addr);

	bool SetGuildPermission(std::int32_t atorID, std::uint32_t perm, std::int32_t guild_id);


	bool SaveStoreItemID(std::uint32_t atorID, std::int32_t lStoreItemID);
	bool AddMoney(std::uint32_t atorID, std::int32_t money);

	bool SaveDaily(CPlayer& pPlayer);
};

enum ResDBTypeID {
	enumRESDB_TYPE_LOOK, //
	enumRESDB_TYPE_KITBAG, //
	enumRESDB_TYPE_BANK, //
	enumRESDB_TYPE_KITBAGTMP, //
};

// Add by lark.li 20080521 begin
enum IssueState {
	enumCURRENT = 0, //
	enumPASTDUE = 1, //
	enumDISUSE = 2, //
};

struct AmphitheaterSetting {
	enum AmphitheaterSateSetting {
		enumCURRENT = 0,
	};
};

//Add by sunny.sun 20080725
struct AmphitheaterTeam {
	enum AmphitheaterSateTeam {
		enumNotUse = 0, //
		enumUse = 1, //
		enumPromotion = 2, //
		enumRelive = 3, //
		enumOut = 4, //
	};
};

// Resource — kitbag/bank хранилище (OdbcDatabase)
class CTableResource {
public:
	explicit CTableResource(OdbcDatabase& db) : _db(db) {
	}

	bool Create(std::int32_t& lDBID, std::int32_t lChaId, std::int32_t lTypeId);
	bool ReadKitbagData(CCharacter& pCCha);
	bool SaveKitbagData(CCharacter& pCCha);
	bool ReadKitbagTmpData(CCharacter& pCCha);
	bool SaveKitbagTmpData(CCharacter& pCCha);
	bool ReadBankData(CPlayer& pCPly, std::int8_t chBankNO = -1);
	bool SaveBankData(CPlayer& pCPly, std::int8_t chBankNO = -1);

private:
	OdbcDatabase& _db;
};

// MapMask — миграция на OdbcDatabase
class CTableMapMask {
public:
	explicit CTableMapMask(OdbcDatabase& db) : _db(db) {
	}

	bool Create(std::int32_t& lDBID, std::int32_t lChaId);
	bool ReadData(CPlayer& pCPly);
	bool SaveData(CPlayer& pCPly, bool bDirect = FALSE);

	void HandleSaveList();
	void SaveAll();

	static bool GetColNameByMapName(const std::string& szMapName, std::string& colName);

private:
	OdbcDatabase& _db;
	std::list<std::string> _saveQueue;
};


class CTableBoat {
public:
	explicit CTableBoat(OdbcDatabase& db) : _db(db) {
	}

	bool Create(std::uint32_t& dwBoatID, const BOAT_DATA& Data);
	bool GetBoat(CCharacter& Boat);
	bool SaveBoat(CCharacter& Boat, std::int8_t chSaveType);
	bool SaveBoatTempData(CCharacter& Boat, std::uint8_t byIsDeleted = 0);
	bool SaveBoatTempData(std::uint32_t dwBoatID, std::uint32_t dwOwnerID, std::uint8_t byIsDeleted = 0);
	bool SaveBoatDelTag(std::uint32_t dwBoatID, std::uint8_t byIsDeleted = 0);

	bool SaveAllData(CPlayer& pPlayer, std::int8_t chSaveType);
	bool ReadCabin(CCharacter& Boat);
	bool SaveCabin(CCharacter& Boat, std::int8_t chSaveType);
	bool SaveAllCabin(CPlayer& pPlayer, std::int8_t chSaveType);

private:
	OdbcDatabase& _db;
};

class CTableGuild {
	OdbcDatabase& _db;

public:
	explicit CTableGuild(OdbcDatabase& db) : _db(db) {
	}


	struct BankLog {
		short type;
		time_t time;
		std::int64_t parameter; // ItemID or Gold value
		short quantity; // 1-99 for items, 0 for gold;
		short userID; // chaID of the actor
	};

	//std::vector<BankLog> data;

	std::int32_t Create(CCharacter& pCha, const std::string& guildname, const std::string& passwd);
	bool ListAll(CCharacter& pCha, std::int8_t disband_days);
	void TryFor(CCharacter& pCha, std::uint32_t guildid);
	void TryForConfirm(CCharacter& pCha, std::uint32_t guildid);
	bool GetGuildBank(std::uint32_t guildid, CKitbag* bag);

	bool UpdateGuildBank(std::uint32_t guildid, CKitbag* bag);
	std::int32_t GetGuildLeaderID(std::uint32_t guildid);

	bool SetGuildLog(std::vector<BankLog> log, std::uint32_t guildid);
	std::vector<BankLog> GetGuildLog(std::uint32_t guildid);


	bool UpdateGuildBankGold(std::int32_t guildID, std::int32_t money);
	std::int64_t GetGuildBankGold(std::uint32_t guildid);

	bool GetGuildInfo(CCharacter& pCha, std::uint32_t guildid);
	bool ListTryPlayer(CCharacter& pCha, char disband_days);
	bool Approve(CCharacter& pCha, std::uint32_t chaid);
	bool Reject(CCharacter& pCha, std::uint32_t chaid);
	bool Kick(CCharacter& pCha, std::uint32_t chaid);
	bool Leave(CCharacter& pCha);
	bool Disband(CCharacter& pCha, const std::string& passwd);
	bool Motto(CCharacter& pCha, const std::string& motto);
	bool GetGuildName(std::int32_t lGuildID, std::string& strGuildName);

	//
	bool Challenge(CCharacter& pCha, std::uint8_t byLevel, std::uint32_t dwMoney);
	bool Leizhu(CCharacter& pCha, std::uint8_t byLevel, std::uint32_t dwMoney);
	void ListChallenge(CCharacter& pCha);
	bool GetChallInfo(std::uint8_t byLevel, std::uint32_t& dwGuildID1, std::uint32_t& dwGuildID2, std::uint32_t& dwMoney);
	bool StartChall(std::uint8_t byLevel);
	bool HasCall(std::uint8_t byLevel);
	void EndChall(std::uint32_t dwGuild1, std::uint32_t dwGuild2, bool bChall);
	void ChallMoney(std::uint8_t byLevel, bool bChall, std::uint32_t dwGuildID, std::uint32_t dwChallID, std::uint32_t dwMoney);
	bool ChallWin(bool bUpdate, std::uint8_t byLevel, std::uint32_t dwWinGuildID, std::uint32_t dwFailerGuildID);
	bool HasGuildLevel(CCharacter& pChar, std::uint8_t byLevel);
};

class CGameDB {
public:
	CGameDB();
	~CGameDB();

	bool Init();

	OdbcTransaction BeginTransaction();

	// Совместимость со старым кодом
	bool BeginTran();
	bool RollBack();
	bool CommitTran();

	bool ReadPlayer(CPlayer& pPlayer, std::uint32_t atorID);
	// bForceWithPos — см. PlayerStorage::SaveAllData.
	bool SavePlayer(CPlayer& pPlayer, std::int8_t chSaveType, bool bForceWithPos = false);

	bool SavePlayerKitbag(CPlayer& pPlayer, std::int8_t chSaveType = enumSAVE_TYPE_TRADE);
	bool SaveChaAssets(CCharacter& pCCha);

	// Лотерея — LotterySetting
	bool GetWinItemno(std::int32_t issue, std::string& itemno);
	bool GetLotteryIssue(std::int32_t& issue);
	bool AddIssue(std::int32_t issue);
	bool DisuseIssue(std::int32_t issue, std::int32_t state);

	// Лотерея — Ticket
	bool LotteryIsExsit(std::int32_t issue, const std::string& itemno);
	bool AddLotteryTicket(CCharacter& pCCha, std::int32_t issue, char itemno[6][2]);
	bool CalWinTicket(std::int32_t issue, std::int32_t max, std::string& itemno);

	// Амфитеатр
	bool IsValidAmphitheaterTeam(std::int32_t teamID, std::int32_t captainID, std::int32_t member1, std::int32_t member2);
	bool IsMasterRelation(std::int32_t masterID, std::int32_t prenticeID);

	// === AmphitheaterSetting ===
	bool GetAmphitheaterSeasonAndRound(std::int32_t& season, std::int32_t& round);
	bool AddAmphitheaterSeason(std::int32_t season);
	bool DisuseAmphitheaterSeason(std::int32_t season, std::int32_t state, const std::string& winner);
	bool UpdateAmphitheaterRound(std::int32_t season, std::int32_t round);

	// === AmphitheaterTeam ===
	bool GetAmphitheaterTeamCount(std::int32_t& count);
	bool GetAmphitheaterNoUseTeamID(std::int32_t& teamID);
	bool AmphitheaterTeamSignUP(std::int32_t& teamID, std::int32_t captain, std::int32_t member1, std::int32_t member2);
	bool AmphitheaterTeamCancel(std::int32_t teamID);
	bool IsAmphitheaterLogin(std::int32_t pActorID);
	bool IsMapFull(std::int32_t MapID, std::int32_t& PActorIDNum);
	bool UpdateMapNum(std::int32_t Teamid, std::int32_t Mapid, std::int32_t MapFlag);
	bool GetMapFlag(std::int32_t Teamid, std::int32_t& Mapflag);
	bool SetMaxBallotTeamRelive();
	bool SetMatchResult(std::int32_t Teamid1, std::int32_t Teamid2, std::int32_t Id1state, std::int32_t Id2state);
	bool GetCaptainByMapId(std::int32_t Mapid, std::string& Captainid1, std::string& Captainid2);
	bool UpdateMap(std::int32_t Mapid);
	bool UpdateMapAfterEnter(std::int32_t CaptainID, std::int32_t MapID);
	bool GetPromotionAndReliveTeam(std::vector<std::vector<std::string>>& dataPromotion,
								   std::vector<std::vector<std::string>>& dataRelive);
	bool UpdatReliveNum(std::int32_t ReID);
	bool UpdateAbsentTeamRelive();
	bool UpdateWinnum(std::int32_t teamid);
	bool GetUniqueMaxWinnum(std::int32_t& teamid);
	bool SetMatchnoState(std::int32_t teamid);
	bool UpdateState();
	bool CloseReliveByState(std::int32_t& statenum);
	bool CleanMapFlag(std::int32_t teamid1, std::int32_t teamid2);
	bool GetStateByTeamid(std::int32_t teamid, std::int32_t& state);

	bool UpdateIMP(CPlayer& ply);
	bool SaveGmLv(CPlayer& ply);

	std::string GetChaNameByID(std::int32_t cha_id);
	void ShowExpRank(CCharacter& pCha, std::int32_t top);
	bool SavePlayerPos(CPlayer& pPlayer);
	bool SavePlayerKBagDBID(CPlayer& pPlayer);
	bool SavePlayerKBagTmpDBID(CPlayer& pPlayer);
	bool SavePlayerMMaskDBID(CPlayer& pPlayer);

	bool CreatePlyBank(CPlayer& pCPly);
	bool SavePlyBank(CPlayer& pCPly, char chBankNO = -1);
	std::uint32_t GetPlayerMasterDBID(CPlayer& pPlayer);

	bool AddCreditByDBID(std::uint32_t atorID, std::int32_t lCredit);
	bool SaveStoreItemID(std::uint32_t atorID, std::int32_t lStoreItemID);
	bool AddMoney(std::uint32_t atorID, std::int32_t money);

	bool ReadKitbagTmpData(std::uint32_t res_id, std::string& strData);
	bool SaveKitbagTmpData(std::uint32_t res_id, const std::string& strData);

	bool IsChaOnline(std::uint32_t atorID, bool& bOnline);
	Long GetChaAddr(std::uint32_t atorID);
	Long SetGuildPermission(std::int32_t atorID, std::uint32_t perm, std::int32_t guild_id);
	Long SetChaAddr(std::uint32_t atorID, Long addr);

	bool SaveMissionData(CPlayer& pPlayer, std::uint32_t atorID);

	// Лодки
	bool Create(std::uint32_t& dwBoatID, const BOAT_DATA& Data);
	bool GetBoat(CCharacter& Boat);
	bool SaveBoat(CCharacter& Boat, char chSaveType);
	bool SaveBoatDelTag(std::uint32_t dwBoatID, std::uint8_t byIsDeleted = 0);
	bool SaveBoatTempData(CCharacter& Boat, std::uint8_t byIsDeleted = 0);
	bool SaveBoatTempData(std::uint32_t dwBoatID, std::uint32_t dwOwnerID, std::uint8_t byIsDeleted = 0);

	// Гильдии
	std::int32_t CreateGuild(CCharacter& pCha, const std::string& guildname, const std::string& passwd);
	std::int32_t GetGuildBank(std::uint32_t guildid, CKitbag* bag);
	std::int32_t UpdateGuildBank(std::uint32_t guildid, CKitbag* bag);
	bool SetGuildLog(std::vector<CTableGuild::BankLog> log, std::uint32_t guildid);
	std::vector<CTableGuild::BankLog> GetGuildLog(std::uint32_t guildid);
	std::int64_t GetGuildBankGold(std::uint32_t guildid);
	bool UpdateGuildBankGold(std::int32_t guildID, std::int32_t money);
	std::int32_t GetGuildLeaderID(std::uint32_t guildid);

	bool ListAllGuild(CCharacter& pCha, char disband_days = 1);
	void GuildTryFor(CCharacter& pCha, std::uint32_t guildid);
	void GuildTryForConfirm(CCharacter& pCha, std::uint32_t guildid);
	bool GuildListTryPlayer(CCharacter& pCha, char disband_days);
	bool GuildApprove(CCharacter& pCha, std::uint32_t chaid);
	bool GuildReject(CCharacter& pCha, std::uint32_t chaid);
	bool GuildKick(CCharacter& pCha, std::uint32_t chaid);
	bool GuildLeave(CCharacter& pCha);
	bool GuildDisband(CCharacter& pCha, const std::string& passwd);
	bool GuildMotto(CCharacter& pCha, const std::string& motto);

	CTableMapMask* GetMapMaskTable();

	bool GetGuildName(std::int32_t lGuildID, std::string& strGuildName);
	bool Challenge(CCharacter& pCha, std::uint8_t byLevel, std::uint32_t dwMoney);
	bool Leizhu(CCharacter& pCha, std::uint8_t byLevel, std::uint32_t dwMoney);
	void ListChallenge(CCharacter& pCha);
	bool StartChall(std::uint8_t byLevel);
	bool GetChall(std::uint8_t byLevel, std::uint32_t& dwGuildID1, std::uint32_t& dwGuildID2, std::uint32_t& dwMoney);
	void EndChall(std::uint32_t dwGuild1, std::uint32_t dwGuild2, bool bChall);
	bool HasChall(std::uint8_t byLevel);
	bool HasGuildLevel(CCharacter& pChar, std::uint8_t byLevel);

	// Логирование
	void ExecLogSQL(const std::string& pszSQL);
	void ExecTradeLogSQL(const std::string& gameServerName, const std::string& action,
						 const std::string& pszChaFrom, const std::string& pszChaTo, const std::string& pszTrade);

	bool m_bInitOK{false};

protected:
	// ODBC API — объявлен первым, т.к. все таблицы зависят от _db
	OdbcDatabase _db{};

	// Legacy-таблицы (прямые члены)
	PlayerStorage _tab_cha;
	CTableResource _tab_res;
	CTableMapMask _tab_mmask;
	CTableGuild _tab_gld;
	CTableBoat _tab_boat;

	// Типизированные таблицы
	TableAccount _accounts;
	TableAmphitheaterSetting _amphiSettings;
	TableAmphitheaterTeam _amphiTeams;
	TableBoat _boats;
	TableCharacter _characters;
	TableCharacterLog _characterLogs;
	TableFriends _friends;
	TableGuildTyped _guilds;
	TableLotterySetting _lotterySettings;
	TableMapMaskTyped _mapMasks;
	TableMaster _masters;
	TableParam _params;
	TablePersonAvatar _personAvatars;
	TablePersonInfo _personInfo;
	TableProperty _properties;
	TableResourceTyped _resources;
	TableStatLog _statLogs;
	TableStatDegree _statDegrees;
	TableStatGender _statGenders;
	TableStatJob _statJobs;
	TableStatLogin _statLogins;
	TableStatMap _statMaps;
	TableTicket _tickets;
	TableTradeLog _tradeLogs;
	TableWeekReport _weekReports;
	TableWinTicket _winTickets;
};


extern CGameDB game_db;
