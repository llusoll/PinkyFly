using System;

namespace Lab4
{
    public class BusinessBooking : Booking, IBookable, IPriorityService
    {
        private bool hasPriorityBoarding = true;
        private bool hasExtraBaggage = true;

        public BusinessBooking(string pnr, string fn, string pid, string seat, BookingStatus st, double basePr, string date)
            : base(pnr, fn, pid, seat, st, basePr, date)
        {
            priceMultiplier = 2.5;
        }

        public override double CalculatePrice() => basePrice * priceMultiplier;
        public override string GetDescription() => "Бизнес-класс (+багаж, приоритет)";

        // Перегрузка С вызовом базового
        public override void Cancel()
        {
            if (status == BookingStatus.Paid)
                Console.WriteLine("Возврат средств для бизнес-брони!");
            base.Cancel();
        }



        public void GrantPriorityBenefits()
        {
            Console.WriteLine("Предоставлены: приоритетная посадка и +20 кг багажа.");
        }

        public override Booking DeepClone()
        {
            return new BusinessBooking(bookingNumber, flightNumber, passengerId, seatNumber, status, basePrice, bookingDate);
        }


        public void AssignFromBase(Booking other)
        {
            if (other == null) return;

            this.bookingNumber = other.BookingNumber;
            this.flightNumber = other.FlightNumber;
            this.passengerId = other.PassengerId;
            this.seatNumber = other.SeatNumber;
            this.status = other.Status;
            this.basePrice = other.BasePrice; // priceMultiplier остаётся 2.5!
            this.bookingDate = other.BookingDate;
        }
    }
}
