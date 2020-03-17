#include<fstream.h>
#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
 
//Global variables to be used in different functions
 
char trump;                   	//Trump suit
int playedplayer[5];          	//Details of a played card
int playednumber[4];
int playedval[5];
char playedcolour[5];
int handswon[2]={0,0};        	//Hands won by each team
int turn;                     	//Number of the turn
  

class card
{
 	char colour;        	//Suit of the card
 	int value;          	//Face value of the card
 	int player;         	//The player that possesses the card
int cardnumber;      	
public:
 	card () { player=0; }
 	int CheckClr (char Cclr);
 	int CheckPly (int Cpl);
 	int CheckCn (int Ccn);
 	void GetNumber (int n);
 	void makecard(int i, int j);
 	void distribute (int count);
 	char PutClr ();
 	int PutVal ();
 	void showcard(int b);
 	void reset ();
};
 
//Checking the details of a card
int card::CheckClr (char Cclr)
{
if (Cclr==colour)
      	return 0;
else
      	return 1;
}
 
int card::CheckPly (int Cpl)
{
if (Cpl==player)
      	return 0;
else
      	return 1;
}
 
int card::CheckCn (int Ccn)
{
if (Ccn==cardnumber)
      	return 0;
else
      	return 1;
}
 

//Logically creating and distributing a single card
void card::GetNumber (int n)
{
cardnumber=n;
}
 
void card::makecard (int i, int j)
{
value=j+2;
switch (i)
 	{
 	case 0: colour='S'; break;
 	case 1: colour='H'; break;
 	case 2: colour='D'; break;
 	case 3: colour='C'; break;
 	}
}

void card::distribute (int count)
{
player=count;
}
 

//Member functions for output
char card::PutClr ()
{
return colour;
}
 
int card::PutVal ()
{
return value;
}
 
void card::showcard (int b)
{
cout<<"\t"<<colour;
switch (value)
 	{
 	case 11: cout<<" Jack"; break;
 	case 12: cout<<" Queen"; break;
 	case 13: cout<<" King"; break;
 	case 14: cout<<" Ace"; break;
 	default: cout<<" "<<value;
 	}
if (b==1)
 	cout<<" - ("<<cardnumber<<")";
}
 
 
//Introduction to the game
 
void intro ()
{
cout<<"\n\n\n\n\n\t\t\t      Welcome to Kot Pees!\n\n\n\n";
cout<<"\t\t=================================================\n\n\n\n";
cout<<"\t\t\t   Press any key to continue\n";
getch();
}


//Logically creating and storing the deck
void create ()
{
fstream f;
f.open ("DECK.DAT",ios::binary|ios::out);
card c;
int a[4]={0,0,0,0}, count, seedv=1;
 
for (int i=0;i<4;i++)
 	for (int j=0;j<13;j++)
 	{
      	c.makecard(i,j);
      	while (c.CheckPly(0)==0) 		//Distributing the cards
          	{
          	srand (time (NULL) + seedv );
          	count = rand()%4;
 
          	if (a[count]<13)
               	{
               	c.distribute (count);
               	a[count]++;
               	}
          	seedv++;
          	}
 
      	f.write((char*) &c, sizeof(c));
      	c.distribute(0);   //Resetting player of card object
      	}
f.close();
}


//Logically removing the used cards from the deck
void del (int p, int cn)
{
fstream f1,f2;
f1.open("DECK.DAT",ios::binary|ios::in);
f2.open("TEMP.DAT" ,ios::binary|ios::out);
card c;
 
while (f1.read((char*) &c, sizeof (c)))
      	{
      	if (c.CheckPly(p)==0 && c.CheckCn(cn)==0);
      	else
          	f2.write((char*) &c, sizeof (c));
      	}
 
f1.close();
f2.close();
remove("DECK.DAT");
rename("TEMP.DAT", "DECK.DAT");
}


