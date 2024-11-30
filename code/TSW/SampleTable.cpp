#include "SampleTable.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
using namespace std;

void SampleTable::modify_butterfly(Node *pos_s, Node *pos_d, long long last_mark, int op, long long w_time, int pos) // op = 1 means add, op = -1 means minus
{
    unsigned int s_num = pos_s->nodeID;
    // unsigned int d_num = pos_d->nodeID;
    // int edge_s = pos_s->first_edge;
    int edge_d = pos_d->first_edge;

    while (edge_d >= 0)
    {
        assert(edge_d < size);
        unsigned int tmp;
        int next_index;
        tmp = edge_table->table[edge_d].s;
        next_index = edge_table->table[edge_d].pointers[last_d]; // index of the next edge
        // cout<<next_index<<endl;
        // cout<<edge_d<<" "<<next_index<<endl;
        if (tmp == s_num)
        {
            edge_d = next_index;
            continue;
        }
        if (!(edge_table->table[edge_d].vice.timestamp < last_mark || edge_table->table[edge_d].vice.timestamp > edge_table->table[edge_d].timestamp))
        {
            edge_d = next_index;
            continue;
        } // TODO: only count the valid edge ... correct or not?

        if (op == 1)
        {
            if (edge_table->table[edge_d].timestamp < w_time)
            {
                edge_d = next_index;
                continue;
            }
            #ifdef MT
            coaffiliation_count++;
            #ifdef AMS
            ams->update(min(s_num, tmp), max(s_num, tmp), 0);
            #endif
            #ifdef REC
            rec[min(s_num, tmp)][max(s_num, tmp)]++;
            f2 += (2 * rec[min(s_num, tmp)][max(s_num, tmp)] - 1);
            #endif
            #endif
        }
        else
        {
            /*if(edge_table->table[edge_d].timestamp < w_time && edge_table->table[edge_d].timestamp < edge_table->table[pos].timestamp){
                edge_d = next_index;
                continue;
            }*/
            #ifdef MT
            coaffiliation_count--;
            #ifdef AMS
            ams->update(min(s_num, tmp), max(s_num, tmp), 1);
            #endif
            #ifdef REC
            rec[min(s_num, tmp)][max(s_num, tmp)]--;
            f2 -= (2 * rec[min(s_num, tmp)][max(s_num, tmp)] + 1);
            #endif
            #endif
        }
        edge_d = next_index;
    }
    // if(f2<coaffiliation_count){
    //     // cout<<f2<<" "<<coaffiliation_count<<endl;
    //     exit(-1);
    // }
    // cout<<op<<" "<<f2<<" "<<coaffiliation_count<<endl;
}

void SampleTable::modify_butterfly_illusion(Node *pos_s, Node *pos_d, long long land_mark, int op)
{
    unsigned int s_num = pos_s->nodeID;
    unsigned int d_num = pos_d->nodeID;
    int edge_s = pos_s->first_edge;
    int edge_d = pos_d->first_edge;

    while (edge_d >= 0)
    {
        assert(edge_d < size);
        unsigned int tmp;
        int next_index;
        tmp = edge_table->table[edge_d].s;
        next_index = edge_table->table[edge_d].pointers[last_d];
        if (tmp == s_num)
        {
            edge_d = next_index;
            continue;
        }
        if (edge_table->table[edge_d].timestamp < land_mark)
        {
            edge_d = next_index;
            continue;
        }
        if (op == 1)
        {
            #ifdef MT
            illusion_coaffiliation_count++;
            #ifdef AMS
            illusion_ams->update(min(s_num, tmp), max(s_num, tmp), 0);
            #endif
            #ifdef REC
            illusion_rec[min(s_num, tmp)][max(s_num, tmp)]++;
            illusion_f2 += (2 * illusion_rec[min(s_num, tmp)][max(s_num, tmp)] - 1);
            #endif
            #endif
        }
        else
        {
            #ifdef MT
            illusion_coaffiliation_count--;
            #ifdef AMS
            illusion_ams->update(min(s_num, tmp), max(s_num, tmp), 1);
            #endif
            #ifdef REC
            illusion_rec[min(s_num, tmp)][max(s_num, tmp)]--;
            illusion_f2 -= (2 * illusion_rec[min(s_num, tmp)][max(s_num, tmp)] + 1);
            #endif
            #endif
        }
        edge_d = next_index;
    }
    // if (illusion_f2 < illusion_coaffiliation_count)
    // {
    //     // cout << "illusion " << illusion_f2 << " " << illusion_coaffiliation_count << endl;
    //     exit(-1);
    // }
}

