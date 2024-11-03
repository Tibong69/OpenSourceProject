#include <iostream>
#include <map>
#include <string>

struct Student {
    int mid;
    int fin;
};

int main() {
    std::map<std::string, Student> student_map;
    int n;

    std::cout << "최대학생 수: ";
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        std::string name;
        Student student;

        std::cout << "이름, 중간 점수, 기말 점수를 입력하세요: ";
        std::cin >> name >> student.mid >> student.fin;

        auto result = student_map.insert({name, student});
        if (result.second) {
            std::cout << "\"" << name << "\" 학생이 트리에 추가됨.\n";
        } else {
            std::cout << "\"" << name << "\" 학생이 트리에 이미 존재함.\n";
        }
    }

    std::cout << "\n학생 정보:\n";
    for (const auto &pair : student_map) {
        const auto &student = pair.second;
        std::cout << "이름 = " << pair.first 
                  << ", 중간고사 = " << student.mid 
                  << ", 기말고사 = " << student.fin << '\n';
    }

    return 0;
}
