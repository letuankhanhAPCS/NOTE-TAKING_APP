#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include<Windows.h>
#include <stdio.h>
#include <iomanip>
#include <stdio.h>
#include <conio.h>

using namespace std;

#define default_ColorCode		7


#define key_Up		72
#define key_Down	80
#define key_Left	75
#define key_Right	77
#define key_none	-1

class Tag { 
public:
	vector <string> Tags;

	Tag(const string fileName);

	void viewAllTags();
};

class note {
public:
	vector <string> tags;
	string text;

	note(vector <string> ta, string te) {
		tags = ta;
		text = te;
	}
};

class Note{
public:
	vector <note> Notes;

	Note(string fileName); 

	note getNote(vector <string> ta, string te) {
		return note(ta, te);
	}

	void viewANote();
	void viewATagsNotes(Tag t);
	void addANote(Tag& t, vector <note>& n);
	void deleteATag(Tag& t, vector <note>& Note);
	void deleteANote(vector <note>& Note);
	void search(vector <note> Note);

	void updateFiles(string fileName1, string fileName2, Tag t, vector <note> n);
};

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
