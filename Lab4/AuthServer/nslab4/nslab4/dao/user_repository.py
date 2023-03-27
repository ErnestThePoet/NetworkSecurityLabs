from typing import Iterable
import sqlite3
from nslab4.nslab4.entity.account import Account

AUTH_DB_PATH = "../database/auth.db"
ACCOUNTS_TABLE_NAME = "accounts"


class AccountRepository:
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
        self._exec_sql(f"CREATE TABLE IF NOT EXISTS {ACCOUNTS_TABLE_NAME} ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "account VARCHAR(50), "
                       "hash1Base64 VARCHAR(50))")

    def insert_account(self, account: Account) -> None:
        self._exec_sql(f"INSERT INTO {ACCOUNTS_TABLE_NAME} (account, hash1Base64) VALUES "
                       f"('{account.account}','{account.hash1_base64}')")

    def get_account_by_account(self, account: str) -> Account | None:
        self._exec_sql(f"SELECT * FROM {ACCOUNTS_TABLE_NAME} WHERE account='{account}'")
        account_list = self.cursor.fetchall()
        if len(account_list) == 0:
            return None
        else:
            return Account(
                account_list[0][0],
                account_list[0][1],
                account_list[0][2]
            )

    def update_account(self, account: Account) -> None:
        self._exec_sql(f"UPDATE {ACCOUNTS_TABLE_NAME} SET "
                       f"account='{account.account}', hash1Base64='{account.hash1_base64}' "
                       f"WHERE id={account.id}")
