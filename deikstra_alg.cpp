#include "deikstra_alg.h"

const extern int INF = 10e6;

vector<string> deikstra_alg (vector<string> map_array) {
    map <int,TDot> all_nodes;
    vector<TDot> camp_dots;
    vector<string> comand_set;
    TDot start_dot, camp_dot, end_dot;
    unsigned int column_size = map_array[1].length();
    unsigned int string_size = map_array.size()-1;
    unsigned int size_ways_matrix = column_size * string_size;

    int** coordinat_map_array = new int* [size_ways_matrix]; // создаем матрицу связности и заполняем ее бесконечностями
    for (int i = 0; i < size_ways_matrix; i++) {
        coordinat_map_array[i] = new int [size_ways_matrix];
        for (int j = 0; j < size_ways_matrix; j ++) {
            coordinat_map_array[i][j] = INF;
            if (i == j) coordinat_map_array[i][j] = 0; // диагональные элементы заполняются нулями
        }
    }

    for (int i = 0; i < string_size; i++) {
        for (int j = 0; j < column_size; j++) {
            TDot node;
            node.x = i;
            node.y = j;
            all_nodes[i*column_size+j] = node;
            // Нахождение соседей
            //  [i][j-1]   [i][j]   [i][j+1]
            // [i+1][j-1] [i+1][j] [i+1][j+1]
            // [i+2][j-1] [i+2][j] [i+2][j+1]
            // А в матрице связности для элемента [i+j] (мы убираем ненужный для нее +1)
            // при условии что элемент сосед существует и != 'X' =>
            if (map_array[i+1][j] != 'X') {
                if (i > 0 and j-1 >= 0 and map_array[i][j-1] != 'X') coordinat_map_array[i*column_size+j][(i-1)*column_size+j-1] = 1;
                if (i > 0 and map_array[i][j] != 'X') coordinat_map_array[i*column_size+j][(i-1)*column_size+j] = 1;
                if (i > 0 and j+1 < column_size and map_array[i][j+1] != 'X') coordinat_map_array[i*column_size+j][(i-1)*column_size+j+1] = 1;
                if (j-1 >= 0 and map_array[i+1][j-1] != 'X') coordinat_map_array[i*column_size+j][i*column_size+j-1] = 1;
                if (j+1 < column_size and map_array[i+1][j+1] != 'X') coordinat_map_array[i*column_size+j][i*column_size+j+1] = 1;
                if (i+1 < string_size and j-1 >= 0 and map_array[i+2][j-1] != 'X') coordinat_map_array[i*column_size+j][(i+1)*column_size+j-1] = 1;
                if (i+1 < string_size and map_array[i+2][j] != 'X') coordinat_map_array[i*column_size+j][(i+1)*column_size+j] = 1;
                if (i+1 < string_size and j+1 < column_size and map_array[i+2][j+1] != 'X') coordinat_map_array[i*column_size+j][(i+1)*column_size+j+1] = 1;
            }
            // Следовательно соотнести существование соседа, что он не препятсивие и записать это в матрицу путей;

            if (map_array[i+1][j] == 'S') {
                start_dot.x = i;
                start_dot.y = j;
            }
            else if (map_array[i+1][j] == '&') {
                camp_dot.x = i;
                camp_dot.y = j;
                camp_dots.push_back(camp_dot);
            }
            else if (map_array[i+1][j] == 'O') {
                end_dot.x = i;
                end_dot.y = j;
            }
        }
    }

    camp_dots.push_back(end_dot);

    int d[size_ways_matrix]; // минимальное расстояние
    int v[size_ways_matrix]; // посещенные вершины
    int temp, minindex, min;
    int begin_index = start_dot.x*column_size + start_dot.y;

    for (int i = 0; i<size_ways_matrix; i++) {
        d[i] = 10000;
        v[i] = 1;
    }
    d[begin_index] = 0;
    do {
        minindex = 10000;
        min = 10000;
        for (int i = 0; i<size_ways_matrix; i++) { // Если вершину ещё не обошли и вес меньше min
            if ((v[i] == 1) && (d[i]<min)) { // Переприсваиваем значения
                min = d[i];
                minindex = i;
            }
        }
        // Добавляем найденный минимальный вес
        // к текущему весу вершины
        // и сравниваем с текущим минимальным весом вершины
        if (minindex != 10000) {
            for (int i = 0; i<size_ways_matrix; i++) {
                if (coordinat_map_array[minindex][i] > 0) {
                    temp = min + coordinat_map_array[minindex][i];
                    if (temp < d[i]) {
                        d[i] = temp;
                    }
                }
            }
            v[minindex] = 0;
        }
    } while (minindex < 10000);

    for (int i=1; i<camp_dots.size()-1; i++) {
        for (int j = 1; j < camp_dots.size() - i; j++) {
            if (d[camp_dots[i].x*column_size + camp_dots[i].y] < d[camp_dots[i+1].x*column_size + camp_dots[i+1].y]) {
                TDot temp = camp_dots[i];
                camp_dots[i] = camp_dots[i+1];

            }
        }
    }

    int ver[size_ways_matrix+1]; // массив посещенных вершин
    int end = end_dot.x*column_size + end_dot.y; // индекс конечной вершины = 5 - 1
    ver[0] = end + 1; // начальный элемент - конечная вершина
    int k = 1; // индекс предыдущей вершины
    int weight = d[end]; // вес конечной вершины

    while (end != begin_index) { // пока не дошли до начальной вершины
        for (int i = 0; i<size_ways_matrix; i++) { // просматриваем все вершины
            if (coordinat_map_array[i][end] != 0) {   // если связь есть
                int temp = weight - coordinat_map_array[i][end]; // определяем вес пути из предыдущей вершины
                if (temp == d[i]) { // если вес совпал с рассчитанным значит из этой вершины и был переход
                    weight = temp; // сохраняем новый вес
                    end = i;       // сохраняем предыдущую вершину
                    ver[k] = i + 1; // и записываем ее в массив
                    k++;
                }
            }
        }
    }

    // Вывод пути (начальная вершина оказалась в конце массива из k элементов)

    for (int i = k - 1; i > 0; i--) {
        int x1, y1, x2, y2;
        string signal = "";
        x1 = all_nodes[ver[i] - 1].x;
        y1 = all_nodes[ver[i] - 1].y;
        x2 = all_nodes[ver[i - 1] - 1].x;
        y2 = all_nodes[ver[i - 1] - 1].y;
        if (x1 - x2 > 0) {
            signal += "up";
        }
        if (x1 - x2 < 0) {
            signal += "down";
        }
        if (y1 - y2 > 0) {
            if (signal != "") signal += "-";
            signal += "left";
        }
        if (y1 - y2 < 0) {
            if (signal != "") signal += "-";
            signal += "right";
        }
        if (signal != "-") comand_set.push_back(signal);
    }  cout << endl;

    for (auto str : comand_set) {
        cout << str << endl;
    }

    for (int i = 0; i < size_ways_matrix; i++) {
        delete[] coordinat_map_array[i];
    }
    delete[] coordinat_map_array;

    return comand_set;
}