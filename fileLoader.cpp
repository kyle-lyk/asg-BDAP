#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <ctime>
#include <utility>
#include <iomanip>

#if defined(__linux__) || defined(__APPLE__)
#define slash "/"
#define mkdir "mkdir -p"
#else
#define slash "\\"
#define mkdir "mkdir "
#endif

using namespace std;

void showHeader()
{
    cout << "+--------------------------------------------------+" << endl;
    cout << "|            Basic Data Analysis Program           |" << endl;
    cout << "|                    (  BDAP  )                    |" << endl;
    cout << "+--------------------------------------------------+" << endl;
}

class File
{
private:
    typedef vector<vector<int>> vector2d;
    typedef vector<pair<string, int>> vecpair;
    int numCol = 0, numRow = 0;
    vector2d data;
    vecpair title;
    vector<int> compute;
    ifstream inFile;
    bool error;
    string dir = "Users_Folder", dataDir, reportDir, htmlDir;

    int readCol(ifstream &file)
    {
        int col;
        file >> col;
        if (!col)
        {
            cout << "Number of column is missing" << endl;
            cout << "Press enter to continue" << endl;
            error = true;
            cin.get();
            return 0;
        }
        file.ignore(100, '\n');
        return col;
    }

    vecpair readTitle(ifstream &file)
    {
        vecpair titles;
        pair<string, int> temp;
        stringstream stream;
        string line;
        getline(inFile, line);
        stream << line;
        string col;
        int index = 1;
        while (stream >> col)
        {
            temp = {col, index};
            titles.push_back(temp);
            index++;
        }

        if (titles.size() != numCol)
        {
            error = true;
            int difference = numCol - titles.size();
            cout << difference << " title " << (difference > 1 ? "are" : "is") << " missing" << endl;
            cout << "Press enter to continue" << endl;
            titles.clear();
            cin.get();
        }
        return titles;
    }

    vector<int> readCompute(ifstream &file)
    {
        vector<int> computes;
        stringstream stream;
        string line;
        getline(inFile, line);
        stream << line;
        int col;
        while (stream >> col)
        {
            computes.push_back(col);
        }

        if (computes.size() != numCol)
        {
            error = true;
            int difference = numCol - computes.size();
            computes.clear();
            cout << difference << " computability " << (difference > 1 ? "are" : "is") << " missing" << endl;
            cout << "Press enter to continue" << endl;
            cin.get();
        }
        return computes;
    }

    int readRow(ifstream &file)
    {
        int row;
        file >> row;
        if (!row)
        {
            error = true;
            cout << "Number of row is missing" << endl;
            cout << "Press enter to continue" << endl;
            cin.get();
            return 0;
        }
        file.ignore(100, '\n');
        return row;
    }

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

