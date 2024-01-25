#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <fstream>
using namespace std;
#define MAX_SIZE 100

int totalMember = 0;
struct trans
{
    string description;
    double amount;
    double balance;
};
struct myCashMember
{
    string mobileNo;
    string name;
    double amount;
    string pin;
    int totalTrans;
    struct trans transaction[MAX_SIZE];
};

class History
{
private:
    struct Transaction
    {
        int tranID;
        string description;
        double amount;
        double balance;
    };

    Transaction transactions[MAX_SIZE];
    int lastTransactionID;
    int transactionNo;

public:
    History() : lastTransactionID(101), transactionNo(0) {}

    int getTransactionNo() const { return transactionNo; }
    void incrementTransactionNo() { transactionNo++; }

    void addTransaction(const string &desc, double amt, double bal, int no)
    {
        Transaction newTransaction;
        newTransaction.tranID = lastTransactionID++;
        newTransaction.description = desc;
        newTransaction.amount = amt;
        newTransaction.balance = bal;
        transactions[no] = newTransaction;
    }

    void displayTransactionHistory(int no)
    {
        cout << "\nTransaction History: " << endl;
        int count = 0;
        for (int i = 0; i < no; i++)
        {
            cout << "ID: " << transactions[i].tranID << " | Description: " << transactions[i].description
                 << " | Amount: " << transactions[i].amount << " | Balance: " << transactions[i].balance << endl;
            count++;
        }

        if (!count)
        {
            cout << "No History!"
                 << "\n";
        }
    }
};

int generateOTP();
bool verifyOTP(int otp);

class Member
{
private:
    string mobileNo;
    string name;
    double amount;
    string pin;
    History transactionHistory;
    static int memberNo;

public:
    Member() : amount(0.0) {}
    Member(string mobile, string n, double a, string p) : mobileNo(mobile), name(n), amount(a), pin(p) {}

    int getMemberNo() const { return memberNo; }
    void incrementMemberNo() { memberNo++; }
    void decrementMemberNo() { memberNo--; }

    string getMobileNo() const { return mobileNo; }
    string getName() const { return name; }
    double getAmount() const { return amount; }
    string getPin() const { return pin; }
    History &getTransactionHistory() { return transactionHistory; }

    void setMobileNo(string mobile) { mobileNo = mobile; }
    void setName(string n) { name = n; }
    void setAmount(double a) { amount = a; }
    void setPin(string p) { pin = p; }

    void updateMember()
    {
        cout << "Old Name: " << getName() << endl;

        string newName;
        cout << "New Name (enter to ignore): ";
        cin.ignore();
        getline(cin, newName);

        string oldPin;
        string newPin, confirmNewPin;
        cout << "Old pin: ";
        cin >> oldPin;

        if (oldPin == getPin())
        {
            int flag = 0;
            cout << "New pin (enter to ignore): ";
            cin.ignore();
            getline(cin, newPin);

            if (!newPin.empty())
            {
                cout << "Confirm New pin: ";
                cin >> confirmNewPin;
                if (newPin == confirmNewPin)
                {
                    flag = 1;
                }
                else
                {
                    cout << "\nPINs must be equal. PIN not change!" << endl;
                }
            }

            int otp = generateOTP();

            if (verifyOTP(otp))
            {
                if (flag)
                {
                    setPin(newPin);
                }
                if (!newName.empty())
                {
                    flag = 1;
                    setName(newName);
                }

                if (flag)
                {
                    cout << "\nUpdate is Successful" << endl;
                }
                else
                {
                    cout << "\nNothing changed"
                         << "\n";
                }
            }
            else
            {
                cout << "\nIncorrect OTP. Update canceled."
                     << endl;
            }
        }
        else
        {
            cout << "\nInvalid PIN. Please try again."
                 << endl;
        }
    }
};

int Member::memberNo = 0;

void displayLoginMenu();
void displayMainMenu(Member &loggedInMember, Member members[], int &index);
void registerMember(Member members[]);
void removeMember(int &index, Member members[]);
Member *loginMember(Member members[], int &index);
void sendMoney(Member &loggedInMember, Member members[]);
void cashIn(Member &loggedInMember);
void cashOut(Member &loggedInMember);
void payBill(Member &loggedInMember);
void checkBalance(Member &loggedInMember);
void viewTransactionHistory(History &transactionHistory);

