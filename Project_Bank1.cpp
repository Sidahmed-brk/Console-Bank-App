#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// ============================================================
// GLOBAL VARIABLES & STRUCTURES / ENUMS
// ============================================================

string FileName = R"(C:\Users\sidah\OneDrive\Desktop\Clients_File.txt)";
string FileName_Users = R"(C:\Users\sidah\OneDrive\Desktop\Users.txt)";

struct MyStruct_Client
{
	string Account_Number;
	string PinCode;
	string FullName;
	string Phone_Number;
	double Account_Balance = 0;
	bool MarkForDelete = false;
};

struct MyStruct_User
{
	string UserName;
	string Password;
	int Permission = 0;
	bool MarkUserForDelete = false;
};

MyStruct_User CurrentUser;

enum MyEnum_Choice
{
	Show_Client_List = 1,
	Add_New_Client = 2,
	Delete_Client = 3,
	Update_Client_Info = 4,
	Find_Client = 5,
	Transactions = 6,
	Manage_Users = 7,
	Logout = 8
};

enum MyEnum_Transactions_Choice
{
	Deposit = 1,
	Withdraw = 2,
	Total_Balence = 3,
	Main_Menu = 4
};

enum MyEnum_Manage_User_Choices
{
	Show_List_Users = 1,
	Add_New_User = 2,
	Delete_User = 3,
	Update_User = 4,
	Find_User = 5,
	MainMenu = 6
};

// ============================================================
// UTILITY & STRING HELPERS
// ============================================================

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

	// Prevent "vector subscript out of range" crash if line is malformed
	if (Vclient.size() >= 5) {
		Client.Account_Number = Vclient[0];
		Client.PinCode = Vclient[1];
		Client.FullName = Vclient[2];
		Client.Phone_Number = Vclient[3];
		Client.Account_Balance = stod(Vclient[4]);
	}

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

string ConvertUserRecordToLine(MyStruct_User User, string Separateur = "#//#") {
	string Line = "";
	Line += User.UserName + Separateur;
	Line += User.Password + Separateur;
	Line += to_string(User.Permission);

	return Line;
}

MyStruct_User ConvertLineToUserRecord(string Line) {
	MyStruct_User User;
	vector<string> VUser = SplitFunction(Line, "#//#");

	// Prevent "vector subscript out of range" crash if line is malformed
	if (VUser.size() >= 3) {
		User.UserName = VUser[0];
		User.Password = VUser[1];
		User.Permission = stoi(VUser[2]);
	}

	return User;
}

// ============================================================
// FILE I/O OPERATIONS
// ============================================================

vector<MyStruct_Client> LoadClientDataFromFile(string FileName_Clients) {
	fstream MyFile;
	vector<MyStruct_Client> Vclient;
	MyFile.open(FileName_Clients, ios::in); // Read Mode
	if (MyFile.is_open()) {
		string Line;
		while (getline(MyFile, Line)) {
			if (Line != "") {
				MyStruct_Client Client = ConvertLineToRecord(Line);
				if (!Client.Account_Number.empty()) {
					Vclient.push_back(Client);
				}
			}
		}
		MyFile.close();
	}
	return Vclient;
}

