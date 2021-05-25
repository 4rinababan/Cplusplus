#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include <cpr/cpr.h>
#include <curl/curl.h>
#include "json.hpp"
#include <windows.data.json.h>

#define DLL_EXPORT
#include "head.h"

namespace
{
	std::size_t callback(
		const char* in,
		std::size_t size,
		std::size_t num,
		std::string* out)
	{
		const std::size_t totalBytes(size * num);
		out->append(in, totalBytes);
		return totalBytes;
	}
}
//using namespace tiodbc;
using namespace std;
string USERID = "USER";
string PASSWORD = "PASS";
string szTSP;
string szDeviceID;
string szTYPE;
string odbcname;
string odbcuser;
string odbcpass;
string MapDrive;
string UploadPath;
string UploadPathforUpload;
string host;
char type[10];
string typ;
int connecting = 0;
int login = 1;
int checkroute = 0;
int testresult = 0;
int upload = 0;
int repair = 0;
int skip = 0;
int returnvalue = 1;
int isrepair = 0;
time_t now;
//statement query;


wstring SW(const string str)
{
	wstring a(str.begin(), str.end());
	return a;
}
wstring CW(const char* ch)
{
	string _a(ch);
	wstring a(_a.begin(), _a.end());
	return a;
}
string CS(const char* ch)
{
	string _a(ch);
	return _a;
}
string IS(const char* ch)
{
	char buffer[10];
	int i = 0;
	i = atoi(ch);
	itoa(i, buffer, 10);
	string _a(buffer);
	return _a;
}
LPCWSTR CLW(const char* ch)
{
	wstring a = CW(ch);
	LPCWSTR _a = a.c_str();
	return _a;
}
string GetFileName(const char* ch)
{
	string t(ch);
	int first = t.find_last_of('\\') + 1;
	t = t.substr(first);
	return t;
}
void createFolder(const string path)
{
	DWORD a = GetFileAttributesA(path.c_str());
	string mkdir("mkdir ");
	char dirpath[1024];
	mkdir += path;
	strcpy_s(dirpath, sizeof(dirpath), mkdir.c_str());
	if (a == INVALID_FILE_ATTRIBUTES)
	{
		system(dirpath);
	}
}

void clear()
{
	szTSP = "";
	szDeviceID = "";
	odbcname = "";
	odbcuser = "";
	odbcpass = "";
	MapDrive = "";
	UploadPath = "";
	UploadPathforUpload = "";
	connecting = 0;
	login = 0;
	checkroute = 0;
	testresult = 0;
	upload = 0;
	repair = 0;
	skip = 0;
	returnvalue = 0;
	isrepair = 0;
	//query.close();
}

