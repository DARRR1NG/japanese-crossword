#include <iostream>
#include <vector>
#include <windows.h> // Для работы с Windows API (цвета, позиционирование курсора)
#include <clocale> // Для русского языка
#include <iomanip> // Для форматированного вывода (setw)
#include <string>
#include <sstream> // Для работы со строковыми потоками (разбор ввода)
#include <limits> // Для работы с пределами типов
#include <conio.h> // Добавлено для _getch

using namespace std;

// Константы для цветов консоли
const int RED = 12;
const int WHITE = 15;
const int BLUE = 9;

// Выбор уровня
class Level {
private:
    int size;
    vector<vector<int>> solution;
    vector<vector<int>> rowHints;
    vector<vector<int>> colHints;
    int maxRowHints;
    int maxColHints;

public:
    Level(int sz, const vector<vector<int>>& sol, const vector<vector<int>>& rowH,
          const vector<vector<int>>& colH, int maxRH, int maxCH)
        : size(sz), solution(sol), rowHints(rowH), colHints(colH),
          maxRowHints(maxRH), maxColHints(maxCH) {}

    //Геттеры предоставляют контролируемый доступ к приватным полям
    int getSize() const { return size; }
    const vector<vector<int>>& getSolution() const { return solution; }
    const vector<vector<int>>& getRowHints() const { return rowHints; }
    const vector<vector<int>>& getColHints() const { return colHints; }
    int getMaxRowHints() const { return maxRowHints; }
    int getMaxColHints() const { return maxColHints; }
};

class JapaneseCrossword {
private:
    vector<Level*> levels;
    vector<vector<int>>* grid;
    vector<vector<int>>* solution;
    vector<vector<int>>* rowHints;
    vector<vector<int>>* colHints;
    int N;
    int maxRowHints;
    int maxColHints;

