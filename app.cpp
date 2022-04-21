#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"
#include	<string.h>
#include	<fstream>
#include	<algorithm>
#include	<iomanip>



using namespace std;

void checkstudent();
void confirmation();
string convertToString(char*, int);
bool isNumber(const string&);
ifstream& GotoLine(ifstream&, int); 
void linecountinfile(string);
void writelisttofile(string, List, LibStudent);
bool Redundant(List, LibStudent);
bool checklistempty(List);
bool printList(List);
bool ReadFile(string, List*);
bool DeleteRecord(List*, char*);
bool Display(List*, int, int);
bool InsertBook(string, List*);
bool SearchStudent(List*, char* id, LibStudent&);
bool computeAndDisplayStatistics(List*);
bool printStuWithSameBook(List*, char*);
bool displayWarnedStudent(List*, List*, List*);
int menu();

LibStudent student, stu;
LibBook book[100];
List student_list, list1, list2;
Date dateborrow[100], datedue[100];
string author_name[500];
int stunum = 0, outcome = -1, line_count = 0, tempo = 0, storenum, source = 0, detail = 0, dupcount = 0, loop = 0, stuid_length = 0, repeat = 0, options;
char id, quit = 'N', callNum[20];

int main()
{

	do
	{
		menu();

		if (options == 1)
		{
			if (ReadFile("student.txt", &student_list) == true)
			{
				printList(student_list);
				cout << endl << stunum << " new records have been successfully read." << endl << endl;

				if (dupcount > 0)
				{
					if (dupcount == 1)
					{
						cout << "There are " << dupcount << " duplicate record found!" << endl;
					}

					else
					{
						cout << "There are " << dupcount << " duplicate records found!" << endl;
					}

				}
			}
		}

		else if (options == 2)
		{

			stuid_length = 0;
			if (DeleteRecord(&student_list, &id) == false)
			{
				if (loop != 5 && stuid_length == 1)
				{
					cout << "Error! You had enter invalid student id!" << endl;
				}
			}
			
		}

		else if (options == 3)
		{	
			stuid_length = 0;
			if (SearchStudent(&student_list, &id, stu) == true)
			{
				stu.print(cout);
			}
			else
			{
				if (loop != 5 && stuid_length == 1)
				{
					cout << endl << "Error! You had enter invalid ID! Please try again!" << endl;
				}

			}
		}

		else if (options == 4)
		{

			InsertBook("book.txt", &student_list);
			cout << "Books inserted successfully to student list.\n\n" << endl;
		}

		else if (options == 5)
		{
			source = 0;
			detail = 0;
			cout << "Where do you want to display the output (1 - File / 2 - Screen): ";
			cin >> source;

			cout << "Do you want to display book list for every student (1 - YES / 2 - NO): ";
			cin >> detail;
			cout << endl;

			Display(&student_list, source, detail);
			
		}

		else if (options == 6)
		{
			
			computeAndDisplayStatistics(&student_list);
		}

		else if (options == 7)
		{
			cout << "Which call number?: ";
			cin >> callNum;
			cout << callNum << endl;
			printStuWithSameBook(&student_list, callNum);
		}

		else if (options == 8)
		{
			displayWarnedStudent(&student_list, &list1, &list2);
		}

		else if (options == 9)
		{
			quit = 'Y';
		}

		else 
		{
			cout << "Error! Please enter valid option!" << endl;
		}
	


	} while (quit != 'Y');

	cout << "\n\n";
	return 0;
}

