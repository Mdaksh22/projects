
#include <iostream>
#include <iomanip> 
using namespace std;

class Customer 
{
public:
    string name;
    int accountNumber;
    double balance;
    int pin;
    double loan;

    Customer(string n, int accNum, double bal, int p) 
    {
        name = n;
        accountNumber = accNum;
        balance = bal;
        pin = p;
        loan = 0.0;
    }
};

class StackNode 
{
public:
    Customer* customer;
    StackNode* next;

    StackNode(Customer* cust) 
    {
        customer = cust;
        next = NULL;
    }
};

class QueueNode 
{
public:
    Customer* customer;
    QueueNode* next;

    QueueNode(Customer* cust) 
    {
        customer = cust;
        next = NULL;
    }
};

class Bank 
{
private:
    StackNode* transactionHistory;
    QueueNode* front;
    QueueNode* rear;

public:
    Bank() 
    {
        transactionHistory = NULL;
        front = rear = NULL;
    }

    void addCustomer(string name, int accNum, double balance, int pin) 
    {
        Customer* customer = new Customer(name, accNum, balance, pin);
        QueueNode* temp = new QueueNode(customer);

        if (rear == NULL) 
        {
            front = rear = temp;
        } 
        else 
        {
            rear->next = temp;
            rear = temp;
        }

        cout << "Customer added to the queue: " << customer->name << endl;
    }

    void serveCustomer() 
    {
        if (front == NULL) 
        {
            cout << "No customers in the queue." << endl;
            return;
        }

        QueueNode* temp = front;
        front = front->next;

        if (front == NULL) 
        {
            rear = NULL;
        }

        cout << "Serving customer: " << temp->customer->name << endl;
        delete temp;
    }

    void deposit(int accNum, int pin, double amount) 
    {
        for (QueueNode* temp = front; temp != NULL; temp = temp->next) 
        {
            if (temp->customer->accountNumber == accNum) 
            {
                if (temp->customer->pin == pin) 
                {
                    temp->customer->balance += amount;
                    pushTransaction(temp->customer);
                    generateReceipt(temp->customer, "Deposit", amount);
                    return;
                } 
                else 
                {
                    cout << "Incorrect PIN!" << endl;
                    return;
                }
            }
        }
        cout << "Invalid account number." << endl;
    }

    void withdraw(int accNum, int pin, double amount) 
    {
        for (QueueNode* temp = front; temp != NULL; temp = temp->next) 
        {
            if (temp->customer->accountNumber == accNum) 
            {
                if (temp->customer->pin == pin) 
                {
                    if (temp->customer->balance < amount) 
                    {
                        cout << "Insufficient balance." << endl;
                    } 
                    else 
                    {
                        temp->customer->balance -= amount;
                        pushTransaction(temp->customer);
                        generateReceipt(temp->customer, "Withdraw", amount);
                    }
                    return;
                } 
                else 
                {
                    cout << "Incorrect PIN!" << endl;
                    return;
                }
            }
        }
        cout << "Invalid account number." << endl;
    }

    void createFixedDeposit(int accNum, int pin, double fdAmount, double interestRate, int years) 
    {
        for (QueueNode* temp = front; temp != NULL; temp = temp->next) 
        {
            if (temp->customer->accountNumber == accNum) 
            {
                if (temp->customer->pin == pin) 
                {
                                        double maturityAmount = fdAmount * (1 + (interestRate / 100) * years);
                    cout << "Fixed deposit created for " << temp->customer->name 
                         << " with amount " << fdAmount << " and interest " << interestRate 
                         << "% for " << years << " years." << endl;
                    cout << "Maturity amount after " << years << " years: " << maturityAmount << endl;

                    temp->customer->balance -= fdAmount; // Update balance for tracking
                    pushTransaction(temp->customer);
                    generateReceipt(temp->customer, "Fixed Deposit", fdAmount);
                    return;
                } 
                else 
                {
                    cout << "Incorrect PIN!" << endl;
                    return;
                }
            }
        }
        cout << "Invalid account number." << endl;
    }

    
    void applyForLoan(int accNum, int pin, double loanAmount, double interestRate, int years) 
    {
        for (QueueNode* temp = front; temp != NULL; temp = temp->next) 
        {
            if (temp->customer->accountNumber == accNum) 
            {
                if (temp->customer->pin == pin) 
                {
                     double repaymentAmount = loanAmount * (1 + (interestRate / 100) * years);
                    temp->customer->loan = repaymentAmount;
                    temp->customer->balance += loanAmount; 
                    cout << temp->customer->name << " took a loan of " << loanAmount << " with interest " 
                         << interestRate << "% for " << years << " years." << endl;

                    cout << "Total repayment amount: " << repaymentAmount << endl;                       pushTransaction(temp->customer);
                    generateReceipt(temp->customer, "Loan", loanAmount);
                    return;
                } 
                else 
                {
                    cout << "Incorrect PIN!" << endl;
                    return;
                }
            }
        }
        cout << "Invalid account number." << endl;
    }

