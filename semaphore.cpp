#include <iostream>
#include <fstream>
#include <thread>
#include <vector>  
#include <ctime>
#include <chrono>
#include <Windows.h>

using namespace std;

//global handle for semaphore
HANDLE hSemaphore;

typedef std::chrono::duration<double> timet;
#define timestamp() std::chrono::steady_clock::now()
#define time_gap(stop,start) std::chrono::duration<double>(stop-start).count()

std::ofstream outfile("result.txt");

void search(vector <string> lines, string singleword, int start, int end, std::thread::id this_id) {

	for (int j = start; j < end; j++) {
		string singleline = lines[j];
		auto before = timestamp();
		if (singleline.find(singleword) != std::string::npos)
		{
			auto after = timestamp();

			WaitForSingleObject(hSemaphore, INFINITE);

			auto start = std::chrono::high_resolution_clock::now();
			outfile << singleword << " was found in line: " << j << "    by thread: "
				<< this_id << "   Elapsed time:  " << time_gap(after, before) << " s\n";
			auto finish = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = finish - start;
			outfile << "Elapsed time to write output to file: " << elapsed.count() << " s\n";

			ReleaseSemaphore(hSemaphore, 1, 0);
		}
	}
}

void thread_search(vector<string> lines, vector<string> words, int start, int end)
{
	std::thread::id this_id = std::this_thread::get_id();

	for (int i = 0; i < words.size(); i++) {
		search(lines, words[i], start, end, this_id);
	}
}

int main(void)
{
	//create semaphore
	hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
	string line;
	int line_count = 0;
	vector<string> lines;
	vector<string> wordslist;
	string word;

	std::ifstream myfile("file.txt");

	if (!myfile.is_open()) {
		cout << "Unable to open file";
	}

	while (!myfile.eof()) { //or myfile.good()
		getline(myfile, line);
		// cout << line << endl;
		lines.push_back(line);
		line_count++;
	}
	cout << "The input file was scanned\n";
	cout << "\n-> number of lines in text file: " << line_count-- << endl;
	myfile.close();

	cout << "Please enter your set of words you would like to search in the given text file, separated with space and ending with zero \n";
	while (cin >> word && word != "0")
	{
		wordslist.push_back(word);
	}
	 
	auto startthread = std::chrono::high_resolution_clock::now();

	int part = lines.size() / 4;
	thread th[4];

	for (int i = 0; i < 4; i++) {
		int start = i * part;
		int end = (i + 1) * part;

		th[i] = thread(thread_search, lines, wordslist, start, end);
	}
	auto finishthread = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 4; ++i) {
		th[i].join();
	}
	CloseHandle(hSemaphore);

	std::chrono::duration<double> timethread = finishthread - startthread;
	cout << " time four thread : " << timethread.count() * 1000000 << " Microsecond\n";

	cout << "open the file to see result\n\n";

	return 0;
}