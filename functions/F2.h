/* 
    All functions in F2.h are implemented by 
    Student: LIM CHIH ZHEN (1201101263)
*/

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <ctime>
#include <utility>
#include <iomanip>
#include "misc.h"

#if defined(__linux__) || defined(__APPLE__)
#define slash "/"
#define mkdir "mkdir -p "
#else
#define slash "\\"
#define mkdir "mkdir "
#endif

using namespace std;

// Show header
// Effective lines: 7
void showHeader()
{
    clearScreen();
    cout << "+--------------------------------------------------+" << endl;
    cout << "|            Basic Data Analysis Program           |" << endl;
    cout << "|                    (  BDAP  )                    |" << endl;
    cout << "+--------------------------------------------------+" << endl;
    cout << endl
         << endl;
}

typedef vector<vector<int>> vector2d;
typedef vector<pair<string, int>> vecpair;

class File
{
private:
    int numCol = 0, numRow = 0;
    vector2d data;
    vecpair title;
    vector<int> compute;
    bool error;
    string dir = "Users_Folder", dataDir, reportDir, htmlDir;

    // ------------------------------------------------------------------------

    // Read number of columns from the data file
    // Effective lines: 9
    // In: Reference of ifstream file
    // Out: Number of column
    int readCol(ifstream &file)
    {
        int col;
        file >> col;
        if (!col)
        {
            cout << "Number of column is missing" << endl;

            error = true;
            pressEnter();
            return 0;
        }
        file.ignore(100, '\n');
        return col;
    }

    // Read titles from the data file
    // In: Reference of ifstream file
    // Out: Title and index in vector of pair
    // Effective lines: 23
    vecpair readTitle(ifstream &file)
    {
        vecpair titles;
        pair<string, int> temp;
        stringstream ss;
        string line;
        getline(file, line);
        ss << line;
        string col;
        int index = 1;
        while (getline(ss, col, ','))
        {
            temp = {col, index};
            titles.push_back(temp);
            index++;
        }

        if (titles.size() != numCol)
        {
            error = true;
            int difference = numCol - titles.size();
            if (difference > 0)
            {
                cout << difference << " title " << (difference > 1 ? "are" : "is") << " missing" << endl;
            }
            else
            {
                cout << -difference << " title " << (-difference > 1 ? "are" : "is") << " extra" << endl;
            }
            cout << "Please modify the data file" << endl;
            titles.clear();
            pressEnter();
        }
        return titles;
    }

    //Read computable from data file
    // In: Reference of ifstream file
    // Out: Computable of data in vector of int
    // Effective lines: 15
    vector<int> readCompute(ifstream &file)
    {
        vector<int> computes;
        stringstream ss;
        string line;
        getline(file, line);
        ss << line;
        string col;
        while (getline(ss, col, ','))
        {
            computes.push_back(stoi(col));
        }

        if (computes.size() != numCol)
        {
            error = true;
            int difference = numCol - computes.size();
            computes.clear();
            if (difference > 0)
            {
                cout << -difference << " computability " << (difference > 1 ? "are" : "is") << " extra" << endl;
            }
            else
            {
                cout << -difference << " computability " << (-difference > 1 ? "are" : "is") << " extra" << endl;
            }
            cout << "Please modify the data file" << endl;
            pressEnter();
        }
        return computes;
    }

    // Read number of row from data file
    // In: Reference of ifstream file
    // Out: Number of row
    // Effective lines: 9
    int readRow(ifstream &file)
    {
        int row;
        file >> row;
        if (!row)
        {
            error = true;
            cout << "Number of row is missing" << endl;
            pressEnter();
            return 0;
        }
        file.ignore(100, '\n');
        return row;
    }

    // Combine readcol, readTitle, readCompute & readRow
    // In: Reference of ifstream file
    // Out: -
    // Effective lines: 13
    void readInfo(ifstream &file)
    {
        string line;

        numCol = readCol(file);
        if (error)
            return;

        title = readTitle(file);
        if (error)
            return;

        compute = readCompute(file);
        if (error)
            return;

        numRow = readRow(file);
        if (error)
            return;
    }

