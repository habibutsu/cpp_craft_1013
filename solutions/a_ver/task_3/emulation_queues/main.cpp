#include <stdint.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <vector>
#include <sstream>
#include <thread>
#include <system_error>

#include <deal.h>

using namespace std;

const static unsigned int COUNT_FILES = 5;
const static string INPUT_PREFIX(SOURCE_DIR "/input");
const static string FILE_EXT(".txt");
const static unsigned short int MAX_BUFFER_SIZE = 2048;
const static unsigned int MAX_THRESHOLD_TYPE = 100000;

struct Aggregated{
    uint32_t count;
    uint32_t count_seconds;
    uint32_t current_time;
    uint32_t current_buffer_size;

    Aggregated() : count(0), count_seconds(0), current_time(0), current_buffer_size(0) {}
};

int processing_file(
    const unsigned int number,
    map<uint32_t, Aggregated > &aggregated_data
){
    stringstream buffer;
    buffer.width(3);
    buffer.fill('0');
    buffer << number;
   
    ifstream i_fs(INPUT_PREFIX + buffer.str() + FILE_EXT, ifstream::binary);
    if(!i_fs){
        cout << "Could not open input file for number = " << number << endl;
        return 1;
    }
    
    MessageHeader m_header;

    while(i_fs.good()){
        i_fs >> m_header;
        if(i_fs.fail()){
            break;
        }
        i_fs.seekg(m_header.len, ios_base::cur);

        if(m_header.time > MAX_THRESHOLD_TYPE){
            continue;
        }

        if(aggregated_data.find(m_header.type) == aggregated_data.end() ){
            aggregated_data[m_header.type] = Aggregated();
        }
        
        Aggregated &item = aggregated_data[m_header.type];

        if(item.current_time != m_header.time){
            item.current_time = m_header.time;
            item.count_seconds++;
            item.current_buffer_size = 0;
        }
        if( (item.current_buffer_size + m_header.len + M_HEADER_SIZEOF) > MAX_BUFFER_SIZE){
            continue;
        }
        item.current_buffer_size += m_header.len;
        item.count += 1;
    }

    i_fs.close();
    return 0;
}

void threads_join(vector<thread> &threads){
    for(auto &t : threads){
        t.join();
    }
}
typedef map<uint32_t, Aggregated> aggregated_map;
typedef vector<aggregated_map> res_vector;

int main(){
    vector<thread> threads;
    res_vector threads_results;
    threads_results.reserve(COUNT_FILES);
    // step 1 - map    
    for(unsigned int i = 1; i < COUNT_FILES+1; i++){
        threads_results.push_back(aggregated_map());
        try{
            threads.push_back(
                thread(processing_file, i, ref(threads_results[i-1])));
        }catch(const system_error &e){
            cout << "system error: code = " << e.code();
            cout << " what = " << e.what() << endl;
            if(e.code().value() == EAGAIN){
                threads_join(threads);
                threads.clear();
                threads.push_back(
                    thread(processing_file, i, ref(threads_results[i-1])));
            }
        }
    }
    threads_join(threads);

    // step 2 - reduce
    map<uint32_t, Aggregated> result;

    for(res_vector::iterator it_v = threads_results.begin(); it_v != threads_results.end(); it_v++){
        for(aggregated_map::iterator it_m = it_v->begin(); it_m != it_v->end(); it_m++){
           Aggregated &item = result[it_m->first];
           item.count += it_m->second.count;
           item.count_seconds += it_m->second.count_seconds;
       }
    }

    ofstream o_fs(SOURCE_DIR "/output.txt", ofstream::binary);
    if(!o_fs){
        cout << "Could not open output file" << endl;
        return 1;
    }

    double average_count;
    for (auto it=result.cbegin(); it!=result.cend(); ++it){
        o_fs.write(reinterpret_cast<const char*>(&it->first), sizeof(it->first));
        average_count = (it->second.count / (double)it->second.count_seconds);
        o_fs.write(reinterpret_cast<const char*>(&average_count), sizeof(average_count));
    }

    o_fs.close();

    return 0;
}
