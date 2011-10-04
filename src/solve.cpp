#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <boost/unordered_set.hpp>
#include <queue>
#include <string>
#include <utility>
#include <algorithm>

using namespace std;

#define WALL_X  36
#define WALL_Y  5
int wall[WALL_X][WALL_Y];

void make_wall(int x,int y,string board) {
  for(int i = 0 ; i < WALL_X ; i++ ){
    for(int j = 0 ; j < WALL_Y ; j++ ){
      wall[i][j] = -1; 
    }   
  }

  int max = x * y;
  for(int i = 0 ; i < max ; i++ ){
    int j = 0;
    if( (i % x != 0) && (board[i-1] != '=') )   { wall[i][j] = i-1; j++;  }
    if( (i % x != x-1) && (board[i+1] != '=') ) { wall[i][j] = i+1; j++;  }
    if( ( i >= x ) && (board[i-x] != '=') )     { wall[i][j] = i-x; j++;  }
    if( ( i / x < y-1) && (board[i+x] != '=') ) { wall[i][j] = i+x; j++;  }
  }
}

int main(int argc, char *argv[]){
  int no_i,x,y;
  string init,answer;

  if( argc != 4 ){
    cout << "usage: solve x y problem " << argc << endl;
    exit(-1);
  }
  else {
    x = atoi(argv[1]);
    y = atoi(argv[2]);
    init = string(argv[3]);
  }

 for(int i=0 ; i < x * y ; i++){
    int n = '1' + i;
    if(i > 8) n += 7;
    if(i+1 == x * y) n = '0';
    if(init[i] == '=') n = '=';
    answer.push_back( n );
  }

  make_wall(x,y,init);

  vector< pair<string,int> > qu1,qu2;
  boost::unordered_set<string> dejavu1,dejavu2;

  qu1.push_back(make_pair(init,-1));
  qu2.push_back(make_pair(answer,-1));

  unsigned long long int i = 0;

  int dir = -1;

  for( i = 0 ;; i++ ){
    int pos,pos_after;

    /* init => answer */
    string board1 = qu1.at(i).first;
    if(dejavu2.find(board1) != dejavu2.end()){
      dir = 0;
      break;
    }
    /* answer => init */
    string board2 = qu2.at(i).first;
    if(dejavu1.find(board2) != dejavu1.end()){
      dir = 1;
      break;
    }

    /* init => answer */
    pos = board1.find('0');
    for(int j = 0 ; (pos_after = wall[pos][j]) != -1 ; j++ ){
      string next = board1;
      next[pos] = next[pos_after];
      next[pos_after] = '0';
      if( dejavu1.insert(next).second ){
        qu1.push_back(make_pair(next,i));
      }
    }

    /* answer => init */
    pos = board2.find('0');
    for(int j = 0 ; (pos_after = wall[pos][j]) != -1 ; j++ ){
      string next = board2;
      next[pos] = next[pos_after];
      next[pos_after] = '0';
      if( dejavu2.insert(next).second ){
        qu2.push_back(make_pair(next,i));
      }
    }
  }
  
  cout << "clear : " << i << " (*2)  [dir = " << dir << "]" << endl;
  string final_answer = "";
  if( dir == 0 ){
    for( int j = i ; qu1.at(j).second != -1 ; j = qu1.at(j).second){
      pair<string,int> _now = qu1.at(j);
      pair<string,int> prev = qu1.at(_now.second);
      int mov = _now.first.find('0') - prev.first.find('0');
      if     ( mov == -1 ) final_answer.push_back('L');
      else if( mov == +1 ) final_answer.push_back('R');
      else if( mov == -x ) final_answer.push_back('U');
      else if( mov == +x ) final_answer.push_back('D');
    } 
    reverse( final_answer.begin(),final_answer.end() );

    vector< pair<string,int> >::reverse_iterator it = qu2.rbegin();
    unsigned long long int k = dejavu2.size();
    while( it != qu2.rend()){
      if( qu2.at(k).first == qu1.at(i).first ){
        break;
      }
      it++;
      k--;
    }

    for( int j = k ; qu2.at(j).second != -1 ; j = qu2.at(j).second){
      pair<string,int> _now = qu2.at(j);
      pair<string,int> prev = qu2.at(_now.second);
      int mov = _now.first.find('0') - prev.first.find('0');
      if     ( mov == -1 ) final_answer.push_back('R');
      else if( mov == +1 ) final_answer.push_back('L');
      else if( mov == -x ) final_answer.push_back('D');
      else if( mov == +x ) final_answer.push_back('U');
    } 
    cout << final_answer << endl;
  }
  else{
    vector< pair<string,int> >::reverse_iterator it = qu1.rbegin();
    unsigned long long int k = dejavu1.size();
    while( it != qu1.rend()){
      if( qu1.at(k).first == qu2.at(i).first ){
        break;
      }
      it++;
      k--;
    }

    for( int j = k ; qu1.at(j).second != -1 ; j = qu1.at(j).second){
      pair<string,int> _now = qu1.at(j);
      pair<string,int> prev = qu1.at(_now.second);
      int mov = _now.first.find('0') - prev.first.find('0');
      if     ( mov == -1 ) final_answer.push_back('L');
      else if( mov == +1 ) final_answer.push_back('R');
      else if( mov == -x ) final_answer.push_back('U');
      else if( mov == +x ) final_answer.push_back('D');
    } 
    reverse( final_answer.begin(),final_answer.end() );

    for( int j = i ; qu2.at(j).second != -1 ; j = qu2.at(j).second){
      pair<string,int> _now = qu2.at(j);
      pair<string,int> prev = qu2.at(_now.second);
      int mov = _now.first.find('0') - prev.first.find('0');
      if     ( mov == -1 ) final_answer.push_back('R');
      else if( mov == +1 ) final_answer.push_back('L');
      else if( mov == -x ) final_answer.push_back('D');
      else if( mov == +x ) final_answer.push_back('U');
    } 
    cout << final_answer << endl;

 }

  exit(0);

}

