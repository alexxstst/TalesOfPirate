using Corsairs.Admin.Web.Services;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddRazorComponents()
    .AddInteractiveServerComponents();

// gRPC клиенты
var config = builder.Configuration.GetSection("Servers");
builder.Services.AddSingleton(sp =>
{
    var accountUrl = config["AccountServer"] ?? "http://localhost:15000";
    var channel = Grpc.Net.Client.GrpcChannel.ForAddress(accountUrl);
    return new Corsairs.Platform.Grpc.Contracts.Account.AccountAdmin.AccountAdminClient(channel);
});
builder.Services.AddSingleton(sp =>
{
    var gateUrl = config["GateServer"] ?? "http://localhost:15001";
    var channel = Grpc.Net.Client.GrpcChannel.ForAddress(gateUrl);
    return new Corsairs.Platform.Grpc.Contracts.Gate.GateAdmin.GateAdminClient(channel);
});
builder.Services.AddSingleton(sp =>
{
    var groupUrl = config["GroupServer"] ?? "http://localhost:15002";
    var channel = Grpc.Net.Client.GrpcChannel.ForAddress(groupUrl);
    return new Corsairs.Platform.Grpc.Contracts.Group.GroupAdmin.GroupAdminClient(channel);
});

builder.Services.AddScoped<DashboardService>();

var app = builder.Build();

if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Error");
}

app.UseStaticFiles();
app.UseAntiforgery();

app.MapRazorComponents<Corsairs.Admin.Web.Components.App>()
    .AddInteractiveServerRenderMode();

app.Run();
