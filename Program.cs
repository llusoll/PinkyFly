using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Lab4
{
    public class Program
    {
        static List<Aircraft> aircrafts = new List<Aircraft>();
        static List<Flight> flights = new List<Flight>();
        static List<Passenger> passengers = new List<Passenger>();
        static List<Booking> bookings = new List<Booking>();
        static List<Employee> employees = new List<Employee>();
        static List<Airport> airports = new List<Airport>();

        public static void Main(string[] args)
        {
            Console.InputEncoding = System.Text.Encoding.UTF8;
            Console.OutputEncoding = System.Text.Encoding.UTF8;

            Utils.EnsureDataDir();

            LoadAllData();
            InitializeDefaultData();

            Employee currentUser = Authenticate();
            if (currentUser == null)
            {
                Console.WriteLine("Ошибка входа!");
                Pause();
                return;
            }

            switch (currentUser.Role)
            {
                case EmployeeRole.Admin:
                    MenuAdmin();
                    break;
                case EmployeeRole.Dispatcher:
                    MenuDispatcher();
                    break;
                case EmployeeRole.FlightAttendant:
                    MenuFlightAttendant();
                    break;
            }

            SaveAllData();
        }

        static void LoadAllData()
        {
            aircrafts = Aircraft.LoadAll();
            flights = Flight.LoadAll();
            passengers = Passenger.LoadAll();
            bookings = Booking.LoadAll();
            employees = Employee.LoadAll();
            airports = Airport.LoadAll();
        }

        static void SaveAllData()
        {
            Aircraft.SaveAll(aircrafts);
            Flight.SaveAll(flights);
            Passenger.SaveAll(passengers);
            Booking.SaveAll(bookings);
            Employee.SaveAll(employees);
            Airport.SaveAll(airports);
        }

        static void InitializeDefaultData()
        {
            if (employees.Count == 0)
            {
                employees.Add(new Employee("E001", "Admin", "Adminov", EmployeeRole.Admin, "admin", "123"));
                employees.Add(new Employee("E002", "Dispatcher", "Ivanov", EmployeeRole.Dispatcher, "disp", "123"));
                employees.Add(new Employee("E003", "Attendant", "Petrova", EmployeeRole.FlightAttendant, "att", "123"));
                Employee.SaveAll(employees);
            }

            if (aircrafts.Count == 0)
            {
                aircrafts.Add(new Aircraft("RA123", "Boeing 737", 180, 24, 156));
                Aircraft.SaveAll(aircrafts);
            }

            if (flights.Count == 0)
            {
                flights.Add(new Flight("PF100", "RA123", "SVO", "LED", "2025-11-30 10:00", "2025-11-30 12:00", 5000, 15000));
                Flight.SaveAll(flights);
            }

            if (airports.Count == 0)
            {
                airports.Add(new Airport("SVO", "Sheremetyevo", "Moscow", "Russia"));
                airports.Add(new Airport("LED", "Pulkovo", "Saint Petersburg", "Russia"));
                Airport.SaveAll(airports);
            }
        }

        static Employee Authenticate()
        {
            Console.WriteLine("=== Pinky Fly ===");
            Console.Write("Логин: ");
            string login = Console.ReadLine();
            Console.Write("Пароль: ");
            string pwd = Console.ReadLine();

            return employees.Find(e => e.Login == login && e.Authenticate(pwd));
        }

        static void ClearScreen()
        {
            Console.Clear();
        }

        static void Pause()
        {
            Console.WriteLine("\nНажмите Enter, чтобы продолжить...");
            Console.ReadLine();
        }

        // ========== МЕНЮ ==========

        static void MenuAdmin()
        {
            while (true)
            {
                ClearScreen();
                Console.WriteLine("=== Меню Администратора ===");
                Console.WriteLine("1. Воздушные суда");
                Console.WriteLine("2. Рейсы");
                Console.WriteLine("3. Пассажиры");
                Console.WriteLine("4. Бронирования");
                Console.WriteLine("5. Сотрудники");
                Console.WriteLine("6. Аэропорты");
                Console.WriteLine("7. Выход");
                Console.Write("Выберите пункт: ");
                if (!int.TryParse(Console.ReadLine(), out int choice))
                {
                    Console.WriteLine("Неверный ввод.");
                    Pause();
                    continue;
                }

                switch (choice)
                {
                    case 1: ModuleAircraft(); break;
                    case 2: ModuleFlights(); break;
                    case 3: ModulePassengers(); break;
                    case 4: ModuleBookings(); break;
                    case 5: ModuleEmployees(); break;
                    case 6: ModuleAirports(); break;
                    case 7: return;
                    default: Console.WriteLine("Неверный выбор."); Pause(); break;
                }
            }
        }

        static void MenuDispatcher()
        {
            while (true)
            {
                ClearScreen();
                Console.WriteLine("=== Меню Диспетчера ===");
                Console.WriteLine("1. Воздушные суда (просмотр)");
                Console.WriteLine("2. Рейсы");
                Console.WriteLine("3. Пассажиры");
                Console.WriteLine("4. Бронирования");
                Console.WriteLine("5. Аэропорты (просмотр)");
                Console.WriteLine("6. Выход");
                Console.Write("Выберите пункт: ");
                if (!int.TryParse(Console.ReadLine(), out int choice))
                {
                    Console.WriteLine("Неверный ввод.");
                    Pause();
                    continue;
                }

                switch (choice)
                {
                    case 1: Aircraft.DisplayTable(aircrafts); Pause(); break;
                    case 2: ModuleFlights(); break;
                    case 3: ModulePassengers(); break;
                    case 4: ModuleBookings(); break;
                    case 5: DisplayTableAirports(airports); Pause(); break;
                    case 6: return;
                    default: Console.WriteLine("Неверный выбор."); Pause(); break;
                }
            }
        }

        static void MenuFlightAttendant()
        {
            while (true)
            {
                ClearScreen();
                Console.WriteLine("=== Меню Бортпроводника ===");
                Console.WriteLine("1. Просмотр рейсов");
                Console.WriteLine("2. Схемы салонов");
                Console.WriteLine("3. Изменить статус рейса");
                Console.WriteLine("4. Выход");
                Console.Write("Выберите пункт: ");
                if (!int.TryParse(Console.ReadLine(), out int choice))
                {
                    Console.WriteLine("Неверный ввод.");
                    Pause();
                    continue;
                }

                switch (choice)
                {
                    case 1: Flight.DisplayTable(flights); Pause(); break;
                    case 2: ShowSeatMapSample(); Pause(); break;
                    case 3: UpdateFlightStatus(); Pause(); break;
                    case 4: return;
                    default: Console.WriteLine("Неверный выбор."); Pause(); break;
                }
            }
        }

        // ========== МОДУЛИ ==========

        static void ModuleAircraft()
        {
            while (true)
            {
                ClearScreen();
                Aircraft.DisplayTable(aircrafts);
                Console.WriteLine("\n1. Добавить ВС\n2. Редактировать ВС\n3. Удалить ВС\n4. Назад");
                if (!int.TryParse(Console.ReadLine(), out int choice)) continue;

                switch (choice)
                {
                    case 1:
                        Console.Write("Бортовой номер: ");
                        string tn = Console.ReadLine();
                        Console.Write("Модель: ");
                        string mdl = Console.ReadLine();
                        Console.Write("Всего мест: ");
                        int total = int.Parse(Console.ReadLine());
                        Console.Write("Мест бизнес-класса: ");
                        int biz = int.Parse(Console.ReadLine());
                        Console.Write("Мест эконом-класса: ");
                        int eco = int.Parse(Console.ReadLine());
                        aircrafts.Add(new Aircraft(tn, mdl, total, biz, eco));
                        Aircraft.SaveAll(aircrafts);
                        break;
                    case 2:
                        Console.Write("Бортовой номер для редактирования: ");
                        string editTn = Console.ReadLine();
                        var ac = aircrafts.FirstOrDefault(a => a.TailNumber == editTn);
                        if (ac != null)
                        {
                            Console.Write("Новая модель: ");
                            ac.Model = Console.ReadLine();
                            Console.Write("Всего мест: ");
                            ac.TotalSeats = int.Parse(Console.ReadLine());
                            Console.Write("Бизнес: ");
                            ac.BusinessSeats = int.Parse(Console.ReadLine());
                            Console.Write("Эконом: ");
                            ac.EconomySeats = int.Parse(Console.ReadLine());
                            Console.Write("Статус (InService/Maintenance/Retired): ");
                            ac.Status = Aircraft.StringToStatus(Console.ReadLine());
                            Console.Write("Дата ТО (YYYY-MM-DD): ");
                            ac.MaintenanceSchedule = Console.ReadLine();
                            Aircraft.SaveAll(aircrafts);
                        }
                        else
                        {
                            Console.WriteLine("ВС не найдено!");
                            Pause();
                        }
                        break;
                    case 3:
                        Console.Write("Бортовой номер для удаления: ");
                        string delTn = Console.ReadLine();
                        aircrafts.RemoveAll(a => a.TailNumber == delTn);
                        Aircraft.SaveAll(aircrafts);
                        break;
                    case 4: return;
                    default: Console.WriteLine("Неверный выбор."); Pause(); break;
                }
            }
        }

        static void ModuleFlights()
        {
            while (true)
            {
                ClearScreen();
                Flight.DisplayTable(flights);
                Console.WriteLine("\n1. Добавить рейс\n2. Изменить статус\n3. Задержать рейс\n4. Удалить рейс\n5. Назад");
                if (!int.TryParse(Console.ReadLine(), out int choice)) continue;

                switch (choice)
                {
                    case 1:
                        Console.Write("Номер рейса: ");
                        string fn = Console.ReadLine();
                        Console.Write("Бортовой номер ВС: ");
                        string tail = Console.ReadLine();
                        if (!aircrafts.Any(a => a.TailNumber == tail))
                        {
                            Console.WriteLine("ВС не найден!");
                            Pause(); continue;
                        }
                        Console.Write("Аэропорт вылета (код): ");
                        string dep = Console.ReadLine();
                        Console.Write("Аэропорт прилёта (код): ");
                        string arr = Console.ReadLine();
                        Console.Write("Время вылета (YYYY-MM-DD HH:MM): ");
                        string schedDep = Console.ReadLine();
                        Console.Write("Время прилёта (YYYY-MM-DD HH:MM): ");


                        string schedArr = Console.ReadLine();
                        Console.Write("Базовая цена эконом: ");
                        double eco = double.Parse(Console.ReadLine());
                        Console.Write("Базовая цена бизнес: ");
                        double biz = double.Parse(Console.ReadLine());
                        flights.Add(new Flight(fn, tail, dep, arr, schedDep, schedArr, eco, biz));
                        Flight.SaveAll(flights);
                        break;
                    case 2:
                        Console.Write("Номер рейса: ");
                        string fn2 = Console.ReadLine();
                        var fl = flights.FirstOrDefault(f => f.FlightNumber == fn2);
                        if (fl != null)
                        {
                            Console.Write("Новый статус (Scheduled/Departed/Delayed/Arrived/Canceled): ");
                            fl.Status = Flight.StringToStatus(Console.ReadLine());
                            Flight.SaveAll(flights);
                        }
                        else
                        {
                            Console.WriteLine("Рейс не найден!");
                            Pause();
                        }
                        break;
                    case 3:
                        Console.Write("Номер рейса: ");
                        string fn3 = Console.ReadLine();
                        Console.Write("Новое время вылета (YYYY-MM-DD HH:MM): ");
                        string newTime = Console.ReadLine();
                        var fl3 = flights.FirstOrDefault(f => f.FlightNumber == fn3);
                        if (fl3 != null)
                        {
                            fl3.DelayFlight(newTime);
                            Flight.SaveAll(flights);
                        }
                        else
                        {
                            Console.WriteLine("Рейс не найден!");
                            Pause();
                        }
                        break;
                    case 4:
                        Console.Write("Номер рейса для удаления: ");
                        string fn4 = Console.ReadLine();
                        flights.RemoveAll(f => f.FlightNumber == fn4);
                        Flight.SaveAll(flights);
                        break;
                    case 5: return;
                    default: Console.WriteLine("Неверный выбор."); Pause(); break;
                }
            }
        }

        static void ModulePassengers()
        {
            while (true)
            {
                ClearScreen();
                Passenger.DisplayTable(passengers);
                Console.WriteLine("\n1. Добавить пассажира\n2. Редактировать контактные данные\n3. Удалить пассажира\n4. Назад");
                if (!int.TryParse(Console.ReadLine(), out int choice)) continue;

                switch (choice)
                {
                    case 1:
                        Console.Write("Имя: ");
                        string fn = Console.ReadLine();
                        Console.Write("Фамилия: ");
                        string ln = Console.ReadLine();
                        Console.Write("Дата рождения (YYYY-MM-DD): ");
                        string dob = Console.ReadLine();
                        while (dob.Length != 10 || dob[4] != '-' || dob[7] != '-')
                        {
                            Console.Write("Неверный формат даты! Повторите (YYYY-MM-DD): ");
                            dob = Console.ReadLine();
                        }
                        Console.Write("Паспорт (6–15 цифр): ");
                        string pp = Console.ReadLine();
                        while (pp.Length < 6 || pp.Length > 15 || !pp.All(char.IsDigit))
                        {
                            Console.Write("Неверный паспорт! Повторите: ");
                            pp = Console.ReadLine();
                        }
                        Console.Write("Телефон (+7... или 8...): ");


                        string phone = Console.ReadLine();
                        while (!IsValidPhone(phone))
                        {
                            Console.Write("Неверный телефон! Повторите: ");
                            phone = Console.ReadLine();
                        }
                        Console.Write("Email: ");
                        string email = Console.ReadLine();
                        while (!IsValidEmail(email))
                        {
                            Console.Write("Неверный email! Повторите: ");
                            email = Console.ReadLine();
                        }
                        string id = "P" + (passengers.Count + 1);
                        passengers.Add(new Passenger(id, fn, ln, dob, pp, phone, email));
                        Passenger.SaveAll(passengers);
                        break;
                    case 2:
                        Console.Write("Паспорт для редактирования: ");
                        string editPp = Console.ReadLine();
                        var p = passengers.FirstOrDefault(x => x.PassportNumber == editPp);
                        if (p != null)
                        {
                            Console.Write("Новый телефон: ");
                            string newPhone = Console.ReadLine();
                            while (!IsValidPhone(newPhone)) { Console.Write("Неверный телефон! Повторите: "); newPhone = Console.ReadLine(); }
                            Console.Write("Новый email: ");
                            string newEmail = Console.ReadLine();
                            while (!IsValidEmail(newEmail)) { Console.Write("Неверный email! Повторите: "); newEmail = Console.ReadLine(); }
                            p.UpdateContactInfo(newPhone, newEmail);
                            Passenger.SaveAll(passengers);
                        }
                        else
                        {
                            Console.WriteLine("Пассажир не найден!");
                            Pause();
                        }
                        break;
                    case 3:
                        Console.Write("Паспорт для удаления: ");
                        string delPp = Console.ReadLine();
                        passengers.RemoveAll(p => p.PassportNumber == delPp);
                        Passenger.SaveAll(passengers);
                        break;
                    case 4: return;
                    default: Console.WriteLine("Неверный выбор."); Pause(); break;
                }
            }
        }

        static void ModuleBookings()
        {
            if (flights.Count == 0)
            {
                Console.WriteLine("Нет рейсов для бронирования!");
                Pause();
                return;
            }

            Flight.DisplayTable(flights);
            Console.Write("Введите номер рейса: ");
            string fn = Console.ReadLine();
            var flIt = flights.FirstOrDefault(f => f.FlightNumber == fn);
            if (flIt == null)
            {
                Console.WriteLine("Рейс не найден!");
                Pause();
                return;
            }

            int rows = 30, perRow = 6;
            int bizRows = (flIt.AircraftTail == "RA123") ? 4 : 2;
            var seatMap = new SeatMap(rows, perRow, (1, bizRows));

            foreach (var b in bookings.Where(b => b.FlightNumber == fn))
            {
                string code = b.SeatNumber;
                if (!int.TryParse(new string(code.Where(char.IsDigit).ToArray()), out int row)) continue;
                char seatChar = code.LastOrDefault(c => char.IsLetter(c));
                int seatIdx = seatChar - 'A';
                if (row >= 1 && row <= rows && seatIdx >= 0 && seatIdx < perRow)
                    seatMap.MarkSeatOccupied(row, seatIdx);
            }

            seatMap.Display();

            Console.Write("Введите место (например, 12C): ");
            string seat = Console.ReadLine();

            if (!int.TryParse(new string(seat.Where(char.IsDigit).ToArray()), out int selectedRow))
            {
                Console.WriteLine("Неверный формат места.");
                Pause();
                return;
            }
            char selectedChar = seat.LastOrDefault(c => char.IsLetter(c));
            int selectedIdx = selectedChar - 'A';

            if (selectedRow < 1 || selectedRow > rows || selectedIdx < 0 || selectedIdx >= perRow || !seatMap.IsSeatAvailable(selectedRow, selectedIdx))
            {
                Console.WriteLine("Место занято или некорректно!");
                Pause();
                return;
            }

            double price = seatMap.GetSeatPrice(selectedRow, flIt.BasePriceEconomy, flIt.BasePriceBusiness);
            Console.WriteLine($"Стоимость: {price} руб.");

            Console.Write("Паспорт пассажира: ");
            string pp = Console.ReadLine();
            var ps = passengers.FirstOrDefault(p => p.PassportNumber == pp);

            if (ps == null)
            {
                Console.WriteLine("Новый пассажир.");
                Console.Write("Имя: "); string fnP = Console.ReadLine();
                Console.Write("Фамилия: "); string lnP = Console.ReadLine();
                Console.Write("Дата рождения (YYYY-MM-DD): "); string dobP = Console.ReadLine();
                while (dobP.Length != 10 || dobP[4] != '-' || dobP[7] != '-')
                {
                    Console.Write("Неверный формат даты! Повторите: ");
                    dobP = Console.ReadLine();
                }
                Console.Write("Телефон (+7...): "); string phoneP = Console.ReadLine();
                while (!IsValidPhone(phoneP)) { Console.Write("Неверный телефон! Повторите: "); phoneP = Console.ReadLine(); }
                Console.Write("Email: "); string emailP = Console.ReadLine();
                while (!IsValidEmail(emailP)) { Console.Write("Неверный email! Повторите: "); emailP = Console.ReadLine(); }

                string newId = "P" + (passengers.Count + 1);
                ps = new Passenger(newId, fnP, lnP, dobP, pp, phoneP, emailP);
                passengers.Add(ps);
                Passenger.SaveAll(passengers);
            }

            string pnr = "PNR" + (1000 + bookings.Count + 1);
            string today = "2025-12-05";
            var newBooking = new Booking(pnr, fn, ps.PassengerId, seat, BookingStatus.Confirmed, price, today);
            bookings.Add(newBooking);
            Booking.SaveAll(bookings);

            Console.WriteLine($"\nБронирование успешно создано! PNR: {pnr}");
            Pause();
        }

        static void ModuleEmployees()
        {
            while (true)
            {
                ClearScreen();
                Employee.DisplayTable(employees);
                Console.WriteLine("\n1. Добавить сотрудника\n2. Удалить сотрудника\n3. Назад");
                if (!int.TryParse(Console.ReadLine(), out int choice)) continue;

                switch (choice)
                {
                    case 1:
                        Console.Write("ID: ");
                        string id = Console.ReadLine();
                        Console.Write("Имя: ");
                        string fn = Console.ReadLine();
                        Console.Write("Фамилия: ");
                        string ln = Console.ReadLine();
                        Console.WriteLine("Роль (1-Admin, 2-Dispatcher, 3-FlightAttendant): ");
                        int roleInt = int.Parse(Console.ReadLine());
                        var role = roleInt switch { 1 => EmployeeRole.Admin, 2 => EmployeeRole.Dispatcher, _ => EmployeeRole.FlightAttendant };
                        Console.Write("Логин: ");
                        string login = Console.ReadLine();
                        Console.Write("Пароль: ");
                        string pwd = Console.ReadLine();
                        employees.Add(new Employee(id, fn, ln, role, login, pwd));
                        Employee.SaveAll(employees);
                        break;
                    case 2:
                        Console.Write("ID сотрудника для удаления: ");


                        string delId = Console.ReadLine();
                        employees.RemoveAll(e => e.EmployeeId == delId);
                        Employee.SaveAll(employees);
                        break;
                    case 3: return;
                    default: Console.WriteLine("Неверный выбор."); Pause(); break;
                }
            }
        }

        static void ModuleAirports()
        {
            while (true)
            {
                ClearScreen();
                DisplayTableAirports(airports);
                Console.WriteLine("\n1. Добавить аэропорт\n2. Удалить аэропорт\n3. Назад");
                if (!int.TryParse(Console.ReadLine(), out int choice)) continue;

                switch (choice)
                {
                    case 1:
                        Console.Write("Код (например, SVO): ");
                        string code = Console.ReadLine();
                        Console.Write("Название: ");
                        string name = Console.ReadLine();
                        Console.Write("Город: ");
                        string city = Console.ReadLine();
                        Console.Write("Страна: ");
                        string country = Console.ReadLine();
                        airports.Add(new Airport(code, name, city, country));
                        Airport.SaveAll(airports);
                        break;
                    case 2:
                        Console.Write("Код аэропорта для удаления: ");
                        string delCode = Console.ReadLine();
                        airports.RemoveAll(a => a.Code == delCode);
                        Airport.SaveAll(airports);
                        break;
                    case 3: return;
                    default: Console.WriteLine("Неверный выбор."); Pause(); break;
                }
            }
        }

        static void UpdateFlightStatus()
        {
            Flight.DisplayTable(flights);
            Console.Write("Введите номер рейса: ");
            string fn = Console.ReadLine();
            var fl = flights.FirstOrDefault(f => f.FlightNumber == fn);
            if (fl != null)
            {
                Console.WriteLine($"Текущий статус: {Flight.StatusToString(fl.Status)}");
                Console.Write("Новый статус (Scheduled/Departed/Delayed/Arrived/Canceled): ");
                fl.Status = Flight.StringToStatus(Console.ReadLine());
                Flight.SaveAll(flights);
                Console.WriteLine("Статус обновлён.");
            }
            else
            {
                Console.WriteLine("Рейс не найден!");
            }
        }

        static void ShowSeatMapSample()
        {
            if (flights.Count == 0)
            {
                Console.WriteLine("Нет рейсов.");
                return;
            }

            int rows = 30, perRow = 6;
            var seatMap = new SeatMap(rows, perRow, (1, 4));
            seatMap.Display();
        }

        // ========== ВСПОМОГАТЕЛЬНЫЕ ==========

        static void DisplayTableAirports(List<Airport> list)
        {
            if (list == null || list.Count == 0)
            {
                Console.WriteLine("Нет данных.");
                return;
            }

            Console.WriteLine("\n=== Аэропорты ===");
            int codeWidth = 10;
            int nameWidth = 25;
            int cityWidth = 20;
            int countryWidth = 20;

            Console.WriteLine(
                $"{LeftPad("Код", codeWidth)} " +
                $"{LeftPad("Название", nameWidth)} " +
                $"{LeftPad("Город", cityWidth)} " +
                $"{LeftPad("Страна", countryWidth)}"
            );

            Console.WriteLine(new string('-', codeWidth + nameWidth + cityWidth + countryWidth + 3));

            foreach (var a in list)
            {
                Console.WriteLine(
                    $"{LeftPad(a.Code, codeWidth)} " +
                    $"{LeftPad(a.Name, nameWidth)} " +
                    $"{LeftPad(a.City, cityWidth)} " +
                    $"{LeftPad(a.Country, countryWidth)}"
                );
            }
        }

        static string LeftPad(string text, int width)
        {
            if (text == null) text = "";
            return text.Length > width ? text.Substring(0, width) : text.PadRight(width);
        }

        static bool IsValidEmail(string email)
        {
            int at = email.IndexOf('@');
            int dot = email.IndexOf('.', at);
            return at > 0 && dot > at + 1;
        }

        static bool IsValidPhone(string phone)
        {
            return (phone.StartsWith("+7") && phone.Length == 12) ||
                   (phone.StartsWith("8") && phone.Length == 11);
        }
    }
}
