using System;
using System.Collections.Generic;
using System.IO;

namespace Lab4
{
    public enum EmployeeRole
    {
        FlightAttendant,
        Dispatcher,
        Admin
    }

    public class Employee
    {
        public string EmployeeId { get; set; }
        public string FirstName { get; set; }
        public string LastName { get; set; }
        public EmployeeRole Role { get; set; }
        public string Login { get; set; }
        public string PasswordHash { get; set; }

        public Employee() { }

        public Employee(string id, string fn, string ln, EmployeeRole role, string login, string password)
        {
            EmployeeId = id;
            FirstName = fn;
            LastName = ln;
            Role = role;
            Login = login;
            PasswordHash = Utils.SimpleHash(password);
        }

        public static string RoleToString(EmployeeRole r)
        {
            return r switch
            {
                EmployeeRole.Admin => "Admin",
                EmployeeRole.Dispatcher => "Dispatcher",
                EmployeeRole.FlightAttendant => "FlightAttendant",
                _ => "FlightAttendant"
            };
        }

        public static EmployeeRole StringToRole(string s)
        {
            return s switch
            {
                "Admin" => EmployeeRole.Admin,
                "Dispatcher" => EmployeeRole.Dispatcher,
                "FlightAttendant" => EmployeeRole.FlightAttendant,
                _ => EmployeeRole.FlightAttendant
            };
        }

        public bool Authenticate(string password)
        {
            return Utils.SimpleHash(password) == PasswordHash;
        }

        public static List<Employee> LoadAll()
        {
            var employees = new List<Employee>();
            if (!File.Exists("data/employees.txt"))
                return employees;

            foreach (var line in File.ReadAllLines("data/employees.txt"))
            {
                if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;

                var parts = Utils.Split(line, ';');
                if (parts.Count != 6) continue;

                employees.Add(new Employee
                {
                    EmployeeId = parts[0],
                    FirstName = parts[1],
                    LastName = parts[2],
                    Role = StringToRole(parts[3]),
                    Login = parts[4],
                    PasswordHash = parts[5]
                });
            }

            return employees;
        }

        public static void SaveAll(List<Employee> employees)
        {
            Utils.EnsureDataDir();
            using var writer = new StreamWriter("data/employees.txt", false);
            writer.WriteLine("# ID;FirstName;LastName;Role;Login;PasswordHash");
            foreach (var e in employees)
            {
                writer.WriteLine($"{e.EmployeeId};{e.FirstName};{e.LastName};" +
                                 $"{RoleToString(e.Role)};{e.Login};{e.PasswordHash}");
            }
        }

        // Таблица сотрудников
        public static void DisplayTable(List<Employee> list)
        {
            if (list == null || list.Count == 0)
            {
                Console.WriteLine("Нет данных.");
                return;
            }

            Console.WriteLine("\n=== Сотрудники ===");

            int idWidth = 10;
            int nameWidth = 20;
            int roleWidth = 18;
            int loginWidth = 12;

            // Заголовок — первая строка
            Console.WriteLine(
                $"{LeftPad("ID", idWidth)} " +
                $"{LeftPad("ФИО", nameWidth)} " +
                $"{LeftPad("Роль", roleWidth)} " +
                $"{LeftPad("Логин", loginWidth)}"
            );

            Console.WriteLine(new string('-', idWidth + nameWidth + roleWidth + loginWidth + 3));

            foreach (var e in list)
            {
                string fullName = $"{e.LastName} {e.FirstName}";
                Console.WriteLine(
                    $"{LeftPad(e.EmployeeId, idWidth)} " +
                    $"{LeftPad(fullName, nameWidth)} " +
                    $"{LeftPad(RoleToString(e.Role), roleWidth)} " +
                    $"{LeftPad(e.Login, loginWidth)}"
                );
            }
        }

        private static string LeftPad(string text, int width)
        {
            if (text == null) text = "";
            return text.Length > width ? text.Substring(0, width) : text.
    PadRight(width);
        }
    }
}