    // Check and convert string to integer
    // In: String of number
    // Out: Integer of number
    // Effective lines: 11
    int getInt(const string &strNum)
    {
        int intNum;
        stringstream stream;
        for (int i = 0; i < strNum.size(); i++)
        {
            if (!(isdigit(strNum[i])))
            {
                return -1;
            }
        }
        stream << strNum;
        stream >> intNum;
        if (intNum < 0)
        {
            return -1;
        }
        return intNum;
    }

    // Read and check the line of the data from the data file
    // In: String of line & number of current row
    // Out: Data in vector of integer
    // Effective lines: 18
    vector<int> getLineData(const string &line, const int &currentRow)
    {
        vector<int> row;
        stringstream ss;
        string strCol;
        int intCol;
        ss << line;
        while (getline(ss, strCol, ','))
        {
            intCol = getInt(strCol);
            if (intCol != -1)
            {
                row.push_back(intCol);
            }
        }
        if (row.size() == numCol)
        {
            return row;
        }
        else
        {
            error = true;
            int difference = numCol - row.size();
            cout << difference << " data in row " << currentRow + 1 << (difference > 1 ? " are" : " is") << " missing or invalid" << endl;
            row.clear();
            pressEnter();
            return row;
        }
    }

    // Read the data from the data file
    // In: Reference of ifstream file
    // Out: -
    // Effective lines: 15
    void readData(ifstream &file)
    {
        string line;
        int countRow = 0;
        while (countRow < numRow)
        {
            getline(file, line);
            vector<int> d = getLineData(line, countRow);
            if (error)
            {
                pressEnter();
                return;
            }
            data.push_back(d);
            countRow++;
        }
        if (data.size() != numRow)
        {
            error = true;
            int difference = numRow - data.size();
            cout << difference << " row " << (difference > 1 ? "are" : "is") << " missing" << endl;
            pressEnter();
        }
    }

    // Combine the readInfo & readData
    // In: Path of the file
    // Out: -
    // Effective lines: 15
    void readFile(const string &path)
    {
        ifstream inFile;
        inFile.open(path);
        if (inFile)
        {
            readInfo(inFile);
            if (error)
            {
                inFile.close();
                return;
            }
            readData(inFile);
            if (error)
            {
                inFile.close();
                return;
            }
            inFile.close();
        }
        else
        {
            error = true;
            cout << "The file cannot open" << endl;
            pressEnter();
            return;
        }
    }

    // ------------------------------------------------------------------------

    // Check if the file exist or not
    // In: Path of the file
    // Out: Boolean of file exist
    // Effective lines: 4
    bool fileExist(const string &path)
    {
        ifstream file(path);
        bool exist = file ? true : false;
        file.close();
        return exist;
    }

    // ------------------------------------------------------------------------

    // Check if the file name accepted by system
    // In: Filename
    // Out: Boolean of proper filename
    // Effective lines: 8
    bool properName(const string &filename)
    {
        char character[] = {'\\', '/', ':', '*', '?', '<', '>', '|'};
        for (int i = 0; i < filename.size(); i++)
        {
            for (char c : character)
            {
                if (c == filename[i])
                {
                    cout << "The filename cannot contain '\\', '/', ':', '*', '?', '<', '>', '|' character." << endl;
                    pressEnter();
                    return false;
                }
            }
        }
        return true;
    }

    // A screen will show if the file exists
    // In: -
    // Out: Boolean of replace or set new name
    // Effective lines: 11
    bool fileExistScreen()
    {
        string res;
        do
        {
            showHeader();
            cout << "The filename already exist" << endl;
            cout << "Type 'y' to replace and 'n' to set a new name" << endl;
            getline(cin, res);
            if (res != "y" && res != "n")
            {
                cout << "Please enter a valid input" << endl;
                pressEnter();
            }

        } while (res != "y" && res != "n");

        return res == "y" ? true : false;
    }

