#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
using namespace std;
int width = 10;
int height = 20;

enum block_t {I,O,L,J,T,S,Z,n_blocks};

vector<vector<vector<int>>> tetrominoes = {
    { {1,1,1,1} },                        // I
    { {1,1},{1,1} },                      // O
    { {1,0,0},{1,1,1} },                  // L
    { {0,0,1},{1,1,1} },                  // J
    { {0,1,0},{1,1,1} },                  // T
    { {0,1,1},{1,1,0} },                  // S
    { {1,1,0},{0,1,1} }                   // Z
};

struct Point { int x,y; };
struct Block { int type; vector<vector<int>> shape; Point pos; };
class Board {
    vector<vector<int>> grid;
public:
    Board() : grid(height, vector<int>(width,0)) {}
    bool canPlace(Block b, int dx=0, int dy=0) {
        for(int i=0;i<b.shape.size();i++)
            for(int j=0;j<b.shape[i].size();j++)
                if(b.shape[i][j]) {
                    int x=b.pos.x+j+dx,y=b.pos.y+i+dy;
                    if(x<0||x>=width||y>=height) return false;
                    if(y>=0&&grid[y][x]) return false;
                }
        return true;
    }
    void place(Block b) {
        for(int i=0;i<b.shape.size();i++) {
            for(int j=0;j<b.shape[i].size();j++) {
                if(b.shape[i][j]) {
                    int x=b.pos.x+j,y=b.pos.y+i;
                    if(y>=0) grid[y][x]=1;
                }
            }
        }
    }

    void clearLines() {
        for(int i=height-1;i>=0;i--) {
            bool full=1;
            for(int j=0;j<width;j++) if(!grid[i][j]) full=false;
            if(full) {
                for(int k=i;k>0;k--) grid[k]=grid[k-1];
                grid[0]=vector<int>(width,0);
                i++;
            }
        }
    }

    void draw(Block b) {
        system("clear");
        vector<vector<int>> disp=grid;
        for(int i=0;i<b.shape.size();i++) {
            for(int j=0;j<b.shape[i].size();j++) {
                if(b.shape[i][j]) {
                    int x=b.pos.x+j, y=b.pos.y+i;
                    if(y>=0&&y<height) disp[y][x]=1;
                }
            }
        }
        for(int i=0;i<height;i++) {
            cout<<"|";
            for(int j=0;j<width;j++) cout<<(disp[i][j]?'#':' ');
            cout<<"|\n";
        }
        for(int j=0;j<width+2;j++) cout<<"-";
        cout<<"\n";
    }
};

int kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch!=EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

Block newBlock() {
    int t=rand()%n_blocks;
    Block b{t,tetrominoes[t],{width/2-2,0}};
    return b;
}

void rotate(Block& b) {
    vector<vector<int>> rot(b.shape[0].size(), vector<int>(b.shape.size()));
    for(int i=0;i<b.shape.size();i++) {
        for(int j=0;j<b.shape[i].size();j++) {
            rot[j][b.shape.size()-1-i]=b.shape[i][j];
        }
    }
    b.shape=rot;
}

int main() {
    srand(time(0));
    Board board;
    Block block=newBlock();
    while(1) {
        board.draw(block);
        if(kbhit()) {
            char c=getchar();
            if(c=='a') if(board.canPlace(block,-1,0)) block.pos.x--;
            if(c=='d') if(board.canPlace(block,1,0)) block.pos.x++;
            if(c=='s') if(board.canPlace(block,0,1)) block.pos.y++;
            if(c=='w') {
                Block tmp=block;
                rotate(tmp);
                if(board.canPlace(tmp)) {
                    block=tmp;
                }
            }
            if(c==' ') while(board.canPlace(block,0,1)) block.pos.y++;
        }

        if(board.canPlace(block,0,1)) block.pos.y++;
        else {
            board.place(block);
            board.clearLines();
            block=newBlock();
            if(!board.canPlace(block)) {
                cout<<"Game Over\n";
                break;
            }
        }
        usleep(300000);
    }
}
