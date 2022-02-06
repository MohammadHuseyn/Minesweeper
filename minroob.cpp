/* Tozihat:
  BOMB !
  NORMAL O
  FLAG F
  colors:
  numbers: Yellow (BiHal) 247, 240, 109
  main game: white 255, 255, 255
  Bombs: red 255, 0, 0
  flag and neutralized Mine 252, 151, 0
  message & play numbers 69, 221, 230
*/
#include <time.h>
#include <stdio.h>
#include <iostream>
#include "clui.h"
#include <string>
#include <fstream>
using namespace std;
//Global vatiants
int x=0,y=0;
string name,gameslot,slotlist,hidden="SAVED GAMES/",str; // str is a temp to save/load game
//Functions!
//message the player
void message(char status,int FlagsLeft) {
  cursor_to_pos(get_window_rows()-1,1);
  for (int i = 0; i < get_window_cols(); i++) {
    cout << "-";
  }
  cursor_to_pos(get_window_rows(),1);
    change_color_rgb(252, 151, 0);
  cout << " Flags: ";
    change_color_rgb(255,255,255);
  cout << FlagsLeft << " |";
    change_color_rgb(69, 221, 230);
  cout << " Message: ";
    change_color_rgb(255,255,255);
  switch (status) {
    case 'n': cout << "There is a number!"; break;
    case 'c': cout << "You can not open a flaged house."; break;
    case 'o': cout << "House (" << x/3 << ',' << y-2 << ") just opened!"; break;
    case 'f': cout << "House (" << x/3 << ',' << y-2 << ") just flaged!"; break;
    case 'u': cout << "House (" << x/3 << ',' << y-2 << ") just unflaged!"; break;
    case 's': cout << "Want to save game with name: "; change_color_rgb(66, 135, 245); cout << name; change_color_rgb(255,255,255); cout << "? (y/n) "; break;
    case 'q': cout << "Sure want to quit without saving? (y/n): "; break;
    case 'r': cout << "You are ran out of flags";break;
    default: cout << "Move with WASD and use space button or f to play!";
  }
}
//move in x
int movex(int letter ,int minofrange, int maxofrange,int n){
  //use A & D keybuttons to move
  if (letter=='A'||letter=='a') {
    if (x==minofrange) {
      cursor_to_pos(y,x);
    }else cursor_to_pos(y,x=x-n);
  }else if (letter=='D'||letter=='d') {
    if (x==maxofrange) {
      cursor_to_pos(y,x);
    }else cursor_to_pos(y,x=x+n);
  }
  return x;
}
//move in y
int movey(int letter,int minofrange, int maxofrange, int n){
  if (letter=='S'||letter=='s') {
    if (y==maxofrange){
      cursor_to_pos(y,x);
    }else cursor_to_pos(y=y+n,x);
  }else if (letter=='W'||letter=='w') {
    if(y==minofrange){
      cursor_to_pos(y,x);
    }else cursor_to_pos(y=y-n,x);
  }
  return y;
}
//check if the char is one of wasd
bool AllowToMove(char character){
  if (character=='A' || character=='a' || character=='S' || character=='s' || character=='D' || character=='d' || character=='w' || character=='W') {
    return true;
  } else return false;
}
//choose from a menu
int chooseone(int ymin,int ymax,int cursor_to) {
  cursor_to_pos(y=cursor_to,x=1);
  char character=1;
  while (true) {
      character=getch();
      if (AllowToMove(character)) {
        y=movey(character,ymin,ymax,1);
      }else if (character==10 || character==32) {
          return y;
        }
  }
}
//count number of Bombs
int countbombs(int i, int j,char** ground_mine,int row){
  int res=0;
  if (i!=0) {
    if (j!=0){
      if (ground_mine[i-1][j-1]=='!')
        res++;
    }
      if (ground_mine[i-1][j]=='!')
        res++;
    if (j!=row-1){
      if (ground_mine[i-1][j+1]=='!')
      res++;
    }
  }
  if (true){
    if (j!=0){
      if (ground_mine[i][j-1]=='!')
        res++;
    }
      if (ground_mine[i][j]=='!')
        res++;
    if (j!=row-1){
      if (ground_mine[i][j+1]=='!')
      res++;
    }
  }
  if (i!=row-1) {
    if (j!=0){
      if (ground_mine[i+1][j-1]=='!')
        res++;
    }
      if (ground_mine[i+1][j]=='!')
        res++;
    if(j!=row-1){
      if (ground_mine[i+1][j+1]=='!')
        res++;
    }
  }
  return res;
}
//A function for printing row and column numbers.
void cout_radif(int row) {
  cout << "    ";
  for (int i = 0; i < row; i++) {
    cout << i+1 << ' ';
    if (i<9)
      cout << ' ';
  }
  cout << '\n' << "   ";
  for (int i = 0; i < row; i++) {
    cout << "---";
  }}