DECLDIR int __stdcall SATNUSA_SFIS_Connect(SATNUSA_SFIS_Interface *dev)
{
	clear();
	ifstream file(dev->szINIFile);
	if (!file)
	{
		connecting = 0;
		strcpy_s(dev->szMessage, sizeof(dev->szMessage), "INI FILE NOT FOUND.");
		dev->iReturn = SATNUSA_SFIS_CONNECT_FAIL;
		return SATNUSA_SFIS_CONNECT_FAIL;
	}
	if (file)
	{
		string line;
		int i = 0;
		while (getline(file, line))
		{
			if (i == 0) { szTSP = line; }
			if (i == 1) { szDeviceID = line; }
			if (i == 2) { szTYPE = line; }
			if (i == 3) { odbcname = line; }
			if (i == 4) { odbcuser = line; }
			if (i == 5) { odbcpass = line; }
			if (i == 6) { MapDrive = line; }
			if (i == 7) { host = line; }
			i++;
		}

		strcpy_s(dev->szTSP, sizeof(dev->szTSP), szTSP.c_str());
		strcpy_s(dev->szDeviceID, sizeof(dev->szDeviceID), szDeviceID.c_str());
		//connection con(SW(odbcname), SW(odbcuser), SW(odbcpass));
		//if (con.connected())
		if (checkConection())
		{

			/*wstring W = SW("spSFIS 0,'','" + szTSP + "','" + szDeviceID + "',0,'','" + szTYPE + "'");
			if (query.execute_direct(con, W.c_str()))
			{
				while (query.fetch_next())
				{*/
			returnvalue = 0;
			dev->iReturn = SATNUSA_SFIS_SUCCESS;//query.field(2).as_unsigned_long();
			//sprintf_s(dev->szMessage, "%ls","CONNECTION SUCCESS");//query.field(3).as_string().c_str());
			//sprintf_s(dev->szMessage, sizeof(dev->szMessage), "CONNECT SUCCESSFULLY.");
			strcpy_s(dev->szMessage, sizeof(dev->szMessage), "CONNECT SUCCESS");
			dev->iCheckRouteType = 0;
			/*isrepair = query.field(5).as_short();*/
		//}
			if (returnvalue == 0) { connecting = 1; }
			if (returnvalue == 1) { connecting = 0; }
			return SATNUSA_SFIS_SUCCESS;
			////}
			//else
			//{
			//	connecting = 0;
			//	strcpy_s(dev->szMessage, sizeof(dev->szMessage), "EXECUTE COMMAND FAIL. PLEASE CHECK YOUR SFIS SETTINGS.");
			//	dev->iReturn = SATNUSA_SFIS_CONNECT_FAIL;
			//	return SATNUSA_SFIS_CONNECT_FAIL;
			//}
		}
		else
		{
			connecting = 0;
			strcpy_s(dev->szMessage, sizeof(dev->szMessage), "CONNECT TO DATABASE FAIL. PLEASE CHECK YOUR CONNECTION SETTINGS.");
			dev->iReturn = SATNUSA_SFIS_CONNECT_FAIL;
			return SATNUSA_SFIS_CONNECT_FAIL;
		}
	}




}
DECLDIR int __stdcall SATNUSA_SFIS_Disconnect(void)
{
	clear();
	return SATNUSA_SFIS_SUCCESS;


}
DECLDIR int __stdcall SATNUSA_SFIS_Login(SATNUSA_SFIS_Interface *dev)
{
	return 0;
	if (connecting == 0)
	{
		login = 0;
		strcpy_s(dev->szMessage, sizeof(dev->szMessage), "DLL NOT CONNECTED. PLEASE CONNECT FIRST.");
		dev->iReturn = SATNUSA_SFIS_LOGIN_FAIL;
		return SATNUSA_SFIS_LOGIN_FAIL;
	}
	/*if(strlen(dev->szUserID) <=0)
	{
	login = 0;
	strcpy_s(dev->szMessage,sizeof(dev->szMessage),"USERNAME IS EMPTY.");
	dev->iReturn = SATNUSA_SFIS_LOGIN_FAIL;
	return SATNUSA_SFIS_LOGIN_FAIL;
	}
	if(dev->szUserID != USERID)
	{
	login = 0;
	strcpy_s(dev->szMessage,sizeof(dev->szMessage),"WRONG USERNAME. DEFAULT IS USER");
	dev->iReturn = SATNUSA_SFIS_LOGIN_FAIL;
	return SATNUSA_SFIS_LOGIN_FAIL;
	}
	if(strlen(dev->szUserPassword) <=0)
	{
	login = 0;
	strcpy_s(dev->szMessage,sizeof(dev->szMessage),"PASSWORD IS EMPTY.");
	dev->iReturn = SATNUSA_SFIS_LOGIN_FAIL;
	return SATNUSA_SFIS_LOGIN_FAIL;
	}
	if(dev->szUserPassword != PASSWORD)
	{
	login = 0;
	strcpy_s(dev->szMessage,sizeof(dev->szMessage),"WRONG PASSWORD. DEFAULT IS PASS");
	dev->iReturn = SATNUSA_SFIS_LOGIN_FAIL;
	return SATNUSA_SFIS_LOGIN_FAIL;
	}
	if(dev->iLoginMode == NULL)
	{
	login = 0;
	strcpy_s(dev->szMessage,sizeof(dev->szMessage),"LOGIN MODE NOT DEFINED.");
	dev->iReturn = SATNUSA_SFIS_LOGIN_FAIL;
	return SATNUSA_SFIS_LOGIN_FAIL;
	}*/
	if (connecting == 1)
	{
		//connection con(SW(odbcname),SW(odbcuser),SW(odbcpass));
		if (checkConection())
		{
			////wstring W = SW("spSFIS 2,'','" + szTSP + "','" + szDeviceID + "',0,'','" + szTYPE + "'");	
			//auto response = cpr::Post(cpr::Url{ host },
			////cpr::Authentication{ "userId", "pass" },
			//cpr::Parameters{ {"username", odbcname},{"password",odbcpass},{"station_name",szDeviceID},{"device_name",szTSP} });
			//auto json = nlohmann::json::parse(response.text);

			returnvalue = 0;
			dev->iReturn = 0;
			/*sprintf_s(dev->szMessage,"%ls", json.at(0)["message"]);
			dev->iCheckRouteType = json.at(0)["check_route"];
			isrepair = json.at(0)["is_repair"];*/

			if (returnvalue == 0) { login = 1; }
			if (returnvalue == 1) { login = 0; }
			return returnvalue;
		}
		else
		{
			connecting = 0;
			login = 0;
			strcpy_s(dev->szMessage, sizeof(dev->szMessage), "EXECUTE COMMAND FAIL. PLEASE CHECK YOUR SFIS SETTINGS.");
			dev->iReturn = SATNUSA_SFIS_LOGIN_FAIL;
			return SATNUSA_SFIS_LOGIN_FAIL;
		}
	}
	else
	{
		connecting = 0;
		login = 0;
		strcpy_s(dev->szMessage, sizeof(dev->szMessage), "CONNECT TO DATABASE FAIL. PLEASE CHECK YOUR CONNECTION SETTINGS.");
		dev->iReturn = SATNUSA_SFIS_LOGIN_FAIL;
		return SATNUSA_SFIS_LOGIN_FAIL;
	}

}
DECLDIR int __stdcall SATNUSA_SFIS_GetInfo(SATNUSA_SFIS_Interface *dev, MAP_SOURCE *map)
{
	/*if(login ==0)
		{
			checkroute = 0;
			strcpy_s(dev->szMessage,sizeof(dev->szMessage),"DLL NOT LOGGED. PLEASE LOGIN FIRST.");
			dev->iReturn = SATNUSA_SFIS_GETINFO_FAIL;
			return SATNUSA_SFIS_GETINFO_FAIL;
		}
		if(strlen(dev->szISN) <=0)
		{
		checkroute = 0;
		strcpy_s(dev->szMessage,sizeof(dev->szMessage), ("ISN / SSN IS EMPTY : Data passing = " + CS(dev->szISN)).c_str());
		dev->iReturn = SATNUSA_SFIS_GETINFO_FAIL;
		return SATNUSA_SFIS_GETINFO_FAIL;
		}
		if (login ==1)
		{
		connection con(SW(odbcname),SW(odbcuser),SW(odbcpass));
		if(con.connected())
		{

		wstring W = SW("spSFIS 3,'" + CS(dev->szISN) + "','" + szTSP + "','" + szDeviceID + "',0,'','" + szTYPE + "'");
		if(query.execute_direct(con,W.c_str()))
		{
				while(query.fetch_next())
				{
					if(query.count_columns()>29)
					{

						sprintf_s(map->SSN, "%ls", query.field(1).as_string().c_str());
						sprintf_s(map->ISN, "%ls", query.field(2).as_string().c_str());
						sprintf_s(map->Typ, "%ls", query.field(3).as_string().c_str());
						sprintf_s(map->IMEI, "%ls", query.field(4).as_string().c_str());
						sprintf_s(map->IMEI2, "%ls", query.field(5).as_string().c_str());
						sprintf_s(map->BTMAC, "%ls", query.field(6).as_string().c_str());
						sprintf_s(map->WIFIMAC, "%ls", query.field(7).as_string().c_str());
						sprintf_s(map->ColorID, "%ls", query.field(8).as_string().c_str());
						sprintf_s(map->CountryCode, "%ls", query.field(9).as_string().c_str());
						sprintf_s(map->CustomerID, "%ls", query.field(10).as_string().c_str());
						sprintf_s(map->UnlockCode, "%ls", query.field(11).as_string().c_str());
						sprintf_s(map->MapID, "%ls", query.field(12).as_string().c_str());
						sprintf_s(map->WIFI_Region, "%ls", query.field(13).as_string().c_str());
						sprintf_s(map->OS_Version, "%ls", query.field(14).as_string().c_str());
						sprintf_s(map->Modem_version, "%ls", query.field(15).as_string().c_str());
						sprintf_s(map->ExtensionRom_version, "%ls", query.field(16).as_string().c_str());
						sprintf_s(map->MapString, "%ls", query.field(17).as_string().c_str());
						sprintf_s(map->MPversion, "%ls", query.field(18).as_string().c_str());
						sprintf_s(map->MapCheckSum, "%ls", query.field(19).as_string().c_str());
						sprintf_s(map->SIMLockType, "%ls", query.field(20).as_string().c_str());
						sprintf_s(map->MCCMNC, "%ls", query.field(21).as_string().c_str());
						sprintf_s(map->SIMLockKey, "%ls", query.field(22).as_string().c_str());
						sprintf_s(map->SIMLockUnblockKey, "%ls", query.field(23).as_string().c_str());
						sprintf_s(map->KeyBox, "%ls", query.field(24).as_string().c_str());
						sprintf_s(map->SBK, "%ls", query.field(25).as_string().c_str());
						sprintf_s(map->Certificate, "%ls", query.field(26).as_string().c_str());
						sprintf_s(map->shippingFile, "%ls", query.field(27).as_string().c_str());
						sprintf_s(map->Voucher, "%ls", query.field(28).as_string().c_str());
						sprintf_s(map->RamSize, "%ls", query.field(29).as_string().c_str());
						sprintf_s(map->EmmcSize, "%ls", query.field(30).as_string().c_str());
						sprintf_s(map->CalFront, "%ls", query.field(31).as_string().c_str());
						sprintf_s(map->CalRear, "%ls", query.field(32).as_string().c_str());
						sprintf_s(map->SystemMd5, "%ls", query.field(33).as_string().c_str());
						sprintf_s(map->JobID, "%ls", query.field(34).as_string().c_str());
						sprintf_s(map->createdDate, "%ls", query.field(35).as_string().c_str());
						sprintf_s(map->HW, "%ls", query.field(36).as_string().c_str());
						sprintf_s(map->MEID, "%ls", query.field(37).as_string().c_str());
						sprintf_s(map->AttestationKey, "%ls", query.field(38).as_string().c_str());
						sprintf_s(map->VoucherPath, "%ls", query.field(39).as_string().c_str());
						sprintf_s(map->KeyBoxPath, "%ls", query.field(40).as_string().c_str());
						sprintf_s(map->LatestOsVersion, "%ls", query.field(41).as_string().c_str());
						sprintf_s(map->ATTESTPATH, "%ls", query.field(42).as_string().c_str());
						sprintf_s(map->A90PN, "%ls", query.field(43).as_string().c_str());
						sprintf_s(map->SIM, "%ls", query.field(44).as_string().c_str());
						sprintf_s(map->ModelName, "%ls", query.field(45).as_string().c_str());
						sprintf_s(map->PartName, "%ls", query.field(46).as_string().c_str());
						strcpy_s(dev->szMessage,sizeof(dev->szMessage),"GET INFO SUCCESFUL.");
					}
					else
					{
					returnvalue = query.field(1).as_short();
					dev->iReturn = query.field(2).as_unsigned_long();
					sprintf_s(dev->szMessage,"%ls",query.field(3).as_string().c_str());
					dev->iCheckRouteType = query.field(4).as_short();
					isrepair = query.field(5).as_short();
					}
				}

		return returnvalue;
		}
		else
		{
		strcpy_s(dev->szMessage,sizeof(dev->szMessage),"EXECUTE COMMAND FAIL. PLEASE CHECK YOUR SFIS SETTINGS.");
		dev->iReturn = SATNUSA_SFIS_GETINFO_FAIL;
		return SATNUSA_SFIS_GETINFO_FAIL;
		}
		}
		else
		{
		strcpy_s(dev->szMessage,sizeof(dev->szMessage),"CONNECT TO DATABASE FAIL. PLEASE CHECK YOUR CONNECTION SETTINGS.");
		dev->iReturn = SATNUSA_SFIS_GETINFO_FAIL;
		return SATNUSA_SFIS_GETINFO_FAIL;
		}
		}*/
	return SATNUSA_SFIS_GETINFO_FAIL;
}