    vector<int> getLineData(const string &line, const int &currentRow)
    {
        vector<int> row;
        stringstream stream;
        string strCol;
        int intCol;
        stream << line;
        while (stream >> strCol)
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
            cout << "Press enter to continue" << endl;
            cin.get();
            return row;
        }
    }

    void readData(vector2d &data, int row)
    {
        string line;
        int countRow = 0;
        while (countRow < row)
        {
            getline(inFile, line);
            vector<int> d = getLineData(line, countRow);
            if (error)
            {
                cout << "Press enter to continue" << endl;
                cin.get();
                return;
            }
            data.push_back(d);
            countRow++;
        }
        inFile.close();
        if (data.size() != numRow)
        {
            error = true;
            int difference = numRow - data.size();
            cout << difference << " row " << (difference > 1 ? "are" : "is") << " missing" << endl;
            cout << "Press enter to continue" << endl;
            cin.get();
        }
    }

    void readFile(string path)
    {
        inFile.open(path);
        if (inFile)
        {
            readInfo(inFile);
            if (error)
            {
                inFile.close();
                return;
            }
            readData(data, numRow);
            if (error)
            {
                inFile.close();
                return;
            }
        }
        else
        {
            error = true;
            cout << "The file cannot open" << endl;
            cout << "Press enter to continue" << endl;
            cin.get();
            return;
        }
    }

    // ------------------------------------------------------------------------
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
                    cout << "Press enter to continue" << endl;
                    cin.get();
                    return false;
                }
            }
        }
        return true;
    }

    bool fileExistScreen()
    {
        string res;
        do
        {
            showHeader();
            cout << endl
                 << endl;
            cout << "The filename already exist" << endl;
            cout << "Type 'yes' to replace and 'no' to set a new name" << endl;
            getline(cin, res);
            if (res != "yes" && res != "no")
            {
                cout << "Please enter a valid input" << endl;
                cout << "Press enter to continue" << endl;
                cin.get();
            }

        } while (res != "yes" && res != "no");

        return res == "yes" ? true : false;
    }

    void writeLine(ofstream &file, const int &data)
    {
        file << data << endl;
    }

    void writeLine(ofstream &file, const vecpair &data)
    {
        for (pair<string, int> temp : data)
        {
            file << temp.first << " ";
        }
        file << endl;
    }

    void writeLine(ofstream &file, const vector<int> &data)
    {
        for (int i : data)
        {
            file << i << " ";
        }
        file << endl;
    }

    void writeLine(ofstream &file, const vector2d &data)
    {
        for (int i = 0; i < numRow; i++)
        {
            for (int col : data[i])
            {
                file << col << " ";
            }
            if (i < numRow - 1)
            {
                file << endl;
            }
        }
    }

    bool fileExist(const string &path)
    {
        ifstream file(path);
        bool exist = file ? true : false;
        file.close();
        return exist;
    }

    void askFilename(string &filename)
    {
        while (true)
        {
            showHeader();
            cout << endl
                 << endl;
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

    // ------------------------------------------------------------------------

    void askTxtName(string &filename)
    {
        while (true)
        {
            showHeader();
            cout << endl
                 << endl;
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

    void askHtmlName(string &filename)
    {
        while (true)
        {
            showHeader();
            cout << endl
                 << endl;
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
public:
    File(string username)
    {
        dir += slash + username;
        dataDir = dir + slash + "data";
        reportDir = dir + slash + "report";
        htmlDir = dir + slash + "html";
        system((mkdir + dataDir).c_str());
        system((mkdir + reportDir).c_str());
        system((mkdir + htmlDir).c_str());
    }

    void loadScreen()
    {
        do
        {
            data.clear();
            title.clear();
            compute.clear();
            error = false;
            string path, fileName;

            showHeader();
            cout << endl
                 << endl;
            cout << "What is the filename" << endl;
            getline(cin, fileName);
            path = dataDir + slash + fileName;
            // cout << path << endl;
            readFile(path);
            if (error)
                continue;
            break;
        } while (error);
    }

    int getCol()
    {
        return numCol;
    }

    vecpair getTitle()
    {
        return title;
    }

    vector<int> getCompute()
    {
        return compute;
    }

    int getRow()
    {
        return numRow;
    }

    vector2d getData()
    {
        return data;
    }

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
    }

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
        cout << "Do you want to view the report? Type 'yes' to view or press enter to skip" << endl;
        cin >> input;
        if (input == "yes")
        {
            cout << data << endl;
        }
    }

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
        cout << "Do you want to view the report? Type 'yes' to view or press enter to skip" << endl;
        cin >> input;
        if (input == "yes")
        {
            cout << path << endl;
            system(path.c_str());
        }
    }

    void printTxtReport(string path)
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

    void txtReportScreen()
    {
        string filename;
        while (true)
        {
            showHeader();
            cout << "Please enter the name of txt report (without file extension -> .txt) " << endl;
            cin.ignore(30, '\n');
            getline(cin, filename);
            filename += ".txt";
            string path = reportDir + slash + filename;
            if (fileExist(path))
            {
                printTxtReport(path);
                break;
            }
            else
            {
                cout << "The file cannot found" << endl;
                cout << "Press enter to continue" << endl;
                cin.get();
            }
        }
    }

    void htmlReportScreen()
    {
        string filename;
        while (true)
        {
            showHeader();
            cout << "Please enter the name of html report (without file extension -> .html) " << endl;
            cin.ignore(30, '\n');
            getline(cin, filename);
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
                cout << "Press enter to continue" << endl;
                cin.get();
            }
        }
    }
};

class Log
{
    string date, time, operation;
};

class Logger
{
private:
    string logPath, logDir = "Users_Folder";
    vector<string> logs;

    string getCurrentTime()
    {
        stringstream t;
        string day, month, date, times, year;
        time_t now = time(0);
        t << ctime(&now);
        t >> day >> month >> date >> times >> year;
        return year + "-" + month + "-" + date + "  " + times;
    }

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

    bool getValidInput(int &i, int &num)
    {
        string input;
        cout << "Please enter your input" << endl;
        // cin.ignore(30,'\n');
        cin >> input;
        cin.ignore(30, '\n');
        if (isdigit(input[0]))
        {
            i = stoi(input);
        }
        else
        {
            cout << "Please enter a valid input" << endl;
            cout << "Press enter to continue" << endl;
            cin.get();
            return false;
        }

        if (i < 1 || i > num)
        {
            cout << "Please enter a valid input" << endl;
            cout << "Press enter to continue" << endl;
            cin.get();
            return false;
        }

        return true;
    }

public:
    Logger(string username)
    {
        logDir += slash + username + slash + "log";
        string createDir = mkdir + logDir;
        system(createDir.c_str());
        logPath += logDir + slash + "log.txt";
    }

    void log(string operation)
    {
        ofstream file(logPath, ios::app);
        file << left;
        file << setw(25) << getCurrentTime() << operation << endl;
        file.close();
    }

    void showLogScreen()
    {
        int start = 0, end = 10, input;
        while (true)
        {
            string options[3];
            int num = 1;

            showHeader();
            cout << endl
                 << endl;
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

// int main()
// {
//     File file("limcz");
//     file.loadScreen();
//     int col = file.getCol();
//     int row = file.getRow();
//     vector<pair<string, int>> title = file.getTitle();
//     vector<int> compute = file.getCompute();
//     vector<vector<int>> data = file.getData();
    

//     cout << col << endl;
//     cout << row << endl;

//     for (auto c : compute)
//     {
//         cout << c << ' ';
//     }

//     cout << endl;

//     for (auto p : title)
//     {
//         cout << p.first << " ";
//     }
//     cout << endl;

//     for (auto row : data)
//     {
//         for (auto col : row)
//         {
//             cout << col << " ";
//         }
//         cout << endl;
//     }
// }