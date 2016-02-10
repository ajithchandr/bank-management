//**************************************
// Name: BankingManagement
// By: c.s.e-A
//**************************************
#include<dos.h>
#include<fstream.h>
#include<iomanip.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<conio.h>
const char BELL = '\a';
enum boolean {FALSE,TRUE};
void clrBox(int left, int top, int right, int bottom)
{
	window(left,top,right,bottom);
	clrscr();
	window(1,1,80,25);
}
void drawBox(int left, int top, int right, int bottom)
{
	gotoxy(left,top);	putch( char(201) );
	gotoxy(right,top); putch( char(187) );
	gotoxy(left,bottom);putch( char(200) );
	gotoxy(right,bottom);putch( char(188) );
	for(int i=left+1; i<right; i++)
	{
		gotoxy(i,top);	putch( char(205) );
		gotoxy(i,bottom);putch( char(205) );
	}
	for(i=top+1; i<bottom; i++)
	{
		gotoxy(left,i);	putch( char(186) );
		gotoxy(right,i);	putch( char(186) );
	}
}
void normalColor()
{
	 textbackground(0);
	 textcolor(WHITE);
}
void blueColor()
{
	 textbackground(0);
	 textcolor(BLUE);
}
void warningColor()
{
	 textbackground(7);
	 textcolor(RED+BLINK);
}
class Bank
{
	private:
		long int accNo;
		char name[25];
		long float balance;
	public:
		void getAccount()
		{
			gotoxy(41,15);
			cout<<"Enter account number : ";
			cin>>accNo;
			gotoxy(41,17);
			cout<<" Name : ";
			gets(name);
			gotoxy(35,19);
			cout<<"	 Opening account :Rs.";
			cin>>balance;
		}
		void showAccount()
		{
			cout<<setiosflags(ios::left)<<setw(10)<<accNo;
			cout<<setw(1)<<setiosflags(ios::right)<<name;
			cout<<setw(20)<<setprecision(2)
				<<setiosflags(ios::fixed)
				<<setiosflags(ios::right)
				<<setiosflags(ios::showpoint)
				<<balance;
		}
		void getNewAccount()
		{
			gotoxy(41,13);
			cout<<"Enter name: ";
			gets(name);
			gotoxy(41,15);
			cout<<"Opening account :Rs.";
			cin>>balance;
		}
		void showNewAccount()
		{
			cout<<name<<endl<<balance<<endl;
		}
		void query()
		{
			gotoxy(46,15);
			cout<<"Name 	 :"<<name;
			gotoxy(46,17);
			cout<<"Balance:Rs."<<balance;
		}
	 /*	float getDeposit()
		{
			float deposit;
			gotoxy(37,20);
			cout<<"Enter the deposit amount: ";
			cin>>deposit;
			balance=balance+deposit;
			return deposit;
		}
		float getWithdraw()
		{
			float withdraw;
			gotoxy(37,17);
			cout<<"Enter the withdraw amount: ";
			cin>>withdraw;
			balance=balance-withdraw;
			return withdraw;
		} */
		void search(int);
		void createAccount();
		void openAccount(int);
		void deposit(int);
		void withdraw(int);
		void cancel(int);
		void report();
};
Bank b;
void Bank :: createAccount()
{
	fstream outFile;
	char ch;
	outFile.open("ACCOUNT.DAT",ios::out | ios::binary);
	do
	{
		clrBox(30,8,79,24);
		drawBox(30,8,79,24);
		drawBox(32,13,77,23);
		drawBox(43,10,65,12);
		gotoxy(45,11);
		cout<<"ACCOUNT CREATION";
		b.getAccount();
		int A;
		if(A==b.accNo)
		{
			drawBox(35,20,65,22);
			warningColor();
			gotoxy(37,21);
			cprintf("Duplicate account number!\n");
			normalColor();
		}
		if(b.balance<1000)
		{
			drawBox(35,20,76,22);
			warningColor();
			gotoxy(37,21);
			cprintf("Sorry your balance is less than 1000! ");
			normalColor();
		}
		else
		if(A!=b.accNo)
		{
			outFile.write((char*)&b,sizeof(b));
			drawBox(37,20,76,22);
			warningColor();
			gotoxy(40,21);
			cprintf("Account is successfully inserted!\n");
			normalColor();
		}
		A=b.accNo;
		blueColor();
		gotoxy(30,25);
		cout<<"Do you want to continue (Y/N) ? ";
		cin>>ch;
		ch=toupper(ch);
		normalColor();
		gotoxy(43,10);
		clreol();
	}while(ch=='y'||ch=='Y');
	outFile.close();
}
void Bank :: openAccount(int aNo)
{
	fstream inFile;
	inFile.open("ACCOUNT.DAT",ios::in|ios::binary);
	if(!inFile)
	{
		gotoxy(47,16);
		drawBox(37,15,76,25);
		gotoxy(47,16);
		warningColor();
		cprintf("File does not exist \n");
		normalColor();
		return;
	}
	inFile.read((char*)&b,sizeof(b));
	fstream outFile;
	outFile.open("TEMP.DAT",ios::out | ios::binary);
	boolean found=FALSE;
	while(!inFile.eof() && !found )
	{
		if(aNo==b.accNo)
		{
			drawBox(40,12,60,14);
			gotoxy(42,13);
			warningColor();
			cprintf("Duplicate number \n");
			normalColor();
			return;
		}
		else
		if(aNo>b.accNo)
		{
			outFile.write((char*)&b,sizeof(b));
			inFile.read((char*)&b,sizeof(b));
		}
		else
			found=TRUE;
	}
	Bank newAcc;
	newAcc.accNo=aNo;
	newAcc.getNewAccount();
	if(newAcc.balance<1000)
	{
		drawBox(30,16,73,18);
		gotoxy(32,17);
		warningColor();
		cprintf("Sorry! your balance is less than Rs.1000");
		normalColor();
		inFile.close();
		outFile.close();
		return;
	}
	else
		outFile.write((char*)&newAcc,sizeof(newAcc));
	while(!inFile.eof())
	{
		outFile.write((char*)&b,sizeof(b));
		inFile.read((char*)&b,sizeof(b));
	}
	drawBox(30,16,75,18);
	gotoxy(32,17);
	warningColor();
	cprintf("Account number ");
	cout<<aNo;
	cprintf(" inserted successfully \n");
	normalColor();
	inFile.close();
	outFile.close();
	remove("ACCOUNT.DAT");
	rename("TEMP.DAT","ACCOUNT.DAT");
}
void Bank :: search(int aNo)
{
fstream inFile;
inFile.open("ACCOUNT.DAT",ios::in | ios::binary);
if(!inFile)
{
	 gotoxy(47,11);
	 drawBox(37,10,76,12);
	 gotoxy(47,11);
	 warningColor();
	 cout<<"File does not exists\n";
	 normalColor();
	 return;
}
inFile.read( (char*)&b,sizeof(b) );
boolean found = FALSE;
while( (!inFile.eof()) && (!found) )
{
	 if(aNo == b.accNo)
	 {
		 found = TRUE;
		 drawBox(38,12,67,14);
		 gotoxy(40,13);
		 cout<<"Account Details Follows:\n";
		 b.query();
	 }
	 else
		 inFile.read( (char*)&b,sizeof(b) );
}
if(!found)
{
	 drawBox(43,15,74,17);
	 gotoxy(46,16);
	 warningColor();
	 cprintf("Account No.");
	 cout<<aNo;
	 cprintf(" Not Found!\n");
	 normalColor();
	 return;
}
inFile.close();
}
void Bank :: deposit(int aNo)
{
	float deposit;
	fstream modifyAcc;
	modifyAcc.open("ACCOUNT.DAT",ios::in |ios::out|ios::binary);
	if(!modifyAcc)
	{
		gotoxy(47,16);
		drawBox(37,18,76,20);
		gotoxy(47,19);
		warningColor();
		cprintf("File is not found \n");
		normalColor();
		return;
	}
	modifyAcc.read((char*)&b,sizeof(b));
	boolean found=FALSE;
	int acc=0;
	while(!modifyAcc.eof() && !found)
	{
		acc++;
		if(aNo==b.accNo)
		{
			found=TRUE;
			drawBox(35,14,76,18);
			gotoxy(37,15);
			cout<<"Content of the file follows : \n";
			gotoxy(39,17);
			b.showAccount();
			gotoxy(37,20);
			cout<<"Enter the deposit amount: ";
			cin>>deposit;
			b.balance=b.balance+deposit;
			modifyAcc.seekg((acc-1)*sizeof(b),ios::beg);
			modifyAcc.write((char*)&b,sizeof(b));
		}
		else
			modifyAcc.read((char*)&b,sizeof(b));
	}
	if(found)
	{
		drawBox(35,20,65,22);
		gotoxy(39,21);
		warningColor();
		cprintf("Rs");
		cout<<deposit;
		cprintf(" is deposited \n");
		normalColor();
	}
	else
	{
		drawBox(37,20,76,22);
		gotoxy(46,21);
		warningColor();
		cout<<aNo;
		cprintf(" is not found!");
		normalColor();
		return;
	}
	modifyAcc.close();
}
void Bank :: withdraw(int aNo)
{
	fstream modifyAcc;
	float withdraw;
	modifyAcc.open("ACCOUNT.DAT",ios::in |ios::out|ios::binary);
	if(!modifyAcc)
	{
		drawBox(37,18,76,20);
		gotoxy(47,19);
		warningColor();
		cprintf("File is not found \n");
		normalColor();
		return;
	}
	modifyAcc.read((char*)&b,sizeof(b));
	boolean found=FALSE;
	int acc=0;
	while(!modifyAcc.eof() && !found)
	{
		acc++;
		if(aNo == b.accNo)
		{
			found=TRUE;
			drawBox(35,12,76,16);
			gotoxy(37,13);
			cout<<"Content of the file follows : \n";
			gotoxy(39,15);
			b.showAccount();					gotoxy(37,17);
			cout<<"Enter the withdraw amount:Rs.";
			cin>>withdraw;
			b.balance=b.balance-withdraw;
			if(b.balance<1000)
			{
				drawBox(35,18,70,21);
				warningColor();
				gotoxy(37,19);
				cprintf("Your account is less than 1000,");
				gotoxy(39,20);
				cprintf(" cannot withdraw \n");
				normalColor();
			}
			else
			{
				modifyAcc.seekg((acc-1)*sizeof(b),ios::beg);
				modifyAcc.write((char*)&b,sizeof(b));
			 }
		}
		else
		 modifyAcc.read((char*)&b,sizeof(b));
	}
	if(found && b.balance>=1000)
	{
		drawBox(35,20,65,22);
		gotoxy(37,21);
		warningColor();
		cprintf("Rs. ");
		cout<<withdraw;
		cprintf(" is withdrawn \n");
		normalColor();
	}
	else
	if(!found)
	{
		drawBox(37,15,76,17);
		gotoxy(46,16);
		warningColor();
		cout<<aNo;
		cprintf(" is not found \n");
		normalColor();
		return;
	}
	modifyAcc.close();
}
void Bank :: cancel(int aNo)
{
	fstream inFile;
	inFile.open("ACCOUNT.DAT",ios :: in | ios :: binary);
	if(!inFile)
	{
		warningColor();
		cout<<"File doesnot exist \n";
		normalColor();
		return;
	}
	inFile.read((char*)&b,sizeof(b));
	fstream outFile;
	outFile.open("TEMP.DAT",ios :: out | ios :: binary);
	boolean found = FALSE;
	while(!inFile.eof())
	{
		if(aNo!= b.accNo)
		{
			outFile.write((char*)&b,sizeof(b));
		}
		else
		{
			found = TRUE;
			drawBox(30,12,70,16);
			gotoxy(32,13);
			cout<<"Account detail follows : \n";
			gotoxy(34,15);
			b.showAccount();
			char confirm;
			drawBox(30,17,70,19);
			warningColor();
			gotoxy(31,18);
			cprintf("Are you sure to cancel (Y/N) ? ");
			cin>>confirm;
			confirm = toupper(confirm);
			normalColor();
			if(confirm != 'Y')
				outFile.write((char*)&b,sizeof(b));
			else
			{
				blueColor();
				gotoxy(30,20);
				cprintf("Account# ");
				cout<<aNo;
				cprintf("is deleted successfully");
				normalColor();
			}
		}
		inFile.read((char*)&b,sizeof(b));
	}
	if(!found)
	{
		drawBox(35,15,76,17);
		warningColor();
		gotoxy(38,16);
		cprintf("Account number");
		cout<<aNo;
		cprintf(" is not found!");
		normalColor();
		return;
	}
	inFile.close();
	outFile.close();
	remove("ACCOUNT.DAT");
	rename("TEMP.DAT","ACCOUNT.DAT");
}
void Bank :: report()
{
	fstream inFile;
	inFile.open("ACCOUNT.DAT",ios::in|ios::binary);
	inFile.read((char*)&b,sizeof(b));
	clrBox(28,5,79,23);
	drawBox(28,5,79,23);
	drawBox(46,6,55,8);
	gotoxy(48,7);
	cout<<"REPORT";
	drawBox(30,9,75,11);
	gotoxy(36,10);
	cout<<"Acc#"<<'\t';
	gotoxy(48,10);
	cout<<"Name"<<'\t';
	gotoxy(62,10);
	cout<<"Balance"<<'\t';
	int row = 12;
	while(!inFile.eof())
	{
		if(row==22)
		{
			blueColor();
			gotoxy(36,row);
			cprintf("Press any key to continue...");
			getche();
			row=12;
			normalColor();
			clrBox(30,12,75,22);
		}
		gotoxy(38,row++);
		b.showAccount();
		inFile.read((char*)&b,sizeof(b));
	}
	gotoxy(29,row);
	cout<<"-------------------END OF REPORT------------------";
	inFile.close();
	getche();
}
void main()
{
	int aNo;
	char wish;
	int choice;
	Bank a;
	do
	{
		clrscr();
		normalColor();
		gotoxy(30,2);
		cout<<"LOADING..."<<endl;
		for(int i=1;i<2;i++)
		{cout<<i;
		 delay(1000);
		 cout<<". . .";
		 }
		 clrscr();
		cout<<"PATTANAAMAM Bank \n ";
		drawBox(21,3,60,19);
		gotoxy(28,4);
		cout<<"1. Create file\n";
		gotoxy(28,6);
		cout<<"2. Opening an account\n";
		gotoxy(28,8);
		cout<<"3. Query\n";
		gotoxy(28,10);
		cout<<"4. Deposit\n";
		gotoxy(28,12);
		cout<<"5. Withdraw\n";
		gotoxy(28,14);
		cout<<"6. Cancelling an account\n";
		gotoxy(28,16);
		cout<<"7. Report\n";
		gotoxy(28,18);
		cout<<"8. Quit"<<endl;
		gotoxy(28,20);
		cout<<"Enter your choice (1-8) : ";
		cin>>choice;
		switch(choice)
		{
			case 1 :a.createAccount();
				break;
			case 2 : do
				 {
					clrBox(27,5,77,20);
					drawBox(28,5,77,20);
					drawBox(40,6,60,8);
					gotoxy(45,7);
					cout<<"INSERTION";
					drawBox(29,9,76,11);
					gotoxy(30,10);
					cout<<"Enter the account number to be opened: ";
					cin>>aNo;
					a.openAccount(aNo);
					blueColor();
					gotoxy(37,21);
					cprintf("Do you wish to Insert (Y/N) ? ");
					cin>>wish;
					wish=toupper(wish);
					normalColor();
					gotoxy(37,21);
					clreol();
				 }while(wish=='Y' || wish=='y');
				 break;
			case 3: do
				{
					clrBox(34,6,79,23);
					drawBox(34,5,79,23);
					drawBox(50,6,62,8);
					gotoxy(52,7);
					cout<<"SEARCH";
					drawBox(35,9,78,11);
					gotoxy(41,10);
					cout<<"Enter account number: ";
					cin>>aNo;
					a.search(aNo);
					blueColor();
					gotoxy(37,24);
					cprintf("Do you wish to continue (Y/N) ? ");
					cin>>wish;
					wish=toupper(wish);
					normalColor();
					gotoxy(37,24);
					clreol();
				}while(wish=='Y' || wish=='y');
				 break;
			case 4: do
				 {
					clrBox(34,6,79,23);
					drawBox(34,5,79,23);
					drawBox(50,6,62,8);
					gotoxy(53,7);
					cout<<"DEPOSIT";
					drawBox(35,10,78,12);
					gotoxy(41,11);
					cout<<"Enter the account number : ";
					cin>>aNo;
					a.deposit(aNo);
					blueColor();
					gotoxy(37,24);
					cprintf("Do you wish to continue (Y/N) ? ");
					cin>>wish;
					wish=toupper(wish);
					normalColor();
					gotoxy(37,24);
					clreol();
				 }while(wish=='Y' || wish=='Y');
				 break;
			case 5: do
				 {
					clrBox(34,6,79,23);
					drawBox(34,5,79,23);
					drawBox(50,6,62,8);
					gotoxy(52,7);
					cout<<"WITHDRAW";
					drawBox(35,9,78,11);
					gotoxy(41,10);
					cout<<"Enter the account number : ";
					cin>>aNo;
					a.withdraw(aNo);
					blueColor();
					gotoxy(37,24);
					cprintf("Do you wish to continue (Y/N) ? ");
					cin>>wish;
					wish=toupper(wish);
					normalColor();
					gotoxy(37,24);
					clreol();
				 }while(wish=='Y' || wish=='y');
				 break;
			case 6: do
				 {
					clrBox(28,5,79,23);
					drawBox(28,5,79,23);
					drawBox(50,6,64,8);
					gotoxy(52,7);
					cout<<"CANCELLATION";
					drawBox(29,9,78,11);
					gotoxy(30,10);
					cout<<"Enter the account number to cancel: ";
					cin>>aNo;
					a.cancel(aNo);
					blueColor();
					gotoxy(32,24);
				cprintf("Do you wish to cancel again (Y/N) ? ");
				cin>>wish;
				wish=toupper(wish);
				normalColor();
				gotoxy(32,24);
				clreol();
				 }while(wish=='Y' || wish=='y');
				 break;
			case 7: a.report();
				 break;
			case 8: break;
			default:cout<<"Wrong choice...retry\n";
		}
	}while(choice!=8);
}
