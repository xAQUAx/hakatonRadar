# Хакатон Радар
# Данная программа с помощью модификации симплекс-метода находит комбинацю траекторных гипотез для глобальной траекторной гипотезы с наибольшим суммарным весом.
# Для запуска программы из IDE требуется, чтобы input_with_weights.csv находился в папке вместе с файлом Source.cpp.
# Для запуска программы с помощью .exe файла требуется, чтобы input_with_weights.csv находился в папке с .exe файлом.

# По функциям:
void readFile(vector<vector<int>>& compability, vector<double>& weights, string filePath) - читает csv файл формата, заданного на хакатоне.

template<typename T> void printVector(const vector<T>& vect) - выводит одномерный массив.

template<typename T> void printMatrixT(const vector<vector<T>>& matrix) - выводит двумерный массив в виде верхней треугольной матрицы.

template<typename T> void printMatrix(const vector<vector<T>>& matrix) - выводит двумерный массив.

vector<vector<int>> countUnCompatibles(vector<vector<int>> matrix) - для каждой вершины графа находятся все вершины графов, не совместные с ней. По итогу получаем массив элементов для каждой вершины, где указан полный список несовместных с ней вершин.

int getCustomNext(const vector<vector<int>>& uncomps, vector<int>& curr_elements, const vector<double>& weights, int place) - функция, которая не используется в данной версии. В предыдущей версии нам не всегда был важен самый лучший из существующих вариант. Мы могли выбрать 2 или 3 или 5 вершину графа по величине оценки, которую мы даём в этой функции.
