using System;
using System.Collections.Generic;
using System.IO;

namespace Lab4
{

    public enum AircraftStatus
    {
        InService,
        Maintenance,
        Retired
    }

    public class Aircraft
    {
        public string TailNumber { get; set; }
        public string Model { get; set; }
        public int TotalSeats { get; set; }
        public int BusinessSeats { get; set; }
        public int EconomySeats { get; set; }
        public AircraftStatus Status { get; set; }
        public string MaintenanceSchedule { get; set; } // формат: YYYY-MM-DD
        public double FlightHours { get; set; }

        public Aircraft() { }

        public Aircraft(string tailNumber, string model, int totalSeats, int businessSeats, int economySeats)
        {
            TailNumber = tailNumber;
            Model = model;
            TotalSeats = totalSeats;
            BusinessSeats = businessSeats;
            EconomySeats = economySeats;
            Status = AircraftStatus.InService;
            FlightHours = 0.0;
        }

        public static string StatusToString(AircraftStatus status)
        {
            return status switch
            {
                AircraftStatus.InService => "InService",
                AircraftStatus.Maintenance => "Maintenance",
                AircraftStatus.Retired => "Retired",
                _ => "InService"
            };
        }

        public static AircraftStatus StringToStatus(string statusStr)
        {
            return statusStr switch
            {
                "InService" => AircraftStatus.InService,
                "Maintenance" => AircraftStatus.Maintenance,
                "Retired" => AircraftStatus.Retired,
                _ => AircraftStatus.InService
            };
        }

        public void ScheduleMaintenance(string date)
        {
            MaintenanceSchedule = date;
        }

        public void UpdateFlightHours(double hours)
        {
            FlightHours += hours;
        }

        public void ChangeStatus(AircraftStatus status)
        {
            Status = status;
        }

        public static List<Aircraft> LoadAll()
        {
            var aircrafts = new List<Aircraft>();
            if (!File.Exists("data/aircrafts.txt"))
                return aircrafts;

            foreach (var line in File.ReadAllLines("data/aircrafts.txt"))
            {
                if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;

                var parts = Utils.Split(line, ';');
                if (parts.Count != 8) continue;

                var ac = new Aircraft
                {
                    TailNumber = parts[0],
                    Model = parts[1],
                    TotalSeats = int.Parse(parts[2]),
                    BusinessSeats = int.Parse(parts[3]),
                    EconomySeats = int.Parse(parts[4]),
                    Status = StringToStatus(parts[5]),
                    MaintenanceSchedule = parts[6],
                    FlightHours = double.Parse(parts[7])
                };
                aircrafts.Add(ac);
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
            using var writer = new StreamWriter("data/aircrafts.txt", false);
            writer.WriteLine("# Tail;Model;Total;Business;Economy;Status;Maintenance;FlightHours");
            foreach (var ac in aircrafts)
            {
                writer.WriteLine($"{ac.TailNumber};{ac.Model};{ac.TotalSeats};{ac.BusinessSeats};{ac.EconomySeats};" +
                                 $"{StatusToString(ac.Status)};{ac.MaintenanceSchedule};{ac.FlightHours}");
            }
        }
        public static void DisplayTable(List<Aircraft> list)
        {
            if (list == null || list.Count == 0)
            {
                Console.WriteLine("Нет данных.");
                return;
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
                $"{LeftPad("Всего", totalWidth)} " +
                $"{LeftPad("Бизнес", bizWidth)} " +
                $"{LeftPad("Статус", statusWidth)} " +
                $"{LeftPad("Налёт", hoursWidth)}"
            );

            Console.WriteLine(new string('-', tailWidth + modelWidth + totalWidth + bizWidth + statusWidth + hoursWidth + 5));

            foreach (var a in list)
            {
                Console.WriteLine(
                    $"{LeftPad(a.TailNumber, tailWidth)} " +
                    $"{LeftPad(a.Model, modelWidth)} " +
                    $"{LeftPad(a.TotalSeats.ToString(), totalWidth)} " +
                    $"{LeftPad(a.BusinessSeats.ToString(), bizWidth)} " +
                    $"{LeftPad(StatusToString(a.Status), statusWidth)} " +
                    $"{LeftPad(a.FlightHours.ToString("F1"), hoursWidth)}"
                );
            }
        }
    }
}