//To show all the cards possessed by a player
void displayhand (int p)
{
fstream f;
int i=1;
f.open("DECK.DAT" ,ios::binary|ios::in);
card c;
while ( f.read((char*) &c, sizeof(c)))
 	if ( c.CheckPly(p)==0 )
      	{
      	c.showcard(1);      	//Displaying card with card number
      	if (i%4==0)
          	cout<<endl;
      	i++ ;
      	}
 
cout<<endl;
f.close();
}


//To select the trump suit (assuming player 1 to be the trump-caller)
void trumpselection ()
{
fstream f;
card c;
int i=1, temp, seedv=1;
 
while (i<=5)
 	{
 	f.open("DECK.DAT" ,ios::binary|ios::in);
 	while ( f.read((char*) &c, sizeof(c)) && i<=5)
 
      	if ( c.CheckPly(1)==0 )
          	{
 	      	srand ( time (NULL) + seedv);     	
          	temp=rand()%13+1;   	//To show 5 random cards
          	if (temp<=5)
               	{
               	c.showcard(0); //Displaying card without card no.
               	i++;
               	}
          	seedv++;
          	}
 
 	f.close();
 	}
 
cout<<endl;
cout<<"\nSelect the Trump Suit"<<endl;
cout<<"Enter - 	S: Spades\tH:Hearts\tD:Diamonds\tC:Clubs\n";
 
do
 	{
 	cin>>trump;
 	if (trump!='S' && trump!='H' && trump!='D' && trump!='C')
      	cout<<"INVALID COLOUR. TRY AGAIN! (Case Sensitive)\n";
 	} while (trump!='S' && trump!='H' && trump!='D' && trump!='C');
 
clrscr();
}


//To assign a reference variable to each card possessed by a player during a trick
void applycardno (int p)
{
card c;
int i=1;
fstream f1, f2;
f1.open("DECK.DAT", ios::binary|ios::in);
f2.open("TEMP.DAT", ios::binary|ios::out);
 
while ( f1.read((char*) &c, sizeof (c)) )
 	{
 	if (c.CheckPly(p)==0)
      	{
      	c.GetNumber(i);
      	i++;
      	}
 	f2.write ((char*) &c, sizeof (c));
 	}
 
f1.close();
f2.close();
remove("DECK.DAT");
rename("TEMP.DAT","DECK.DAT");
}


//To show the cards played by previous players
void showplay ()
{
fstream f;
int i,found;
card c;
 
cout<<"\n\nCurrent play :-\n";
for (i=0; i<turn; i++)
 	{
 	found=0;
 	f.open("DECK.DAT", ios::binary|ios::in);
 
 	while ( found==0 )
      	{
      	f.read((char*) &c, sizeof (c) );
 
      	if (c.CheckPly( playedplayer[i] )==0 && c.CheckCn( playednumber[i] )==0)
          	{
          	cout<<"Player- "<<playedplayer[i]<<" ";
          	c.showcard(0);
          	cout<<endl;
          	found=1;
          	}
 
      	}
 
 	f.close();
 	}
}
 
 
//To check the correctness of the play made by a player
int check ()
{
fstream f;
card c;
int found=0, legal=1;
f.open("DECK.DAT", ios::binary|ios::in);
 
while ( found==0 )
 	{
 	f.read((char*) &c, sizeof (c) );
 
 	if (c.CheckPly( playedplayer[turn] )==0 && c.CheckCn( playednumber[turn] )==0)
      	{
      	playedval[turn] = c.PutVal ();
      	playedcolour[turn] = c.PutClr ();
      	found = 1;
      	}
 	}
f.close();
 
if (playedcolour[turn]!=playedcolour[0])
 	{
 	f.open("DECK.DAT", ios::binary|ios::in);
 	while ( f.read((char*) &c, sizeof (c)) )
 
      	if (c.CheckClr( playedcolour[0] )==0 && c.CheckPly ( playedplayer[turn] )==0 )
          	{
          	legal=0;
          	}
 	f.close();
 	}
 
/*A different suit can be played if and only if the player does not possess a card of the leading suit*/
 
return legal;
}


