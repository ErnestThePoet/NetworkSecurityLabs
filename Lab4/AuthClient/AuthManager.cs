using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Net.Http.Json;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
using System.Security.Principal;
using System.Windows;
using System.Text.Json;
using System.Windows.Input;

namespace AuthClient
{
    class AuthManager
    {
        record class CreateUserRequest(
            string account, string hash1Base64);
        record class AuthUserRequest(
            string account, string hash2Base64,
            string clientAuthCodeBase64);
        record class ChandePasswordRequest(
            string account, string hash2Base64,
            string clientAuthCodeBase64, string newHash1Base64);

        record class SuccessMsgResponse(bool success, string msg);
        record class AuthUserResponse(
            bool success, string msg,string clientAuthCodeEncryptedBase64):
            SuccessMsgResponse(success,msg);

        private delegate Task<HttpResponseMessage> RequestFunc(
            string? uri, HttpContent? content);

        public AuthManager() { }

        private const int ACCOUNT_LENGTH_MIN = 3;
        private const int ACCOUNT_LENGTH_MAX = 15;
        private const int PASSWORD_LENGTH_MIN = 5;
        private const int PASSWORD_LENGTH_MAX = 15;

        private byte[] CalculateHash1(string account, string password)
        {
            using (SHA256 sha256 = SHA256.Create())
            {
                return sha256.ComputeHash(
                    Encoding.UTF8.GetBytes(account).Concat(
                        Encoding.UTF8.GetBytes(password))
                    .ToArray());
            }
        }

        private byte[] CalculateHash2(byte[] hash1, byte[] clientAuthCode)
        {
            using (SHA256 sha256 = SHA256.Create())
            {
                return sha256.ComputeHash(hash1.Concat(clientAuthCode).ToArray());
            }
        }

        private string Base64EncodeToString(byte[] data)
        {
            return Convert.ToBase64String(data);
        }

        private StringContent GetRequestJsonStringContent<T>(T data)
        {
            return new StringContent(
                JsonSerializer.Serialize(data), Encoding.UTF8, "application/json");
        }

        // returned Item2 is error message
        private async Task<(bool, string, TRes?)> Request<TReq, TRes>(
            RequestFunc requestFunc, string url, HttpContent content)
        {
            try
            {
                using (var response = await requestFunc(url, content))
                {
                    if (response.IsSuccessStatusCode)
                    {
                        TRes? responseData = await JsonSerializer.DeserializeAsync<TRes>(
                            response.Content.ReadAsStream());
                        return (true,
                            "",
                            responseData);
                    }
                    else
                    {
                        return (false,
                            $"请求错误，HTTP状态码{response.StatusCode}",
                            default(TRes));
                    }
                }
            }
            catch (Exception ex)
            {
                return (false, ex.Message, default(TRes));
            }
        }

        private async Task<(bool, string, TRes?)> Post<TReq, TRes>(
            string uri, TReq data)
        {
            HttpClient httpClient = new HttpClient();
            StringContent stringContent = GetRequestJsonStringContent(data);
            return await Request<TReq,TRes>(httpClient.PostAsync, uri, stringContent);
        }

        private async Task<(bool, string, TRes?)> Put<TReq, TRes>(
            string uri, TReq data)
        {
            HttpClient httpClient = new HttpClient();
            StringContent stringContent = GetRequestJsonStringContent(data);
            return await Request<TReq, TRes>(httpClient.PutAsync, uri, stringContent);
        }

        public async Task<(ResultType, string)> CreateUser(
            string account, string password, string passwordConfirm)
        {
            if (account.Length < ACCOUNT_LENGTH_MIN
                || account.Length > ACCOUNT_LENGTH_MAX)
            {
                return (ResultType.Failure, $"账号长度在{ACCOUNT_LENGTH_MIN}至{ACCOUNT_LENGTH_MAX}之间");
            }

            if (password.Length < PASSWORD_LENGTH_MIN
                || password.Length > PASSWORD_LENGTH_MAX)
            {
                return (ResultType.Failure, $"密码长度在{PASSWORD_LENGTH_MIN}至{PASSWORD_LENGTH_MAX}之间");
            }

            if (password != passwordConfirm)
            {
                return (ResultType.Failure, "两次密码输入不一致");
            }

            var (isSuccess, errorMessage, responseData) =
                await Post<CreateUserRequest, SuccessMsgResponse>(
                    Apis.CREATE_USER, new CreateUserRequest(
                        account,
                        Base64EncodeToString(
                            CalculateHash1(account, password))));

            if(!isSuccess)
            {
                return (ResultType.Failure, errorMessage);
            }
            else if (!responseData!.success)
            {
                return (ResultType.Failure, responseData!.msg);
            }

            return (ResultType.Success, "注册成功");
        }
    }
}
