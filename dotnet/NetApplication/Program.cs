using System;

namespace NetApplication
{
    public static class Program
    {
        public static void Main(string[] args)
        {
            Console.WriteLine("NetApplication has been called.");

            foreach (var arg in args)
            {
                Console.WriteLine($"Argument: {arg}");
            }
        }

    }
}
