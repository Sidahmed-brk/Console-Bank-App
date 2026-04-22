#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>


using namespace std;
void MainMenuScreen();

// Full path to file:
string FileName = R"(C:\Users\sidah\OneDrive\Desktop\Clients_File.txt)";


struct MyStruct_Client
{
	string Account_Number;
	string PinCode;
	string FullName;
	string Phone_Number;
	double Account_Balance;
	bool MarkForDelete = false;
};

enum MyEnum_Choice
{
	Show_Client_List= 1,
	Add_New_Client=2,
	Delete_Client=3,
	Update_Client_Info=4,
	Find_Client=5,
	Transactions=6,
	Exit=7

};
enum MyEnum_Transactions_Choice
{
	Deposit=1,
	Withdraw=2,
	Total_Balence=3,
	Main_Menu=4

};

vector<string> SplitFunction(string Sentence, string Separateur = "#//#") {
	vector <string> Vsentence;
	short Pos = 0;
	string Sword;

	while ((Pos = Sentence.find(Separateur)) != std::string::npos) {
		Sword = Sentence.substr(0, Pos);
		if (Sword != "") {
			Vsentence.push_back(Sword);
		}
		Sentence.erase(0, Pos + Separateur.length());
	}
	
	if (Sentence != "") {
		Vsentence.push_back(Sentence);
	}

	return Vsentence;

}

MyStruct_Client ConvertLineToRecord(string Line) {
	MyStruct_Client Client;
	vector<string> Vclient = SplitFunction(Line, "#//#");
	Client.Account_Number = Vclient[0];
	Client.PinCode = Vclient[1];
	Client.FullName = Vclient[2];
	Client.Phone_Number = Vclient[3];
	Client.Account_Balance = stod(Vclient[4]);

	return Client;
}

string ConvertRecordToLine(MyStruct_Client Client, string Separateur = "#//#") {
	string Line = "";
	Line += Client.Account_Number + Separateur;
	Line += Client.PinCode + Separateur;
	Line += Client.FullName + Separateur;
	Line += Client.Phone_Number + Separateur;
	Line += to_string(Client.Account_Balance);

	return Line;
}

vector<MyStruct_Client> LoadClientDataFromFile(string FileName) {
	fstream MyFile;
	vector<MyStruct_Client> Vclient;
	MyFile.open(FileName, ios::in);//Read Mode
	if (MyFile.is_open()) {
		string Line;
		while (getline(MyFile, Line)) {
			if (Line != "") {
				MyStruct_Client Client = ConvertLineToRecord(Line);
				Vclient.push_back(Client);
			}
		}
		MyFile.close();
	}
		return Vclient;

}

//Print One Client:
void PrintOneClientRecord(MyStruct_Client &Client) {
	cout << "| " << setw(15) << left << Client.Account_Number;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.FullName;
	cout << "| " << setw(12) << left << Client.Phone_Number;
	cout << "| " << setw(12) << left << Client.Account_Balance;
	cout << endl;

}

//Print All Client:

void PrintAllClientsData(vector<MyStruct_Client> &Vclient) {
	cout << "\n\t\t\tClient List(" << Vclient.size() << ") Client(s) :" << endl;
	cout << "__________________________________________________________________________________________________\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance\n";
    cout<<"_______________________________________________________________________________________________\n"; 
	for (MyStruct_Client& C : Vclient) {
		PrintOneClientRecord(C);
	}
	cout << "___________________________________________________________________________________________________\n";
}

void ShowClientList() {
	system("cls");
	vector<MyStruct_Client> Vclient = LoadClientDataFromFile(FileName);
	if (!Vclient.empty()) {
		PrintAllClientsData(Vclient);
	}
	else
	{
		cout << "Error,No Client Data Loaded!.\n";
	}
}

void AddDataLineToFile(string FileName, string DataLine) {
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << DataLine << endl;
		MyFile.close();
	}
}

bool IsClientExist(string AccountNumber)
	{
		vector<MyStruct_Client> Vclient = LoadClientDataFromFile(FileName);

		for (MyStruct_Client& C : Vclient)
		{
			if (C.Account_Number == AccountNumber)
			{
				return true;
			}
		}

		return false;
	}

