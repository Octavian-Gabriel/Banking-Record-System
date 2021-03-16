#include "stdafx.h"
#include "BankingSystem.h"
#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<Windows.h>
#include<filesystem>
#include<vector>
#include <stdio.h>
using std::cout;
using std::cin;
namespace fs = std::filesystem;
#define NOT_FOUND "-1"
BankingSystem::BankingSystem()
{
	std::string accountsString;;
	accountsString = fs::current_path().string();
	accountsString.append("\\accounts.txt");
	this->accountsFile = accountsString;
	std::string pathRecords;
	pathRecords = fs::current_path().string();
	pathRecords.append("\\Records");
	this->PathToRecords = pathRecords;
	
}


BankingSystem::~BankingSystem()
{
}

void BankingSystem::Start()
{
	cout << "******* Welcome to Bank Record System *******\n";
	cout << "Please select one of the options below: \n";

	
	while (DisplayMenu())
	{
		std::string option;
		cout << "Your option: ";
		cin >> option;
		if (StringIsNumber(option))
		{
			int opt = std::stoi(option);
			if (MethodDispatcher(opt))
				continue;
			else
			{
				cout << "\n Option is not available\n";
			}
		}
		else
		{
			cout << "Numbers please!\n";
		}
		
	}

}

bool BankingSystem::MethodDispatcher(int option)
{
	bool flag;
	switch (option)
	{
	case 1:
	{
		flag = AddRecord();
		return flag;
	}
	case 2:
	{
		flag = ShowData();
		return flag;
	}
	case 3:
	{
		flag = UpdateRecord();
		return flag;
	}
	case 4:
	{
		flag = this->DeleteRecord();
		return flag;
	}
	case 5:
	{
		cout << "\n Exiting program\n";
		exit(0);
	}
	default:
		flag = false;
		return flag;
	}
}

bool BankingSystem::AddRecord()
{
	std::string AccountNumber;
	while (!StringIsNumber(AccountNumber))
	{
		cout << "Enter Account Number: ";
		cin >> AccountNumber;
		if (!StringIsNumber(AccountNumber))
		{
			cout << AccountNumber << " is not a number\n";
		}
	}
	
	int record = std::stoi(AccountNumber);
	if (FileRecordExists(record))
	{
		cout << "This record already exists!\n\n";
		return true;

	}
	else
	{

		std::string FirstName{};
		std::string LastName{};
		std::string balance{};
		cout << "Enter First Name: ";
		cin >> FirstName;
		cout << "Enter Last Name: ";
		cin >> LastName;
		while (!StringIsNumber(balance))
		{
			cout << "Enter balance: ";
			cin >> balance;
		}
		int balanceNumber = std::stoi(balance);
		std::fstream RecordFile, AccFile;
		AccFile.open(this->accountsFile, std::ios::app);
		AccFile << '\n' << AccountNumber;
		AccFile.close();

		AccountNumber.append(".txt");
		std::string RecordPath = this->PathToRecords;

		RecordPath.append("\\").append(AccountNumber);
		RecordFile.open(RecordPath, std::ios::out);
		if (RecordFile << AccountNumber[0] << '\n' << FirstName << '\n' << LastName << '\n' << balance)
			cout << "\nRecord added succesfully\n\n";
		RecordFile.close();
	}
	return true;
}

bool BankingSystem::OverwriteRecord()
{
	std::string AccountNumber;
	while (!StringIsNumber(AccountNumber))
	{
		cout << "Enter Account Number: ";
		cin >> AccountNumber;
		if (!StringIsNumber(AccountNumber))
		{
			cout << AccountNumber << " is not a number\n";
		}
	}
	this->DeleteAccountNumber(AccountNumber);
	int record = std::stoi(AccountNumber);
	if (FileRecordExists(record))
	{
		cout << "This record already exists!\n\n";
		return true;

	}
	else
	{

		std::string FirstName{};
		std::string LastName{};
		std::string balance{};
		cout << "Enter First Name: ";
		cin >> FirstName;
		cout << "Enter Last Name: ";
		cin >> LastName;
		while (!StringIsNumber(balance))
		{
			cout << "Enter balance: ";
			cin >> balance;
		}
		int balanceNumber = std::stoi(balance);
		std::fstream RecordFile, AccFile;
		AccFile.open(this->accountsFile, std::ios::app);
		AccFile << '\n' << AccountNumber;
		AccFile.close();
		AccountNumber.append(".txt");
		std::string RecordPath = this->PathToRecords;

		RecordPath.append("\\").append(AccountNumber);
		RecordFile.open(RecordPath, std::ios::out);
		if (RecordFile << AccountNumber[0] << '\n' << FirstName << '\n' << LastName << '\n' << balance)
			cout << "\nRecord added succesfully\n\n";
		RecordFile.close();
	}
	return true;
}

