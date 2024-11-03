#include <iostream>
#include <vector>

int main() {
    int rows, cols;

    std::cout << "행렬의 행 수를 입력하세요: ";
    std::cin >> rows;
    std::cout << "행렬의 열 수를 입력하세요: ";
    std::cin >> cols;

    std::vector<std::vector<int>> matrix1(rows, std::vector<int>(cols));
    std::vector<std::vector<int>> matrix2(rows, std::vector<int>(cols));
    std::vector<std::vector<int>> result(rows, std::vector<int>(cols));

    std::cout << "첫 번째 행렬의 값을 입력하세요:\n";
    for (int i = 0; i < rows; i++) {
        std::cout << "matrix1[" << i << "] = ";
        for (int j = 0; j < cols; j++) {
            std::cin >> matrix1[i][j];
        }
    }

    std::cout << "두 번째 행렬의 값을 입력하세요:\n";
    for (int i = 0; i < rows; i++) {
        std::cout << "matrix2[" << i << "] = ";
        for (int j = 0; j < cols; j++) {
            std::cin >> matrix2[i][j];
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    std::cout << "두 행렬의 합:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << result[i][j] << " ";
        }
        std::cout << "\n";
    }
    return 0;
}