bool ReadFile(string filename, List* list)
{
	string temp;
	int count = 1;

	ifstream read(filename);

	stunum = 0;
	read.seekg(0, ios_base::beg);

	if (!read.fail())
	{
		dupcount = 0;
		char tmp;
		while (!read.eof())
		{
			read >> temp; // read from file
			read >> tmp;
			read >> temp;
			read >> student.id; // read into LibStudent struct
			getline(read, temp, '=');
			read.ignore();
			read.getline(student.name, 30);
			getline(read, temp, '=');
			read >> student.course;
			getline(read, temp, '=');
			read >> student.phone_no;

			if (tmp == '=')
			{
				if (!Redundant(student_list, student)) // check duplicate
				{
					student_list.insert(1, student); // insert into list, better performance compare insert to the last node
					count++;
					stunum++; // count student number
				}
				else if (Redundant(student_list, student) == true)
				{
					dupcount++;
				}

			}
		}
		read.close();

		return true;
	}

	else // cant find or open the found
	{
		cout << "Error: File does not exist or cannot be found :(" << endl;
		return false;
	}
}

bool Redundant(List list, LibStudent p)
{
	Node* cur; //*pre
	int start = 1;

	//check if list is empty
	if (list.empty())
	{
		return false;
	}

	//assign cur to first node
	cur = list.head;
	
	//go through each node in list
	while (cur != NULL)
	{
		//operation - check if cur item name is the same with p name
		if (cur->item.compareName2(p)) //if return true mean same name
		{
			return true;
		}

		//move cur to next node
		cur = cur->next;

		start++;
	}

	return false; //cannot find any node with same name as p - thus return false


}

bool DeleteRecord(List* list, char* id)
{
	int checkname = 0;
	char user;
	string stuid;
	type temp;

	loop = 0;

	do
	{
		if (loop == 0)
		{
			cout << "Please key in the student id you want to delete: ";
			cin >> stuid;
		}

		else if (loop >= 1 && loop < 5)
		{
			cout << "Error! Please try again!" << endl << endl;
			cout << "Please key in the student id you want to delete: ";
			cin >> stuid;
		}

		else if (loop == 5) //When user stuck in error for 5 times, prompt user to quit
		{
			cout << endl << "Seem like you are facing some problem here, do you want to quit to main menu?:\n(Y) or (N)o\n";
			cin >> user;
			user = toupper(user);
			if (user == 'Y')
			{
				break;
			}
			else if (user == 'N')
			{
				loop = -1;
			}

			else
			{
				cout << "Error! You had enter invalid option! please try again!" << endl;
			}

		}
		loop++;

	} while (stuid.length() != 7);

	checklistempty(student_list); // check whether list is empty or not

	for (int i = 1; i <= student_list.size(); i++)
	{
		if (!student_list.get(i, temp))
		{
			cout << "\n\nCannot get value in node " << i << "\n\n";
			return false;
		}

		if (stuid == temp.id)
		{
			checkname++;
			temp.print(cout);
			confirmation();
			if (outcome == 1)
			{
				student_list.remove(i);
				writelisttofile("student.txt", student_list, student);
				stunum--;
				cout << "Successfully deleted!" << endl;
				break;
				return true;
			}

		}


	}
	if (checkname == 0)
	{
		stuid_length = 1;
		return false;
	}
	else
	{
		return true;
	}
}

bool SearchStudent(List* list, char* id, LibStudent& stu)
{
	int loop = 0;
	char user;
	string stuid;
	type temp;

	do
	{
		if (loop == 0)
		{
			cout << "Please key in the student id you want to search: ";
			cin >> stuid;
		}

		else if (loop >= 1 && loop < 5)
		{
			cout << endl << "Error! Please try again!" << endl;
			cout << "Please key in the student id you want to search: ";
			cin >> stuid;
		}

		else if (loop == 5)
		{
			cout << "Seem like you are facing some problem here, do you want to quit to main menu?:\n(Y) or (N)o\n";
			cin >> user;
			user = toupper(user);
			if (user == 'Y')
			{
				break;
			}
			else if (user == 'N')
			{
				loop = -1;
			}

			else
			{
				cout << "Error! You had enter invalid option! Please try again!" << endl;
			}
		}
		loop++;

	} while (stuid.length() != 7);

	for (int i = 1; i <= student_list.size(); i++)
	{
		if (!student_list.get(i, temp))
		{
			cout << "\n\nCannot get value in node " << i << "\n\n";
			return false;
		}

		if (stuid == temp.id) //when id key in by user same with the list
		{
			stu = temp;
			return true;

		}
		
	}

	stuid_length = 1;
	return false;
}

