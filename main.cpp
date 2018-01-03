#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;


// ADDRANDOMLINK: add a random non-existing link to the graph
void addRandomLink (long n, int step, int steps, bool debug, string filename, string tmp_filename) {
    long node_1;
    long node_2;
    fstream graph;
    fstream tmp_graph;

    string line;
    string target_line;
    string target_line_reverse;

    bool exists = false;

    int len;

    if (debug)
        cout << endl << "[DEBUG] Creating a link ..." << endl;

    tmp_graph.open(filename);
    if (! tmp_graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the temp file" << endl;

        return;
    }

    node_1 = rand() % (n+1);
    node_2 = rand() % (n+1);

    if (debug) {
        cout << "[DEBUG] node_1: " << node_1 << endl;
        cout << "[DEBUG] node_2: " << node_2 << endl;
    }

    // check the non-existence of the link
    // ******************************************************************

    target_line = to_string(node_1) + " " + to_string(node_2);
    target_line_reverse = to_string(node_2) + " " + to_string(node_1);
    len = target_line.length();

    if (debug){
        cout << "[DEBUG] target line: " << target_line << endl;
        cout << "[DEBUG] target line reverse: " << target_line_reverse << endl;
    }

    while (getline(tmp_graph, line)) {
        exists = false;
        //cout << "LINE: " << line << endl;

        if (line.find(target_line.c_str(), 0, len) != string::npos || line.find(target_line_reverse.c_str(), 0, len) != string::npos ){
            exists = true;
            if (debug)
                cout << "[DEBUG] the link exists" << endl;

            break;
        }
    }
    tmp_graph.clear();
    tmp_graph.seekg(0, ios::beg);

    // slow if there are a lot of links
    while (exists) {
        exists = false;

        node_1 = rand() % (n+1);
        node_2 = rand() % (n+1);

        if (debug) {
            cout << "[DEBUG] node_1: " << node_1 << endl;
            cout << "[DEBUG] node_2: " << node_2 << endl;
        }

        target_line = to_string(node_1) + " " + to_string(node_2);
        target_line_reverse = to_string(node_2) + " " + to_string(node_1);
        len = target_line.length();

        if (debug){
            cout << "[DEBUG] target line: " << target_line << endl;
            cout << "[DEBUG] target line reverse: " << target_line_reverse << endl;
        }

        while (getline(tmp_graph, line)) {
            if (line.find(target_line.c_str(), 0, len) != string::npos  || line.find(target_line_reverse.c_str(), 0, len) != string::npos){
                exists = true;
            }
        }
        tmp_graph.clear();
        tmp_graph.seekg(0, ios::beg);

    }
    tmp_graph.close();
    // ******************************************************************

    //add the link to the file
    graph.open(filename, fstream::app);
    if (! graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the file" << endl;

        return;
    }

    tmp_graph.open(tmp_filename, fstream::out | fstream::app);
    if (! tmp_graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the temp file" << endl;

        return;
    }

    if (debug)
        cout << "[DEBUG] adding: " << node_1 << "-" << node_2 << endl;

    tmp_graph << node_1 << " " << node_2 << endl;
    graph << node_1 << " " << node_2 << " " << steps-step << " C\n";

    tmp_graph.close();
    graph.close();
    if (debug)
        cout << endl;
}


