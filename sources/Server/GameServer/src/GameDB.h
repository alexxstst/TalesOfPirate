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
	int ato_id;
	std::string ato_nome;
	int jmes;
	std::string ator_ids;
	std::string last_ip;
	std::string disc_reason;
	std::string last_leave;
	std::string password;
	int merge_state;
	int IMP;
	int total_votes;
	int credit;
};

struct AmphitheaterSettingRow {
	int section;
	int season;
	int round;
	int state;
	std::string createdate;
	std::string updatetime;
};

struct AmphitheaterTeamRow {
	int id;
	int captain;
	std::string member;
	int matchno;
	int state;
	int map;
	int mapflag;
	int winnum;
	int losenum;
	int relivenum;
	std::string createdate;
	std::string updatetime;
};

struct BoatRow {
	int boat_id;
	int boat_berth;
	std::string boat_name;
	int boat_boatid;
	int boat_header;
	int boat_body;
	int boat_engine;
	int boat_cannon;
	int boat_equipment;
	int boat_bagsize;
	std::string boat_bag;
	int boat_diecount;
	std::string boat_isdead;
	int cur_endure;
	int mx_endure;
	int cur_supply;
	int mx_supply;
	std::string skill_state;
	int boat_ownerid;
	std::string boat_createtime;
	std::string boat_isdeleted;
	std::string map;
	int map_x;
	int map_y;
	int angle;
	int degree;
	int exp;
	int version;
};

struct CharacterRow {
	int atorID;
	std::string atorNome;
	std::string motto;
	int icon;
	int version;
	int pk_ctrl;
	int endeMem;
	int ato_id;
	int guild_id;
	int guild_stat;
	int64_t guild_permission;
	std::string job;
	int degree;
	int64_t exp;
	int hp;
	int sp;
	int ap;
	int tp;
	int bomd;
	int str, dex, agi, con, sta, luk;
	int sail_lv;
	int sail_exp;
	int sail_left_exp;
	int live_lv;
	int live_exp;
	std::string map;
	int map_x;
	int map_y;
	int radius;
	int angle;
	std::string olhe;
	int kb_capacity;
	std::string kitbag;
	std::string skillbag;
	std::string shortcut;
	std::string mission;
	std::string misrecord;
	std::string mistrigger;
	std::string miscount;
	std::string birth;
	std::string login_cha;
	int live_tp;
	std::string map_mask;
	int delflag;
	std::string operdate;
	std::string deldate;
	std::string main_map;
	std::string skill_state;
	std::string bank;
	std::string estop;
	int estoptime;
	int kb_locked;
	int kitbag_tmp;
	int credit;
	int store_item;
	std::string extend;
	int chatColour;
	int IMP;
};

struct CharacterLogRow {
	int atorID;
	std::string atorNome;
	int ato_id;
	int guild_id;
	std::string job;
	int degree;
	int exp;
	int hp, sp, ap, tp, bomd;
	int str, dex, agi, con, sta, luk;
	std::string map;
	int map_x, map_y, radius;
	std::string olhe;
	std::string del_date;
};

struct FriendsRow {
	int cha_id1;
	int cha_id2;
	std::string relation;
	std::string createtime;
};

struct GuildRow {
	int guild_id;
	std::string guild_name;
	std::string motto;
	std::string passwd;
	int leader_id;
	int64_t exp;
	int64_t gold;
	std::string bank;
	int level;
	int member_total;
	int try_total;
	std::string disband_date;
	int challlevel;
	int challid;
	int64_t challmoney;
	int challstart;
};

struct LotterySettingRow {
	int section;
	int issue;
	int state;
	std::string createdate;
	std::string updatetime;
	std::string itemno;
};

struct MapMaskRow {
	int id;
	int atorID;
	std::string content1;
	std::string content2;
	std::string content3;
	std::string content4;
	std::string content5;
};

struct MasterRow {
	int cha_id1;
	int cha_id2;
	int finish;
	std::string relation;
};

struct ParamRow {
	int id;
	int param1, param2, param3, param4, param5;
	int param6, param7, param8, param9, param10;
};

struct PersonAvatarRow {
	int atorID;
	std::vector<uint8_t> avatar;
};

