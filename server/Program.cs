
var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
// Learn more about configuring OpenAPI at https://aka.ms/aspnet/openapi
builder.Services.AddOpenApi();

// Configure Kestrel to listen on port 5050 on all network interfaces
builder.WebHost.ConfigureKestrel(serverOptions =>
{
    serverOptions.ListenAnyIP(5050); // this allows 0.0.0.0:5050
});

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.MapOpenApi();
}

app.UseHttpsRedirection();

var group = app.MapGroup("/alert");



group.MapGet("/", () =>
{
    var forecast =  Enumerable.Range(1, 5).Select(index =>
        new WeatherForecast
        (
            "hi","hi","hi"
        ))
        .ToArray();
    return forecast;
});

int i = 0;

group.MapPost("/create", ()=>{
    i += 1;
    Console.WriteLine("Get request from hardware: " + i.ToString());
    return Results.Ok("Printed successfully!");
});

app.Run();

record WeatherForecast(string Date, string TemperatureC, string Summary);