int main()
{
    Member members[MAX_SIZE];
    struct myCashMember mcmembers[MAX_SIZE];

    ifstream inputFile("data.txt");

    if (!inputFile.is_open())
    {
        ofstream outputFile("data.txt");
        outputFile.close();
    }
    else
    {
        while (totalMember < MAX_SIZE && inputFile >> mcmembers[totalMember].name >> mcmembers[totalMember].mobileNo >> mcmembers[totalMember].amount >> mcmembers[totalMember].pin >> mcmembers[totalMember].totalTrans)
        {
            if (mcmembers[totalMember].totalTrans)
            {
                string s = "TH" + mcmembers[totalMember].mobileNo + ".txt";
                ifstream inputFile(s);
                if (!inputFile.is_open())
                {
                    mcmembers[totalMember].totalTrans = 0;
                }
                else
                {
                    int i = 0;
                    while (i < MAX_SIZE && inputFile >> mcmembers[totalMember].transaction[i].amount >> mcmembers[totalMember].transaction[i].balance >> mcmembers[totalMember].transaction[i].description)
                    {
                        i++;
                    }
                }
                inputFile.close();
            }
            totalMember++;
        }
        inputFile.close();
    }

    bool exitProgram = false;
    while (!exitProgram)
    {
        displayLoginMenu();

        int loginOption;
        cin >> loginOption;
        cin.ignore();

        switch (loginOption)
        {
        case 1:
        {
            int index;
            Member *loggedInMember = loginMember(members, index);
            if (loggedInMember)
            {
                displayMainMenu(*loggedInMember, members, index);
            }

            break;
        }
        case 2:
            registerMember(members);
            break;
        case 3:
            exitProgram = true;
            break;
        default:
            cout << "\nInvalid Option. Please try again.\n"
                 << endl;
        }
    }

    return 0;
}

void displayLoginMenu()
{
    cout << "*** MyCash Login ***" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter Your Option: ";
}

void displayMainMenu(Member &loggedInMember, Member members[], int &index)
{
    bool logout = false;
    while (!logout)
    {
        cout << "********** MyCash Menu **********" << endl;
        cout << "1. Update Me" << endl;
        cout << "2. Remove Me" << endl;
        cout << "3. Send Money" << endl;
        cout << "4. Cash-in" << endl;
        cout << "5. Cash-out" << endl;
        cout << "6. Pay Bill" << endl;
        cout << "7. Check Balance" << endl;
        cout << "8. History" << endl;
        cout << "9. Logout" << endl;
        cout << "Enter Your Option (1-9): ";

        int option;
        cin >> option;

        switch (option)
        {
        case 1:
            loggedInMember.updateMember();
            break;
        case 2:
            removeMember(index, members);
            logout = true;
            break;
        case 3:
            sendMoney(loggedInMember, members);
            break;
        case 4:
            cashIn(loggedInMember);
            break;
        case 5:
            cashOut(loggedInMember);
            break;
        case 6:
            payBill(loggedInMember);
            break;
        case 7:
            checkBalance(loggedInMember);
            break;
        case 8:
            viewTransactionHistory(loggedInMember.getTransactionHistory());
            break;
        case 9:
            logout = true;
            cout << "\nLogged out successfully."
                 << endl;
            break;
        default:
            cout << "\nInvalid Option. Please try again."
                 << endl;
        }

        cout << "\nPress any key go to main menu....";
        cin.ignore();
        cin.get();
        cout << "\n";
    }
}

int generateOTP()
{
    srand(static_cast<unsigned>(time(0)));
    int r = rand() % 9000 + 1000;
    cout << "\nGenerated OTP: " << r << "\n";
    return r;
}

bool verifyOTP(int otp)
{
    int enteredOTP;
    cout << "Enter OTP: ";
    cin >> enteredOTP;
    return enteredOTP == otp;
}