struct PersonInfoRow {
	int atorID;
	std::string motto;
	int showmotto;
	std::string sex;
	int age;
	std::string name;
	std::string animal_zodiac;
	std::string blood_type;
	int birthday;
	std::string state;
	std::string city;
	std::string constellation;
	std::string career;
	int avatarsize;
	int prevent;
	int support;
	int oppose;
};

struct PropertyRow {
	int64_t id;
	int64_t atorID;
	std::string context;
	int64_t sum;
	std::string time;
};

struct ResourceRow {
	int id;
	int atorID;
	int type_id;
	std::string content;
};

struct StatLogRow {
	std::string track_date;
	int login_num;
	int play_num;
	int wgplay_num;
};

struct StatDegreeRow {
	std::string statDate;
	int degree;
	int64_t characterCount;
};

struct StatGenderRow {
	std::string statDate;
	std::string gender;
	int64_t genderCount;
};

struct StatJobRow {
	std::string statDate;
	std::string job;
	int64_t characterCount;
};

struct StatLoginRow {
	std::string statDate;
	int64_t loginCount;
};

struct StatMapRow {
	std::string statDate;
	std::string map;
	int64_t playCount;
};

struct TicketRow {
	int id;
	int atorID;
	int issue;
	std::string itemno;
	int real;
	std::string buydate;
};

struct TradeLogRow {
	int ID;
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
	int degree;
	std::string ip;
	std::string createdate;
	std::string logouttime;
	int playtime;
	std::string Guild_Name;
};

struct WinTicketRow {
	int issue;
	std::string itemno;
	int grade;
	std::string createdate;
	int num;
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
			MakeColumn("last_leave",   &AccountRow::last_leave),
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
			MakeColumn("createdate", &AmphitheaterSettingRow::createdate),
			MakeColumn("updatetime", &AmphitheaterSettingRow::updatetime),
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
			MakeColumn("createdate", &AmphitheaterTeamRow::createdate),
			MakeColumn("updatetime", &AmphitheaterTeamRow::updatetime),
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
			MakeColumn("operdate",         &CharacterRow::operdate),
			MakeColumn("deldate",          &CharacterRow::deldate),
			MakeColumn("main_map",         &CharacterRow::main_map),
			MakeColumn("skill_state",      &CharacterRow::skill_state),
			MakeColumn("bank",             &CharacterRow::bank),
			MakeColumn("estop",            &CharacterRow::estop),
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
			MakeColumn("del_date", &CharacterLogRow::del_date),
		}) {}
};

class TableFriends : public OdbcTable<FriendsRow> {
public:
	explicit TableFriends(OdbcDatabase& db)
		: OdbcTable(db, "friends", {
			MakeColumn("cha_id1",    &FriendsRow::cha_id1),
			MakeColumn("cha_id2",    &FriendsRow::cha_id2),
			MakeColumn("relation",   &FriendsRow::relation),
			MakeColumn("createtime", &FriendsRow::createtime),
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
			MakeColumn("disband_date", &GuildRow::disband_date),
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
			MakeColumn("createdate", &LotterySettingRow::createdate),
			MakeColumn("updatetime", &LotterySettingRow::updatetime),
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
			MakeColumn("time",    &PropertyRow::time),
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
			MakeColumn("track_date",  &StatLogRow::track_date, PrimaryKey),
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
			MakeColumn("buydate", &TicketRow::buydate),
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
			MakeColumn("createdate", &WeekReportRow::createdate),
			MakeColumn("logouttime", &WeekReportRow::logouttime),
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
			MakeColumn("createdate", &WinTicketRow::createdate),
			MakeColumn("num",        &WinTicketRow::num),
		}) {}
};

// ============================================================================
// Legacy table classes
// ============================================================================

class PlayerStorage {
	OdbcDatabase& _db;
	TableCharacter& _characters;

	// Совместимые хелперы — cfl_rs API через OdbcDatabase (без try/catch)
	enum { MAX_COL = 64, MAX_DATALEN = 8192 };

	UCHAR _buf[MAX_COL][MAX_DATALEN]{};
	SDWORD _buf_len[MAX_COL]{};

