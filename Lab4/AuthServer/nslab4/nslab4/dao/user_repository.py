import sqlite3
from ..entity.user import User

AUTH_DB_PATH = "./database/auth.db"
USER_TABLE_NAME = "users"


class UserRepository:
    def __init__(self):
        self.connection = sqlite3.connect(AUTH_DB_PATH)
        self.cursor = self.connection.cursor()
        self._initialize_table()

    def __del__(self):
        self.cursor.close()
        self.connection.close()

    def _exec_sql(self, sql: str):
        self.cursor.execute(sql)
        self.connection.commit()

    def _initialize_table(self):
        self._exec_sql(f"CREATE TABLE IF NOT EXISTS {USER_TABLE_NAME} ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "account VARCHAR(50), "
                       "hash1Base64 VARCHAR(50))")

    def insert(self, account: User) -> None:
        self._exec_sql(f"INSERT INTO {USER_TABLE_NAME} (account, hash1Base64) VALUES "
                       f"('{account.account}','{account.hash1_base64}')")

    def get_by_account(self, account: str) -> User | None:
        self._exec_sql(f"SELECT * FROM {USER_TABLE_NAME} WHERE account='{account}'")
        account_list = self.cursor.fetchall()
        if len(account_list) == 0:
            return None
        else:
            return User(
                account_list[0][0],
                account_list[0][1],
                account_list[0][2]
            )

    def exists_by_account(self, account: str) -> bool:
        self._exec_sql(
            f"SELECT EXISTS(SELECT 1 FROM {USER_TABLE_NAME} WHERE account='{account}')")
        account_list = self.cursor.fetchall()
        if len(account_list) == 0:
            return False
        else:
            return account_list[0][0] != 0

    def update(self, user: User) -> None:
        self._exec_sql(f"UPDATE {USER_TABLE_NAME} SET "
                       f"account='{user.account}', hash1Base64='{user.hash1_base64}' "
                       f"WHERE id={user.id}")
