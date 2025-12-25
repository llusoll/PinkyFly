amespace Lab4
{
    public enum AircraftStatus
{
    InService,
	@@ -14,13 +13,16 @@ public enum AircraftStatus

    public class Aircraft
{
    // Статическое поле — для подсчёта общего числа ВС
    public static int TotalCount { get; private set; } = 0;

    public string TailNumber { get; set; }
    public string Model { get; set; }
    public int TotalSeats { get; set; }
    public int BusinessSeats { get; set; }
    public int EconomySeats { get; set; }
    public AircraftStatus Status { get; set; }
    public string MaintenanceSchedule { get; set; }
    public double FlightHours { get; set; }

    public Aircraft() { }
public Aircraft(string tailNumber, string model, int totalSeats, int businessSea
            EconomySeats = economySeats;
    Status = AircraftStatus.InService;
            FlightHours = 0.0;
            TotalCount++; // инкремент статического счётчика
        }

public static string StatusToString(AircraftStatus status)
	 public static List<Aircraft> LoadAll()
            foreach (var line in File.ReadAllLines("data/aircrafts.txt"))
{
    if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;
    var parts = Utils.Split(line, ';');
    if (parts.Count != 8) continue;

	public static List<Aircraft> LoadAll()
                    MaintenanceSchedule = parts[6],
                    FlightHours = double.Parse(parts[7])
                };
                // TotalCount не инкрементируем здесь — только в конструкторе при создании новых
            }
            return aircrafts;
        }

        private static string LeftPad(string text, int width)
{
    if (text == null) text = "";
    return text.Length > width ? text.Substring(0, width) : text.PadRight(width);
}

public static void SaveAll(List<Aircraft> aircrafts)
{
    Utils.EnsureDataDir();
	@@ -117,9 + 118,10 @@ public static void SaveAll(List<Aircraft> aircrafts)
            foreach (var ac in aircrafts)
{
    writer.WriteLine($"{ac.TailNumber};{ac.Model};{ac.TotalSeats};{ac.BusinessSeats};{ac.EconomySeats};" +
$"{StatusToString(ac.Status)};{ac.MaintenanceSchedule};{ac.FlightHours}");
}
        }

        public static void DisplayTable(List<Aircraft> list)
{
    if (list == null || list.Count == 0)
	public static void DisplayTable(List<Aircraft> list)
            }

            Console.WriteLine("\n=== Воздушные суда ===");
int tailWidth = 12;
int modelWidth = 18;
int totalWidth = 8;
int bizWidth = 10;
int statusWidth = 14;
int hoursWidth = 12;

Console.WriteLine(
    $"{LeftPad("Борт", tailWidth)} " +
    $"{LeftPad("Модель", modelWidth)} " +