void registerMember(Member members[])
{
    string mobileNo, name, pin, rePin;

    cout << "\nEnter Mobile No. (11-digit): ";
    cin >> mobileNo;

    if (mobileNo[0] != '0' || mobileNo[1] != '1' || mobileNo.length() != 11)
    {
        cout << "\nInvalid number."
             << "\n\n";
        return;
    }

    for (int i = 0; i < members[0].getMemberNo(); i++)
    {
        if (mobileNo == members[i].getMobileNo())
        {
            cout << "\nMember already exists"
                 << "\n";
            return;
        }
    }

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter 5 digit PIN: ";
    // cin >> pin;
    for (int i = 0; i < 5; i++)
    {
        pin += getch();
        cout << "*";
    }

    cout << "\nReconfirm PIN: ";

    for (int i = 0; i < 5; i++)
    {
        rePin += getch();
        cout << "*";
    }

    if (pin == rePin)
    {
        int otp = generateOTP();

        if (verifyOTP(otp))
        {
            members[members[0].getMemberNo()].setMobileNo(mobileNo);
            members[members[0].getMemberNo()].setPin(pin);
            members[members[0].getMemberNo()].setName(name);
            members[0].incrementMemberNo();
            cout << "\nRegistration Successful.\n"
                 << endl;
        }
        else
        {
            cout << "\nRegestration canceled. Incorrect OTP.\n"
                 << endl;
        }
    }
    else
    {
        cout << "\nPINs must be equal. Try again!\n"
             << "\n";
        return;
    }
}

Member *loginMember(Member members[], int &index)
{
    Member *loggedInMember = nullptr;

    cout << "\nEnter Mobile No. (11-digit): ";
    string mobile;
    cin >> mobile;
    int isLogin = 0;

    for (int i = 0; i < members[0].getMemberNo(); i++)
    {
        if (mobile == members[i].getMobileNo())
        {
            isLogin = 1;
            string enteredPin;
            cout << "Enter your 5 digit PIN: ";
            // cin >> enteredPin;
            for (int i = 0; i < 5; i++)
            {
                enteredPin += getch();
                cout << "*";
            }

            if (enteredPin == members[i].getPin())
            {
                index = i;
                cout << "\nLogin Successful.\n"
                     << endl;
                loggedInMember = &members[i];
                return loggedInMember;
            }
            else
            {
                cout << "\nInvalid PIN. Login Failed. Please try again.\n"
                     << endl;
                return loggedInMember;
            }
        }
    }
    if (!isLogin)
    {
        cout << "\nMember not found. Login Failed. Please try again.\n"
             << endl;
        return loggedInMember;
    }
}

void removeMember(int &index, Member members[])
{
    int otp = generateOTP();

    if (verifyOTP(otp))
    {
        for (int i = index; i < members[0].getMemberNo() - 1; i++)
        {
            members[i] = members[i + 1];
        }

        members[0].decrementMemberNo();

        cout << "Account removed successfully."
             << endl;
    }
    else
    {
        cout << "Incorrect OTP. Removal canceled."
             << endl;
    }
}

void sendMoney(Member &loggedInMember, Member members[])
{
    cout << "\nEnter recipient's Mobile No. (11-digit): ";
    string recipientMobile;
    cin >> recipientMobile;

    // Check if the recipient exists
    bool recipientFound = false;
    Member *recipientMember = nullptr;
    for (int i = 0; i < members[0].getMemberNo(); i++)
    {
        if (recipientMobile == members[i].getMobileNo() && recipientMobile != loggedInMember.getMobileNo())
        {
            recipientMember = &members[i];
            recipientFound = true;
            break;
        }
    }

    if (!recipientFound)
    {
        cout << "\nRecipient not found. Please check the Mobile No."
             << endl;
        return;
    }

    cout << "Enter amount to send: ";
    double amount;
    cin >> amount;

    if (amount <= 0)
    {
        cout << "\nInvalid amount. Please enter a positive value."
             << endl;
        return;
    }

    if (loggedInMember.getAmount() < amount)
    {
        cout << "\nInsufficient balance. Transaction canceled."
             << endl;
        return;
    }

    cout << "Sending " << amount << " to " << recipientMobile << "\n";

    cout << "Want to pay(Y/N)? ";
    char c;
    cin >> c;
    if (c == 'Y')
    {
        int otp = generateOTP();

        if (verifyOTP(otp))
        {
            loggedInMember.setAmount(loggedInMember.getAmount() - amount);
            recipientMember->setAmount(recipientMember->getAmount() + amount);
            cout << "\nTransaction Successful."
                 << endl;
            loggedInMember.getTransactionHistory().addTransaction("Sent money to " + recipientMobile, amount, loggedInMember.getAmount(), loggedInMember.getTransactionHistory().getTransactionNo());
            recipientMember->getTransactionHistory().addTransaction("Received money from " + loggedInMember.getMemberNo(), amount, loggedInMember.getAmount(), loggedInMember.getTransactionHistory().getTransactionNo());

            loggedInMember.getTransactionHistory().incrementTransactionNo();
            recipientMember->getTransactionHistory().incrementTransactionNo();
        }
        else
        {
            cout << "\nTransaction canceled. Incorrect OTP."
                 << endl;
        }
    }
    else
    {
        cout << "\nBill payment canceled."
             << "\n";
    }
}