DECLDIR int __stdcall SATNUSA_SFIS_CheckRoute(SATNUSA_SFIS_Interface *dev)
{
	if (login == 0)
	{
		checkroute = 0;
		strcpy_s(dev->szMessage, sizeof(dev->szMessage), "DLL NOT LOGGED. PLEASE LOGIN FIRST.");
		dev->iReturn = SATNUSA_SFIS_CHECKROUTE_FAIL;
		return SATNUSA_SFIS_CHECKROUTE_FAIL;
	}
	if (strlen(dev->szISN) <= 0)
	{
		checkroute = 0;
		strcpy_s(dev->szMessage, sizeof(dev->szMessage), ("ISN / SSN IS EMPTY : Data passing = " + CS(dev->szISN)).c_str());
		//strcpy_s(dev->szMessage,sizeof(dev->szMessage),"ISN/SSN IS EMPTY.");
		dev->iReturn = SATNUSA_SFIS_CHECKROUTE_FAIL;
		return SATNUSA_SFIS_CHECKROUTE_FAIL;
	}
	if (login == 1)
	{
		if (checkConection())
		{
			auto response = cpr::Post(cpr::Url{ host },
				//cpr::Authentication{ "userId", "pass" },
				cpr::Parameters{ {"scan_item", CS(dev->szISN)},{"station_name", szDeviceID},{"device_name", szTSP} });
			auto json = nlohmann::json::parse(response.text);

			if (json.at(0)["success"] == "true")
			{
				returnvalue = 0;
				dev->iReturn = json.at(0)["return"];
				//sprintf_s(dev->szMessage, "%ls", json.at(0)["message"]);
				//strcpy_s(dev->szMessage, sizeof(dev->szMessage), json.at(0)["message"]);
				dev->iCheckRouteType = 1;
				isrepair = json.at(0)["is_repair"];
				sprintf_s(type, "%ls", json.at(0)["type"]);
				typ = string(type);
			}
			else
			{
				returnvalue = 1;
				dev->iReturn = json.at(0)["return"];
				sprintf_s(dev->szMessage, "%ls", json.at(0)["message"]);
				dev->iCheckRouteType = 0;
				isrepair = json.at(0)["is_repair"];
				sprintf_s(type, "%ls", json.at(0)["type"]);
				typ = string(type);
			}


			if (returnvalue == 0) { checkroute = 1; }
			if (returnvalue == 1) { checkroute = 0; }

			now = time(0);
			char tim[255];
			tm *ltm = localtime(&now);
			strftime(tim, sizeof(tim), "%Y%m%d", ltm);
			string _tim(tim);
			UploadPath = MapDrive + "\\" + typ + "\\" + szDeviceID + "\\" + _tim + "\\" + szTSP + "\\";
			UploadPathforUpload = MapDrive + "\\" + typ + "\\" + szDeviceID + "\\" + _tim + "\\" + szTSP + "\\UPLOAD\\";
			createFolder(UploadPath);
			createFolder(UploadPathforUpload);
			strcpy_s(dev->szUploadFilePath, sizeof(dev->szUploadFilePath), UploadPathforUpload.c_str());

			return returnvalue;
		}
		else
		{
			checkroute = 0;
			strcpy_s(dev->szMessage, sizeof(dev->szMessage), "CONNECT TO DATABASE FAIL. PLEASE CHECK YOUR CONNECTION SETTINGS.");
			dev->iReturn = SATNUSA_SFIS_CHECKROUTE_FAIL;
			return SATNUSA_SFIS_CHECKROUTE_FAIL;
		}
	}
	else
	{
		checkroute = 0;
		strcpy_s(dev->szMessage, sizeof(dev->szMessage), "CONNECT TO DATABASE FAIL. PLEASE LOGIN FIRST.");
		dev->iReturn = SATNUSA_SFIS_CHECKROUTE_FAIL;
		return SATNUSA_SFIS_CHECKROUTE_FAIL;
	}
}

