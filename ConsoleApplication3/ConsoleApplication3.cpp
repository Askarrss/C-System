#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;

// Структура для рейсов
struct Flight {
    int id;                 // Уникальный номер рейса
    string destination;     // Место назначения
    int availableSeats;     // Количество доступных мест
    int totalSeats;         // Всего мест
};

// Структура для бронирований
struct Booking {
    int flightId;           // ID рейса
    string passengerName;   // Имя пассажира
    int seatsBooked;        // Забронированное количество мест
};

// Глобальные данные
vector<Flight> flights = {
    {1, "Almaty - Astana", 50, 50},
    {2, "Almaty - Shymkent", 30, 30},
    {3, "Almaty - Karaganda", 20, 20}
};
vector<Booking> bookings;  // Список бронирований

// Прототипы функций
void showMenu();
void displayFlights(const vector<Flight>& flights);
void bookTicket(vector<Flight>& flights);
void cancelBooking(vector<Flight>& flights);
void displayBookings(const vector<Booking>& bookings, const vector<Flight>& flights);
void saveFlights(const vector<Flight>& flights);
void loadFlights(vector<Flight>& flights);
void saveBookings(const vector<Booking>& bookings);
void loadBookings(vector<Booking>& bookings);

// Функция для отображения меню
void showMenu() {
    cout << "1. Prosmotr reysov\n";
    cout << "2. Bronirovanie biletov\n";
    cout << "3. Otmena bronirovaniya\n";
    cout << "4. Prosmotr zabronirovannykh reysov\n";
    cout << "5. Sohranit reysy i bronirovaniya v fail\n";
    cout << "6. Zagrusit reysy i bronirovaniya iz faila\n";
    cout << "7. Vykhod\n";
}

// Функция для отображения списка рейсов
void displayFlights(const vector<Flight>& flights) {
    cout << "Dostupnye reysy:\n";
    for (const auto& flight : flights) {
        cout << "ID: " << flight.id
            << ", Marshrut: " << flight.destination
            << ", Svobodnye mesta: " << flight.availableSeats << "\n";
    }
}

// Функция для бронирования билетов
void bookTicket(vector<Flight>& flights) {
    int flightId, seats;
    string passengerName;
    cout << "Vvedite ID reysa: ";
    cin >> flightId;
    cout << "Vvedite kolichestvo mest: ";
    cin >> seats;
    cout << "Vvedite imya passazhira: ";
    cin.ignore(); // Очищаем буфер после ввода числа
    getline(cin, passengerName);

    for (auto& flight : flights) {
        if (flight.id == flightId) { // Поиск рейса по ID
            if (flight.availableSeats >= seats) { // Проверка доступных мест
                flight.availableSeats -= seats;

                // Добавление информации о бронировании
                bookings.push_back({ flightId, passengerName, seats });

                cout << "Bronirovanie uspeshno! Zabronirovano " << seats << " mest dlya passazhira " << passengerName << ".\n";
            }
            else {
                cout << "Nedostatochno svobodnykh mest.\n";
            }
            return;
        }
    }
    cout << "Reys s ukazannym ID ne nayden.\n";
}

// Функция для отмены бронирования
void cancelBooking(vector<Flight>& flights) {
    int flightId, seats;
    cout << "Vvedite ID reysa: ";
    cin >> flightId;
    cout << "Vvedite kolichestvo mest dlya otmeny: ";
    cin >> seats;

    for (auto& flight : flights) {
        if (flight.id == flightId) {
            if (flight.availableSeats + seats <= flight.totalSeats) {
                flight.availableSeats += seats;
                cout << "Otmena bronirovaniya uspeshna! Osvobozhdeno " << seats << " mest.\n";
            }
            else {
                cout << "Vy ukazali bolshe mest, chem bylo zabronirovano.\n";
            }
            return;
        }
    }
    cout << "Reys s ukazannym ID ne nayden.\n";
}

// Функция для отображения списка бронирований
void displayBookings(const vector<Booking>& bookings, const vector<Flight>& flights) {
    if (bookings.empty()) {
        cout << "Zabronirovannykh reysov net.\n";
        return;
    }

    cout << "Informatsiya o zabronirovannykh reysakh:\n";
    for (const auto& booking : bookings) {
        // Поиск рейса по ID, чтобы отобразить маршрут
        string destination = "Neizvestnoe napravlenie";
        for (const auto& flight : flights) {
            if (flight.id == booking.flightId) {
                destination = flight.destination;
                break;
            }
        }

        cout << "ID reysa: " << booking.flightId
            << ", Marshrut: " << destination
            << ", Imya passazhira: " << booking.passengerName
            << ", Zabronirovanno mest: " << booking.seatsBooked << "\n";
    }
}

// Функция для сохранения рейсов в файл
void saveFlights(const vector<Flight>& flights) {
    ofstream outFile("flights.txt");
    for (const auto& flight : flights) {
        outFile << flight.id << " "
            << flight.destination << " "
            << flight.availableSeats << " "
            << flight.totalSeats << "\n";
    }
    cout << "Reysy sohraneny v fail flights.txt\n";
}

// Функция для загрузки рейсов из файла
void loadFlights(vector<Flight>& flights) {
    ifstream inFile("flights.txt");
    Flight flight;
    flights.clear();
    while (inFile >> flight.id >> flight.destination >> flight.availableSeats >> flight.totalSeats) {
        flights.push_back(flight);
    }
    cout << "Reysy zagruseny iz faila flights.txt\n";
}

// Функция для сохранения бронирований в файл
void saveBookings(const vector<Booking>& bookings) {
    ofstream outFile("bookings.txt");
    for (const auto& booking : bookings) {
        outFile << booking.flightId << " "
            << booking.passengerName << " "
            << booking.seatsBooked << "\n";
    }
    cout << "Bronirovaniya sohraneny v fail bookings.txt\n";
}

// Функция для загрузки бронирований из файла
void loadBookings(vector<Booking>& bookings) {
    ifstream inFile("bookings.txt");
    Booking booking;
    bookings.clear();
    while (inFile >> booking.flightId) {
        inFile.ignore(); // Игнорируем пробел перед именем
        getline(inFile, booking.passengerName, ' ');
        inFile >> booking.seatsBooked;
        bookings.push_back(booking);
    }
    cout << "Bronirovaniya zagruseny iz faila bookings.txt\n";
}

// Главная функция программы
int main() {
    SetConsoleOutputCP(65001); // Установка кодировки UTF-8
    SetConsoleCP(65001);

    int choice;
    do {
        showMenu();
        cout << "Vyberite deystvie: ";
        cin >> choice;

        switch (choice) {
        case 1:
            displayFlights(flights);
            break;
        case 2:
            bookTicket(flights);
            break;
        case 3:
            cancelBooking(flights);
            break;
        case 4:
            displayBookings(bookings, flights);
            break;
        case 5:
            saveFlights(flights);
            saveBookings(bookings); // Сохраняем бронирования
            break;
        case 6:
            loadFlights(flights);
            loadBookings(bookings); // Загружаем бронирования
            break;
        case 7:
            cout << "Vykhod iz programmy.\n";
            break;
        default:
            cout << "Nevernyy vybor. Poprobuyte snova.\n";
        }
    } while (choice != 7);

    return 0;
}