	void handle_err(SQLHANDLE, SQLSMALLINT, RETCODE, const char* = nullptr, bool = false);

	std::string _tbl_name{"character"};
	int _max_col{100};

	const char* _get_table() const;
	SQLRETURN exec_sql_direct(const char* sql);
	int get_affected_rows();
	bool _get_row(std::string buf[], int maxCol, const char* param, const char* filter, int* affect_rows = nullptr);
	bool _get_row2(const char* sql, std::string buf[], int maxCol, int* rows_got = nullptr);
	bool _get_row3(std::string buf[], int maxCol, const char* param, const char* filter, int* affect_rows = nullptr);
	bool begin_tran();
	bool commit_tran();
	bool rollback();
	bool getalldata(const char* sql, std::vector<std::vector<std::string>>& data);

public:
	PlayerStorage(OdbcDatabase& db, TableCharacter& characters)
		: _db(db), _characters(characters) {
	}

	bool ShowExpRank(CCharacter* pCha, int count);
	bool Init(void);
	bool ReadAllData(CPlayer& player, DWORD atorID);
	bool SaveAllData(CPlayer* pPlayer, char chSaveType); //
	bool SavePos(CPlayer* pPlayer); //
	bool SaveMoney(CPlayer* pPlayer);
	bool SaveKBagDBID(CPlayer* pPlayer);
	bool SaveKBagTmpDBID(CPlayer* pPlayer); // ID
	bool SaveKBState(CPlayer* pPlayer); //
	bool SaveMMaskDBID(CPlayer* pPlayer);
	bool SaveBankDBID(CPlayer* pPlayer);
	bool SaveTableVer(DWORD atorID); //
	BOOL SaveMissionData(CPlayer* pPlayer, DWORD atorID); //
	BOOL VerifyName(const char* pszName); //
	std::string GetName(int cha_id);

	BOOL AddCreditByDBID(DWORD atorID, long lCredit);
	BOOL IsChaOnline(DWORD atorID, BOOL& bOnline);
	Long GetChaAddr(DWORD atorID);
	bool SetChaAddr(DWORD atorID, Long addr);

	bool SetGuildPermission(int atorID, unsigned long perm, int guild_id);


	BOOL SaveStoreItemID(DWORD atorID, long lStoreItemID);
	BOOL AddMoney(DWORD atorID, long money);

	BOOL SaveDaily(CPlayer* pPlayer);
};

class CTableMaster : public cfl_rs {
public:
	CTableMaster(cfl_db* pDB)
		: cfl_rs(pDB, "master", 6) {
	}

	bool Init(void);
	unsigned long GetMasterDBID(CPlayer* pPlayer);
	bool IsMasterRelation(int masterID, int prenticeID);
};

//
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

//
class CTableLotterySetting : public cfl_rs {
public:
	CTableLotterySetting(cfl_db* pDB)
		: cfl_rs(pDB, "LotterySetting", 10) {
	}

	bool Init(void);
	bool GetCurrentIssue(int& issue);
	bool AddIssue(int issue);
	bool DisuseIssue(int issue, int state);
	bool SetWinItemNo(int issue, const char* itemno);
	bool GetWinItemNo(int issue, std::string& itemno);
};

//
class CTableTicket : public cfl_rs {
public:
	CTableTicket(cfl_db* pDB)
		: cfl_rs(pDB, "Ticket", 10) {
	}

	bool Init(void);
	bool AddTicket(int atorID, int issue, char itemno[6][2]);
	bool IsExist(int issue, char* itemno);
	bool CalWinTicket(int issue, int max, std::string& itemno);

private:
	bool AddTicket(int atorID, int issue, char itemno1, char itemno2, char itemno3, char itemno4, char itemno5,
				   char itemno6, int real = 1);
};

//
class CTableWinTicket : public cfl_rs {
public:
	CTableWinTicket(cfl_db* pDB)
		: cfl_rs(pDB, "WinTicket", 10) {
	}

	bool Init(void);
	bool GetTicket(int issue);
	bool AddTicket(int issue, char* itemno, int grade);
	bool Exchange(int issue, char* itemno);
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

//
//
class CTableAmphitheaterSetting : public cfl_rs {
public:
	CTableAmphitheaterSetting(cfl_db* pDB)
		: cfl_rs(pDB, "AmphitheaterSetting", 10) {
	}