DECLDIR int __stdcall SATNUSA_SFIS_TestResult(SATNUSA_SFIS_Interface *dev)
{
	if (checkroute == 0)
	{
		strcpy_s(dev->szMessage, sizeof(dev->szMessage), "CHECKROUTE FAIL. PLEASE CHECKROUTE FIRST.");
		dev->iReturn = SATNUSA_SFIS_TESTRESULT_FAIL;
		return SATNUSA_SFIS_TESTRESULT_FAIL;
	}
	ifstream source(dev->szLogFile);
	if (!source)
	{
		testresult = 0;
		string pathtemp("SOURCE FILE NOT EXIST > ");
		pathtemp += string(dev->szLogFile);
		strcpy_s(dev->szMessage, sizeof(dev->szMessage), pathtemp.c_str());
		dev->iReturn = SATNUSA_SFIS_TESTRESULT_FAIL;
		return SATNUSA_SFIS_TESTRESULT_FAIL;
	}
	else
	{
		ifstream src(dev->szLogFile, ios::binary);
		char outputfile[1024];
		string pathtemp("output/");
		pathtemp += string(GetFileName(dev->szLogFile));
		strcpy_s(outputfile, sizeof(outputfile), pathtemp.c_str());
		ofstream dst(outputfile, ios::binary);
		dst << src.rdbuf();


	}
	source.close();
	if (checkroute == 1)
	{

		if (checkConection())
		{
			//wstring W = SW("spSFIS 5,'" + CS(dev->szISN) + "','" + szTSP + "','" + szDeviceID + "'," + dev->szErrorCode + ",'" + UploadPathforUpload  + GetFileName(dev->szLogFile) + "','" + szTYPE + "'");			
			auto response = cpr::Get(cpr::Url{ host },
				//cpr::Authentication{ "userId", "pass" },
				cpr::Parameters{ {"key_item", CS(dev->szISN)},{"device_name", szTSP},{"station_name", szDeviceID},{"error_code", dev->szErrorCode},{"log_path", UploadPathforUpload + GetFileName(dev->szLogFile)}, {"get_versions", szTYPE} });
			auto json = nlohmann::json::parse(response.text);

			if (json.at(0)["success"] == "true")
			{
				returnvalue = 0;
				dev->iReturn = 0;
				sprintf_s(dev->szMessage, "%ls", json.at(0)["message"]);
				dev->iCheckRouteType = 1;
				if (json.at(0)["is_ng_over_limit"] == "true") { isrepair = 1; }

			}
			else
			{
				returnvalue = 1;
				//dev->iReturn = json.at(0)["return"];
				sprintf_s(dev->szMessage, "%ls", json.at(0)["message"]);
				dev->iCheckRouteType = 0;

			}
			std::ifstream  src(dev->szLogFile, std::ios::binary);
			std::ofstream  dst(UploadPathforUpload + GetFileName(dev->szLogFile), std::ios::binary);
			dst << src.rdbuf();

			return returnvalue;
		}
		else
		{
			checkroute = 0;
			strcpy_s(dev->szMessage, sizeof(dev->szMessage), "EXECUTE COMMAND FAIL. PLEASE CHECK YOUR SFIS SETTINGS.");
			dev->iReturn = SATNUSA_SFIS_TESTRESULT_FAIL;
			return SATNUSA_SFIS_TESTRESULT_FAIL;
		}
	}
	else
	{
		checkroute = 0;
		strcpy_s(dev->szMessage, sizeof(dev->szMessage), "CONNECT TO DATABASE FAIL. PLEASE CHECK YOUR CONNECTION SETTINGS.");
		dev->iReturn = SATNUSA_SFIS_TESTRESULT_FAIL;
		return SATNUSA_SFIS_TESTRESULT_FAIL;
	}

}
//DECLDIR int __stdcall SATNUSA_SFIS_UploadFile(SATNUSA_SFIS_Interface *dev)
//{	
//	if(checkroute ==0)
//		{	strcpy_s(dev->szMessage,sizeof(dev->szMessage),"CHECKROUTE FAIL. PLEASE CHECKROUTE FIRST.");
//			dev->iReturn = SATNUSA_SFIS_UPLOADFILE_FAIL;
//			return SATNUSA_SFIS_UPLOADFILE_FAIL;
//		}
//		ifstream source(dev->szUploadFilePath);
//		if(!source)
//		{
//			checkroute = 0;				
//			string pathtemp("SOURCE FILE NOT EXIST > ");
//			pathtemp += string(dev->szUploadFilePath);
//			strcpy_s(dev->szMessage,sizeof(dev->szMessage),pathtemp.c_str());
//			dev->iReturn = SATNUSA_SFIS_UPLOADFILE_FAIL;
//			return SATNUSA_SFIS_UPLOADFILE_FAIL;
//		}
//		else
//		{
//			ifstream src(dev->szUploadFilePath,ios::binary);
//			char outputfile[1024];
//			string pathtemp("upload/");
//			pathtemp += string(GetFileName(dev->szUploadFilePath));
//			strcpy_s(outputfile,sizeof(outputfile),pathtemp.c_str());
//			ofstream dst(outputfile,ios::binary);
//			dst << src.rdbuf();
//			