// REMOVERANDOMLINK: remove a random existing link from the graph
void removeRandomLink (long n, int step, int steps, bool debug, string filename, string tmp_filename, string log_tmp_filename) {
    long node_1;
    long node_2;
    fstream graph;
    fstream tmp_graph;
    fstream log_tmp_graph;

    string line;
    string target_line;
    string target_line_reverse;

    bool exists = false;

    int len;


    if (debug)
        cout << endl << "[DEBUG] Deleting a link ..." << endl;

    tmp_graph.open(tmp_filename);
    if (! tmp_graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the temp file" << endl;

        return;
    }

    node_1 = long (rand() % (n+1));
    node_2 = long (rand() % (n+1));


    if (debug) {
        cout << "[DEBUG] node_1: " << node_1 << endl;
        cout << "[DEBUG] node_2: " << node_2 << endl;
    }

    // check the existence of the link

    target_line = to_string(node_1) + " " + to_string(node_2);
    target_line_reverse = to_string(node_2) + " " + to_string(node_1);
    len = target_line.length();

    if (debug) {
        cout << "[DEBUG] target line: " << target_line << endl;
        cout << "[DEBUG] target line reverse: " << target_line_reverse << endl;
    }

    while (getline(tmp_graph, line)) {
        if (line.find(target_line.c_str(), 0, len) != string::npos || line.find(target_line_reverse.c_str(), 0, len) != string::npos){
            exists = true;
        }
    }
    tmp_graph.clear();
    tmp_graph.seekg(0, ios::beg);

    while (!exists) {
        exists = false;

        node_1 = rand() % (n+1);
        node_2 = rand() % (n+1);

        if (debug) {

            cout << "[DEBUG] node_1: " << node_1 << endl;
            cout << "[DEBUG] node_2: " << node_2 << endl;

        }

        target_line = to_string(node_1) + " " + to_string(node_2);
        target_line_reverse = to_string(node_2) + " " + to_string(node_1);
        len = target_line.length();

        if (debug) {
            cout << "[DEBUG] target line: " << target_line << endl;
            cout << "[DEBUG] target line reverse: " << target_line_reverse << endl;
        }

        while (getline(tmp_graph, line)) {
            if (line.find(target_line.c_str(), 0, len) != string::npos || line.find(target_line_reverse.c_str(), 0, len) != string::npos){
                exists = true;
                break;
            }
        }
        tmp_graph.clear();
        tmp_graph.seekg(0, ios::beg);
    }

    // remove the link from the file

    graph.open(filename, fstream::out | fstream::app);
    if (! graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the file" << endl;

        return;
    }

    log_tmp_graph.open(log_tmp_filename, fstream::out | fstream::app);
    if (! log_tmp_graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the log temp file" << endl;

        return;
    }

    while (getline(tmp_graph, line)) {
        if (line.find(target_line.c_str(), 0, len) != string::npos || line.find(target_line_reverse.c_str(), 0, len) != string::npos){
            graph << node_1 << " " << node_2 << " " << steps-step+1 << " D\n";
        }
        if (line.find(target_line.c_str(), 0, len) == string::npos && line.find(target_line_reverse.c_str(), 0, len) == string::npos){
            log_tmp_graph << line << endl;
        }
    }

    tmp_graph.clear();
    tmp_graph.seekg(0, ios::beg);

    const char * c_tmp_filename = tmp_filename.c_str();
    const char * c_log_tmp_filename = log_tmp_filename.c_str();

    std::rename(c_tmp_filename , "../log.graph");
    std::rename(c_log_tmp_filename, c_tmp_filename);

    graph.close();
    tmp_graph.close();
    log_tmp_graph.close();

    if (debug)
        cout << endl;
}


//READFILE
void readFile (bool debug, string filename) {
    string line;
    fstream graph;

    cout << endl << "[STATUS] Printing the graph ..." << endl;

    graph.open(filename);
    if (! graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the file" << endl;

        return;
    }
    while (getline(graph, line)) {
        cout << "[OUT] " << line << endl;
    }
    graph.clear();
    graph.seekg(0, ios::beg);

    graph.close();
}


// EDGEMARKOVIAN: the core of the evolving edge-markovian model
void edgeMarkovian (long n, float p, float q, int steps, bool debug, string filename, string tmp_filename, string log_tmp_filename){
    int prob_creation;
    int prob_deletion;

    int max_steps = steps;

    ofstream graph;
    ofstream tmp_graph;

    //ROOM FOR IMPROVEMENTS: load the graph in memory as an adjList

    graph.open (filename);
    if (! graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the file" << endl;

        return;
    }

    tmp_graph.open(tmp_filename);
    if (! tmp_graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the tmp file" << endl;

        return;
    }

    graph.close();
    tmp_graph.close();

    srand(time(0));

    p = p * 100;
    q = q * 100;

    if (debug) {
        cout << "p = " << p << endl;
        cout << "q = " << q << endl;
    }

    while (steps > 0) {
        prob_creation = rand() % 100;
        prob_deletion = rand() % 100;

        if (debug) {
            cout << "prob_creation = " << prob_creation << endl;
            cout << "prob_deletion = " << prob_deletion << endl;
        }

        if (prob_creation < p) {
            addRandomLink (n, steps, max_steps, debug, filename, tmp_filename);
        }

        if (prob_deletion < q) {
            removeRandomLink (n, steps, max_steps, debug, filename, tmp_filename, log_tmp_filename);
        }
        steps --;
    }
}


// MAIN: everything is tested in this function
int main() {
    long nodes;
    float p, q;
    int steps;
    bool debug;

    string filename;
    string tmp_filename;
    string log_tmp_filename;

    filename = "../edgeMarkovian.graph";
    tmp_filename = "../tmp_edgeMarkovian.graph";
    log_tmp_filename = "../log_edgeMarkovian.graph";

    nodes = 100;
    p = 0.8;
    q = 0.2;
    steps = 1000;

    debug = true;

    cout << "[STATUS] Running the program ..." << endl;

    edgeMarkovian(nodes, p, q, steps, debug, filename, tmp_filename, log_tmp_filename);
    //readFile(debug, filename);

    return 0;
}