#include <iostream>
#include <string>
using namespace std;

// Class for Queue (Customer)
class CustomerNode {
public:
    string name;
    string pickupLocation;
    string destination;
    CustomerNode* next;

    CustomerNode(string n, string p, string d) : name(n), pickupLocation(p), destination(d), next(nullptr) {}
};

// Custom Queue for Customers
class CustomerQueue {
private:
    CustomerNode* frontNode;
    CustomerNode* rearNode;

public:
    CustomerQueue() : frontNode(nullptr), rearNode(nullptr) {}

    void push(string name, string pickupLocation, string destination) {
        CustomerNode* newNode = new CustomerNode(name, pickupLocation, destination);
        if (rearNode) {
            rearNode->next = newNode;
        } else {
            frontNode = newNode; // Queue was empty
        }
        rearNode = newNode;
        cout << "Customer " << name << " booked a ride from " << pickupLocation << " to " << destination << ".\n";
    }

    CustomerNode* pop() {
        if (isEmpty()) {
            cout << "No customers in the queue.\n";
            return nullptr;
        }
        CustomerNode* poppedNode = frontNode;
        frontNode = frontNode->next;
        if (!frontNode) {
            rearNode = nullptr; // Queue is now empty
        }
        return poppedNode;
    }

    bool isEmpty() {
        return frontNode == nullptr;
    }
};

// Class for Drivers
class Driver {
public:
    int distanceFromCustomer;
    int id;
    Driver* next;

    Driver(int distance, int id) : distanceFromCustomer(distance), id(id), next(nullptr) {}
};

// Custom Priority Queue for Drivers
class DriverPriorityQueue {
private:
    Driver* head;

public:
    DriverPriorityQueue() : head(nullptr) {}

    void push(int distanceFromCustomer, int driverId) {
        Driver* newDriver = new Driver(distanceFromCustomer, driverId);
        if (!head || head->distanceFromCustomer > distanceFromCustomer) {
            newDriver->next = head;
            head = newDriver;
        } else {
            Driver* current = head;
            while (current->next && current->next->distanceFromCustomer <= distanceFromCustomer) {
                current = current->next;
            }
            newDriver->next = current->next;
            current->next = newDriver;
        }
        cout << "Driver with ID " << driverId << " added with distance " << distanceFromCustomer << ".\n";
    }

    Driver* pop() {
        if (isEmpty()) {
            cout << "No drivers available.\n";
            return nullptr;
        }
        Driver* topDriver = head;
        head = head->next;
        return topDriver;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    void display() {
        if (isEmpty()) {
            cout << "No drivers available.\n";
            return;
        }
        cout << "Available Drivers (closest first):\n";
        Driver* current = head;
        while (current) {
            cout << "Driver ID: " << current->id << ", Distance: " << current->distanceFromCustomer << " from customer\n";
            current = current->next;
        }
    }

    void updateDistances(string customerLocation) {
        Driver* current = head;

        while (current) {
            int newDistance;
            cout << "Enter the updated distance for Driver ID " << current->id << " from " << customerLocation << ": ";
            cin >> newDistance;
            current->distanceFromCustomer = newDistance;
            current = current->next;
        }

        sortDrivers();
    }

    void sortDrivers() {
        if (!head || !head->next) return; 

        Driver* sorted = nullptr;

        while (head) {
            Driver* current = head;
            head = head->next;

            if (!sorted || sorted->distanceFromCustomer >= current->distanceFromCustomer) {
                current->next = sorted;
                sorted = current;
            } else {
                Driver* temp = sorted;
                while (temp->next && temp->next->distanceFromCustomer < current->distanceFromCustomer) {
                    temp = temp->next;
                }
                current->next = temp->next;
                temp->next = current;
            }
        }
        head = sorted;
    }
};

// Class for storing completed rides
class RideNode {
public:
    string customerName;
    string pickupLocation;
    string destination;
    RideNode* next;

    RideNode(string name, string pickup, string dest) : customerName(name), pickupLocation(pickup), destination(dest), next(nullptr) {}
};

// Custom Stack for Completed Rides
class RideStack {
private:
    RideNode* topNode;

public:
    RideStack() : topNode(nullptr) {}

    void push(string name, string pickupLocation, string destination) {
        RideNode* newNode = new RideNode(name, pickupLocation, destination);
        newNode->next = topNode;
        topNode = newNode;
        cout << "Ride for " << name << " added to completed rides.\n";
    }

    RideNode* pop() {
        if (isEmpty()) {
            cout << "No completed rides to pop.\n";
            return nullptr;
        }
        RideNode* poppedNode = topNode;
        topNode = topNode->next;
        return poppedNode;
    }

    bool isEmpty() {
        return topNode == nullptr;
    }