bool InsertBook(string filename, List* list)
{
	
	string stuid,checkauthor, borrow_day, due_day, borrow_month, borrow_year, due_month, due_year, random;
	double due_date;
	Date book_borrow, book_due;
	int n = 0, numauthor = 0, duejulian, duepart1, duepart2, duepart3;
	type temp;

	ifstream read(filename);

	if (read.fail())
	{
		cout << "Error: File does not exist or cannot be found :(" << endl;
		return false;
	}
	
	if (repeat == 0)
	{
		read.seekg(0, ios::beg);
		while (!read.eof())
		{
			read >> stuid >> checkauthor >> book[tempo].title >> book[tempo].publisher >> book[tempo].ISBN >> book[tempo].yearPublished >> book[tempo].callNum;

			getline(read, borrow_day, '/');
			getline(read, borrow_month, '/');
			getline(read, borrow_year, ' ');
			getline(read, due_day, '/');
			getline(read, due_month, '/');
			getline(read, due_year);

			dateborrow[tempo].day = stoi(borrow_day); //convert and store for later assign
			dateborrow[tempo].month = stoi(borrow_month);
			dateborrow[tempo].year = stoi(borrow_year);
			datedue[tempo].day = stoi(due_day);
			datedue[tempo].month = stoi(due_month);
			datedue[tempo].year = stoi(due_year);

			int author_length = checkauthor.length();

			for (int i = 0; i < author_length; i++) //replace (_) in author name to ( )
			{

				if (checkauthor[i] == '_')
				{
					checkauthor[i] = ' ';
				}

			}


			for (int count = 0; count < author_length; count++) //check for multiple author name and store
			{

				if (checkauthor[count] == '/')
				{
					book[tempo].author[n] = (char*)author_name[storenum].c_str();
					storenum++;
					n++;

				}
				else
				{
					author_name[storenum] = author_name[storenum] + checkauthor[count];
				}


			}
			book[tempo].author[n] = (char*)author_name[storenum].c_str();
			storenum++;

			for (int i = 0; i < 10; i++)
			{
				author_name[i] = ""; //reset the temporary variable
			}

			int title_length = strlen(book[tempo].title);

			for (int i = 0; i < title_length; i++)
			{

				if (book[tempo].title[i] == '_')
				{
					book[tempo].title[i] = ' ';
				}

			}

			int publisher_length = strlen(book[tempo].publisher);

			for (int i = 0; i < publisher_length; i++)
			{

				if (book[tempo].publisher[i] == '_')
				{
					book[tempo].publisher[i] = ' ';
				}

			}

			duepart1 = (14 - datedue[tempo].month) / 12;
			duepart2 = datedue[tempo].month + (12 * duepart1) - 3;
			duepart3 = datedue[tempo].year + 4800 - duepart1;
			duejulian = datedue[tempo].day + (((153 * duepart2) + 2) / 5) + (365 * duepart3) + (duepart3 / 4) - (duepart3 / 100) + (duepart3 / 400) - 32045;

			due_date = (2458938 - duejulian);

			if (due_date > 0)
			{
				book[tempo].fine = due_date * 0.5;
			}
			else
			{
				book[tempo].fine = 0;
			}

			for (int j = 1; j <= stunum; j++) //insert into student list and corresponding node
			{
				LibStudent gg = student_list.find(j)->item;
				if (stuid == gg.id)
				{
					int numbook = gg.totalbook;

					for (int i = 0; i < 20; i++)
					{
						gg.book[numbook].callNum[i] = book[tempo].callNum[i];
					}

					for (int i = 0; i < 250; i++)
					{
						gg.book[numbook].title[i] = book[tempo].title[i];
					}

					for (int i = 0; i < 10; i++)
					{
						gg.book[numbook].author[i] = book[tempo].author[i];
					}

					for (int i = 0; i < 30; i++)
					{
						gg.book[numbook].publisher[i] = book[tempo].publisher[i];
					}

					for (int i = 0; i < 11; i++)
					{
						gg.book[numbook].ISBN[i] = book[tempo].ISBN[i];
					}

					gg.book[numbook].yearPublished = book[tempo].yearPublished;
					gg.book[numbook].borrow = dateborrow[tempo];
					gg.book[numbook].due = datedue[tempo];
					gg.book[numbook].fine = book[tempo].fine;
					gg.totalbook++;

					student_list.find(j)->item = gg;

				}
			}

			n = 0;
			tempo++;

		}

		read.close();

		for (int i = 1; i <= stunum; i++)
		{
			LibStudent gg = student_list.find(i)->item;
			gg.calculateTotalFine();
			student_list.set(i, gg);
			student_list.find(i)->item = gg;
		}

	}
	repeat = 1;
	return true;
	

}

