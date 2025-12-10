using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Lab4
{

    public enum BookingStatus
    {
        Confirmed,
        Paid,
        Canceled
    }

    public class Booking
    {
        public string BookingNumber { get; set; }
        public string FlightNumber { get; set; }
        public string PassengerId { get; set; }
        public string SeatNumber { get; set; }
        public BookingStatus Status { get; set; }
        public double Price { get; set; }
        public string BookingDate { get; set; }

        public Booking() { }

        public Booking(string pnr, string fn, string pid, string seat, BookingStatus st, double pr, string date)
        {
            BookingNumber = pnr;
            FlightNumber = fn;
            PassengerId = pid;
            SeatNumber = seat;
            Status = st;
            Price = pr;
            BookingDate = date;
        }

        public static string StatusToString(BookingStatus s)
        {
            return s switch
            {
                BookingStatus.Confirmed => "Confirmed",
                BookingStatus.Paid => "Paid",
                BookingStatus.Canceled => "Canceled",
                _ => "Confirmed"
            };
        }

        public static BookingStatus StringToStatus(string s)
        {
            return s switch
            {
                "Confirmed" => BookingStatus.Confirmed,
                "Paid" => BookingStatus.Paid,
                "Canceled" => BookingStatus.Canceled,
                _ => BookingStatus.Confirmed
            };
        }

        public void CancelBooking()
        {
            Status = BookingStatus.Canceled;
        }

        public void ConfirmPayment()
        {
            Status = BookingStatus.Paid;
        }

        public void ChangeSeat(string newSeat)
        {
            SeatNumber = newSeat;
        }

        public static List<Booking> LoadAll()
        {
            var bookings = new List<Booking>();
            if (!File.Exists("data/bookings.txt"))
                return bookings;

            foreach (var line in File.ReadAllLines("data/bookings.txt"))
            {
                if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;

                var parts = Utils.Split(line, ';');
                if (parts.Count != 7) continue;

                var bk = new Booking
                {
                    BookingNumber = parts[0],
                    FlightNumber = parts[1],
                    PassengerId = parts[2],
                    SeatNumber = parts[3],
                    Status = StringToStatus(parts[4]),
                    Price = double.Parse(parts[5]),
                    BookingDate = parts[6]
                };
                bookings.Add(bk);
            }

            return bookings;
        }

        public static void SaveAll(List<Booking> bookings)
        {
            Utils.EnsureDataDir();
            using var writer = new StreamWriter("data/bookings.txt", false);
            writer.WriteLine("# PNR;Flight;Passenger;Seat;Status;Price;Date");
            foreach (var b in bookings)
            {
                writer.WriteLine($"{b.BookingNumber};{b.FlightNumber};{b.PassengerId};{b.SeatNumber};" +
                                 $"{StatusToString(b.Status)};{b.Price};{b.BookingDate}");
            }
        }


        public static void DisplayTable(List<Booking> list)
        {
            if (list == null || list.Count == 0)
            {
                Console.WriteLine("Нет данных.");
                return;
            }

            Console.WriteLine("\n=== Бронирования ===");

            // Ширины колонок 
            int pnrWidth = 10;
            int flightWidth = 12;
            int passengerWidth = 14;
            int seatWidth = 10;
            int statusWidth = 12;

            // Заголовок — первая строка таблицы 
            Console.WriteLine(
                $"{LeftPad("PNR", pnrWidth)} " +
                $"{LeftPad("Рейс", flightWidth)} " +
                $"{LeftPad("Пассажир", passengerWidth)} " +
                $"{LeftPad("Место", seatWidth)} " +
                $"{LeftPad("Статус", statusWidth)}"
            );

            // Горизонтальная линия (опционально, для наглядности)
            Console.WriteLine(new string('-', pnrWidth + flightWidth + passengerWidth + seatWidth + statusWidth + 4));

            // Данные
            foreach (var b in list)
            {
                Console.WriteLine(
                    $"{LeftPad(b.BookingNumber, pnrWidth)} " +
                    $"{LeftPad(b.FlightNumber, flightWidth)} " +
                    $"{LeftPad(b.PassengerId, passengerWidth)} " +
                    $"{LeftPad(b.SeatNumber, seatWidth)} " +
                    $"{LeftPad(StatusToString(b.Status), statusWidth)}"
                );
            }
        }

        // Вспомогательный метод для выравнивания текста слева (аналог setw + left в C++)
        private static string LeftPad(string text, int width)
        {
            if (text == null) text = "";
            return text.Length > width ? text.Substring(0, width) : text.PadRight(width);
        }
    }
}
