#include <iostream>
#include <algorithm> // for sort
#include <cmath> // for pow
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;

// find a specific column or all columns
char select_column()
{
    cout << "Specific column or all column?"
            << " (1 for specific, 2 for all): ";
    char choice;
    cin >> choice;
    return choice;
}


//find min from a 2d array
int find_min(vector2d arr , int row, int column)
{
    int min = arr[0][column];
    for (int i = 0; i < row; i++)
    {
        if (arr[i][column] < min)
        {
            min = arr[i][column];
        }
    }
    return min;
}


// find max from a 2d array
int find_max(vector2d arr , int row, int column)
{
    int max = arr[0][column];
    for (int i = 0; i < row; i++)
    {
        if (arr[i][column] > max)
        {
            max = arr[i][column];
        }
    }
    return max;
}

// find median from a 2d array
double find_median(vector2d arr , int row, int column)
{
    double median = 0;
    double temp[row];
    for (int i = 0; i < row; i++)
    {
        temp[i] = arr[i][column];
    }
    sort(temp, temp + row);
    if (row % 2 == 0)
    {
        median = (temp[row / 2] + temp[row / 2 - 1]) / 2;
    }
    else
    {
        median = temp[row / 2];
    }
    return median;
}

double find_mean(vector2d arr , int row, int column)
{
    double sum = 0;
    for (int i = 0; i < row; i++)
    {
        sum += arr[i][column];
    }
    return sum / row;
}

double find_variance(vector2d arr, int row, int column)
{
    double mean = find_mean(arr, row, column);
    double sum = 0;
    for (int i = 0; i < row; i++)
    {
        sum += pow(arr[i][column] - mean, 2);
    }
    return sum / row;
}

double find_stdv(vector2d arr , int row, int column)
{
    double variance = find_variance(arr, row, column);
    return sqrt(variance);
}



// // Allocates dynamic array
// int **allocateArray(int row, int col)
// {
//     int **arr;
//     arr = new int*[row];
//     for (int i=0; i<row; i++)   
//         arr[i] = new int[col];
//     return arr;
// }

// //Free dynamic array
// void freeArray(int **arr, int row)
// {
//     for (int i=0; i<row; i++)   
//         delete[] arr[i];
//     delete[] arr;
// }


vector<vector<int>> classInterval(vector<int> data) {
  int c, min , max ;
  vector<vector<int>> interval;
	// min = find_min();
  c = ceil(1 + (3.3 * log10(data.size())));
  int classWidth = ceil((max - min) / float(c));
  int lowerBoundary = min;
  int upperBoundary = min + classWidth - 1;

  for (int i = 0; i < c; i++) {
    vector<int> temp = {lowerBoundary, upperBoundary};
    interval.push_back(temp);
    // 125
    lowerBoundary += classWidth;
    // 129
    upperBoundary += classWidth;
  }

  return interval;
}

vector<pair<double, int>> countData(vector<int> data,
                                    vector<vector<int>> interval) {
  vector<pair<double, int>> count;
  for (auto i : interval) {
    int counter = 0;
    for (auto d : data) {
      if (d >= i[0] && d <= i[1]) {
        counter++;
      }
    }
    double midpoint = (i[0] + i[1]) / 2;
    pair<int, double> temp = {midpoint, counter};
    count.push_back(temp);
  }
  return count;
}

string createTable(vector<pair<double, int>> count) {
  stringstream table;

  table << left;
  table << "Counts\t"
        << "Mid-points" << endl;
  for (int i = count.size() - 1; i >= 0; i--) {
    table << setw(10) << count[i].second << setw(4) << count[i].first << "|";
    for (int j = 0; j < count[i].second; j++) {
      table << "=";
    }
    table << endl;
  }
  table << setw(14) << ""
        << "+---------+---------+---------+---------+---------+" << endl;
  table << setw(14) << ""
        << "0         10        20        30        40        50" << endl;
  return table.str();
}

string histogram(vector2d data, int min, int max, int row, int index) {
  vector<vector<int>> interval = classInterval(min, max, row);
  vector<pair<double, int>> count = countData(data, interval, index);
  string table = createTable(count);
  cout << table;
  return table;
}



// Show Computable Titles
void showComputableTitles(vecpair titles, vector<int> computable)
{
  cout << "       Computable Titles       " << endl;
  cout << "+---------+-------------------+" << endl;
  cout << "|  Index  |       Titles      |" << endl;
  cout << "+---------+-------------------+" << endl;
  for (int i = 0; i < titles.size(); i++)
  {
      if (computable[i] == 1)
      {
          cout << "|" << setw(5) << titles[i].second << setw(5)
                << "|" << setw(13) << titles[i].first << setw(7)
                << "|" << endl;
      }
  }
  cout << "+---------+-------------------+" << endl;
}

int returnFunction(char choice, vector2d arr2d, int row, int index)
{
  if (choice == '1')
    return find_min(arr2d, row, index);
  else if (choice == '2')
    return find_max(arr2d, row, index);
  else if (choice == '3')
    return find_median(arr2d, row, index);
  else if (choice == '4')
    return find_mean(arr2d, row, index);
  else if (choice == '5')
    return find_variance(arr2d, row, index);
  else if (choice == '6')
    return find_stdv(arr2d, row, index);
}

int singleColumnCompute
(string method,
char &choice,
vecpair titles, 
vector<int> computable,
vector2d arr2d, 
int row,
bool &flag2)
{
  cout << "Select title by index" << endl;
  cin >> choice;
  int choicetoint = choice - '0';
  int index = choicetoint - 1;

  if (computable[index] == 0)
  {
      cout << "Error: Title not computable" << endl;
      pressEnter();
  }
  else if (choicetoint < 0 || choicetoint > computable.size())
  {
      cout << "Error: Invalid index" << endl;
      pressEnter();
  }
  else
  {
      int result = returnFunction(choice, arr2d, row, index);
      cout << method << " of " 
          << titles[index].first << ": " 
          << result << endl;

      cin.ignore();
      pressEnter();
      flag2 = true;
      return result;
  }
}

void allColumnCompute(string method, vecpair titles, vector<int> computable, char choice, vector2d arr2d, int row)
{
  for (int i = 0; i < titles.size(); i++)
    {
      if (computable[i] == 1)
      {
          int result = returnFunction(choice, arr2d, row, i);
          cout << method << " of " 
              << titles[i].first << ": " 
              << result << endl;

      }
    }
}

