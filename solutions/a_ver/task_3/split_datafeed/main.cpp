//#include <cstdint>
#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <array>
#include <time.h> 
#include <thread>
#include <system_error>


using namespace std;

const static string OUTPUT_PREFIX(SOURCE_DIR "/output");
const static string FILE_EXT(".txt");

struct Message{
    char stock_name[8];
    char date_time[8];
    double price;
    double vwap;
    uint32_t volume;
    double f1;
    double t1;
    double f2;
    double f3;
    double f4;
};

struct OutMessage{
    uint32_t count_days;
    double vwap;
    uint32_t volume;
    double f2;

    OutMessage(
        uint32_t p_count_days, double p_vwap, uint32_t p_volume,
        double p_f2) : count_days(p_count_days), vwap(p_vwap),
        volume(p_volume), f2(p_f2) {}
};

int processing_data(const string &stock_name,
                    const vector<OutMessage> &out_items){
    ofstream o_fs(
        OUTPUT_PREFIX + stock_name + FILE_EXT, ofstream::binary);
    if(!o_fs){
        cout << "Could not open input file" << endl;
        return 1;
    }

    for(auto it = out_items.begin(); it != out_items.end(); it++){
        o_fs.write((char *)stock_name.c_str(), 8);
        o_fs << '\0';
        o_fs.write((char *)&it->count_days, sizeof(it->count_days));
        o_fs.write((char *)&it->vwap, sizeof(it->vwap));
        o_fs.write((char *)&it->volume, sizeof(it->volume));
        o_fs.write((char *)&it->f2, sizeof(it->f2));
    }
    o_fs.close();
    return 0;
}

void threads_join(vector<thread> &threads){
    for(auto &t : threads){
        t.join();
    }
}

int main(){
    ifstream i_fs(SOURCE_DIR "/input.txt", ifstream::binary);
    if(!i_fs){
        cout << "Could not open input file" << endl;
        return 1;
    }
    Message message;
    map<string, vector< OutMessage > > out_data;
    short unsigned int year, month, day;
    uint32_t count_days;

    while(i_fs.good()){
        fill((char *)&message, (char *)&message + sizeof(Message), 0);

        i_fs.read((char *)&message.stock_name, sizeof(message.stock_name));
        i_fs.read((char *)&message.date_time, sizeof(message.date_time));
        i_fs.read((char *)&message.price, sizeof(message.price));
        i_fs.read((char *)&message.vwap, sizeof(message.vwap));
        i_fs.read((char *)&message.volume, sizeof(message.volume));
        
        i_fs.read((char *)&message.volume, sizeof(message.f1));
        i_fs.read((char *)&message.t1, sizeof(message.t1));
        i_fs.read((char *)&message.f2, sizeof(message.f2));
        i_fs.read((char *)&message.f3, sizeof(message.f3));
        i_fs.read((char *)&message.f4, sizeof(message.f4));
        if(i_fs.fail()){
            break;
        }
        
        sscanf(message.date_time, "%4hu%2hu%2hu", &year, &month, &day);

        count_days = 372*(year-1) + 31*(month-1) + day;

        out_data[string(message.stock_name)].push_back(
            OutMessage(count_days, message.vwap, message.volume, message.f2));
    }
    vector<thread> threads;

    for(auto it = out_data.begin(); it != out_data.end(); it++){
        try{
            threads.push_back(
                    thread(processing_data,
                           cref(it->first),
                           cref(it->second)));
        }catch(const system_error &e){
            cout << "system error: code = " << e.code();
            cout << " what = " << e.what() << endl;
            if(e.code().value() == EAGAIN){
                threads_join(threads);
                threads.clear();
                threads.push_back(
                    thread(processing_data,
                           cref(it->first),
                           cref(it->second)));
            }

        }
    }
    i_fs.close();
    threads_join(threads);

    return 0;
}