void SampleTable::modify_butterfly_both(Node *pos_s, Node *pos_d, long long last_mark, long long land_mark, int op, long long w_time, int pos)
{
    unsigned int s_num = pos_s->nodeID;
    unsigned int d_num = pos_d->nodeID;
    // cout<<s_num<<" "<<d_num<<endl;
    int edge_s = pos_s->first_edge;
    int edge_d = pos_d->first_edge;

    while (edge_d >= 0)
    {
        assert(edge_d < size);
        unsigned int tmp;
        int next_index;
        tmp = edge_table->table[edge_d].s;
        next_index = edge_table->table[edge_d].pointers[last_d];
        if (tmp == s_num)
        {
            edge_d = next_index;
            continue;
        }

        if (edge_table->table[edge_d].vice.timestamp < last_mark || edge_table->table[edge_d].vice.timestamp > edge_table->table[edge_d].timestamp)
        {
            // cout << "op:" << op << endl;
            if (op == 1)
            {
                if (edge_table->table[edge_d].timestamp >= w_time)
                {
                    #ifdef MT
                    coaffiliation_count++;
                    #ifdef AMS
                    ams->update(min(s_num, tmp), max(s_num, tmp), 0);
                    #endif
                    #ifdef REC
                    rec[min(s_num, tmp)][max(s_num, tmp)]++;
                    f2 += (2 * rec[min(s_num, tmp)][max(s_num, tmp)] - 1);
                    #endif
                    #endif
                }
                // rec[min(s_num,tmp)][max(s_num,tmp)]++;
                // coaffiliation_count++;
                // f2 += (2*rec[min(s_num,tmp)][max(s_num,tmp)]-1);
            }
            else
            {
                #ifdef MT
                coaffiliation_count--;
                #ifdef AMS
                ams->update(min(s_num, tmp), max(s_num, tmp), 1);
                #endif
                #ifdef REC
                rec[min(s_num, tmp)][max(s_num, tmp)]--;
                f2 -= (2 * rec[min(s_num, tmp)][max(s_num, tmp)] + 1);
                #endif
                #endif
            }
        }
        // cout << "both_1" << endl;

        if (edge_table->table[edge_d].timestamp >= land_mark)
        {
            if (op == 1)
            {
                #ifdef MT
                illusion_coaffiliation_count++;
                #ifdef AMS
                illusion_ams->update(min(s_num, tmp), max(s_num, tmp), 0);
                #endif
                #ifdef REC
                illusion_rec[min(s_num, tmp)][max(s_num, tmp)]++;
                illusion_f2 += (2 * illusion_rec[min(s_num, tmp)][max(s_num, tmp)] - 1);
                #endif
                #endif
            }
            else
            {
                #ifdef MT
                illusion_coaffiliation_count--;
                #ifdef AMS
                illusion_ams->update(min(s_num, tmp), max(s_num, tmp), 1);
                #endif
                #ifdef REC
                illusion_rec[min(s_num, tmp)][max(s_num, tmp)]--;
                illusion_f2 -= (2 * illusion_rec[min(s_num, tmp)][max(s_num, tmp)] + 1);
                #endif
                #endif
            }
        }
        // cout << "both_2" << endl;

        edge_d = next_index;
    }
    // if(f2<coaffiliation_count){
    //     // cout<<"both "<<f2<<" "<<coaffiliation_count<<endl;
    //     exit(-1);
    // }
    // cout<<"both "<<op<<" "<<f2<<" "<<coaffiliation_count<<endl;
}