	bool Init(void);
	bool GetCurrentSeason(int& season, int& round);
	bool AddSeason(int season);
	bool DisuseSeason(int season, int state, const char* winner);
	bool UpdateRound(int season, int round);
};

//
class CTableAmphitheaterTeam : public cfl_rs {
public:
	CTableAmphitheaterTeam(cfl_db* pDB)
		: cfl_rs(pDB, "AmphitheaterTeam", 10) {
	}

	bool Init(void);
	bool GetTeamCount(int& count);
	bool GetNoUseTeamID(int& teamID);
	bool TeamSignUP(int& teamID, int captain, int member1, int member2);
	bool TeamCancel(int teamID);

	bool TeamUpdate(int teamID, int matchNo, int state, int winnum, int losenum, int relivenum);
	bool IsValidAmphitheaterTeam(int teamID, int captainID, int member1, int member2);
	bool IsLogin(int pActorID); //Add by sunny.sun20080714
	bool IsMapFull(int MapID, int& PActorIDNum);
	bool UpdateMapNum(int Teamid, int Mapid, int MapFlag);
	bool SetMaxBallotTeamRelive(void);
	bool SetMatchResult(int Teamid1, int Teamid2, int Id1state, int Id2state);
	bool GetMapFlag(int Teamid, int& Mapflag);
	bool GetCaptainByMapId(int Mapid, std::string& Captainid1, std::string& Captainid2);
	bool UpdateMap(int Mapid);

	bool GetPromotionAndReliveTeam(std::vector<std::vector<std::string>>& dataPromotion,
								   std::vector<std::vector<std::string>>& dataRelive);
	bool UpdatReliveNum(int ReID);
	bool UpdateAbsentTeamRelive(void);
	bool UpdateMapAfterEnter(int CaptainID, int MapID);
	bool UpdateWinnum(int teamid); //Add by sunnysun20080818
	bool GetUniqueMaxWinnum(int& teamid);
	bool SetMatchnoState(int teamid);
	bool UpdateState(void);
	bool CloseReliveByState(int& statenum);
	bool CleanMapFlag(int teamid1, int teamid2);
	bool GetStateByTeamid(int teamid, int& state);
};

// End

//Add by sunny.sun 20080822
//Begin
class CTablePersoninfo : public cfl_rs {
public:
	CTablePersoninfo(cfl_db* pDB)
		: cfl_rs(pDB, "personinfo", 10) {
	}

	bool Init(void);
	bool GetPersonBirthday(int chaid, int& birthday);
};

//End

// Resource — kitbag/bank хранилище (OdbcDatabase)
class CTableResource {
public:
	explicit CTableResource(OdbcDatabase& db) : _db(db) {
	}

	bool Create(long& lDBID, long lChaId, long lTypeId);
	bool ReadKitbagData(CCharacter* pCCha);
	bool SaveKitbagData(CCharacter* pCCha);
	bool ReadKitbagTmpData(CCharacter* pCCha);
	bool SaveKitbagTmpData(CCharacter* pCCha);
	bool ReadBankData(CPlayer* pCPly, char chBankNO = -1);
	bool SaveBankData(CPlayer* pCPly, char chBankNO = -1);

private:
	OdbcDatabase& _db;
};

// MapMask — миграция на OdbcDatabase
class CTableMapMask {
public:
	explicit CTableMapMask(OdbcDatabase& db) : _db(db) {
	}

	bool Create(long& lDBID, long lChaId);
	bool ReadData(CPlayer* pCPly);
	bool SaveData(CPlayer* pCPly, BOOL bDirect = FALSE);

	void HandleSaveList();
	void SaveAll();

	static bool GetColNameByMapName(const char* szMapName, std::string& colName);

private:
	OdbcDatabase& _db;
	std::list<std::string> _saveQueue;
};


class CTableBoat {
public:
	explicit CTableBoat(OdbcDatabase& db) : _db(db) {
	}

