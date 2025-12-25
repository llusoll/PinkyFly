using System;
using System.Collections.Generic;
using System.Linq;

namespace Lab4
{
    public class BookingRegistry<T> where T : Booking
    {
        private readonly List<T> _bookings = new List<T>();

        public void Add(T booking)
        {
            if (booking == null) throw new ArgumentNullException(nameof(booking));
            _bookings.Add(booking);
        }

        public void SortByPrice()
        {
            _bookings.Sort((a, b) => a.CalculatePrice().CompareTo(b.CalculatePrice()));
        }

        public T FindByPNR(string pnr)
        {
            return _bookings.FirstOrDefault(b => b.BookingNumber == pnr);
        }

        public double GetTotalRevenue()
        {
            return _bookings.Where(b => b.Status == BookingStatus.Paid)
                            .Sum(b => b.CalculatePrice());
        }

        // Копия списка (без возможности модификации)
        public IReadOnlyList<T> GetAll() => _bookings.AsReadOnly();
    }
}