void SampleTable::link_list(Node *pos_s, Node *pos_d, int pos, unsigned int s_num, unsigned int d_num)
{

    edge_table->table[pos].set_last_s(pos_s->first_edge);
    edge_table->table[pos].set_last_d(pos_d->first_edge);

    if (pos_s->first_edge >= 0)
    {
        if (edge_table->table[pos_s->first_edge].s == s_num)
            edge_table->table[pos_s->first_edge].set_next_s(pos);
        else
            edge_table->table[pos_s->first_edge].set_next_d(pos);
    }

    if (pos_d->first_edge >= 0)
    {
        if (edge_table->table[pos_d->first_edge].s == d_num)
            edge_table->table[pos_d->first_edge].set_next_s(pos);
        else
            edge_table->table[pos_d->first_edge].set_next_d(pos);
    }
    pos_s->set_first_edge(pos);
    pos_d->set_first_edge(pos); // set the cross list;
}

void SampleTable::dismiss(Node *pos_s, Node *pos_d, int pos)
{
    unsigned int s = pos_s->nodeID;
    unsigned int d = pos_d->nodeID;

    int last_edge_s = edge_table->table[pos].pointers[last_s]; // isolate this edge from the list
    int last_edge_d = edge_table->table[pos].pointers[last_d];
    int next_edge_s = edge_table->table[pos].pointers[next_s];
    int next_edge_d = edge_table->table[pos].pointers[next_d];

    if (pos_s->first_edge == pos)
    {
        // cout<<"delete source node"<<endl;
        if (last_edge_s < 0) // there are no edges left for this node
        {
            pos_s = NULL;
            node_table->delete_via_ID(s);
            node_count--;
        }
        else
            pos_s->first_edge = last_edge_s;
    }

    if (!pos_d || pos_d->nodeID != d)
    { // ?
        pos_d = node_table->ID_to_pos(d);
    }

    if (pos_d->first_edge == pos)
    {
        // cout<<"delete destinition node"<<endl;
        if (last_edge_d < 0)
        {
            pos_d = NULL;
            node_table->delete_via_ID(d);
            node_count--;
        }
        else
            pos_d->first_edge = last_edge_d;
    }

    if (last_edge_s >= 0)
    {
        //	if (last_edge_s == 14042)
        //		// cout << "pause" << endl;
        if (edge_table->table[last_edge_s].s == s)
            edge_table->table[last_edge_s].set_next_s(next_edge_s);
        else
            edge_table->table[last_edge_s].set_next_d(next_edge_s);
    }

    if (next_edge_s >= 0)
    {
        if (edge_table->table[next_edge_s].s == s)
            edge_table->table[next_edge_s].set_last_s(last_edge_s);
        else
            edge_table->table[next_edge_s].set_last_d(last_edge_s);
    }

    if (last_edge_d >= 0)
    {
        //	if (last_edge_d == 14042)
        //		// cout << "pause" << endl;
        if (edge_table->table[last_edge_d].d == d)
            edge_table->table[last_edge_d].set_next_d(next_edge_d);
        else
            edge_table->table[last_edge_d].set_next_s(next_edge_d);
    }

    if (next_edge_d >= 0)
    {
        // if (pos == 14042)
        //		// cout << "pause" << endl;
        if (edge_table->table[next_edge_d].d == d)
            edge_table->table[next_edge_d].set_last_d(last_edge_d);
        else
            edge_table->table[next_edge_d].set_last_s(last_edge_d);
    }
}