bool Display(List* list, int source, int detail)
{
	type temp, random;

	ofstream write1("student_booklist.txt", ios::trunc);
	ofstream write2("student_info.txt", ios::trunc);


	if (source == 1) //write into file
	{
		if (detail == 1) 
		{
			for (int x = 1; x <= student_list.size(); x++)
			{
				if (!student_list.get(x, temp))
				{
					cout << "\n\nCannot get value in node " << x << "\n\n";
					return false;
				}
				write1 << "STUDENT " << x;
				temp.print(write1);
				write1 << endl << "BOOK LIST:" << endl << endl;
				LibStudent gg = student_list.find(x)->item;
				for (int i = 0; i < gg.totalbook; i++) //for multiple book
				{
					write1 << "Book " << i + 1 << endl;
					gg.book[i].print(write1);
					write1 << endl;
				}

				for (int i = 0; i < 100; i++)
				{
					write1 << "*";
				}
				write1 << endl;
				write1 << endl;

			}
			cout << "Successfully display output to student_booklist.txt" << endl;
		}

		else if (detail == 2)
		{
			for (int i = 1; i <= student_list.size(); i++)
			{
				if (!student_list.get(i, temp))
				{
					cout << "\n\nCannot get value in node " << i << "\n\n";
					return false;
				}
				write2 << "STUDENT " << i;
				temp.print(write2);
				write2 << endl;
				for (int i = 0; i < 100; i++)
				{
					write2 << "*";
				}
				write2 << endl;
				write2 << endl;

			}
			cout << "Successfully display output to student_info.txt" << endl;
		}
		else
		{
			cout << "You had enter invalid options! Please try again" << endl;
		}
	}

	else if (source == 2) //display to user
	{
		if (detail == 1)
		{
			for (int x = 1; x <= student_list.size(); x++)
			{
				if (!student_list.get(x, temp))
				{
					cout << "\n\nCannot get value in node " << x << "\n\n";
					return false;
				}
				cout << "STUDENT " << x;
				temp.print(cout);
				cout << endl << "BOOK LIST:" << endl << endl;
				LibStudent gg = student_list.find(x)->item;
				for (int i = 0; i < gg.totalbook; i++)
				{
					cout << "Book " << i + 1 << endl;
					gg.book[i].print(cout);
					cout << endl;
				}

				for (int i = 0; i < 100; i++)
				{
					cout << "*";
				}
				cout << endl;
				cout << endl;

			}

		}
		else if (detail == 2)
		{

			for (int i = 1; i <= student_list.size(); i++)
			{
				if (!student_list.get(i, temp))
				{
					cout << "\n\nCannot get value in node " << i << "\n\n";
					return false;
				}
				cout << "STUDENT " << i;
				temp.print(cout);
				cout << endl;
				for (int i = 0; i < 100; i++)
				{
					cout << "*";
				}
				cout << endl;
				cout << endl;

			}
		}
		else
		{
			cout << "You had enter invalid options! Please try again" << endl;
		}
	}

	else
	{
		cout << "You had enter invalid options! Please try again" << endl;
		return false;
	}

	return true;


}