vector<MyStruct_User> LoadUserDataFromFile(string FileName) {
	fstream MyFile;
	vector<MyStruct_User> VUser;
	MyFile.open(FileName, ios::in); // Read Mode
	if (MyFile.is_open()) {
		string Line;
		while (getline(MyFile, Line)) {
			if (Line != "") {
				MyStruct_User User = ConvertLineToUserRecord(Line);
				if (!User.UserName.empty()) {
					VUser.push_back(User);
				}
			}
		}
		MyFile.close();
	}
	return VUser;
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

vector<MyStruct_Client> SaveClientDataToFile(string FileName, vector<MyStruct_Client>& Vclient) {
	fstream MyFile;
	MyFile.open(FileName, ios::out); // Overwrite
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

vector<MyStruct_User> SaveUserDataToFile(string FileName_Users, vector<MyStruct_User>& VUser) {
	fstream MyFile;
	MyFile.open(FileName_Users, ios::out); // Overwrite
	string DataLine;
	if (MyFile.is_open()) {
		for (MyStruct_User& U : VUser) {
			if (U.MarkUserForDelete == false) {
				DataLine = ConvertUserRecordToLine(U, "#//#");
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return VUser;
}

// ============================================================
// SEARCH & EXISTENCE CHECKS
// ============================================================

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

bool IsUserExist(string Username) {
	vector<MyStruct_User> VUser = LoadUserDataFromFile(FileName_Users);
	for (MyStruct_User& U : VUser) {
		if (U.UserName == Username) {
			return true;
		}
	}
	return false;
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

bool MarkUserForDeleteByUsername(string Username, vector<MyStruct_User>& VUser) {
	for (MyStruct_User& U : VUser) {
		if (U.UserName == Username) {
			U.MarkUserForDelete = true;
			return true;
		}
	}
	return false;
}

bool FindUserByInformations(string Username, string Password, vector<MyStruct_User> VUser, MyStruct_User& User) {
	for (MyStruct_User& S : VUser) {
		if (S.UserName == Username && S.Password == Password) {
			User = S;
			return true;
		}
	}
	return false;
}

bool FindUserByUsername(string Username, vector<MyStruct_User> VUser, MyStruct_User& User) {
	for (MyStruct_User& S : VUser) {
		if (S.UserName == Username) {
			User = S;
			return true;
		}
	}
	return false;
}

// ============================================================
// DISPLAY & PRINT CARDS
// ============================================================

void PrintOneClientRecord(MyStruct_Client& Client) {
	cout << "| " << setw(15) << left << Client.Account_Number;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.FullName;
	cout << "| " << setw(12) << left << Client.Phone_Number;
	cout << "| " << setw(12) << left << Client.Account_Balance;
	cout << endl;
}

void PrintAllClientsData(vector<MyStruct_Client>& Vclient) {
	cout << "\n\t\t\tClient List(" << Vclient.size() << ") Client(s) :" << endl;
	cout << "__________________________________________________________________________________________________\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance\n";
	cout << "_______________________________________________________________________________________________\n";
	for (MyStruct_Client& C : Vclient) {
		PrintOneClientRecord(C);
	}
	cout << "___________________________________________________________________________________________________\n";
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

void PrintUserCard(MyStruct_User& User) {
	cout << "\nThe Following are The user details:\n";
	cout << "-------------------------------------\n";
	cout << "Username   : " << User.UserName << endl;
	cout << "Password   : " << User.Password << endl;
	cout << "Permission : " << User.Permission << endl;
	cout << "-------------------------------------\n";
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

void PrintOneUserRecord(MyStruct_User& User) {
	cout << "| " << setw(15) << left << User.UserName;
	cout << "| " << setw(10) << left << User.Password;
	cout << "| " << setw(15) << left << User.Permission;
	cout << endl;
}

void PrintAllUsersData(vector<MyStruct_User>& VUser) {
	cout << "\n\t\t\tUsers List(" << VUser.size() << ") User(s) :" << endl;
	cout << "__________________________________________________________________________________________________\n";
	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(10) << "Password";
	cout << "| " << left << setw(15) << "Permission\n";

	cout << "_______________________________________________________________________________________________\n";
	for (MyStruct_User& U : VUser) {
		PrintOneUserRecord(U);
	}
	cout << "___________________________________________________________________________________________________\n";
}

bool FindUserPrincipal(string Username, vector<MyStruct_User> VUser) {
	MyStruct_User User;
	if (FindUserByUsername(Username, VUser, User)) {
		PrintUserCard(User);
		return true;
	}
	else {
		cout << "\n\nUser With Username (" << Username << ") is Not Found! " << endl;
		return false;
	}
}

// ============================================================
// READERS & PERMISSIONS
// ============================================================

string ReadAccountNumber() {
	string AccountNumber = "";
	cout << "\nPlease Enter The Account Number of This Client :  " << endl;
	cin >> AccountNumber;
	return AccountNumber;
}

MyStruct_User ReadUserInfo() {
	MyStruct_User User;
	cout << "\nEnter Username: ";
	getline(cin >> ws, User.UserName);
	cout << "\nEnter Password: ";
	getline(cin, User.Password);

	return User;
}

string ReadUsername() {
	string Username = "";
	cout << "\nPlease Enter The Username:  " << endl;
	cin >> Username;
	return Username;
}

int ReadPermissionToSet() {
	MyStruct_User User;
	char PermissionAnswer;
	cout << "\nDo You Want to give this User all access? (Y/N):" << endl;
	cin >> PermissionAnswer;
	if (PermissionAnswer == 'y' || PermissionAnswer == 'Y')
	{
		User.Permission = -1;
	}
	else
	{
		User.Permission = 0;
		char Answer;
		cout << "\nShow Client List? (Y/N): ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			User.Permission |= 1;
		}

		cout << "\nAdd New Client? (Y/N): ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			User.Permission |= 2;
		}

		cout << "\nDelete Client? (Y/N): ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			User.Permission |= 4;
		}

		cout << "\nUpdate Client? (Y/N): ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			User.Permission |= 8;
		}

		cout << "\nFind Client? (Y/N): ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			User.Permission |= 16;
		}

		cout << "\nTransactions? (Y/N): ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			User.Permission |= 32;
		}

		cout << "\nManage Users? (Y/N): ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			User.Permission |= 64;
		}
	}
	return User.Permission;
}

// ============================================================
// UI HEADERS & NAVIGATION HELPERS
// ============================================================

void LoginScreen() {
	cout << "=========================================\n";
	cout << "\tLogin Screen\n";
	cout << "=========================================\n";
}

void AddNewClientScreen() {
	cout << "=========================================\n";
	cout << "\tAdd New Client Screen\n";
	cout << "=========================================\n";
}

void AddNewUserScreen() {
	cout << "=========================================\n";
	cout << "\tAdd New User Screen\n";
	cout << "=========================================\n";
}

void DeleteClientScreen() {
	cout << "=========================================\n";
	cout << "\tDelete Client Screen\n";
	cout << "=========================================\n";
}

void UpdateClientInfoScreen() {
	cout << "=========================================\n";
	cout << "\tUpdate Client Info Screen\n";
	cout << "=========================================\n";
}

void FindClientScreen() {
	cout << "=========================================\n";
	cout << "\tFind Client Screen\n";
	cout << "=========================================\n";
}

void Go_Back_To_Main_Menu() {
	cout << "\nPress any key to go back to Main Menu Screen...";
	system("pause>0");
}

void Go_Back_To_Transactions_Menu() {
	cout << "\nPress any key to go back to Main Menu Transactions Screen...";
	system("pause>0");
}

void Go_Back_To_Users_Menu() {
	cout << "\nPress any key to go back to Main Menu Users Screen...";
	system("pause>0");
}

void DepositScreen() {
	cout << "=========================================\n";
	cout << "\tDeposit Screen\n";
	cout << "=========================================\n";
}

void WithdrawScreen() {
	cout << "=========================================\n";
	cout << "\tWithdraw Screen\n";
	cout << "=========================================\n";
}

void AccessDenied() {
	system("cls");
	cout << "\nYou don't have permission to access this section!\nPlease contact your admin!" << endl;
	system("pause>0");
	Go_Back_To_Main_Menu();
}

void DeleteUserScreen() {
	cout << "=========================================\n";
	cout << "\tDelete User Screen\n";
	cout << "=========================================\n";
}

void UpdateUserScreen() {
	cout << "=========================================\n";
	cout << "\tUpdate User Screen\n";
	cout << "=========================================\n";
}

void FindUserScreen() {
	cout << "=========================================\n";
	cout << "\tFind User Screen\n";
	cout << "=========================================\n";
}

// ============================================================
// CLIENT ACTIONS
// ============================================================

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
	Go_Back_To_Main_Menu();
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

	} while (toupper(AddMore) == 'Y');
	Go_Back_To_Main_Menu();
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<MyStruct_Client>& Vclient) {
	MyStruct_Client Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, Vclient, Client)) {
		PrintClientCard(Client);
		cout << "\nAre You Sure you want to Delete this Client (Y/N) ?: " << endl;
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			MarkClientForDeleteByAccountNumber(AccountNumber, Vclient);
			SaveClientDataToFile(FileName, Vclient);
			Vclient = LoadClientDataFromFile(FileName);
			cout << "\n\nClinet deleted successfully! " << endl;
			return true;
		}
	}
	else {
		cout << "\n\nClinet with Account Number(" << AccountNumber << ") is not found! " << endl;
		return false;
	}
	return false;
}

