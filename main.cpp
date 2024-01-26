
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <locale>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "print_utils.h"

using namespace std;

// Database config
const string data_file = "Data.txt";

// Point config 
const int Good_Point = 8;
const int Normal_Point = 5;

struct Student {
	string _full_name;
	int _age;
	float _score;
	Student* _next = NULL;

	Student() {}

	Student(string full_name, int age, float score) {
		_full_name = full_name;
		_age = age;
		_score = score;
	}
	
	Student(const Student* other) {
		clone_info(other);
	}
	
	void clone_info(const Student* other) {
		if (other) {
			_full_name = other->_full_name;
			_age = other->_age;
			_score = other->_score;
		}
	}
};

struct StudentList {
	Student* _head = NULL;
	
	/////////////////////////////////////////////////////
	/***************** Main function ******************/
	///////////////////////////////////////////////////
	void init()
	{
		ifstream data(data_file);
		string info;

		while (getline(data, info)) {
			const int info_count = 3;
			string student_info[info_count];
			int i = 0;
			string info_delimiter = ";";

			int pos_of_delimiter = 0;
			while ((pos_of_delimiter = info.find(info_delimiter)) != std::string::npos) {
				student_info[i++] = info.substr(0, pos_of_delimiter);
				info = info.substr(pos_of_delimiter + 1);
			}
			student_info[info_count - 1] = info.substr(pos_of_delimiter + 1);

			append(student_info[0],
			        stoi(student_info[1]), 
			        stof(student_info[2]));
		}
		data.close();
	}

	void print() {
		print_row();
		print_row("STT", "Ho Ten", "Tuoi", "Diem");
		print_row();
		
		int order = 0;
		Student* p = _head;
		while (p != NULL){
			print_row(++order, p->_full_name, p->_age, p->_score);
			p = p->_next;		
		}

		if (order == 0) {
			print_row(" ", " ", " ", " ");
		}
		print_row();
	}

	void analyze() {
		int i = 0;
		Student* p = _head;
		if (!p) {
			cout << "No data to analyze\n";
		}

		float average_of_point = 0;
		int num_of_good_student = 0;
		int num_of_normal_student = 0;
		int num_of_bad_student = 0;
		StudentList* best_student_list = new StudentList;
		while (p) {
			i++;
			average_of_point += p->_score / count();
			if (p->_score > Good_Point)
				num_of_good_student++;
			else if (p->_score > Normal_Point && p->_score < Good_Point)
				num_of_normal_student++;
			else
				num_of_bad_student++;
			
			if (!best_student_list->_head) {
				best_student_list->append(p->_full_name, p->_age, p->_score);
			}
			else if (best_student_list->_head->_score <= p->_score) {
				if (best_student_list->_head->_score != p->_score) {
					delete best_student_list;
					best_student_list = new StudentList;
				}
				best_student_list->append(p->_full_name, p->_age, p->_score);
			}
			
			p = p->_next;
		}
		
		cout << "\t(!) Diem trung binh ca lop: " << average_of_point << "\n";
		cout << "\t(!) Ti le sinh vien gioi: " << (float)num_of_good_student / count() * 100 << "%\n";
		cout << "\t(!) Ti le sinh vien kha: " << (float)num_of_normal_student / count() * 100 << "%\n";
		cout << "\t(!) Ti le sinh vien kem: " << (float)num_of_bad_student / count() * 100 << "%\n";
		cout << "\t(!) Sinh vien diem cao nhat (" << best_student_list->_head->_score <<  ") la: \n";
		p = best_student_list->_head;
		while (p) {
			cout << "\t\t" << p->_full_name << "\n";
			p = p->_next;
		}
	}
    
	void append(string name, int age, float score) {
		Student* student = new Student(name, age, score);
		
		if (_head == NULL) {
			_head = student;
			return;
		}

		Student* p = _head;
		while (p->_next != NULL)
			p = p->_next;
		p->_next = student;
	}

