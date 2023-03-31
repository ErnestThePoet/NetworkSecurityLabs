#### 注册账号接口
* 请求方法：POST
* 请求参数：
{
    account:string;
    hash1Base64:string;
}
* 返回类型：json
* 返回格式：
{
	success:boolean;
	msg:string;
}

#### 认证接口
* 请求方法：PUT
* 请求参数：
{
    account:string;
    hash2Base64:string;
    clientAuthCodeBase64:string;
}
* 返回类型：json
* 返回格式：
{
	success:boolean;
	msg:string;
    serverAuthCodeEncryptedBase64?:string;
}

#### 修改密码接口
* 请求方法：PUT
* 请求参数：
{
    account:string;
    hash2Base64:string;
    clientAuthCodeBase64:string;
    newHash1Base64:string;
}
* 返回类型：json
* 返回格式：
{
	success:boolean;
	msg:string;
}