void DeleteClient() {
	system("cls");
	DeleteClientScreen();
	vector<MyStruct_Client> Vclient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, Vclient);
	Go_Back_To_Main_Menu();
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

bool UpdateClientByAccountNumber(string AccountNumber, vector<MyStruct_Client>& Vclient) {
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
	return false;
}

void UpdateClient() {
	system("cls");
	UpdateClientInfoScreen();
	vector<MyStruct_Client> Vclient = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, Vclient);
	Go_Back_To_Main_Menu();
}

bool Find_Client_ByAccountNumber(string AccountNumber, vector<MyStruct_Client> Vclient) {
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
	Go_Back_To_Main_Menu();
}

// ============================================================
// USER ACTIONS
// ============================================================

bool DeleteUserByUsername(string Username, vector<MyStruct_User>& VUser) {
	MyStruct_User User;
	char Answer = 'n';

	if (FindUserByUsername(Username, VUser, User)) {
		PrintUserCard(User);

		
		if (Username == "Admin" || User.Permission == -1) {
			cout << "\n\nYou cannot delete This User! (Admin Account Protected)" << endl;
			return false; 
		}

		cout << "\nAre You Sure you want to Delete this user (Y/N) ?: " << endl;
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y') {
			MarkUserForDeleteByUsername(Username, VUser);
			SaveUserDataToFile(FileName_Users, VUser);
			VUser = LoadUserDataFromFile(FileName_Users);
			cout << "\n\nUser deleted successfully! " << endl;
			return true;
		}
	}
	else {
		cout << "\n\nUser with username(" << Username << ") is not found! " << endl;
		return false;
	}
	return false;
}
void DeleteUser() {
	system("cls");
	DeleteUserScreen();
	vector<MyStruct_User> VUser = LoadUserDataFromFile(FileName_Users);
	string Username = ReadUsername();
	DeleteUserByUsername(Username, VUser);
}

