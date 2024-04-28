#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>

using namespace std;

// Чтение csv файла
void readFile(vector<vector<int>>& compability, vector<double>& weights, string filePath) {
	ifstream file(filePath);
	string weights_str;

	string temp;

	getline(file, temp);
	compability.push_back(vector<int>());

	size_t j = 0, i = 0;
	string compability_str = temp + "|";
	temp = "";

	for (i = 0; i < compability_str.size() - 1; i++) {
		if (compability_str[i] != ',')
			compability[j].push_back(compability_str[i] - '0');
	}
	j++; i++;

	size_t times = compability[0].size();

	for (size_t i = 0; i < times - 1; i++) {
		getline(file, temp);
		compability_str += temp + "|";
		temp = "";
	}

	while (!file.eof()) {
		getline(file, temp);
		weights_str += temp;
		temp = "";
	}
	compability.push_back(vector<int>());
	for (; i < compability_str.size(); i++) {
		if (compability_str[i] == '|') {
			compability.push_back(vector<int>());
			j++;
		}
		else if (compability_str[i] != ',')
			compability[j].push_back(compability_str[i] - '0');
	}
	compability.pop_back();
	double tempd;
	for (size_t i = 0; i < weights_str.size(); i++) {
		if (weights_str[i] != ',')
			temp += weights_str[i];
		else if (weights_str[i] != '\n') {
			istringstream(temp) >> tempd;
			weights.push_back(tempd);
			temp = "";
		}
	}
	istringstream(temp) >> tempd;
	weights.push_back(tempd);

	file.close();
}

// Вывести веса элементов
template<typename T>
void printVector(const vector<T>& vect) {
	for (size_t i = 0; i < vect.size(); i++) {
		cout << vect[i] << " ";
	}
}

// Вывести матрицу совместности
template<typename T>
void printMatrixT(const vector<vector<T>>& matrix) {
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			if (j <= i) cout << "  ";
			else cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

template<typename T>
void printMatrix(const vector<vector<T>>& matrix) {
	for (size_t i = 0; i < matrix.size(); i++) {
		cout << i << "\t";
		for (size_t j = 0; j < matrix[i].size(); j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

// Посчитать множества несовместных элементов для каждой вершины графа
vector<vector<int>> countUnCompatibles(vector<vector<int>> matrix) {
	vector<vector<int>> result;
	result.reserve(matrix.size());

	for (size_t i = 0; i < matrix.size(); i++) {
		result.push_back(vector<int>());
		size_t j;
		for (j = 0; j < i; j++) {
			if (!matrix[j][i] && (i != j))
				result[i].push_back(j);
		}
		for (; j < matrix.size(); j++) {
			if (!matrix[i][j] && (i != j))
				result[i].push_back(j);
		}
	}

	return result;
}

// Следующего в очереди
int getCustomNext(const vector<vector<int>>& uncomps, vector<int>& curr_elements,
	const vector<double>& weights, int place) {

	// 1 значение - индекс, 2 значение - коэффициент
	vector<pair<int, double>> top;

	// Для элементов без несовместностей
	vector<int> superTop;

	for (size_t i = 0; i < curr_elements.size(); i++) {
		if (uncomps[curr_elements[i]].size())
			top.push_back(make_pair(i, (1.0 / uncomps[curr_elements[i]].size()) * weights[curr_elements[i]]));
		else superTop.push_back(i);
	}
	sort(top.begin(), top.end(), [](pair<int, double> i, pair<int, double> j) {return i.second > j.second; });
	if (place < superTop.size()) return curr_elements[superTop[place]];
	else return curr_elements[top[place - superTop.size()].first];
}

// Следующего в очереди
int getNext(const vector<vector<int>>& uncomps, vector<int>& curr_elements, const vector<double>& weights) {
	double coeff = INT_MIN;
	int index = 0;
	for (size_t i = 0; i < curr_elements.size(); i++) {
		if (!uncomps[curr_elements[i]].size()) return curr_elements[i];
		double temp_coeff = (1.0 / uncomps[curr_elements[i]].size()) * weights[curr_elements[i]];
		if (coeff < temp_coeff) {
			coeff = temp_coeff;
			index = i;
		}
	}
	return curr_elements[index];
}

void removeIndexes(const vector<vector<int>>& uncomps, const vector<double>& weights, vector<int>& curr_elements, int newEl) {
	vector<int> elementsToDelete, newCurrElements;
	
	elementsToDelete = uncomps[newEl];
	elementsToDelete.push_back(newEl);

	for (size_t i = 0; i < curr_elements.size(); i++) {
		bool add_it = true;
		for (size_t j = 0; j < elementsToDelete.size(); j++) {
			if (elementsToDelete[j] == curr_elements[i]) {
				add_it = false;
				break;
			}
		}
		if (add_it) newCurrElements.push_back(curr_elements[i]);
	}

	curr_elements = newCurrElements;
}

vector<vector<int>> getResult(const vector<vector<int>>& uncomps, const vector<double>& weights) {
	vector<int> input;
	vector<vector<int>> result;
	result.reserve(5);
	input.reserve(uncomps.size());
	for (size_t i = 0; i < uncomps.size(); i++) {
		input.push_back(i);
	}
	
	vector<int> steps;
	steps.reserve(20);

	const vector<int> numbers = input;

	result.push_back(vector<int>());
	while (!input.empty()) {
		int next = getNext(uncomps, input, weights);
		steps.push_back(next);
		result[0].push_back(numbers[next]);
		removeIndexes(uncomps, weights, input, numbers[next]);
	}

	for (size_t i = 1; i < 5; i++) {
		for (size_t j = 0; j < numbers.size(); j++) {
			if (numbers[j] != steps[steps.size() - i - 1])
				input.push_back(numbers[j]);
		}

		result.push_back(vector<int>());
		while (!input.empty()) {
			int next = getNext(uncomps, input, weights);
			steps.push_back(next);
			result[i].push_back(numbers[next]);
			removeIndexes(uncomps, weights, input, numbers[next]);
		}
	}

	return result;
}

int main(int argc, char* argv[]) {
	auto start = std::chrono::high_resolution_clock::now();

	vector<vector<int>> compability;
	vector<double> weights;

	readFile(compability, weights, "input_with_weights.csv");

	vector<vector<int>> setUncomps = countUnCompatibles(compability);

	vector<vector<int>> result = getResult(setUncomps, weights);
	sort(result.begin(), result.end());

	double weight[5]{};

	for (size_t i = 0; i < result.size(); i++) {
		for (size_t j = 0; j < result[i].size(); j++) {
			weight[i] += weights[result[i][j]];
		}
	}

	ofstream ost("out.csv");

	ost << ",TH1,TH2,TH3,TH4,TH5,TH6,TH7,TH8,TH9,TH10,TH11,TH12,TH13,TH14,TH15,TH16,TH17,TH18,TH19,TH20,TH21,TH22,TH23,TH24,TH25,TH26,TH27,TH28,sum(w)\n";
	for (size_t k = 0; k < 5; k++) {
		ost << "GH" << k + 1 << ",";
		for (size_t i = 0; i < setUncomps.size(); i++) {
			bool addZero = true;
			for (size_t j = 0; j < result[k].size(); j++) {
				if (result[k][j] == i) {
					addZero = false;
					break;
				}
			}
			if (addZero) ost << "0";
			else ost << "1";
			if (i < setUncomps.size()) ost << ",";
		}
		ost << weight[k] << endl;
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
	
	return 0;
}