bool computeAndDisplayStatistics(List* list)
{
	int cs = 0, ia = 0, ib = 0, cn = 0, ct = 0;
	int cs_borrow = 0, ia_borrow = 0, ib_borrow = 0, cn_borrow = 0, ct_borrow = 0;
	double cs_total_fine = 0, ia_total_fine = 0, ib_total_fine = 0, cn_total_fine = 0, ct_total_fine = 0;
	int cs_total_overdue = 0, ia_total_overdue = 0, ib_total_overdue = 0, cn_total_overdue = 0, ct_total_overdue = 0;
	type temp;


	if (checklistempty(student_list))
	{

		for (int x = 1; x <= student_list.size(); x++)
		{
			if (!student_list.get(x, temp))
			{
				cout << "\n\nCannot get value in node " << x << "\n\n";
				return false;
			}

			LibStudent gg = student_list.find(x)->item;

			string random = gg.course;
			if (random == "CS") //check the course name
			{
				cs++;
				cs_borrow += gg.totalbook;
				cs_total_fine += gg.total_fine;

				for (int i = 0; i < gg.totalbook; i++)
				{
					if (gg.book[i].fine > 0)
					{
						cs_total_overdue++;
					}
				}


			}
			else if (random == "IA")
			{
				ia++;
				ia_borrow += gg.totalbook;
				ia_total_fine += gg.total_fine;
				for (int i = 0; i < gg.totalbook; i++)
				{
					if (gg.book[i].fine > 0)
					{
						ia_total_overdue++;
					}
				}

			}
			else if (random == "IB")
			{
				ib++;
				ib_borrow += gg.totalbook;
				ib_total_fine += gg.total_fine;
				for (int i = 0; i < gg.totalbook; i++)
				{
					if (gg.book[i].fine > 0)
					{
						ib_total_overdue++;
					}
				}

			}
			else if (random == "CN")
			{
				cn++;
				cn_borrow += gg.totalbook;
				cn_total_fine += gg.total_fine;
				for (int i = 0; i < gg.totalbook; i++)
				{
					if (gg.book[i].fine > 0)
					{
						cn_total_overdue++;
					}
				}

			}
			else
			{
				ct++;
				ct_borrow += gg.totalbook;
				ct_total_fine += gg.total_fine;
				for (int i = 0; i < gg.totalbook; i++)
				{
					if (gg.book[i].fine > 0)
					{
						ct_total_overdue++;
					}
				}

			}

		}
		cout << "_____________________________________________________________________________________________________" << endl;
		cout << "| Course | Number of Students | Total Books Borrowed | Total Overdue Books | Total Overdue Fine(RM) |" << endl;
		cout << "-----------------------------------------------------------------------------------------------------" << endl;
		cout << "|   CS   |        " << cs << "           |           " << cs_borrow << "          |          " << cs_total_overdue << "          |          " << setprecision(2) << fixed << cs_total_fine << "          |" << endl;
		cout << "-----------------------------------------------------------------------------------------------------" << endl;
		cout << "|   IA   |        " << ia << "           |           " << ia_borrow << "          |          " << ia_total_overdue << "          |          " << ia_total_fine << "          |" << endl;
		cout << "-----------------------------------------------------------------------------------------------------" << endl;
		cout << "|   IB   |        " << ib << "           |           " << ib_borrow << "          |          " << ib_total_overdue << "          |          " << ib_total_fine << "          |" << endl;
		cout << "-----------------------------------------------------------------------------------------------------" << endl;
		cout << "|   CN   |        " << cn << "           |           " << cn_borrow << "          |          " << cn_total_overdue << "          |          " << cn_total_fine << "          |" << endl;
		cout << "-----------------------------------------------------------------------------------------------------" << endl;
		cout << "|   CT   |        " << ct << "           |           " << ct_borrow << "          |          " << ct_total_overdue << "          |          " << ct_total_fine << "          |" << endl;
		cout << "_____________________________________________________________________________________________________" << endl;

		return true;
	}

	else
	{
		return false;
	}

	
}