    // Ask the filename from the user
    // In: Filename
    // Out: -
    // Effective lines: 13
    void askFilename(string &filename)
    {
        while (true)
        {
            showHeader();
            cout << "What is the name of new file (without the file extension -> .txt)" << endl;
            cin >> filename;
            cin.ignore(50, '\n');
            if (!properName(filename))
                continue;

            filename += ".txt";
            string path = dataDir + slash + filename;
            if (fileExist(path))
            {
                if (!fileExistScreen())
                {
                    continue;
                }
            }
            break;
        }
    }

    // Write one integer data such as number of column/row into the file
    // In: Reference of output file & integer of data (number of column & row)
    // Out: -
    // Effective lines: 1
    void writeLine(ofstream &file, const int &data)
    {
        file << data << endl;
    }

    // Write title into the file
    // In: Reference of output file & vector of pair of data (titles)
    // Out: -
    // Effective lines: 6
    void writeLine(ofstream &file, const vecpair &data)
    {
        for (int i = 0; i < numCol; i++)
        {
            if (i != numCol - 1)
            {
                file << data[i].first << ",";
            }
            else
            {
                file << data[i].first;
            }
        }
        file << endl;
    }

    // Write a vector of data such as computable into the file
    // In: Reference of output file & vector of integer of data (computable)
    // Out: -
    // Effective lines: 6
    void writeLine(ofstream &file, const vector<int> &data)
    {
        for (int i = 0; i < numCol; i++)
        {
            if (i != numCol - 1)
            {
                file << data[i] << ",";
            }
            else
            {
                file << data[i];
            }
        }
        file << endl;
    }

    // Write the data into the file
    // In: Reference of output file & vector of pair of data (data)
    // Out: -
    // Effective lines: 8
    void writeLine(ofstream &file, const vector2d &data)
    {
        for (int i = 0; i < numRow; i++)
        {
            for (int j = 0; j < numCol; j++)
            {
                if (j != numCol - 1)
                {
                    file << data[i][j] << ",";
                }
                else
                {
                    file << data[i][j];
                }
            }
            if (i < numRow - 1)
            {
                file << endl;
            }
        }
    }

    // ------------------------------------------------------------------------

    // Ask text report name from the user
    // In: Reference of filename
    // Out: -
    // Effective lines: 13
    void askTxtName(string &filename)
    {
        while (true)
        {
            showHeader();
            cout << "What is the name of txt report (without file extension -> .txt)" << endl;
            cin >> filename;
            cin.ignore(50, '\n');
            if (!properName(filename))
                continue;
            filename += ".txt";
            string path = reportDir + slash + filename;
            if (fileExist(path))
            {
                if (!fileExistScreen())
                {
                    continue;
                }
            }
            break;
        }
    }

    // ------------------------------------------------------------------------

    // Ask html report name from the user
    // In: Reference of filename
    // Out: -
    // Effective lines: 14
    void askHtmlName(string &filename)
    {
        while (true)
        {
            showHeader();
            cout << "What is the name of html report (without file extension -> .html)" << endl;
            cin >> filename;
            cin.ignore(50, '\n');
            if (!properName(filename))
            {
                cout << "The filename cannot contain '\\', '/', ':', '*', '?', '<', '>', '|' character.";
                continue;
            }
            filename += ".html";
            string path = htmlDir + slash + filename;
            if (fileExist(path))
            {
                if (!fileExistScreen())
                {
                    continue;
                }
            }
            break;
        }
    }
    // ------------------------------------------------------------------------

    // Show the content of text report
    // In: Path of the file
    // Out: -
    // Effective lines: 6
    void printTxtReport(const string &path)
    {
        ifstream file(path);
        string line;
        while (!file.eof())
        {
            getline(file, line);
            cout << line << endl;
        }
        file.close();
    }

    // ----------------------------------------------------------------------------------------------------------------------------------------------------

public:
    // Logger constructor - accept username and create directory(data, html & report)
    // In: username(folder name in Users_Folder)
    // Effective lines: 8
    File(string username)
    {
        dir += slash + username;
        dataDir = dir + slash + "data";
        reportDir = dir + slash + "report";
        htmlDir = dir + slash + "html";
        system((mkdir + dataDir).c_str());
        system((mkdir + reportDir).c_str());
        system((mkdir + htmlDir).c_str());
        clearScreen();
    }

