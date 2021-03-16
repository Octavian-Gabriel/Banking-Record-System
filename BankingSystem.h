#pragma once
#include <string>
#include<vector>
class BankingSystem
{
	 std::string accountsFile;
	 std::string PathToRecords;
public:
	BankingSystem();
	~BankingSystem();
	void Start();
	bool MethodDispatcher(int option);
	bool AddRecord();
	bool OverwriteRecord();
	bool ShowData();
	bool SearchRecord(int RecordNumber);
	std::vector<std::string> SearchRecord(std::string name);
	std::string FindFileBasedOnContent(std::string givenContent);
	std::string FindFileBasedOnRecordNumber(std::string givenRecord);
	bool UpdateRecord();
	bool DeleteRecord();
	std::string AskForRecord();
	bool FileRecordExists(int recordNumber);
	bool DeleteAccountNumber(std::string accountNumber);
	bool DeleteAccountFile(std::string filePath);
	bool DisplayMenu();
	bool StringIsNumber(std::string StrOption);
};