    void setColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void resetColor() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
    }

    void initializeLevels() {
        // Легкий (5x5)
        levels.push_back(new Level(
            5,
            {
                {0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {1, 1, 0, 0, 1},
                {0, 1, 1, 1, 0},
                {0, 1, 1, 1, 0}
            },
            {
                {0}, {1}, {2, 1}, {3}, {3}
            },
            {
                {1}, {4}, {2}, {2}, {1}
            },
            2, 1
        ));

        // Средний (10x10)
        levels.push_back(new Level(
            10,
            {
                {0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
                {0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
                {0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
                {0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
                {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
                {0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
                {0, 0, 1, 1, 1, 1, 1, 1, 0, 0}
            },
            {
                {2, 2}, {2, 2}, {2, 2}, {2, 2}, {8}, {10}, {10}, {2, 4, 2}, {3, 3}, {6}
            },
            {
                {3}, {5}, {7, 2}, {10}, {4, 1}, {4, 1}, {10}, {7, 2}, {5}, {3}
            },
            3, 1
        ));

        // Сложный (15x15)
        levels.push_back(new Level(
            15,
            {
                {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
                {0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0},
                {0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1},
                {0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1},
                {0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1},
                {0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
                {0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
                {0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
                {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
                {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}
            },
            {
                {3}, {4, 2}, {6, 6}, {6, 2, 1}, {1, 4, 2, 1}, {6, 3, 2}, {6, 7}, {6, 8}, {1, 10}, {1, 10}, {1, 10}, {1, 1, 4, 4}, {3, 4, 4}, {4, 4}, {4, 4}
            },
            {
                {1}, {11}, {3, 3, 1}, {7, 2}, {7}, {15}, {1, 5, 7}, {2, 8}, {14}, {9}, {1, 6}, {1, 9}, {1, 9}, {1, 10}, {12}
            },
            4, 2
        ));
    }

    // Обновление консоли
    void clearScreen() {
        system("cls");
    }

    // Написание правил справа от игрового поля
    void printRules(int y) {
        COORD coord; // Структура для координат в консоли
        coord.X = maxRowHints * 3 + N * 3 + 5; // Расчет позиции по X
        coord.Y = y;                           // Расчет позиции по Y
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        cout << "Правила игры:";
        coord.Y++;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << "1. Числа слева и сверху указывают, сколько подряд идущих";
        coord.Y++;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << "   закрашенных клеток должно быть в строке/столбце.";
        coord.Y++;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << "2. Порядок чисел соответствует порядку закрашенных блоков.";
        coord.Y++;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << "3. Красные числа - нумерация строк и столбцов.";
        coord.Y++;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << "4. Для выхода в меню нажмите: '0 0'";
        coord.Y++;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << "5. Для выхода нажмите клавишу 'esc'";
    }

    // Правильная обработка ошибок, чтобы не было проблем
    void showError(const string& message) {
        COORD coord;
        coord.X = 0;
        coord.Y = maxColHints + N + 2; // Позиция под игровым полем
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << message << " Нажмите любую клавишу для продолжения...";
        _getch(); // Ожидание нажатия клавиши
    }

public:
    JapaneseCrossword() : grid(nullptr), solution(nullptr), rowHints(nullptr), colHints(nullptr), N(0), maxRowHints(0), maxColHints(0) {
        initializeLevels();
    }

    // Деструктор - освобождение памяти
    ~JapaneseCrossword() {
        for (auto level : levels) {
            delete level;
        }
        if (grid) delete grid;
        if (solution) delete solution;
        if (rowHints) delete rowHints;
        if (colHints) delete colHints;
    }

    void printGrid() {
        clearScreen();

        // Увеличение размера консоли для игрового поля 15 на 15
        if (N >= 15) {
            system("mode con: cols=150 lines=50");
        }

        // Вывод подсказок для столбцов (числа сверху)
        for (int hintRow = 0; hintRow < maxColHints; hintRow++) {
            cout << string(maxRowHints * 3, ' '); // Отступ выравнивания подсказок для строк (рассчитанные, что на одну подсказку идет 3 символа)
            cout << "    ";
            // Вывод подсказок для каждого столбца
            for (int col = 0; col < N; col++) {
                if (hintRow < (*colHints)[col].size()) { // Если подсказка существует, выводим её с форматированием (ширина 2 символа)
                    cout << setw(2) << (*colHints)[col][hintRow] << " ";
                } else {
                    cout << "   "; // Если подсказки нет, выводим пробелы для сохранения структуры
                }
            }
            cout << endl;
        }

        // Нумерация столбцов (красный цвет)
        cout << string(maxRowHints * 3, ' ') << "    ";
        setColor(RED);
        for (int col = 1; col <= N; col++) {
            // Вывод номеров столбцов с форматированием
            cout << setw(2) << col << " ";
        }
        resetColor();
        cout << endl;

        // Горизонтальная линия для визуального разделения подсказок и игрового поля
        cout << string(maxRowHints * 4, ' ') << string(N * 4, '_') << endl;

        // Вывод игрового поля и подсказок для строк
        for (int i = 0; i < N; ++i) {

            // Вывод подсказок для текущей строки (способ приравнять подсказки по правому краю при случаи, что кол-во подсказок меньше максмального кол-ва и понадобятся пробелы)
            for (int h = 0; h < maxRowHints; h++) {
                int idx = (*rowHints)[i].size() - maxRowHints + h;
                if (idx >= 0) {
                    cout << setw(2) << (*rowHints)[i][idx] << " ";
                } else {
                    cout << "   ";
                }
            }

            // Нумерация строк (синий цвет)
            setColor(BLUE);

            cout << setw(2) << i + 1 << " "; // Вывод номера строки

            resetColor();

            cout << "|";

            // Заполнение игрового поля
            for (int j = 0; j < N; ++j) {
                if ((*grid)[i][j] == 1) {
                    cout << " X ";
                } else {
                    cout << " . ";
                }
            }
            cout << endl;
        }

        printRules(0);
    }

    bool isValidMove(int row, int col) {
        return (*solution)[row][col] == 1;
    }

    // Проверка на победу
    bool checkWin() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if ((*grid)[i][j] != (*solution)[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    // Функция для безопасного ввода целого числа с проверкой диапазона
    int getIntInput(const string& prompt, int minVal, int maxVal) {
        int value;
        while (true) {
            cout << prompt;
            string input; // Строка для хранения ввода пользователя
            getline(cin, input); // Чтение всей строки ввода (включая пробелы)

            bool valid = true;
            size_t pos = 0; // Позиция для stoi (показывает, до какого символа выполнено преобразование)
            try {
                // Попытка преобразования строки в целое число (stoi преобразует строку в int, &pos сохраняет позицию первого непереведенного символа)
                value = stoi(input, &pos);
                // Проверка, что вся строка была обработана (нет лишних символов)
                if (pos != input.length()) {
                    valid = false;
                }
            } catch (...) {
                valid = false;
            }

            if (!valid) {
                // Случай 1: Ввод содержал нечисловые символы
                cout << "Ошибка ввода! Пожалуйста, введите только одно число от " << minVal << " до " << maxVal << ": ";
            } else if (value == 0) {
                // Случай 2: Пользователь ввел 0 - специальное значение для выхода
                return 0;
            } else if (value < minVal || value > maxVal) {
                // Случай 3: Число вне допустимого диапазона
                cout << "Неверное значение! Пожалуйста, введите число от " << minVal << " до " << maxVal << ": ";
            } else {
                // Случай 4: Корректный ввод - возвращаем значение
                return value;
            }
        }
    }

    // Стартовое окно консоли
    void selectLevel() {
        clearScreen();
        cout << "Добро пожаловать в игру 'Японский кроссворд'!\n";
        cout << '\n';
        cout << "Правила:\n";
        cout << "1.Числа слева и сверху указывают, сколько подряд идущих закрашенных клеток должно быть в строке/столбце.\n";
        cout << "2.Порядок чисел соответствует порядку закрашенных блоков.\n";
        cout << "3.Неизвестно, где именно находятся блоки — их нужно вычислить логически, учитывая пересечения строк и столбцов.\n";
        cout << '\n';
        cout << "Выберите уровень сложности кроссворда, в который вы хотите сыграть:\n";
        cout << "Введите '1' - Легкий (5x5)\n";
        cout << "Введите '2' - Средний (10x10)\n";
        cout << "Введите '3' - Сложный (15x15)\n";
        cout << '\n';
        cout << "Введите '0' - для выхода из игры\n";
        cout << '\n';

        int choice = getIntInput("Ваш выбор: ", 1, 3);
        if (choice == 0) {
            exit(0);
        }

        Level* selected = levels[choice - 1];
        // N - сохраняем размер игрового поля (5, 10 или 15)
        N = selected->getSize();

        // Освобождаем предыдущие данные, если они есть
        if (grid) delete grid;
        if (solution) delete solution;
        if (rowHints) delete rowHints;
        if (colHints) delete colHints;

        // Создаем новые динамические объекты
        grid = new vector<vector<int>>(N, vector<int>(N, 0));
        solution = new vector<vector<int>>(selected->getSolution());
        rowHints = new vector<vector<int>>(selected->getRowHints());
        colHints = new vector<vector<int>>(selected->getColHints());
        maxRowHints = selected->getMaxRowHints();
        maxColHints = selected->getMaxColHints();
    }

    // Ход игры и вывод ошибок
    void play() {
            while (true) {
            selectLevel();
            printGrid();

            int row, col;
            string input;
            while (true) {
                // Проверяем нажатие клавиши функцией _kbhit(), которая возвращает true или false
                if (_kbhit()) {
                    int key = _getch();
                    if (key == 27) { // 27 — ASCII-код клавиши ESC
                        exit(0); // Завершаем программу
                    }
                }

                COORD coord;
                coord.X = 0;
                coord.Y = maxColHints + N + 2;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                cout << "Введите номер строки и столбца (1-" << N << "), чтобы закрасить клетку (например: ";
                setColor(BLUE);
                cout << "2";
                resetColor();
                cout << " ";
                setColor(RED);
                cout << "3";
                resetColor();
                cout << "): ";

                // Используем посимвольный ввод вместо getline, чтобы не блокировать ESC (и устраняем очень много костылей)
                input.clear();
                char ch;
                while ((ch = _getch()) != '\r') { // Читаем символы, пока не нажат Enter
                    if (ch == 27) {
                        exit(0);
                    }
                    if (ch == '\b' && !input.empty()) { // ASCII-код Backspace, если нажат Backspace и строка не пуста
                        input.pop_back(); // Удаляет последний символ из строки input
                        cout << "\b \b"; // Стираем символ в консоли, первое /b - перемещает курсор назад на 1 позицию и затирает символ, второе - перемещает курсор назад (готов к новому вводу)
                    }
                    else if (isdigit(ch) || ch == ' ') {
                        // Если символ цифра или пробел добавляем его в строку и выводим на экран
                        input.push_back(ch);
                        cout << ch;
                    }
                }
                cout << endl;

                // Разбор введённой строки
                istringstream iss(input); // Создаём поток для чтения из строки input, iss читает данные до первого пробела/перевода строки и автоматически преобразует в нужный тип данных
                if (!(iss >> row >> col)) {
                    showError("Ошибка ввода! Пожалуйста, введите два числа (например: 2 3).");
                    printGrid();
                    continue;
                }

                // Вдруг что-то есть после row и col
                string remaining;
                if (iss >> remaining) {
                    showError("Ошибка ввода! Пожалуйста, введите ТОЛЬКО ДВА ЧИСЛА (например: 2 3).");
                    printGrid();
                    continue;
                }

                // Возврат в меню
                if (row == 0 && col == 0) {
                    break;
                }

                if (row < 1 || row > N || col < 1 || col > N) {
                    showError("Неверные координаты. Попробуйте снова.");
                    printGrid();
                    continue;
                }

                if (!isValidMove(row - 1, col - 1)) {
                    showError("Ошибка: Здесь не должна быть закрашенная клетка!");
                    printGrid();
                    continue;
                }

                (*grid)[row - 1][col - 1] = 1;
                printGrid();

                if (checkWin()) {
                    clearScreen();
                    cout << "Поздравляем! Вы выиграли!" << endl;
                    cout << "Нажмите любую клавишу для продолжения...";
                    _getch();
                    break;
                }
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    JapaneseCrossword game; // Создание объекта игры
    game.play(); // Запуск игрового процесса
    return 0;
}
