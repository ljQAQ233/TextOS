int foo() {
    return 1;
}

int bar() {
    return foo();
}

int main() {
    char a[] = "Hello World!";
    char b[10];
    return bar();
}