void AddClient()
	{
		MyStruct_Client Client;

		cout << "\nPlease Enter Your Account Number: ";
		getline(cin >> ws, Client.Account_Number);

		while (IsClientExist(Client.Account_Number))
		{
			cout << "\nClient with [" << Client.Account_Number << "] already exists!\n";
			cout << "Please enter another Account Number: ";
			getline(cin >> ws, Client.Account_Number);
		}

		cout << "Please Enter Your PinCode: ";
		getline(cin, Client.PinCode);

		cout << "Please Enter Your FullName: ";
		getline(cin, Client.FullName);

		cout << "Please Enter Your Phone Number: ";
		getline(cin, Client.Phone_Number);

		cout << "Please Enter Your Account Balance: ";
		cin >> Client.Account_Balance;

		AddDataLineToFile(FileName, ConvertRecordToLine(Client));
	}

void AddNewClientScreen() {
		cout << "=========================================\n";
		cout << "\tAdd New Client Screen\n";
		cout << "=========================================\n";
	}

void AddNewClient() {
	
	char AddMore = 'Y';
	do
	{
		system("cls");
		AddNewClientScreen();
		cout << "\n\nAdding New Client: " << endl;
		AddClient();
		cout << "\n\nClient added Successfully!\nDo You Want To Add more clients? (Y/N):  " << endl;
		cin >> AddMore;


	} while (toupper(AddMore)=='Y');

}

void PrintClientCard(MyStruct_Client& Client)
{
	cout << "\nThe Following are The Client details:\n";
	cout << "-------------------------------------\n";
	cout << "Account Number : " << Client.Account_Number << endl;
	cout << "Pin Code       : " << Client.PinCode << endl;
	cout << "Full Name      : " << Client.FullName << endl;
	cout << "Phone Number   : " << Client.Phone_Number << endl;
	cout << "Balance        : " << Client.Account_Balance << endl;
	cout << "-------------------------------------\n";
}

string ReadAccountNumber() {
	string AccountNumber = "";
	cout << "\nPlease Enter The Account Number of This Client :  " << endl;
	cin >> AccountNumber;
	return AccountNumber;
}

bool FindClientByAccountNumber(string AccountNumber, vector<MyStruct_Client> Vclient, MyStruct_Client& Client) {
	for (MyStruct_Client& C : Vclient) {
		if (C.Account_Number == AccountNumber) {
			Client = C;
			return true;
		}
	}
	return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<MyStruct_Client>& Vclient) {
	for (MyStruct_Client& C : Vclient) {
		if (C.Account_Number == AccountNumber) {
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector<MyStruct_Client> SaveClientDataToFile(string FileName, vector<MyStruct_Client> &Vclient) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite 
	string DataLine;
	if (MyFile.is_open()) {
		for (MyStruct_Client& C : Vclient) {
			if (C.MarkForDelete == false) {
				DataLine = ConvertRecordToLine(C, "#//#");
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return Vclient;
}

bool DeleteCientByAccountNumber(string AccountNumber, vector<MyStruct_Client>& Vclient) {
	MyStruct_Client Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, Vclient, Client)) {
		PrintClientCard(Client);
		cout << "\nAre You Sure you want to Delete this Client (Y/N) ?: " << endl;
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			MarkClientForDeleteByAccountNumber(AccountNumber, Vclient);
			SaveClientDataToFile(FileName, Vclient);
			//Refrech clients: 
			Vclient = LoadClientDataFromFile(FileName);
			cout << "\n\nClinet deleted successfully! " << endl;
			return true;
		}
	}
	else {
		cout << "\n\nClinet with Account Number(" << AccountNumber << ") is not found! " << endl;
		return false;
	}
}

void DeleteClientScreen() {
	cout << "=========================================\n";
	cout << "\tDelete Client Screen\n";
	cout << "=========================================\n";

}

void DeleteClient() {
	system("cls");
	DeleteClientScreen();
	vector<MyStruct_Client> Vclient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	DeleteCientByAccountNumber(AccountNumber, Vclient);

}

MyStruct_Client ChangeClientRecord(string AccountNumber) {
	MyStruct_Client Client;
	Client.Account_Number = AccountNumber;
	cout << "\n\nEnter PinCode: " << endl;
	getline(cin >> ws, Client.PinCode);

	cout << "\nPlease Enter Your FullName: ";
	getline(cin, Client.FullName);

	cout << "\nPlease Enter Your Phone Number: ";
	getline(cin, Client.Phone_Number);

	cout << "\nPlease Enter Your Account Balance: ";
	cin >> Client.Account_Balance;

	return Client;

}

bool UpdateClientByAccountNumber(string AccountNumber, vector<MyStruct_Client> &Vclient) {
	MyStruct_Client Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, Vclient, Client)) {
		PrintClientCard(Client);
		cout << "\n\nDo You Want to Update This Client (Y/N)?: " << endl;
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			for (MyStruct_Client& C : Vclient) {
				if (C.Account_Number == AccountNumber) {
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientDataToFile(FileName, Vclient);
			cout << "\n\nClient Updated Successfully! " << endl;
			return true;
		}
	}

	else
	{
		cout << "\n\nClient with Account Number (" << AccountNumber << ") is Not Found! " << endl;
		return false;
	}

}

void UpdateClientInfoScreen() {
	cout << "=========================================\n";
	cout << "\tUpdate Client Info Screen\n";
	cout << "=========================================\n";
}

void UpdateClient() {
	system("cls");
	UpdateClientInfoScreen();
	vector<MyStruct_Client> Vclient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, Vclient);

}