bool printStuWithSameBook(List* list, char* callNum)
{
	int samebook = 0;
	type temp;
	List  book_list;

	if (checklistempty(student_list))
	{
		for (int x = 1; x <= student_list.size(); x++)
		{
			if (!student_list.get(x, temp))
			{
				cout << "\n\nCannot get value in node " << x << "\n\n";
				return false;
			}

			LibStudent gg = student_list.find(x)->item;


			for (int i = 0; i < gg.totalbook; i++)
			{
				string random = gg.book[i].callNum;
				if (random == callNum) //if the user input callNum same with the one in the list
				{
					if (!Redundant(book_list, gg)) //Prevent duplicate record
					{
						book_list.insert(1, gg); //insert into list
						samebook++;
					}

				}
			}

		}
		if (samebook > 1)
		{
			cout << endl << "There are " << samebook << " students that borrow the book with call number " << callNum << " as shown below:"
				<< endl << endl;
		}
		else
		{
			cout << endl << "There are " << samebook << " student that borrow the book with call number " << callNum << " as shown below:"
				<< endl << endl;
		}


		for (int j = 1; j <= book_list.size(); j++) //access the list and print out
		{
			if (!book_list.get(j, temp))
			{
				cout << "\n\nCannot get value in node " << j << "\n\n";
				return false;
			}

			cout << "Student Id = " << temp.id << endl;
			cout << "Name = " << temp.name << endl;
			cout << "Course = " << temp.course << endl;
			cout << "Phone Number = " << temp.phone_no << endl;

			for (int i = 0; i < temp.totalbook; i++)
			{
				string random = temp.book[i].callNum;
				if (random == callNum)
				{
					cout << "Borrow Date: ";
					temp.book[i].borrow.print(cout);
					cout << endl;
					cout << "Due Date: ";
					temp.book[i].due.print(cout);
					cout << endl;
					cout << endl;
					break;
				}
			}
		}
		return true;
	}

	else
	{
		return false;
	}

}

bool displayWarnedStudent(List* list, List* type1, List* type2)
{
	type temp;
	int due_count = 0, all_due = 0;

	if (checklistempty(student_list))
	{
		for (int x = 1; x <= student_list.size(); x++)
		{
			if (!student_list.get(x, temp))
			{
				cout << "\n\nCannot get value in node " << x << "\n\n";
				return false;
			}

			LibStudent gg = student_list.find(x)->item;

			for (int i = 0; i < gg.totalbook; i++)
			{
				int day = gg.book[i].fine / 0.5;
				if (day >= 10)
				{
					due_count++;
					all_due++;
				}
				else if (day > 0 && day < 10)
				{

					all_due++;
				}


			}

			int totalfine = gg.total_fine;
			int totalbook = gg.totalbook;
			if (totalfine > 50)
			{
				if (all_due == totalbook)
				{
						list2.insert(1, gg);
				}
			}

			if (due_count > 2) //if at least 3 book overdue more than or equal to 10 days
			{
				list1.insert(1, gg);
			}

			

			due_count = 0;
			all_due = 0;

		}
		if (list1.size() == 0 && list2.size() == 0) //check whether the list is empty
		{
			cout << "The list 1 and list 2 are empty!" << endl;
			return false;
		}

		
		if (list1.size() == 0) 
		{
			cout << "\n\nThe list 1 is empty! " << "\n\n";
		}

		else 
		{
			cout << "List 1" << endl << endl;
			for (int x = 1; x <= list1.size(); x++)
			{
				list1.get(x, temp);
				
				cout << "STUDENT " << x;
				temp.print(cout);
				cout << endl << "BOOK LIST:" << endl << endl;

				int totalbook = temp.totalbook;
				for (int i = 0; i < totalbook; i++)
				{
					cout << "Book " << i + 1 << endl;
					temp.book[i].print(cout);
					cout << endl;
				}

				for (int i = 0; i < 100; i++)
				{
					cout << "*";
				}
				cout << endl;
				cout << endl;

			}
		}

		if (list2.size() == 0)
		{
			cout << "\n\nThe list 2 is empty! " << "\n\n";
		}

		else 
		{
			cout << "List 2" << endl << endl;
			for (int x = 1; x <= list2.size(); x++)
			{

				list2.get(x, temp);

				cout << "STUDENT " << x;
				temp.print(cout);
				cout << endl << "BOOK LIST:" << endl << endl;

				int totalbook = temp.totalbook;
				for (int i = 0; i < totalbook; i++)
				{
					cout << "Book " << i + 1 << endl;
					temp.book[i].print(cout);
					cout << endl;
				}

				for (int i = 0; i < 100; i++)
				{
					cout << "*";
				}
				cout << endl;
				cout << endl;

			}
		}

		
		return true;
	}

	else
	{
		return false;
	}



}

