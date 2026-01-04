#include <iostream>
#include <iomanip>
#include <conio.h>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <regex>
#include <thread>
// Color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define LIGHTRED "\033[91m"
#define GREEN   "\033[92m"
#define YELLOW  "\033[33m"
#define LIGHTYELLOW "\033[93m"
#define BLUE    "\033[94m"
#define PURPLE "\033[95m"
#define DARKPURPLE "\033[35m"
#define CYAN    "\033[96m"
using namespace std;

enum bookingType { TREATMENT, CONSULTATION, AVAILABLE, NOT_AVAILABLE };
enum paymentMethod { NONE, ONLINE_BANKING, E_WALLET, CREDIT_CARD };
enum workPosition { EXPERT, ADMIN };

struct Staff {
    string username = "";
    string password = "";
    workPosition role; // "Expert" or "Admin"
};


struct Customer {
    string username = "";
    string password = "";
    string name = "";
    string contact = "";
    string email = "";
};

struct ServiceType {
    int serviceID = -1;
    string name = "";
    double consultPrice = 0.0;
    double treatPrice = 0.0;
};

struct Booking {
    string slotID = "";
    ServiceType service;
    int day = 0;
    int date = 0;
    int timeSlot = 0;
    string cusUsername;
    bookingType status = AVAILABLE;
    paymentMethod paymentUse = NONE;
    double totalPrice = 0;
};

struct Expert {
    string name = "";
    int age = 0;
    string gender = "";
    string experience = "";
    string education = "";
    Booking slots[4][30];
    int hoursworked[4][5] = { { 0,0,0,0,0 },{ 0,0,0,0,0 }, { 0,0,0,0,0 }, { 0,0,0,0,0 } };
};

struct SaleRecord {
    string slotID;
    string customerUsername;
    string date;
    string bookingType;
    string service;
    string expert;
    double price;
    double tax;
    double totalAmount;
};

string colorText(string text, string color);
void displayLogo();
void displayStartPage();
void displayEnding();
void displayMainMenu();
void moreInfo();
void displayCustomerMenu();
int getValidIntInput(int min, int max);
void customerMenu(Expert experts[], const ServiceType services[], const string days[], const string timeslots[]);
void customerAuthenticationPage(Customer& currentCustomer, bool* loginCusStatus);
void cusLogin(Customer& currentCustomer, bool* loginCusStatus);
void regCus();
string getHiddenPassword();
char getValidCustomerInput();

void servicePage(const ServiceType services[], int serviceChoice);
void serviceList(const ServiceType services[]);
void displayService1Info(const ServiceType& service);
void displayService2Info(const ServiceType& service);
void displayService3Info(const ServiceType& service);

void expertList(Expert experts[]);
void displayExpertInfo(const Expert& expert);

string generateSlotID(char prefix, int slotNumber);
void initializeExpertSlots(Expert& expert);
void retrieveBookings(Expert& expert, ServiceType const services[]);
void updateBookingStatus(Expert& expert);
int getIndexExpert();
int countAvailableSlots(Expert& expert, int week);
bool isWeekFullyBooked(const Expert& expert, int week);
int displayWeeksAndSelect(Expert& expert);
void markRemainingSlotStatus(Expert& expert, int week, int day, bool makeAvailable);
void displaySchedule(Expert& expert, int week, const string days[], const string timeSlots[], bool isExpertView = false);
paymentMethod processPayment(double amount, double& taxAmount, double& totalAmount);
paymentMethod online_bank();
paymentMethod ewallet();
paymentMethod creditcard();

void generateReceipt(const Expert& expert, const Customer& customer, int week, int slotIndex, const string days[], const string& timeRange, double price, double taxAmount, double totalAmount);
string calculateTimeRange(int slotIndex, bookingType typeChoice);
bool makeBooking(Expert& expert, const ServiceType services[], const Customer& currentCustomer, int week, const string days[], const string timeSlots[]);
void displayBookingDetails(string slotID, string expertName, string service, bookingType typeStatus, int week, string day, int date, string timeRange);

void viewCustomerAppointments(const Customer& customer, const Expert experts[], const string days[], const string timeSlots[]);
int linearSearch(const Booking slots[], int n, const string& val); // implement the linear search

void staffPage(Expert experts[], const ServiceType services[], const string days[], const string timeslots[]);
int staffLogin(const Staff staffs[]);
void expertMenu(Expert expert[], const ServiceType services[], const string days[], const string timeSlots[], int staffIndex);

void viewCustomerDetails(const Expert experts[]);
void selectionSort(Customer arr[], double sales[], int n, bool sortByName);
void displaySortedList(Customer arr[], double sales[], int n, bool sortedByName);
void displayCustomerDetails(Customer arr[], double sales[], int n, const string& username);
double calculateTotalSales(const Customer& customer, const Expert experts[]);

void viewIndividualSchedule(Expert& expert, const string days[], const string timeSlots[]);
void viewSalesReport(const Expert experts[], const ServiceType services[]);
void adminMenu(Expert experts[], const ServiceType services[], const string days[], const string timeSlots[]);

bool handleAdminSelection(Expert experts[], const string days[], const string timeSlots[]);
void viewOverallSchedules(Expert experts[], const string days[], const string timeSlots[]);
bool processRefund(Expert experts[], const string days[], const string timeSlots[]);


int main()
{
    // Move large arrays to the heap
    ServiceType* services = new ServiceType[3]{ {0,"Makeup", 200, 500}, {1,"Body Waxing", 100, 300}, {2,"Lazer Hair Removal", 300, 600} };
    Expert* experts = new Expert[3]{ {"Steve", 35, "Male", "10+ years", "Universiti Teknologi MARA (UiTM)"}, {"Vernice", 28, "Female", "7 years", "Universiti Kebangsaan Malaysia (UKM)"}, {"Paul", 40, "Male", "15 years", "Universiti Malaya (UM)"} };
    string days[5] = { "Mon", "Tue", "Wed", "Thu", "Fri" };
    string timeSlots[6] = { "8-10", "10-12", "12-14", "14-16", "16-18", "18-20" };
    // Initialize slots for each expert
    for (int i = 0; i < 3; i++) {
        initializeExpertSlots(experts[i]);
        retrieveBookings(experts[i], services);
        updateBookingStatus(experts[i]);
    }

    displayStartPage();

    int mainInput;
    // switch case
    do {
        system("cls");
        displayLogo();
        displayMainMenu();
        mainInput = getValidIntInput(1, 4);
        switch (mainInput)
        {
        case 1: customerMenu(experts, services, days, timeSlots); break;
        case 2: staffPage(experts, services, days, timeSlots);  break;
        case 3: moreInfo(); break;
        case 4: displayEnding(); break;
        }
    } while (mainInput != 4);

    // Clean up heap-allocated memory
    delete[] services;
    delete[] experts;

    return 0;
}

// Helper function to apply color to text
string colorText(string text, string color) {
    return color + text + RESET;
}

void displayLogo() {
    cout << "\033[95m _    _   ___  ___   _ ______   __  ____  _____    _   _   _ _______   __  \n";
    cout << "| |  | | | \\ \\/ / | | |  _ \\ \\ / / | __ )| ____|  / \\ | | | |_   _\\ \\ / / \n";
    cout << "| |  | | | |\\  /| | | | |_) \\ V /  |  _ \\|  _|   / _ \\| | | | | |  \\ V /  \n";
    cout << "| |__| |_| |/  \\| |_| |  _ < | |   | |_) | |___ / ___ \\ |_| | | |   | |   \n";
    cout << "|_____\\___//_/\\_\\\\___/|_| \\_\\|_|   |____/|_____/_/   \\_\\___/  |_|   |_|   \n\033[0m";
}

void displayStartPage() {
    system("cls");
    // Print the flower with "LUXURY BEAUTY" lower on the left side
    cout << colorText("                                                                                ,;;'''';;,                    ,;;;;, \n", PURPLE);
    cout << colorText("                                                                      ,        ;;'      `;;,               .,;;;'   ; \n", PURPLE);
    cout << colorText("                                                                   ,;;;       ;;          `;;,';;;,.     ,%;;'     ' \n", PURPLE);
    cout << colorText("                                                                 ,;;,;;       ;;         ,;`;;;, `;::.  %%;' \n", PURPLE);
    cout << colorText("                                                                ;;;,;;;       `'       ,;;; ;;,;;, `::,%%;' \n", PURPLE);
    cout << colorText("                                                                ;;;,;;;,          .,%%%%%'% ;;;;,;;   %;;; \n", PURPLE);
    cout << colorText("                                                         ,%,.   `;;;,;;;,    .,%%%%%%%%%'%; ;;;;;,;;  %;;; \n", PURPLE);
    cout << colorText("                                                        ;,`%%%%%%%%%%`;;,;;'%%%%%%%%%%%%%'%%'  `;;;;;,;, %;;; \n", PURPLE);
    cout << colorText("                                                        ;;;,`%%%%%%%%%%%,; ..`%%%%%%%%;'%%%'    `;;;;,;; %%;; \n", PURPLE);
    cout << colorText("                                                         `;;;;;,`%%%%%,;;/, .. `\"\"\"'',%%%%%      `;;;;;; %%;;, \n", PURPLE);
    cout << colorText("                                                            `;;;;;;;,;;/////,.    ,;%%%%%%%        `;;;;,`%%;; \n", PURPLE);
    cout << colorText("                                                                   ;;;/%%%%,%///;;;';%%%%%%,          `;;;%%;;, \n", PURPLE);
    cout << colorText("                                                                  ;;;/%%%,%%%%%/;;;';;'%%%%%,             `%%;; \n", PURPLE);
    cout << colorText("                                                                 .;;/%%,%%%%%//;;'  ;;;'%%%%%,             %%;;, \n", PURPLE);
    cout << colorText("                                                                 ;;//%,%%%%//;;;'   `;;;;'%%%%             `%;;; \n", PURPLE);
    cout << colorText("                                                                 ;;//%,%//;;;;'      `;;;;'%%%              %;;;, \n", PURPLE);
    cout << colorText("                                                                 `;;//,/;;;'          `;;;'%%'              `%;;; \n", PURPLE);
    cout << colorText("                                                                   `;;;;'               `;'%'                `;;;; \n", PURPLE);
    cout << colorText("                                                                      '      .,,,.        `;;;; \n", PURPLE);
    cout << colorText("                                                                          ,;;;;;;;;;;,     `;;;; \n", PURPLE);
    cout << colorText("                                                                         ;;;'    ;;;,;;,    `;;;; \n", PURPLE);
    cout << colorText("                                                                         ;;;      ;;;;,;;.   `;;;; \n", PURPLE);
    cout << colorText("                                                                          `;;      ;;;;;,;;   ;;;; \n", PURPLE);
    cout << colorText("                                                                            `'      `;;;;,;;  ;;;; \n", PURPLE);
    cout << colorText("                                                                                       `;;,;, ;;;; \n", PURPLE);
    cout << colorText("                                                                                          ;;, ;;;; \n", PURPLE);
    cout << colorText("                                                                                            ';;;;; \n", PURPLE);
    cout << colorText(" _                                  ____                   _ \n", PURPLE);
    cout << colorText("| |   _   ___  ___   _ _ __ _   _  | __ )  ___  __ _ _   _| |_ _   _ \n", PURPLE);
    cout << colorText("| |  | | | \\ \\/ / | | | '__| | | | |  _ \\ / _ \\/ _` | | | | __| | | |\n", PURPLE);
    cout << colorText("| |__| |_| |>  <| |_| | |  | |_| | | |_) |  __/ (_| | |_| | |_| |_| |\n", PURPLE);
    cout << colorText("|_____\\__,_/_/\\_\\\\__,_|_|   \\__, | |____/ \\___|\\__,_|\\__,_|\\__|\\__, |\n", PURPLE);
    cout << colorText("| __ )  __ _ _ __           |___/                              |___/ \n", PURPLE);
    cout << colorText("|  _ \\ / _` | '__|                                                   \n", PURPLE);
    cout << colorText("| |_) | (_| | |                                                      \n", PURPLE);
    cout << colorText("|____/ \\__,_|_|                                                      \n\n\n", PURPLE);
    system("pause");
}