	void edit_full_name(int position, string full_name) {
		Student *student = find_by_position(position);
		if (student) {
			student->_full_name = full_name;
		}
	}

	void edit_age(int position, int age) {
		Student* student = find_by_position(position);
		if (student)
			student->_age = age;
	}

	void edit_score(int position, int score) {
		Student* student = find_by_position(position);
		if (student)
			student->_score = score;
	}

	void remove(int position) {
		position--;
		if (!_head) return;

		if (position == 0) {
			_head = _head->_next;
			return;
		}

		int i = 1;
		Student* pPrev = _head;
		Student* p = _head->_next;;

		do {
			if (i == position)
			{
				pPrev->_next = p->_next;
				break;
			}
			pPrev = pPrev->_next;
			p = p->_next;
			i++;
		} while (p);
	}

	Student* find_by_position(int position) {
		int i = 1;
		Student* p = _head;
		while (p != NULL) {
			
			if (i == position) {
				return p;
			}

			p = p->_next;
			i++;
		}

		return NULL;
	}

	StudentList* find_by_word(string word) {
		StudentList* result = new StudentList;
		Student* p = _head;
		while (p != NULL) {
			if (p->_full_name.find(word) != string::npos) {
				result->append(p->_full_name, p->_age, p->_score);
			}

			p = p->_next;
		}
		return result;
	}

	void order_by_name() {
		Student* p = _head;
		while (p) {
			Student* q = p->_next;
			while (q) {
				int i = p->_full_name.length() - 1;
				for (; i >= 0; i--) {
					if (i - 1 >= 0 && p->_full_name[i - 1] == ' '
						|| i == 0)
					{
						break;
					}
				}

				int j = q->_full_name.length() - 1;
				for (; j >= 0; j--) {
					if (j - 1 >= 0 && q->_full_name[j - 1] == ' '
						|| j == 0)
					{
						break;
					}
				}

				if (p->_full_name.substr(i) > q->_full_name.substr(j)) {
					Student* temp = new Student;
					temp->clone_info(p);
					p->clone_info(q);
					q->clone_info(temp);
					delete temp;
				}
				q = q->_next;
			}
			p = p->_next;
		}
		print();
	}

	void order_by_age() {
		Student* p = _head;
		while (p) {
			Student* q = p->_next;
			while (q != NULL) {
				if (p->_age > q->_age) {
					Student* temp = new Student;
					temp->clone_info(p);
					p->clone_info(q);
					q->clone_info(temp);
					delete temp;
				}
				q = q->_next;
			}
			p = p->_next;
		}
		print();
	}

	void order_by_score() {
		Student* p = _head;
		while (p) {
			Student* q = p->_next;
			while (q) {
				if (p->_score > q->_score) {
					Student* temp = new Student;
					temp->clone_info(p);
					p->clone_info(q);
					q->clone_info(temp);
					delete temp;
				}
				q = q->_next;
			}
			p = p->_next;
		}
		print();
	}

	void save() {
		ofstream out(data_file);

		Student* p = _head;
		while (p) {
			out << p->_full_name << ";" << p->_age << ";" << p->_score << "\n";
			p = p->_next;
		}
		out.close();
	}

	int count() {
		int count = 0;
		Student* p = _head;
		while (p) {
			count++;
			p = p->_next;
		} 
		return count;
	}
};