int menu()
{
	cout <<
		R"(
		Menu
		1. Read file.
		2. Delete record.
		3. Search student.
		4. Insert book.
		5. Display output
		6. Compute and Display Statistics
		7. Student with Same Book
		8. Display Warned Student
		9. Exit.
		Enter your choice :)";

	cin >> options;
	cout << endl;

	return options;
}

bool printList(List list) //print list
{
	type temp;

	for (int i = 1; i <= list.size(); i++)
	{
		if (!list.get(i, temp))
		{
			cout << "\n\nCannot get value in node " << i << "\n\n";
			return false;
		}

		temp.print(cout);
	}

	return true;
} 

bool checklistempty(List list) //check list empty
{
	if (list.empty())
	{
		cout << "\n\nCannot print empty list. \n\n";
		return false;
	}

	return true;

}

void confirmation() //Function to prompt user to make decision
{
	char yorn;

	do 
	{
		cout << "\nAre you confirm with your decision?:\n(Y) or (N)o\n ";
		cin >> yorn;
		yorn = toupper(yorn);

		if (yorn == 'Y')
		{
			outcome = 1;
			cout << "Successfully update your decision!" << endl;
		}

		else if (yorn == 'N')
		{
			outcome = 0;
			cout << "Your update had been cancelled!" << endl;
		}

		else
		{
			outcome = -1;
			cout << "Error! You had enter invalid option! please try again!" << endl;
		}
	} while (outcome == -1);


}

void writelisttofile(string filename, List list, LibStudent p) //insert the list into file
{

	ofstream write(filename, ios::trunc);

	type temp;

	
	for (int i = 1; i <= student_list.size(); i++)
	{
		if (write.is_open())
		{
			if (student_list.get(i, temp))
			{
				if (i == student_list.size())
				{
					write << "Student Id = ";
					write << temp.id;
					write << endl;
					write << "Name = ";
					write << temp.name;
					write << endl;
					write << "course = ";
					write << temp.course;
					write << endl;
					write << "Phone Number = ";
					write << temp.phone_no;
				}
				
				else
				{
					write << "Student Id = ";
					write << temp.id;
					write << endl;
					write << "Name = ";
					write << temp.name;
					write << endl;
					write << "course = ";
					write << temp.course;
					write << endl;
					write << "Phone Number = ";
					write << temp.phone_no << endl;
					write << endl;
					write << endl;
				}


			
			}

			

		}
	}

}

ifstream& GotoLine(ifstream& file, int num) //Go to specific line
{
	file.seekg(ios::beg);
	for (int i = 0; i < num - 1; ++i) 
	{
		file.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return file;
}

bool isNumber(const string& str) //check whether is number
{
	return str.find_first_not_of("0123456789") != string::npos;
}

void linecountinfile(string filename) //Count file line number
{
	ifstream read(filename, ios::in);

	string random;

	read.seekg(0, ios_base::beg);

	line_count = 0;

	if (read.is_open())
	{
		while (read.peek() != EOF)
		{
			read >> random;;
			if (isNumber(random) == false)
			{
				getline(read, random);
				line_count++;
			}

		}
		read.close();
	}
}

string convertToString(char* a, int size)
{
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