    // Screen that ask filename
    // In: -
    // Out: Boolean of error
    // Effective lines: 19
    bool loadScreen()
    {
        do
        {
            data.clear();
            title.clear();
            compute.clear();
            error = false;
            string path, fileName;

            showHeader();
            cout << "Press enter to go back" << endl
                 << endl;
            cout << "What is the filename (without file extension -> .txt)" << endl;
            getline(cin, fileName);
            if (fileName == "")
            {
                return true;
            }
            path = dataDir + slash + fileName + ".txt";
            readFile(path);
            if (error)
                continue;
            break;
        } while (error);
        return false;
    }

    // Return number of column
    // In: -
    // Out: Number of column
    // Effective lines: 1
    int getCol()
    {
        return numCol;
    }

    // Return titles
    // In: -
    // Out: Vector of pair that contain title and index
    // Effective line: 1
    vecpair getTitle()
    {
        return title;
    }

    // Return compute
    // In: -
    // Out: Vector of integer of computable
    // Effective lines: 1
    vector<int> getCompute()
    {
        return compute;
    }

    // Return number of row
    // In: -
    // Out: Number of row
    // Effective line: 1
    int getRow()
    {
        return numRow;
    }

    // Return data
    // In: -
    // Out: 2-dimensional vector of data
    // Effective lines: 1
    vector2d getData()
    {
        return data;
    }

    // Save the data file as another name
    // In: -
    // Out: -
    // Effective lines: 12
    void savedAs()
    {
        string filename, path;
        ofstream outFile;

        askFilename(filename);

        path = dataDir + slash + filename;
        outFile.open(path);
        writeLine(outFile, numCol);
        writeLine(outFile, title);
        writeLine(outFile, compute);
        writeLine(outFile, numRow);
        writeLine(outFile, data);

        cout << "Your data file is saved as '" << filename << "' successfully" << endl;
        pressEnter();
    }

    // Save the generated plain text report
    // In: Generated plain text report data
    // Out: -
    // Effective lines: 20
    void saveTxtReport(const string &data)
    {
        ofstream outFile;
        string filename;
        string input;
        askTxtName(filename);
        string path = reportDir + slash + filename;
        outFile.open(path);
        outFile << data;
        outFile.close();
        while (true)
        {
            cout << "Do you want to view the report? Type 'y' to view or 'n' to skip" << endl;
            cin >> input;
            cin.ignore(50, '\n');
            if (input == "y")
            {
                cout << data << endl;
                pressEnter();
                return;
            }
            else if (input == "n")
            {
                return;
            }
            else
            {
                cout << "Invalid input" << endl;
            }
        }
    }

    // Save the generated html report
    // In: Generated HTML report data
    // Out: -
    // Effective lines: 20
    void saveHtmlReport(const string &data)
    {
        ofstream outFile;
        string filename;
        string input;
        askHtmlName(filename);
        string path = htmlDir + slash + filename;
        outFile.open(path);
        outFile << data;
        outFile.close();
        while (true)
        {
            cout << "Do you want to view the report? Type 'y' to view or 'n' to skip" << endl;
            cin >> input;
            cin.ignore(50, '\n');
            if (input == "y")
            {
                system(path.c_str());
                pressEnter();
                return;
            }
            else if (input == "n")
            {
                return;
            }
            else
            {
                cout << "Invalid input" << endl;
            }
        }
    }

    // Screen that show plain text report
    // In: -
    // Out: -
    // Effective lines: 17
    void txtReportScreen()
    {
        string filename;
        while (true)
        {
            showHeader();
            cout << "Please enter the name of txt report (without file extension -> .txt)" << endl;
            cout << "Press enter to go back" << endl;

            getline(cin, filename);

            if (filename == "")
            {
                return;
            }
            filename += ".txt";
            string path = reportDir + slash + filename;
            if (fileExist(path))
            {
                printTxtReport(path);
                pressEnter();
                break;
            }
            else
            {
                cout << "The file cannot found" << endl;
                pressEnter();
            }
        }
    }

