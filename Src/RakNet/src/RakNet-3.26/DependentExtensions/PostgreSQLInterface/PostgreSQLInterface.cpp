#include "PostgreSQLInterface.h"

// libpq-fe.h is part of PostgreSQL which must be installed on this computer to use the PostgreRepository
#include "libpq-fe.h"

#ifdef _CONSOLE_1
#include "Console1Includes.h"
#elif defined(_WIN32)
#include <winsock2.h> // htonl
#elif defined(_CONSOLE_2)
#include "Console2Includes.h"
#else
#include <arpa/inet.h>
#endif

// alloca
#ifdef _COMPATIBILITY_1
#elif defined(_WIN32)
#include <malloc.h>
#else
//#include <stdlib.h>
#endif

#include "RakString.h"
#include "RakAssert.h"
#include "BitStream.h"
#include "FormatString.h"
#include "LinuxStrings.h"

#define PQEXECPARAM_FORMAT_TEXT		0
#define PQEXECPARAM_FORMAT_BINARY	1

PostgreSQLInterface::PostgreSQLInterface()
{
	pgConn=0;
	isConnected=false;
	lastError[0]=0;
	pgConnAllocatedHere=false;
}
PostgreSQLInterface::~PostgreSQLInterface()
{
	if (isConnected && pgConnAllocatedHere)
		PQfinish(pgConn);
}
bool PostgreSQLInterface::Connect(const char *conninfo)
{
	if (isConnected==false)
	{
		pgConn=PQconnectdb(conninfo);
		ConnStatusType status = PQstatus(pgConn);
		isConnected=status==CONNECTION_OK;
		if (isConnected==false)
		{
			PQfinish(pgConn);
			return false;
		}
		pgConnAllocatedHere=true;
	}

	return isConnected;
}
void PostgreSQLInterface::AssignConnection(PGconn *_pgConn)
{
	pgConnAllocatedHere=false;
	pgConn=_pgConn;
	ConnStatusType status = PQstatus(pgConn);
	isConnected=status==CONNECTION_OK;
}
PGconn *PostgreSQLInterface::GetPGConn(void) const
{
	return pgConn;
}
void PostgreSQLInterface::Disconnect(void)
{
	if (isConnected)
	{
		PQfinish(pgConn);
		isConnected=false;
	}
}
void PostgreSQLInterface::Rollback(void)
{
	PGresult *result = PQexec(pgConn, "ROLLBACK;");
	PQclear(result);
}
bool PostgreSQLInterface::IsResultSuccessful(PGresult *result, bool rollbackOnFailure)
{
	if (result==0)
		return false;

	bool success=false;
	ExecStatusType execStatus = PQresultStatus(result);
	strcpy(lastError,PQresultErrorMessage(result));
	switch (execStatus)
	{
	case PGRES_COMMAND_OK:
		success=true;
		break;
	case PGRES_EMPTY_QUERY:
		break;
	case PGRES_TUPLES_OK:
		success=true;
		break;
	case PGRES_COPY_OUT:
		break;
	case PGRES_COPY_IN:
		break;
	case PGRES_BAD_RESPONSE:
		break;
	case PGRES_NONFATAL_ERROR:
		break;
	case PGRES_FATAL_ERROR:
		if (rollbackOnFailure)
			Rollback();
		break;
	}
	return success;
}
bool PostgreSQLInterface::ExecuteBlockingCommand(const char *command, PGresult **result, bool rollbackOnFailure)
{
	*result = PQexec(pgConn, command);
	return IsResultSuccessful(*result, rollbackOnFailure);
}
bool PostgreSQLInterface::PQGetValueFromBinary(int *output, PGresult *result, unsigned int rowIndex, const char *columnName)
{
	return PQGetValueFromBinary(output,result,(int) rowIndex,columnName);
}
bool PostgreSQLInterface::PQGetValueFromBinary(int *output, PGresult *result, int rowIndex, const char *columnName)
{
	int columnIndex = PQfnumber(result, columnName); if (columnIndex==-1) return false;
	char *binaryData = PQgetvalue(result, rowIndex, columnIndex);
	if (binaryData==0)
		return false;
	if (binaryData)
	{
		RakAssert(PQgetlength(result, rowIndex, columnIndex)==sizeof(int));
		memcpy(output, binaryData, sizeof(int));
		EndianSwapInPlace((char*)output, sizeof(int));
		return true;
	}
	return false;
}
bool PostgreSQLInterface::PQGetValueFromBinary(unsigned int *output, PGresult *result, int rowIndex, const char *columnName)
{
	int columnIndex = PQfnumber(result, columnName); if (columnIndex==-1) return false;
	char *binaryData = PQgetvalue(result, rowIndex, columnIndex);
	if (binaryData==0)
		return false;	
	if (binaryData)
	{
		RakAssert(PQgetlength(result, rowIndex, columnIndex)==sizeof(unsigned int));
		memcpy(output, binaryData, sizeof(unsigned int));
		EndianSwapInPlace((char*)output, sizeof(unsigned int));
		return true;
	}
	return false;
}
bool PostgreSQLInterface::PQGetValueFromBinary(long long *output, PGresult *result, int rowIndex, const char *columnName)
{
	int columnIndex = PQfnumber(result, columnName); if (columnIndex==-1) return false;
	char *binaryData = PQgetvalue(result, rowIndex, columnIndex);
	if (binaryData==0)
		return false;
	if (binaryData)
	{
		RakAssert(PQgetlength(result, rowIndex, columnIndex)==sizeof(long long));
		memcpy(output, binaryData, sizeof(long long));
		EndianSwapInPlace((char*)output, sizeof(long long));
		return true;
	}
	return false;
}
bool PostgreSQLInterface::PQGetValueFromBinary(float *output, PGresult *result, int rowIndex, const char *columnName)
{
	int columnIndex = PQfnumber(result, columnName); if (columnIndex==-1) return false;
	char *binaryData = PQgetvalue(result, rowIndex, columnIndex);
	if (binaryData==0)
		return false;
	int len = PQgetlength(result, rowIndex, columnIndex);
	RakAssert(len==sizeof(float));
	RakAssert(binaryData);
	if (binaryData)
	{
		memcpy(output, binaryData, sizeof(float));
		EndianSwapInPlace((char*)output, sizeof(float));
		return true;
	}
	return false;
}
bool PostgreSQLInterface::PQGetValueFromBinary(bool *output, PGresult *result, int rowIndex, const char *columnName)
{
	int columnIndex = PQfnumber(result, columnName); if (columnIndex==-1) return false;
	char *binaryData = PQgetvalue(result, rowIndex, columnIndex);
	if (binaryData==0)
		return false;
	*output = binaryData[0]!=0;
	
	// Seems to return 1 byte only
	/*
	int tempResult=0;
//	int len = PQgetlength(result, rowIndex, columnIndex);
	// RakAssert(len==sizeof(int)); // bug here, returns 0, but is an int
	RakAssert(binaryData);
	if (binaryData)
	{
		memcpy(&tempResult, binaryData, sizeof(int));
		EndianSwapInPlace((char*)&tempResult, sizeof(int));
		*output = tempResult!=0;
		return true;
	}
	*/
	
	return true;
}
bool PostgreSQLInterface::PQGetValueFromBinary(RakNet::RakString *output, PGresult *result, int rowIndex, const char *columnName)
{
	int columnIndex = PQfnumber(result, columnName); if (columnIndex==-1) return false;
	char *gv;
	gv = PQgetvalue(result, rowIndex, columnIndex);
	if (gv && gv[0])
		*output=gv;
	else
		output->Clear();
	return output->IsEmpty()==false;
}
bool PostgreSQLInterface::PQGetValueFromBinary(char **output, unsigned int *outputLength, PGresult *result, int rowIndex, const char *columnName)
{
	return PQGetValueFromBinary(output, (int*) outputLength,result, rowIndex,columnName);
}
bool PostgreSQLInterface::PQGetValueFromBinary(char **output, int *outputLength, PGresult *result, int rowIndex, const char *columnName)
{
	int columnIndex = PQfnumber(result, columnName);
	if (columnIndex!=-1)
	{
		*outputLength=PQgetlength(result, rowIndex, columnIndex);
		if (*outputLength==0)
		{
			*output=0;
			return false;
		}
		else
		{
			*output = new char[*outputLength];
			memcpy(*output, PQgetvalue(result, rowIndex, columnIndex), *outputLength);
			return true;
		}
	}
	else
		return false;
	
}
void PostgreSQLInterface::EndianSwapInPlace(char* data, int dataLength)
{
	static bool alreadyNetworkOrder=(htonl(12345) == 12345);
	if (alreadyNetworkOrder)
		return;
	int i;
	char tmp;
	for (i=0; i < dataLength/2; i++)
	{
		tmp=data[i];
		data[i]=data[dataLength-1-i];
		data[dataLength-1-i]=tmp;
	}
}

