using System;
using System.Collections.Generic;
using System.IO;

namespace Lab4
{

    public class Passenger
    {
        public string PassengerId { get; set; }
        public string FirstName { get; set; }
        public string LastName { get; set; }
        public string DateOfBirth { get; set; }
        public string PassportNumber { get; set; }
        public string ContactPhone { get; set; }
        public string Email { get; set; }

        public Passenger() { }

        public Passenger(string id, string fn, string ln, string dob, string passport, string phone, string email)
        {
            PassengerId = id;
            FirstName = fn;
            LastName = ln;
            DateOfBirth = dob;
            PassportNumber = passport;
            ContactPhone = phone;
            Email = email;
        }

        public void UpdateContactInfo(string phone, string email)
        {
            ContactPhone = phone;
            Email = email;
        }

        public bool ValidatePassport()
        {
            return PassportNumber != null && PassportNumber.Length >= 6 && PassportNumber.Length <= 15;
        }

        public static List<Passenger> LoadAll()
        {
            var passengers = new List<Passenger>();
            if (!File.Exists("data/passengers.txt"))
                return passengers;

            foreach (var line in File.ReadAllLines("data/passengers.txt"))
            {
                if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;

                var parts = Utils.Split(line, ';');
                if (parts.Count != 7) continue;

                passengers.Add(new Passenger(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6]));
            }

            return passengers;
        }

        public static void SaveAll(List<Passenger> passengers)
        {
            Utils.EnsureDataDir();
            using var writer = new StreamWriter("data/passengers.txt", false);
            writer.WriteLine("# ID;FirstName;LastName;DOB;Passport;Phone;Email");
            foreach (var p in passengers)
            {
                writer.WriteLine($"{p.PassengerId};{p.FirstName};{p.LastName};{p.DateOfBirth};" +
                                 $"{p.PassportNumber};{p.ContactPhone};{p.Email}");
            }
        }

        // Таблица пассажиров
        public static void DisplayTable(List<Passenger> list)
        {
            if (list == null || list.Count == 0)
            {
                Console.WriteLine("Нет данных.");
                return;
            }

            Console.WriteLine("\n=== Пассажиры ===");

            int passportWidth = 16;
            int nameWidth = 20;
            int phoneWidth = 14;
            int emailWidth = 20;

            // Заголовок — первая строка 
            Console.WriteLine(
                $"{LeftPad("Паспорт", passportWidth)} " +
                $"{LeftPad("ФИО", nameWidth)} " +
                $"{LeftPad("Телефон", phoneWidth)} " +
                $"{LeftPad("Email", emailWidth)}"
            );

            Console.WriteLine(new string('-', passportWidth + nameWidth + phoneWidth + emailWidth + 3));

            foreach (var p in list)
            {
                string fullName = $"{p.LastName} {p.FirstName}";
                Console.WriteLine(
                    $"{LeftPad(p.PassportNumber, passportWidth)} " +
                    $"{LeftPad(fullName, nameWidth)} " +
                    $"{LeftPad(p.ContactPhone, phoneWidth)} " +
                    $"{LeftPad(p.Email, emailWidth)}"
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