MyStruct_User ChangeUserRecord(string Username) {
	MyStruct_User User;
	User.UserName = Username;
	cout << "\n\nEnter Password: " << endl;
	getline(cin >> ws, User.Password);

	User.Permission = ReadPermissionToSet();

	return User;
}

bool UpdateUserByUsername(string Username, vector<MyStruct_User>& VUser) {
	MyStruct_User User;
	char Answer = 'n';
	if (FindUserByUsername(Username, VUser, User)) {
		PrintUserCard(User);
		cout << "\n\nDo You Want to Update This User (Y/N)?: " << endl;
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			for (MyStruct_User& U : VUser) {
				if (U.UserName == Username) {
					U = ChangeUserRecord(Username);
					break;
				}
			}
			SaveUserDataToFile(FileName_Users, VUser);
			cout << "\n\nUser Updated Successfully! " << endl;
			return true;
		}
	}
	else
	{
		cout << "\n\nUser with username (" << Username << ") is Not Found! " << endl;
		return false;
	}
	return false;
}

void UpdateUser() {
	system("cls");
	UpdateUserScreen();
	vector<MyStruct_User> VUser = LoadUserDataFromFile(FileName_Users);
	string Username = ReadUsername();
	UpdateUserByUsername(Username, VUser);
}

void FindUser() {
	system("cls");
	FindUserScreen();
	vector<MyStruct_User> VUser = LoadUserDataFromFile(FileName_Users);
	string Username = ReadUsername();
	FindUserPrincipal(Username, VUser);
}

void ShowUsersList() {
	system("cls");
	vector<MyStruct_User> VUser = LoadUserDataFromFile(FileName_Users);
	if (!VUser.empty()) {
		PrintAllUsersData(VUser);
	}
	else
	{
		cout << "Error,No Users Data Loaded!.\n";
	}
	Go_Back_To_Users_Menu();
}

void AddUser()
{
	MyStruct_User User;

	cout << "\nPlease Enter Your Username: ";
	getline(cin >> ws, User.UserName);

	while (IsUserExist(User.UserName))
	{
		cout << "\nUser with [" << User.UserName << "] already exists!\n";
		cout << "Please enter another Username: ";
		getline(cin >> ws, User.UserName);
	}

	cout << "Please Enter Your Password: ";
	getline(cin, User.Password);

	User.Permission = ReadPermissionToSet();

	AddDataLineToFile(FileName_Users, ConvertUserRecordToLine(User));
}

