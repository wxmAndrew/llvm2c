
// struct declarations

// anonymous struct declarations

// type definitions

// struct definitions

// union definitions

// function declarations
int main(void);

// global variable definitions

int main(void){
    unsigned int var0;
    unsigned long result;
    block0: ;
    var0 = 0;
    __asm__("mov    $42, %%rbx;   \n\tmov    %%rbx, %0;    \n\t"
        : "=c" (result)
        : 
        : "%rbx"
    );
    return (unsigned int)result;
}