	BOOL Create(DWORD& dwBoatID, const BOAT_DATA& Data);
	BOOL GetBoat(CCharacter& Boat);
	BOOL SaveBoat(CCharacter& Boat, char chSaveType);
	BOOL SaveBoatTempData(CCharacter& Boat, BYTE byIsDeleted = 0);
	BOOL SaveBoatTempData(DWORD dwBoatID, DWORD dwOwnerID, BYTE byIsDeleted = 0);
	BOOL SaveBoatDelTag(DWORD dwBoatID, BYTE byIsDeleted = 0);

	bool SaveAllData(CPlayer* pPlayer, char chSaveType);
	bool ReadCabin(CCharacter& Boat);
	bool SaveCabin(CCharacter& Boat, char chSaveType);
	bool SaveAllCabin(CPlayer* pPlayer, char chSaveType);

private:
	OdbcDatabase& _db;
};

class CTableGuild {
	OdbcDatabase& _db;

	// Совместимые хелперы — cfl_rs API через OdbcDatabase (без try/catch — исключения пробрасываются)
	enum { MAX_COL = 64, MAX_DATALEN = 8192 };

	UCHAR _buf[MAX_COL][MAX_DATALEN]{};
	SDWORD _buf_len[MAX_COL]{};

	void handle_err(SQLHANDLE, SQLSMALLINT, RETCODE, const char* = nullptr, bool = false);

	std::string _tbl_name{"guild"};
	int _max_col{100};

	const char* _get_table() const;
	SQLRETURN exec_sql_direct(const char* sql);
	int get_affected_rows();
	bool _get_row(std::string buf[], int maxCol, const char* param, const char* filter, int* affect_rows = nullptr);
	bool _get_row3(std::string buf[], int maxCol, const char* param, const char* filter);
	bool begin_tran();
	bool commit_tran();
	bool rollback();
	bool getalldata(const char* sql, std::vector<std::vector<std::string>>& data);

public:
	explicit CTableGuild(OdbcDatabase& db) : _db(db) {
	}


	struct BankLog {
		short type;
		time_t time;
		unsigned long long parameter; // ItemID or Gold value
		short quantity; // 1-99 for items, 0 for gold;
		short userID; // chaID of the actor
	};

	//std::vector<BankLog> data;

	long Create(CCharacter* pCha, char* guildname, cChar* passwd);
	bool ListAll(CCharacter* pCha, char disband_days);
	void TryFor(CCharacter* pCha, uLong guildid);
	void TryForConfirm(CCharacter* pCha, uLong guildid);
	bool GetGuildBank(uLong guildid, CKitbag* bag);

	bool UpdateGuildBank(uLong guildid, CKitbag* bag);
	int GetGuildLeaderID(uLong guildid);

	bool SetGuildLog(std::vector<BankLog> log, uLong guildid);
	std::vector<BankLog> GetGuildLog(uLong guildid);


	bool UpdateGuildBankGold(int guildID, int money);
	unsigned long long GetGuildBankGold(uLong guildid);

	bool GetGuildInfo(CCharacter* pCha, uLong guildid);
	bool ListTryPlayer(CCharacter* pCha, char disband_days);
	bool Approve(CCharacter* pCha, uLong chaid);
	bool Reject(CCharacter* pCha, uLong chaid);
	bool Kick(CCharacter* pCha, uLong chaid);
	bool Leave(CCharacter* pCha);
	bool Disband(CCharacter* pCha, cChar* passwd);
	bool Motto(CCharacter* pCha, cChar* motto);
	bool GetGuildName(long lGuildID, std::string& strGuildName);

	//
	bool Challenge(CCharacter* pCha, BYTE byLevel, DWORD dwMoney);
	bool Leizhu(CCharacter* pCha, BYTE byLevel, DWORD dwMoney);
	void ListChallenge(CCharacter* pCha);
	bool GetChallInfo(BYTE byLevel, DWORD& dwGuildID1, DWORD& dwGuildID2, DWORD& dwMoney);
	bool StartChall(BYTE byLevel);
	bool HasCall(BYTE byLevel);
	void EndChall(DWORD dwGuild1, DWORD dwGuild2, BOOL bChall);
	void ChallMoney(BYTE byLevel, BOOL bChall, DWORD dwGuildID, DWORD dwChallID, DWORD dwMoney);
	bool ChallWin(BOOL bUpdate, BYTE byLevel, DWORD dwWinGuildID, DWORD dwFailerGuildID);
	bool HasGuildLevel(CCharacter* pChar, BYTE byLevel);
};

// Log
class CTableLog : public cfl_rs {
public:
	CTableLog(cfl_db* pDB)
		: cfl_rs(pDB, "gamelog", 10) {
	}
};

class CTableItem : public cfl_rs {
public:
	CTableItem(cfl_db* pDB)
		: cfl_rs(pDB, "property", 10) {
	}

