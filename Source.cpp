#include<iostream>
#include<cctype>
#include<windows.h>
#include<vector>
#include<iomanip>
#include<fstream>
#include<string>
#include"\Users\j7664\Source\Repos\Main Library\Main Library/Sounds.h"
#include"\Users\j7664\Source\Repos\Main Library\Main Library/ScreenDesigner.h"
#include"\Users\j7664\Source\Repos\Main Library\Main Library/Numbers.h"
#include"\Users\j7664\Source\Repos\Main Library\Main Library/Text.h"
using namespace std;
string ClientsFile = "Clients.txt";
enum enATM { eQickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogout = 5 };
enum enQickWithdraw { q20 = 1, q50 = 2, q100 = 3, q200 = 4, q400 = 5, q600 = 6, q800 = 7, q1000 = 8, Exit = 9 };
void GoBackToMainMenue();
void ATMProgram();
void ATMAplication();
struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
};
stClient CurrentClient;
void ATM_MainMenue()
{
	cout << string(50, '=') << endl;
	cout << "\t\tATM Main Menue" << endl;
	cout << string(50, '=') << endl;
	cout << "\t[1] Qick Withdraw\n";
	cout << "\t[2] Normal Withdraw\n";
	cout << "\t[3] Deposit\n";
	cout << "\t[4] Check Balance\n";
	cout << "\t[5] Logout\n";
	cout << string(50, '=') << endl;
}
void CheckBalance()
{
	cout << string(50, '=') << endl;
	cout << "\t\t  Chek Balance" << endl;
	cout << string(50, '=') << endl;
}
void NormalWithdraw()
{
	cout << "\n" << string(50, '-') << endl;
	cout << "\t     Normal Withdraw Screen" << endl;
	cout << string(50, '-') << endl;
}
void QuickWithdraw()
{
	cout << string(50, '=') << endl;
	cout << "\t\tQuick Withdraw" << endl;
	cout << string(50, '=') << endl;
	cout << "\t[1] 20\t\t[2] 50\n";
	cout << "\t[3] 100\t\t[4] 200\n";
	cout << "\t[5] 400\t\t[6] 600\n";
	cout << "\t[7] 800\t\t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << string(50, '=') << endl;
}
void LoginScreen()
{
	cout << "\n" << ScreenDesigner::Line(60, '-') << endl;
	cout << "\t\t\tLogin Screen" << endl;
	cout << ScreenDesigner::Line(60, '-') << endl;
}
string ConvertClientsRecordToLine(stClient Client, string delimiter = "#//#")
{
	string S = "";
	S += Client.AccountNumber + delimiter;
	S += Client.PinCode + delimiter;
	S += Client.Name + delimiter;
	S += Client.Phone + delimiter;
	S += to_string(Client.AccountBalance);
	return S;
}
stClient ConvertLineToClientsRecord(string Line, string delimiter = "#//#")
{
	vector<string> VClient = Text::SplitString(Line, delimiter);
	stClient Client;
	Client.AccountNumber = VClient[0];
	Client.PinCode = VClient[1];
	Client.Name = VClient[2];
	Client.Phone = VClient[3];
	Client.AccountBalance = stod(VClient[4]);
	return Client;
}
void AddRecordToFile(string FileName, string Line)
{
	fstream File;
	File.open(FileName, ios::out | ios::app);
	if (File.is_open())
	{
		File << Line << endl;
		File.close();
	}
}
vector<stClient> LoadCleintsDataFromFile(string FileName)
{
	vector<stClient> VClientsData;
	fstream File;
	File.open(FileName, ios::in);
	if (File.is_open())
	{
		string Line = "";
		while (getline(File, Line))
		{
			VClientsData.push_back(ConvertLineToClientsRecord(Line));
		}
		File.close();
	}
	return VClientsData;
}
void SaveClientRecordToFile(string FileName, vector<stClient> VClientsData)
{
	fstream File;
	string ClientData = "";
	File.open(FileName, ios::out);
	if (File.is_open())
	{
		for (stClient& C : VClientsData)
		{
			if (C.MarkForDelete == false)
			{
				ClientData = ConvertClientsRecordToLine(C);
				File << ClientData << endl;
			}
		}
		File.close();
	}
}
bool ClientExistsByAccountNumberAndPinCode(string AccountNumber, string PinCode, string FileName)
{
	stClient Client;
	vector<stClient>VClient;
	fstream File;
	File.open(FileName, ios::in);
	if (File.is_open())
	{
		string Line;
		while (getline(File, Line))
		{
			Client = ConvertLineToClientsRecord(Line);
			if (Client.AccountNumber == AccountNumber && Client.PinCode == PinCode)
			{
				File.close();
				return true;
			}
			VClient.push_back(Client);
		}
		File.close();
	}
	return false;
}
bool FindClientByAccountNumberAndPinCode(string FileName, string AccountNumber, string PinCode, stClient& Client)
{
	vector<stClient> VClientsData = LoadCleintsDataFromFile(FileName);
	for (stClient& C : VClientsData)
	{
		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
stClient ReadClientInfo()
{
	stClient Client;
	Client.AccountNumber = Text::ReadText("Enter Account Number? ");
	Client.PinCode = Text::ReadText("Enter PinCode? ");

	return Client;
}
//00000000000000000000000000000000000
void CheckBalanceScreen()
{
	CheckBalance();
	cout << "your Balance is " << CurrentClient.AccountBalance << endl;
}
void CheckBalanceProgram()
{
	system("cls");
	CheckBalanceScreen();
	GoBackToMainMenue();
}
//1111111111111111111111111111111111
bool DepositeBalancetoClient(string AccountNumber, double Ammount, vector<stClient>VClientsData)
{
	char Answer = 'n';
	cout << "Are you sure you want perform this transaction [y/n] ";
	cin >> Answer;
	Sounds::KeypressConfirmation();
	if (toupper(Answer) == 'Y')
	{
		for (stClient& C : VClientsData)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Ammount;
				SaveClientRecordToFile(ClientsFile, VClientsData);
				cout << "Done seccessfully new Balance is: " << C.AccountBalance;
				Sounds::OperationCompleteSound();
				return true;
			}
		}
	}
	return false;
}
int ReadWithdrawAmmount()
{
	int Ammount = Numbers::ReadPositiveNumber<int>("\nEnter an amount multiple of 5's\n");
	while (cin.fail()||Ammount % 5 != 0)
	{
		Ammount = Numbers::ReadPositiveNumber<int>("\nEnter an amount multiple of 5's\n");
		Sounds::OperationDeclinedSound();
    }
	return Ammount;
}
void NormalWithdrawMonyScreen()
{
	NormalWithdraw();
	int Ammount = ReadWithdrawAmmount();
	if (Ammount > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		Sounds::OperationDeclinedSound();
		return;
	}
	else
	{
		vector<stClient> VClient = LoadCleintsDataFromFile(ClientsFile);
		DepositeBalancetoClient(CurrentClient.AccountNumber, Ammount * (-1), VClient);
		CurrentClient.AccountBalance -= Ammount;
	}
};
void NormalWithdrawMony()
{
	system("cls");
	NormalWithdrawMonyScreen();
	GoBackToMainMenue();
}
//22222222222222222222222222
int QuickWithdrawAmmount(enQickWithdraw WithdrawAmmount)
{
	switch (WithdrawAmmount)
	{
	case enQickWithdraw::q20:
		return 20;
	case enQickWithdraw::q50:
		return 50;
	case enQickWithdraw::q100:
		return 100;
	case enQickWithdraw::q200:
		return 200;
	case enQickWithdraw::q400:
		return 400;
	case enQickWithdraw::q600:
		return 600;
	case enQickWithdraw::q800:
		return 800;
	case enQickWithdraw::q1000:
		return 1000;
	default:
		return 0;
	}
}
void QuickWithdrawMonyScreen(enQickWithdraw WithdrawAmmount)
{
	int Ammount = QuickWithdrawAmmount(WithdrawAmmount);
	if (Ammount > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		Sounds::OperationDeclinedSound();
		return;
	}
	else
	{
		vector<stClient> VClient = LoadCleintsDataFromFile(ClientsFile);
		DepositeBalancetoClient(CurrentClient.AccountNumber, Ammount * (-1), VClient);
		CurrentClient.AccountBalance -= Ammount;
	}
};
enQickWithdraw ReadQuickWithdraw_MenueOption(string message)
{
	int num;
	do
	{
		cout << message << endl;
		cin >> num;
		if (cin.fail() || (num > 9 || num < 1))
		{
			Sounds::WarningSound();
			cin.clear();
			cin.ignore(1000000000, '\n');
		}
		else
			Sounds::KeypressConfirmation();
	} while (num <= 0);
	return (enQickWithdraw)num;
}
void PerfromManageATMMenueOption(enQickWithdraw ManageQickWithdrawOption)
{
	if (ManageQickWithdrawOption == enQickWithdraw::Exit)
		return;
	else
		QuickWithdrawMonyScreen(ManageQickWithdrawOption);

}
void QuickWithdrawProgram()
{
	system("cls");
	QuickWithdraw();
	PerfromManageATMMenueOption(ReadQuickWithdraw_MenueOption("Choose what do you want to do? [1 to 9]"));
	GoBackToMainMenue();
}
//3333333333333333333333333333
void DepositeMonyScreen()
{
	vector<stClient> VClient = LoadCleintsDataFromFile(ClientsFile);
	double Ammount=0;
	NormalWithdraw();
	Ammount = Numbers::ReadPositiveNumber<double>("\nEnter a positive Deposite Ammount\n");
	DepositeBalancetoClient(CurrentClient.AccountNumber, Ammount , VClient);
	CurrentClient.AccountBalance += Ammount;
};
void DepositeMonyProgram()
{
	system("cls");
	DepositeMonyScreen();
	GoBackToMainMenue();
}
//4444444444444444444444444444444444
enATM ReadATM_MenueOption(string message)
{
	int num;
	do
	{
		cout << message << endl;
		cin >> num;
		if (cin.fail() || (num > 5 || num < 1))
		{
			Sounds::WarningSound();
			cin.clear();
			cin.ignore(1000000000, '\n');
		}
		else
			Sounds::KeypressConfirmation();
	} while (num <= 0);
	return (enATM)num;
}
void PerfromManageATMMenueOption(enATM ManageATMMenueOption)
{
	switch (ManageATMMenueOption)
	{
	case enATM::eQickWithdraw:
		QuickWithdrawProgram();
		break;
	case enATM::eNormalWithdraw:
		NormalWithdrawMony();
		break;
	case enATM::eDeposit:
		DepositeMonyProgram();
		break;
	case enATM::eCheckBalance:
		CheckBalanceProgram();
		break;
	case enATM::eLogout:
		system("cls");
		ATMAplication();
		break;
	}
}
void GoBackToMainMenue()
{
	cout << "\nPress any key to go back to Main Menu...";
	system("pause>0");
	Sounds::KeypressConfirmation();
	ATMProgram();
}
void ATMProgram()
{
	system("cls");
	ATM_MainMenue();
	PerfromManageATMMenueOption(ReadATM_MenueOption("Choose what do you want to do? [1 to 5]"));
}
bool  LoadClientInfo(string AccountNumber, string Password)
{
	if (FindClientByAccountNumberAndPinCode(ClientsFile, AccountNumber, Password, CurrentClient))
		return true;
	else
		return false;
}
void ATMAplication()
{
	bool LoginFaild = false;
	stClient Client;
	do
	{
		system("cls");
		LoginScreen();
		if (LoginFaild)
		{
			cout << "Invalid AccountNumber/PinCode!\n";
		}
		Client = ReadClientInfo();

		LoginFaild = !LoadClientInfo(Client.AccountNumber, Client.PinCode);
	} while (LoginFaild);
	ATMProgram();
}

int main()
{
	ATMAplication();
	return 0;
}