#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<list>
#include<stack>
using namespace std;

string lan2coord(string in){
    string out="";
    out+=56-in[1];
    out+=in[0]-97;

    if(in.size()>3){
        out+=56-in[3];
        out+=in[2]-97;
    }
    if(in.size()==3)
        out+=in[2];
    else if(in.size()==5)
        out+=in[4];

    return out;
}

class Position{
    char board[8][8];
    string en_passant;

public:
    void clear_board(){
        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++)
                board[i][j]='.';
    }
    void display_board(){
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++)
                cout<<board[i][j]<<' ';
            cout<<'\n';}
    }
    void fen2position(string fen){
        clear_board();
        int f=0;
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(fen[f]<=56&&fen[f]>48)
                    j+=fen[f]-49;
                else if(fen[f]=='/')
                    j--;
                else
                    board[i][j]=fen[f];
                f++;
            }
        }
        f++;
        //properties(fen.substr(f));
    }
    string position2fen(){                  //do dopracowania
        string fen="";
        char q;
        for(int i=0;i<8;i++){
            q=0;
            for(int j=0;j<8;j++){
                if(board[i][j]=='.')
                    q++;
                else{
                    if(q>0){
                        fen+=(char)q+48;
                        q=0;
                    }
                    fen+=board[i][j];
                }
            }
            if(q>0)
                fen+=(char)q+48;
            fen+='/';
        }
        fen[fen.size()-1]=' ';
        cout<<fen<<'\n';
        return fen;
    }
    void new_game(){
        fen2position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
    void set_fig(string s){
        s = lan2coord(s);
        board[s[0]][s[1]]=s[2];
    }
    bool legal(string m){                   //sprawdza legalnosc ruchu, nie sprawdza szacha
        if(m[0]<'a'||m[0]>'h'||m[2]<'a'||m[2]>'h'||m[1]<'1'||m[1]>'8'||m[3]<'1'||m[3]>'8')
            return 0;
        if(m[0]==m[2]&&m[1]==m[3])
            return 0;
        m = lan2coord(m);
        char fig = board[m[0]][m[1]];
        char target = board[m[2]][m[3]];
        if(fig>='a'&&fig<='z'&&target>='a'&&target<='z')
            return 0;
        if(fig>='A'&&fig<='Z'&&target>='A'&&target<='Z')
            return 0;

        switch(fig){
        case('k'):
            if(m[0]==0&&m[1]==4){
                if(m[2]==0&&m[3]==6){
                    if(board[0][5]!='.')
                        return 0;
                    return 1;
                }
                else if(m[2]==0&&m[3]==2){
                    if(board[0][3]!='.')
                        return 0;
                    return 1;
                }
            }

            if(abs(m[0]-m[2])>1 || abs(m[1]-m[3])>1)
                return 0;
            return 1;
        case('K'):
            if(m[0]==7&&m[1]==4){
                if(m[2]==7&&m[3]==6){
                    if(board[7][5]!='.')
                        return 0;
                    return 1;
                }
                else if(m[2]==7&&m[3]==2){
                    if(board[7][3]!='.')
                        return 0;
                    return 1;
                }
            }
            if(abs(m[0]-m[2])>1 || abs(m[1]-m[3])>1)
                return 0;
            return 1;

        case('p'):
            if(m[3]==en_passant[0]-97&&m[2]==56-en_passant[1]){
                if(m[0]==4&&abs(m[1]-m[3])==1)
                    return 1;
                return 0;
            }
            if(abs(m[1]-m[3])==1&&m[0]+1==m[2]&&board[m[2]][m[3]]>='A'&&board[m[2]][m[3]]<='Z')
                return 1;
            if(m[1]==m[3]&&board[m[2]][m[3]]=='.'){
                if(m[0]==1&&m[2]==3&&board[2][m[1]]=='.')
                    return 1;
                if(m[0]+1==m[2])
                    return 1;
            }
            return 0;

        case('P'):
            if(m[3]==en_passant[0]-97&&m[2]==56-en_passant[1]){
                if(m[0]==3&&abs(m[1]-m[3])==1)
                    return 1;
                return 0;
            }
            if(abs(m[1]-m[3])==1&&m[0]==m[2]+1&&board[m[2]][m[3]]>='a'&&board[m[2]][m[3]]<='z')
                return 1;
            if(m[1]==m[3]&&board[m[2]][m[3]]=='.'){
                if(m[0]==6&&m[2]==4&&board[5][m[1]]=='.')
                    return 1;
                if(m[0]==m[2]+1)
                    return 1;
            }
            return 0;

        default: break;
        }

        if(fig<91)
            fig+=32;

        switch(fig){

        case('b'):{
            if(abs(m[0]-m[2])!=abs(m[1]-m[3]))
                return 0;
            char j = min(m[0],m[2]) + 1;
            if((m[0]<m[2]&&m[1]<m[3])||(m[2]<m[0]&&m[3]<m[1]))
                for(char i = min(m[1],m[3]) + 1;i < max(m[1],m[3]);++i){
                    if(board[j][i] != '.')
                        return 0;
                    j++;
                }
            else
                for(char i = max(m[1],m[3]) - 1;i > min(m[1],m[3]);--i){
                    if(board[j][i] != '.')
                        return 0;
                    j++;
                }
            return 1;
        }

        case('r'):
            if(m[0]==m[2]){
                for(char i = min(m[1],m[3]) + 1; i < max(m[1],m[3]); i++)
                    if(board[m[0]][i]!='.')
                        return 0;
                return 1;
            }
            if(m[1]==m[3]){
                for(char i = min(m[0],m[2]) + 1; i < max(m[0],m[2]); i++)
                    if(board[i][m[1]]!='.')
                        return 0;
                return 1;
            }
            return 0;

        case('q'):{
            if(m[0]==m[2]){
                for(char i = min(m[1],m[3]) + 1; i < max(m[1],m[3]); i++)
                    if(board[m[0]][i]!='.')
                        return 0;
                return 1;
            }
            if(m[1]==m[3]){
                for(char i = min(m[0],m[2]) + 1; i < max(m[0],m[2]); i++)
                    if(board[i][m[1]]!='.')
                        return 0;
                return 1;
            }

            if(abs(m[0]-m[2])!=abs(m[1]-m[3]))
                return 0;
            char j = min(m[0],m[2]) + 1;
            if((m[0]<m[2]&&m[1]<m[3])||(m[2]<m[0]&&m[3]<m[1]))
                for(char i = min(m[1],m[3]) + 1;i < max(m[1],m[3]);++i){
                    if(board[j][i] != '.')
                        return 0;
                    j++;
                }
            else
                for(char i = max(m[1],m[3]) - 1;i > min(m[1],m[3]);--i){
                    if(board[j][i] != '.')
                        return 0;
                    j++;
                }
            return 1;
        }

        case('n'):
            if(abs(m[0]-m[2])==2&&abs(m[1]-m[3])==1)
                return 1;
            if(abs(m[0]-m[2])==1&&abs(m[1]-m[3])==2)
                return 1;
            return 0;

        default: return 0;
    }
}
    bool make_move(string mov){
        if(!legal(mov))
            return 0;
        mov = lan2coord(mov);
        if(mov.size()==5)
            board[mov[2]][mov[3]]=mov[4];
        else
            board[mov[2]][mov[3]]=board[mov[0]][mov[1]];
        board[mov[0]][mov[1]] = '.';

        if(board[mov[2]][mov[3]]=='k'){                         // roszada czarnego
            if(mov[0]==0&&mov[1]==4&&mov[2]==0&&mov[3]==6){         // 0-0
                board[0][5]='r';
                board[0][7]='.';
            }
            else if(mov[0]==0&&mov[1]==4&&mov[2]==0&&mov[3]==2){    // 0-0-0
                board[0][3]='r';
                board[0][0]='.';
            }
        }
        if(board[mov[2]][mov[3]]=='K'){                         // roszada bialego
            if(mov[0]==7&&mov[1]==4&&mov[2]==7&&mov[3]==6){         // 0-0
                board[7][5]='R';
                board[7][7]='.';
            }
            else if(mov[0]==7&&mov[1]==4&&mov[2]==7&&mov[3]==2){    // 0-0-0
                board[7][3]='R';
                board[7][0]='.';
            }
        }

        if(board[mov[2]][mov[3]]=='P'&&56-en_passant[1]==mov[2]&&en_passant[0]-97==mov[3]){         //bicie w przelocie
            board[mov[2]+1][mov[3]]='.';
        }
        else if(board[mov[2]][mov[3]]=='p'&&56-en_passant[1]==mov[2]&&en_passant[0]-97==mov[3]){
            board[mov[2]-1][mov[3]]='.';
        }

        if(board[mov[2]][mov[3]]=='P'&&mov[0]-mov[2]==2){
            en_passant="";
            en_passant+=char(mov[3]+97);
            en_passant+=char(55-mov[2]);
        }
        else if(board[mov[2]][mov[3]]=='p'&&mov[2]-mov[0]==2){
            en_passant="";
            en_passant+=char(mov[3]+97);
            en_passant+=char(57-mov[2]);
        }
        else en_passant="-";

        return 1;
    }
    string find_missing(char figure, string lan, bool white){       //na razie zrobione dosc lopatologicznie, do optymalizacji
        if(lan[0]!='?'&&lan[1]!='?')
            return lan;

        if(figure=='p'){
            if(lan[0]!='?')
                lan[1]=lan[3]+1;
            else{
                lan[0]=lan[2];
                if(board[56-lan[3]-1][lan[2]-97]=='p')
                    lan[1]=lan[3]+1;
                else lan[1]=lan[3]+2;
            }
            return lan;
        }
        else if(figure=='P'){
            if(lan[0]!='?')
                lan[1]=lan[3]-1;
            else{
                lan[0]=lan[2];
                if(board[56-lan[3]+1][lan[2]-97]=='P')
                    lan[1]=lan[3]-1;
                else lan[1]=lan[3]-2;
            }
            return lan;
        }

        if(lan[0]=='?'&&lan[1]=='?'){
            for(char i='a';i<='h';i++)
                for(char j='1';j<='8';j++)
                    if(board[56-j][i-97]==figure){
                        lan[0]=i;
                        lan[1]=j;
                        if(legal(lan))
                            return lan;
                    }
        }
        if(lan[0]=='?'){
            for(char i='a';i<='h';i++)
                if(board[56-lan[1]][i-97]==figure){
                    lan[0]=i;
                    if(legal(lan))
                        return lan;
                }
        }
        if(lan[1]=='?'){
            for(char i='1';i<='8';i++)
                if(board[56-i][lan[0]-97]==figure){
                    lan[1]=i;
                    if(legal(lan))
                        return lan;
                }
        }

        return "error";
    }
    string san2lan(string san, bool white = 0){
        if(san[0] == 'O'){                    //O-O
            if(white&&san.size()>4)
                return "e1c1";
            if(white)
                return "e1g1";
            if(san.size()>4)
                return "e8c8";
            return "e8g8";
        }

        int i = san.size()-1;
        string lan="";
        if(san[i]=='+'||san[i]=='#')        // mat i szach
            i--;
        if(san[i]>56){                      //promocja
            lan += san[i];
            i--;
            if(san[i]=='=')
                i--;
        }
        lan = san[i] + lan;                 //pole docelowe
        i--;
        lan = san[i] + lan;
        i--;
        if(san[i]=='x')
            i--;
        if(san[i]>48&&san[i]<57){           //pole startowe
            lan = san[i] + lan;
            i--;
        }
        else lan = '?' + lan;
        if(san[i]>96){
            lan = san[i] + lan;
            i--;
        }
        else lan = '?' + lan;

        char figure;
        if(i==0)
            figure = san[i];
        else
            figure = 'P';

        if(!white)
            figure+=32;

        lan = find_missing(figure, lan, white);

        return lan;

    }

};