void SampleTable::dismiss(unsigned int s, unsigned int d, int pos)
{

    int last_edge_s = edge_table->table[pos].pointers[last_s]; // isolate this edge from the list
    int last_edge_d = edge_table->table[pos].pointers[last_d];
    int next_edge_s = edge_table->table[pos].pointers[next_s];
    int next_edge_d = edge_table->table[pos].pointers[next_d];

    Node *pos_s = node_table->ID_to_pos(s);

    if (pos_s->first_edge == pos)
    {
        // cout<<"delete source node"<<endl;
        if (last_edge_s < 0) // there are no edges left for this node
        {
            pos_s = NULL;
            node_table->delete_via_ID(s);
            node_count--;
        }
        else
            pos_s->first_edge = last_edge_s;
    }

    Node *pos_d = node_table->ID_to_pos(d);
    if (pos_d->first_edge == pos)
    {
        // cout<<"delete destinition node"<<endl;
        if (last_edge_d < 0)
        {
            pos_d = NULL;
            node_table->delete_via_ID(d);
            node_count--;
        }
        else
            pos_d->first_edge = last_edge_d;
    }

    if (last_edge_s >= 0)
    {
        //	if (last_edge_s == 14042)
        //		// cout << "pause" << endl;
        if (edge_table->table[last_edge_s].s == s)
            edge_table->table[last_edge_s].set_next_s(next_edge_s);
        else
            edge_table->table[last_edge_s].set_next_d(next_edge_s);
    }

    if (next_edge_s >= 0)
    {
        if (edge_table->table[next_edge_s].s == s)
            edge_table->table[next_edge_s].set_last_s(last_edge_s);
        else
            edge_table->table[next_edge_s].set_last_d(last_edge_s);
    }

    if (last_edge_d >= 0)
    {
        //	if (last_edge_d == 14042)
        //		// cout << "pause" << endl;
        if (edge_table->table[last_edge_d].d == d)
            edge_table->table[last_edge_d].set_next_d(next_edge_d);
        else
            edge_table->table[last_edge_d].set_next_s(next_edge_d);
    }

    if (next_edge_d >= 0)
    {
        // if (pos == 14042)
        //		// cout << "pause" << endl;
        if (edge_table->table[next_edge_d].d == d)
            edge_table->table[next_edge_d].set_last_d(last_edge_d);
        else
            edge_table->table[next_edge_d].set_last_s(last_edge_d);
    }
}