bool BankingSystem::ShowData()
{
	std::string record = AskForRecord();
	if (StringIsNumber(record))
	{
		int rec = std::stoi(record);
		if (SearchRecord(rec))
		{
			cout << "\n Record exists \n\n";
			std::string foundContent{ this->FindFileBasedOnRecordNumber(record) };
			foundContent.erase(foundContent.length() - 4, foundContent.length());
			cout << foundContent << '\n\n';
		}
			
		
	}
	else
	{
		std::vector<std::string> results;
		results = SearchRecord(record);
		if (!results.size())
			cout << "\n Record doesn't exist \n\n";
		else
		{
			if (results.size() == 1)
			{
				cout << "\n Record exists \n\n";
				for (auto result : results)
					cout << '\n' << result << '\n' << '\n';
			}
			else
			{
				cout << '\n' << "Multiple results found!" << '\n';
				for (auto result : results)
					cout << '\n' << result << '\n' << '\n';
			}
			
		}
	}
		
	return true;
}

bool BankingSystem::SearchRecord(int RecordNumber)
{
	//int record=AskForRecord();
	//_chdir(this->NormalPath);
	return this->FileRecordExists(RecordNumber);
}

std::vector<std::string> BankingSystem::SearchRecord(std::string name)
{
	std::vector<std::string> results;
	std::string path;
	for (const auto & entry : fs::directory_iterator(this->PathToRecords))
	{
		path = entry.path().string();
		std::ifstream f;
		f.open(path, std::ios::in | std::ios::binary);
		const std::size_t& size = std::filesystem::file_size(path);
		std::string content(size, '\0');
		f.read(content.data(), size);
		if (content.find(name) != std::string::npos)
		{
			results.push_back(content);
		}
		
	}
	return results;
}

std::string BankingSystem::FindFileBasedOnContent(std::string givenContent)
{
	std::string path;
	for (const auto & entry : fs::directory_iterator(this->PathToRecords))
	{
		path = entry.path().string();
		std::ifstream f;
		f.open(path, std::ios::in | std::ios::binary);
		const std::size_t& size = std::filesystem::file_size(path);
		std::string content(size, '\0');
		f.read(content.data(), size);
		if (content.compare(givenContent) == 0)
		{
			return path;
		}
	
	}
	return path;
}

std::string BankingSystem::FindFileBasedOnRecordNumber(std::string givenRecord)
{
	std::string wantedPath = this->PathToRecords;
	wantedPath.append("\\").append(givenRecord.append(".txt"));
	std::vector<std::string> results;
	std::string path;
	for (const auto & entry : fs::directory_iterator(this->PathToRecords))
	{
		path = entry.path().string();
		std::ifstream f;
		f.open(path, std::ios::in | std::ios::binary);
		const std::size_t& size = std::filesystem::file_size(path);
		if (path.compare(wantedPath) == 0)
		{
			std::string content(size, '\0');
			f.read(content.data(), size);
			return content;
		}

	}
}

bool BankingSystem::UpdateRecord()
{
	std::string record = AskForRecord();
	if (StringIsNumber(record))
	{
		int rec = std::stoi(record);
		if (SearchRecord(rec))
		{
			cout << "\n Previous Record  \n";
			std::string foundContent{ this->FindFileBasedOnRecordNumber(record) };
			foundContent.erase(foundContent.length() - 4, foundContent.length());
			cout << foundContent << '\n\n';
			std::string fileToDelete = this->PathToRecords;
			fileToDelete.append("\\").append(record.append(".txt"));
			if (this->DeleteAccountFile(fileToDelete.c_str()));
			else
			{
				cout << "\n There is a problem removing the file " << fileToDelete << " \n \n";
				return true;
			}
			cout << "\n\nNew record\n";
			this->OverwriteRecord();
			
		}
	}
	else
	{
		std::vector<std::string> results;
		results = SearchRecord(record);
		if (!results.size())
			cout << "\n Record doesn't exist \n\n";
		else
		{
			if (results.size() == 1)
			{
				cout << "\n Previous record \n\n";
				for (auto result : results)
					cout << '\n' << result << '\n' << '\n';
				cout << "New Record\n";
				std::string PathToFile{ this->FindFileBasedOnContent(results[0]) };
				if (this->DeleteAccountFile(PathToFile.c_str()));
				else
				{
					cout << "\n There is a problem removing the file " << PathToFile << " \n \n";
					return true;
				}	
				this->OverwriteRecord();
			}
			else
			{
				cout << '\n' << "Multiple results found! Please refine your search" << '\n';
				for (auto result : results)
					cout << '\n' << result << '\n' << '\n';
			}
		}
	}
	return true;
}

