using System;
using System.Collections.Generic;
using System.IO;

namespace Lab4
{

    public class Airport
    {
        public string Code { get; set; }
        public string Name { get; set; }
        public string City { get; set; }
        public string Country { get; set; }

        public Airport() { }

        public Airport(string code, string name, string city, string country)
        {
            Code = code;
            Name = name;
            City = city;
            Country = country;
        }

        public static List<Airport> LoadAll()
        {
            var airports = new List<Airport>();
            if (!File.Exists("data/airports.txt"))
                return airports;

            foreach (var line in File.ReadAllLines("data/airports.txt"))
            {
                if (string.IsNullOrWhiteSpace(line) || line.StartsWith("#")) continue;

                var parts = Utils.Split(line, ';');
                if (parts.Count != 4) continue;

                airports.Add(new Airport(parts[0], parts[1], parts[2], parts[3]));
            }

            return airports;
        }

        public static void SaveAll(List<Airport> airports)
        {
            Utils.EnsureDataDir();
            using var writer = new StreamWriter("data/airports.txt", false);
            writer.WriteLine("# Code;Name;City;Country");
            foreach (var a in airports)
            {
                writer.WriteLine($"{a.Code};{a.Name};{a.City};{a.Country}");
            }
        }
    }
}