//		}
//		source.close();
//		if (checkroute ==1)
//		{							
//			if(checkConection())
//			{		
//				auto response = cpr::Get(cpr::Url{ host },
//				//cpr::Authentication{ "userId", "pass" },
//				cpr::Parameters{ {"ssn", CS(dev->szISN)} });
//				auto json = nlohmann::json::parse(response.text);

//				//wstring W = SW("spSFIS 6,'" + CS(dev->szISN) + "','" + szTSP + "','" + szDeviceID + "'," + IS(dev->szErrorCode) + ",'" + UploadPathforUpload + GetFileName(dev->szUploadFilePath) + "','" + szTYPE + "'");			
//				returnvalue = json.at(0)["result"];
//				dev->iReturn = json.at(0)["return"];
//				sprintf_s(dev->szMessage,"%ls", json.at(0)["message"]);
//				dev->iCheckRouteType = json.at(0)["check_route"];
//				isrepair = json.at(0)["is_repair"];

//				return returnvalue;			
//			}
//			else
//			{			
//				checkroute = 0;
//				strcpy_s(dev->szMessage,sizeof(dev->szMessage),"EXECUTE COMMAND FAIL. PLEASE CHECK YOUR SFIS SETTINGS.");
//				dev->iReturn = SATNUSA_SFIS_UPLOADFILE_FAIL;
//				return SATNUSA_SFIS_UPLOADFILE_FAIL;
//			}				
//		}
//		else
//		{	
//			checkroute = 0;
//			strcpy_s(dev->szMessage,sizeof(dev->szMessage),"CONNECT TO DATABASE FAIL. PLEASE CHECK YOUR CONNECTION SETTINGS.");
//			dev->iReturn = SATNUSA_SFIS_UPLOADFILE_FAIL;
//			return SATNUSA_SFIS_UPLOADFILE_FAIL;
//		}			
//}


