#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#define SPACE_WIDTH 1000
#define SPACE_HEIGHT 1000
using namespace std;

char code[SPACE_WIDTH][SPACE_HEIGHT] ={0};
int memory[SPACE_WIDTH][SPACE_HEIGHT] = {0};
int N;
int width,height;
const int dx[8] ={};
const int dy[8] ={};
bool if_DEBUG = false;
int get_input(){
	int c;
	if((c=getchar())== EOF)
		return -1;
	return c;
}
class player{
public:
	int n = 0;
	int power = 0;
	int dx = 1;
	int dy = 0;
	bool valid = false;
	int x,y;
	int xo,yo;
	bool move(){
		x += dx;
		y += dy;
		return (x >= 0 && x < width && y>= 0 && y < height);
	}
	void return_home(bool if_get_input){
		x = xo;
		y = yo;
		if(if_get_input){
			n = get_input();
		}
	}
	bool if_vertical(){
		return dx*dy == 0;
	}
	void mirror1(){
		if(dx == 0){
			if(dy ==-1){
				dx = -1;
				dy = 1;
			}else if(dy == 1){
				dx = 1;
				dy = -1;
			}
		}else if(dx == 1){
			if (dy == 0){
				dy =  1;
			}else if(dy == -1){
				dx = 0;
				dy = 1;
			}else if(dy == 1){
				dy = 0;
			}
		}else if(dx == -1){
			if (dy == 0){
				dy =-1;
			}else if(dy == -1){
				dy = 0;
			}else if(dy == 1){
				dx = 0;
				dy = -1;
			}		
		}
	
	}
	void mirror2(){
		if(dx == 0){
			if(dy == -1){
				dx =  1;
				dy =  1;
			}else if(dy == 1){
				dx = -1;
				dy = -1;
			}
		}else if(dx == 1){
			if (dy == 0){
				dy = -1;
			}else if(dy == -1){
				dy = 0;
			}else if(dy ==  1){
				dx = 0;
				dy =-1;
			}
		}else if(dx == -1){
			if (dy == 0){
				dy = 1;
			}else if(dy == -1){
				dy = 1;
				dx = 0;
			}else if(dy == 1){
				dy = 0;
			}		
		}	
	
	}
};
player ps[36];

void init_code(){
	for(int i = 0; i < SPACE_WIDTH; i++){
		for(int j = 0; j < SPACE_HEIGHT; j++){
			code[i][j] = ' ';
			memory[i][j] = 0;
		}
	}
}

int input(FILE *fp) {
    int x = 0, y = 0;
    width = height = 0;
	N = 0;
	bool num[36];
	for(int i = 0; i < 36; i++)
		num[i] = false;

    while (!feof(fp)) {
        int c = fgetc(fp);
        if (c == '\n' || c == EOF) {
            if (width < x) {
                width = x;
            }
            x = 0;
            y++;
        } else {
			code[x][y] = c;
			if(isdigit(c) || (c>='a' && c <= 'z')){
				int n;
				if(isdigit(c))
					n = c - '0';
				else
					n = c - 'a'+10;
				if(num[n]){
					cout << "There are some same player in source code." << endl;
					return -1;
				}
				num[n] = true;
				ps[n].valid = true;
				ps[n].xo = x;
				ps[n].yo = y;
				ps[n].x = x;
				ps[n].y = y;
			}
            x++;
        }
    }
    height = y;
	for(int i =0; i < 36; i++){
		if(num[i]){
			N++;
		}
	}
	if(N == 0){
		cout << "Need more than 1 player" << endl;
		return -1;
	}
}

void print_code(){
	for(int j = 0; j < height; j++){
		for(int i = 0; i < width; i++)
			cout<<code[i][j];
		cout << endl;
	}
}

void print_now_map(){
	for(int j = 0; j < height; j++){
		for(int i = 0; i < width; i++){
			bool if_char = false;
			for(int k = 0; k < 36; k++){
				if(ps[k].valid&&ps[k].x == i&&ps[k].y==j){
					if(k < 10)
						cout << (char)('0'+k);
					else
						cout << (char)('a'+k-10);
					if_char = true;
				}
			}
			if(!if_char)
				cout << code[i][j];
		}
		cout << endl;
	}


}

