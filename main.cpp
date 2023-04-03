#include <iostream>
#include "Utility.h"
#include "mains.h"

int main()
{
    using std::cout, std::cin, std::endl;

    int choice = 0;
    while (choice != 6)
    {
        cout << "1. Run P1 version of Airport simulator\n";
        cout << "2. Run P2 version of Airport simulator\n";
        cout << "3. Run P3 version of Airport simulator\n";
        cout << "4. Run P4 version of Airport simulator\n";
        cout << "5. Run P5 version of Airport simulator\n";
        cout << "6. Quit the program\n";
        cout << "\nYour choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            main_1();
            break;
        case 2:
            main_2();
            break;
        case 3:
            main_3();
            break;
        case 4:
            main_4();
            break;
        case 5:
            main_5();
        }
    }

    return 0;
}