void displayEnding() {
    system("cls");
    cout << colorText(" _____ _   _    _    _   _ _  ______    _____ ___  ____   __     _____ ____ ___ _____ \n", PURPLE);
    cout << colorText("|_   _| | | |  / \\  | \\ | | |/ / ___|  |  ___/ _ \\|  _ \\  \\ \\   / /_ _/ ___|_ _|_   _|\n", PURPLE);
    cout << colorText("  | | | |_| | / _ \\ |  \\| | ' /\\___ \\  | |_ | | | | |_) |  \\ \\ / / | |\\___ \\| |  | |  \n", PURPLE);
    cout << colorText("  | | |  _  |/ ___ \\| |\\  | . \\ ___) | |  _|| |_| |  _ <    \\ V /  | | ___) | |  | |  \n", PURPLE);
    cout << colorText(" _|_|_|_| |_/_/   \\_\\_| \\_|_|\\_\\____/ _|_|_ _\\___/|_|_\\_\\_   \\_/_ |___|____/___|_|_|  \n", PURPLE);
    cout << colorText("| | | |  / \\ \\   / / ____|    / \\    | \\ | |_ _/ ___| ____| |  _ \\  / \\ \\ / / | |    \n", PURPLE);
    cout << colorText("| |_| | / _ \\ \\ / /|  _|     / _ \\   |  \\| || | |   |  _|   | | | |/ _ \\ V /  | |    \n", PURPLE);
    cout << colorText("|  _  |/ ___ \\ V / | |___   / ___ \\  | |\\  || | |___| |___  | |_| / ___ \\| |  |_|    \n", PURPLE);
    cout << colorText("|_| |_/_/   \\_\\_/  |_____| /_/   \\_\\ |_| \\_|___\\____|_____| |____/_/   \\_\\_|  (_)    \n\n\n", PURPLE);


    cout << colorText("Exiting", CYAN);
    for (int i = 0; i < 6; i++) {
        cout << colorText(".", CYAN);
        cout.flush();
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << endl;
}

void displayMainMenu() {
    cout << "----------------------------\n";
    cout << "|        Main Menu         |\n";
    cout << "----------------------------\n";
    cout << "| 1. Customer              |\n"
        << "| 2. Staff                 |\n"
        << "| 3. More Information      |\n"
        << "| 4. Exit                  |\n";
    cout << "----------------------------\n";
    cout << "Enter option (1-4) >> ";
}

void displayCustomerMenu() {
    cout << "----------------------------\n";
    cout << "|      Customer Menu       |\n";
    cout << "----------------------------\n";
    cout << "| |S| View Service         |\n"
        << "| |E| View Expert          |\n"
        << "| |C| Check Schedule       |\n"
        << "| |B| Book Service         |\n"
        << "| |A| View Appointment     |\n"
        << "| |R| Sign Out and Return  |\n";
    cout << "----------------------------\n";
    cout << "Enter option [S/E/C/B/A/R] >> ";
}

void displayServiceList() {
    cout << "----------------------------\n";
    cout << "|       Service List       |\n";
    cout << "----------------------------\n";
    cout << "| 1. Makeup Service        |\n"
        << "| 2. Waxing                |\n"
        << "| 3. Laser Hair Removal    |\n"
        << "| 4. Return                |\n";
    cout << "----------------------------\n";
    cout << "Enter option (1-4) >> ";
}

void displayExpertList() {
    cout << "----------------------------\n";
    cout << "|       Expert  List       |\n";
    cout << "----------------------------\n";
    cout << "| 1. Steve                 |\n"
        << "| 2. Vernice               |\n"
        << "| 3. Paul                  |\n"
        << "| 4. Return                |\n";
    cout << "----------------------------\n";
    cout << "Enter option (1-4) >> ";
}

void displayService1Info(const ServiceType& service) {
    cout << "\t\t------------------------------\n";
    cout << "\t\t|      Makeup Service        |\n";
    cout << "\t\t------------------------------\n";
    cout << "\t\t| Cosmetic application for   |\n"
        << "\t\t| events like weddings and   |\n"
        << "\t\t| photoshoots, tailored to   |\n"
        << "\t\t| or transform appearance.   |\n";
    cout << "\t\t------------------------------\n";
    cout << "\t\t| Consultation Price: RM " + to_string(static_cast<int>(service.consultPrice)) + " |\n";
    cout << "\t\t| Treatment Price: RM " + to_string(static_cast<int>(service.treatPrice)) + "    |\n";
    cout << "\t\t----------------------------\n";
}

void displayService2Info(const ServiceType& service) {
    cout << "\t\t-------------------------------\n";
    cout << "\t\t|      Body Waxing            |\n";
    cout << "\t\t-------------------------------\n";
    cout << "\t\t| Body waxing removes hair by |\n"
        << "\t\t| applying warm and pulling   |\n"
        << "\t\t| it out from the roots,      |\n"
        << "\t\t| leaving smooth skin for     |\n"
        << "\t\t| weeks.Common areas include  |\n"
        << "\t\t| the face,body, and bikini   |\n"
        << "\t\t| line.Regular waxing can     |\n"
        << "\t\t| result in finer hair growth |\n";
    cout << "\t\t-------------------------------\n";
    cout << "\t\t| Consultation Price: RM " + to_string(static_cast<int>(service.consultPrice)) + "  |\n";
    cout << "\t\t| Treatment Price: RM " + to_string(static_cast<int>(service.treatPrice)) + "     |\n";
    cout << "\t\t-----------------------------\n";
}

void displayService3Info(const ServiceType& service) {
    cout << "\t\t-------------------------------\n";
    cout << "\t\t|      Lazer Hair Removal     |\n";
    cout << "\t\t-------------------------------\n";
    cout << "\t\t| Laser hair removal uses     |\n"
        << "\t\t| light to target and damage  |\n"
        << "\t\t| hairfollicle, reducing hair |\n"
        << "\t\t| growth.It's effective on    |\n"
        << "\t\t| various areas, requires     |\n"
        << "\t\t| multiple sessions, and lead |\n"
        << "\t\t| to finer, slower regrowth   |\n"
        << "\t\t| over time.                  |\n";
    cout << "\t\t-------------------------------\n";
    cout << "\t\t| Consultation Price: RM " + to_string(static_cast<int>(service.consultPrice)) + "  |\n";
    cout << "\t\t| Treatment Price: RM " + to_string(static_cast<int>(service.treatPrice)) + "     |\n";
    cout << "\t\t-------------------------------\n";
}

void displayExpertInfo(const Expert& expert) {
    cout << "-------------------------------------------------------" << endl;
    cout << "Expert Name      : " << expert.name << endl;
    cout << "Age              : " << expert.age << endl;
    cout << "Gender           : " << expert.gender << endl;
    cout << "Experience       : " << expert.experience << endl;
    cout << "Education        : " << expert.education << endl;
    cout << "-------------------------------------------------------" << endl;
}

void moreInfo() {
    system("cls");
    displayLogo();
    cout << "-----------------------------------------------------------------------" << endl;
    cout << "|                             ABOUT US                                 |" << endl;
    cout << "-----------------------------------------------------------------------" << endl;
    cout << "| At Luxe Beauty Bar, you'll receive a warm welcome and a luxurious    |" << endl;
    cout << "| escape from your daily stresses. We specialize in various services   |" << endl;
    cout << "| including makeup service, waxing service, and laser hair removal.    |" << endl;
    cout << "|                                                                      |" << endl;
    cout << "| Established in 2023 in the heart of Kuala Lumpur, we believe we      |" << endl;
    cout << "| have a responsibility to our clients - to make them look good and    |" << endl;
    cout << "| feel even better. Your beauty is our passion.                        |" << endl;
    cout << "|                                                                      |" << endl;
    cout << "| We encourage you to relax when you come in - sit back and sip a      |" << endl;
    cout << "| beverage while enjoying relaxing music and a stress-free setting.    |" << endl;
    cout << "| Our experts will handle all of your needs during your visit.         |" << endl;
    cout << "|                                                                      |" << endl;
    cout << "|                       Ways To Get In Touch                           |" << endl;
    cout << "|                                                                      |" << endl;
    cout << "| Contact Number: +6031352589                                          |" << endl;
    cout << "| Address\t: No 88, Jalan Sultan Ismail, Taman Bunga Raya,        |" << endl;
    cout << "|           \t  50250 Kuala Lumpur, Malaysia                         |" << endl;
    cout << "| Email\t\t: luxebeautybar88@gmail.com                            |" << endl;
    cout << "|                                                                      |" << endl;
    cout << "|For more information, visit our shop or contact us via email or phone.|" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    system("pause");
}

int getValidIntInput(int min, int max) {
    int option;
    while (true) {
        cin >> option;
        if (cin.fail() || option < min || option > max) {
            cin.clear();// clear the input prevent the while loop keep looping
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard previous input
            cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        }
        else {
            return option;
        }
    }
}

string getHiddenPassword() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') {  // Read characters until Enter is pressed
        if (ch == '\b') {  // Handle backspace
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
    return password;
};

char getValidCustomerInput() {
    char option;
    bool validInput = false;

    while (!validInput) {
        cin >> option;
        option = toupper(option);  // Convert to uppercase for case-insensitive comparison

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again enter a character |S|E|C|B|A|R|: ";
        }
        else if (option == 'S' || option == 'E' || option == 'C' || option == 'B' || option == 'A' || option == 'R') {
            return option;
            validInput = true;
        }
        else {
            cout << "Invalid option. Please try again enter a character |S|E|C|B|A|R|: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return option;
}

void customerMenu(Expert experts[], const ServiceType services[], const string days[], const string timeslots[]) {
    Customer currentCustomer;
    char customerInput;
    bool loginCusStatus = false;
    int indexExpert, indexWeek;
    do {
        if (!loginCusStatus) {
            customerAuthenticationPage(currentCustomer, &loginCusStatus);
        }
        else {
            do {
                system("cls");
                displayLogo();
                displayCustomerMenu();
                customerInput = getValidCustomerInput();
                //switch case
                switch (customerInput) {
                case 'S': serviceList(services); break;
                case 'E': expertList(experts); break;
                case 'C':
                    system("cls");
                    displayLogo();
                    cout << "Select Expert to view their schedules\n\n";
                    indexExpert = getIndexExpert();
                    if (indexExpert >= 0 && indexExpert <= 2) {
                        indexWeek = displayWeeksAndSelect(experts[indexExpert]);
                        if (indexWeek != -1) {
                            system("cls");
                            displayLogo();
                            displaySchedule(experts[indexExpert], indexWeek, days, timeslots);
                            cout << "Enter to Back to Menu\n";
                            system("pause");
                        }
                    };
                    break;
                case 'B':
                    system("cls");
                    displayLogo();
                    cout << "Select Expert to Book Schedules\n\n";
                    indexExpert = getIndexExpert();
                    if (indexExpert >= 0 && indexExpert <= 2) {
                        bool bookingStatus = false;
                        indexWeek = displayWeeksAndSelect(experts[indexExpert]);
                        if (indexWeek != -1) {
                            bookingStatus = makeBooking(experts[indexExpert], services, currentCustomer, indexWeek, days, timeslots);
                            if (!bookingStatus) {
                                cout << "Booking Process have been cancelled...\n";
                                system("pause");
                            }
                        }
                    }
                    break;
                case 'A': viewCustomerAppointments(currentCustomer, experts, days, timeslots); break;
                case 'R': loginCusStatus = false; cout << "Logging Out....\n"; system("pause");
                }
            } while (customerInput != 'R');
        }

    } while (loginCusStatus);
}

void customerAuthenticationPage(Customer& currentCustomer, bool* loginCusStatus) {
    int choice;
    do {
        system("cls");
        displayLogo();
        cout << "---------------------------------------------------------------------------\n";
        cout << "Welcome to Luxury Beauty Bar !" << endl;
        cout << "-------------------------------\n";
        cout << "|  1.Login                    |" << endl;
        cout << "|  2.Register                 |" << endl;
        cout << "|  3.Return To Main Menu      |" << endl;
        cout << "-------------------------------\n";
        cout << "Enter option (1-3) >> ";
        choice = getValidIntInput(1, 3);
        switch (choice) {
        case 1: cusLogin(currentCustomer, loginCusStatus); break;
        case 2: regCus(); break;
        case 3: system("cls"); break;
        default: cout << "Invalid Option Please Try again.\n";
            system("pause");
        }
    } while (choice != 3 && !*loginCusStatus);
}

void cusLogin(Customer& currentCustomer, bool* loginCusStatus) {
    system("cls");
    string inputUsername, inputPassword;
    string storedUN, storedPW, storedName, storedContact, storedEmail;
    string line;
    displayLogo();
    cout << "----------------------------\n";
    cout << "|        LOGIN PAGE        |\n";
    cout << "----------------------------\n";
    cout << "Enter your username: ";
    cin >> inputUsername;
    cout << "Enter your password: ";
    inputPassword = getHiddenPassword();

    bool found = false;
    ifstream read("customers.txt");
    if (!read) {
        cout << "No registered customers found." << endl;
        system("pause");
    }
    else {
        while (getline(read, line)) {
            // Find the positions of commas in the line
            int pos1 = line.find(',');
            int pos2 = line.find(',', pos1 + 1);
            int pos3 = line.find(',', pos2 + 1);
            int pos4 = line.find(',', pos3 + 1);

            // Extract individual details using substr
            storedUN = line.substr(0, pos1);
            storedPW = line.substr(pos1 + 1, pos2 - pos1 - 1);
            storedName = line.substr(pos2 + 1, pos3 - pos2 - 1);
            storedContact = line.substr(pos3 + 1, pos4 - pos3 - 1);
            storedEmail = line.substr(pos4 + 1);

            if (storedUN == inputUsername && storedPW == inputPassword) {
                currentCustomer.username = storedUN;
                currentCustomer.password = storedPW;
                currentCustomer.name = storedName;
                currentCustomer.contact = storedContact;
                currentCustomer.email = storedEmail;
                found = true;
                break;
            }
        }

        if (found) {
            cout << "Login successful!" << endl;
            system("pause");
            *loginCusStatus = true;
        }
        else {
            cout << "Invalid username or password." << endl;
            system("pause");
        }
    }
    read.close();
}

void regCus() {
    system("cls");
    Customer registerCustData;
    bool hasUpper, hasLower, validName, validContact, validEmail;
    bool usernameExists = false;
    displayLogo();
    cout << "-------------------------------\n";
    cout << "|        REGISTER PAGE        |\n";
    cout << "-------------------------------\n";
    cout << "| Username                    |\n"
        << "| - 6 to 8 character          |\n"
        << "| - Cannot contain space      |\n"
        << "| Password                    |\n"
        << "| - Need a Upper & Lower case |\n"
        << "| - 8 to 20 character         |\n";
    cout << "-------------------------------\n";

    cout << endl;

    cin.ignore();
    do {
        cout << "Create username: ";
        getline(cin, registerCustData.username);

        // Check if the username already exists
        ifstream read("customers.txt");
        string line;

        while (getline(read, line)) {
            int pos = line.find(',');
            string storedUN = line.substr(0, pos);
            if (storedUN == registerCustData.username) {
                usernameExists = true;
                break;
            }
            else {
                usernameExists = false;
            }
        }
        read.close();

        if (usernameExists) {
            cout << "Username already exists. Please choose a different username." << endl;
            continue;
        }


        if (registerCustData.username.length() < 6 || registerCustData.username.length() > 8 || registerCustData.username.find(' ') != string::npos) {
            cout << "Username must be in between 6 to 8 characters\n";
            cout << "Username cannot contain space\n";
        }

    } while ((registerCustData.username.length() < 6 || registerCustData.username.length() > 8) || registerCustData.username.find(' ') != string::npos || usernameExists);


    do {
        hasLower = false;
        hasUpper = false;
        cout << "Create password: ";
        registerCustData.password = getHiddenPassword();

        for (char c : registerCustData.password) {
            if (isupper(c)) hasUpper = true;
            if (islower(c)) hasLower = true;
        }

        if (registerCustData.password.length() < 8 || registerCustData.password.length() > 20 || !hasUpper || !hasLower) {
            cout << "Password must be at least 8 characters long and cannot exceed 20 characters." << endl;
            cout << "Password must contain at least one uppercase and one lowercase letter." << endl;
        }

    } while ((registerCustData.password.length() < 8 || registerCustData.password.length() > 20) || !(hasLower) || !(hasUpper));

    do {
        validName = true;

        cout << "Enter your name: ";
        getline(cin, registerCustData.name);

        for (char c : registerCustData.name) {
            if (!isalpha(c) && !isspace(c)) {
                validName = false;
                break;
            }
        }

        if (registerCustData.name.length() < 3 || registerCustData.name.length() > 30 || !validName) {
            cout << "Name must be between 3 and 30 characters long." << endl;
            cout << "Name can only contain letters and spaces." << endl;
            continue;
        }
    } while ((registerCustData.name.length() < 3 || registerCustData.name.length() > 30) || !validName);

    do {
        cout << "Enter your phone number: ";
        getline(cin, registerCustData.contact);

        validContact = true;
        for (char c : registerCustData.contact) {
            if (!isdigit(c)) {
                validContact = false;
                break;
            }
        }

        if (!validContact || registerCustData.contact.length() < 10 || registerCustData.contact.length() > 15) {
            cout << "Contact number can only contain digits.\nMust between (10-15) digits" << endl;
        }
    } while (!validContact || registerCustData.contact.length() < 10 || registerCustData.contact.length() > 15);

    do {
        cout << "Enter your email address: ";
        getline(cin, registerCustData.email);

        // Simple email validation using regex
        regex emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        validEmail = regex_match(registerCustData.email, emailRegex);

        if (!validEmail) {
            cout << "Invalid email format. Please enter a valid email address." << endl;
        }
    } while (!validEmail);


    // Open the file in append mode
    ofstream outCustomerFile("customers.txt", ios::app);
    if (outCustomerFile.fail()) {
        cout << "Failed to open customer file." << endl;
        system("pause");
    }

    // Write all customer details in a single line
    outCustomerFile << registerCustData.username << "," << registerCustData.password << ","
        << registerCustData.name << "," << registerCustData.contact << "," << registerCustData.email << endl;
    outCustomerFile.close();
    cout << "Registration successful! Please go to login." << endl;
    system("pause");
}

void serviceList(const ServiceType services[]) {
    int serviceChoice;
    do {
        system("cls");
        displayLogo();
        displayServiceList();
        serviceChoice = getValidIntInput(1, 4);

        if (serviceChoice <= 3 && serviceChoice >= 1) {
            servicePage(services, serviceChoice);
        }
        else {
            cout << "Exiting" << endl;
        }
    } while (serviceChoice != 4);
}

void servicePage(const ServiceType services[], int serviceChoice) {
    system("cls");
    displayLogo();
    if (serviceChoice == 1) {
        displayService1Info(services[serviceChoice - 1]);
    }
    else if (serviceChoice == 2) {
        displayService2Info(services[serviceChoice - 1]);
    }
    else if (serviceChoice == 3) {
        displayService3Info(services[serviceChoice - 1]);
    }
    cout << endl;
    system("pause");
}

void expertList(Expert experts[]) {
    int option;
    do {
        system("cls");
        displayLogo();
        displayExpertList();
        option = getValidIntInput(1, 4);
        if (option <= 3 && option >= 1) {
            system("cls");
            displayLogo();
            displayExpertInfo(experts[option - 1]);
            system("pause");
        }
        else {
            cout << "Exiting" << endl;
        }
    } while (option != 4);
}

string generateSlotID(char prefix, int slotNumber) {
    return string(1, prefix) + string(3 - to_string(slotNumber).length(), '0') + to_string(slotNumber);
}

void initializeExpertSlots(Expert& expert) {
    int slotCounter = 1;
    for (int i = 0; i < 4; i++) {
        int date = 1 + (i * 7);

        for (int j = 0; j < 5; j++) {
            int day = j + 1;

            for (int z = 0; z < 6; z++) {
                int timeSlot = z + 1;
                int slotIndex = j * 6 + z;
                expert.slots[i][slotIndex].slotID = generateSlotID(expert.name[0], slotCounter);
                expert.slots[i][slotIndex].day = day;
                expert.slots[i][slotIndex].date = date;
                expert.slots[i][slotIndex].status = AVAILABLE;
                expert.slots[i][slotIndex].timeSlot = timeSlot;
                slotCounter++;
            }
            date++;
            //cout << endl;
        }
    }
}

void retrieveBookings(Expert& expert, const ServiceType services[]) {
    for (int week = 0; week < 4; week++) {
        ifstream inExpertFile("Expert\\" + expert.name + "\\week" + to_string(week + 1) + ".txt");

        if (!inExpertFile) {
            cout << "Unable to open expert file" << endl;
        }

        string readSlotID, readCusUsername;
        int readStatus, readDay, readDate, readTimeSlot, readServiceID, readPaymentMethod;
        double readtotalAmount;

        while (inExpertFile >> readSlotID >> readStatus >> readDay >> readDate >> readTimeSlot >> readServiceID >> readPaymentMethod >> readtotalAmount >> readCusUsername) {
            // Find the corresponding slot
            for (int slotIndex = 0; slotIndex < 30; slotIndex++) {
                if (expert.slots[week][slotIndex].slotID == readSlotID) {
                    // Update the slot information
                    expert.slots[week][slotIndex].status = static_cast<bookingType>(readStatus);
                    expert.slots[week][slotIndex].cusUsername = readCusUsername;
                    expert.slots[week][slotIndex].paymentUse = static_cast<paymentMethod>(readPaymentMethod);
                    expert.slots[week][slotIndex].totalPrice = readtotalAmount;
                    // Find and set the correct service
                    for (int i = 0; i < 3; i++) {
                        if (services[i].serviceID == readServiceID) {
                            expert.slots[week][slotIndex].service = services[i];
                            break;
                        }
                    }
                    int retrieveWorkingHrs = 0;
                    //update expert's working hours
                    // Only update working hours for CONSULTATION and TREATMENT
                    if (readStatus == CONSULTATION || readStatus == TREATMENT) { retrieveWorkingHrs = (readStatus == CONSULTATION) ? 1 : 2; }
                    expert.hoursworked[week][readDay - 1] += retrieveWorkingHrs;
                    break;
                }
            }
        }
        inExpertFile.close();
    }
}

int getIndexExpert() {
    int expertChoice;
    displayExpertList();
    expertChoice = getValidIntInput(1, 4);
    return expertChoice - 1;
}

int countAvailableSlots(Expert& expert, int week) {
    int count = 0;
    for (int i = 0; i < 30; i++) {
        if (expert.slots[week][i].status == AVAILABLE) {
            count++;
        }
    }
    return count;
}

void updateBookingStatus(Expert& expert) {
    bool isMarkAvailable = false;
    for (int i = 0; i < 4; i++) {
        for (int day = 0; day < 5; day++) {
            if (expert.hoursworked[i][day] == 6) {
                isMarkAvailable = true;
            }
            else if (expert.hoursworked[i][day] < 6) { //Some booking may be refund or delete by admin so need to update back the slot status also
                isMarkAvailable = false;
            }
            markRemainingSlotStatus(expert, i, day, isMarkAvailable);
        }
    }
}

bool isWeekFullyBooked(const Expert& expert, int week) {
    bool isFullyBooked = true;
    for (int day = 0; day < 5; day++) {
        if (expert.hoursworked[week][day] < 6) {
            isFullyBooked = false;
            return isFullyBooked;// If any day is not fully booked, the week is not fully booked
        }
    }
    return isFullyBooked;  // All days are fully booked
}

int displayWeeksAndSelect(Expert& expert) {
    bool anyWeekAvailable = false, weekFullyBooked = false;
    system("cls");
    displayLogo();
    cout << "\nAvailable slots for " << expert.name << " Schedule:\n\n";
    for (int i = 0; i < 4; i++) {

        weekFullyBooked = isWeekFullyBooked(expert, i);

        if (weekFullyBooked == false) {
            anyWeekAvailable = true;
            int availableSlots = countAvailableSlots(expert, i);
            cout << "Week " << i + 1 << ": Available slots: " << availableSlots << endl;
            cout << "----------------------------------------------\n";
            cout << "  Day     || Mon | Tue | Wed | Thu | Fri |\n";
            cout << "  Date    || ";
            for (int j = 0; j < 5; j++) {
                cout << setw(3) << expert.slots[i][j * 6].date << " | ";
            }
            cout << "\n----------------------------------------------\n";
        }
        else {
            cout << "Week " << i + 1 << colorText(" No more slot Available\n", LIGHTRED) << endl;
        }

    }

    int week;
    if (!anyWeekAvailable) {
        cout << colorText("All weeks are fully booked for this expert.\n", LIGHTRED);
        system("pause");
        week = 0;// Return a special value to indicate no available weeks
    }

    cout << "Enter week number [1-4/0 to Cancel] >> ";
    week = getValidIntInput(0, 4);
    return week - 1; // Return 0-based week index
}

void markRemainingSlotStatus(Expert& expert, int week, int day, bool makeAvailable) {
    for (int i = 0; i < 6; i++) {
        int slotIndex = day * 6 + i;
        if (makeAvailable) {
            if (expert.slots[week][slotIndex].status == AVAILABLE) {
                expert.slots[week][slotIndex].status = NOT_AVAILABLE;
            }
        }
        else {
            if (expert.slots[week][slotIndex].status == NOT_AVAILABLE) {
                expert.slots[week][slotIndex].status = AVAILABLE;
            }
        }
    }
}

void displaySchedule(Expert& expert, int week, const string days[], const string timeSlots[], bool isExpertView) { //not sure can anot
    cout << "\nSchedule for " << expert.name << "\n(Week " << week + 1 << "):" << endl;
    cout << string(92, '-') << endl;
    cout << setw(10) << "Time";
    for (int i = 0; i < 5; i++) {
        cout << " | " << setw(22) << colorText(days[i], BLUE);
    }
    cout << " |" << endl;
    cout << setw(10) << "";
    for (int i = 0; i < 5; i++) {
        int date = expert.slots[week][i * 6].date;
        cout << " | " << setw(22) << colorText(to_string(date) + "/7/2024", BLUE);
    }
    cout << " |" << endl;
    cout << string(92, '-') << endl;

    for (int t = 0; t < 6; t++) {
        cout << setw(10) << timeSlots[t];
        for (int d = 0; d < 5; d++) {
            int slotIndex = d * 6 + t;
            cout << " | ";
            if (isExpertView) {
                switch (expert.slots[week][slotIndex].status) {
                case TREATMENT:
                case CONSULTATION:
                    cout << setw(22) << colorText(expert.slots[week][slotIndex].slotID, CYAN);
                    break;
                case AVAILABLE:
                    cout << setw(22) << colorText("####", PURPLE);
                    break;
                case NOT_AVAILABLE:
                    cout << setw(22) << colorText("----", RED);
                    break;
                }
            }
            else {
                if (expert.hoursworked[week][d] >= 6) {
                    cout << setw(22) << colorText("FULL", PURPLE);
                }
                else if (expert.slots[week][slotIndex].status == AVAILABLE) {
                    cout << setw(22) << colorText(expert.slots[week][slotIndex].slotID, CYAN);
                }
                else {
                    cout << setw(22) << colorText("XXXX", LIGHTRED);
                }
            }
        }
        cout << " |" << endl;
    }
    cout << string(92, '-') << endl;

    // Display hours worked for each day
    cout << setw(10) << "Hours";
    for (int d = 0; d < 5; d++) {
        cout << " | " << setw(22) << colorText(to_string(expert.hoursworked[week][d]) + "/6", (expert.hoursworked[week][d] >= 6.0 ? YELLOW : GREEN));
    }
    cout << " |" << endl;
    cout << string(92, '-') << endl;
}

paymentMethod processPayment(double amount, double& taxAmount, double& totalAmount) {
    int choice;
    paymentMethod methodChoice = NONE;
    double bookingTax = 0.05;
    taxAmount = 0.05 * amount;
    totalAmount = amount + taxAmount;
    system("cls");
    displayLogo();
    taxAmount = amount * 0.05;
    cout << "\nPayment Process\n";
    cout << "---------------\n";
    cout << "Price: RM " << fixed << setprecision(2) << amount << endl;
    cout << "Booking Tax 5%: RM " << fixed << setprecision(2) << taxAmount << endl;
    cout << "Total amount: RM " << fixed << setprecision(2) << totalAmount << endl;
    cout << "\nChoose payment method:\n";
    cout << "1. E-Wallet\n";
    cout << "2. Online Banking\n";
    cout << "3. Credit Card\n";
    cout << "4. Cancel Payment\n";

    cout << "Enter choice (1-4) >> ";
    choice = getValidIntInput(1, 4);

    switch (choice) {
    case 1: methodChoice = ewallet(); break;
    case 2: methodChoice = online_bank(); break;
    case 3: methodChoice = creditcard(); break;
    default:
        cout << "Payment cancelled.\n";
    }

    return methodChoice;
}

string generateOTP() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }
    return to_string(rand() % 900000 + 100000);
}