void FindClientScreen() {
	cout << "=========================================\n";
	cout << "\tFind Client Screen\n";
	cout << "=========================================\n";

}

bool Find_Client_ByAccountNumber(string AccountNumber,vector<MyStruct_Client> Vclient) {
	MyStruct_Client Clinet;
	if (FindClientByAccountNumber(AccountNumber, Vclient, Clinet)) {
		PrintClientCard(Clinet);
		return true;
	}
	else {
		cout << "\n\nClient with Account Number (" << AccountNumber << ") is Not Found! " << endl;
		return false;
	}
}

void FindClient() {
	system("cls");
	FindClientScreen();
	vector<MyStruct_Client> Vclient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
Find_Client_ByAccountNumber(AccountNumber, Vclient);

}


void Go_Back_To_Transactions_Menu() {
	cout << "\nPress any key to go back to Main Menu Transactions Screen...";
	system("pause>0");
}

void DepositScreen() {
	cout << "=========================================\n";
	cout << "\tDeposit Screen\n";
	cout << "=========================================\n";
}


bool Deposit_Transaction() {
	vector<MyStruct_Client> Vclient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	MyStruct_Client Client;

	while (!FindClientByAccountNumber(AccountNumber, Vclient, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] is not found!\n";
		AccountNumber = ReadAccountNumber();
	}
	PrintClientCard(Client);
	double Amount;
	cout << "\nPlease Enter Deposit amount: " << endl;
	cin >> Amount;
	char Answer;
	cout << "\n\nAre You Sure you want Perform this Transaction (Y/N): " << endl;
	cin >> Answer;
	if (Answer != 'Y' && Answer != 'y')
	{
		cout << "\nTransaction Cancelled.\n";
		return false;
	}

	for (MyStruct_Client& C : Vclient) {
		if (C.Account_Number == AccountNumber) {
			C.Account_Balance += Amount;
			Client = C;
			break;
		}
	}
	SaveClientDataToFile(FileName, Vclient);
	cout << "\nDeposit Transaction Done Successfully!\nAccount Balence is: " << Client.Account_Balance << endl;
	return true;

}

void Principle_Deposit() {
	system("cls");
	DepositScreen();
	Deposit_Transaction();
	Go_Back_To_Transactions_Menu();
}


void WithdrawScreen() {
	cout << "=========================================\n";
	cout << "\tWithdraw Screen\n";
	cout << "=========================================\n";
}


bool Withdraw_Transaction() {
	vector<MyStruct_Client> Vclient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	MyStruct_Client Client;

	while (!FindClientByAccountNumber(AccountNumber, Vclient, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] is not found!\n";
		AccountNumber = ReadAccountNumber();
	}
	PrintClientCard(Client);
	double Amount;
	cout << "\nPlease Enter Withdraw amount: " << endl;
	cin >> Amount;
	if (Amount > Client.Account_Balance) {
		cout << "Amount Exceeds The Balence,You Can Withdraw Up To: " << Client.Account_Balance << endl;
		cout << "\nPlease Enter another Amount: " << endl;
		cin >> Amount;
	}
	for (MyStruct_Client& C : Vclient) {
		if (C.Account_Number == AccountNumber) {
			C.Account_Balance -= Amount;
			Client = C;
			break;
		}
	}
	SaveClientDataToFile(FileName, Vclient);
	cout << "\nWithdraw Transaction Done Successfully!\nAccount Balence is: " << Client.Account_Balance << endl;
	return true;
}

void Principle_Withdraw() {
	system("cls");
	WithdrawScreen();
	Withdraw_Transaction();
	Go_Back_To_Transactions_Menu();
}


