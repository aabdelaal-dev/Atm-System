#include <iostream>
#include<vector>
#include<iomanip>
#include<string>
#include<fstream>

using namespace std;

string EnterString(string Message) {

	string str;
	cout << Message;
	getline(cin >> ws, str);
	return str;

}

char EnterChar(string Message) {

	char ch;
	cout << Message;
	cin >> ch;
	return ch;

}

const string ClientsDataFile = "DatabaseFiles/Clients.txt";

struct stClient
{
	string AccNumebr;
	string PinCode;
	string Name;
	string PhoneNumber;
	int Balance;


};

enum enMainMenueChoice
{
	QuickWithdraw = 1, NormalWithdraw = 2, Deposit = 3, CheckBalance = 4, Logout = 5
};

stClient LoginClient;

//String Functions.
vector <string> splitString(string Line, string Delim ) {

	vector <string> sString;

	short pos = Line.find(Delim);

	string word;
	while (pos != string::npos) {

		word = Line.substr(0, pos);
		if (word != "") {
			sString.push_back(word);
		}
		Line.erase(0, pos + Delim.length());
		pos = Line.find(Delim);

	}
	if (Line != "") 
		sString.push_back(Line);

	return sString;

}


//-----------------------------


stClient FillClientRecord(vector<string> ClientData,stClient& Client) {

	Client.AccNumebr = ClientData[0];
	Client.PinCode = ClientData[1];
	Client.Name = ClientData[2];
	Client.PhoneNumber = ClientData[3];
	Client.Balance = stoi(ClientData[4]);

	return Client;

}

stClient ConvertClientLineToRecord(string Line, string Separator = "#//#") {


	stClient Client;

	Client = FillClientRecord(splitString(Line, Separator), Client);

	return Client;

}

vector<stClient> GetClientsDataFromFile(string FileName, vector<stClient>& vClients) {

	fstream File;
 	string Line;
	//vector <stClient> vClients;

	File.open(FileName,ios::in);//Read only
	if (File.is_open()) {

		while (getline(File,Line))
		{
			vClients.push_back(ConvertClientLineToRecord(Line, "#//#"));
			
		}
		File.close();
		return vClients;

	}
}

string ConvertClientRecordToLine(stClient ClientRecord,string Delim) {

	string Line = "";
	
	Line += ClientRecord.AccNumebr + Delim;
	Line += ClientRecord.PinCode + Delim;
	Line += ClientRecord.Name + Delim;
	Line += ClientRecord.PhoneNumber + Delim;
	Line += to_string(ClientRecord.Balance);

	return Line;


}

void SaveDataLineToFile(string Line, string FileName) {

	fstream file;
	file.open(FileName, ios::out | ios::app);
	if (file.is_open())
	{
		//Write line in file.
		file << Line << "\n";
		file.close();
	}

}

vector<stClient> SaveClientsDataToFile(vector<stClient>& vClients, string FileName) {

	fstream file;
	string line;
	file.open(ClientsDataFile,ios:: out);//Write only and delete old file

	if (file.is_open()) {

		for (stClient& C : vClients) {

			line = ConvertClientRecordToLine(C, "#//#");
			SaveDataLineToFile(line, ClientsDataFile);
			
		}
		file.close();
		return vClients;
	}
}

bool FindClientByAccNumberAndPinCode(string AccNumber,string PinCode, vector<stClient> vClients, stClient& Client) {

	//stClient Client;

	for (stClient c : vClients) {
		if (c.AccNumebr == AccNumber && c.PinCode == PinCode) {
			Client = c;
			return true;
		}
	}
	return false;
}

bool LoadClientInfo(string AccNumber,string PinCode, vector<stClient> vClients) {

	stClient Client;
	
	if (FindClientByAccNumberAndPinCode(AccNumber, PinCode, vClients ,Client)) {
		LoginClient = Client;
		return true;
	}
	return false;


}


//------------------Show Screen Menues-------------------
void Login();
void ShowMainMenueScreen();
void QuickWithdrawScreen();

void BackToMainMenue() {
	cout << "\n\nPress Any Key to back To Mamin Menue\n";
	system("pause>0");
	system("cls");
	ShowMainMenueScreen();
}

void UpdateLoginClientBalance(short NewBalance) {
	vector<stClient> vClients;
	vClients = GetClientsDataFromFile(ClientsDataFile, vClients);

	for (stClient& C : vClients)
	{
		if (C.AccNumebr == LoginClient.AccNumebr) {
			C.Balance = NewBalance;
		}
	}
	SaveClientsDataToFile(vClients, ClientsDataFile);

}

void CheckBalanceScreen() {

	cout << "===========================================\n";
	cout << "\t   Quick Withdraw\n";
	cout << "===========================================\n";

	cout << "Your Balance Is : " << LoginClient.Balance << "\n";

}

void MakeClientDeposit(int DipositAmount, stClient& Client) {

	Client.Balance += DipositAmount;

}

void DepositScreen() {

	cout << "===========================================\n";
	cout << "\t   Deposit Screen\n";
	cout << "===========================================\n";
	int DepositAmount;
	char ch = 'n';
	do
	{
		cout << "Please Enter positive deposit amount? ";
		cin >> DepositAmount;


	} while (DepositAmount <= 0 );
	
	cout << "\nAre you sure you want to perform this transaction? (y/n)? ";
	cin >> ch;

	if (ch == 'y') {

		MakeClientDeposit(DepositAmount, LoginClient);
		UpdateLoginClientBalance(LoginClient.Balance);
		cout << "\n\nDone Successfully. new balance is: " << LoginClient.Balance << endl;
	}


}