paymentMethod online_bank() {
    string cust_name, bank_num, input_pin;
    string otp = generateOTP();  // Generate OTP

    system("cls");
    displayLogo();
    cout << "------------------------------" << endl;
    cout << "|       ONLINE BANKING       |" << endl;
    cout << "------------------------------" << endl;

    // Validate bank account name
    bool valid_name = false;
    cin.ignore();
    while (!valid_name) {
        cout << "Enter Bank Account name (5-30 alphabetic characters): ";
        getline(cin, cust_name);

        valid_name = (cust_name.length() >= 5 && cust_name.length() <= 30);
        for (char c : cust_name) {
            if (!isalpha(c) && c != ' ') {
                valid_name = false;
                break;
            }
        }

        if (!valid_name) {
            cout << "Invalid name. Please use only alphabetic characters and spaces, 5-30 characters long." << endl;
        }
    }

    // Validate bank account number
    bool valid_number = false;
    while (!valid_number) {
        cout << "Enter bank account number (10 digits): ";
        getline(cin, bank_num);

        valid_number = (bank_num.length() == 10);
        for (char c : bank_num) {
            if (!isdigit(c)) {
                valid_number = false;
                break;
            }
        }

        if (!valid_number) {
            cout << "Invalid account number. Please enter exactly 10 digits." << endl;
        }
    }

    cout << "Generated OTP: " << otp << endl;
    cout << "A 6-digit OTP has been sent to your contact. Please enter it to make a payment." << endl;

    bool otpValidated = false;
    while (!otpValidated) {
        input_pin.clear();
        cout << "Enter OTP (6 digits): ";
        input_pin = getHiddenPassword();
        if (input_pin == otp) {
            otpValidated = true;
        }
        else {
            cout << "Invalid otp\n";
        }
    }
    return ONLINE_BANKING;
}