    // Screen that show html report
    // In: -
    // Out: -
    // Effective lines: 15
    void htmlReportScreen()
    {
        string filename;
        while (true)
        {
            showHeader();
            cout << "Please enter the name of html report (without file extension -> .html) " << endl;
            cout << "Press enter to go back" << endl;
            getline(cin, filename);
            if (filename == "")
            {
                return;
            }
            filename += ".html";
            string path = htmlDir + slash + filename;
            if (fileExist(path))
            {
                system(path.c_str());
                break;
            }
            else
            {
                cout << "The file cannot found" << endl;
                pressEnter();
            }
        }
    }
};

// Log that contain date, time & operation
class Log
{
    string date, time, operation;
};

// Logger contain the show log and log function
class Logger
{
private:
    string logPath, logDir = "Users_Folder";
    vector<string> logs;

    // Get current date and time
    // In: -
    // Out: Current time (YYYY-MMM-DD HH:MM:SS)
    // Effective line: 6
    string getCurrentTime()
    {
        stringstream t;
        string day, month, date, times, year;
        time_t now = time(0);
        t << ctime(&now);
        t >> day >> month >> date >> times >> year;
        return year + "-" + month + "-" + date + "  " + times;
    }

    // read log from the log file
    // In: -
    // Out: Vector of logs
    // Effective line: 10
    vector<string> readFile()
    {
        ifstream file(logPath);
        string log;
        stringstream ss;

        vector<string> logs;

        while (!file.eof())
        {
            getline(file, log);
            if (log != "")
            {
                logs.push_back(log);
            }
        }
        file.close();
        return logs;
    }

    // Show all logs
    // In: Options(return), number of option(return) , start number of log, end number of log
    // Out: -
    // Effective line: 15
    void showLogMenu(string options[], int &num, int &start, int &end)
    {
        vector<string> logs = readFile();
        for (int i = start; i < end && i < logs.size(); i++)
        {
            cout << logs[i] << endl;
        }

        cout << endl
             << endl;

        if (start != 0)
        {
            cout << num << ". Previous" << endl;
            options[num - 1] = "p";
            num++;
        }
        if (start + 10 < logs.size())
        {
            cout << num << ". Next" << endl;
            options[num - 1] = "n";
            num++;
        }
        cout << num << ". Go back" << endl;
        options[num - 1] = "g";
    }

    // Get and check input from the user
    // In: Reference of i(return) & number of option
    // Out: Boolean of valid of invalid
    // Effective line: 14
    bool getValidInput(int &i, int &num)
    {
        string input;
        cout << "Please enter your input" << endl;
        cin >> input;
        cin.ignore(30, '\n');
        if (isdigit(input[0]))
        {
            i = stoi(input);
        }
        else
        {
            cout << "Please enter a valid input" << endl;
            pressEnter();
            return false;
        }

        if (i < 1 || i > num)
        {
            cout << "Please enter a valid input" << endl;
            pressEnter();
            return false;
        }

        return true;
    }

    // ----------------------------------------------------------------------------------------------------------------------------------------------------

public:
    // Logger constructor - accept username and create directory(log)
    // In: username(folder name in Users_Folder)
    // Effective lines: 5
    Logger(string username)
    {
        logDir += slash + username + slash + "log";
        string createDir = mkdir + logDir;
        system(createDir.c_str());
        clearScreen();
        logPath += logDir + slash + "log.txt";
    }

    // Log and write into the log file
    // In: Operation of user
    // Out: -
    // Effective line: 4
    void log(string operation)
    {
        ofstream file(logPath, ios::app);
        file << left;
        file << setw(25) << getCurrentTime() << operation << endl;
        file.close();
    }

    // Screen that show all logs
    // In: -
    // Out: -
    // Effective line: 16
    void showLogScreen()
    {
        int start = 0, end = 10, input;
        while (true)
        {
            string options[3];
            int num = 1;

            showHeader();
            showLogMenu(options, num, start, end);
            if (!getValidInput(input, num))
                continue;

            if (options[input - 1] == "p")
            {
                start = start - 10;
                end = end - 10;
            }
            else if (options[input - 1] == "n")
            {
                start = start + 10;
                end = end + 10;
            }
            else if (options[input - 1] == "g")
                break;
        }
    };
};