/*DECLDIR int __stdcall SATNUSA_SFIS_UploadMapSource(SATNUSA_SFIS_Interface *dev)
{
	if(checkroute ==0)
		{	strcpy_s(dev->szMessage,sizeof(dev->szMessage),"CHECKROUTE FAIL. PLEASE CHECKROUTE FIRST.");
			dev->iReturn = SATNUSA_SFIS_UPLOADFILE_FAIL;
			return SATNUSA_SFIS_UPLOADFILE_FAIL;
		}
		if (checkroute ==1)
		{
		connection con(SW(odbcname),SW(odbcuser),SW(odbcpass));
		if(con.connected())
		{

		wstring W = SW("spSFIS 8,'" + CS(dev->szISN) + "','" +  CS(dev->szOsVersion) + "','" + CS(dev->szCalFront) + "','','" +  CS(dev->szCalrear) + "','" + CS(dev->szHW) + "'");
		if(query.execute_direct(con,W.c_str()))
		{
				while(query.fetch_next())
				{
					returnvalue = query.field(1).as_short();
					dev->iReturn = query.field(2).as_unsigned_long();
					sprintf_s(dev->szMessage,"%ls",query.field(3).as_string().c_str());
				}

		return returnvalue;
		}
		else
		{
		checkroute = 0;
		strcpy_s(dev->szMessage,sizeof(dev->szMessage),"EXECUTE COMMAND FAIL. PLEASE CHECK YOUR SFIS SETTINGS.");
		dev->iReturn = SATNUSA_SFIS_UPLOADFILE_FAIL;
		return SATNUSA_SFIS_UPLOADFILE_FAIL;
		}
		}
		else
		{
		checkroute = 0;
		strcpy_s(dev->szMessage,sizeof(dev->szMessage),"CONNECT TO DATABASE FAIL. PLEASE CHECK YOUR CONNECTION SETTINGS.");
		dev->iReturn = SATNUSA_SFIS_UPLOADFILE_FAIL;
		return SATNUSA_SFIS_UPLOADFILE_FAIL;
		}
		}
}*/
//DECLDIR int __stdcall SATNUSA_SFIS_Repair(SATNUSA_SFIS_Interface *dev)
//{	
//	if (checkroute == 1 && dev->iCheckRouteType <= 2)
//	{
//		strcpy_s(dev->szMessage, sizeof(dev->szMessage), "CHECKROUTE PASS. NO NEED TO REPAIR.");
//		dev->iReturn = SATNUSA_SFIS_REPAIR_FAIL;
//		return SATNUSA_SFIS_REPAIR_FAIL;
//	}

