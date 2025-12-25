using System;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace Lab4
{

    public static class Utils
    {
        public static List<string> Split(string s, char delimiter)
        {
            if (string.IsNullOrEmpty(s))
                return new List<string>();
            return new List<string>(s.Split(delimiter));
        }

        public static void EnsureDataDir()
        {
            if (!Directory.Exists("data"))
                Directory.CreateDirectory("data");
        }

        public static string SimpleHash(string input)
        {
            using (SHA256 sha256Hash = SHA256.Create())
            {
                byte[] bytes = sha256Hash.ComputeHash(Encoding.UTF8.GetBytes(input));
                StringBuilder builder = new StringBuilder();
                foreach (byte b in bytes)
                {
                    builder.Append(b.ToString("x2"));
                }
                return builder.ToString();
            }
        }
    }
}