    void display() {
        if (isEmpty()) {
            cout << "No completed rides.\n";
            return;
        }
        cout << "Completed Rides:\n";
        RideNode* current = topNode;
        while (current) {
            cout << "Customer: " << current->customerName 
                 << ", From: " << current->pickupLocation 
                 << " to: " << current->destination << "\n";
            current = current->next;
        }
    }
};

// Ride Booking System
class RideBookingSystem {
private:
    CustomerQueue customerQueue;           
    DriverPriorityQueue driverQueue;       
    RideStack completedRides;              
    DriverPriorityQueue tempDriverQueue;   // Temporary queue to hold drivers that reject the ride

public:
    void addCustomer(string name, string pickupLocation, string destination) {
        customerQueue.push(name, pickupLocation, destination);
        updateDriverDistances(pickupLocation);
    }

    void addDriver(int distanceFromCustomer, int driverId) {
        driverQueue.push(distanceFromCustomer, driverId);
    }

    void updateDriverDistances(string customerLocation) {
        cout << "Updating driver distances from the new customer location: " << customerLocation << endl;
        driverQueue.updateDistances(customerLocation);
    }

    void displayAvailableDrivers() {
        driverQueue.display();
    }

    void processRide() {
        if (customerQueue.isEmpty()) {
            cout << "No customers in the queue.\n";
            return;
        }

        if (driverQueue.isEmpty()) {
            cout << "No drivers available.\n";
            return;
        }

        CustomerNode* currentCustomer = customerQueue.pop();
        bool rideAssigned = false;

        while (!driverQueue.isEmpty()) {
            Driver* closestDriver = driverQueue.pop();

            cout << "Notifying driver with ID " << closestDriver->id 
                 << " at distance " << closestDriver->distanceFromCustomer << "...\n";
            char response;
            cout << "Driver, do you accept the ride? (y/n): ";
            cin >> response;

            if (response == 'y') {
                cout << "Driver accepted the ride.\n";
                cout << "Customer " << currentCustomer->name << " has been assigned driver ID " << closestDriver->id
                     << " located " << closestDriver->distanceFromCustomer << " units away.\n";

                char rideComplete;
                cout << "Is the ride completed? (y/n): ";
                cin >> rideComplete;

                if (rideComplete == 'y') {
                    cout << "Ride completed. Driver is now available again.\n";
                    completedRides.push(currentCustomer->name, currentCustomer->pickupLocation, currentCustomer->destination);
                    driverQueue.push(closestDriver->distanceFromCustomer, closestDriver->id);
                } else {
                    cout << "Driver is still on the way.\n";
                }

                delete currentCustomer;
                delete closestDriver;
                rideAssigned = true;
                break;  // Ride assigned, exit the loop
            } else {
                cout << "Driver rejected the ride. Trying the next driver.\n";
                tempDriverQueue.push(closestDriver->distanceFromCustomer, closestDriver->id);
            }

            delete closestDriver; // Free the rejected driver object
        }

        // Re-add rejected drivers to the main driver queue
        while (!tempDriverQueue.isEmpty()) {
            Driver* tempDriver = tempDriverQueue.pop();
            driverQueue.push(tempDriver->distanceFromCustomer, tempDriver->id);
            delete tempDriver; // Free the temporary driver object
        }

        if (!rideAssigned) {
            cout << "No drivers accepted the ride. Customer will remain in the queue.\n";
            customerQueue.push(currentCustomer->name, currentCustomer->pickupLocation, currentCustomer->destination);
            delete currentCustomer;
        }
    }

    void displayCompletedRides() {
        completedRides.display();
    }
};

// Driver section to handle driver-specific tasks
void handleDriverSection(RideBookingSystem& system) {
    int choice;
    while (true) {
        cout << "\n--- Driver Menu ---" << endl;
        cout << "1. Add Driver\n2. Display Available Drivers\n3. Return to Main Menu\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int distanceFromCustomer, driverId;
                cout << "Enter driver ID: ";
                cin >> driverId;
                cout << "Enter driver distance from the customer: ";
                cin >> distanceFromCustomer;
                system.addDriver(distanceFromCustomer, driverId);
                break;
            }
            case 2:
                system.displayAvailableDrivers();
                break;
            case 3:
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Customer section to handle customer-specific tasks
void handleCustomerSection(RideBookingSystem& system) {
    int choice;
    while (true) {
        cout << "\n--- Customer Menu ---" << endl;
        cout << "1. Add Customer\n2. Process Ride\n3. Display Completed Rides\n4. Return to Main Menu\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, pickupLocation, destination;
                cout << "Enter customer name: ";
                cin >> name;
                cout << "Enter pickup location: ";
                cin >> pickupLocation;
                cout << "Enter destination: ";
                cin >> destination;
                system.addCustomer(name, pickupLocation, destination);
                break;
            }
            case 2:
                system.processRide();
                break;
            case 3:
                system.displayCompletedRides();
                break;
            case 4:
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Main function
int main() {
    RideBookingSystem system;
    int choice;

    while (true) {
        cout << "\n--- Ride Booking System ---" << endl;
        cout << "1. Driver Section\n2. Customer Section\n3. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                handleDriverSection(system);
                break;
            case 2:
                handleCustomerSection(system);
                break;
            case 3:
                cout << "Exiting the system.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}