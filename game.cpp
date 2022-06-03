#include "board.cpp"

class Move{
    public:
    int nr=0;
    string san;
    string lan;
    string commentary;
    string adnotation;
    Move* previous;
    list<Move*> next;
    bool white = 1;
};


class Game{
    map<string,string> tags;
    Move* first_move;
    Move* current_move;
    Position chessboard;

public:

    void add_tag(string line){
        string tag_name = line.substr(1, line.find(' ') - 1);
        string tag_value = line.substr(line.find('"') + 1);
        tag_value.erase(tag_value.size() - 2);
        tags[tag_name] = tag_value;
    }
    void display_tags(){
        cout<<"-----------------------\n";
        for (map<string,string>::iterator it=tags.begin(); it!=tags.end(); ++it)
            cout<<it->first<<": "<<it->second<<'\n';
        cout<<"-----------------------\n";
    }
    void current_move_null(){
        if(first_move==NULL)
            return;
        if(current_move==NULL){
            current_move = first_move;
            chessboard.new_game();
        }
    }
    void san2lan(Move* curr, Position bd){
        if(curr==NULL)
            return;
        curr->lan = bd.san2lan(curr->san, curr->white);
        bd.make_move(curr->lan);
        for(list<Move*>::iterator it=(*curr).next.begin();it!=(*curr).next.end();++it){
            san2lan(*it,bd);
        }
    }
    void san2lan_all(){
        if(first_move == NULL)
            return;
        Position bd;
        bd.new_game();
        san2lan(first_move,bd);
    }
    string get_current_move(){
        if(current_move == NULL)
            return "";
        return current_move->lan;
    }
    string get_current_move_san(){
        if(current_move == NULL)
            return "";
        return current_move->san;
    }
    void display_current_move(){
        if(current_move==NULL)
            return;
        cout<<current_move->nr<<(current_move->white?".":"...")<<current_move->san<<'\n';
    }
    bool get_current_color(){
        if(current_move == NULL)
            return 0;
        return current_move->white;
    }
    void display_moves(Move* curr = NULL){
        if(first_move == NULL)
            return;
        if(curr == NULL)
            curr = first_move;
        if((*curr).white)
            cout<<(*curr).nr<<'.';
        cout<<(*curr).san<<' ';
        for(list<Move*>::iterator it=(*curr).next.begin();it!=(*curr).next.end();++it){
            if((*curr).next.size()==1)
               display_moves(*((*curr).next.begin()));
            if(it != (*curr).next.begin()){
                cout<<"(";
                display_moves(*it);
                cout<<")";
            }
        }
        if((*curr).next.size()>1){
            display_moves(*((*curr).next.begin()));
        }
    }
    bool import_PGN(string path){                                    //przetwarza pgn na tablice posuniec
        string line, aux;
        vector<string> move_list;
        ifstream file;
        file.open(path.c_str());
        if(!file.is_open())
            return 0;
        while(getline(file,line)){
            if(line[0]=='[')
                this->add_tag(line);
            else break;
        }
        while(file >> line){

            if(line[0] == '{')
                while(line[line.size()-1] != '}'){
                    file >> aux;
                    line += ' ' + aux;
                }

            move_list.push_back(line);
        }

        variation(move_list,NULL);

        file.close();
        return 1;
    }
    void variation(vector<string> move_list, Move* prev=NULL){      //przetwarza tablice posuniec na drzewo wariantow
        Move* new_move;
        for(int i=0;i<move_list.size();i++){
            new_move = new Move;
            if(move_list[i][0] == '('){
                int brackets = 1;
                vector<string> subvariation;
                i++;
                while(brackets > 0 && i<move_list.size()){
                    if(move_list[i][0] == ')')
                        brackets--;
                    else if(move_list[i][0] == '(')
                        brackets++;

                    subvariation.push_back(move_list[i]);
                    i++;
                }
                subvariation.pop_back();
                variation(subvariation, prev->previous);
                --i;continue;
            }

            else if(move_list[i][move_list[i].size() - 1] == '.'){
                continue;
            }

            else if(move_list[i][0] == '$')
                prev->adnotation = move_list[i];
            else if(move_list[i][0] == '{')
                prev->commentary = move_list[i];

            else if(move_list[i][0] == '1' || move_list[i][0] == '0' || move_list[i][0] == '*')
                break;

            else{
                new_move->san = move_list[i];
                new_move->previous = prev;
                if(prev != NULL){
                    new_move->white = !(prev->white);
                    if(new_move->white)
                        new_move->nr = prev->nr + 1;
                    else
                        new_move->nr = prev->nr;
                }
                else{
                    new_move->nr = 1;
                    new_move->white = 1;
                }
                if(first_move == NULL)
                    first_move = new_move;
                if(prev!=NULL)
                    (*prev).next.push_back(new_move);
                prev = &*(new_move);
            }
        }
        san2lan_all();
    }
    void move_back(){
        if(first_move==NULL||current_move == NULL)
            return;
        if(current_move->previous!=NULL){
            current_move = (*current_move).previous;
            stack<Move> previous_moves;
            Move* prev = current_move;
            while(prev != NULL){
                previous_moves.push(*prev);
                prev = prev->previous;
            }
            chessboard.new_game();
            while(!previous_moves.empty()){
                chessboard.make_move(previous_moves.top().lan);
                previous_moves.pop();
            }
        }

        else {
            chessboard.new_game();
            current_move = NULL;
        }

    }
    void move_forward(int v=0){                 //przyjmuje nr wariantu
        if(first_move == NULL)
            return;
        if(current_move == NULL){
            current_move_null();
            chessboard.make_move(get_current_move());
        }

        else if(current_move->next.size()>0){
            int i=0;
            for(list<Move*>::iterator it=(*current_move).next.begin();it!=(*current_move).next.end();++it){
                if(i==v){
                    current_move = (*it);
                    break;
                }
                i++;
            }
            chessboard.make_move(get_current_move());
        }
    }
    void go_to_move(Move* m){
        if(first_move==NULL||m == NULL)
            return;
        current_move = m;
        if(current_move!=NULL){
            stack<Move> previous_moves;
            Move* prev = current_move;
            while(prev != NULL){
                previous_moves.push(*prev);
                prev = prev->previous;
            }
            chessboard.new_game();
            while(!previous_moves.empty()){
                chessboard.make_move(previous_moves.top().lan);
                previous_moves.pop();
            }
        }
        else {
            chessboard.new_game();
            current_move = NULL;
        }
    }
    void display_position(){
        chessboard.display_board();
    }
    Game(){
        first_move = NULL;
        current_move = NULL;
        chessboard.new_game();
        add_tag("[Site \"?\"]");
        add_tag("[Date \"?\"]");
        add_tag("[Round \"?\"]");
        add_tag("[White \"?\"]");
        add_tag("[Black \"?\"]");
        add_tag("[Result \"?\"]");
    }
};