char *PostgreSQLInterface::GetLocalTimestamp(void)
{
	static char strRes[512];

	PGresult *result;
	if (ExecuteBlockingCommand("SELECT LOCALTIMESTAMP", &result, false))
	{
		char *ts=PQgetvalue(result, 0, 0);
		if (ts)
		{
			sprintf(strRes,"Local timestamp is: %s\n", ts);
		}
		else
		{
			sprintf(strRes,"Can't read current time\n");
		}
		PQclear(result);
	}
	else
		sprintf(strRes,"Failed to read LOCALTIMESTAMP\n");

	return (char*)strRes;

}

const char* PostgreSQLInterface::GetLastError(void) const
{
	return (char*)lastError;
}
void PostgreSQLInterface::EncodeQueryInput(const char *colName, unsigned int value, RakNet::RakString &paramTypeStr, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat)
{
	if (paramTypeStr.IsEmpty()==false)
	{
		paramTypeStr += ", ";
		valueStr += ", ";
	}
	paramTypeStr += colName;
	valueStr += FormatString("%i", value);
}
void PostgreSQLInterface::EncodeQueryUpdate(const char *colName, unsigned int value, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat)
{
	if (valueStr.IsEmpty()==false)
	{
		valueStr += ", ";
	}
	valueStr += colName;
	valueStr += " = ";
	valueStr += FormatString("%i", value);
}
void PostgreSQLInterface::EncodeQueryInput(const char *colName, int value, RakNet::RakString &paramTypeStr, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat)
{
	if (paramTypeStr.IsEmpty()==false)
	{
		paramTypeStr += ", ";
		valueStr += ", ";
	}
	paramTypeStr += colName;
	valueStr += FormatString("%i", value);
}
void PostgreSQLInterface::EncodeQueryInput(const char *colName, bool value, RakNet::RakString &paramTypeStr, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat)
{
	if (paramTypeStr.IsEmpty()==false)
	{
		paramTypeStr += ", ";
		valueStr += ", ";
	}
	paramTypeStr += colName;
	if (value)
		valueStr += "true";
	else
		valueStr += "false";
}
void PostgreSQLInterface::EncodeQueryUpdate(const char *colName, int value, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat)
{
	if (valueStr.IsEmpty()==false)
	{
		valueStr += ", ";
	}
	valueStr += colName;
	valueStr += " = ";
	valueStr += FormatString("%i", value);
}
void PostgreSQLInterface::EncodeQueryInput(const char *colName, float value, RakNet::RakString &paramTypeStr, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat)
{
	if (paramTypeStr.IsEmpty()==false)
	{
		paramTypeStr += ", ";
		valueStr += ", ";
	}
	paramTypeStr += colName;
	valueStr += FormatString("%f", value);
}
void PostgreSQLInterface::EncodeQueryUpdate(const char *colName, float value, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat)
{
	if (valueStr.IsEmpty()==false)
	{
		valueStr += ", ";
	}
	valueStr += colName;
	valueStr += " = ";
	valueStr += FormatString("%f", value);
}
void PostgreSQLInterface::EncodeQueryInput(const char *colName, char *binaryData, int binaryDataLength, RakNet::RakString &paramTypeStr, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat, bool writeEmpty)
{
	if (writeEmpty==false && (binaryData==0 || binaryDataLength==0))
		return;

	if (binaryData==0)
		binaryDataLength=0;

	if (paramTypeStr.IsEmpty()==false)
	{
		paramTypeStr += ", ";
		valueStr += ", ";
	}
	paramTypeStr += colName;
	valueStr+=FormatString("$%i::bytea", numParams+1);

	paramData[numParams]=binaryData;
	paramLength[numParams]=binaryDataLength;
	paramFormat[numParams]=PQEXECPARAM_FORMAT_BINARY;
	numParams++;
}
void PostgreSQLInterface::EncodeQueryUpdate(const char *colName, char *binaryData, int binaryDataLength, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat)
{
	if (binaryData==0 || binaryDataLength==0)
		return;

	if (binaryData==0)
		binaryDataLength=0;

	if (valueStr.IsEmpty()==false)
	{
		valueStr += ", ";
	}
	valueStr += colName;
	valueStr += " = ";
	valueStr+=FormatString("$%i::bytea", numParams+1);

	paramData[numParams]=binaryData;
	paramLength[numParams]=binaryDataLength;
	paramFormat[numParams]=PQEXECPARAM_FORMAT_BINARY;
	numParams++;
}
void PostgreSQLInterface::EncodeQueryInput(const char *colName, const char *str, RakNet::RakString &paramTypeStr, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat, bool writeEmpty, const char *type)
{
	if (writeEmpty==false && (str==0 || str[0]==0))
		return;

	static char *emptyStr="";
	static char *emptyDate="01/01/01";

	if (paramTypeStr.IsEmpty()==false)
	{
		paramTypeStr += ", ";
		valueStr += ", ";
	}
	paramTypeStr += colName;
	valueStr+=FormatString("$%i::%s", numParams+1, type);

	if (writeEmpty && (str==0 || str[0]==0))
	{
		if (strcmp(type,"date")==0)
		{
			paramData[numParams]=emptyDate;
			paramLength[numParams]=(int)strlen(emptyDate);
		}
		else
		{
			paramData[numParams]=emptyStr;
			paramLength[numParams]=0;
		}		
	}
	else
	{
		paramData[numParams]=(char*) str;
		paramLength[numParams]=(int) strlen(str);
	}	
	paramFormat[numParams]=PQEXECPARAM_FORMAT_TEXT;
	numParams++;
}
void PostgreSQLInterface::EncodeQueryUpdate(const char *colName, const char *str, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat, const char *type)
{
	if (str==0 || str[0]==0)
		return;

	if (valueStr.IsEmpty()==false)
	{
		valueStr += ", ";
	}
	valueStr += colName;
	valueStr+=" = ";
	valueStr+=FormatString("$%i::%s", numParams+1, type);

	paramData[numParams]=(char*) str;
	paramLength[numParams]=(int) strlen(str);

	paramFormat[numParams]=PQEXECPARAM_FORMAT_TEXT;
	numParams++;
}
void PostgreSQLInterface::EncodeQueryInput(const char *colName, const RakNet::RakString &str, RakNet::RakString &paramTypeStr, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat, bool writeEmpty, const char *type)
{
	EncodeQueryInput(colName, str.C_String(), paramTypeStr, valueStr, numParams, paramData, paramLength, paramFormat, writeEmpty, type);
}
void PostgreSQLInterface::EncodeQueryUpdate(const char *colName, const RakNet::RakString &str, RakNet::RakString &valueStr, int &numParams, char **paramData, int *paramLength, int *paramFormat, const char *type)
{
	EncodeQueryUpdate(colName, str.C_String(), valueStr, numParams, paramData, paramLength, paramFormat, type);
}
RakNet::RakString PostgreSQLInterface::GetEscapedString(const char *input) const
{
	unsigned long len = (unsigned long) strlen(input);
	char *fn = new char [len*2+1];
	int error;
	PQescapeStringConn(pgConn, fn, input, len, &error);
	RakNet::RakString output;
	// Use assignment so it doesn't parse printf escape strings
	output = fn;
	delete [] fn;
	return output;
}
struct TypeMapping
{
	char inputType;
	const char *type;
};
static const int NUM_TYPE_MAPPINGS=7;
static TypeMapping typeMappings[NUM_TYPE_MAPPINGS] =
{
	{'i', "int"},
	{'d', "int"},
	{'s', "text"},
	{'b', "bool"},
	{'f', "numeric"},
	{'g', "numeric"},
	{'c', "bytea"},
};
unsigned int GetTypeMappingIndex(char c)
{
	unsigned int i;
	for (i=0; i < NUM_TYPE_MAPPINGS; i++ )
		if (typeMappings[i].inputType==c)
			return i;
	return -1;
}
struct IndexAndType
{
	unsigned int strIndex;
	unsigned int typeMappingIndex;
};
void GetTypeMappingIndices( const char *format, DataStructures::List<IndexAndType> &indices )
{
	bool previousCharWasPercentSign;
	unsigned int i;
	unsigned int typeMappingIndex;
	indices.Clear();
	unsigned int len = (unsigned int) strlen(format);
	previousCharWasPercentSign=false;
	bool wroteLeftParen=false;
	for (i=0; i < len; i++)
	{
		if (format[i]=='%')
		{
			previousCharWasPercentSign=true;
		}
		else
		{
			if (previousCharWasPercentSign==true)
			{
				typeMappingIndex = GetTypeMappingIndex(format[i]);
				if (typeMappingIndex!=-1)
				{
					IndexAndType iat;
					iat.strIndex=i-1;
					iat.typeMappingIndex=typeMappingIndex;
					indices.Insert(iat);
				}
			}

			previousCharWasPercentSign=false;
		}
	}
}
PGresult * PostgreSQLInterface::ExecVaridic( const char * input, ... )
{
	RakNet::RakString query;
	PGresult *result;
	unsigned int i;
	DataStructures::List<IndexAndType> indices;
	if ( input==0 || input[0]==0 )
		return 0;
//	unsigned int preparedStatementIndex;
	/*
	for (preparedStatementIndex=0; preparedStatementIndex < preparedStatements.Size(); preparedStatementIndex++)
	{
		if (stricmp(preparedStatements[preparedStatementIndex].C_String(), input)==0)
			break;
	}
	RakNet::RakString preparedStatementId;
	preparedStatementId = RakNet::RakString("__AUTO_PREPARED_%i", preparedStatementIndex);
	if (preparedStatementIndex==preparedStatements.Size())
	{
		query = "PREPARE " + preparedStatementId;
		GetTypeMappingIndices( input, indices );
		if (indices.Size()>0)
			query += " (";
		RakNet::RakString formatCopy;
		formatCopy=input;
		for (i=0; i < indices.Size(); i++)
		{
			if (i!=0)
				query += ",";
			query+=typeMappings[indices[i].typeMappingIndex].type;
			formatCopy.SetChar(indices[i].strIndex, '$');
			if (i < 9)
				formatCopy.SetChar(indices[i].strIndex+1, i+1+'0');
			else
				formatCopy.SetChar(indices[i].strIndex+1, RakNet::RakString("%i", i+1));
		}
		if (indices.Size()>0)
			query += ") ";
		query += " AS ";
		query += formatCopy;
		if (ExecuteBlockingCommand(query.C_String(), &result, false)==false)
		{
			PQclear(result);
			return false;
		}
		PQclear(result);
		preparedStatements.Insert(preparedStatementId);
	}

	// Find out how many params there are
	// Find out the type of each param (%f, %s)
	GetTypeMappingIndices( input, indices );

	// Type mapping indices and this query could be stored as an optimization
	query = "EXECUTE " + preparedStatementId + "(";
	for (i=0; i < indices.Size(); i++)
	{
		if (i!=0)
			query+=',';
		query+='$';
		query+=RakNet::RakString("%i::", i+1);
		query+=typeMappings[indices[i].typeMappingIndex].type;
	}
	query += ')';
	*/

	// Find out how many params there are
	// Find out the type of each param (%f, %s)
	GetTypeMappingIndices( input, indices );

	char *paramData[512];
	int paramLength[512];
	int paramFormat[512];

	va_list argptr;
	int varidicArgIndex;
	va_start(argptr, input);
	for (varidicArgIndex=0, i=0; i < indices.Size(); i++, varidicArgIndex++)
	{
		if (typeMappings[indices[i].typeMappingIndex].inputType=='i' ||
			typeMappings[indices[i].typeMappingIndex].inputType=='d')
		{
			int val = va_arg( argptr, int );
			paramData[i]=(char*) &val;
			paramLength[i]=sizeof(val);
		}
		else if (typeMappings[indices[i].typeMappingIndex].inputType=='s')
		{
			char* val = va_arg( argptr, char* );
			paramData[i]=val;
			paramLength[i]=(int) strlen(val);
		}
		else if (typeMappings[indices[i].typeMappingIndex].inputType=='b')
		{
			bool val = va_arg( argptr, bool );
			paramData[i]=(char*) &val;
			paramLength[i]=sizeof(bool);
		}
		else if (typeMappings[indices[i].typeMappingIndex].inputType=='f')
		{
			float val = va_arg( argptr, float );
			paramData[i]=(char*) &val;
			paramLength[i]=sizeof(float);
		}
		else if (typeMappings[indices[i].typeMappingIndex].inputType=='g')
		{
			double val = va_arg( argptr, double );
			paramData[i]=(char*) &val;
			paramLength[i]=sizeof(double);
		}
		else if (typeMappings[indices[i].typeMappingIndex].inputType=='c')
		{
			char* val = va_arg( argptr, char* );
			paramData[i]=val;
			varidicArgIndex++;
			va_start(argptr, varidicArgIndex);
			paramLength[i]=va_arg( argptr, unsigned int );
		}
		paramFormat[i]=PQEXECPARAM_FORMAT_BINARY;
	}

	// Replace each %whatever with $index::type
	RakNet::RakString inputCopy;
	inputCopy=input;
	unsigned int lastIndex=0;
	for (i=0; i < indices.Size(); i++)
	{
		query+=inputCopy.SubStr(lastIndex, indices[i].strIndex-lastIndex);
		query+=RakNet::RakString("$%i::", i+1);
		query+=typeMappings[indices[i].typeMappingIndex].type;
		lastIndex=indices[i].strIndex+2; // +2 is to skip the %whateverCharacter
	}
	query+=inputCopy.SubStr(lastIndex, (unsigned int)-1);

	result = PQexecParams(pgConn, query.C_String(),indices.Size(),0,paramData,paramLength,paramFormat,PQEXECPARAM_FORMAT_BINARY);
	if (IsResultSuccessful(result, false)==false)
	{
		PQclear(result);
		return 0;
	}
	return result;
}
void PostgreSQLInterface::ClearResult(PGresult *result)
{
	PQclear(result);
}