void PrintOneBalance(MyStruct_Client& Client) {
	cout << "| " << setw(15) << left << Client.Account_Number;
	cout << "| " << setw(40) << left << Client.FullName;
	cout << "| " << setw(12) << left << Client.Account_Balance;
	cout << endl;
}

void Show_Balance(vector<MyStruct_Client>& Vclient) {
	cout << "\n\t\t\tBalance List(" << Vclient.size() << ") Client(s) :" << endl;
	cout << "__________________________________________________________________________________________________\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance\n";
	cout << "_______________________________________________________________________________________________\n";
	double TotalBalances = 0;
	for (MyStruct_Client& C : Vclient) {
		PrintOneBalance(C);
		TotalBalances += C.Account_Balance;
	}
	cout << "___________________________________________________________________________________________________\n";

	cout << "\n\t\t\t\t\t\t\t\tTotal Balances = " << TotalBalances << endl;
}

void Principle_Balance() {
	system("cls");
	vector<MyStruct_Client> Vclient = LoadClientDataFromFile(FileName);
	Show_Balance(Vclient);
	Go_Back_To_Transactions_Menu();
}
 MyEnum_Transactions_Choice GetTransactionChoices () {
	system("cls");
	short Choice;
	cout << "=========================================\n";
	cout << "\tMain Menu Transactions Screen\n";
	cout << "=========================================\n";

	cout << "[1] Deposit.\n";
	cout << "[2] Withdraw.\n";
	cout << "[3] Total Balance.\n";
	cout << "[4] Main Menu.\n";
	cout << "=========================================\n";

	cout << "Please Enter Your Choice [1 to 4]: ";
	cin >> Choice;

	while (Choice < 1 || Choice > 4)
	{
		cout << "Invalid Choice, Please Enter Number Between 1 and 4: ";
		cin >> Choice;
	}

	return (MyEnum_Transactions_Choice)Choice;

}



 void MainMenuTransactionsScreen() {
	 MyEnum_Transactions_Choice Choice;

	 do
	 {
		 system("cls");
		 Choice = GetTransactionChoices();
		 switch (Choice)
		 {
		 case MyEnum_Transactions_Choice::Deposit:
			 Principle_Deposit();
			 break;
		 case MyEnum_Transactions_Choice::Withdraw:
			 Principle_Withdraw();
			 break;
		 case MyEnum_Transactions_Choice::Total_Balence:
			 Principle_Balance();
			 break;
		 case MyEnum_Transactions_Choice::Main_Menu:
			 MainMenuScreen();
			 break;
		 }

	 } while (Choice != MyEnum_Transactions_Choice::Main_Menu);
 }



MyEnum_Choice GetChoice()
{
	short Choice;

	cout << "=========================================\n";
	cout << "\tMain Menu Screen\n";
	cout << "=========================================\n";

	cout << "[1] Show Client List.\n";
	cout << "[2] Add New Client.\n";
	cout << "[3] Delete Client.\n";
	cout << "[4] Update Client Info.\n";
	cout << "[5] Find Client.\n";
	cout << "[6] Transactions.\n";
	cout << "[7] Exit.\n";

	cout << "=========================================\n";

	cout << "Please Enter Your Choice [1 to 7]: ";
	cin >> Choice;

	while (Choice < 1 || Choice > 7)
	{
		cout << "Invalid Choice, Please Enter Number Between 1 and 7: ";
		cin >> Choice;
	}

	return (MyEnum_Choice)Choice;
}

void MainMenuScreen()
{
	MyEnum_Choice Choice;

	do
	{
		system("cls");

		Choice = GetChoice();

		switch (Choice)
		{
		case MyEnum_Choice::Show_Client_List:
			ShowClientList();
			break;

		case MyEnum_Choice::Add_New_Client:
			AddNewClient();
			break;

		case MyEnum_Choice::Delete_Client:
			DeleteClient();
			break;

		case MyEnum_Choice::Update_Client_Info:
			UpdateClient();
			break;

		case MyEnum_Choice::Find_Client:
			FindClient();
			break;
		case MyEnum_Choice::Transactions:
			MainMenuTransactionsScreen();
			break;
		case MyEnum_Choice::Exit:
			cout << "Program Ends...\n";
			break;
		}

		if (Choice != MyEnum_Choice::Exit)
		{
			cout << "\nPress any key to go back to Main Menu...";
			system("pause>0");
		}

	} while (Choice != MyEnum_Choice::Exit);
}


int main()
{
	MainMenuScreen();


}