//	if (checkroute == 0 && dev->iCheckRouteType >= 3 && dev->iReturn == SATNUSA_SFIS_OVER_LIMITTIMES)
//	{

//		if (checkConection())
//		{
//			//wstring W = SW("spSFIS 7,'" + CS(dev->szISN) + "','" + szTSP + "','" + szDeviceID + "'," + IS(dev->szErrorCode) + ",'','" + szTYPE + "'");
//			auto response = cpr::Get(cpr::Url{ host },
//			//cpr::Authentication{ "userId", "pass" },
//			cpr::Parameters{ {"ssn", CS(dev->szISN)} });
//			auto json = nlohmann::json::parse(response.text);

//			returnvalue = json.at(0)["return"];
//			dev->iReturn = json.at(0)["result"];
//			sprintf_s(dev->szMessage, "%ls", json.at(0)["message"]);
//			dev->iCheckRouteType = json.at(0)["check_route"];
//			isrepair = json.at(0)["is_repair"];
//				
//			if (returnvalue == 0) { skip = 1; checkroute = 1; }
//			if (returnvalue == 1) { skip = 0; }

//			return returnvalue;
//		}
//		else
//		{
//			skip = 0;
//			strcpy_s(dev->szMessage, sizeof(dev->szMessage), "CONNECT TO DATABASE FAIL. PLEASE CHECK YOUR CONNECTION SETTINGS.");
//			dev->iReturn = SATNUSA_SFIS_REPAIR_FAIL;
//			return SATNUSA_SFIS_REPAIR_FAIL;
//		}
//	}
//	else
//	{
//		skip = 0;
//		strcpy_s(dev->szMessage, sizeof(dev->szMessage), "NOT OVERLIMIT. NO NEED TO REPAIR.");
//		dev->iReturn = SATNUSA_SFIS_REPAIR_FAIL;
//		return SATNUSA_SFIS_REPAIR_FAIL;
//	}
//}


 
DECLDIR bool __stdcall checkConection(void)
{
	try
	{
		cpr::Response json = cpr::Get(cpr::Url{ host });
		json.status_code;
		if (json.status_code == 200)
		{
			//std::cout << "\nGot successful response from " << host << std::endl;
			return true;
		}
		else
		{
			return false;
			//std::cout << "Couldn't GET from " << host << " - exiting" << std::endl;
		}
	}
	catch (const std::exception&)
	{
		//std::cout << "Couldn't GET from " << host << " - exiting" << std::endl;
		return false;
	}
}