//To enable the user to play a card
void play (int p)
{
int cardmax=13-handswon[0]-handswon[1], legal=1;
 
applycardno (p);
displayhand (p);
showplay ();
cout<<"\n\nHands won by players 1 and 3: "<<handswon[0]<<endl;
cout<<"Hands won by players 2 and 4: "<<handswon[1];
 
do
 	{
 	cout<<"\n\n\nEnter the card number of the card you would like to play: "; 
 
//Card number can be used for unique identification of a card
 
 	cin>>playednumber[turn];
 	playedplayer[turn]=p;
 
 	if (playednumber[turn]<1 || playednumber[turn]>cardmax)
      	legal=0;
 	else
      	legal=check();
 	if (legal==0)
      	cout<<"WRONG TURN! PLEASE TRY AGAIN.";
 	} while (legal==0);
 
clrscr();
cout<<"Press any key to proceed to next turn.\n";
getch();
clrscr ();
}


//To evaluate all cards played and return the player that won the hand
int evaluate ()
{
int i, toevaluate[4]={0,0,0,0}, cut=0, max, winner;
 
for (i=0; i<4; i++)
 	if (playedcolour[i]==trump)
      	cut=1;
 
if (cut==1)
 	{
 	playedcolour[4]=trump;
 	for (i=0; i<4; i++)
      	if ( playedcolour[i]==trump )
          	toevaluate[i] = playedval[i];
 	}
else
 	{
 	playedcolour[4]=playedcolour[0];
 	for (i=0; i<4; i++)
      	if ( playedcolour[i]==playedcolour[0] )
          	toevaluate[i]=playedval[i];
 	}
 
/*Only those cards are to be evaluated that are either of the trump suit or of the leading suit*/
 
max=toevaluate[0];
winner=playedplayer[0];
for (i=1; i<4; i++)
 	if (toevaluate[i]>max)
      	{
      	max=toevaluate[i];
      	winner=playedplayer[i];
      	}
 
/*To determine maximum face value of cards under consideration and match the index to the winning player*/
 
playedval[4]=max;
playedplayer[4]=winner;
return winner;
}

//To carry out the game
void game ()
{
int playerturn=1, result, cardmax=13;
 
while (handswon[0] < 7 && handswon[1] < 7)
 	{
 	turn=0;
 	while (turn<4)
      	{
      	if (cardmax!=13)
          	{
          	cout<<"Previous play was won by: Player - "<<playedplayer[4];
          	cout<<" using "<<playedcolour[4];
          	switch (playedval[4])
               	{
               	case 11: cout<<" Jack"; break;
               	case 12: cout<<" Queen"; break;
               	case 13: cout<<" King"; break;
               	case 14: cout<<" Ace"; break;
               	default: cout<<" "<<playedval[4];
               	}
          	cout<<"\n\n";
          	}
 
                    	cout<<"Trump: "<<trump<<"\nPlayer: "<<playerturn<<"\n\n";
      	play (playerturn);
      	turn++;
      	if (playerturn==4)  	//To rotate turns among 4 players
          	playerturn=1;
      	else
          	playerturn++;
      	}
 
 	result=evaluate();
 	playerturn=result;
 	if (result==1 || result==3)
      	handswon[0]++;
 	else
      	handswon[1]++;
 	for (int i=0; i<4; i++)
      	del (playedplayer[i],playednumber[i]);
 	cardmax--;
 	}
 
if (handswon[0]==7)
 	cout<<"Congratulations! Players 1 and 3 have won the game!";
else
 	cout<<"Congratulations! Players 2 and 4 have won the game!";
}
 
 
void main ()
{
intro();
create();
trumpselection();
game();
}

