#ifndef F4_H
#define F4_H
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "misc.h"

using namespace std;
string Save_Report_Action();
void Welcome_Screen()
{
    clearScreen();
    cout << "+--------------------------------------------------+" << endl;
    cout << "|                    Welcome to                    |" << endl;
    cout << "|            Basic Data Analysis Program           |" << endl;
    cout << "|                    (  BDAP  )                    |" << endl;
    cout << "+--------------------------------------------------+" << endl;
    cout << endl;
}

int CenterText(string name)
{
    int length = name.size();
    int width = 0;
    int pos = (52 / 2) + (length / 2);
    for (int i = 0; i < pos; i++)
    {
        width += 1;
    }

    return width;
}

void Menu_Dashboard()
{
    clearScreen();
    cout << "+--------------------------------------------------+" << endl;
    cout << "|            Basic Data Analysis Program           |" << endl;
    cout << "|                    (  BDAP  )                    |" << endl;
    cout << "+--------------------------------------------------+" << endl;
    cout << endl;
}

char Login_Dashboard()
{
    clearScreen();
    cout << "+--------------------------------------------------+" << endl;
    cout << "|            Basic Data Analysis Program           |" << endl;
    cout << "|                    (  BDAP  )                    |" << endl;
    cout << "+--------------------------------------------------+" << endl;
    cout << endl;
    cout << "Please Choose " << endl
         << endl;
    cout << "1. Login" << endl;
    cout << "2. Quit" << endl
         << endl;
    cout << "Please Enter your choice: ";
    char choice;
    cin >> choice;
    return choice;
}

void Statistical_Analysis_Menu()
{
    Menu_Dashboard();
    cout << "1.   Statistical Calculation" << endl;
    cout << "2.   Plot Histogram" << endl;
    cout << "3.   Correlation" << endl;
    cout << "4.   Distinct data members" << endl;
    cout << "5.   Go Back" << endl;
}

char Statistical_Analysis_Menu_Selection()
{
    char user_choice;
    cout << "Please Enter Your choice: ";
    cin >> user_choice;
    cin.ignore(30, '\n');
    return user_choice;
}

void Statistical_Calculation_Menu()
{
    Menu_Dashboard();
    cout << "1.   Minimum" << endl;
    cout << "2.   Maximum" << endl;
    cout << "3.   Median" << endl;
    cout << "4.   Mean" << endl;
    cout << "5.   Variance" << endl;
    cout << "6.   Standard Deviation" << endl;
    cout << "7.   Go Back" << endl;
}

char Statistical_Calculation_Menu_Selection()
{
    char user_choice;
    cout << "Please Enter Your choice: ";
    cin >> user_choice;
    cin.ignore(30, '\n');
    return user_choice;
}

void User_Menu(string name)
{
    Menu_Dashboard();
    int width = CenterText(name);
    cout << setw(34) << "Welcome User, " << endl;
    cout << right << setw(width) << fixed << name << endl
         << endl;
    cout << "1.   Enter Statistical Analysis Menu" << endl;
    cout << "2.   View Saved Report" << endl;
    cout << "3.   View User Log" << endl;
    cout << "4.   Change Password" << endl;
    cout << "5.   Log out" << endl;
}

void Admin_Menu(string name)
{
    Menu_Dashboard();

    int width = CenterText(name);
    cout << "Width: " << width << endl;
    cout << setw(35) << "Welcome Admin, " << endl;
    cout << right << setw(width) << fixed << name << endl
         << endl;
    cout << "1.   Create User Account" << endl;
    cout << "2.   Modify User Account" << endl;
    cout << "3.   Log out" << endl;
}

string Save_Report_Menu()
{
    //Menu_Dashboard();
    string user_choice;
    cout << "Do you want to save as report" << endl;
    cout << "Type 'y' save the report or 'n' to go back" << endl;

    while (true)
    {
        cout << "Please Enter Your Choice: ";
        getline(cin, user_choice);
        // cin.ignore(30, '\n');
        // cin.get(user_choice);

        if (user_choice == "y")
        {
            user_choice = Save_Report_Action();

            // if (user_choice == '3') {

            // }
            return user_choice;
            break;
        }
        else if (user_choice == "n")
        {
            return user_choice;
            break;
        }
        else
        {
            cout << "Invalid Input" << endl;
            // cin.ignore(50, '\n');
            pressEnter();
        }
    }
}