void cashIn(Member &loggedInMember)
{
    cout << "\nEnter amount to cash-in: ";
    double amount;
    cin >> amount;

    if (amount <= 0)
    {
        cout << "\nInvalid amount. Please enter a positive value."
             << endl;
        return;
    }

    cout << "Want to pay(Y/N)? ";
    char c;
    cin >> c;
    if (c == 'Y')
    {
        int otp = generateOTP();

        if (verifyOTP(otp))
        {
            loggedInMember.setAmount(loggedInMember.getAmount() + amount);
            cout << "\nCash-in Successful. New balance: " << loggedInMember.getAmount() << endl;
            loggedInMember.getTransactionHistory().addTransaction("Cash-in", amount, loggedInMember.getAmount(), loggedInMember.getTransactionHistory().getTransactionNo());

            loggedInMember.getTransactionHistory().incrementTransactionNo();
        }
        else
        {
            cout << "\nCash-in canceled. Incorrect OTP."
                 << endl;
        }
    }
    else
    {
        cout << "\nBill payment canceled."
             << "\n";
    }
}

void cashOut(Member &loggedInMember)
{
    cout << "\nEnter amount to cash-out: ";
    double amount;
    cin >> amount;

    if (amount <= 0)
    {
        cout << "\nInvalid amount. Please enter a positive value."
             << endl;
        return;
    }

    if (loggedInMember.getAmount() < amount)
    {
        cout << "\nInsufficient balance. Cash-out canceled."
             << endl;
        return;
    }

    cout << "Want to pay(Y/N)? ";
    char c;
    cin >> c;
    if (c == 'Y')
    {
        int otp = generateOTP();

        if (verifyOTP(otp))
        {
            loggedInMember.setAmount(loggedInMember.getAmount() - amount);
            cout << "\nCash-out Successful. New balance: " << loggedInMember.getAmount() << endl;
            loggedInMember.getTransactionHistory().addTransaction("Cash-out", amount, loggedInMember.getAmount(), loggedInMember.getTransactionHistory().getTransactionNo());

            loggedInMember.getTransactionHistory().incrementTransactionNo();
        }
        else
        {
            cout << "\nCash-out canceled. Incorrect OTP."
                 << endl;
        }
    }
    else
    {
        cout << "\nBill payment canceled."
             << "\n";
    }
}

void payBill(Member &loggedInMember)
{
    cout << "\nEnter Bill Type (Gas/Electricity/Water/Internet-1/2/3/4): ";
    int n;
    double amount;
    string arr[4] = {"Gas", "Electricity", "Water", "Internet"};

    if (n >= 1 && n <= 4)
    {
        cout << "Enter " << arr[n - 1] << " bill: ";
        cin >> amount;
    }
    else
    {
        cout << "\nInvalid choice! Try again"
             << "\n";
        return;
    }

    if (amount <= 0)
    {
        cout << "\nInvalid amount. Please enter a positive value."
             << endl;
        return;
    }

    if (loggedInMember.getAmount() < amount)
    {
        cout << "\nInsufficient balance. Bill payment canceled."
             << endl;
        return;
    }

    cout << "Want to pay(Y/N)? ";
    char c;
    cin >> c;
    if (c == 'Y')
    {
        int otp = generateOTP();

        if (verifyOTP(otp))
        {
            loggedInMember.setAmount(loggedInMember.getAmount() - amount);
            cout << "\nBill payment Successful. New balance: " << loggedInMember.getAmount() << endl;
            loggedInMember.getTransactionHistory().addTransaction(arr[n - 1] + " bill", amount, loggedInMember.getAmount(), loggedInMember.getTransactionHistory().getTransactionNo());

            loggedInMember.getTransactionHistory().incrementTransactionNo();
        }
        else
        {
            cout << "\nBill payment canceled. Incorrect OTP."
                 << endl;
        }
    }
    else
    {
        cout << "\nBill payment canceled."
             << "\n";
    }
}

void checkBalance(Member &loggedInMember)
{
    cout << "\nCurrent Balance: " << loggedInMember.getAmount() << endl;
}

void viewTransactionHistory(History &transactionHistory)
{
    transactionHistory.displayTransactionHistory(transactionHistory.getTransactionNo());
}
