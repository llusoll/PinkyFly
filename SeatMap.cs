using System;
using System.Collections.Generic;

namespace Lab4
{

    public class SeatMap
    {
        public int TotalRows { get; set; }
        public int SeatsPerRow { get; set; }
        public List<List<bool>> SeatOccupancy { get; set; }
        public List<char> SeatLabels { get; set; }
        public (int, int) BusinessClassRows { get; set; }

        public SeatMap(int rows, int perRow, (int, int) bizRows)
        {
            TotalRows = rows;
            SeatsPerRow = perRow;
            BusinessClassRows = bizRows;
            SeatOccupancy = new List<List<bool>>();
            SeatLabels = new List<char>();

            for (int i = 0; i < SeatsPerRow; i++)
            {
                SeatLabels.Add((char)('A' + i));
            }

            for (int r = 0; r < TotalRows; r++)
            {
                var row = new List<bool>();
                for (int s = 0; s < SeatsPerRow; s++)
                {
                    row.Add(false);
                }
                SeatOccupancy.Add(row);
            }
        }

        public bool IsSeatAvailable(int row, int seat)
        {
            if (row < 1 || row > TotalRows || seat < 0 || seat >= SeatsPerRow)
                return false;
            return !SeatOccupancy[row - 1][seat];
        }

        public void MarkSeatOccupied(int row, int seat)
        {
            if (row >= 1 && row <= TotalRows && seat >= 0 && seat < SeatsPerRow)
            {
                SeatOccupancy[row - 1][seat] = true;
            }
        }

        public string GetSeatCode(int row, int seat)
        {
            return $"{row}{SeatLabels[seat]}";
        }

        public double GetSeatPrice(int row, double baseEco, double baseBiz)
        {
            if (row >= BusinessClassRows.Item1 && row <= BusinessClassRows.Item2)
                return baseBiz;
            return baseEco;
        }

        public void Display()
        {
            Console.WriteLine("\n=== Схема салона ===");

            if (BusinessClassRows.Item1 <= BusinessClassRows.Item2)
            {
                Console.ForegroundColor = ConsoleColor.Yellow;
                Console.WriteLine($"Бизнес-класс: ряды {BusinessClassRows.Item1}–{BusinessClassRows.Item2}");
                Console.ResetColor();
            }

            for (int r = 1; r <= TotalRows; r++)
            {
                Console.Write($"[{r:D2}] ");
                for (int s = 0; s < SeatsPerRow; s++)
                {
                    string code = GetSeatCode(r, s);

                    if (IsSeatAvailable(r, s))
                    {
                        // Свободно — зелёный
                        Console.ForegroundColor = ConsoleColor.Green;
                        Console.Write($"{code} ");
                    }
                    else
                    {
                        // Занято — красный
                        Console.ForegroundColor = ConsoleColor.Red;
                        Console.Write($"{code} ");
                    }
                }
                Console.ResetColor();
                Console.WriteLine();
            }

            Console.ForegroundColor = ConsoleColor.Green;
            Console.Write("Зелёный");
            Console.ResetColor();
            Console.Write(" — свободно, ");
            Console.ForegroundColor = ConsoleColor.Red;
            Console.Write("Красный");
            Console.ResetColor();
            Console.WriteLine(" — занято\n");
        }
    }
}