bool BankingSystem::DeleteRecord()
{
	std::string record = AskForRecord();
	if (StringIsNumber(record))
	{
		int rec = std::stoi(record);
		if (SearchRecord(rec))
		{
			cout << "\n Record to be deleted  \n";
			std::string foundContent{ this->FindFileBasedOnRecordNumber(record) };
			foundContent.erase(foundContent.length() - 4, foundContent.length());
			cout << foundContent << '\n\n';
			std::string fileToDelete = this->PathToRecords;
			fileToDelete.append("\\").append(record.append(".txt"));
			if (this->DeleteAccountFile(fileToDelete.c_str()) and this->DeleteAccountNumber(record));
			else
			{
				cout << "\n There is a problem removing the file " << fileToDelete << " \n \n";
				return true;
			}
		}
	}
	else
	{
		std::vector<std::string> results;
		results = SearchRecord(record);
		if (!results.size())
			cout << "\n Record doesn't exist \n\n";
		else
		{
			if (results.size() ==  1)
			{
				cout << "\n Record to be deleted\n\n";
				for (auto result : results)
					cout << '\n' << result << '\n' << '\n';
				std::string accountNr;
				cout << " Enter account number: ";
				cin >> accountNr;
				std::string PathToFile{ this->FindFileBasedOnContent(results[0]) };
				if (this->DeleteAccountFile(PathToFile.c_str()) and this->DeleteAccountNumber(accountNr))
				{
					cout << "Record deleted succesfully\n";
				}
				else
				{
					cout << "\n There is a problem removing the file " << PathToFile << " \n \n";
					return true;
				}
				
			}
			else
			{
				cout << '\n' << "Multiple results found! Please refine your search" << '\n';
				for (auto result : results)
					cout << '\n' << result << '\n' << '\n';
			}
		}
	}
	return true;
}

 std::string BankingSystem::AskForRecord()
{
	std::string record;
	std::cout << "Record information: ";
	std::cin >> record;
	return record;
}

bool BankingSystem::FileRecordExists(int recordNumber)
{
	std::fstream AccountsFile(this->accountsFile,std::ios::in);

	try
	{
		if (AccountsFile.is_open())
		{
			std::string accountNumber;
			while (std::getline(AccountsFile, accountNumber))
			{
				if (this->StringIsNumber(accountNumber))
				{
					int readNumber = std::stoi(accountNumber);
					if (readNumber == recordNumber)
					{
						AccountsFile.close();
						return true;
					}
				}

			}
			AccountsFile.close();
			return false;
		}
		else
		{
			throw 0;
		}
	}
	catch (int n)
	{
		std::cout << "Accounts file cannot be opened\n";
	}
}

bool BankingSystem::DeleteAccountNumber(std::string accountNumber)
{
	std::fstream accounts;
	accounts.open(this->accountsFile, std::ios::in | std::ios::binary);
	const std::size_t& size = std::filesystem::file_size(this->accountsFile);
	std::string content(size, '\0');
	accounts.read(content.data(), size);
	accounts.close();
	accounts.open(this->accountsFile, std::ios::out | std::ios::trunc);
	accounts.close();
	accounts.open(this->accountsFile, std::ios::out);
	size_t position = content.find(accountNumber);
	if (position != std::string::npos)
	{
		content.erase(position, accountNumber.length() + 2);
		accounts << content.data();
		accounts.close();
		return true;
	}
		
	else
	{
		cout << "eroare\n";
		return false;
	}
		
	
	
	
}



bool BankingSystem::DeleteAccountFile(std::string filePath)
{
	if (remove(filePath.c_str()) != 0)
		return false;
	else
		return true;
}



bool BankingSystem::DisplayMenu()
{
	cout << "1 --> Add record" << std::endl << "2 --> Show record\n" << "3 --> Update record\n" << "4 --> Delete record\n" << "5 --> Quit\n";
		
	return true;
		
		
	/*
	std::ifstream localMenuFile;
	try
	{
		localMenuFile.open(menuFile);
		if (localMenuFile.good())
		{
			std::string option;
			while (std::getline(localMenuFile, option))
				std::cout << option << std::endl;
			localMenuFile.close();
			return true;
		}
		else
		{
			throw 0;
		}
	}
	catch (int n)
	{
		std::cout << "File menu cannot be opened\n";
		return false;
	}
	*/
}

bool BankingSystem::StringIsNumber(std::string AccountNumber)
{
	if (!AccountNumber.empty() && std::all_of(AccountNumber.begin(), AccountNumber.end(), ::isdigit))
		return true;
	return false;
}



