#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "functions/F1.h"
#include "functions/F2.h"
#include "functions/F3.h"
#include "functions/F4.h"
#include "functions/misc.h"

using namespace std;

// Save report function that ask the user to save the repoort as plain text or html
// Effective lines: 18
void saveReport(int index, string method, string result, vecpair titles, Logger &logger, File &f, int index2 = -1)
{
    string s = Save_Report_Menu();
    string d;
    if (s == "1")
    {
        // plain text
        logger.log("User save plain text report");
        if (index2 != -1)
        {
            d = Generate_Plain_Text_Report(method, result, titles[index].first, titles[index2].first);
        }
        else
        {
            d = Generate_Plain_Text_Report(method, result, titles[index].first);
        }
        f.saveTxtReport(d);
    }
    else if (s == "2")
    {
        // html
        logger.log("User save html report");
        if (index2 != -1)
        {
            d = Generate_HTML_Report(method, result, titles[index].first, titles[index2].first);
        }
        else
        {
            d = Generate_HTML_Report(method, result, titles[index].first);
        }
        f.saveHtmlReport(d);
    }
    else if (s == "3")
    {
        logger.log("User go back to the ");
    }
}

// Statistical Calculation, Select 1 = Select only for Single Column
// Effective lines: 11
void select_1(File &f, Logger &logger, string method, char choice, vecpair titles, vector<int> computable, vector2d arr2d, int row)
{
    bool flag = true;
    bool flag2 = false;
    while (flag)
    {
        double result = singleColumnCompute(method, choice, titles, computable, arr2d, row, flag2);
        cin.ignore();
        pressEnter();
        if (flag2)
        {
            int choicetoint = choice - '0';
            int index = choicetoint - 1;
            saveReport(index, method, to_string(result), titles, logger, f);
            flag = false;
        }
    }
}

// Statistical Calculation, Select 2 = Select only for All Column
// Effective lines: 13
void select_2(File &f, Logger &logger, string method, vecpair titles, vector<int> computable, char choice, vector2d arr2d, int row)
{
    string result = allColumnCompute(method, choice, titles, computable, arr2d, row);
    cin.ignore();
    pressEnter();
    string s = Save_Report_Menu();
    string d;

    if (s == "1")
    {
        // plain text
        logger.log("User save plain text report");
        d = Generate_Plain_Text_Report(method, result);
        f.saveTxtReport(d);
    }
    else if (s == "2")
    {
        // html
        logger.log("User save html report");
        d = Generate_HTML_Report(method, result);
        f.saveHtmlReport(d);
    }
}


// Function that contain F3.1 - F3.6
// Effective lines: 24
bool statistical_calculation(File &f, Logger &logger, string method, char choice)
{
    bool error = f.loadScreen();
    if (error)
    {
        logger.log("User go back to statistical calculation menu");
        return false;
    }

    int col = f.getCol();
    int row = f.getRow();
    bool breakLoop = false;
    vecpair titles = f.getTitle();
    vector<int> computable = f.getCompute();
    vector2d arr2d = f.getData();

    Menu_Dashboard();
    showComputableTitles(titles, computable);

    int select_col = select_column();

    while (select_col != '1' && select_col != '2') // bool remain true until user entered valid selection
    {
        cout << "Invalid Input" << endl;
        select_col = select_column();
    }

    if (select_col == '1')
    {
        // Find for single column
        logger.log("User chooses to find single column");
        select_1(f, logger, method, choice, titles, computable, arr2d, row);
    }
    else if (select_col == '2')
    {
        logger.log("User chooses to find all column");
        select_2(f, logger, method, titles, computable, choice, arr2d, row);
        breakLoop = true;
    }
    return breakLoop;
}