paymentMethod ewallet() {
    string contact, input_pin;
    string otp = generateOTP();  // Generate OTP
    system("cls");
    displayLogo();
    cout << "------------------------------" << endl;
    cout << "|          E-WALLET          |" << endl;
    cout << "------------------------------" << endl;

    // Contact number validation
    bool valid_contact = false;
    while (!valid_contact) {
        cout << "Enter customer contact (only numbers): ";
        cin >> contact;

        if (contact.length() >= 10 && contact.length() <= 15) {
            valid_contact = true;
            for (char digit : contact) {
                if (!isdigit(digit)) {
                    valid_contact = false;
                    break;
                }
            }
        }

        if (!valid_contact) {
            cout << "Invalid contact number. Please enter  10 to 15 numbers" << endl;
        }
    }

    // Display the generated OTP (for debugging purposes)
    cout << "Generated OTP: " << otp << endl;
    cout << "A 6-digit OTP has been sent to your contact. Please enter it to make a payment." << endl;

    bool otpValidated = false;
    while (!otpValidated) {
        input_pin.clear();
        cout << "Enter OTP (6 digits): ";
        input_pin = getHiddenPassword();
        if (input_pin == otp) {
            otpValidated = true;
        }
        else {
            cout << "Invalid otp\n";
        }
    }
    return E_WALLET;
}