int main()
{
	StudentList list;
	list.init();

	string input;
	while (true)
	{
		cout << "Danh sach thao tac: \n" \
			<< "1 - In danh sach SV\n" \
			<< "2 - Them\n" \
			<< "3 - Sua\n" \
			<< "4 - Xoa\n" \
			<< "5 - Tim kiem\n" \
			<< "6 - Sap xep\n" \
			<< "7 - Thong ke\n" \
			<< "8 - Sao luu\n" \
			<< "9 - Thoat\n";
		cout << "Nhap lenh: ";
		cin >> input;
		if (input == "1")
		{
			cout << "[1] In danh sach sinh vien: \n";
			list.print();
		}
		else if (input == "2")
		{
			cout << "[2] Them sinh vien \n";
			
			std::string full_name;
			cout << "\t(?) Nhap ten: ";		
			cin.ignore();
			getline(cin, full_name);
            
            int age;
			cout << "\t(?) Nhap tuoi: ";
			cin >> age;
            
            int score;
			cout << "\t(?) Nhap diem: ";
			cin >> score;

			cout << "\t(!) Da them sinh vien co ten: " << full_name 
				<< " , tuoi: " << age 
				<< " , diem: " << score <<  "\n";

			list.append(full_name, age, score);
		}
		else if (input == "3")
		{
			cout << "[3] Sua thong tin sinh vien \n";

			int id = 0;
			cout << "\t(?) Nhap STT cua sinh vien can sua: ";
			cin >> id;

			if (id > list.count() || id < 0)
			{
				cout << "\t(!) STT ko hop le\n";
			}
			else
			{
				cout << "\t(!) Ban muon sua gi:\n\t1 - Ten\n\t2 - Tuoi\n\t3 - Diem\n";
				cout << "\t(?) Chon: ";
				cin >> input;
				if (input == "1")
				{
					cout << "\t(?) Nhap ten: ";
					string fullName;
					cin.ignore();
					getline(cin, fullName);
					list.edit_full_name(id, fullName);
				}
				else if (input == "2")
				{
					cout << "\t(?) Nhap tuoi: ";
					int age;
					cin >> age;
					list.edit_age(id, age);
				}
				else if (input == "3")
				{
					cout << "\t(?) Nhap diem: ";
					int score;
					cin >> score;
					list.edit_score(id, score);
				}
				else
				{
					cout << "\t(!) Khong ton tai lenh nay\n";
				}
				
			}
		}
		else if (input == "4")
		{
			cout << "[4] Xoa sinh vien \n";
			cout << "\t(?) Nhap STT: ";
			
			int id;
			cin >> id;
			if (id > list.count() || id < 0)
			{
				cout << "\t(!) STT ko hop le\n";
			}
			else
			{
				list.remove(id);
				cout << "\t(!) Da xoa sinh vien co STT la " << id << "\n";
			}
		}
		else if (input == "5")
		{
			cout << "[5] Tim kiem \n";
			cout << "\t(?) Nhap ki tu: ";
			string word;
			cin.ignore();
			getline(cin, word);
			StudentList* result = list.find_by_word(word);
			result->print();
			delete result;
		}
		else if (input == "6")
		{
			cout << "[6] Sap xep \n";
			cout << "\t(!) Sap xep the nao:\n " \
				<<	"\t(!) 1 - Sap xep theo ten ABC\n" \
				<<	"\t(!) 2 - Sap xep theo tuoi tang dan\n" \
				<<	"\t(!) 3 - Sap xep theo diem tang dan\n";
			cout << "\t(?) Chon: ";
			cin >> input;
			if (input == "1")
			{
				list.order_by_name();
			}
			else if (input == "2")
			{
				list.order_by_age();
			}
			else if (input == "3")
			{
				list.order_by_score();
			}
			else
			{
				cout << "\t(!) Khong ton tai lenh nay\n";
			}
		}
		else if (input == "7")
		{
			cout << "[7] Thong ke \n";
			list.analyze();
		}
		else if (input == "8")
		{
			cout << "[8] Luu thong tin \n\t(!) Dang luu du lieu: ";
			for (int i = 0; i < 50; i++)
			{
				cout << "|";
				sleep(0.5);
			}
			list.save();
			cout << "\n\t(!) Da luu du lieu thanh cong\n";
		}
		else if (input == "9")
		{
			cout << "[9] Thoat ";
			return 0;
		}
		else
		{
			cout << "Khong ton tai lenh nay\n";
		}

		cout << "Press any key to continue...";
		cin.ignore();
        getline(cin, input);
		system("clear");
	}
	list.save();
	return 0;
}