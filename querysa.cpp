#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <tuple>
#include <cstring>
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <iomanip>
#include <string>
#include <chrono>


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************DEFINED FUNCTIONS***************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//defined functions
std::tuple<std::string, std::vector<int>, std::vector<std::string>, std::vector<uint64_t>, std::vector<uint64_t>> read_SA_array(std::string&);
std::tuple<std::vector<std::string>, std::vector<std::string>> read_query_sequence(std::string&);
void save_result(std::string&, std::vector<std::string>&, std::vector<std::vector<uint64_t>>&, uint64_t);
std::vector<uint64_t> naive_search1_short(std::string&, std::vector<int>&, std::vector<std::string>&, std::vector<uint64_t>&, std::vector<uint64_t>&, std::string&);
std::vector<uint64_t> naive_search1_long(std::string&, std::vector<int>&, std::vector<std::string>&, std::vector<uint64_t>&, std::vector<uint64_t>&, std::string&);
std::vector<uint64_t> naive_search2_short(std::string&, std::vector<int>&, std::vector<std::string>&, std::vector<uint64_t>&, std::vector<uint64_t>&, std::string&);
std::vector<uint64_t> naive_search2_long(std::string&, std::vector<int>&, std::vector<std::string>&, std::vector<uint64_t>&, std::vector<uint64_t>&, std::string&);
std::uint64_t binary_search_loop(uint64_t, uint64_t, std::string&, std::vector<int>&, std::string&);
std::uint64_t binary_search_left(uint64_t, uint64_t, std::string&, std::vector<int>&, std::string&);
std::uint64_t binary_search_right(uint64_t, uint64_t, std::string&, std::vector<int>&, std::string&);
std::vector<uint64_t> simpaccel_search1_short(std::string&, std::vector<int>&, std::vector<std::string>&, std::vector<uint64_t>&, std::vector<uint64_t>&, std::string&);
std::vector<uint64_t> simpaccel_search1_long(std::string&, std::vector<int>&, std::vector<std::string>&, std::vector<uint64_t>&, std::vector<uint64_t>&, std::string&);
std::vector<uint64_t> simpaccel_search2_short(std::string&, std::vector<int>&, std::vector<std::string>&, std::vector<uint64_t>&, std::vector<uint64_t>&, std::string&);
std::vector<uint64_t> simpaccel_search2_long(std::string&, std::vector<int>&, std::vector<std::string>&, std::vector<uint64_t>&, std::vector<uint64_t>&, std::string&);
std::uint64_t binary_search_LCP_lc(uint64_t, uint64_t, std::string&, std::vector<int>&, std::string&);
std::uint64_t binary_search_LCP(uint64_t, uint64_t, std::string&, std::vector<int>&, std::string&);
uint64_t LCP(std::string&, std::string&, uint64_t);


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************MAIN FUNCTION*********************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){


    //Default Parameters
    std::string address_SA = "./out_SA";
    std::string address_query = "./query.fa";
    std::string output = "out_query.txt";
    std::string query_mode = "naive";
    std::string method = "1";
    std::string range = "short";


    //overwrite default by user input
    for(size_t arg_ind = 1; arg_ind < argc; arg_ind++){
        if(argv[arg_ind] == "--index"){
            address_SA = argv[arg_ind + 1];
        }else if(argv[arg_ind] == "--queries"){
            address_query = argv[arg_ind + 1];
        }else if(argv[arg_ind] == "--output"){
            output = argv[arg_ind + 1];
        }else if(argv[arg_ind] == "--query_mode"){
            query_mode = argv[arg_ind + 1];
        }else if(argv[arg_ind] == "--method"){
            method = argv[arg_ind + 1];
        }else if(argv[arg_ind] == "--range"){
            range = argv[arg_ind + 1];
        }
    }

    std::string sequence; 
    std::vector<int> SA;
    std::vector<std::string> pref_string, query_names, query_sequences;
    std::vector<uint64_t> pref_first, pref_last;
    std::vector<std::vector<uint64_t>> query_result;

    
    //reading the index from the buildsa output
    tie(sequence, SA, pref_string, pref_first, pref_last) = read_SA_array(address_SA);
    
    //reading the queries
    tie(query_names, query_sequences) = read_query_sequence(address_query);

    //searching based on the method that user chose or based on the default

    
    //outfile.open ("naive_speed_ref.txt", );
    //std::fstream fs("naive_search1_short.txt", std::ios::in | std::ios::out | std::ios::ate);

     if(query_mode == "naive" && method == "1" && range == "short"){
 
        for (size_t i = 0; i < query_sequences.size(); i++)
        {   
            
            query_result.push_back(naive_search1_short(sequence, SA, pref_string, pref_first, pref_last,query_sequences[i]));

        } 
    }else if(query_mode == "naive" && method == "1" && range == "long"){

        for (size_t i = 0; i < query_sequences.size(); i++)
        {
            query_result.push_back(naive_search1_long(sequence, SA, pref_string, pref_first, pref_last,query_sequences[i]));
        
        } 
    }else if(query_mode == "naive" && method == "2" && range == "short"){

        for (size_t i = 0; i < query_sequences.size(); i++)
        {
            query_result.push_back(naive_search2_short(sequence, SA, pref_string, pref_first, pref_last,query_sequences[i]));
        
        } 
    }else if(query_mode == "naive" && method == "2" && range == "long"){

        for (size_t i = 0; i < query_sequences.size(); i++)
        {
            query_result.push_back(naive_search2_long(sequence, SA, pref_string, pref_first, pref_last,query_sequences[i]));
        
        } 
    }else if(query_mode == "simpaccel" && method == "1" && range == "short"){

        for (size_t i = 0; i < query_sequences.size(); i++)
        {
            query_result.push_back(simpaccel_search1_short(sequence, SA, pref_string, pref_first, pref_last,query_sequences[i]));
        
        } 
    }else if(query_mode == "simpaccel" && method == "1" && range == "long"){

        for (size_t i = 0; i < query_sequences.size(); i++)
        {
            query_result.push_back(simpaccel_search1_long(sequence, SA, pref_string, pref_first, pref_last,query_sequences[i]));
        
        } 
    }else if(query_mode == "simpaccel" && method == "2" && range == "short"){

        for (size_t i = 0; i < query_sequences.size(); i++)
        {
            query_result.push_back(simpaccel_search2_short(sequence, SA, pref_string, pref_first, pref_last,query_sequences[i]));
        
        } 
    }else if(query_mode == "simpaccel" && method == "2" && range == "long"){

        for (size_t i = 0; i < query_sequences.size(); i++)
        {
            query_result.push_back(simpaccel_search2_long(sequence, SA, pref_string, pref_first, pref_last,query_sequences[i]));
        
        } 
    
 
    }
    //saving the result in a text file
    save_result(output, query_names, query_result, sequence.size());
    
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/***********************************READING THE INDEX FROM THE OUTPUT OF BUILDSA*************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//reading the file from SA and prefix table
std::tuple<std::string, std::vector<int>, std::vector<std::string>, std::vector<uint64_t>, std::vector<uint64_t>> read_SA_array(std::string& address_SA){

    std::ifstream os(address_SA, std::ios::binary);
    cereal::BinaryInputArchive archive( os );

    std::string sequence;
    std::vector<int> SA;
    std::vector<std::string> pref_string;
    std::vector<uint64_t> pref_first, pref_last;
    archive(sequence, SA,pref_string, pref_first, pref_last);

    return make_tuple(sequence, SA,pref_string, pref_first, pref_last);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************READING THE QUERIES*********************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//reading the query sequence
std::tuple<std::vector<std::string>, std::vector<std::string>> read_query_sequence(std::string& address_query){

    std::string line, id, sequence , S_rand = "ACGT";
    std::vector<std::string> query_names, query_sequences;
    
    //reading the reference genome file
    std::ifstream infile(address_query);

    while (getline(infile,line))
    {
        if (line.empty())
        {
            continue;
        }

        if (line[0] == '>')
        {
            
            id = line.substr(1);
            query_names.push_back(id);
            if (!sequence.empty())
            {
                query_sequences.push_back(sequence);
            }
            sequence = "";

        }
        else
        {
            sequence += line;
        }
    }
    query_sequences.push_back(sequence);

   //checking foe lower case and convert them to upper case and laso check for n/N in the reference
   for (size_t i = 0; i < query_sequences.size(); i++)
   {
       for (size_t j = 0; j < query_sequences[i].size(); j++)
       {
           if (query_sequences[i][j] == 'a' || query_sequences[i][j] == 'c' || query_sequences[i][j] == 'g' || query_sequences[i][j] == 't')
            {
                query_sequences[i][j] = toupper(query_sequences[i][j]);
            }
            else if (query_sequences[i][j] == 'n' || query_sequences[i][j] == 'N')
            {
                srand(time(0));
                query_sequences[i][j] = S_rand[rand()%4];
            }
       }
   }

   return make_tuple(query_names, query_sequences);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************SAVE THE OUTPUT TO A TEXT FILE**********************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//function to save the results of queries in a readable text file
void save_result(std::string& output, std::vector<std::string>& query_names, std::vector<std::vector<uint64_t>>& query_result, uint64_t sequence_size){

    
    std::ofstream outfile(output);
    
    int max_val = 0;
    int size_mid;
    for(size_t idx = 0; idx < query_names.size(); idx++){
        size_mid = query_names[idx].size();
        max_val = std::max(max_val, size_mid);
    }

    //writing to the output file
    for (size_t i = 0; i < query_names.size(); i++)
    {
        outfile << query_names[i] << std::setw(max_val + 5 - query_names[i].size());

        if (query_result[i][0] == sequence_size )
        {
           outfile << 0 << std::endl; 
        }else{
            outfile << query_result[i].size() << std::setw(10);
            for (size_t j = 0; j < query_result[i].size(); j++)
            {
                if(j != (query_result[i].size() - 1)){
                    outfile << query_result[i][j] << std::setw(10);
                }else{
                    outfile << query_result[i][j] << std::endl;
                }
                
            }
            
        }
        
    }
  
    outfile.close();

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************NAIVE SEARCH - METHOD 1 - SHORT RANGE***************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<uint64_t> naive_search1_short(std::string& sequence, std::vector<int>& SA_vec, std::vector<std::string>& pref_string, std::vector<uint64_t>& pref_first, std::vector<uint64_t>& pref_last, std::string& query){


    uint64_t low_index;
    uint64_t high_index;
    std::vector<uint64_t> Positions;
    if(pref_string.empty()){
        low_index = 0;
        high_index = sequence.size() - 1;
    }else{
        uint64_t i = 0;
        int k = pref_string[0].size();
        while(query.substr(0,k) != pref_string[i] && i < (pref_string.size() - 1)){
            i++;
        }
        if(i < pref_string.size()){
            low_index = pref_first[i];
            high_index = pref_last[i];
        }else{
            low_index = 0;
            high_index = sequence.size() - 1;
        }
    }

    uint64_t first_match;
    if(low_index < high_index){
        first_match = binary_search_loop(low_index, high_index, sequence, SA_vec, query);
    }else{
        first_match = low_index;
    }
    
    

    if (sequence.substr(SA_vec[first_match], query.size()) == query)
    {
    
        low_index = first_match;
        while (low_index > 0 && sequence.substr(SA_vec[low_index - 1], query.size()) == query)
        {
            low_index--;
        }
              

        high_index = first_match;
        while (high_index < (sequence.size() - 1) && sequence.substr(SA_vec[high_index + 1], query.size()) == query)
        {
            high_index++;
            
        }

        for (auto i = low_index; i < high_index + 1; ++i)
        {
            Positions.push_back(SA_vec[i]);
        }
    }else{
        Positions.push_back(sequence.size());
    }
    return Positions;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************NAIVE SEARCH - METHOD 1 - LONG RANGE***************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<uint64_t> naive_search1_long(std::string& sequence, std::vector<int>& SA_vec, std::vector<std::string>& pref_string, std::vector<uint64_t>& pref_first, std::vector<uint64_t>& pref_last, std::string& query){

    uint64_t low_index;
    uint64_t high_index;
    std::vector<uint64_t> Positions;
    if(pref_string.empty()){
        low_index = 0;
        high_index = sequence.size() - 1;
    }else{
        uint64_t i = 0;
        int k = pref_string[0].size();
        while(query.substr(0,k) != pref_string[i] && i < (pref_string.size() - 1)){
            i++;
        }
        if(i < pref_string.size()){
            low_index = pref_first[i];
            high_index = pref_last[i];
        }else{
            low_index = 0;
            high_index = sequence.size() - 1;
        }
    }

    uint64_t first_match;
    if(low_index < high_index){
        first_match = binary_search_loop(low_index, high_index, sequence, SA_vec, query);
    }else{
        first_match = low_index;
    }

    if (sequence.substr(SA_vec[first_match], query.size()) == query)
    {
    
        low_index = first_match;
        while (low_index > 0 && sequence.substr(SA_vec[low_index - 1], query.size()) == query)
        {
            low_index--;
        }
        
        uint64_t second_match;
        if(low_index < high_index){
            second_match = binary_search_loop(low_index, high_index, sequence, SA_vec, query);
        }else{
            second_match = low_index;
        }
        

        high_index = second_match;
        while (high_index < (sequence.size() - 1) && sequence.substr(SA_vec[high_index + 1], query.size()) == query)
        {
            high_index++;
            
        }

        for (auto i = low_index; i < high_index + 1; ++i)
        {
            Positions.push_back(SA_vec[i]);
        }
    }else{
        Positions.push_back(sequence.size());
    }
    return Positions;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************NAIVE SEARCH - METHOD 2 - SHORT RANGE***************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<uint64_t> naive_search2_short(std::string& sequence, std::vector<int>& SA_vec, std::vector<std::string>& pref_string, std::vector<uint64_t>& pref_first, std::vector<uint64_t>& pref_last, std::string& query){

    uint64_t low_index;
    uint64_t high_index;
    std::vector<uint64_t> Positions;
    if(pref_string.empty()){
        low_index = 0;
        high_index = sequence.size() - 1;
    }else{
        uint64_t i = 0;
        int k = pref_string[0].size();
        while(query.substr(0,k) != pref_string[i] && i < (pref_string.size() - 1)){
            i++;
        }
        if(i < pref_string.size()){
            low_index = pref_first[i];
            high_index = pref_last[i];
        }else{
            low_index = 0;
            high_index = sequence.size() - 1;
        }
    }

    uint64_t first_match;
    if(low_index < high_index){
        first_match = binary_search_left(low_index, high_index, sequence, SA_vec, query);
    }else{
        first_match = low_index;
    }

    

    if (sequence.substr(SA_vec[first_match], query.size()) == query)
    {
    
        low_index = first_match;
        while (low_index > 0 && sequence.substr(SA_vec[low_index - 1], query.size()) == query)
        {
            low_index--;
        }
              

        high_index = first_match;
        while (high_index < (sequence.size() - 1) && sequence.substr(SA_vec[high_index + 1], query.size()) == query)
        {
            high_index++;
            
        }

        for (auto i = low_index; i < high_index + 1; ++i)
        {
            Positions.push_back(SA_vec[i]);
        }
    }else{
        Positions.push_back(sequence.size());
    }
    return Positions;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************NAIVE SEARCH - METHOD 2 - LONG RANGE***************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<uint64_t> naive_search2_long(std::string& sequence, std::vector<int>& SA_vec, std::vector<std::string>& pref_string, std::vector<uint64_t>& pref_first, std::vector<uint64_t>& pref_last, std::string& query){

    uint64_t low_index;
    uint64_t high_index;
    std::vector<uint64_t> Positions;
    if(pref_string.empty()){
        low_index = 0;
        high_index = sequence.size() - 1;
    }else{
        uint64_t i = 0;
        int k = pref_string[0].size();
        while(query.substr(0,k) != pref_string[i] && i < (pref_string.size() - 1)){
            i++;
        }
        if(i < pref_string.size()){
            low_index = pref_first[i];
            high_index = pref_last[i];
        }else{
            low_index = 0;
            high_index = sequence.size() - 1;
        }
    }

    uint64_t first_match;
    if(low_index < high_index){
        first_match = binary_search_left(low_index, high_index, sequence, SA_vec, query);
    }else{
        first_match = low_index;
    }
    
    

    if (sequence.substr(SA_vec[first_match], query.size()) == query)
    {
    
        low_index = first_match;
        while (low_index > 0 && sequence.substr(SA_vec[low_index - 1], query.size()) == query)
        {
            low_index--;
        }
        
        uint64_t second_match;
        if(low_index < high_index){
            second_match = binary_search_right(low_index, high_index, sequence, SA_vec, query);
        }else{
            second_match = low_index;
        }
        

        high_index = second_match;
        while (high_index < (sequence.size() - 1) && sequence.substr(SA_vec[high_index + 1], query.size()) == query)
        {
            high_index++;
            
        }

        for (auto i = low_index; i < high_index + 1; ++i)
        {
            Positions.push_back(SA_vec[i]);
        }
    }else{
        Positions.push_back(sequence.size());
    }
    return Positions;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************BINARY SEARCH FUNCTIONS FOR NAIVE SEACH METHODS*********************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////


std::uint64_t binary_search_loop(uint64_t low_index, uint64_t high_index, std::string& sequence, std::vector<int>& SA_vec, std::string& query){
    
    uint64_t mid_index, return_val;

    while (low_index < high_index)
    {
        mid_index = (low_index + high_index) / 2;
        
        int i;

        bool less_query = true;

        //std::cout<<"\n"<<std::min(query.size(), (sequence.size() - SA_vec[mid_index]));
        for(i = 0; i < std::min(query.size(), (sequence.size() - SA_vec[mid_index])); i++){

            if(query[i] == sequence[SA_vec[mid_index] + i]){

                continue;

            }else if(query[i] < sequence[SA_vec[mid_index] + i]){

                break;

            }else if(query[i] > sequence[SA_vec[mid_index] + i]){

                less_query = false;
                break;
            }
        }

        if(i == query.size()){

            return_val = mid_index;
            break;
        }
        if (less_query)
        {
            if(mid_index == low_index + 1 || mid_index == low_index){
                return_val = low_index;
                break;
            }
            high_index = mid_index;
        }else{
            if(mid_index == high_index - 1 || mid_index == high_index){
                return_val = high_index;
                break;
            }
            low_index = mid_index;
        }
        
    }
    return return_val;

}

std::uint64_t binary_search_left(uint64_t low_index, uint64_t high_index, std::string& sequence, std::vector<int>& SA_vec, std::string& query){
    while (low_index < high_index)
    {
        uint64_t mid_index = (low_index + high_index) / 2;
        if (sequence.substr(SA_vec[mid_index]) < query)
        {
            low_index = mid_index + 1;
        }else{
            high_index = mid_index;
        }
        
    }
    return low_index;

}

std::uint64_t binary_search_right(uint64_t low_index, uint64_t high_index, std::string& sequence, std::vector<int>& SA_vec, std::string& query){
    while (low_index < high_index)
    {
        uint64_t mid_index = (low_index + high_index) / 2;
        if (sequence.substr(SA_vec[mid_index]) > query)
        {
            high_index = mid_index;
            
        }else{
            low_index = mid_index + 1;
        }
        
    }
    return low_index;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************SIMPACCEL SEARCH - METHOD 1 - SHORT RANGE***************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<uint64_t> simpaccel_search1_short(std::string& sequence, std::vector<int>& SA_vec, std::vector<std::string>& pref_string, std::vector<uint64_t>& pref_first, std::vector<uint64_t>& pref_last, std::string& query){

    uint64_t low_index;
    uint64_t high_index;
    std::vector<uint64_t> Positions;

    if(pref_string.empty()){
        low_index = 0;
        high_index = sequence.size() - 1;
    }else{
        uint64_t i = 0;
        int k = pref_string[0].size();
        while(query.substr(0,k) != pref_string[i] && i < (pref_string.size() - 1)){
            i++;
        }
        if(i < pref_string.size()){
            low_index = pref_first[i];
            high_index = pref_last[i];
        }else{
            low_index = 0;
            high_index = sequence.size() - 1;
        }
    }

     uint64_t first_match;
    if(low_index < high_index){
        first_match = binary_search_LCP_lc(low_index, high_index, sequence, SA_vec, query);
    }else{
        first_match = low_index;
    }
    
    
    if (sequence.substr(SA_vec[first_match], query.size()) == query)
    {
    
        low_index = first_match;
        while (low_index > 0 && sequence.substr(SA_vec[low_index - 1], query.size()) == query)
        {
            low_index--;
        }


        high_index = first_match;
        //std::cout<<"\nhigh index:"<<second_match<<"\n";
        while (high_index < (sequence.size() - 1) && sequence.substr(SA_vec[high_index + 1], query.size()) == query)
        {
            high_index++;
        }

        for (auto i = low_index; i < (high_index + 1); ++i)
        {
            Positions.push_back(SA_vec[i]);
            //std::cout <<i<<"\n";
        }
    }else{
        Positions.push_back(sequence.size());
    }

    return Positions;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************SIMPACCEL SEARCH - METHOD 1 - LONG RANGE***************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<uint64_t> simpaccel_search1_long(std::string& sequence, std::vector<int>& SA_vec, std::vector<std::string>& pref_string, std::vector<uint64_t>& pref_first, std::vector<uint64_t>& pref_last, std::string& query){

    uint64_t low_index;
    uint64_t high_index;
    std::vector<uint64_t> Positions;

    if(pref_string.empty()){
        low_index = 0;
        high_index = sequence.size() - 1;
    }else{
        uint64_t i = 0;
        int k = pref_string[0].size();
        while(query.substr(0,k) != pref_string[i] && i < (pref_string.size() - 1)){
            i++;
        }
        if(i < pref_string.size()){
            low_index = pref_first[i];
            high_index = pref_last[i];
        }else{
            low_index = 0;
            high_index = sequence.size() - 1;
        }
    }

    uint64_t first_match;
    if(low_index < high_index){
        first_match = binary_search_LCP_lc(low_index, high_index, sequence, SA_vec, query);
    }else{
        first_match = low_index;
    }
    

    
    if (sequence.substr(SA_vec[first_match], query.size()) == query)
    {
    
        low_index = first_match;
        while (low_index > 0 && sequence.substr(SA_vec[low_index - 1], query.size()) == query)
        {
            low_index--;
        }

        //std::cout<<"\nlow index:"<<low_index<<"\n";
        uint64_t second_match;
        if(low_index < high_index){
            second_match = binary_search_LCP(low_index, high_index, sequence, SA_vec, query);
        }else{
            second_match = low_index;
        }
        

        high_index = second_match;
        //std::cout<<"\nhigh index:"<<second_match<<"\n";
        while (high_index < (sequence.size() - 1) && sequence.substr(SA_vec[high_index + 1], query.size()) == query)
        {
            high_index++;
        }

        for (auto i = low_index; i < (high_index + 1); ++i)
        {
            Positions.push_back(SA_vec[i]);
            //std::cout <<i<<"\n";
        }
    }else{
        Positions.push_back(sequence.size());
    }

    return Positions;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************SIMPACCEL SEARCH - METHOD 2 - SHORT RANGE***************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<uint64_t> simpaccel_search2_short(std::string& sequence, std::vector<int>& SA_vec, std::vector<std::string>& pref_string, std::vector<uint64_t>& pref_first, std::vector<uint64_t>& pref_last, std::string& query){

    uint64_t low_index;
    uint64_t high_index;
    std::vector<uint64_t> Positions;

    if(pref_string.empty()){
        low_index = 0;
        high_index = sequence.size() - 1;
    }else{
        uint64_t i = 0;
        int k = pref_string[0].size();
        while(query.substr(0,k) != pref_string[i] && i < (pref_string.size() - 1)){
            i++;
        }
        if(i < pref_string.size()){
            low_index = pref_first[i];
            high_index = pref_last[i];
        }else{
            low_index = 0;
            high_index = sequence.size() - 1;
        }
    }

        uint64_t first_match;
        if(low_index < high_index){
            first_match = binary_search_LCP(low_index, high_index, sequence, SA_vec, query);
        }else{
            first_match = low_index;
        }
    
    if (sequence.substr(SA_vec[first_match], query.size()) == query)
    {
    
        low_index = first_match;
        while (low_index > 0 && sequence.substr(SA_vec[low_index - 1], query.size()) == query)
        {
            low_index--;
        }


        high_index = first_match;
        //std::cout<<"\nhigh index:"<<second_match<<"\n";
        while (high_index < (sequence.size() - 1) && sequence.substr(SA_vec[high_index + 1], query.size()) == query)
        {
            high_index++;
        }

        for (auto i = low_index; i < (high_index + 1); ++i)
        {
            Positions.push_back(SA_vec[i]);
            //std::cout <<i<<"\n";
        }
    }else{
        Positions.push_back(sequence.size());
    }

    return Positions;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************SIMPACCEL SEARCH - METHOD 2 - LONG RANGE***************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<uint64_t> simpaccel_search2_long(std::string& sequence, std::vector<int>& SA_vec, std::vector<std::string>& pref_string, std::vector<uint64_t>& pref_first, std::vector<uint64_t>& pref_last, std::string& query){

    uint64_t low_index;
    uint64_t high_index;
    std::vector<uint64_t> Positions;

    if(pref_string.empty()){
        low_index = 0;
        high_index = sequence.size() - 1;
    }else{
        uint64_t i = 0;
        int k = pref_string[0].size();
        while(query.substr(0,k) != pref_string[i] && i < (pref_string.size() - 1)){
            i++;
        }
        if(i < pref_string.size()){
            low_index = pref_first[i];
            high_index = pref_last[i];
        }else{
            low_index = 0;
            high_index = sequence.size() - 1;
        }
    }

    uint64_t first_match;
        if(low_index < high_index){
            first_match = binary_search_LCP(low_index, high_index, sequence, SA_vec, query);
        }else{
            first_match = low_index;
        }

    
    if (sequence.substr(SA_vec[first_match], query.size()) == query)
    {
    
        low_index = first_match;
        while (low_index > 0 && sequence.substr(SA_vec[low_index - 1], query.size()) == query)
        {
            low_index--;
        }

        //std::cout<<"\nlow index:"<<low_index<<"\n";
        uint64_t second_match;
        if(low_index < high_index){
            second_match = binary_search_LCP(low_index, high_index, sequence, SA_vec, query);
        }else{
            second_match = low_index;
        }
        

        high_index = second_match;
        //std::cout<<"\nhigh index:"<<second_match<<"\n";
        while (high_index < (sequence.size() - 1) && sequence.substr(SA_vec[high_index + 1], query.size()) == query)
        {
            high_index++;
        }

        for (auto i = low_index; i < (high_index + 1); ++i)
        {
            Positions.push_back(SA_vec[i]);
            //std::cout <<i<<"\n";
        }
    }else{
        Positions.push_back(sequence.size());
    }

    return Positions;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************BINARY SEARCH FUNCTIONS FOR SIMPACCEL METHODS; LCP FUNCTION****************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////


uint64_t binary_search_LCP_lc(uint64_t low_index, uint64_t high_index, std::string& sequence, std::vector<int>& SA_vec, std::string& query){
    
    uint64_t mid_index, return_val;
    int i;
    uint64_t LCP_lq, LCP_rq, min_LCP;
    std::string low_index_string, high_index_string;

    low_index_string = sequence.substr(SA_vec[low_index]);    
    high_index_string = sequence.substr(SA_vec[high_index]);

    LCP_lq = LCP(low_index_string, query, query.size());
    LCP_rq = LCP(high_index_string, query, query.size());

    while (low_index < high_index)
    {
        mid_index = (low_index + high_index) / 2;
        
        min_LCP = std::min(LCP_lq, LCP_rq);
        
        bool less_query = true;

        //std::cout<<"\n"<<std::min(query.size(), (sequence.size() - SA_vec[mid_index]));
        for(i = min_LCP; i < std::min(query.size(), (sequence.size() - SA_vec[mid_index])); i++){

            if(query[i] == sequence[SA_vec[mid_index] + i]){

                continue;

            }else if(query[i] < sequence[SA_vec[mid_index] + i]){

                break;

            }else if(query[i] > sequence[SA_vec[mid_index] + i]){

                less_query = false;
                break;
            }
        }

        if(i == query.size()){

            return_val = mid_index;
            break;
        }
        if (less_query)
        {
            if(mid_index == low_index + 1 || mid_index == low_index){
                return_val = low_index;
                break;
            }
            high_index = mid_index;
            LCP_rq = i;
        }else{
            if(mid_index == high_index - 1 || mid_index == high_index){
                return_val = high_index;
                break;
            }
            low_index = mid_index;
            LCP_lq = i;
        }
        
    }
    return return_val;
}


uint64_t binary_search_LCP(uint64_t low_index, uint64_t high_index, std::string& sequence, std::vector<int>& SA_vec, std::string& query){
    
    uint64_t mid_index;
    std::string mid_index_string;
    uint64_t LCP_lc, LCP_rc;
    uint64_t tie_position;
    std::string low_index_string = sequence.substr(SA_vec[low_index]);    
    std::string high_index_string = sequence.substr(SA_vec[high_index]);
    uint64_t LCP_lq = LCP(low_index_string, query, query.size());
    uint64_t LCP_rq = LCP(high_index_string, query, query.size());
    int gg = 1;
    
    while (low_index < high_index)
    {
        mid_index = (low_index + high_index) / 2;
        mid_index_string = sequence.substr(SA_vec[mid_index]); 
        //std::cout<<"\n*********round\t"<<gg;
        //std::cout<<"\nlow_index\t"<<low_index;
        //std::cout<<"\nhigh_index\t"<<high_index;
        //std::cout<<"\nlcp_lq\t"<<LCP_lq;
        //std::cout<<"\nlcp_rq\t"<<LCP_rq;
        //std::cout<<"\nmid_index\t"<<mid_index;
        //std::cout<<"\nmid\t"<<mid_index_string.substr(0,12);
        

        if (LCP_lq > LCP_rq)
        {
            low_index_string = sequence.substr(SA_vec[low_index]);
            LCP_lc = LCP(mid_index_string, low_index_string, query.size());
            //std::cout<<"\nLCP_lc\t"<<LCP_lc;
            if (LCP_lc == LCP_lq)
            {
                uint64_t i;
                for (i = LCP_lc; i < std::min(query.size(),mid_index_string.size()) + 1; i++)
                {
                    if (query[i] < mid_index_string[i])
                    {
                        if (mid_index == low_index +1 || mid_index == low_index)
                        {
                            tie_position = low_index;
                            high_index = low_index - 1;
                        }
                        high_index = mid_index;
                        LCP_rq = i;
                        break;
                        
                    }

                    if (query[i] > mid_index_string[i])
                    {
                        if (mid_index == high_index - 1 || mid_index == high_index)
                        {
                            tie_position = high_index;
                            high_index = low_index - 1;
                        }
                        low_index = mid_index;
                        LCP_lq = i;
                        break;
                    }

                }
                
                if (i == query.size())
                {
                    tie_position = mid_index;
                    break;
                }
                
              
            } else if (LCP_lc > LCP_lq)
            {
                if (mid_index == high_index - 1 || mid_index == high_index)
                {
                    tie_position = high_index;
                    break;
                }
                
                low_index = mid_index;
                LCP_lq = LCP_lq;

            }else if (LCP_lc < LCP_lq)
            {
                if (mid_index == low_index +1 || mid_index == low_index)
                {
                    tie_position = low_index;
                    break;
                }
                
                high_index = mid_index;
                LCP_rq = LCP_lc;
            }
        }else{
            high_index_string = sequence.substr(SA_vec[high_index]);
            LCP_rc = LCP(mid_index_string, high_index_string, query.size());
            

            if (LCP_rc == LCP_rq)
            {
                uint64_t i;
                for (i = LCP_rc; i < std::min(query.size(),mid_index_string.size()); i++)
                {
                    if (query[i] < mid_index_string[i])
                    {
                        if (mid_index == low_index + 1 || mid_index == low_index)
                        {
                            tie_position = low_index;
                            high_index = low_index - 1;
                        }

                        high_index = mid_index;
                        LCP_rq = i;
                        break;
                        
                    }

                    if (query[i] > mid_index_string[i])
                    {
                        if (mid_index == high_index - 1 || mid_index == high_index)
                        {
                            tie_position = high_index;
                            high_index = low_index - 1;
                        }
                        low_index = mid_index;
                        LCP_lq = i;
                        break;
                    }

                }
                
                if (i == query.size())
                {
                    tie_position = mid_index;
                    break;
                }
              
            } else if (LCP_rc > LCP_rq)
            {
                if (mid_index == low_index + 1 || mid_index == low_index)
                {
                    tie_position = low_index;
                    break;
                }
                
                high_index = mid_index;
                LCP_rq = LCP_rq;

            }else if (LCP_rc < LCP_rq)
            {
                if (mid_index == high_index - 1 || mid_index == high_index)
                {
                    tie_position = high_index;
                    break;
                }
                
                low_index = mid_index;
                LCP_lq = LCP_rc;
            }

        }

        
    }
    
    return tie_position;
}


uint64_t LCP(std::string& first_string, std::string& second_string, uint64_t query_size){

    uint64_t common_prefix_size = 0;

    if(!first_string.empty() && !second_string.empty()){
        uint64_t prefix_max_size = std::min(first_string.size(), second_string.size());
        prefix_max_size = std::min(prefix_max_size, query_size);
        for (size_t i = 0; i < prefix_max_size; i++)
        {
            if (first_string[i] == second_string[i])
            {
                common_prefix_size++;
            }else{
                break;
            }
            
        }
    }

    return common_prefix_size;
}