paymentMethod creditcard() {
    string card_num, cvv;
    system("cls");
    displayLogo();
    cout << "------------------------------" << endl;
    cout << "|         CREDIT CARD        |" << endl;
    cout << "------------------------------" << endl;

    // Credit card number validation
    bool valid_card = false;
    cin.ignore();
    while (!valid_card) {
        cout << "Enter credit card number (Format: XXXX-XXXX-XXXX-XXXX or XXXX XXXX XXXX XXXX): ";
        getline(cin, card_num);

        // Remove any spaces or hyphens
        card_num.erase(remove(card_num.begin(), card_num.end(), ' '), card_num.end());
        card_num.erase(remove(card_num.begin(), card_num.end(), '-'), card_num.end());

        if (card_num.length() != 16) {
            cout << "Invalid card number length. Please enter a 16-digit number." << endl;
            continue;
        }

        // Check if all characters are digits
        valid_card = true;
        for (char c : card_num) {
            if (!isdigit(c)) {
                valid_card = false;
                break;
            }
        }

        if (!valid_card) {
            cout << "Invalid credit card number. Please enter only digits." << endl;
        }
    }

    // CVV validation
    bool valid_cvv = false;
    while (!valid_cvv) {
        cout << "Enter CVV (3 or 4 digits): ";
        cin >> cvv;

        if (cvv.length() == 3 || cvv.length() == 4) {
            valid_cvv = true;
            for (char c : cvv) {
                if (!isdigit(c)) {
                    valid_cvv = false;
                    break;
                }
            }
        }

        if (!valid_cvv) {
            cout << "Invalid CVV. Please enter 3 or 4 digits." << endl;
        }
    }
    cout << "Credit card validated successfully!" << endl;
    cout << "Card number: XXXX-XXXX-XXXX-" << card_num.substr(12) << endl;
    cout << "CVV: " << string(cvv.length(), '*') << endl;
    system("pause");
    return CREDIT_CARD;
}

void generateReceipt(const Expert& expert, const Customer& customer, int week, int slotIndex, const string days[], const string& timeRange, double price, double taxAmount, double totalAmount) {
    system("cls");
    const int width = 50;
    string serviceType = (expert.slots[week][slotIndex].status == CONSULTATION) ? "Consultation" : "Treatment";
    cout << string(width, '-') << endl;
    cout << setw(35) << right << "LUXURY BEAUTY BAR" << endl;
    cout << setw(35) << right << "Booking Receipt" << endl;
    cout << string(width, '-') << endl;
    cout << "Receipt#: " << expert.slots[week][slotIndex].slotID << endl;
    cout << "Date: " << expert.slots[week][slotIndex].date << "/7/2024" << endl;
    cout << endl;
    cout << "Customer: " << customer.username << endl;
    cout << "Expert: " << expert.name << endl;
    cout << "Service: " << expert.slots[week][slotIndex].service.name << endl;
    cout << serviceType << endl;
    cout << string(width, '-') << endl;
    cout << right << setw(40) << "Sub Total: " << setw(10) << fixed << setprecision(2) << price << endl;
    cout << right << setw(40) << "Tax (5%): " << setw(10) << fixed << setprecision(2) << taxAmount << endl;
    cout << right << setw(40) << "TOTAL: " << setw(10) << fixed << setprecision(2) << totalAmount << endl;
    cout << endl;
    cout << "Payment Method: ";
    switch (expert.slots[week][slotIndex].paymentUse) {
    case E_WALLET: cout << "E-WALLET"; break;
    case ONLINE_BANKING: cout << "Online Banking"; break;
    case CREDIT_CARD: cout << "Credit Card"; break;
    default: cout << "Unknown";
    }
    cout << endl << endl;
    cout << "Booking Details:" << endl;
    cout << "Week " << week + 1 << ", " << days[expert.slots[week][slotIndex].day - 1] << endl;
    cout << "Time: " << timeRange << endl;
    cout << endl;
    cout << setw(width / 2) << right << "Thanks for your business." << endl;
    cout << string(width, '-') << endl;



    string filename = "Receipts\\receipt_" + expert.slots[week][slotIndex].slotID + ".txt";
    ofstream outReceiptFile(filename);

    if (!outReceiptFile) {
        cout << "Error opening the output file";
    }

    outReceiptFile << string(width, '-') << endl;
    outReceiptFile << setw(35) << right << "LUXURY BEAUTY BAR" << endl;
    outReceiptFile << setw(35) << right << "Booking Receipt" << endl;
    outReceiptFile << string(width, '-') << endl;
    outReceiptFile << "Receipt#: " << expert.slots[week][slotIndex].slotID << endl;
    outReceiptFile << "Date: " << expert.slots[week][slotIndex].date << "/7/2024" << endl;
    outReceiptFile << endl;
    outReceiptFile << "Customer: " << customer.username << endl;
    outReceiptFile << "Expert: " << expert.name << endl;
    outReceiptFile << "Service: " << expert.slots[week][slotIndex].service.name << endl;
    outReceiptFile << serviceType << endl;
    outReceiptFile << string(width, '-') << endl;
    outReceiptFile << right << setw(40) << "Sub Total: " << setw(10) << fixed << setprecision(2) << price << endl;
    outReceiptFile << right << setw(40) << "Tax (5%): " << setw(10) << fixed << setprecision(2) << taxAmount << endl;
    outReceiptFile << right << setw(40) << "TOTAL: " << setw(10) << fixed << setprecision(2) << totalAmount << endl;
    outReceiptFile << endl;
    outReceiptFile << "Payment Method: ";
    switch (expert.slots[week][slotIndex].paymentUse) {
    case E_WALLET: outReceiptFile << "E-WALLET"; break;
    case ONLINE_BANKING: outReceiptFile << "Online Banking"; break;
    case CREDIT_CARD: outReceiptFile << "Credit Card"; break;
    default: outReceiptFile << "Unknown";
    }
    outReceiptFile << endl << endl;
    outReceiptFile << "Booking Details:" << endl;
    outReceiptFile << "Week " << week + 1 << ", " << days[expert.slots[week][slotIndex].day - 1] << endl;
    outReceiptFile << "Time: " << timeRange << endl;
    outReceiptFile << endl;
    outReceiptFile << setw(width / 2) << right << "Thanks for your business." << endl;
    outReceiptFile << string(width, '-') << endl;

    outReceiptFile.close();

#ifdef _WIN32
    string command = "notepad " + filename;
#else
    string command = "open " + filename;
#endif
    system(command.c_str()); // need to close only can continue
    cout << "Receipt generated and saved as " << filename << endl;
}

string calculateTimeRange(int slotIndex, bookingType typeChoice) {
    int startHour = 8 + 2 * (slotIndex % 6);
    string timeRange;
    if (typeChoice == CONSULTATION) { // Consultation
        timeRange = to_string(startHour) + "-" + to_string(startHour + 1);
    }
    else { // Treatment
        timeRange = to_string(startHour) + "-" + to_string(startHour + 2);
    }
    return timeRange;
}

