
// struct declarations
struct s_test;
struct s_sptr;

// anonymous struct declarations

// type definitions

// struct definitions
struct s_test {
    unsigned int structVar0;
};
struct s_sptr {
    struct s_test* structVar1;
};

// union definitions

// function declarations
int main(int var0, char** var1);
extern unsigned long strtol(unsigned char* var0, unsigned char** var1, unsigned int var2);

// global variable definitions

int main(int var0, char** var1){
    unsigned int var2;
    unsigned int argc;
    unsigned char** argv;
    unsigned char* p;
    unsigned long num;
    struct s_test t;
    struct s_sptr s;
    block0: ;
    var2 = 0;
    argc = var0;
    argv = var1;
    if (argc != 2) {
        var2 = -1;
        return var2;
    } else {
        num = strtol(*(((unsigned char**)(argv)) + 1), &p, 10);
        if (((int)(*p)) != 0) {
            var2 = -1;
            return var2;
        } else {
            (t.structVar0) = ((unsigned int)num);
            (s.structVar1) = (&t);
            var2 = ((s.structVar1)->structVar0);
            return var2;
        }
    }
}


