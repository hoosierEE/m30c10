/* m30c9 -- keyboard firmware for Teensy 3.0
   https://github.com/hoosierEE/m30c9
*/
#include <Arduino.h>
#include <array>

// Output data rate limit: 10ms
const uint32_t MS_OUT{10};
elapsedMillis t;

/* Pins */
static const uint8_t COLS{5};
static const uint8_t ROWS{6};
static const uint8_t CAPS{9};
const int teensy_cap_pin[CAPS]{0,1,15,16,17,18,19,22,23};
const int teensy_row_pin[ROWS]{10,11,12,14,20,21};
const int teensy_col_pin[COLS]{2,5,6,7,8};


/* Model */
struct Grid{bool keys[COLS][ROWS];} grid;
class Layer{
public:
    Layer(const char *str){
        int k=0;
        for(int i=0;i<COLS;++i){
            for(int j=0;j<ROWS;++j){
                grid[i][j]=str[k];
                k++;
            }
        }
    }
    char grid[COLS][ROWS];
};

Layer defl("qwertyuiopasdfghjkl'zxcvbnm,.-");

const std::array<char,3>greet{"hi"};
const std::array<std::array<char,3>,2> greetings{{"hi","yo"}};
const std::array<std::array<char,6>,6> default_layer{{"qwert","yuiop","asdfg","hjkl'","zxcvb","nm,.-"}};
const std::array<std::array<char,6>,6> shifted_layer{{"QWERT","YUIOP","ASDFG","HJKL\"","ZXCVB","NM?!:"}};
const std::array<std::array<char,6>,6> numeric_layer{{"75319","02468","12345","67890","%/:-(",")+,.*"}};
const std::array<std::array<char,6>,6> symbols_layer{{"@#$%[","]^&*|"," -+=(",");:/\\","_~`!{","}<,.>"}};


/* Functions */
void set_pins(void){
  for(auto &i:teensy_col_pin){
    //for(int i=0;i<COLS;++i){
        pinMode(teensy_col_pin[i],OUTPUT);
        for(auto &j:teensy_row_pin){
        //for(int j=0;j<ROWS;++j){
            pinMode(teensy_row_pin[j], INPUT_PULLUP);
        }
    }
}

Grid scan(void){
    Grid g;
    for(int i=0;i<COLS;++i){
        digitalWrite(teensy_col_pin[i],LOW);// pull column low
        for(int j=0;j<ROWS;++j){
            g.keys[i][j]=digitalRead(teensy_row_pin[i]);// populate grid
        }
        digitalWrite(teensy_col_pin[i],HIGH);// restore column
    }
    return g;
}

// for debugging
void print_grid(Grid g){
    for(int i=0;i<COLS;++i){
        for(int j=0;j<ROWS;++j){
            // FIXME if(g.keys[i][j]){Serial.print(default_layer[i][j]);}
        }
    }
    Serial.println("");
}


/* Program */
void setup(){
    set_pins();
}

void loop(){
    print_grid(scan());// read inputs
    if(t>=MS_OUT){
        t-=MS_OUT;
        // TODO: output if changed
    }
}