void SampleTable::insert(unsigned int s_num, unsigned int d_num, double p, long long time, long long land_mark, long long last_mark, int hashindex, long long window_size)
{
    string s = my_to_string(s_num);
    string d = my_to_string(d_num);
    string edge = s + d;
    unsigned int pos = (*hfunc[hashindex + 1])((unsigned char *)(edge.c_str()), edge.length()) % size;

    if (edge_table->table[pos].vice.timestamp < last_mark && edge_table->table[pos].vice.timestamp >= 0)
    {
        edge_table->table[pos].vice.reset();
    } // if the vice edge is older than the last_mark, then it is a left test edge and need to be cleaned.

    if (edge_table->table[pos].s == 0 && edge_table->table[pos].d == 0) // no edge in the current pos
    {
        // cout << "1" << endl;
        illusion_valid++;
        illusion_q = illusion_q - 1 + 1 / pow(2, int(-(log(1 - p) / log(2))) + 1); // whatever the case, in the illusion a new edge is inserted.

        Node *pos_s = node_table->ID_to_pos(s_num);
        Node *pos_d = node_table->ID_to_pos(d_num);
        if (!pos_s)
        {
            pos_s = node_table->insert(s_num);
            node_count++;
        }
        if (!pos_d)
        {
            pos_d = node_table->insert(d_num);
            node_count++;
        } // if the node is not in the table ,insert it

        if (edge_table->table[pos].vice.timestamp >= 0)
        // there may be this case: the substream has not received a new item for a long time, and the old sample become test item, but no sample;
        {
            // cout << "1_1" << endl;
            assert(edge_table->table[pos].vice.timestamp < land_mark && edge_table->table[pos].vice.timestamp >= last_mark);

            if (p >= edge_table->table[pos].vice.priority)
            {
                valid_num++;
                edge_table->replace_sample(s_num, d_num, p, time, pos);
                link_list(pos_s, pos_d, pos, s_num, d_num);
                #ifdef MT
                modify_butterfly_both(pos_s, pos_d, last_mark, land_mark, 1, time - window_size, pos); // TODO:改为更新bfc - completed
                #endif
                q_count = q_count - 1 / pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2))) + 1) + 1 / pow(2, int(-(log(1 - p) / log(2))) + 1);
                edge_table->table[pos].vice.reset();
            }
            else
            {
                edge_table->replace_sample(s_num, d_num, p, time, pos);
                link_list(pos_s, pos_d, pos, s_num, d_num);
                #ifdef MT
                modify_butterfly_illusion(pos_s, pos_d, land_mark, 1); // TODO:改为更新vision-bfc - completed
                #endif
            }
        }
        else
        {
            // cout << "1_2" << endl;
            edge_count++;
            valid_num++;
            edge_table->replace_sample(s_num, d_num, p, time, pos); // if there is no sampled edge in this substream, this is the first one
            // cout << "1_2_1" << endl;
            link_list(pos_s, pos_d, pos, s_num, d_num);
            // cout << "1_2_2" << endl;
            #ifdef MT
            modify_butterfly_both(pos_s, pos_d, last_mark, land_mark, 1, time - window_size, pos); // TODO:改为更新bfc - completed
            #endif
            // cout << "1_2_3" << endl;

            q_count = q_count - 1 + 1 / pow(2, int(-(log(1 - p) / log(2))) + 1);
            // cout <<p<<' '<< int((log(1 - p) / log(2)))<< endl;
        }
        return;
    }

    // if the inserted edge has already shown up and is sampled.

    if (edge_table->table[pos].s == s_num && edge_table->table[pos].d == d_num)
    {
        // cout << "2" << endl;
        if (edge_table->table[pos].timestamp < land_mark)
        {
            illusion_q = illusion_q - 1 + 1 / pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2))) + 1);
            illusion_valid++; // ?
            Node *pos_s = node_table->ID_to_pos(s_num);
            Node *pos_d = node_table->ID_to_pos(d_num);
            #ifdef MT
            modify_butterfly_illusion(pos_s, pos_d, land_mark, 1);
            #endif
        }
        edge_table->update_sample(pos, time);
        if (edge_table->table[pos].vice.priority < edge_table->table[pos].priority && edge_table->table[pos].vice.timestamp <= edge_table->table[pos].timestamp)
            edge_table->delete_vice(pos);
        return;
    }

    // else if the sampled edge is in last slice

    if (edge_table->table[pos].timestamp < land_mark)
    {
        // cout << "3" << endl;
        if (p >= edge_table->table[pos].priority) // if larger than the sampled p, replace it;
        {
            if (!(edge_table->table[pos].vice.timestamp >= land_mark || edge_table->table[pos].vice.timestamp < 0))
            {
                // cout << "## " << edge_table->table[pos].vice.timestamp << " " << land_mark << " " << edge_table->table[pos].timestamp << endl;
            }
            assert(edge_table->table[pos].vice.timestamp >= land_mark || edge_table->table[pos].vice.timestamp < 0); // TODO: why?
            // in this case, the vice edge is not needed anyway;
            edge_table->delete_vice(pos);
            // reset the vice edge

            // replace the sample edge
            Node *old_s = node_table->ID_to_pos(edge_table->table[pos].s);
            Node *old_d = node_table->ID_to_pos(edge_table->table[pos].d);
            q_count += -1 / pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2))) + 1) + 1 / pow(2, int(-(log(1 - p) / log(2))) + 1);
            illusion_q += -1 + 1 / pow(2, int(-(log(1 - p) / log(2))) + 1);
            illusion_valid++;
            #ifdef MT
            modify_butterfly(old_s, old_d, last_mark, -1, time - window_size, pos);
            #endif
            dismiss(old_s, old_d, pos);
            edge_table->replace_sample(s_num, d_num, p, time, pos);

            Node *pos_s = node_table->ID_to_pos(s_num);
            Node *pos_d = node_table->ID_to_pos(d_num);
            if (!pos_s)
            {
                pos_s = node_table->insert(s_num);
                node_count++;
            }
            if (!pos_d)
            {
                pos_d = node_table->insert(d_num);
                node_count++;
            } // if the node is not in the table ,insert it
            link_list(pos_s, pos_d, pos, s_num, d_num);
            #ifdef MT
            modify_butterfly_both(pos_s, pos_d, last_mark, land_mark, 1, time - window_size, pos);
            #endif
        }
        else // if smaller than the sampled p, check the vice edge;
        {
            if (p >= edge_table->table[pos].vice.priority) // can replace it;
            {
                edge_table->replace_vice(s_num, d_num, p, time, pos);
                // node_table->insert(s_num);
                //	node_table->insert(d_num);
            }
        }
    }
    else // the sample edge is larger than the landmark
    {
        // cout << "4" << endl;
        if (p >= edge_table->table[pos].priority) // if larger than the sampled p, replace it;
        {
            Node *old_s = node_table->ID_to_pos(edge_table->table[pos].s);
            Node *old_d = node_table->ID_to_pos(edge_table->table[pos].d);
            // in this case, we need to check the vice edge,

            if (edge_table->table[pos].vice.timestamp < land_mark && edge_table->table[pos].vice.timestamp >= 0) // then this is a test edge
            {

                assert(edge_table->table[pos].vice.timestamp >= last_mark);
                if (edge_table->table[pos].vice.priority <= edge_table->table[pos].priority)
                {
                    // cout << "pause" << endl;
                }
                if (p >= edge_table->table[pos].vice.priority) // the new edge can replace the test edge
                {
                    q_count = q_count - 1 / pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2))) + 1) + 1 / pow(2, int(-(log(1 - p) / log(2))) + 1);
                    illusion_q = illusion_q - 1 / pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2))) + 1) + 1 / pow(2, int(-(log(1 - p) / log(2))) + 1);

                    edge_table->delete_vice(pos); 
                    #ifdef MT                          // the test edge is not needed any more;
                    modify_butterfly_illusion(old_s, old_d, land_mark, -1); // in this case the sampled edge is only an illusion
                    #endif
                    
                    dismiss(old_s, old_d, pos);
                    edge_table->replace_sample(s_num, d_num, p, time, pos);

                    Node *pos_s = node_table->ID_to_pos(s_num);
                    Node *pos_d = node_table->ID_to_pos(d_num);
                    if (!pos_s)
                    {
                        pos_s = node_table->insert(s_num);
                        node_count++;
                    }
                    if (!pos_d)
                    {
                        pos_d = node_table->insert(d_num);
                        node_count++;
                    } // if the node is not in the table ,insert it

                    link_list(pos_s, pos_d, pos, s_num, d_num);
                    #ifdef MT
                    modify_butterfly_both(pos_s, pos_d, last_mark, land_mark, 1, time - window_size, pos);
                    #endif
                    valid_num++;
                }
                else
                {
                    illusion_q = illusion_q - 1 / pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2))) + 1) + 1 / pow(2, int(-(log(1 - p) / log(2))) + 1);
                    #ifdef MT
                    modify_butterfly_illusion(old_s, old_d, land_mark, -1); // in this case the sampled edge is only an illusion
                    #endif
                    dismiss(old_s, old_d, pos);
                    edge_table->replace_sample(s_num, d_num, p, time, pos);

                    Node *pos_s = node_table->ID_to_pos(s_num);
                    Node *pos_d = node_table->ID_to_pos(d_num);
                    if (!pos_s)
                    {
                        pos_s = node_table->insert(s_num);
                        node_count++;
                    }
                    if (!pos_d)
                    {
                        pos_d = node_table->insert(d_num);
                        node_count++;
                    } // if the node is not in the table ,insert it
                    link_list(pos_s, pos_d, pos, s_num, d_num);
                    #ifdef MT
                    modify_butterfly_illusion(pos_s, pos_d, land_mark, 1); // the new edge is still an illusion
                    #endif
                }
            }
            else // else there should be no vice edge, we replace the sampled edge.
            {
                if (edge_table->table[pos].vice.timestamp >= 0)
                {
                    // cout << edge_table->table[pos].timestamp << ' ' << time << ' ' << edge_table->table[pos].vice.timestamp << ' ' << land_mark << ' ' << last_mark << endl;
                    assert(false);
                }
                q_count = q_count - 1 / pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2))) + 1) + 1 / pow(2, int(-(log(1 - p) / log(2))) + 1);
                illusion_q = illusion_q - 1 / pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2))) + 1) + 1 / pow(2, int(-(log(1 - p) / log(2))) + 1);
                #ifdef MT
                modify_butterfly_both(old_s, old_d, last_mark, land_mark, -1, time - window_size, pos);
                #endif
                dismiss(old_s, old_d, pos);
                edge_table->replace_sample(s_num, d_num, p, time, pos);

                Node *pos_s = node_table->ID_to_pos(s_num);
                Node *pos_d = node_table->ID_to_pos(d_num);
                if (!pos_s)
                {
                    pos_s = node_table->insert(s_num);
                    node_count++;
                }
                if (!pos_d)
                {
                    pos_d = node_table->insert(d_num);
                    node_count++;
                } // if the node is not in the table ,insert it
                link_list(pos_s, pos_d, pos, s_num, d_num);
                #ifdef MT
                modify_butterfly_both(pos_s, pos_d, last_mark, land_mark, 1, time - window_size, pos);
                #endif
            }
            // reset the vice edge

            // replace the sample edge
            /* Node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
             Node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);
             trcount -= count_triangle(old_s, old_d);
             dismiss(old_s, old_d, pos);
             edge_table->replace_sample(s_num, d_num, p, time, pos);
             link_list(pos_s, pos_d, pos);*/
        }
    }

    //	if (time == 40597)
    //		// cout << "pause 1" << endl;
    return;
}

