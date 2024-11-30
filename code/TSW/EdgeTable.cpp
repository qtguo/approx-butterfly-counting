#include "EdgeTable.h"

void EdgeTable::set_time_list(int pos, int type, int aim) // type == 0 means prev pointer, and type == 1 means next pointer
{
    if(pos==-1){
        return;
    }
    if(pos<size) // set the time list pointer of sample unit at the corresponding pos
    {
        if(type==0)
            table[pos].time_list_prev = aim;
        else
            table[pos].time_list_next = aim;
    }
    else  // set the time list pointer of candidate unit at the corresponding pos
    {
        if(type==0)
            table[pos%size].vice.time_list_prev = aim;
        else
            table[pos%size].vice.time_list_next = aim;

    }
}
void EdgeTable::replace_sample(unsigned int s_num, unsigned int d_num, double p, long long time, int pos) // replace the sample edge at pos with a new edge;
{
    // the cross list will be changed in the upper level, as node table is needed.
    int tsl_pos = pos;

    int prev = table[pos].time_list_prev;
    int next = table[pos].time_list_next;
    set_time_list(prev, 1, next);
    set_time_list(next, 0, prev);
    if(tsl_head == tsl_pos)
        tsl_head = next;
    if(expiration == tsl_pos)
        expiration = next;
    if(tsl_tail == tsl_pos)
        tsl_tail = prev;  // split this pos from the time list.

    table[pos].reset(s_num, d_num, p, time, tsl_tail, -1); // insert the new edge;

    set_time_list(tsl_tail, 1, tsl_pos);
    tsl_tail = tsl_pos;
    if(tsl_head == -1)
        tsl_head = tsl_pos;
    if(expiration == -1)
        expiration = tsl_pos;
    // the cross list of the new edge remain to be set by the upper level, as node table is needed.
//    if(table[tsl_tail].s>800000000 || table[tsl_tail].d>800000000){
//        cout<<"%%%% replace_sample"<<endl;
//        exit(-1);
//    }
}
void EdgeTable::replace_vice(unsigned int s_num, unsigned int d_num, double p, long long time, int pos) // replace the vice edge at pos with a new edge;
{
    int tsl_pos = pos + size;

    int prev = table[pos].vice.time_list_prev;
    // cout<<"pppp "<<prev<<" "<<table[pos].vice.time_list_prev<<endl;
    int next = table[pos].vice.time_list_next;
    set_time_list(prev, 1, next);
    set_time_list(next, 0, prev);
    // cout<<"qqqq "<<tsl_pos<<" "<<tsl_head<<" "<<tsl_tail<<endl;

    if(tsl_head == tsl_pos)
        tsl_head = next;
    if(expiration == tsl_pos)
        expiration = next;
    if(tsl_tail == tsl_pos)
        tsl_tail = prev;

    table[pos].vice.reset(s_num, d_num, p, time, tsl_tail, -1); // insert the new edge;
    set_time_list(tsl_tail, 1, tsl_pos);
    tsl_tail = tsl_pos;
    if (tsl_head == -1)
        tsl_head = tsl_pos;
    if(expiration == -1)
        expiration = tsl_pos;
//    if(table[tsl_tail].s>800000000 || table[tsl_tail].d>800000000){
//        cout<<"%%%% replace_vice"<<endl;
//        exit(-1);
//    }
}
void EdgeTable::delete_sample(int pos) // delete the sample edge in the pos
{
    int tsl_pos = pos;

    int prev = table[pos].time_list_prev;
    int next = table[pos].time_list_next;
    set_time_list(prev, 1, next);
    set_time_list(next, 0, prev);
    if(tsl_head == tsl_pos)
        tsl_head = next;
    if(expiration == tsl_pos)
        expiration = next;
    if(tsl_tail == tsl_pos)
        tsl_tail = prev;

    table[pos].reset(); // reset the bucket;
//    if(table[tsl_tail].s>800000000 || table[tsl_tail].d>800000000){
//        cout<<"%%%% delete_sample"<<endl;
//        exit(-1);
//    }
}
void EdgeTable::delete_vice(int pos)
{
    int tsl_pos = pos + size;

    int prev = table[pos].vice.time_list_prev;
    int next = table[pos].vice.time_list_next;
    set_time_list(prev, 1, next);
    set_time_list(next, 0, prev);

    if(tsl_head == tsl_pos)
        tsl_head = next;
    if(expiration == tsl_pos)
        expiration = next;
    if(tsl_tail == tsl_pos)
        tsl_tail = prev;

    table[pos].vice.reset(); // reset the bucket;
//    if(table[tsl_tail].s>800000000 || table[tsl_tail].d>800000000){
//        cout<<"%%%% delete_vice"<<endl;
//        exit(-1);
//    }
}

void EdgeTable::update_sample(int pos, long long time)
{
    int tsl_pos = pos;
    int prev = table[pos].time_list_prev;
    int next = table[pos].time_list_next;

    table[pos].timestamp = time;

    set_time_list(prev, 1, next);
    set_time_list(next, 0, prev);
    if(tsl_head == tsl_pos)
        tsl_head = next;
    if(expiration == tsl_pos)
        expiration = next;
    if(tsl_tail == tsl_pos)
        tsl_tail = prev;

    table[pos].time_list_prev = tsl_tail;
    table[pos].time_list_next = -1;
    set_time_list(tsl_tail, 1, tsl_pos);
    tsl_tail = tsl_pos;
    if (tsl_head == -1)
        tsl_head = tsl_pos;
    if(expiration == -1)
        expiration = tsl_pos;
//    if(table[tsl_tail].s>800000000 || table[tsl_tail].d>800000000){
//        cout<<"%%%% update_sample"<<endl;
//        exit(-1);
//    }
}

void EdgeTable::update_vice(int pos, long long time)
{
    int tsl_pos = pos + size;
    int prev = table[pos].vice.time_list_prev;
    int next = table[pos].vice.time_list_next;

    table[pos].vice.timestamp = time;

    set_time_list(prev, 1, next);
    set_time_list(next, 0, prev);
    if(tsl_head == tsl_pos)
        tsl_head = next;
    if(expiration == tsl_pos)
        expiration = next;
    if(tsl_tail == tsl_pos)
        tsl_tail = prev;

    table[pos].vice.time_list_prev = tsl_tail;
    table[pos].vice.time_list_next = -1;
    set_time_list(tsl_tail, 1, tsl_pos);
    tsl_tail = tsl_pos;
    if (tsl_head == -1)
        tsl_head = tsl_pos;
    if(expiration == -1)
        expiration = tsl_pos;
//    if(table[tsl_tail].s>800000000 || table[tsl_tail].d>800000000){
//        cout<<"%%%% update_vice"<<endl;
//        exit(-1);
//    }
}