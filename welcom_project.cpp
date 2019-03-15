#include<iostream>
#include<conio.h>
#include<fstream>
#include<windows.h>
#include<stdio.h>
#include<string.h>
using namespace std;
void gotoxy(int x,int y)
{
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
void title()
{
	gotoxy(40,5);
	cout<<"!!==============================!!";
    gotoxy(40,6);
	cout<<"  LOVELY PROFESSIONAL UNIVERSITY";
    gotoxy(40,7);
	cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
}
int main()
{
	int d,p;
	system("color 60");
	//LOVELY
	gotoxy(40,1);
	cout<<"**       **   *       * ******* **     **     **";
	gotoxy(40,2);
	cout<<"**      *  *   *     *  **      **      **   **";
	gotoxy(40,3);
	cout<<"**     *    *   *   *   *****   **        ***";
	gotoxy(40,4);
	cout<<"**      *  *     * *    **      **         *";
	gotoxy(40,5);
	cout<<"*******  **       *     ******* *******    *";
	//PROFESSIONAL
	gotoxy(20,7);
	cout<<"*****  *****    **   ***** *****  ****   ****  *****   **   **    *     **     **";
	gotoxy(20,8);
	cout<<"**   * **   *  *  *  *     *     *      *        *    *  *  * *   *    *  *    **";
	gotoxy(20,9);
	cout<<"*****  *****  *    * ***   ***    ****   ****    *   *    * *  *  *   *    *   **";
	gotoxy(20,10);
	cout<<"**     ** *    *  *  *     *          *      *   *    *  *  *   * *  ** ** **  **";
	gotoxy(20,11);
	cout<<"**     **   *   **   *     *****  ****   ****  *****   **   *    ** *        * *******";
	//UNIVERSITY
	gotoxy(25,13);
	cout<<"**   ** **    * ***** *       * ***** *****   ****  ***** ***** **     **";
	gotoxy(25,14);
	cout<<"**   ** * *   *   *    *     *  *     **   * *        *     *    **   **";
	gotoxy(25,15);
	cout<<"**   ** *  *  *   *     *   *   ***   *****   ****    *     *      ***";
	gotoxy(25,16);
	cout<<"**   ** *   * *   *      * *    *     ** *        *   *     *       *";
	gotoxy(25,17);
	cout<<"  ***   *    ** *****     *     ***** **   *  ****  *****   *       *";
	gotoxy(45,18);
	cout<<"<--LPU Scholarship-->";
	gotoxy(10,20);
	cout<<"Guided By :- Mr. Vipin Kumar";
	gotoxy(65,21);
	cout<<"Submitted By :- Sahil Kumar Singh";
	gotoxy(65,22);
	cout<<"Reg. No. :- 11713141";
	gotoxy(65,23);
	cout<<"Sec. :- K17YH";
	gotoxy(35,26);
	cout<<"Press ENTER to Continue....!!! :-D";
	getch();
	system("cls");
	abc:;
	title();
	cout<<"What do you want?\n"<<endl;
    cout<<"1. Enter Details"<<endl;
    cout<<"2. Check Details"<<endl;
    cout<<"3. Calculate Scholarship"<<endl;
    cout<<"4. Exit\n";
    cin>>d;
    switch(d)
    {
    	case 1:
    		cout<<"enter";
    		cin>>p;
    		system("cls");
    		goto abc;
	}
	getch();
	return 0;
}

