#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <tuple>
#include <cstring>
#include <divsufsort.h>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <string>
#include <chrono>
//#include <filesystem>
#include <sys/stat.h>

using namespace std;
using namespace std::chrono;



//////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************DEFINED FUNCTIONS***************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

tuple<string,int> read_sequence(string&);
tuple<vector<string>,vector<uint64_t>,vector<uint64_t>> pref_func(string&, int*, int&);


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************MAIN FUNCTION*********************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){

    //Default Parameters
    string reference = "./ecoli4.6M.fa";
    string output = "./out_SA";
    int k = 0;

    //overwrite default by user input
    for(size_t arg_ind = 1; arg_ind < argc; arg_ind++){
        if(argv[arg_ind] == "--reference"){
            reference = argv[arg_ind + 1];
        }else if(argv[arg_ind] == "--output"){
            output = argv[arg_ind + 1];
        }else if(argv[arg_ind] == "--preftab"){
            k = atoi(argv[arg_ind + 1]);
        }
    }

    string sequence;
    int n;


    tie(sequence,n) = read_sequence(reference);
    
    int *SA = (int *)malloc(n * sizeof(int));


    //auto start = high_resolution_clock::now();
    divsufsort((const unsigned char *)sequence.c_str(), SA, n);
    

    //k-mer prefix table
    vector<string> pref_string;
    vector<uint64_t> pref_first, pref_last;

    if( k != 0){
        tie(pref_string, pref_first, pref_last) = pref_func(sequence, SA, k);
    }

    //auto stop = high_resolution_clock::now();
    //duration<double> elapsedTime = (stop - start);
    
    
    //writing the output to a binary file
    vector<int> SA_vec;

    for(size_t j=0; j<n; j++){
        SA_vec.push_back(SA[j]);
    }
    free(SA);
    
    ofstream os(output, std::ios::binary);
    cereal::BinaryOutputArchive archive( os );
    archive(sequence, SA_vec, pref_string, pref_first, pref_last);


    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************READING THE REFERENCE GENOME***************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//reading data function

tuple<string,int> read_sequence(string& seq_address){

    string line, id, sequence , S_rand = "ACGT";
    
    //reading the reference genome file
    ifstream infile(seq_address);

    while (getline(infile,line))
    {
        if (line.empty())
        {
            continue;
        }

        if (line[0] == '>')
        {
            
            id = line.substr(1);
        }
        else
        {
            sequence += line;
        }
    }
    sequence = sequence + "$";
   //checking foe lower case and convert them to upper case and laso check for n/N in the reference
   for (size_t i = 0; i < sequence.size(); i++)
   {
       if (sequence[i] == 'a' || sequence[i] == 'c' || sequence[i] == 'g' || sequence[i] == 't')
       {
           sequence[i] = toupper(sequence[i]);
       }
       else if (sequence[i] == 'n' || sequence[i] == 'N')
       {
          srand(time(0));
          sequence[i] = S_rand[rand()%4];
       }
   }

    return make_tuple(sequence,sequence.size());
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************CONSTRUCTING PREFIX TABLE FUNTION**********************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////


tuple<vector<string>,vector<uint64_t>,vector<uint64_t>> pref_func(string& sequence, int *SA, int& k){

  //vector<vector<uin64_t>> pre_table;
  //vector<uin64_t> vec;
  vector<string> Pref_string;
  vector<uint64_t> Pref_First;
  vector<uint64_t> Pref_Last;
  string K_MER="", K_MER_previous="";
  int counter=0;

  for(size_t i = 1; i < sequence.size(); i++){

    //sequence.size() - SA[i] <k ? continue:K_MER = sequence.substr(SA[i],k);
    if((sequence.size() - SA[i] - 1) <k){
        continue;
    }else{
        K_MER_previous = K_MER;
        K_MER = sequence.substr(SA[i],k);
    }

    if(K_MER == K_MER_previous){
        counter++;
        Pref_Last.pop_back();
        Pref_Last.push_back(Pref_First.back() + counter -1);

    }

    if(K_MER != K_MER_previous){
        counter = 1;
        Pref_string.push_back(K_MER);
        Pref_First.push_back(i);
        Pref_Last.push_back(i + counter -1);
    }
  }  

  return make_tuple(Pref_string, Pref_First, Pref_Last);
}