int main()
{
    //// Welcome Dashboard ////
    bool found = false;   // if Account is found
    bool isAdmin = false; // if Found Account is Admin
    char choice;          // User's Choice
    char flag = true;     // Flag if True or False
    string name;          // User's Name

    Welcome_Screen();                  // Show Welcome Screen
    Login_Choice(flag, isAdmin, name); // Show Login Selection

    while (flag) // Flag remain true until user entered valid selection
    {
        Welcome_Screen();
        cout << "Invalid Choice!" << endl
             << endl;
        Login_Choice(flag, isAdmin, name);
    }

    // Determine if user is admin or user and log into respective menu
    if (isAdmin)
    {
        while (true)
        {

            Admin_Menu(name);
            // Admin_Menu_Section will return a string which will be assign to choice
            choice = Admin_Menu_Selection(name);
            while (choice != '1' && choice != '2' && choice != '3') // Bool remain true until user entered valid selection
            {
                Admin_Menu(name);
                cout << "Invalid Input" << endl;
                choice = Admin_Menu_Selection(name);
            }

            if (choice == '1')
            {
                UserRegister();
            }
            else if (choice == '2')
            {
                while (true)
                {
                    Modify_User_Menu();
                    choice = Modify_User_Selection();
                    while (choice != '1' && choice != '2' && choice != '3')
                    {
                        Modify_User_Menu();
                        cout << "Invalid Choice" << endl;
                        choice = Modify_User_Selection();
                    }
                    if (choice == '1')
                    {
                        AdminChangePassword();
                    }
                    else if (choice == '2')
                    {
                        UserDelete();
                    }
                    else if (choice == '3')
                    {
                        break;
                    }
                }
            }
            else if (choice == '3')
            {
                cout << "Logging Out..." << endl;
                cin.ignore();
                pressEnter();
                main();
                break;
            }
        }
    }
    else
    {
        Logger logger(name);
        File f(name);
        logger.log("User login");
        while (true) // Remain in User Menu
        {
            User_Menu(name);
            choice = User_Menu_Selection(name);
            cin.ignore(50, '\n');

            while (choice != '1' && choice != '2' && choice != '3' && choice != '4' && choice != '5' && choice != '6') // Bool remain true until user entered valid selection
            {
                User_Menu(name);
                cout << "Invalid Input" << endl;
                logger.log("User Enter Invalid Input");
                choice = User_Menu_Selection(name);
            }
           
            if (choice == '1') // Statistical Analysis Menu
            {
                logger.log("User enter statistical analysis menu");
                while (true) // Remain in Statistical Analysis Menu
                {
                    // Show Statistical Analysis Menu
                    Statistical_Analysis_Menu();
                    choice = Statistical_Analysis_Menu_Selection();

                    while (choice != '1' && choice != '2' && choice != '3' && choice != '4' && choice != '5') // bool remain true until user entered valid selection
                    {
                        Statistical_Analysis_Menu();
                        cout << "Invalid Input" << endl;
                        choice = Statistical_Analysis_Menu_Selection();
                    }
                    double result;
                    string method;

                    if (choice == '1')
                    {
                        logger.log("User enter statistical calculation menu");
                        while (true) // Remain in Statistical Calculation Menu
                        {

                            // Show Statistical Calculation Menu
                            Statistical_Calculation_Menu();
                            choice = Statistical_Calculation_Menu_Selection();

                            while (choice != '1' && choice != '2' && choice != '3' && choice != '4' && choice != '5' && choice != '6' && choice != '7') // bool remain true until user entered valid selection
                            {
                                Statistical_Calculation_Menu();
                                cout << "Invalid Input" << endl;
                                choice = Statistical_Calculation_Menu_Selection();
                            }
                            // Find Minimum
                            if (choice == '1')
                            {
                                logger.log("User chooses minimum");
                                method = "MINIMUM";

                                bool breakLoop = statistical_calculation(f, logger, method, choice);
                                if (breakLoop)
                                {
                                    break;
                                }
                            }

                            else if (choice == '2')
                            {
                                // Find Maximum
                                logger.log("User chooses maximum");
                                method = "MAXIMUM";

                                bool breakLoop = statistical_calculation(f, logger, method, choice);
                                if (breakLoop)
                                {
                                    break;
                                }
                            }
                            else if (choice == '3')
                            {
                                logger.log("User chooses median");
                                // Find Median
                                method = "MEDIAN";

                                bool breakLoop = statistical_calculation(f, logger, method, choice);
                                if (breakLoop)
                                {
                                    break;
                                }
                            }
                            else if (choice == '4')
                            {
                                // Find Mean
                                logger.log("User chooses mean");
                                method = "MEAN";

                                bool breakLoop = statistical_calculation(f, logger, method, choice);
                                if (breakLoop)
                                {
                                    break;
                                }
                            }
                            else if (choice == '5')
                            {
                                logger.log("User chooses variance");
                                // Find Variance
                                method = "VARIANCE";
                                bool breakLoop = statistical_calculation(f, logger, method, choice);
                                if (breakLoop)
                                {
                                    break;
                                }
                            }
                            else if (choice == '6')
                            {
                                // Find Standard Deviation
                                logger.log("User chooses standard deviation");
                                method = "STANDARD DEVIATION";

                                bool breakLoop = statistical_calculation(f, logger, method, choice);
                                if (breakLoop)
                                {
                                    break;
                                }
                            }
                            else if (choice == '7')
                            {
                                //Go Back
                                logger.log("User go back to statistical analysis menu");
                                break;
                            }
                        }
                    }
                    else if (choice == '2')
                    {

                        logger.log("User chooses histogram");
                        bool error = f.loadScreen();
                        if (error)
                        {
                            logger.log("User go back to statistical analysis menu");
                            continue;
                        }
                        int col = f.getCol();
                        int row = f.getRow();

                        // ------------------------------------------------------------------------------

                        vecpair titles = f.getTitle();
                        vector<int> computable = f.getCompute();
                        vector2d arr2d = f.getData();
                        Menu_Dashboard();
                        showComputableTitles(titles, computable);
                        // Find STDV for single column
                        while (true)
                        {
                            cout << "Select title by index" << endl;
                            int choice;
                            cin >> choice;
                            int index = choice - 1;

                            if (computable[index] == 0)
                            {
                                cout << "Error: Title not computable" << endl;
                                pressEnter();
                            }
                            else if (choice < 0 || choice > computable.size())
                            {
                                cout << "Error: Invalid index" << endl;
                                pressEnter();
                            }
                            else
                            {
                                int min = find_min(arr2d, row, index);
                                int max = find_max(arr2d, row, index);
                                string method = "HISTOGRAM";
                                cout << method << " of "
                                     << titles[index].first << endl;

                                string graph = histogram(arr2d, min, max, row, index);
                                cin.ignore();
                                pressEnter();

                                saveReport(index, method, graph, titles, logger, f);

                                break;
                            }
                        }
                    }

                    else if (choice == '3')
                    {
                        logger.log("User chooses correlation");
                        //Correlation
                        bool error = f.loadScreen();
                        if (error)
                        {
                            logger.log("User go back to statistical analysis menu");
                            continue;
                        }
                        int col = f.getCol();
                        int row = f.getRow();

                        // ------------------------------------------------------------------------------

                        vecpair titles = f.getTitle();
                        vector<int> computable = f.getCompute();
                        vector2d arr2d = f.getData();
                        Menu_Dashboard();
                        showComputableTitles(titles, computable);

                        // Find STDV for single column
                        while (true)
                        {
                            cout << "Select first title by index" << endl;
                            int choice1, choice2;
                            cin >> choice1;
                            int index1 = choice1 - 1;

                            cout << "Select second title by index" << endl;
                            cin >> choice2;
                            int index2 = choice2 - 1;

                            if (computable[index1] == 0 || computable[index2] == 0)
                            {
                                cout << "Error: Title not computable" << endl;
                                pressEnter();
                            }
                            else if (choice1 < 0 || choice2 < 0 || choice1 > computable.size() || choice2 > computable.size())
                            {
                                cout << "Error: Invalid index" << endl;
                                pressEnter();
                            }
                            else
                            {
                                double result = correlation(arr2d, row, index1, index2);
                                method = "CORRELATION";
                                cout << "Correlation of "
                                     << titles[index1].first << " & " << titles[index2].first << ": "
                                     << result << endl;

                                /// ---------------------------------------------------------
                                cin.ignore();
                                pressEnter();

                                saveReport(index1, method, to_string(result), titles, logger, f, index2);
                                break;
                            }
                        }
                    }
                    else if (choice == '4')
                    {
                        logger.log("User chooses distinct data members");
                        bool error = f.loadScreen();
                        if (error)
                        {
                            logger.log("User go back to statistical analysis menu");
                            continue;
                        }
                        int col = f.getCol();
                        int row = f.getRow();

                        // ------------------------------------------------------------------------------

                        vecpair titles = f.getTitle();
                        vector<int> computable = f.getCompute();
                        vector2d arr2d = f.getData();
                        Menu_Dashboard();
                        showComputableTitles(titles, computable);

                        // Find STDV for single column
                        while (true)
                        {
                            cout << "Select title by index" << endl;
                            int choice;
                            cin >> choice;
                            int index = choice - 1;

                            if (computable[index] == 0)
                            {
                                cout << "Error: Title not computable" << endl;
                                pressEnter();
                            }
                            else if (choice < 0 || choice > computable.size())
                            {
                                cout << "Error: Invalid index" << endl;
                                pressEnter();
                            }
                            else
                            {
                                string method = "DISTINCT DATA MEMBERS";
                                vector<pair<int, int>> distinct = distinctMember(arr2d, index);

                                cout << method << " of "
                                     << titles[index].first << endl;
                                string form = tabularForm(distinct);

                                cin.ignore();
                                pressEnter();

                                saveReport(index, method, form, titles, logger, f);
                                break;
                            }
                        }
                    }
                    else if (choice == '5')
                    {
                        //Go Back
                        logger.log("User go back to main menu");
                        break;
                        // continue;
                    }
                }
            }
            else if (choice == '2')
            {
                logger.log("User go to view save report menu");
                // View Saved Report
                char input = Report_Menu();
                if (input == '1')
                {
                    logger.log("User view plain text report");
                    f.txtReportScreen();
                }
                else if (input == '2')
                {
                    logger.log("User view html report");
                    f.htmlReportScreen();
                }
                else
                {
                    logger.log("User go back to main menu");
                    continue;
                }
            }
            else if (choice == '3')
            {
                logger.log("User view user log");
                logger.showLogScreen();
            }
            else if (choice == '4')
            {
                // Change Password
                logger.log("User change password");
                Menu_Dashboard();
                UserChangePassword(name);
            }
            else if (choice == '5')
            {
                logger.log("User save the data file as another name");
                bool error = f.loadScreen();
                if (error)
                {
                    logger.log("User go back to main menu");
                    continue;
                }
                f.savedAs();
            }
            else if (choice == '6')
            {
                // Log out
                logger.log("User logout");
                cout << "Logging Out..." << endl;
                pressEnter();
                main();
                break;
            }
        }
    }

    return 0;
}
