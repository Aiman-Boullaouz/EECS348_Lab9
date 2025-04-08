#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <type_traits>
#include <limits> // Added for numeric_limits

using namespace std;

template<typename T>
class Matrix {
private:
    int size;
    vector<vector<T>> data;

public:
    Matrix(int n) : size(n), data(n, vector<T>(n)) {}

    void readFromFile(ifstream& in) {
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                in >> data[i][j];
    }

    void display() const {
        for (const auto& row : data) {
            for (const auto& elem : row)
                cout << setw(10) << fixed << setprecision(2) << static_cast<double>(elem);
            cout << "\n";
        }
    }

    Matrix<T> operator+(const Matrix<T>& other) const {
        Matrix<T> result(size);
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                result.data[i][j] = data[i][j] + other.data[i][j];
        return result;
    }

    Matrix<T> operator*(const Matrix<T>& other) const {
        Matrix<T> result(size);
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                for (int k = 0; k < size; ++k)
                    result.data[i][j] += data[i][k] * other.data[k][j];
        return result;
    }

    void printDiagonalSums() const {
        T mainDiag = 0, secDiag = 0;
        for (int i = 0; i < size; ++i) {
            mainDiag += data[i][i];
            secDiag += data[i][size - i - 1];
        }
        cout << "Main Diagonal Sum: " << mainDiag << endl;
        cout << "Secondary Diagonal Sum: " << secDiag << endl;
    }

    void swapRows(int r1, int r2) {
        if (r1 >= 0 && r2 >= 0 && r1 < size && r2 < size)
            swap(data[r1], data[r2]);
    }

    void swapCols(int c1, int c2) {
        if (c1 >= 0 && c2 >= 0 && c1 < size && c2 < size)
            for (int i = 0; i < size; ++i)
                swap(data[i][c1], data[i][c2]);
    }

    void updateElement(int r, int c, T val) {
        if (r >= 0 && c >= 0 && r < size && c < size)
            data[r][c] = val;
    }
};

template<typename T>
void polymorphicSwap(vector<T>& row1, vector<T>& row2) {
    swap(row1, row2);
}

template<typename T>
void polymorphicSwap(vector<vector<T>>& matrix, int c1, int c2, bool isColumn) {
    if (isColumn) {
        for (auto& row : matrix) {
            if (c1 >= 0 && c2 >= 0 && c1 < row.size() && c2 < row.size())
                swap(row[c1], row[c2]);
        }
    }
}

int main() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file.\n";
        return 1;
    }

    int n, typeFlag;
    file >> n >> typeFlag;

    if (typeFlag == 0) {
        Matrix<int> m1(n), m2(n);
        m1.readFromFile(file);
        m2.readFromFile(file);

        cout << "Matrix 1:\n"; m1.display();
        cout << "Matrix 2:\n"; m2.display();

        cout << "\nMatrix Addition:\n"; (m1 + m2).display();
        cout << "\nMatrix Multiplication:\n"; (m1 * m2).display();
        cout << "\nDiagonal Sums for Matrix 1:\n"; m1.printDiagonalSums();

        int r1, r2;
        bool validInput = false;
        do {
            cout << "\nEnter two row indices to swap (Example: '0 2'): ";
            if (cin >> r1 >> r2) {
                if (r1 >= 0 && r1 < n && r2 >= 0 && r2 < n) {
                    validInput = true;
                } else {
                    cout << "Row indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter two integers.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!validInput);
        m1.swapRows(r1, r2);
        cout << "After Swapping Rows:\n"; m1.display();

        int c1, c2;
        validInput = false;
        do {
            cout << "\nEnter two column indices to swap (Example: '1 0'): ";
            if (cin >> c1 >> c2) {
                if (c1 >= 0 && c1 < n && c2 >= 0 && c2 < n) {
                    validInput = true;
                } else {
                    cout << "Column indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter two integers.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!validInput);
        m1.swapCols(c1, c2);
        cout << "After Swapping Columns:\n"; m1.display();

        int ur, uc, newVal;
        validInput = false;
        do {
            cout << "\nEnter row, column, and new value to update (Example: '0 0 99'): ";
            if (cin >> ur >> uc >> newVal) {
                if (ur >= 0 && ur < n && uc >= 0 && uc < n) {
                    validInput = true;
                } else {
                    cout << "Row and column indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter three integers (row, column, value).\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!validInput);
        m1.updateElement(ur, uc, newVal);
        cout << "After Updating Element:\n"; m1.display();

    } else {
        Matrix<double> m1(n), m2(n);
        m1.readFromFile(file);
        m2.readFromFile(file);

        cout << "Matrix 1:\n"; m1.display();
        cout << "Matrix 2:\n"; m2.display();

        cout << "\nMatrix Addition:\n"; (m1 + m2).display();
        cout << "\nMatrix Multiplication:\n"; (m1 * m2).display();
        cout << "\nDiagonal Sums for Matrix 1:\n"; m1.printDiagonalSums();

        int r1, r2;
        bool validInput = false;
        do {
            cout << "\nEnter two row indices to swap (Example: '0 2'): ";
            if (cin >> r1 >> r2) {
                if (r1 >= 0 && r1 < n && r2 >= 0 && r2 < n) {
                    validInput = true;
                } else {
                    cout << "Row indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter two integers.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!validInput);
        m1.swapRows(r1, r2);
        cout << "After Swapping Rows:\n"; m1.display();

        int c1, c2;
        validInput = false;
        do {
            cout << "\nEnter two column indices to swap (Example: '1 0'): ";
            if (cin >> c1 >> c2) {
                if (c1 >= 0 && c1 < n && c2 >= 0 && c2 < n) {
                    validInput = true;
                } else {
                    cout << "Column indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter two integers.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!validInput);
        m1.swapCols(c1, c2);
        cout << "After Swapping Columns:\n"; m1.display();

        int ur, uc;
        double newVal;
        validInput = false;
        do {
            cout << "\nEnter row, column, and new value to update (Example: '0 0 99'): ";
            if (cin >> ur >> uc >> newVal) {
                if (ur >= 0 && ur < n && uc >= 0 && uc < n) {
                    validInput = true;
                } else {
                    cout << "Row and column indices must be between 0 and " << n-1 << ". Please try again.\n";
                }
            } else {
                cout << "Invalid input. Please enter two integers followed by a double (row column value).\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!validInput);
        m1.updateElement(ur, uc, newVal);
        cout << "After Updating Element:\n"; m1.display();
    }

    return 0;
}