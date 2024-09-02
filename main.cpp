#include <iostream>
#include <dlfcn.h> // For dlopen, dlsym, dlclose
#include <string>
#include <fstream>

int main() {
    std::cout << "Starting the main program\n";

    // CREATING A SHARED LIBRARY ----------------------------
    std::string program = "#include <iostream>\n extern \"C\" void my_function() { std::cout << \"Hello from the shared library!\\n\"; }";
    
    std::string filename = "mylibrary.cpp";
    std::ofstream out(filename);
    out << program;
    out.close();

    std::string command = "g++ -shared -fPIC -o mylibrary.so " + filename;
    system(command.c_str());


    // OPENING A SHARED LIBRARY ----------------------------
    // Open the shared library
    void* handle = dlopen("./mylibrary.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    // Reset errors
    dlerror();

    // Load the symbol (function)
    typedef void (*my_function_t)();
    my_function_t my_function = (my_function_t) dlsym(handle, "my_function");

    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol 'my_function': " << dlsym_error << '\n';
        dlclose(handle);
        return 1;
    }

    // Use the function
    my_function();

    // Close the library
    dlclose(handle);

    std::cout << "End of the main program\n";

    return 0;
}