bool MakeClientWithdraw(int WithdrawAmount, stClient& Client) {

	if (WithdrawAmount > Client.Balance)
		return false;
	else
		Client.Balance -= WithdrawAmount;

	return true;

}

void PerformWithdrawMenueScreen(unsigned short Choice) {

	bool IsSuccessful = false;

	switch (Choice)
	{
	case 1:
		IsSuccessful = MakeClientWithdraw(20,LoginClient);
		break;
	case 2:
		IsSuccessful = MakeClientWithdraw(50, LoginClient);
		break;
	case 3:
		IsSuccessful = MakeClientWithdraw(100, LoginClient);
		break;
	case 4:
		IsSuccessful = MakeClientWithdraw(200, LoginClient);
		break;
	case 5:
		IsSuccessful = MakeClientWithdraw(400, LoginClient);
		break;
	case 6:
		IsSuccessful = MakeClientWithdraw(600, LoginClient);
		break;
	case 7:
		IsSuccessful = MakeClientWithdraw(800, LoginClient);
		break;
	case 8:
		IsSuccessful = MakeClientWithdraw(1000, LoginClient);
		break;
	case 9:
		BackToMainMenue();
		break;
	default:
		cout << "Invalid input!\n";
		break;
	}

	if (IsSuccessful) {

		UpdateLoginClientBalance(LoginClient.Balance);
		cout << "\n\nDone Successfully. new balance is: " << LoginClient.Balance << endl;
	}
	else {
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		QuickWithdrawScreen();
	}

}

void QuickWithdrawScreen() {

	cout << "===========================================\n";
	cout << "\t   Quick Withdraw\n";
	cout << "===========================================\n";
	cout << "\t[1] 20  \t[2] 50\n";
	cout << "\t[3] 100 \t[4] 200\n";
	cout << "\t[5] 400 \t[6] 600\n";
	cout << "\t[7] 800 \t[8] 1000\n";
	cout << "\t[9] Exit \n";
	cout << "===========================================\n";

	unsigned short WithdrawChoice;
	char Choice = 'n';
	cout << "Your Balance Is : " << LoginClient.Balance << "\n";

	do
	{
		cout << "\nChose What to Withdraw (1 - 8)? ";
		cin >> WithdrawChoice;
		if (WithdrawChoice < 1 || WithdrawChoice >=9) {

			if (WithdrawChoice == 9)
				PerformWithdrawMenueScreen(WithdrawChoice);
			else
				cout << "Invalid input!\n";
		}
		else {
			cout << "\n\nAre you sure you want to perform this transaction (y/n)? ";
			cin >> Choice;


			if (tolower(Choice) == 'y') {

				PerformWithdrawMenueScreen(WithdrawChoice);

			}
		}


	} while (WithdrawChoice < 1 || WithdrawChoice >9);

}

void NormalWithdrawScreen() {

	cout << "===========================================\n";
	cout << "\t   Quick Withdraw\n";
	cout << "===========================================\n";

	int WithdrawAmount;

	do
	{
		cout << "\nEnter an amount multible of 5's ? ";
		cin >> WithdrawAmount;

	} while ((WithdrawAmount % 5) != 0);

	if (!MakeClientWithdraw(WithdrawAmount, LoginClient))
		cout << "\nThe amount exceeds your balance, make another choice.\n";
	else {
		UpdateLoginClientBalance(LoginClient.Balance);
		cout << "\n\nDone Successfully. new balance is: " << LoginClient.Balance << endl;
	}

	

}

void PerformMainMenueChoice(enMainMenueChoice Choice) {

	//vector<stClient> vClients;

	//GetClientsDataFromFile(ClientsDataFile, vClients);

	switch (Choice)
	{
	case QuickWithdraw:
		system("cls");
		QuickWithdrawScreen();
		BackToMainMenue();
		break;
	case NormalWithdraw:
		system("cls");
		NormalWithdrawScreen();
		BackToMainMenue();
		break;
	case Deposit:
		system("cls");
		DepositScreen();
		BackToMainMenue();
		break;
	case CheckBalance:
		system("cls");
		CheckBalanceScreen();
		BackToMainMenue();
		break;
	case Logout:
		system("cls");
		Login();
		break;
	default:
		break;
	}

}

void ShowMainMenueScreen() {
	system("cls");
	cout << "===========================================\n";
	cout << "\t   ATM Main Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "===========================================\n";

	short Choice;
	cout << "\nChoose What Do You Want To Do (1-5)? ";
	cin >> Choice;

	PerformMainMenueChoice((enMainMenueChoice)Choice);
}

void Login() {

	string AccNumber, PinCode;
	vector<stClient> vClients;
	bool LoginFaild = false;

	cout << "-----------------------------------------\n";
	cout << "\tLogin Screen\n";
	cout << "-----------------------------------------\n";

	vClients = GetClientsDataFromFile(ClientsDataFile, vClients);

	do
	{
		if (LoginFaild) {
			cout << "Invalid Account Number/PinCode\n";

			system("cls");

			cout << "-----------------------------------------\n";
			cout << "\tLogin Screen\n";
			cout << "-----------------------------------------\n";
		}

		AccNumber = EnterString("Enter Account Number : ");
		PinCode = EnterString("Enter PinCode : ");

		
		LoginFaild = !LoadClientInfo(AccNumber,PinCode, vClients);



	} while (LoginFaild);

	ShowMainMenueScreen();
}

int main()
{

	Login();
	
	system("pause>0");
	return 0;
}
