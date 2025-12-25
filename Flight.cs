using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Lab4
{

    public enum FlightStatus
    {
        Scheduled,
        Departed,
        Delayed,
        Arrived,
        Canceled
    }

    public class Flight
    {
        public string FlightNumber { get; set; }
        public string AircraftTail { get; set; }
        public string DepartureAirport { get; set; }
        public string ArrivalAirport { get; set; }
        public string ScheduledDeparture { get; set; }
        public string ScheduledArrival { get; set; }
        public string ActualDeparture { get; set; }
        public string ActualArrival { get; set; }
        public FlightStatus Status { get; set; }
        public double BasePriceEconomy { get; set; }
        public double BasePriceBusiness { get; set; }

        public Flight() { }

        public Flight(string fn, string tail, string dep, string arr, string schedDep, string schedArr, double eco, double biz)
        {
            FlightNumber = fn;
            AircraftTail = tail;
            DepartureAirport = dep;
            ArrivalAirport = arr;
            ScheduledDeparture = schedDep;
            ScheduledArrival = schedArr;
            ActualDeparture = "";
            ActualArrival = "";
            Status = FlightStatus.Scheduled;
            BasePriceEconomy = eco;
            BasePriceBusiness = biz;
        }

        public static string StatusToString(FlightStatus s)
        {
            return s switch
            {
                FlightStatus.Scheduled => "Scheduled",
                FlightStatus.Departed => "Departed",
                FlightStatus.Delayed => "Delayed",
                FlightStatus.Arrived => "Arrived",
                FlightStatus.Canceled => "Canceled",
                _ => "Scheduled"
            };
        }

        public static FlightStatus StringToStatus(string s)
        {
            return s switch
            {
                "Scheduled" => FlightStatus.Scheduled,
                "Departed" => FlightStatus.Departed,
                "Delayed" => FlightStatus.Delayed,
                "Arrived" => FlightStatus.Arrived,
                "Canceled" => FlightStatus.Canceled,
                _ => FlightStatus.Scheduled
            };
        }

        public void DelayFlight(string newTime)
        {
            ScheduledDeparture = newTime;
            Status = FlightStatus.Delayed;
        }

        public void UpdateStatus(FlightStatus st)
        {
            Status = st;
        }

        public int GetAvailableSeats(List<Booking> bookings)
        {
            const int total = 180;
            int booked = bookings.Count(b => b.FlightNumber == FlightNumber);
            return total - booked;
        }

        public static List<Flight> LoadAll()
        {
            var flights = new List<Flight>();
            if (!File.Exists("data/flights.txt"))
                return flights;

            foreach (var line in File.ReadAllLines("data/flights.txt"))
            {
                if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;

                var parts = Utils.Split(line, ';');
                if (parts.Count < 11) continue;

                var fl = new Flight
                {
                    FlightNumber = parts[0],
                    AircraftTail = parts[1],
                    DepartureAirport = parts[2],
                    ArrivalAirport = parts[3],
                    ScheduledDeparture = parts[4],
                    ScheduledArrival = parts[5],
                    ActualDeparture = parts[6],
                    ActualArrival = parts[7],
                    Status = StringToStatus(parts[8]),
                    BasePriceEconomy = double.Parse(parts[9]),
                    BasePriceBusiness = double.Parse(parts[10])
                };
                flights.Add(fl);
            }

            return flights;
        }

        public static void SaveAll(List<Flight> flights)
        {
            Utils.EnsureDataDir();
            using var writer = new StreamWriter("data/flights.txt", false);
            writer.WriteLine("# Flight;Aircraft;Dep;Arr;SchedDep;SchedArr;ActDep;ActArr;Status;EcoPrice;BizPrice");
            foreach (var fl in flights)
            {
                writer.WriteLine($"{fl.FlightNumber};{fl.AircraftTail};{fl.DepartureAirport};{fl.ArrivalAirport};" +
                                 $"{fl.ScheduledDeparture};{fl.
    ScheduledArrival};{fl.ActualDeparture};{fl.ActualArrival};" +
                                 $"{StatusToString(fl.Status)};{fl.BasePriceEconomy};{fl.BasePriceBusiness}");
            }
        }

        // Таблица рейсов
        public static void DisplayTable(List<Flight> list)
        {
            if (list == null || list.Count == 0)
            {
                Console.WriteLine("Нет данных.");
                return;
            }

            Console.WriteLine("\n=== Рейсы ===");

            int flightWidth = 12;
            int routeWidth = 18;
            int timeWidth = 20;
            int statusWidth = 12;

            // Заголовок — первая строка 
            Console.WriteLine(
                $"{LeftPad("Рейс", flightWidth)} " +
                $"{LeftPad("Маршрут", routeWidth)} " +
                $"{LeftPad("Вылет (расп.)", timeWidth)} " +
                $"{LeftPad("Статус", statusWidth)}"
            );

            Console.WriteLine(new string('-', flightWidth + routeWidth + timeWidth + statusWidth + 3));

            foreach (var f in list)
            {
                string route = $"{f.DepartureAirport}→{f.ArrivalAirport}";
                Console.WriteLine(
                    $"{LeftPad(f.FlightNumber, flightWidth)} " +
                    $"{LeftPad(route, routeWidth)} " +
                    $"{LeftPad(f.ScheduledDeparture, timeWidth)} " +
                    $"{LeftPad(StatusToString(f.Status), statusWidth)}"
                );
            }
        }

        private static string LeftPad(string text, int width)
        {
            if (text == null) text = "";
            return text.Length > width ? text.Substring(0, width) : text.PadRight(width);
        }
    }
}