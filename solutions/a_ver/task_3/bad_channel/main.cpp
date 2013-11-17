#include <fstream>
#include <algorithm>
#include <sstream>
#include <thread>
#include <vector>
#include <system_error>
#include <deal.h>

using namespace std;

const static unsigned int COUNT_FILES = 999;
const static string INPUT_PREFIX(SOURCE_DIR "/input");
const static string OUTPUT_PREFIX(SOURCE_DIR "/output");
const static string FILE_EXT(".txt");

int processing_file(const unsigned int number){
    stringstream buffer;
    buffer.width(3);
    buffer.fill('0');
    buffer << number;
   
    ifstream i_fs(INPUT_PREFIX + buffer.str() + FILE_EXT, ifstream::binary);
    if(!i_fs){
        cout << "Could not open input file for number = " << number << endl;
        return 1;
    }
    
    ofstream o_fs(OUTPUT_PREFIX + buffer.str() + FILE_EXT, ofstream::binary);
    if(!o_fs){
        cout << "Could not open output file for number = " << number << endl;
        return 2;
    }
    MessageHeader m_header;

    while(i_fs.good()){
        i_fs >> m_header;

        if(i_fs.fail()){
            break;
        }
        if(is_outdated_deal(m_header)){
            i_fs.seekg(m_header.len, ios_base::cur);
            continue;
        }
        o_fs.write((char *)&m_header, M_HEADER_SIZEOF);
        copy_n(istreambuf_iterator<char>(i_fs), m_header.len,
               ostreambuf_iterator<char>(o_fs));
        i_fs.seekg(1, ios_base::cur);
    }
    i_fs.close();
    o_fs.close();
    return 0;
}

void threads_join(vector<thread> &threads){
    for(auto &t : threads){
        t.join();
    }
}

int main(){
    vector<thread> threads;
    for(unsigned int i = 1; i < COUNT_FILES+1; i++){
        try{
            threads.push_back(thread(processing_file, i));
        }catch(const system_error &e){
            cout << "system error: code = " << e.code();
            cout << " what = " << e.what() << endl;
            if(e.code().value() == EAGAIN){
                threads_join(threads);
                threads.clear();
                threads.push_back(thread(processing_file, i));
            }
        }
    }
    threads_join(threads);
    return 0;
}
