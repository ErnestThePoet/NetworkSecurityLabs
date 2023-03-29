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
using System.IO;

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
            bool success, string msg, string serverAuthCodeEncryptedBase64) :
            SuccessMsgResponse(success, msg);

        private delegate Task<HttpResponseMessage> RequestFunc(
            string? uri, HttpContent? content);

        private const int ACCOUNT_LENGTH_MIN = 3;
        private const int ACCOUNT_LENGTH_MAX = 15;
        private const int PASSWORD_LENGTH_MIN = 5;
        private const int PASSWORD_LENGTH_MAX = 15;

        private const int CLIENT_AUTH_CODE_LENGTH = 32;
        private const int SERVER_AUTH_CODE_LENGTH = 32;

        public bool IsAuthorized { get; set; }
        public string Account { get; set; }
        public byte[] ServerAuthCode { get; set; }

        public AuthManager()
        {
            IsAuthorized = false;
            Account = string.Empty;
            ServerAuthCode = new byte[0];
        }

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

        private (bool, string) CheckAccount(string account)
        {
            if (account.Length < ACCOUNT_LENGTH_MIN
                || account.Length > ACCOUNT_LENGTH_MAX)
            {
                return (false, $"账号长度在{ACCOUNT_LENGTH_MIN}至{ACCOUNT_LENGTH_MAX}之间");
            }

            return (true, "");
        }

        private (bool, string) CheckPassword(string password)
        {
            if (password.Length < PASSWORD_LENGTH_MIN
                || password.Length > PASSWORD_LENGTH_MAX)
            {
                return (false, $"密码长度在{PASSWORD_LENGTH_MIN}至{PASSWORD_LENGTH_MAX}之间");
            }

            return (true, "");
        }

        // returned Item2 is error message
        private async Task<(bool, string, TRes?)> RequestAsync<TReq, TRes>(
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
                            $"请求错误，HTTP状态码{((int)response.StatusCode)}",
                            default(TRes));
                    }
                }
            }
            catch (Exception ex)
            {
                return (false, ex.Message, default(TRes));
            }
        }

        private async Task<(bool, string, TRes?)> PostAsync<TReq, TRes>(
            string uri, TReq data)
        {
            HttpClient httpClient = new HttpClient();
            StringContent stringContent = GetRequestJsonStringContent(data);
            return await RequestAsync<TReq, TRes>(httpClient.PostAsync, uri, stringContent);
        }

        private async Task<(bool, string, TRes?)> PutAsync<TReq, TRes>(
            string uri, TReq data)
        {
            HttpClient httpClient = new HttpClient();
            StringContent stringContent = GetRequestJsonStringContent(data);
            return await RequestAsync<TReq, TRes>(httpClient.PutAsync, uri, stringContent);
        }

        public async Task<(ResultType, string)> CreateUserAsync(
            string account, string password, string passwordConfirm)
        {
            var (isValid, checkErrorMessage) = CheckAccount(account);
            if (!isValid)
            {
                return (ResultType.Failure, checkErrorMessage);
            }

            (isValid, checkErrorMessage) = CheckPassword(password);
            if (!isValid)
            {
                return (ResultType.Failure, checkErrorMessage);
            }

            if (password != passwordConfirm)
            {
                return (ResultType.Failure, "两次密码输入不一致");
            }

            var (isSuccess, errorMessage, responseData) =
                await PostAsync<CreateUserRequest, SuccessMsgResponse>(
                    Apis.CREATE_USER, new CreateUserRequest(
                        account,
                        Base64EncodeToString(
                            CalculateHash1(account, password))));

            if (!isSuccess)
            {
                return (ResultType.Failure, errorMessage);
            }
            else if (!responseData!.success)
            {
                return (ResultType.Failure, responseData!.msg);
            }

            return (ResultType.Success, "注册成功");
        }

        public async Task<(ResultType, string?, ResultType, string)> SubmitAuthAsync(
            string account, string password)
        {
            var (isValid, checkErrorMessage) = CheckAccount(account);
            if (!isValid)
            {
                return (ResultType.Failure, null,
                    ResultType.Failure, checkErrorMessage);
            }

            (isValid, checkErrorMessage) = CheckPassword(password);
            if (!isValid)
            {
                return (ResultType.Failure, null,
                    ResultType.Failure, checkErrorMessage);
            }

            byte[] clientAuthCode = new byte[CLIENT_AUTH_CODE_LENGTH];
            new Random().NextBytes(clientAuthCode);
            string clientAuthCodeBase64 = Base64EncodeToString(clientAuthCode);

            byte[] hash1 = CalculateHash1(account, password);
            byte[] hash2 = CalculateHash2(hash1, clientAuthCode);

            var (isSuccess, errorMessage, responseData) =
                await PutAsync<AuthUserRequest, AuthUserResponse>(
                    Apis.AUTH_USER, new AuthUserRequest(
                        account,
                        Base64EncodeToString(hash2),
                        Base64EncodeToString(clientAuthCode)));

            if (!isSuccess)
            {
                return (ResultType.Success, clientAuthCodeBase64,
                    ResultType.Failure, errorMessage);
            }
            else if (!responseData!.success)
            {
                return (ResultType.Success, clientAuthCodeBase64,
                    ResultType.Failure, responseData!.msg);
            }

            byte[] serverAuthCodeEncrypted = Convert.FromBase64String(
                        responseData.serverAuthCodeEncryptedBase64);


            try
            {
                using (Aes aes = Aes.Create())
                {
                    aes.Padding = PaddingMode.PKCS7;
                    aes.Mode = CipherMode.CBC;
                    aes.KeySize = 256;
                    aes.BlockSize = 128;
                    aes.Key = hash1;

                    byte[] iv = new byte[16];
                    Array.Copy(hash1, iv, 16);
                    aes.IV = iv;

                    ICryptoTransform decryptor = aes.CreateDecryptor(aes.Key, aes.IV);
                    using (MemoryStream msDecrypt = new MemoryStream())
                    {
                        using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Write))
                        {
                            csDecrypt.Write(serverAuthCodeEncrypted);
                            csDecrypt.FlushFinalBlock();

                            if (msDecrypt.Length != SERVER_AUTH_CODE_LENGTH)
                            {
                                return (ResultType.Success, clientAuthCodeBase64,
                                    ResultType.Failure, $"服务端认证码长度非法({msDecrypt.Length})");
                            }

                            ServerAuthCode = msDecrypt.ToArray();
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                return (ResultType.Success, clientAuthCodeBase64,
                    ResultType.Failure, ex.Message);
            }

            IsAuthorized = true;
            Account = account;

            return (ResultType.Success, clientAuthCodeBase64,
                    ResultType.Success, "认证成功");
        }
    }
}