bool makeBooking(Expert& expert, const ServiceType services[], const Customer& currentCustomer, int week, const string days[], const string timeSlots[]) {
    string userSlotID;
    int serviceIndex;
    system("cls");
    displayLogo();
    displaySchedule(expert, week, days, timeSlots);

    bool slotFound = false;
    int slotIndex = 0;
    cin.ignore();
    do {
        cout << "Please Q to quit\nEnter the slot ID you want to book: ";
        getline(cin, userSlotID);

        if (userSlotID == "Q" || userSlotID == "q") {
            return false;
        }
        else {
            //Find the slot with the given ID
            for (int j = 0; j < 30; j++) {
                slotIndex = j;
                if (expert.slots[week][slotIndex].slotID == userSlotID) {
                    slotFound = true;
                    break;
                }
            }
            if (!slotFound || expert.slots[week][slotIndex].status != AVAILABLE) {
                slotFound = false;
                cout << colorText("Invalid the slot ID or the ID already booked Please Enter Again", LIGHTRED) << endl;
            }
        }
    } while (!slotFound);

    int day = expert.slots[week][slotIndex].day - 1;
    int currentHours = expert.hoursworked[week][day];

    cout << "\nAvailable services:" << endl;
    for (int i = 0; i < 3; i++) {
        cout << i + 1 << ". " << services[i].name << endl;
    }
    cout << "Enter service number: ";
    serviceIndex = getValidIntInput(1, 3);
    serviceIndex--; // Adjust for 0-based indexing
    int bookingTypeChoice;

    if (currentHours == 5) {
        cout << "Only 1-hour consultation is available for this slot.\nEnter y to continue or other to cancel: ";
        char choice;
        cin >> choice;
        if (toupper(choice) == 'Y') {
            bookingTypeChoice = 2; // Consultation
        }
        else {
            return false;
        }
    }
    else {
        cout << "\nChoose:\n";
        cout << "1.Treatment [2 hours]" << endl;
        cout << "2.Consulation [1 hours]" << endl;
        cout << "Do you want a treatment (1) or  consultation (2) or cancel (3)? ";
        bookingTypeChoice = getValidIntInput(1, 3);
        if (bookingTypeChoice == 3) {
            return false;
        }
    }


    // Calculate duration and price
    int duration = (bookingTypeChoice == 2) ? 1 : 2; // 1 hour for consultation, 2 hours for treatment
    double price = (bookingTypeChoice == 2) ? services[serviceIndex].consultPrice : services[serviceIndex].treatPrice;
    double taxAmount = 0.0, totalAmount = 0.0;
    //Calculate the time slot
    string timeRange = calculateTimeRange(slotIndex, static_cast<bookingType>(bookingTypeChoice - 1));

    // Display booking information
    system("cls");
    displayLogo();
    cout << "\n\nBooking Information:" << endl;
    displayBookingDetails(userSlotID, expert.name, services[serviceIndex].name, static_cast<bookingType>(bookingTypeChoice - 1), week, days[expert.slots[week][slotIndex].day - 1], expert.slots[week][slotIndex].date, timeRange);

    // Confirm booking
    char confirm;
    cout << "\nDo you want to confirm this booking? (Y to confirm others to Cancel): ";
    cin >> confirm;
    if (toupper(confirm) != 'Y') {
        return false;
    }
    //Payment process
    expert.slots[week][slotIndex].paymentUse = processPayment(price, taxAmount, totalAmount);

    if (expert.slots[week][slotIndex].paymentUse == NONE) {
        return false;
    }

    //Update work hours for the expert
    expert.hoursworked[week][expert.slots[week][slotIndex].day - 1] += duration;
    // Update booking information
    expert.slots[week][slotIndex].status = (bookingTypeChoice == 2) ? CONSULTATION : TREATMENT;
    expert.slots[week][slotIndex].service = services[serviceIndex];
    expert.slots[week][slotIndex].cusUsername = currentCustomer.username;
    expert.slots[week][slotIndex].totalPrice = totalAmount;
    // check wheter the day have full of 6 hours if have then update the remain status to unavailble
    updateBookingStatus(expert);
    cout << "Booking confirmed successfully!" << endl;

    // save into files
    ofstream outExpertFile("Expert\\" + expert.name + "\\week" + to_string(week + 1) + ".txt", ios::app);
    if (!outExpertFile) {
        cout << "Error opening the output file";
    }

    outExpertFile << expert.slots[week][slotIndex].slotID << " "
        << expert.slots[week][slotIndex].status << " " << expert.slots[week][slotIndex].day << " " << expert.slots[week][slotIndex].date
        << " " << expert.slots[week][slotIndex].timeSlot << " " << expert.slots[week][slotIndex].service.serviceID << " " << expert.slots[week][slotIndex].paymentUse << " "
        << expert.slots[week][slotIndex].totalPrice << " " << expert.slots[week][slotIndex].cusUsername << endl;
    outExpertFile.close();
    generateReceipt(expert, currentCustomer, week, slotIndex, days, timeRange, price, taxAmount, totalAmount);
    system("pause");
    return true;
}

int linearSearch(const Booking slot[], int n, const string& val) {
    // Going through array linearly
    for (int i = 0; i < n; i++)
    {
        if (slot[i].cusUsername == val)
            return i + 1;
    }
    return -1;
}

void displayBookingDetails(string slotID, string expertName, string service, bookingType typeStatus, int week, string day, int date, string timeRange) {
    cout << colorText("------------------------------------------------\n", DARKPURPLE);
    cout << colorText("Booking ID: " + slotID, CYAN) << endl;
    cout << colorText("------------------------------------------------\n", DARKPURPLE);
    cout << colorText("Expert: " + expertName, CYAN) << endl;
    cout << colorText("Service: " + service, CYAN) << endl;
    cout << colorText("Type: " + string(typeStatus == CONSULTATION ? "Consultation" : "Treatment"), CYAN) << endl;
    cout << colorText("Week: " + to_string(week + 1) + ", Day: " + day + ", Date: " + to_string(date) + "/7/2024", CYAN) << endl;
    cout << colorText("Duration: " + string(typeStatus == CONSULTATION ? "1" : "2") + " hour(s)", CYAN) << endl;
    cout << colorText("Time: " + timeRange, CYAN) << endl;
    cout << colorText("------------------------------------------------\n", DARKPURPLE);
}

void viewCustomerAppointments(const Customer& customer, const Expert experts[], const string days[], const string timeSlots[]) {
    system("cls");
    displayLogo();
    cout << colorText("\nYour Appointments:\n\n", LIGHTYELLOW);
    bool hasAppointments = false;

    for (int e = 0; e < 3; e++) {  // Iterate through all experts
        for (int w = 0; w < 4; w++) {  // Iterate through all weeks
            int n = 30; // size of array (slots per week)
            int searchStart = 0;
            bool foundAppointments = true;

            while (foundAppointments && searchStart < n) {
                int res = linearSearch(experts[e].slots[w] + searchStart, n - searchStart, customer.username);

                if (res == -1) {
                    foundAppointments = false;  // No more appointments found in this week
                }
                else {
                    hasAppointments = true;
                    int s = searchStart + res - 1; // Adjust index

                    // Display the found appointment
                    int day = experts[e].slots[w][s].day - 1;

                    displayBookingDetails(experts[e].slots[w][s].slotID,
                        experts[e].name, experts[e].slots[w][s].service.name,
                        experts[e].slots[w][s].status, w,
                        days[day], experts[e].slots[w][s].date, calculateTimeRange(s, experts[e].slots[w][s].status));
                    cout << endl;
                    // Move search start to next position
                    searchStart = s + 1;
                }
            }
        }
    }

    if (!hasAppointments) {
        cout << colorText("You have no appointments booked.\n\n", PURPLE);
    }
    system("pause");
}

int staffLogin(const Staff staffs[]) {
    string username, password;
    cout << "\nStaff Login" << endl;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    password = getHiddenPassword();

    for (int i = 0; i < 4; i++) {
        if (staffs[i].username == username && staffs[i].password == password) {
            cout << "Login successful. Welcome, " << staffs[i].username << "!" << endl;
            system("pause");
            return i;
        }
    }
    cout << "Invalid username or password." << endl;
    system("pause");
    return -1;
}

void staffPage(Expert experts[], const ServiceType services[], const string days[], const string timeslots[]) {
    Staff staffs[4] = {
    {"Steve", "steve123", EXPERT},
    {"Vernice", "vernice123", EXPERT},
    {"Paul", "paul123", EXPERT},
    {"Admin", "admin123", ADMIN}
    };

    int staffIndex = staffLogin(staffs);
    if (staffIndex != -1) {
        if (staffs[staffIndex].role == EXPERT) {
            expertMenu(experts, services, days, timeslots, staffIndex);
        }
        else if (staffs[staffIndex].role == ADMIN) {
            adminMenu(experts, services, days, timeslots);
        }
        system("pause");
    }
}