    void pushTransaction(Customer* customer) 
    {
        StackNode* temp = new StackNode(customer);
        temp->next = transactionHistory;
        transactionHistory = temp;
    }

    void undoLastTransaction() 
    {
        if (transactionHistory == NULL) 
        {
            cout << "No transaction to undo." << endl;
            return;
        }

        StackNode* temp = transactionHistory;
        transactionHistory = transactionHistory->next;
        cout << "Undoing last transaction for " << temp->customer->name << endl;
        delete temp;
    }

    void displayCustomerDetails(int accNum) 
    {
        for (QueueNode* temp = front; temp != NULL; temp = temp->next) 
        {
            if (temp->customer->accountNumber == accNum) 
            {
                cout << "Name: " << temp->customer->name 
                     << ", Account Number: " << temp->customer->accountNumber 
                     << ", Balance: " << fixed << setprecision(2) << temp->customer->balance
                     << ", Loan Repayment Amount: " << temp->customer->loan << endl;
                return;
            }
        }
        cout << "Invalid account number." << endl;
    }

    void displayQueue() 
    {
        if (front == NULL) 
        {
            cout << "No customers in the queue." << endl;
            return;
        }

        QueueNode* temp = front;
        cout << "Customers in the queue:" << endl;
        while (temp != NULL) 
        {
            cout << "Name: " << temp->customer->name 
                 << ", Account Number: " << temp->customer->accountNumber 
                 << ", Balance: " << fixed << setprecision(2) << temp->customer->balance << endl;
            temp = temp->next;
        }
    }

    void generateReceipt(Customer* customer, string transactionType, double amount)
    {
        cout << "\n--- Receipt ---" << endl;
        cout << "Customer Name: " << customer->name << endl;
        cout << "Account Number: " << customer->accountNumber << endl;
        cout << "Transaction Type: " << transactionType << endl;
        cout << "Transaction Amount: " << amount << endl;
        cout << "Current Balance: " << fixed << setprecision(2) << customer->balance << endl;
        cout << "-------------------" << endl;
    }
};

int main() 
{
    Bank bank;
    string name;
    int accNum, pin;
    double balance;
    double loanAmount, interestRate, fdAmount;
    int years;
    int choice;

    while (true) 
    {
        cout << "\n--- Bank Management System Menu ---" << endl;
        cout << "1. Add Customer to Queue" << endl;
        cout << "2. Display Queue" << endl;
        cout << "3. Serve Customer" << endl;
        cout << "4. Deposit Money" << endl;
        cout << "5. Withdraw Money" << endl;
        cout << "6. Create Fixed Deposit" << endl;
        cout << "7. Apply for Loan" << endl;
        cout << "8. Undo Last Transaction" << endl;
        cout << "9. Display Customer Details" << endl;
        cout << "10. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 10) 
        {
            cout << "Exiting the system..." << endl;
            break;
        }

        switch (choice) 
        {
            case 1:
                cout << "Enter customer name: ";
                cin >> name;
                cout << "Enter account number: ";
                cin >> accNum;
                cout << "Enter initial balance: ";
                cin >> balance;
                cout << "Set PIN for account: ";
                cin >> pin;
                bank.addCustomer(name, accNum, balance, pin);
                break;

            case 2:
                bank.displayQueue();
                break;

            case 3:
                bank.serveCustomer();
                break;

            case 4:
                cout << "Enter account number: ";
                cin >> accNum;
                cout << "Enter PIN: ";
                cin >> pin;
                cout << "Enter deposit amount: ";
                cin >> balance;                
               bank.deposit(accNum, pin, balance);
                break;

            case 5:
                cout << "Enter account number: ";
                cin >> accNum;
                cout << "Enter PIN: ";
                cin >> pin;
                cout << "Enter withdrawal amount: ";
                cin >> balance; 
                bank.withdraw(accNum, pin, balance);
                break;

            case 6:
                cout << "Enter account number: ";
                cin >> accNum;
                cout << "Enter PIN: ";
                cin >> pin;
                cout << "Enter FD amount: ";
                cin >> fdAmount;
                cout << "Enter interest rate (%): ";
                cin >> interestRate;
                cout << "Enter FD duration (years): ";
                cin >> years;
                bank.createFixedDeposit(accNum, pin, fdAmount, interestRate, years);
                break;

            case 7:
                cout << "Enter account number: ";
                cin >> accNum;
                cout << "Enter PIN: ";
                cin >> pin;
                cout << "Enter loan amount: ";
                cin >> loanAmount;
                cout << "Enter interest rate (%): ";
                cin >> interestRate;
                cout << "Enter loan duration (years): ";
                cin >> years;
                bank.applyForLoan(accNum, pin, loanAmount, interestRate, years);
                break;

            case 8:
                bank.undoLastTransaction();
                break;

            case 9:
                cout << "Enter account number: ";
                cin >> accNum;
                bank.displayCustomerDetails(accNum);
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