	bool LockItem(SItemGrid* sig, int iChaId);
	bool UnlockItem(SItemGrid* sig, int iChaId);
};

class CGameDB {
public:
	CGameDB();
	~CGameDB();

	BOOL Init();

	OdbcTransaction BeginTransaction();

	// Совместимость со старым кодом
	bool BeginTran();
	bool RollBack();
	bool CommitTran();

	bool ReadPlayer(CPlayer* pPlayer, DWORD atorID);
	bool SavePlayer(CPlayer* pPlayer, char chSaveType);

	bool SavePlayerKitbag(CPlayer* pPlayer, char chSaveType = enumSAVE_TYPE_TRADE);
	bool SaveChaAssets(CCharacter* pCCha);

	// Лотерея — LotterySetting
	bool GetWinItemno(int issue, std::string& itemno);
	bool GetLotteryIssue(int& issue);
	bool AddIssue(int issue);
	bool DisuseIssue(int issue, int state);

	// Лотерея — Ticket
	bool LotteryIsExsit(int issue, char* itemno);
	bool AddLotteryTicket(CCharacter* pCCha, int issue, char itemno[6][2]);
	bool CalWinTicket(int issue, int max, std::string& itemno);

	// Амфитеатр
	bool IsValidAmphitheaterTeam(int teamID, int captainID, int member1, int member2);
	bool IsMasterRelation(int masterID, int prenticeID);

	// === AmphitheaterSetting ===
	bool GetAmphitheaterSeasonAndRound(int& season, int& round);
	bool AddAmphitheaterSeason(int season);
	bool DisuseAmphitheaterSeason(int season, int state, const char* winner);
	bool UpdateAmphitheaterRound(int season, int round);

	// === AmphitheaterTeam ===
	bool GetAmphitheaterTeamCount(int& count);
	bool GetAmphitheaterNoUseTeamID(int& teamID);
	bool AmphitheaterTeamSignUP(int& teamID, int captain, int member1, int member2);
	bool AmphitheaterTeamCancel(int teamID);
	bool IsAmphitheaterLogin(int pActorID);
	bool IsMapFull(int MapID, int& PActorIDNum);
	bool UpdateMapNum(int Teamid, int Mapid, int MapFlag);
	bool GetMapFlag(int Teamid, int& Mapflag);
	bool SetMaxBallotTeamRelive();
	bool SetMatchResult(int Teamid1, int Teamid2, int Id1state, int Id2state);
	bool GetCaptainByMapId(int Mapid, std::string& Captainid1, std::string& Captainid2);
	bool UpdateMap(int Mapid);
	bool UpdateMapAfterEnter(int CaptainID, int MapID);
	bool GetPromotionAndReliveTeam(std::vector<std::vector<std::string>>& dataPromotion,
								   std::vector<std::vector<std::string>>& dataRelive);
	bool UpdatReliveNum(int ReID);
	bool UpdateAbsentTeamRelive();
	bool UpdateWinnum(int teamid);
	bool GetUniqueMaxWinnum(int& teamid);
	bool SetMatchnoState(int teamid);
	bool UpdateState();
	bool CloseReliveByState(int& statenum);
	bool CleanMapFlag(int teamid1, int teamid2);
	bool GetStateByTeamid(int teamid, int& state);

	bool UpdateIMP(CPlayer* ply);
	bool SaveGmLv(CPlayer* ply);

