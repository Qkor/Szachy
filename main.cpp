#include "game.cpp"

void play_mode(){
        cout<<"move in SAN/LAN notation - make move\nn - new game\ns [file][rank][figure] - set figure\nc - clear board\nk - change side on move\nq - quit\n\n";
        Position board;
        board.new_game();
        cout<<"White on move\n\n";
        board.display_board();
        string x;
        bool color = 1;
        while(true){
            cin>>x;
            if(x.size()==1)
                switch(x[0]){
                case('q'):
                    return;
                case('n'):
                    board.new_game();
                    color = 1;
                    break;
                case('s'):
                    cin>>x;
                    board.set_fig(x);
                    break;
                case('c'):
                    board.clear_board();
                    break;
                case('k'):
                    color = !color;
                    break;
                default:
                    cout<<"move in SAN/LAN notation - make move\nn - new game\ns [file][rank][figure] - set figure\nc - clear board\nk - change side on move\nq - quit\n\n";
                    break;
                }
            else{
                x=board.san2lan(x,color);
                if(board.make_move(x))
                    color = !color;
            }
            cout<<"\n"<<(color?"White":"Black")<<" on move\n\n";
            board.display_board();
        }
}

void pgn_view(){
        cout<<"Input file name/path: \n";
        Game game;
        string path;
        cin>>path;
        while(!game.import_PGN(path.c_str())){
            cout<<"File could not be opened, try again? (y/n) \n";
            cin>>path;
            if(path[0]=='n'||path[0]=='N')
                return;
            else{
                cout<<"Input file name/path: \n";
                cin>>path;
            }
        }

        cout<<"File loaded successfully!\n";
        cout<<"\n1.Move back \n2.Move forward \n3.Move forward [variation nr] \n4.Display game tags \n5.Display move list \n6.Display position \n7.Quit\n\n";

        game.display_tags();
        game.display_moves();
        cout<<'\n';
        game.display_position();

        char x;int y;

        while(true){
            cin>>x;
            switch(x){
            case('1'):
                game.move_back();
                game.display_current_move();
                game.display_position();
                break;
            case('2'):
                game.move_forward();
                game.display_current_move();
                game.display_position();
                break;
            case('3'):
                cin>>y;
                game.move_forward(y);
                game.display_current_move();
                game.display_position();
                break;
            case('4'):
                game.display_tags();
                break;
            case('5'):
                game.display_moves();
                cout<<'\n';
                break;
            case('6'):
                game.display_position();
                break;
            case('7'):
                return;
            default:
                cout<<"\n1.Move back \n2.Move forward \n3.Move forward [variation nr] \n4.Display game tags \n5.Display move list \n6.Display position \n7.Quit\n\n";
                break;
            }
        }
}

int main(){
    cout<<"1.Chessboard \n2.PGN viewer \n3.Quit \n";
    char x;
    while(1){
        cin>>x;
        switch(x){
        case('1'):
            play_mode();
            cout<<"\n1.Chessboard \n2.PGN viewer \n3.Quit \n";
            break;
        case('2'):
            pgn_view();
            cout<<"\n1.Chessboard \n2.PGN viewer \n3.Quit \n";
            break;
        case('3'):
            return 0;
        default:
            break;
        }
    }

}
