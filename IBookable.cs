namespace Lab4
{
    public interface IBookable
    {
        double CalculatePrice();
        void Cancel();
        string GetDescription();
    }
}
