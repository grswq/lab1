#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>
#include <windows.h>
#include <cmath>

using namespace std;

struct Result {
    int size;
    double time;
    long long operations;
    bool verified;
};

double* readMatrixFromFile(const string& filename, int N) {
    double* matrix = new double[N * N];
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: cannot open file " << filename << endl;
        return nullptr;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            file >> matrix[i * N + j];
        }
    }

    file.close();
    return matrix;
}

double* multiplyMatrices(double* A, double* B, int N) {
    double* C = new double[N * N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i * N + j] = 0;
            for (int k = 0; k < N; k++) {
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }

    return C;
}

void saveResult(const string& filename, double* C, int N) {
    ofstream out(filename);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            out << C[i * N + j];
            if (j < N - 1) out << " ";
        }
        out << endl;
    }
    out.close();
}

bool verifyResult(double* A, double* B, double* C, int N) {
    if (N > 5) {
        return true;
    }

    double* C_ref = new double[N * N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C_ref[i * N + j] = 0;
            for (int k = 0; k < N; k++) {
                C_ref[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }

    bool correct = true;
    double epsilon = 1e-9;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fabs(C[i * N + j] - C_ref[i * N + j]) > epsilon) {
                correct = false;
                break;
            }
        }
    }

    delete[] C_ref;
    return correct;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<int> sizes = { 100, 200, 400, 800, 1200, 1600 };
    vector<Result> results;

    cout << "========================================" << endl;
    cout << "Matrix Multiplication" << endl;
    cout << "========================================" << endl;

    for (int N : sizes) {
        cout << "\nProcessing matrices size " << N << "x" << N << "..." << endl;

        string fileA = "matrix_A_" + to_string(N) + ".txt";
        string fileB = "matrix_B_" + to_string(N) + ".txt";
        string fileC = "matrix_C_" + to_string(N) + ".txt";

        double* A = readMatrixFromFile(fileA, N);
        double* B = readMatrixFromFile(fileB, N);

        if (!A || !B) {
            cout << "  Error: cannot read files!" << endl;
            continue;
        }

        clock_t start = clock();
        double* C = multiplyMatrices(A, B, N);
        clock_t end = clock();

        double elapsed = double(end - start) / CLOCKS_PER_SEC;
        long long operations = 2LL * N * N * N;

        saveResult(fileC, C, N);

        bool verified = verifyResult(A, B, C, N);

        Result res = { N, elapsed, operations, verified };
        results.push_back(res);

        cout << "  Time: " << elapsed << " seconds" << endl;
        cout << "  Operations: ~" << operations << endl;
        cout << "  Result saved: " << fileC << endl;

        if (N <= 5) {
            if (verified) {
                cout << "  Verification: PASSED" << endl;
            }
            else {
                cout << "  Verification: FAILED" << endl;
            }
        }
        else {
            cout << "  Verification: SKIPPED (N > 5)" << endl;
        }

        delete[] A;
        delete[] B;
        delete[] C;
    }

    ofstream resultsFile("all_results.txt");
    resultsFile << "========================================" << endl;
    resultsFile << "MATRIX MULTIPLICATION RESULTS" << endl;
    resultsFile << "========================================" << endl;
    resultsFile << "Size N\t\tTime (sec)\tOperations\tVerified" << endl;
    resultsFile << "----------------------------------------" << endl;

    for (const auto& res : results) {
        resultsFile << res.size << "\t\t" << res.time << "\t\t" << res.operations << "\t\t";
        if (res.size <= 5) {
            resultsFile << (res.verified ? "YES" : "NO") << endl;
        }
        else {
            resultsFile << "SKIPPED" << endl;
        }
    }
    resultsFile << "========================================" << endl;
    resultsFile.close();

    cout << "\n========================================" << endl;
    cout << "FINAL RESULTS:" << endl;
    cout << "========================================" << endl;
    cout << "Size N\t\tTime (sec)\tOperations" << endl;
    cout << "----------------------------------------" << endl;
    for (const auto& res : results) {
        cout << res.size << "\t\t" << res.time << "\t\t" << res.operations << endl;
    }
    cout << "========================================" << endl;
    cout << "\nResults saved to: all_results.txt" << endl;

    return 0;
}