void AddNewUser() {

	char AddMore = 'Y';
	do
	{
		system("cls");
		AddNewUserScreen();
		cout << "\n\nAdding New User: " << endl;
		AddUser();
		cout << "\n\nUser added Successfully!\nDo You Want To Add more Users? (Y/N):  " << endl;
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

// ============================================================
// TRANSACTIONS
// ============================================================

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

void Principle_Balance() {
	system("cls");
	vector<MyStruct_Client> Vclient = LoadClientDataFromFile(FileName);
	Show_Balance(Vclient);
	Go_Back_To_Transactions_Menu();
}

// ============================================================
// MENU NAVIGATION & SCREENS
// ============================================================

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
	cout << "[7] Manage Users.\n";
	cout << "[8] Logout.\n";

	cout << "=========================================\n";

	cout << "Please Enter Your Choice [1 to 8]: ";
	cin >> Choice;

	while (Choice < 1 || Choice > 8)
	{
		cout << "Invalid Choice, Please Enter Number Between 1 and 8: ";
		cin >> Choice;
	}

	return (MyEnum_Choice)Choice;
}

MyEnum_Transactions_Choice GetTransactionChoices() {
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

MyEnum_Manage_User_Choices GetManageUserChoices() {
	system("cls");
	short Choice;
	cout << "=========================================\n";
	cout << "\tManage Users Menu Screen\n";
	cout << "=========================================\n";

	cout << "[1] List Users.\n";
	cout << "[2] Add New User.\n";
	cout << "[3] Delete User.\n";
	cout << "[4] Update User.\n";
	cout << "[5] Find User.\n";
	cout << "[6] Main Menu.\n";
	cout << "=========================================\n";

	cout << "Please Enter Your Choice [1 to 6]: ";
	cin >> Choice;

	while (Choice < 1 || Choice > 6)
	{
		cout << "Invalid Choice, Please Enter Number Between 1 and 6: ";
		cin >> Choice;
	}

	return (MyEnum_Manage_User_Choices)Choice;
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
			break;
		}

	} while (Choice != MyEnum_Transactions_Choice::Main_Menu);
}

void ManageUserMenuScreen() {
	MyEnum_Manage_User_Choices Choice;

	do
	{
		system("cls");
		Choice = GetManageUserChoices();
		switch (Choice)
		{
		case MyEnum_Manage_User_Choices::Show_List_Users:
			ShowUsersList();
			break;
		case MyEnum_Manage_User_Choices::Add_New_User:
			AddNewUser();
			break;
		case MyEnum_Manage_User_Choices::Delete_User:
			DeleteUser();
			Go_Back_To_Main_Menu();
			break;
		case MyEnum_Manage_User_Choices::Update_User:
			UpdateUser();
			Go_Back_To_Main_Menu();
			break;
		case MyEnum_Manage_User_Choices::Find_User:
			FindUser();
			Go_Back_To_Main_Menu();
			break;
		case MyEnum_Manage_User_Choices::MainMenu:
			break;
		}

	} while (Choice != MyEnum_Manage_User_Choices::MainMenu);
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
			if (CurrentUser.Permission & 1) {
				ShowClientList();
				
			}
			else {
				AccessDenied();
			}
			break;

		case MyEnum_Choice::Add_New_Client:
			if (CurrentUser.Permission & 2)
			{
				AddNewClient();
			}
			else {
				AccessDenied();
			}
			break;

		case MyEnum_Choice::Delete_Client:
			if (CurrentUser.Permission & 4)
			{
				DeleteClient();
			}
			else
			{
				AccessDenied();
			}
			break;

		case MyEnum_Choice::Update_Client_Info:
			if (CurrentUser.Permission & 8)
			{
				UpdateClient();
			}
			else
			{
				AccessDenied();
			}
			break;

		case MyEnum_Choice::Find_Client:
			if (CurrentUser.Permission & 16)
			{
				FindClient();
				
			}
			else
			{
				AccessDenied();
			}
			break;

		case MyEnum_Choice::Transactions:
			if (CurrentUser.Permission & 32)
			{
				MainMenuTransactionsScreen();
			}
			else
			{
				AccessDenied();
			}
			break;

		case MyEnum_Choice::Manage_Users:
			if (CurrentUser.Permission & 64)
			{
				ManageUserMenuScreen();
			}
			else
			{
				AccessDenied();
			}
			break;

		case MyEnum_Choice::Logout:
			break;
		}

	} while (Choice != MyEnum_Choice::Logout);
}

// ============================================================
// AUTHENTICATION & MAIN
// ============================================================

void Login() {
	MyStruct_User User;
	bool Found;
	system("cls");
	LoginScreen();
	do
	{
		User = ReadUserInfo();
		Found = FindUserByInformations(User.UserName, User.Password,
			LoadUserDataFromFile(FileName_Users), User);

		if (Found) {
			CurrentUser = User;
			MainMenuScreen();
			Found = false;
			system("cls");
			LoginScreen();
		}
		else
		{
			cout << "Invalid Username/Password!" << endl;
		}

	} while (!Found);
}

int main()
{
	Login();
	return 0;
}