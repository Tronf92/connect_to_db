#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

using namespace std;

void show_err(unsigned int HandleType, const SQLHANDLE& handle){
	SQLCHAR sqlstate[1024];
	SQLCHAR sqlmessage[1024];
	if(SQL_SUCCESS == SQLGetDiagRec(HandleType , handle, 1, sqlstate, NULL, sqlmessage, 1024, NULL)){
		cout << "Message:" << sqlmessage << "\nSQL State:" << sqlstate << endl;
	}

}


int main(){
	SQLHANDLE SqlEnvHandle= nullptr;
	SQLHANDLE SqlConnHandle = nullptr;
	SQLHANDLE SqlStateHandle = nullptr;
	SQLRETURN retcode;

	if(SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV , SQL_NULL_HANDLE, &SqlEnvHandle)){
		goto FINISHED;

	}

	if(SQL_SUCCESS != SQLSetEnvAttr(SqlEnvHandle,SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)){
		goto FINISHED;
	}

	if(SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SqlEnvHandle, &SqlConnHandle)){
		goto FINISHED;
	}

	SQLCHAR RetConString[1024];
	
	switch (SQLDriverConnect (SqlConnHandle,
                NULL,
                (SQLCHAR*)"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=firme;UID=adrian;PWD=blabla;",
                SQL_NTS,
                RetConString,
                1024,
                NULL,
                SQL_DRIVER_NOPROMPT)){

	case SQL_SUCCESS_WITH_INFO:
		show_err(SQL_HANDLE_DBC, SqlConnHandle);
		break;
	case SQL_INVALID_HANDLE:
	case SQL_ERROR:
		show_err(SQL_HANDLE_DBC, SqlConnHandle);
		goto FINISHED;
	default:
		break;
	}

	if(SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle , &SqlStateHandle)){
		goto FINISHED;
	}
	if(SQL_SUCCESS != SQLExecDirect(SqlStateHandle, (SQLCHAR*)"select * from Tabel_firme",SQL_NTS)){
		goto FINISHED;
	}
	else{
		char name[64];
		char id[64];
		char traseu[64];
		char tonaj[64];
		char suma[64];
		char pret_km[64];
		char Tplata[64];
		char achitat[128];
		char zile[64];
		
		while(SQLFetch(SqlStateHandle)==SQL_SUCCESS){
			SQLGetData(SqlStateHandle, 1, SQL_C_CHAR, name, 64, NULL);
			SQLGetData(SqlStateHandle, 2, SQL_C_CHAR, id, 64, NULL);
			SQLGetData(SqlStateHandle, 3, SQL_C_CHAR, traseu, 64, NULL);
			SQLGetData(SqlStateHandle, 4, SQL_C_CHAR, tonaj, 64, NULL);
			SQLGetData(SqlStateHandle, 5, SQL_C_CHAR, suma, 64, NULL);
			SQLGetData(SqlStateHandle, 6, SQL_C_CHAR, pret_km, 64, NULL);
			SQLGetData(SqlStateHandle, 7, SQL_C_CHAR, Tplata, 64, NULL);
			SQLGetData(SqlStateHandle, 8, SQL_C_CHAR, achitat, 128, NULL);
			SQLGetData(SqlStateHandle, 9, SQL_C_CHAR, zile, 64, NULL);
			cout << id << " " << name << " " << traseu << " " << tonaj << " " << suma << " " << pret_km << " " << Tplata << " " << achitat << " " << zile << endl;
		}
	}

FINISHED:
	SQLFreeHandle(SQL_HANDLE_STMT, SqlStateHandle );
    SQLDisconnect(SqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, SqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, SqlEnvHandle);



	system("pause");
}