	std::string GetChaNameByID(int cha_id);
	void ShowExpRank(CCharacter* pCha, int top);
	bool SavePlayerPos(CPlayer* pPlayer);
	bool SavePlayerKBagDBID(CPlayer* pPlayer);
	bool SavePlayerKBagTmpDBID(CPlayer* pPlayer);
	bool SavePlayerMMaskDBID(CPlayer* pPlayer);

	bool CreatePlyBank(CPlayer* pCPly);
	bool SavePlyBank(CPlayer* pCPly, char chBankNO = -1);
	unsigned long GetPlayerMasterDBID(CPlayer* pPlayer);

	BOOL AddCreditByDBID(DWORD atorID, long lCredit);
	BOOL SaveStoreItemID(DWORD atorID, long lStoreItemID);
	BOOL AddMoney(DWORD atorID, long money);

	BOOL ReadKitbagTmpData(DWORD res_id, std::string& strData);
	BOOL SaveKitbagTmpData(DWORD res_id, const std::string& strData);

	BOOL IsChaOnline(DWORD atorID, BOOL& bOnline);
	Long GetChaAddr(DWORD atorID);
	Long SetGuildPermission(int atorID, unsigned long perm, int guild_id);
	Long SetChaAddr(DWORD atorID, Long addr);

	BOOL SaveMissionData(CPlayer* pPlayer, DWORD atorID);

	// Лодки
	BOOL Create(DWORD& dwBoatID, const BOAT_DATA& Data);
	BOOL GetBoat(CCharacter& Boat);
	BOOL SaveBoat(CCharacter& Boat, char chSaveType);
	BOOL SaveBoatDelTag(DWORD dwBoatID, BYTE byIsDeleted = 0);
	BOOL SaveBoatTempData(CCharacter& Boat, BYTE byIsDeleted = 0);
	BOOL SaveBoatTempData(DWORD dwBoatID, DWORD dwOwnerID, BYTE byIsDeleted = 0);

	// Гильдии
	long CreateGuild(CCharacter* pCha, char* guildname, cChar* passwd);
	long GetGuildBank(uLong guildid, CKitbag* bag);
	long UpdateGuildBank(uLong guildid, CKitbag* bag);
	bool SetGuildLog(std::vector<CTableGuild::BankLog> log, uLong guildid);
	std::vector<CTableGuild::BankLog> GetGuildLog(uLong guildid);
	unsigned long long GetGuildBankGold(uLong guildid);
	bool UpdateGuildBankGold(int guildID, int money);
	int GetGuildLeaderID(uLong guildid);

	bool ListAllGuild(CCharacter* pCha, char disband_days = 1);
	void GuildTryFor(CCharacter* pCha, uLong guildid);
	void GuildTryForConfirm(CCharacter* pCha, uLong guildid);
	bool GuildListTryPlayer(CCharacter* pCha, char disband_days);
	bool GuildApprove(CCharacter* pCha, uLong chaid);
	bool GuildReject(CCharacter* pCha, uLong chaid);
	bool GuildKick(CCharacter* pCha, uLong chaid);
	bool GuildLeave(CCharacter* pCha);
	bool GuildDisband(CCharacter* pCha, cChar* passwd);
	bool GuildMotto(CCharacter* pCha, cChar* motto);

	CTableMapMask* GetMapMaskTable();

	bool GetGuildName(long lGuildID, std::string& strGuildName);
	bool Challenge(CCharacter* pCha, BYTE byLevel, DWORD dwMoney);
	bool Leizhu(CCharacter* pCha, BYTE byLevel, DWORD dwMoney);
	void ListChallenge(CCharacter* pCha);
	bool StartChall(BYTE byLevel);
	bool GetChall(BYTE byLevel, DWORD& dwGuildID1, DWORD& dwGuildID2, DWORD& dwMoney);
	void EndChall(DWORD dwGuild1, DWORD dwGuild2, BOOL bChall);
	bool HasChall(BYTE byLevel);
	bool HasGuildLevel(CCharacter* pChar, BYTE byLevel);

	// Логирование
	void ExecLogSQL(const char* pszSQL);
	void ExecTradeLogSQL(const char* gameServerName, const char* action,
						 const char* pszChaFrom, const char* pszChaTo, const char* pszTrade);

	BOOL m_bInitOK{false};

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
