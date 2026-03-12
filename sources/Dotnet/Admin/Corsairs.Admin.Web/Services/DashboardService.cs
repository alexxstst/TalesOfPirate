using Corsairs.Platform.Grpc.Contracts.Account;
using Corsairs.Platform.Grpc.Contracts.Gate;
using Corsairs.Platform.Grpc.Contracts.Group;
using Corsairs.Platform.Grpc.Contracts.Common;

namespace Corsairs.Admin.Web.Services;

public class DashboardService(
    AccountAdmin.AccountAdminClient accountClient,
    GateAdmin.GateAdminClient gateClient,
    GroupAdmin.GroupAdminClient groupClient)
{
    public record AllStatuses(
        ServerStatusResponse? Account,
        ServerStatusResponse? Gate,
        ServerStatusResponse? Group);

    public async Task<AllStatuses> GetAllStatuses()
    {
        var empty = new Empty();
        ServerStatusResponse? account = null, gate = null, group = null;

        var tasks = new List<Task>
        {
            Task.Run(async () => { try { account = await accountClient.GetServerStatusAsync(empty); } catch { } }),
            Task.Run(async () => { try { gate = await gateClient.GetServerStatusAsync(empty); } catch { } }),
            Task.Run(async () => { try { group = await groupClient.GetServerStatusAsync(empty); } catch { } })
        };

        await Task.WhenAll(tasks);
        return new AllStatuses(account, gate, group);
    }
}
