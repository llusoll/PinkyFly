using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Lab4
{
    public enum BookingStatus { Confirmed, Paid, Canceled }

    public abstract class Booking : ICloneable
    {
        // Защищённые поля — доступны в потомках
        protected string bookingNumber;
        protected string flightNumber;
        protected string passengerId;
        protected string seatNumber;
        protected BookingStatus status;
        protected double basePrice;
        protected string bookingDate;
        protected double priceMultiplier = 1.0;

        // Статическое поле
        public static int TotalBookings { get; private set; }

        // Конструктор с вызовом из потомков
        protected Booking(string pnr, string fn, string pid, string seat, BookingStatus st, double basePr, string date)
        {
            bookingNumber = pnr;
            flightNumber = fn;
            passengerId = pid;
            seatNumber = seat;
            status = st;
            basePrice = basePr;
            bookingDate = date;
            TotalBookings++;
        }

        // === АБСТРАКТНЫЕ МЕТОДЫ ===
        public abstract double CalculatePrice();
        public abstract string GetDescription();

        // === ВИРТУАЛЬНЫЙ метод с реализацией ===
        public virtual void Cancel()
        {
            if (status == BookingStatus.Paid)
                throw new InvalidOperationException("Нельзя отменить оплаченное бронирование без возврата.");
            status = BookingStatus.Canceled;
        }

        // === СВОЙСТВА (публичные геттеры) ===
        public string BookingNumber => bookingNumber;
        public string FlightNumber => flightNumber;
        public string PassengerId => passengerId;
        public string SeatNumber => seatNumber;
        public BookingStatus Status => status;
        public double BasePrice => basePrice;          // <-- ДОБАВЛЕНО для безопасного доступа извне
        public string BookingDate => bookingDate;      // <-- ДОБАВЛЕНО

        // === КЛОНИРОВАНИЕ ===
        public object Clone() => MemberwiseClone();  // поверхностное
        public abstract Booking DeepClone();         // глубокое — реализуем в потомках

        // === СТАТИЧЕСКИЕ МЕТОДЫ ===
        public static string StatusToString(BookingStatus s) =>
            s switch
            {
                BookingStatus.Confirmed => "Confirmed",
                BookingStatus.Paid => "Paid",
                BookingStatus.Canceled => "Canceled",
                _ => "Confirmed"
            };

        public static BookingStatus StringToStatus(string s) =>
            s switch
            {
                "Confirmed" => BookingStatus.Confirmed,
                "Paid" => BookingStatus.Paid,
                "Canceled" => BookingStatus.Canceled,
                _ => BookingStatus.Confirmed
            };

        // === ЗАГРУЗКА / СОХРАНЕНИЕ ===
        public static List<Booking> LoadAll()
        {
            var bookings = new List<Booking>();
            if (!File.Exists("data/bookings.txt")) return bookings;

            foreach (var line in File.ReadAllLines("data/bookings.txt"))
            {
                if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;
                var parts = Utils.Split(line, ';');
                if (parts.Count != 8) continue;

                try
                {
                    string type = parts[0];
                    var pnr = parts[1];
                    var fn = parts[2];
                    var pid = parts[3];
                    var seat = parts[4];
                    var status = StringToStatus(parts[5]);
                    var basePrice = double.Parse(parts[6]);
                    var date = parts[7];

                    if (type == "ECONOMY")
                        bookings.Add(new EconomyBooking(pnr, fn, pid, seat, status, basePrice, date));
                    else if (type == "BUSINESS")
                        bookings.
Add(new BusinessBooking(pnr, fn, pid, seat, status, basePrice, date));
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Пропущено: {ex.Message}");
                }
            }
            return bookings;
        }

        public static void SaveAll(List<Booking> bookings)
        {
            Utils.EnsureDataDir();
            using var writer = new StreamWriter("data/bookings.txt", false);
            writer.WriteLine("# Type;PNR;Flight;Passenger;Seat;Status;BasePrice;Date");
            foreach (var b in bookings)
            {
                string type = b is EconomyBooking ? "ECONOMY" : "BUSINESS";
                writer.WriteLine($"{type};{b.BookingNumber};{b.FlightNumber};{b.PassengerId};{b.SeatNumber};" +
                                 $"{StatusToString(b.Status)};{b.BasePrice};{b.BookingDate}");
            }
        }

        // === ВЫВОД ТАБЛИЦЫ ===
        public static void DisplayTable(List<Booking> list)
        {
            if (list == null || list.Count == 0)
            {
                Console.WriteLine("Нет данных.");
                return;
            }
            Console.WriteLine("\n=== Бронирования ===");
            int pnrWidth = 10, flightWidth = 12, passengerWidth = 14, seatWidth = 10, statusWidth = 12;
            Console.WriteLine($"{LeftPad("PNR", pnrWidth)}" +
                              $"{LeftPad("Рейс", flightWidth)}" +
                              $"{LeftPad("Пассажир", passengerWidth)}" +
                              $"{LeftPad("Место", seatWidth)}" +
                              $"{LeftPad("Статус", statusWidth)}");
            Console.WriteLine(new string('-', pnrWidth + flightWidth + passengerWidth + seatWidth + statusWidth + 4));
            foreach (var b in list)
            {
                Console.WriteLine($"{LeftPad(b.BookingNumber, pnrWidth)}" +
                                  $"{LeftPad(b.FlightNumber, flightWidth)}" +
                                  $"{LeftPad(b.PassengerId, passengerWidth)}" +
                                  $"{LeftPad(b.SeatNumber, seatWidth)}" +
                                  $"{LeftPad(StatusToString(b.Status), statusWidth)}");
            }
        }

        private static string LeftPad(string text, int width)
        {
            if (text == null) text = "";
            return text.Length > width ? text.Substring(0, width) : text.PadRight(width);
        }
    }
}
