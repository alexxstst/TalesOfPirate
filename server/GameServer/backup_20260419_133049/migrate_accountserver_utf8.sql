-- Полная миграция AccountServer на UTF-8 collation.

SET NOCOUNT ON;
SET XACT_ABORT ON;
BEGIN TRAN;

PRINT '-- PHASE 1: DROP indexes/PK/UK';
ALTER TABLE [dbo].[account_login] DROP CONSTRAINT [IX_account_login];
ALTER TABLE [dbo].[account_login] DROP CONSTRAINT [IX_account_login_name];
ALTER TABLE [dbo].[LogRandomAccount] DROP CONSTRAINT [PK_LogRandomAccount];

PRINT '-- PHASE 2: ALTER COLUMNS';
ALTER TABLE [dbo].[account_details] ALTER COLUMN [squestion] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_details] ALTER COLUMN [answer] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_details] ALTER COLUMN [email] varchar(400) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_details] ALTER COLUMN [truename] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_details] ALTER COLUMN [contact] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_details] ALTER COLUMN [country] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_details] ALTER COLUMN [ipaddr] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_details] ALTER COLUMN [ip2country] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_details] ALTER COLUMN [actcode] varchar(80) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_details] ALTER COLUMN [invtcode] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_invt] ALTER COLUMN [invt_code] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_invt] ALTER COLUMN [assignto] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_login] ALTER COLUMN [name] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_login] ALTER COLUMN [password] varchar(1020) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_login] ALTER COLUMN [salt] varchar(1020) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NULL;
ALTER TABLE [dbo].[account_login] ALTER COLUMN [login_group] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_login] ALTER COLUMN [last_login_ip] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_login] ALTER COLUMN [last_login_mac] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_login] ALTER COLUMN [email] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [name] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [password] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [squestion] varchar(400) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [answer] varchar(400) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [email] varchar(400) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [truename] varchar(400) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [contact] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [country] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [ipaddr] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [ip2country] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [actcode] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[account_pending] ALTER COLUMN [invtcode] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[LogRandomAccount] ALTER COLUMN [accountName] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[LogRandomAccount] ALTER COLUMN [ip] varchar(60) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[LogRandomAccount] ALTER COLUMN [plainPassword] varchar(128) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[server_group] ALTER COLUMN [group_name] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[server_list] ALTER COLUMN [server_name] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[TradeRecord] ALTER COLUMN [record_id] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[user_log] ALTER COLUMN [user_name] varchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;
ALTER TABLE [dbo].[user_log] ALTER COLUMN [login_ip] varchar(80) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NOT NULL;

PRINT '-- PHASE 3: CREATE indexes/PK/UK';
ALTER TABLE [dbo].[account_login] ADD CONSTRAINT [IX_account_login] UNIQUE NONCLUSTERED ([name]);
ALTER TABLE [dbo].[account_login] ADD CONSTRAINT [IX_account_login_name] UNIQUE NONCLUSTERED ([name]);
ALTER TABLE [dbo].[LogRandomAccount] ADD CONSTRAINT [PK_LogRandomAccount] PRIMARY KEY CLUSTERED ([accountName]);

COMMIT;
PRINT 'DONE';
