#include "Final.h"

void clrscr()
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;
	HANDLE	hConsoleOut;
	COORD	Home = { 0,0 };
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);

	FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}
int inputKey()
{
	if (_kbhit())
	{
		int key = _getch();

		/*if (key == 224)	// special key
		{
			key = _getch();
			return key + 1000;
		}*/

		return key;//+1000;
	}
	else
	{
		return key_none;
	}

	return key_none;
}

void gotoXY(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void TextColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void MoveMouse(int key, int& x, int& y, int upperlimit, int downlimit)
{
	if (key == key_Down && y < upperlimit)
	{
		y += 4;
	}
	if (key == key_Up && y > downlimit)
	{
		y -= 4;
	}
}

void MoveAndEdit(int key, int& x, int& y, int& i, int& leftlimit, int& rightlimit, int& upperlimitleft, int& downlimitleft, int& upperlimitright, int& downlimitright)
{
	if (x == leftlimit && key == key_Down && y < upperlimitleft)
	{
		i++;
		y = 4 * (i + 1);
	}
	if (x == leftlimit && key == key_Up && y > downlimitleft)
	{
		i--;
		x = leftlimit;
		y = 4 * (i + 1);
	}
	if (x == leftlimit && key == key_Right)
	{
		x = rightlimit;
		y = downlimitright;
	}
	if (x == rightlimit && key == key_Down && y < upperlimitright)
	{
		y += 2;
	}

	if (x == rightlimit && key == key_Up && y > downlimitright)
	{
		y -= 2;
	}
	if (x == rightlimit && key == key_Left)
	{
		x = leftlimit;
		i = 0;
		y = 4 * (i + 1);
	}
}



void Menu(Tag& t, Note& n)
{
	clrscr();
	gotoXY(42, 4);
	cout << "ADD A NOTE";
	gotoXY(42, 8);
	cout << "VIEW";
	gotoXY(42, 12);
	cout << "DELETE";
	gotoXY(42, 16);
	cout << "SEARCH";
	gotoXY(42, 20);
	TextColor(4);
	cout << "SAVE CHANGES AND EXIT";
	TextColor(7);
	int x = 42, y = 4;
	int key = inputKey();
	int a = 20, b = 4;
	gotoXY(x, y);
	while (key != 13)
	{
		MoveMouse(key, x, y, a, b);
		gotoXY(x, y);
		key = inputKey();
	}
	if (y == 4)
	{ 
		clrscr();
		n.addANote(t, n.Notes);
		system("pause");
		Menu(t, n);
	}
	else if (y == 8)
	{
		MenuView(t, n);
		system("pause");
		Menu(t, n);
	}
	else if (y == 12) {
		clrscr();
		MenuDelete(t, n);
		system("pause");
		Menu(t, n);
	}
	else if (y == 16) {
		clrscr();
		MenuSearch(t, n);
		system("pause");
		Menu(t, n);
	}
	else {
		n.updateFiles("Tags.txt", "Notes.txt", t, n.Notes);
		exit(0);
	}
}

//class Tag methods
Tag::Tag(const string fileName) {
	ifstream fin;
	fin.open(fileName);
	if (!fin.is_open()) {
		cout << "Can not load data ! Please check resource files and run again!";
		exit(0);
	}
	else {
		string temp;
		getline(fin, temp, '\n');
		while (temp != "/") {
			Tags.push_back(temp);
			getline(fin, temp, '\n');
		}
		//		fin.ignore(1000, '\n');
	}
	fin.close();
}

void Tag::viewAllTags() {
	cout << "All available tags: ";
	for (int i = 0; i < Tags.size() - 1; i++){
		cout << Tags[i] << ", ";
	}
	cout << Tags[Tags.size() - 1] << ".";
}

//class Note methods 
Note::Note(string fileName) {
	ifstream fin;
	fin.open(fileName);
	if (!fin.is_open()) {
		cout << "Can not load data ! Please check resource files and run again!";
		exit(0);
	}
	else {
		string temp;
		vector<string> tempTags;
		getline(fin, temp, '\n');
		while (temp != "/*/*") {
			while (temp != "**") {
				tempTags.push_back(temp);
				getline(fin, temp, '\n');
			}
			getline(fin, temp, '\n');
			Notes.push_back(getNote(tempTags, temp));

			fin.ignore(1000, '\n');
			getline(fin, temp, '\n');
			tempTags.clear();
		}
	}
	fin.close();
}
 
void Note::viewANote() {
	int n;

	cout << "Select a note by its number: \n\n";
	for (int i = 0; i < Notes.size(); i++) {
		cout << i + 1 << ". " << Notes[i].text << '\n';
	}
	cout << "\nYour choice: ";
	cin >> n;
	while (n < 1 || n > Notes.size()) {
		cout << "Out of range!! Input again!! ";
		cin >> n;
	}

	cout << "\n\nNote No. " << n << " : \nTags: ";
	n -= 1;

	for (int i = 0; i < Notes[n].tags.size() - 1; i++) {
		cout << Notes[n].tags[i] << ", ";
	}
	cout << Notes[n].tags[Notes[n].tags.size() - 1] << ".";
	cout << "\n" << Notes[n].text << "\n\n";
}

void Note::viewATagsNotes(Tag t) {
	int n;
	string tmp;
	bool note = 0;

	cout << "Select an available tag by its number: \n";
	for (int i = 0; i < t.Tags.size(); i++) {
		cout << i + 1 << ". " << t.Tags[i] << '\n';
	}
	cout << "\nYour choice: ";
	cin >> n;
	while (n < 1 || n > t.Tags.size()) {
		cout << "Out of range!! Input again!! ";
		cin >> n;
	}
	tmp = t.Tags[n - 1];

	for (auto it = Notes.begin(); it != Notes.end(); it++) {
		for (auto ito = it->tags.begin(); ito != it->tags.end(); ito++) {
			if (tmp == *ito) {
				cout << "\n\nTags: ";

				for (int i = 0; i < it->tags.size() - 1; i++) {
					cout << it->tags[i] << ", ";
				}
				cout << it->tags[it->tags.size() - 1] << ".";
				cout << "\n" << it->text << "\n\n";
				note = 1;
			}
		}
	}
	if (!note)
		cout << "No note with tag <" << tmp << "> found!!\n";
}

void Note::addANote(Tag& t, vector <note>& n) {
	string cnt, tmp;
	vector<string> tmpTags;
	bool duplicate = 0;
	
	cout << "Please enter the content of the note: ";
	getline(cin, cnt, '\n');
	cout << "Now enter its tags: (stop by entering //)";
	getline(cin, tmp, '\n');
	while (tmp != "//") {
		tmpTags.push_back(tmp);
		cin >> tmp;
	}
	n.push_back(getNote(tmpTags, cnt));

//check if new tag is already in vactor Tags 
	for (auto it = tmpTags.begin(); it != tmpTags.end(); it++) {
		for (auto ito = t.Tags.begin(); ito != t.Tags.end(); ito++) {
			if (*ito == *it) {
				duplicate = 1;
				break;
			}
		}
		if (!duplicate)
			t.Tags.push_back(*it);
		else duplicate = 0;
	}
	cout << "New note added successfully!";
} 

void Note::deleteATag(Tag& t, vector <note>& Note) {
	int n;
	string tmp;
	bool tag = 0;

	cout << "Select a tag to delete by its number: \n";
	for (int i = 0; i < t.Tags.size(); i++) {
		cout << i + 1 << ". " << t.Tags[i] << '\n';
	}
	cout << "\nYour choice: ";
	cin >> n;
	while (n < 1 || n > t.Tags.size()) {
		cout << "Out of range!! Input again!! ";
		cin >> n;
	}
	tmp = t.Tags[n - 1];

//erase tag from vector Tags
	vector<string>::iterator it = t.Tags.begin();
	for (int i = 1; i < n; i++) {
		it++;
	}
	t.Tags.erase(it);
	
//erase tag from notes that have it
	for (auto it1 = Note.begin(); it1 != Note.end(); it1++) {
		for (auto it2 = it1->tags.begin(); it2 != it1->tags.end(); it2++) {
			if (tmp == *it2) {
				it = it2;
				tag = 1;
			}
		}
		if(tag)	it1->tags.erase(it);
		tag = 0;
	}

	cout << "Tag deleted successfully!\n";
}

void Note::deleteANote(vector <note>& Note) {
	int n;

	cout << "Select a note to delete by its number: \n";
	for (int i = 0; i < Note.size(); i++) {
		cout << i + 1 << ". " << Note[i].text << '\n';
	}
	cout << "\nYour choice: ";
	cin >> n;
	while (n < 1 || n > Note.size()) {
		cout << "Out of range!! Input again!! ";
		cin >> n;
	}

	vector<note>::iterator it = Note.begin();
	for (int i = 1; i < n; i++) {
		it++;
	}
	Note.erase(it);
	cout << "Note deleted successfully!\n";
}

void Note::search(vector <note> Note) {
	string str;
	bool Found = 0;

	cout << "Enter keyword: ";
	getline(cin, str, '\n');

	for (auto it = Note.begin(); it != Note.end(); it++) {
		size_t found = it->text.find(str);
		if (found != string::npos) {
			Found = 1;
			cout << "\n\nTags: ";

			for (int i = 0; i < it->tags.size() - 1; i++) {
				cout << it->tags[i] << ", ";
			}
			cout << it->tags[it->tags.size() - 1] << ".";
			cout << "\n" << it->text << "\n\n";
		}
	}
	if (!Found) cout << "Nothing with keyword <" << str << "> found!!\n";
}

//update files before exiting
void Note::updateFiles(string fileName1, string fileName2, Tag t, vector <note> n) {
	ofstream fout;
	//update file Tags
	fout.open(fileName1, ios::out | ios::trunc);
	if (!fout.is_open()) {
		cout << "Can not open file !";
		system("pause");
		return;
	}
	else {
		for (auto it = t.Tags.begin(); it != t.Tags.end(); it++) {
			fout << *it << '\n';
		}
		fout << "/";
	}
	fout.close();

	//update file Notes
	fout.open(fileName2, ios::out | ios::trunc);
	if (!fout.is_open()) {
		cout << "Can not open file !";
		system("pause");
		return;
	}
	else {
		for (auto it = n.begin(); it != n.end(); it++) {
			for (auto ito = it->tags.begin(); ito != it->tags.end(); ito++) {
				fout << *ito << '\n';
			}
			fout << "**\n" << it->text << "\n\n";
		}
		fout << "/*/*";
	}
	fout.close();
}


//sub menus

void MenuView(Tag &t, Note& n) {
	clrscr();
	gotoXY(42, 4);
	cout << "View all tags";
	gotoXY(42, 8);
	cout << "View all notes of a tag";
	gotoXY(42, 12);
	cout << "View a note";
	gotoXY(42, 16);
	TextColor(1);
	cout << "Return to Menu ";
	TextColor(7);
	int x = 42, y = 4;
	int a = 28, b = 4;
	int key = inputKey();
	gotoXY(x, y);
	while (key != 13)
	{
		MoveMouse(key, x, y, a, b);
		gotoXY(x, y);
		key = inputKey();
	}
	if (y == 4) {
		clrscr();
		t.viewAllTags();
	}
	else if (y == 8) {
		clrscr();
		n.viewATagsNotes(t);
	}
	else if (y == 12) {
		clrscr();
		n.viewANote();
	}
	else if (y == 16) {
		Menu(t, n);
	}
}

void MenuDelete(Tag& t, Note& n) {
	clrscr();
	gotoXY(42, 4);
	cout << "Delete a Note";
	gotoXY(42, 8);
	cout << "Delete a Tag";
	gotoXY(42, 12);
	TextColor(1);
	cout << "Return to Menu ";
	TextColor(7);
	int x = 42, y = 4;
	int a = 28, b = 4;
	int key = inputKey();
	gotoXY(x, y);
	while (key != 13)
	{
		MoveMouse(key, x, y, a, b);
		gotoXY(x, y);
		key = inputKey();
	}
	if (y == 4) {
		clrscr();
		n.deleteANote(n.Notes);
	}
	else if (y == 8) {
		clrscr();
		n.deleteATag(t, n.Notes);
	}
	else if (y == 12) {
		Menu(t, n);
	}
}

void MenuSearch(Tag& t, Note& n) {
	clrscr();
	n.search(n.Notes);
}