// how many digits are my number?
int count_digit(int number) {
     int count=0;
     while(number!=0) {
        number/=10;
        count++;
     }
     return count;
  }
//Main
int main() {
 change_color_rgb(255,255,255); // <= THE MAIN COLOR!
  //variables defining
  char status,temploadgame[20][20],temploadground[20][20];
  bool lose=false,win=false,gamestarted=false; // gamestarted -> true => Exit the Menu and start the game!
  int row,mine,FlagsLeft,Y_counter,MenuStatus=0; /* Menu Status:
                                                  0 -> MAIN MENU
                                                  1 -> NEW GAME MENU
                                                  2 -> LAOD GAME MENU
                                                  3 -> CHANGE USER MENU
                                                */

  //menu
  init_clui(); // from clui librarry

  while (true) { // The whole game runs in a loop
   while (!gamestarted) { // a while that will exit from menus with The obtained values of row, mine or game setting.
     if (name[0]==0) { //fint the player a name, if he doesn't.
       ifstream names (hidden+"names.txt");
       getline(names,name);
     }

     if (MenuStatus==0){ //The main Menu
       clear_screen();
       cout << "! Signed in as ";
       change_color_rgb(0,255,0);
       cout << name;
       change_color_rgb(255,255,255);
       cout << " | Developed by Mohammad Huseyn Amini" << '\n' << "# New Game" << '\n' << "# Load Game" << '\n' << "# Change user" << '\n' << "# How to play" << '\n' << "< Quit" << '\n';
       switch (chooseone(2,6,2)) {
         /*New*/    case 2:MenuStatus=1;break;
         /*Load*/   case 3:MenuStatus=2;break;
         /*Change*/ case 4:MenuStatus=3;break;
         /*How to play*/   case 5:{
                                   clear_screen();
                                   cout << "< Back" << '\n';
                                   std::cout << "1. Mines are randomly planted in houses. When you open a house, you may detonate a mine or see a number. If a mine explodes, the game is over and you lose, but if a number appears, that number is the sum of the mines of the 8 surrounding houses." << '\n';
                                   std::cout << "2. The game will continue until a mine explodes or you find all the mine-free houses. You can also use flags to mark a house as a mine" << '\n';
                                   std::cout << "3. At the beginning of the game, wherever cursor is (Randomly selected), there is not any mine" << '\n';
                                   std::cout << "4. move cursor with WASD keys." << '\n'<< "5. Open a house with space button." << '\n' << "6. Flag or Unflag a house with F key." << '\n' << "7. Save the game with O key (You can load it from main menu)" << '\n' << "8. Quit the game without saving by Q key";
                                   chooseone(1,1,1);
                                   MenuStatus=0;
         }break;
         /*quit*/ case 6: clear_screen();cout << "You are out of the game Minroob. Wanna play again? run the command below:" << '\n' << "./minroob.out" << '\n'; return 0;
       }

     }else if (MenuStatus==1) { //The New game Menu
       clear_screen();
       cout << "< Back !--> Signed in as "; change_color_rgb(0,255,0);cout << name; change_color_rgb(255,255,255); cout << ". Select Back and go to 3rd option to change user!" << '\n';
       cursor_to_pos(2,1);
           cout << "# Easy   | rows: 5, mines: 4" << '\n' << "# Medium | rows: 12, mines: 28" << '\n' << "# Hard   | rows: 20, mines: 96" << '\n' << "# Custom | rows: ?, mines: ?" << '\n';
           switch (chooseone(1,5,2)) {
             case 1: MenuStatus=0;break;
             /* Easy */  case 2: row=5; mine=4;gamestarted=true;break;
             /* Medium */case 3: row=12; mine=28;gamestarted=true;break;
             /* Hard */  case 4: row=20; mine=96;gamestarted=true;break;
             /* Custom */case 5:{
                 cursor_to_pos(5,18);
                 cin >> row;
                   while (!(row>=5 && row<=20)) { // Check the row number | Custom play ground!
                     cursor_to_pos(7,1);
                     cout << "Enter a number between 5 and 20!               " << '\n';
                     cursor_to_pos(5,18);
                     cout << " , mines: ?   " << '\n';
                     cursor_to_pos(5,18);
                     cin >> row;
                   }
                   mine=row*row*0.19;
                   cursor_to_pos(6,1);
                   cout << "we suggest " << mine << " as numebr of mine, nut you can select any number you wnat! just type it.                                                   " << '\n';
                   cursor_to_pos(5,28);
                   cin >> mine;
                   while (!(mine>=0 && mine<=row*row/2)) { // Check the mine number | Custom play ground!
                     cursor_to_pos(6,1);
                     cout << "Enter a number between 0 and "<< row*row/2 << "!                                                                                 " << '\n';
                     cursor_to_pos(5,28);
                     cout << "      " << '\n';
                     cursor_to_pos(5,28);
                     cin >> mine;
                   }
             }gamestarted=true;break;
       }

     }else if (MenuStatus==2) { // The Load game menu
       clear_screen();

       if (name[0]==0) { // get our player a name! if he doesn't
         ifstream names (hidden+"names.txt"); // get all player names
         getline(names,name);
       }
       slotlist="list"+name+".txt"; // Making the NAME of SAVE/LOAD files.
       ifstream list (hidden+slotlist); //get the play ground file
       // cout the list.txt
       cout << "Which game do you want to load?" << '\n' << "< Back" << '\n';

       Y_counter=0; // counter for load options
       int temp_position=3;
       if (!list.fail()) { // maybe there isnt any files!
         while (!list.eof()) { // Till the file ending
           getline(list,str);
           cout << "# " << str;
           if (str[0]!=0) {
             cout << '\n';
           }
           Y_counter++;
         }
       }else{
       Y_counter=1;
       temp_position=2;
       }
       change_color_rgb(86, 97, 96);
       cout << "End of the list" << '\n';
       change_color_rgb(255,255,255);

       //Choose a game to LOAD!
       int ito=chooseone(2,Y_counter+1,temp_position); // i should go to...?
       switch (ito) {
         case 2: MenuStatus=0;break; // < Back
         default:{
           ifstream listnew (hidden+slotlist); // OPening player game slots list
           for (int i = 0; i < ito-3; i++) {
             getline(listnew,gameslot);
           }
           getline(listnew,gameslot); // got the game slot
           cout << "\033]0;" << name << " | " << gameslot << "\007"; // Changin The Tilte of TERMINAL
           gameslot+=".txt"; // .txt

           //LOAD the Chosen
           clear_screen();
           ifstream bazi (hidden+gameslot); //get THE GAME SLOT
           bool end=false; // There is a loop ahead. whene should I end it?
           int place=0,rowcounter=0,whichif=0; // load variables

           while (!end) {
             if (place>0)
             bazi.seekg(place,ios::beg);
             getline(bazi,str);
               if (whichif==0){
                 mine = stoi(str);
                 place+=str.length()+1;//place=3
                 whichif++;
               }else if (whichif==1) {
                 row = stoi(str);
                 place+=str.length()+1;//place=6
                 whichif++;
               }else if (whichif<row+2){
                 for (int j = 0,c=0; j < row; j++, c+=3) {
                   temploadgame[rowcounter][j]=str[c];
                 }
                 rowcounter++;
                 place+=3*row-1;
                 whichif++;
               }else if (whichif<2+row+mine){
                 int temp_i,temp_j;
                 temp_i=stoi(str);
                 getline(bazi,str);
                 temp_j=stoi(str);
                 cout << temp_i << '\n' << temp_j << '\n';
                 temploadground[temp_i][temp_j]='!';
                 place=-1;
                 whichif++;
               }else {gamestarted=true;end=true;}
             }

         }
       }

     }else if (MenuStatus==3){ //THe Change User menu
       clear_screen();
       Y_counter=0;

       ifstream names (hidden+"names.txt"); // get player names list
       cout << "< Back" << '\n';

       change_color_rgb(155,155,0);
       cout << "* Add a player" << '\n';
       change_color_rgb(255,255,255);

       while (!names.eof()) { //print names
         getline(names,name);
         cout << "# " << name;
         if (name[0]!=0) {
           cout << '\n';
         }
         Y_counter++;
       }

       change_color_rgb(86, 97, 96);
       cout << "End of the list" << '\n';
       change_color_rgb(255,255,255);

       int ito=chooseone(1,Y_counter+1,3);
       switch (ito) {
         case 1: MenuStatus=0;break; // < Back
         case 2: { // * Add New player
           ofstream names_new (hidden+"names.txt", fstream::app);
           cout << "Player name:    " ;
           cursor_to_pos(2,14);
           getline(cin,name);
           names_new << name << '\n';
         }break;

         default : { // Choose a name
           ifstream names_new (hidden+"names.txt");
           for (int i = 0; i < ito-3; i++) {
             getline(names_new,name);
           }
           getline(names_new,name);
         }break;

       }
       MenuStatus=0;
     }

   }


   //BRAIN!

   // Make game arrays with pointer
   char** game = new char*[row];
   char** ground_mine = new char*[row];
   for (int i = 0; i < row; i++) {
     game[i] = new char[row];
     ground_mine[i] = new char[row];
   }
   for (int i = 0; i < row; i++) {
     for (int j = 0; j < row; j++) {
       game[i][j]='O';
       ground_mine[i][j]=' ';
     }
   }

   if (MenuStatus!=2) { // if this game IS NOT LOADed, plant some MINES
     //planting Mine
     srand(time(0));
    int rand_row=rand()%5, rand_column=(rand()*(rand()%5))%5,temp=0;
     for (int i = 0; i < mine; i++,temp++) {
         rand_row=rand()%row;
         rand_column=(rand()*(rand()%mine))%row;
         if(rand_column<0)
          rand_column*=(-1);
          cout << rand_row+1 << " " << rand_column+1 << '\n';
          if (ground_mine[rand_row][rand_column]=='!') {
            i--;
            temp--;
          }else
           ground_mine[rand_row][rand_column]='!';
     }
   }else { // if this game IS LOADed, Put the mines where they should be
   for (int i = 0; i < row; i++) {
     for (int j = 0; j < row; j++) {
       game[i][j]=temploadgame[i][j];
       ground_mine[i][j]=temploadground[i][j];
     }
   }
 }
   cursor_to_pos(y=3,x=5);

   //PLAY TIME!
   char character; // for getch() and orders

   //THE WHILE!!
   FlagsLeft=mine;
   while (!lose) { // Run the game until the player loses
       clear_screen();
       message(status,FlagsLeft); //What should I tell to player? find the status further

       if (status!='s' && status!='q')
         status='!'; //get out of last status, if it is'nt save/quit

       cursor_to_pos(1,1);

       //cout
       change_color_rgb(247, 240, 109);

       // Number of Rows and Colums
       cout_radif(row);
       cout << " |" << '\n';
       for (int i = 0; i < row; i++) {
         cout << i+1;
         if (i<9)
           cout << ' ';
           cout << "| ";
           for (int j = 0; j < row; j++) {
             change_color_rgb(255,255,255);
             if (game[i][j]=='!') // If the player finds a mine, he loses
               lose=true;
             else if (game[i][j]=='F') // If the player FLAGED somewhere get it a color!
               change_color_rgb(255, 0, 247);
             else if(game[i][j]<='8' && game[i][j]>='0'){ // Colors for numbers
             switch (game[i][j]) {
               case '1': change_color_rgb(255, 194, 10) ;break;
               case '2': change_color_rgb(168, 111, 50) ;break;
               case '3': change_color_rgb(168, 66, 50) ;break;
               case '4': change_color_rgb(179, 29, 29) ;break;
               case '5': change_color_rgb(105, 51, 4) ;break;
               case '6': change_color_rgb(64, 32, 4) ;break;
               case '7': change_color_rgb(38, 19, 3) ;break;
               case '8': change_color_rgb(38, 19, 2) ;break;
             }
           }
             if (game[i][j]=='0'){ // if there isnt any mines around, cout a space
             cout << "   ";
             }else cout  << game[i][j] << "  ";
             change_color_rgb(255,255,255);
           }
         change_color_rgb(247, 240, 109);
         cout << "| " << '\n';
       }
     cursor_to_pos(y,x); //go to last house

     //SAVING GAME: IF player pressed O or o, Save the game
     if (status=='s') {
         cursor_to_pos(get_window_rows(),58+name.length()+count_digit(FlagsLeft));
       int slotnum;
       cin >> character; // get the order
       if (character=='y' || character=='Y') {
         cout << " Number of slot? : ";
         cin >> slotnum; //Select a slot
         slotlist="list"+name+".txt";

         ofstream savelist(hidden+slotlist ,fstream::in | fstream::out | fstream::app ); // put the slot name in player slot list
         gameslot=name+"_slot"+to_string(slotnum);
         savelist << gameslot << '\n';
         gameslot+=".txt";

         ofstream save (hidden+gameslot,ios::trunc); // Save The Slot!
         save << mine << endl << row << endl;
         for (int i = 0; i < row; i++) {
           for (int j = 0; j < row; j++) {
             save << game[i][j];
             if (j<row-1) {
               save << "  ";
             }
           }
            save << endl;
         }
         for (int i = 0; i < row; i++) {
           for (int j = 0; j < row; j++) {
             if (ground_mine[i][j]=='!') {
               save << i << endl << j << endl;
             }
           }
         }
         status='!'; // get out of saving status
         clear_screen();
         cursor_to_pos(1,1);
         cout << "game saved" << '\n';

         //get back to main menu and set status
         gamestarted=false;
         MenuStatus=0;
         status='!';
         getch(); // For extra enter
         break;

       }else if (character=='N' || character=='n') {
         status='!';
       }
     }else if (status=='q'){ // IF pleyer pressed q or Q, get out of game
       cursor_to_pos(get_window_rows(),62+count_digit(FlagsLeft));
       cin >> character;
       if (character=='y' || character=='Y') {
         clear_screen();

         //get back to main menu and set status
         gamestarted=false;
         MenuStatus=0;
         status='!';
         getch(); // For extra enter
         break;

       }else if (character=='n' || character=='N'){
         status='!';
       }
     }

     // IF player Lost:
     if(lose)
     break;

     // PLAYER WIN: search for victory
     win=true;
     for (int i = 0; i < row; i++) {
       for (int j = 0; j < row; j++) {
         if (( game[i][j]=='O' || game[i][j]=='F') && ground_mine[i][j]!='!') {
           win=false;
         }
       }
     }

     //If player WON:
     if(win)
     break;

     //get the ORDERS from player
     if (status!='s') //first check that we are not in saving mode
       character=getch();
     if (AllowToMove(character)){ // Can we move by this characters?
       x=movex(character,5,row*3+2,3);
       y=movey(character,3,row+2,1);
     }else if (character==32){ // IF player pressed space button, open the house
       if (game[y-3][x/3-1]=='F')
       status='c';
       else if (ground_mine[y-3][x/3-1]=='!'){
       game[y-3][x/3-1]=ground_mine[y-3][x/3-1];
       }else{
        game[y-3][x/3-1]='0'+countbombs(y-3,x/3-1,ground_mine,row);
        status='o';
       }
     }else if (character=='F' || character=='f'){ // IF player pressed the flag button, Check there is a flag or not and...
       if (game[y-3][x/3-1]=='F'){ //unflag
           status='u';
           game[y-3][x/3-1]='O';
           FlagsLeft++;
       }else if (game[y-3][x/3-1]<='8' && game[y-3][x/3-1]>='0') //cant flag this one
         status='n';
         else { //flag
           if (FlagsLeft!=0) {
             game[y-3][x/3-1]='F';
             status='f';
             FlagsLeft--;
           }else status='r';
         }
     }else if (character=='o' || character=='O') { // IF player pressed o or O, Save the game
       status='s';
     }else if (character=='q' || character=='Q') { // IF player pressed q or Q, Exit the game
       status='q';
     }
   } //End of "THE WHILE!!" loop

   //If player lost, cout the play ground and exit
   if (lose){
     clear_screen();
     // nuber of rows
     for (int i = 0; i < row; i++) {
       for (int j = 0; j < row; j++) {
         if (game[i][j]=='O') {
           game[i][j]='0'+countbombs(i,j,ground_mine,row);
         }
       }
     }

     cursor_to_pos(3,1);
     change_color_rgb(247, 240, 109);

     cout_radif(row);
     cout << '\n';
     for (int i = 0; i < row; i++) {
       cout << i+1;
       if (i<9)
       cout << ' ';
       cout << "| ";
         for (int j = 0; j < row; j++) {
         if (ground_mine[i][j]=='!'){
           change_color_rgb(255, 0, 251);
           cout << ground_mine[i][j] << "  ";
           change_color_rgb(247, 240, 109);
         }else {
           if (game[i][j]<='8' && game[i][j]>='0') {
             switch (game[i][j]) {
               case '1': change_color_rgb(255, 194, 10) ;break;
               case '2': change_color_rgb(168, 111, 50) ;break;
               case '3': change_color_rgb(168, 66, 50) ;break;
               case '4': change_color_rgb(179, 29, 29) ;break;
               case '5': change_color_rgb(105, 51, 4) ;break;
               case '6': change_color_rgb(64, 32, 4) ;break;
               case '7': change_color_rgb(38, 19, 3) ;break;
               case '8': change_color_rgb(38, 19, 2) ;break;
             }
           }else change_color_rgb(255,255,255);
           if (game[i][j]==0)
           cout << "   ";
           else cout << game[i][j] << "  ";
           change_color_rgb(247, 240, 109);
         }
       }
       cout << '\n';
     }

     //delay
     for (int sec = 1; sec < 6; sec++) {
       cursor_to_pos(1,1);
       change_color_rgb(255,0,0);
       cout << "Game Over, You Lost :( ";
       change_color_rgb(255,255,255);
       cout << "You'll be taken to the main menu in ";
       change_color_rgb(0,0,255);
       cout << 6-sec;
       change_color_rgb(255,255,255);
       cout << " second(s)" << '\n';
       cout << "DO NOT PRESS ENTER!" << '\n';
       delay(1000);
     }
     lose=false;
     gamestarted=false;
     MenuStatus=0;
   } // End of "if player lost"

   //If player Won:
   if(win){
     clear_screen();
     cursor_to_pos(3,1);
     cout_radif(row);
     cout << '\n';
     for (int i = 0; i < row; i++) {
       cout << i+1;
       if (i<9)
       cout << ' ';
       cout << "| ";
       for (int j = 0; j < row; j++) {
         if (ground_mine[i][j]=='!'){
           change_color_rgb(252, 151, 0);
           cout << ground_mine[i][j] << "  ";
           change_color_rgb(247, 240, 109);
         }else{
             if(game[i][j]<='8' && game[i][j]>='0')
             change_color_rgb(155,155,155);
            cout << game[i][j] << "  ";
            change_color_rgb(247, 240, 109);
         }
       }
       cout << '\n';
     }

          //delay
          for (int sec = 1; sec < 6; sec++) {
            cursor_to_pos(1,1);
            change_color_rgb(0,255,0);
            cout << "Game Over, You WON :) ";
            change_color_rgb(255,255,255);
            cout << "You'll be taken to the main menu in ";
            change_color_rgb(0,0,255);
            cout << 6-sec;
            change_color_rgb(255,255,255);
            cout << " second(s)" << '\n';
            cout << "DO NOT PRESS ENTER!" << '\n';
            delay(1000);
          }
          win=false;
          gamestarted=false;
          MenuStatus=0;

   }// End of "If player WON"

   // delete command for pointers
   for (int i = 0; i < row; i++) {
       delete[] game[i];
       delete[] ground_mine[i];
   }
   delete[] game;
   delete[] ground_mine;

  }
    return 0;
}
