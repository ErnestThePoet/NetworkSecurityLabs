using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AuthClient
{
    class Apis
    {
        // Using localhost will make request slower
        private static string API_PREFIX = "http://127.0.0.1:32100/";

        public static string CREATE_USER = API_PREFIX + "api/create_user/";
        public static string AUTH_USER = API_PREFIX + "api/auth_user/";
        public static string CHANGE_PASSWORD = API_PREFIX + "api/change_password/";
    }
}