int run(){
	for(int i = 10; i < 36; i++){
		if(!ps[i].valid)
			continue;
		ps[i].n = get_input();
	}
	bool if_valid = true;
	while(if_valid){
		if(if_DEBUG)
			print_now_map();
		for(int i = 0; i <36; i++){
			if(!ps[i].valid){
				continue;
			}
			if(code[ps[i].x][ps[i].y] == '#')
				if(ps[i].n >= 0)
					ps[i].move();
			if(!ps[i].move()){
				cout << "player" << i << "drop out the map at (" <<ps[i].x<<","<<ps[i].y<<")" << endl;
				return -1;
			}
			for(int j = 0; j < 36; j++){
				if(i == j)
					continue;
				if(!ps[j].valid)
					continue;
				if(ps[i].x == ps[j].x && ps[i].y == ps[j].y){
					if(i+ps[i].power < j + ps[j].power){
						ps[i].return_home(i>9);
					}else if( i +ps[i].power > j+ps[j].power){
						ps[j].return_home(j>9);
					}else if(i < j){
						ps[i].return_home(i>9);
					}else{
						ps[j].return_home(j>9);
					}
				}
			}
			char c = code[ps[i].x][ps[i].y];
			switch(c){
			case '+':
				if(ps[i].if_vertical())
					memory[ps[i].x][ps[i].y] = ps[i].n;
				else
					ps[i].n += memory[ps[i].x][ps[i].y];
				break;
			case '-':
				if(ps[i].if_vertical())
					memory[ps[i].x][ps[i].y] = ps[i].n;
				else
					ps[i].n -= memory[ps[i].x][ps[i].y];
				break;
			case '*':
				if(ps[i].if_vertical())
					memory[ps[i].x][ps[i].y] = ps[i].n;
				else
					ps[i].n = memory[ps[i].x][ps[i].y] * ps[i].n;
				break;
			case ':':
				if(ps[i].if_vertical())
					memory[ps[i].x][ps[i].y] = ps[i].n;
				else
					ps[i].n /= memory[ps[i].x][ps[i].y];
				break;
			case '%':
				if(ps[i].if_vertical())
					memory[ps[i].x][ps[i].y] = ps[i].n;
				else
					ps[i].n %= memory[ps[i].x][ps[i].y];
				break;
			case 'U':
				ps[i].power++;
				break;
			case 'D':
				ps[i].power--;
				break;
			case 'O':
				printf("%d",ps[i].n);
				break;
			case 'S':
				printf(" ");
				break;
			case 'E':
				printf("\n");
				break;
			case 'C':
				printf("%c",ps[i].n);
				break;
			case 'N':
				if(isdigit(ps[i].n))
					ps[i].n -= '0';
				break;
			case 'G':
				ps[i].n = i + ps[i].power;
				break;
			case '>':
				if(ps[i].if_vertical())
					ps[i].dy = 0;
				ps[i].dx = 1;
				break;
			case '<':
				if(ps[i].if_vertical())
					ps[i].dy = 0;
				ps[i].dx = -1;
				break;
			case '^':
				if(ps[i].if_vertical())
					ps[i].dx = 0;
				ps[i].dy =-1;
				break;
			case 'V':
				if(ps[i].if_vertical())
					ps[i].dx = 0;
				ps[i].dy = 1;
				break;
			case '/':
				ps[i].mirror1();
				break;
			case '\\':
				ps[i].mirror2();
				break;
			case '@':
				if_valid = false;
				return 0;
				break;
			case '_':
				ps[i].dy *= -1;
				break;
			case '|':
				ps[i].dx *= -1;
				break;
			}
			
			
		}
	}
}


int main(int argc, char *argv[]){
    FILE *fp;
    int i;
	int error = 0;
    char *path = NULL;
	if(argc == 3){
		if(!strcmp(argv[1], "-d"))
			if_DEBUG = true;
		path = argv[2];
	}else{
		path = argv[1];
	}
	init_code();
    fp = fopen(path, "r");
    error = input(fp);
	fclose(fp);
//	print_code();
	if(error == -1)
		return -1;
	error = run();
	if(error == -1)
		return -1;
	return 0;
}