void SampleTable::update(long long time, long long land_mark, long long last_mark)
// when the sampled edge expires, delete it and move the candidate edge one rank upper.
// Before this function the cross lists including this pos should be changed, and after this function
// the new sampled edge (valid or not) should be added into the corresponding cross lists;
// time: current_time - window_size
{
    int tsl_pos = edge_table->tsl_head;
    if (tsl_pos < 0)
    {
        return;
    }
    int pos = tsl_pos % size;

    while (edge_table->table[pos].timestamp < time) // delete all the expired sampled edges
    {
        // cout<<pos<<endl;
        tsl_pos = edge_table->table[pos].time_list_next;
        assert(tsl_pos < size);

        if (edge_table->table[pos].vice.timestamp < last_mark && edge_table->table[pos].vice.timestamp >= 0)
        {
            edge_table->table[pos].vice.reset();
        } // if the vice edge is older than the last_mark, then it is a left test edge and need to be cleaned.

        if (edge_table->table[pos].vice.timestamp >= time)
        {
            // TODO: why minus one?
            illusion_q = illusion_q - 1 + 1 / pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2))) + 1);

            Node *old_s = node_table->ID_to_pos(edge_table->table[pos].s);
            Node *old_d = node_table->ID_to_pos(edge_table->table[pos].d);
            // TODO: change to modify butterfly - completed
            #ifdef MT
            modify_butterfly(old_s, old_d, last_mark, -1, time, pos); // why not also modify the illusion?
            #endif
            dismiss(old_s, old_d, pos);

            Edge tmp = edge_table->table[pos];

            edge_table->delete_sample(pos); // delete the expired sample;

            valid_num--; // the valid num decreases, but the illusion_valid do not decrease, as after the landmark this bucket will still be valid;

            edge_table->table[pos].reset(tmp.vice.s, tmp.vice.d, tmp.vice.priority, tmp.vice.timestamp, tmp.vice.time_list_prev, tmp.vice.time_list_next); // the vice edge is an invalid sample now

            edge_table->set_time_list(tmp.vice.time_list_prev, 1, pos);
            edge_table->set_time_list(tmp.vice.time_list_next, 0, pos); // the pointer used to be pos + total_size (candidate unit), now updated to pos (sample unit);
            if (edge_table->tsl_tail == pos + size)
                edge_table->tsl_tail = pos;
            if (edge_table->tsl_head == pos + size)
                edge_table->tsl_head = pos;

            Node *pos_s = node_table->ID_to_pos(tmp.vice.s);
            Node *pos_d = node_table->ID_to_pos(tmp.vice.d);
            if (!pos_s)
            {
                pos_s = node_table->insert(tmp.vice.s);
                node_count++;
            }
            if (!pos_d)
            {
                pos_d = node_table->insert(tmp.vice.d);
                node_count++;
            } // if the node is not in the table ,insert it

            link_list(pos_s, pos_d, pos, tmp.vice.s, tmp.vice.d);  // link the cross list;
            #ifdef MT
            modify_butterfly_illusion(pos_s, pos_d, land_mark, 1); // the inserted is invalid, therefore only the illusion is increased;
            #endif
            illusion_valid++;

            edge_table->table[pos].vice.reset(tmp.s, tmp.d, tmp.priority, tmp.timestamp); // set vice to be the previous sample
        }
        else // if there is no vice edge
        {
            Node *old_s = node_table->ID_to_pos(edge_table->table[pos].s);
            Node *old_d = node_table->ID_to_pos(edge_table->table[pos].d);
            // TODO: change to modify butterfly - completed
            #ifdef MT
            modify_butterfly(old_s, old_d, last_mark, -1, time, pos);
            #endif
            dismiss(old_s, old_d, pos);

            valid_num--; // TODO: why not modify illusion_valid -- illusion_valid 每过一个slice会清零，这条边已经不算在illusion_valid中

            edge_table->table[pos].vice.reset(edge_table->table[pos].s, edge_table->table[pos].d, edge_table->table[pos].priority, edge_table->table[pos].timestamp);

            edge_table->delete_sample(pos);
        }

        if (tsl_pos < 0)
            break;
        pos = tsl_pos % size;
        assert(pos == tsl_pos);
    }
}


void SampleTable::count(long long time, long long last_mark){
    vector<pair<int,int>> sample_edges;
    for(int i=0;i<edge_table->size;i++){
        if(edge_table->table[i].timestamp > 0 && (edge_table->table[i].vice.timestamp < last_mark || edge_table->table[i].vice.timestamp > edge_table->table[i].timestamp)){
            if(edge_table->table[i].timestamp<time){
                cout<<edge_table->table[i].s<<" "<<edge_table->table[i].d<<endl;
                cout<<edge_table->table[i].timestamp<<" "<<time<<endl;
            }
            assert(edge_table->table[i].timestamp>=time);
            sample_edges.push_back(make_pair(edge_table->table[i].s, edge_table->table[i].d));
        }else{
            continue;
        }
    }
    cout<<"sample edge size: "<<sample_edges.size()<<endl;
    // BiGraph sample_graph(sample_edges);
    // bfcount = bfc_exact(sample_edges);
    bfcount = (long double)batch_bfly_count(sample_edges);
}