string Save_Report_Action()
{
    string user_choice;
    Menu_Dashboard();

    cout << "1. Generate Plain Text Report" << endl;
    cout << "2. Generate HTML Report" << endl;
    cout << "3. Go Back" << endl;

    while (true)
    {
        cout << "Please Enter Your Choice: ";
        getline(cin, user_choice);

        if (user_choice == "1")
        {
            return user_choice;
            break;
        }
        else if (user_choice == "2")
        {
            return user_choice;
            break;
        }
        else if (user_choice == "3")
        {
            Save_Report_Menu();
            break;
        }
        else
        {
            cout << "Invalid input" << endl;
            pressEnter();
        }
    }
}

string reportMethod(string method)
{
    stringstream ss;
    ss << "+";
    for (int i = 0; i < (method.length() + 4); i++)
    {
        ss << "-";
    }
    ss << "+" << endl;
    ss << "|  " + method + "  |" << endl;
    ss << "+";
    for (int i = 0; i < (method.length() + 4); i++)
    {
        ss << "-";
    }
    ss << "+";

    return ss.str();
}

string Generate_HTML_Report(string method, string data, string title, string title2 = "")
{
    stringstream htmlData;
    string m = reportMethod(method);

    htmlData << "<html>" << endl;
    htmlData << "<body>" << endl;
    htmlData << "<h1>"
             << "<pre>" << m << "</pre>"
             << "</h1>" << endl;
    htmlData << "<h2>"<< "Title: " << title ;
    if (title2 != "")
    {
        htmlData << " & "<< title2;
    }
    htmlData << "</h2>" << endl;
    htmlData << "<pre>" << data << "</pre>" << endl;
    htmlData << "</body>" << endl;
    htmlData << "</html>" << endl;
    return htmlData.str();
}

string Generate_Plain_Text_Report(string method, string data, string title, string title2 = "")
{
    stringstream plainTextData;

    string m = reportMethod(method);
    plainTextData << m << endl;

    plainTextData << "Title: " << title;
    if (title2 != "")
    {
        plainTextData << " & " << title2;
    }
    plainTextData << endl;
    plainTextData << data;
    return plainTextData.str();
}

char User_Menu_Selection(string name)
{
    char user_choice;
    cout << "Please Enter Your choice: ";
    cin >> user_choice;

    if (user_choice == '1')
    {
        // Statistical Menu
        return '1';
    }

    else if (user_choice == '2')
    {
        // View Saved Report
        return '2';
    }

    else if (user_choice == '3')
    {
        // View User Log
        return '3';
    }

    else if (user_choice == '4')
    {
        // Change Password
        // UserChangePassword();
        return '4';
    }

    else if (user_choice == '5')
    {
        // Log out
        cin.ignore();
        return '5';
    }
}

char Admin_Menu_Selection(string name)
{
    char admin_choice;
    cout << "Please Enter Your choice: ";
    cin >> admin_choice;

    if (admin_choice == '1')
    {
        // Create User Account
        return '1';
    }

    else if (admin_choice == '2')
    {
        // Modify User Account
        return '2';
    }

    else if (admin_choice == '3')
    {
        // Log Out
        cin.ignore();
        return '3';
    }
}

void Modify_User_Menu()
{
    clearScreen();
    bool valid = true;
    char user_choice;
    Menu_Dashboard();
    cout << "1.   Change Password" << endl;
    cout << "2.   Delete User Account" << endl;
    cout << "3.   Back" << endl;
}

char Modify_User_Selection()
{
    char modify_choice;
    cout << "Please Enter Your choice: ";
    cin >> modify_choice;

    if (modify_choice == '1')
    {
        // Change User Password
        return '1';
    }

    else if (modify_choice == '2')
    {
        // Delete User Account
        return '2';
    }

    else if (modify_choice == '3')
    {
        // Back
        cin.ignore();
        return '3';
    }
}

char Report_Menu()
{
    bool valid;
    char user_choice;

    cout << "1.   View plain text report" << endl;
    cout << "2.   View HTML report" << endl;

    while (valid)
    {
        cout << "Please Enter Your choice: ";
        cin >> user_choice;
        cin.ignore(30, '\n');

        if (user_choice == '1')
        {
            return user_choice;
            // cout << "Plain Text Report" << endl;
        }

        else if (user_choice == '2')
        {
            // cout << "HTML Report" << endl;
            return user_choice;
        }

        else
        {
            cout << "Invalid Input" << endl;
            // return '0';
        }
    }
}

#endif