void expertMenu(Expert experts[], const ServiceType services[], const string days[], const string timeSlots[], int staffIndex) {
    int choice;
    do {
        system("cls");
        displayLogo();
        cout << "-----------------------------\n";
        cout << "|        Expert Menu        |\n";
        cout << "-----------------------------\n";
        cout << "| |1| View Customer Details |\n"
            << "| |2| View My Schedule      |\n"
            << "| |3| View Sales Report     |\n"
            << "| |4| Return & Sign Up      |\n";
        cout << "----------------------------\n";
        cout << "Enter option (1-4) >> ";
        choice = getValidIntInput(1, 4);
        switch (choice) {
        case 1: viewCustomerDetails(experts); break;
        case 2: viewIndividualSchedule(experts[staffIndex], days, timeSlots); break;
        case 3:viewSalesReport(experts, services); break;
        case 4: cout << "Logging out...\n"; break;
        default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);
}

void adminMenu(Expert experts[], const ServiceType services[], const string days[], const string timeSlots[]) {
    int choice;
    do {
        system("cls");
        displayLogo();
        cout << "-----------------------------\n";
        cout << "|        Admin Menu         |\n";
        cout << "-----------------------------\n";
        cout << "| |1| View Customer Details |\n"
            << "| |2| View Overall Schedule |\n"
            << "| |3| View Sales Report     |\n"
            << "| |4| Refund Booking        |\n"
            << "| |5| Return & Sign Up      |\n";
        cout << "----------------------------\n";
        cout << "Enter option (1-5) >> ";
        choice = getValidIntInput(1, 5);
        switch (choice) {
        case 1:  viewCustomerDetails(experts); break;
        case 2: viewOverallSchedules(experts, days, timeSlots); break;
        case 3: viewSalesReport(experts, services); break;
        case 4:
            if (processRefund(experts, days, timeSlots)) {
                cout << "Refund processed successfully.\n";
            }
            else {
                cout << "Unsuccessful Refund\n";
            };
            system("pause");
            break;
        case 5: cout << "Logging out...\n"; break;
        default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}

void viewCustomerDetails(const Expert experts[]) {
    Customer* customers = new Customer[50];  // Assuming a maximum of 50 customers
    int* bookings = new int[360];
    double* sales = new double[50];  // To store total sales for each customer customerCount = 0;
    int choice;
    int customerCount = 0;
    bool isSortByName = false;
    // Read all customers from the single file
    ifstream inCustomerFile("customers.txt");
    string line;
    while (getline(inCustomerFile, line) && customerCount < 50) {
        // Find the positions of commas in the line
        int pos1 = line.find(',');
        int pos2 = line.find(',', pos1 + 1);
        int pos3 = line.find(',', pos2 + 1);
        int pos4 = line.find(',', pos3 + 1);

        // Extract individual details using substr
        customers[customerCount].username = line.substr(0, pos1);
        customers[customerCount].password = line.substr(pos1 + 1, pos2 - pos1 - 1);
        customers[customerCount].name = line.substr(pos2 + 1, pos3 - pos2 - 1);
        customers[customerCount].contact = line.substr(pos3 + 1, pos4 - pos3 - 1);
        customers[customerCount].email = line.substr(pos4 + 1);

        sales[customerCount] = calculateTotalSales(customers[customerCount], experts);
        customerCount++;
    }
    inCustomerFile.close();

    do {
        system("cls");
        displayLogo();
        cout << "-----------------------------\n";
        cout << "|       Customer Details    |\n";
        cout << "-----------------------------\n";
        cout << "| |1| Sort By Name          |\n"
            << "| |2| Sort By Expenses      |\n"
            << "| |3| Return                |\n";
        cout << "-----------------------------\n";
        cout << "Enter option (1-3) >> ";
        choice = getValidIntInput(1, 3);

        if (choice == 1 || choice == 2) {
            isSortByName = (choice == 1) ? true : false;
            selectionSort(customers, sales, customerCount, isSortByName);
            displaySortedList(customers, sales, customerCount, isSortByName);
        }
        else {
            cout << "Returning to previous menu...\n";
        }
    } while (choice != 3);

    //Clean up heap - allocated memory
    delete[] customers;
    delete[] sales;
}

void displaySortedList(Customer arr[], double sales[], int n, bool sortedByName) {
    string username;
    do {
        system("cls");
        displayLogo();
        cout << "Customers sorted by " << (sortedByName ? "name" : "total sales") << ":\n\n";
        cout << "+" << string(60, '-') << "+" << endl;
        cout << "|" << setw(3) << "No." << " | "
            << setw(19) << "Name" << " | "
            << setw(14) << "Username" << " | "
            << setw(14) << "Total Expense" << " |" << endl;
        cout << "+" << string(60, '-') << "+" << endl;

        for (int i = 0; i < n; i++) {
            cout << "|" << setw(3) << i + 1 << " | "
                << setw(19) << arr[i].name << " | "
                << setw(14) << arr[i].username << " | "
                << setw(14) << fixed << setprecision(2) << showpoint << sales[i] << " |" << endl;

        }
        cout << "+" << string(60, '-') << "+" << endl;
        cout << "\nEnter a customer's username to view details (or 'q' to return): ";
        cin >> username;

        if (username != "q") {
            displayCustomerDetails(arr, sales, n, username);
        }
    } while (username != "q");
}

void displayCustomerDetails(Customer arr[], double sales[], int n, const string& username) {
    bool customerFound = false;
    cout << endl;
    for (int i = 0; i < n; i++) {
        if (arr[i].username == username) {
            cout << "Customer Details:\n";
            cout << "------------------------\n";
            cout << "Name: " << arr[i].name << endl;
            cout << "Username: " << arr[i].username << endl;
            cout << "Contact: " << arr[i].contact << endl;
            cout << "Email: " << arr[i].email << endl;
            cout << "Total Expenses: RM " << fixed << setprecision(2) << showpoint << sales[i] << endl;
            cout << "------------------------\n";
            customerFound = true;
            break;
        }
    }
    if (!customerFound) {
        cout << "Customer not found.\n";
    }
    system("pause");
}

void selectionSort(Customer arr[], double sales[], int n, bool sortByName) {
    int i, j, selectedIndex;
    for (i = 0; i < n - 1; i++) {
        selectedIndex = i;
        for (j = i + 1; j < n; j++) {
            if (sortByName) {
                if (arr[j].name < arr[selectedIndex].name)
                    selectedIndex = j;
            }
            else {
                if (sales[j] > sales[selectedIndex])
                    selectedIndex = j;
            }
        }
        if (selectedIndex != i) {
            // Swap customers
            Customer tempCustomer = arr[selectedIndex];
            arr[selectedIndex] = arr[i];
            arr[i] = tempCustomer;
            // Swap corresponding sales
            double tempSales = sales[selectedIndex];
            sales[selectedIndex] = sales[i];
            sales[i] = tempSales;
        }
    }
}

double calculateTotalSales(const Customer& customer, const Expert experts[]) {
    double totalSales = 0.0;
    for (int e = 0; e < 3; e++) {
        for (int w = 0; w < 4; w++) {
            for (int s = 0; s < 30; s++) {
                if (experts[e].slots[w][s].cusUsername == customer.username) {
                    totalSales += experts[e].slots[w][s].totalPrice;
                }
            }
        }
    }
    return totalSales;
}

void viewIndividualSchedule(Expert& expert, const string days[], const string timeSlots[]) {
    string slotID;

    do {
        system("cls");
        displayLogo();
        for (int i = 0; i < 4; i++) {
            displaySchedule(expert, i, days, timeSlots, true);  // true indicates expert view
            cout << endl;
        }
        cout << "\033[96mSlot ID \033[0m- Booked\n\033[95m#### \033[0m- Available\n\033[31m----  \033[0m- Not Available" << endl;
        cout << "(Press 0 To Return) ";
        cout << "Enter the Slot ID to view details: ";
        cin >> slotID;

        if (slotID != "0") {
            bool slotFound = false;
            for (int z = 0; z < 4 && !slotFound; z++) {
                for (int j = 0; j < 30 && !slotFound; j++) {
                    if (expert.slots[z][j].slotID == slotID) {
                        slotFound = true;
                        if (expert.slots[z][j].status == TREATMENT || expert.slots[z][j].status == CONSULTATION) {
                            system("cls");
                            displayLogo();
                            displayBookingDetails(slotID, expert.name, expert.slots[z][j].service.name, expert.slots[z][j].status, z, days[expert.slots[z][j].day - 1], expert.slots[z][j].date, calculateTimeRange(j, expert.slots[z][j].status));
                            cout << "Customer: " << expert.slots[z][j].cusUsername << endl;
                        }
                        else {
                            cout << "This slot is not booked." << endl;
                        }
                    }
                }
            }
            if (!slotFound) {
                cout << "Invalid Slot ID." << endl;
            }
            system("pause");
        }
    } while (slotID != "0");
}

void viewSalesReport(const Expert experts[], const ServiceType services[]) {
    system("cls");
    displayLogo();

    int salesCount = 0;
    double  totalprice = 0.0;
    int totalBookings = 0;
    double totalRevenue = 0.0;
    double treatmentRevenue = 0.0;
    double consultationRevenue = 0.0;
    double expertRevenue[3] = { 0.0 };  // Assuming 3 experts
    double serviceRevenue[3] = { 0.0 }; // Assuming 3 services
    SaleRecord salesRecords[500];


    // Calculate the renevue
    for (int e = 0; e < 3; e++) {
        for (int w = 0; w < 4; w++) {
            for (int s = 0; s < 30; s++) {
                if (experts[e].slots[w][s].status == TREATMENT || experts[e].slots[w][s].status == CONSULTATION) {// Using linear search
                    totalBookings++;
                    totalprice = experts[e].slots[w][s].totalPrice;
                    totalRevenue += totalprice;
                    expertRevenue[e] += totalprice;

                    if (experts[e].slots[w][s].status == TREATMENT) {
                        treatmentRevenue += totalprice;
                    }
                    else {
                        consultationRevenue += totalprice;
                    }
                    int serviceIndex = experts[e].slots[w][s].service.serviceID;
                    if (serviceIndex >= 0 && serviceIndex < 3) {
                        serviceRevenue[serviceIndex] += totalprice;
                    }

                    // Collect sale record
                    salesRecords[salesCount].slotID = experts[e].slots[w][s].slotID;
                    salesRecords[salesCount].customerUsername = experts[e].slots[w][s].cusUsername;
                    salesRecords[salesCount].date = to_string(experts[e].slots[w][s].date); // You need to implement a way to get the date
                    salesRecords[salesCount].bookingType = (experts[e].slots[w][s].status == TREATMENT) ? "Treatment" : "Consultation";
                    salesRecords[salesCount].service = services[serviceIndex].name;
                    salesRecords[salesCount].expert = experts[e].name;
                    salesRecords[salesCount].price = totalprice / 1.05;
                    salesRecords[salesCount].tax = totalprice - salesRecords[salesCount].price;
                    salesRecords[salesCount].totalAmount = totalprice;
                    salesCount++;

                    if (salesCount >= 500) {
                        cout << "Warning: Maximum number of sales reached. Some sales may not be included in the report." << endl;
                        break;
                    }
                }
            }if (salesCount >= 500) { break; }
        } if (salesCount >= 500) { break; }
    }

    // Display Sales Report
    cout << "+" << string(135, '-') << "+\n";
    cout << "|" << setw(78) << right << "Beauty Service Sales Report" << setw(59) << "|\n";
    cout << "+" << string(135, '-') << "+\n";
    cout << "| " << setw(5) << left << "Slot" << " | "
        << setw(15) << left << "Customer" << " | "
        << setw(10) << left << "Date" << " | "
        << setw(14) << left << "Booking Type" << " | "
        << setw(20) << left << "Service" << " | "
        << setw(15) << left << "Expert" << " | "
        << setw(10) << right << "Price" << " | "
        << setw(8) << right << "Tax" << " | "
        << setw(12) << right << "Total Amount" << " |\n";
    cout << "+" << string(135, '-') << "+\n";

    for (int i = 0; i < salesCount; i++) {
        cout << "| " << setw(5) << left << salesRecords[i].slotID << " | "
            << setw(15) << left << salesRecords[i].customerUsername << " | "
            << setw(10) << left << (salesRecords[i].date + "/07/2024") << " | "
            << setw(14) << left << salesRecords[i].bookingType << " | "
            << setw(20) << left << salesRecords[i].service << " | "
            << setw(15) << left << salesRecords[i].expert << " | "
            << setw(10) << right << fixed << setprecision(2) << salesRecords[i].price << " | "
            << setw(8) << right << fixed << setprecision(2) << salesRecords[i].tax << " | "
            << setw(12) << right << fixed << setprecision(2) << salesRecords[i].totalAmount << " |\n";
    }

    cout << "+" << string(135, '-') << "+\n";
    cout << "| " << setw(121) << right << "Total Sales:"
        << setw(12) << right << fixed << setprecision(2) << totalRevenue << " |\n";
    cout << "+" << string(135, '-') << "+\n\n";

    // Ask user if they want to print the summary sales report
    char choice;
    cout << "Do you want to view and generate the summary sales report? (Y -> Yes/ others -> Quit): ";
    cin >> choice;

    if (toupper(choice) == 'Y') {
        system("cls");
        displayLogo();
        // Title box
        cout << "+" << string(58, '-') << "+\n";
        cout << "|" << setw(44) << right << "Summary Beauty Service Sales Report" << setw(16) << "|\n";
        cout << "+" << string(58, '-') << "+\n";

        // Total Summary
        cout << "| Total Summary" << setw(46) << right << "|\n";
        cout << "| Total Bookings: " << setw(41) << left << totalBookings << "|\n";
        cout << "| Total Revenue:  RM " << setw(38) << left << fixed << setprecision(2) << totalRevenue << "|\n";

        // Revenue by Booking box
        cout << "+" << string(58, '-') << "+\n";
        cout << "| Revenue by Booking" << setw(41) << right << "|\n";
        cout << "| Treatment:     RM " << setw(39) << left << fixed << setprecision(2) << treatmentRevenue << "|\n";
        cout << "| Consultation:  RM " << setw(39) << left << fixed << setprecision(2) << consultationRevenue << "|\n";

        // Revenue by Expert box
        cout << "+" << string(58, '-') << "+\n";
        cout << "| Revenue by Expert" << setw(42) << right << "|\n";
        cout << "| " << setw(15) << left << experts[0].name << ": RM " << setw(37) << left << fixed << setprecision(2) << expertRevenue[0] << "|\n";
        cout << "| " << setw(15) << left << experts[1].name << ": RM " << setw(37) << left << fixed << setprecision(2) << expertRevenue[1] << "|\n";
        cout << "| " << setw(15) << left << experts[2].name << ": RM " << setw(37) << left << fixed << setprecision(2) << expertRevenue[2] << "|\n";

        // Revenue by Service box
        cout << "+" << string(58, '-') << "+\n";
        cout << "| Revenue by Service" << setw(41) << right << "|\n";
        cout << "| " << setw(22) << left << services[0].name << ": RM " << setw(30) << left << fixed << setprecision(2) << serviceRevenue[0] << "|\n";
        cout << "| " << setw(22) << left << services[1].name << ": RM " << setw(30) << left << fixed << setprecision(2) << serviceRevenue[1] << "|\n";
        cout << "| " << setw(22) << left << services[2].name << ": RM " << setw(30) << left << fixed << setprecision(2) << serviceRevenue[2] << "|\n";
        cout << "+" << string(58, '-') << "+\n";

        string filename = "Sales_Report.txt";
        ofstream outSalesFile(filename);
        // Title box
        outSalesFile << "+" << string(58, '-') << "+\n";
        outSalesFile << "|" << setw(44) << right << "Summary Beauty Service Sales Report" << setw(16) << "|\n";
        outSalesFile << "+" << string(58, '-') << "+\n";

        // Total Summary
        outSalesFile << "| Total Summary" << setw(46) << right << "|\n";
        outSalesFile << "| Total Bookings: " << setw(41) << left << totalBookings << "|\n";
        outSalesFile << "| Total Revenue:  RM " << setw(38) << left << fixed << setprecision(2) << totalRevenue << "|\n";

        // Revenue by Booking box
        outSalesFile << "+" << string(58, '-') << "+\n";
        outSalesFile << "| Revenue by Booking" << setw(41) << right << "|\n";
        outSalesFile << "| Treatment:     RM " << setw(39) << left << fixed << setprecision(2) << treatmentRevenue << "|\n";
        outSalesFile << "| Consultation:  RM " << setw(39) << left << fixed << setprecision(2) << consultationRevenue << "|\n";

        // Revenue by Expert box
        outSalesFile << "+" << string(58, '-') << "+\n";
        outSalesFile << "| Revenue by Expert" << setw(42) << right << "|\n";
        outSalesFile << "| " << setw(15) << left << experts[0].name << ": RM " << setw(37) << left << fixed << setprecision(2) << expertRevenue[0] << "|\n";
        outSalesFile << "| " << setw(15) << left << experts[1].name << ": RM " << setw(37) << left << fixed << setprecision(2) << expertRevenue[1] << "|\n";
        outSalesFile << "| " << setw(15) << left << experts[2].name << ": RM " << setw(37) << left << fixed << setprecision(2) << expertRevenue[2] << "|\n";

        // Revenue by Service box
        outSalesFile << "+" << string(58, '-') << "+\n";
        outSalesFile << "| Revenue by Service" << setw(41) << right << "|\n";
        outSalesFile << "| " << setw(22) << left << services[0].name << ": RM " << setw(30) << left << fixed << setprecision(2) << serviceRevenue[0] << "|\n";
        outSalesFile << "| " << setw(22) << left << services[1].name << ": RM " << setw(30) << left << fixed << setprecision(2) << serviceRevenue[1] << "|\n";
        outSalesFile << "| " << setw(22) << left << services[2].name << ": RM " << setw(30) << left << fixed << setprecision(2) << serviceRevenue[2] << "|\n";
        outSalesFile << "+" << string(58, '-') << "+\n";

        outSalesFile.close();

#ifdef _WIN32
        string command = "notepad " + filename;
#else
        string command = "open " + filename;
#endif
        system(command.c_str()); // need to close only can continue
        cout << "Sales Report generated and saved as " << filename << endl;
    }
    system("pause");
}

bool handleAdminSelection(Expert experts[], const string days[], const string timeSlots[]) {
    int expertOption = getValidIntInput(1, 4);
    if (expertOption >= 1 && expertOption <= 3) {
        viewIndividualSchedule(experts[expertOption - 1], days, timeSlots);
        return true;
    }
    return expertOption != 4;
}

void viewOverallSchedules(Expert experts[], const string days[], const string timeSlots[]) {
    bool continueViewing;
    do {
        system("cls");
        displayLogo();
        cout << "Overall Schedule\n";
        cout << "Select the expert to view the schedule:\n";
        displayExpertList();
        continueViewing = handleAdminSelection(experts, days, timeSlots);
    } while (continueViewing);
}


bool processRefund(Expert experts[], const string days[], const string timeSlots[]) {
    string slotID;
    bool slotFound = false;
    int expertIndex, weekIndex, slotIndex;

    system("cls");
    displayLogo();
    cout << "Process Refund\n";
    cout << "Enter the Slot ID to refund (or '0' to cancel): ";
    cin >> slotID;

    if (slotID == "0") {
        cout << "Refund process cancelled.\n";
        return false;
    }

    // Find the slot
    for (expertIndex = 0; expertIndex < 3; expertIndex++) {
        for (weekIndex = 0; weekIndex < 4; weekIndex++) {
            for (slotIndex = 0; slotIndex < 30; slotIndex++) {
                if (experts[expertIndex].slots[weekIndex][slotIndex].slotID == slotID) {
                    slotFound = true;
                    break;
                }
            }
            if (slotFound) break;
        }
        if (slotFound) break;
    }

    if (!slotFound) {
        cout << "Invalid Slot ID. No booking found.\n";
        return false;
    }

    Booking& removeSlot = experts[expertIndex].slots[weekIndex][slotIndex];

    if (removeSlot.status != TREATMENT && removeSlot.status != CONSULTATION) {
        cout << "This slot is not booked.\n";
        return false;
    }

    // Display booking details
    displayBookingDetails(removeSlot.slotID, experts[expertIndex].name, removeSlot.service.name, removeSlot.status, weekIndex, days[removeSlot.day - 1], removeSlot.date, calculateTimeRange(slotIndex, removeSlot.status));
    cout << "Customer: " << removeSlot.cusUsername << endl;
    cout << "Refund Amount: RM " << fixed << setprecision(2) << showpoint << removeSlot.totalPrice << endl;

    // Confirm refund
    char confirm;
    cout << "\nAre you sure you want to process a refund for this booking? (Y -> Yes / Other -> No): ";
    cin >> confirm;

    if (toupper(confirm) != 'Y') {
        cout << "Refund cancelled.\n";
        return false;
    }

    // Process refund
    int hoursToRemove = (removeSlot.status == CONSULTATION) ? 1 : 2;
    experts[expertIndex].hoursworked[weekIndex][removeSlot.day - 1] -= hoursToRemove;

    // Reset slot status
    removeSlot.status = AVAILABLE;
    removeSlot.cusUsername = "";
    removeSlot.paymentUse = NONE;
    removeSlot.totalPrice = 0;

    //Update make the slot that not available back to available because of the maximum 6 hours
    updateBookingStatus(experts[expertIndex]);

    // Delete receipt file
    string receiptFilename = "Receipts\\receipt_" + removeSlot.slotID + ".txt";
    if (remove(receiptFilename.c_str()) != 0) {
        cout << "Error deleting receipt file.\n";
    }
    // Update expert file
    string expertFilename = "Expert\\" + experts[expertIndex].name + "\\week" + to_string(weekIndex + 1) + ".txt";
    ofstream outFile(expertFilename, ios::trunc);

    if (!outFile) {
        cout << "Error: Unable to update expert file.\n";
        return false;
    }

    for (int i = 0; i < 30; i++) {
        if (experts[expertIndex].slots[weekIndex][i].status != AVAILABLE && experts[expertIndex].slots[weekIndex][i].status != NOT_AVAILABLE) {
            outFile << experts[expertIndex].slots[weekIndex][i].slotID << " " << experts[expertIndex].slots[weekIndex][i].status << " " << experts[expertIndex].slots[weekIndex][i].day << " "
                << experts[expertIndex].slots[weekIndex][i].date << " " << experts[expertIndex].slots[weekIndex][i].timeSlot << " " << experts[expertIndex].slots[weekIndex][i].service.serviceID << " "
                << experts[expertIndex].slots[weekIndex][i].paymentUse << " " << experts[expertIndex].slots[weekIndex][i].totalPrice << " " << experts[expertIndex].slots[weekIndex][i].cusUsername << endl;
        }
    }

    outFile.close();

    return true;
}