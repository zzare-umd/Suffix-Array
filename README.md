# Suffix-Array

--Language: C++11.2

--Reference to the lectures and online resources used to implement the querysa() function:

    https://rob-p.github.io/CMSC858D_S22/static_files/presentations/CMSC858D_S22_11.pdf
    
    https://www.mimuw.edu.pl/~szczurek/TSG2/04_suffix_arrays.pdf
    
    https://stackoverflow.com/questions/49500809/find-all-occurrences-using-binary-search-in-a-suffix-array
    
-- Running the buildsa(): 

    g++ buildsa.cpp -ldivsufsort -I cereal-master/include/ -o buildsa.out
    
    ./buildsa.out --reference [address of input reference genome] --output [address of saving the output binary file]
    
-- Running the querysa(): 

    g++ querysa.cpp -I cereal-master/include/ -o querysa.out
    
    ./querysa.out --index [address of the input file] --queries [query file] --output [address of saving the output] --query_mode [naive/simpaccel] --method [1/2]
    --range [short/long]
    
    
