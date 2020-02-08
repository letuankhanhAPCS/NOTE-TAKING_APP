Email: ltkhanh18@apcs.vn
Student ID: 18125087
Name: Le Tuan Khanh

+List all fucntions:
void viewAllTags();
Tag(const string fileName);
note(vector <string> ta, string te);
Note(string fileName); 
note getNote(vector <string> ta, string te);
void viewANote();
void viewATagsNotes(Tag t);
void addANote(Tag& t, vector <note>& n);
void deleteATag(Tag& t, vector <note>& Note);
void deleteANote(vector <note>& Note);
void search(vector <note> Note);
void updateFiles(string fileName1, string fileName2, Tag t, vector <note> n);
void clrscr();
int inputKey(); 
void gotoXY(int column, int line); 
void TextColor(int color);
void MoveMouse(int key, int& x, int& y, int upperlimit, int downlimit);
void MoveAndEdit(int key, int& x, int& y, int& i, int& leftlimit, int& rightlimit, int& upperlimitleft, int& downlimitleft, int& upperlimitright, int& downlimitright);
void Menu(Tag& t, Note& n);
void MenuView(Tag& t, Note& n);
void MenuDelete(Tag& t, Note& n);
void MenuSearch(Tag& t, Note& n);

+List of OOP aspects that have been implemented in my project:
- Object
- Class
- Encapsulation
