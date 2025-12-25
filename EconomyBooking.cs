using System;

namespace Lab4
{
    public class EconomyBooking : Booking, IBookable
    {
        public EconomyBooking(string pnr, string fn, string pid, string seat, BookingStatus st, double basePr, string date)
            : base(pnr, fn, pid, seat, st, basePr, date)
        {
            priceMultiplier = 1.0; // эконом
        }

        public override double CalculatePrice() => basePrice * priceMultiplier;

        public override string GetDescription() => "Эконом-класс";

        // Реализация интерфейса
        public void Cancel() => base.Cancel();

        // Глубокое клонирование
        public override Booking DeepClone()
        {
            return new EconomyBooking(bookingNumber, flightNumber, passengerId, seatNumber, status, basePrice, bookingDate);
        }

        // Метод БЕЗ вызова базового — скидка
        public void ApplyDiscount(double percent)
        {
            if (percent < 0 || percent > 100)
                throw new ArgumentException("Скидка должна быть от 0 до 100%");
            basePrice *= (1.0 - percent / 100.0);
        }
    }
}

