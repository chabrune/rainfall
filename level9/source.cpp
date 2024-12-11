class N {
private:
    char annotation[100];  // offset 0x4
    int number;           // offset 0x68

public:
    N(int n) {
        number = n;
    }

    void setAnnotation(char* str) {
        memcpy(annotation, str, strlen(str));
    }

    // Virtual function table implies there's a virtual method
    virtual void operator()(N* other) = 0;
};

int main(int argc, char** argv) {
    if (argc <= 1) {
        exit(1);
    }

    N* n1 = new N(5);
    N* n2 = new N(6);

    n1->setAnnotation(argv[1]);
    (*n2)(n1);

    return 0;
}