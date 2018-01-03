#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;


// ADDRANDOMLINK: add a random non-existing link to the graph
void addRandomLink (long n, bool debug, string filename) {
    long node_1;
    long node_2;
    fstream graph;

    string line;
    string target_line;
    string target_line_reverse;

    bool exists = false;

    if (debug)
        cout << endl << "[DEBUG] Creating a link ..." << endl;

    graph.open(filename);
    if (! graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the file" << endl;

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

    if (debug){
        cout << "[DEBUG] target line: " << target_line << endl;
        cout << "[DEBUG] target line reverse: " << target_line_reverse << endl;
    }

    while (getline(graph, line)) {
        exists = false;
        //cout << "LINE: " << line << endl;

        if (line.find(target_line) != string::npos || line.find(target_line_reverse) != string::npos ){
            exists = true;
            if (debug)
                cout << "[DEBUG] the link exists" << endl;

            break;
        }
    }
    graph.clear();
    graph.seekg(0, ios::beg);

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
        if (debug){
            cout << "[DEBUG] target line: " << target_line << endl;
            cout << "[DEBUG] target line reverse: " << target_line_reverse << endl;
        }

        while (getline(graph, line)) {
            if (line.find(target_line) != string::npos  || line.find(target_line_reverse) != string::npos){
                exists = true;
            }
        }
        graph.clear();
        graph.seekg(0, ios::beg);

    }
    graph.close();
    // ******************************************************************

    //add the link to the file
    graph.open(filename, fstream::app);
    if (! graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the file" << endl;

        return;
    }

    if (debug)
        cout << "[DEBUG] adding: " << node_1 << "-" << node_2 << endl;

    graph << node_1 << " " << node_2 << "\n";
    graph.close();
    if (debug)
        cout << endl;
}


// REMOVERANDOMLINK: remove a random existing link from the graph
void removeRandomLink (long n, bool debug, string filename, string tmp_filename) {
    long node_1;
    long node_2;
    fstream graph;
    fstream tmp_graph;

    string line;
    string target_line;
    string target_line_reverse;

    //string tmp_filename = "../tmp.edgeMarkovian.graph";

    bool exists = false;


    if (debug)
        cout << endl << "[DEBUG] Deleting a link ..." << endl;

    graph.open(filename);
    if (! graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the file" << endl;

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

    if (debug) {
        cout << "[DEBUG] target line: " << target_line << endl;
        cout << "[DEBUG] target line reverse: " << target_line_reverse << endl;
    }

    while (getline(graph, line)) {
        if (line.find(target_line) != string::npos || line.find(target_line_reverse) != string::npos){
            exists = true;
        }
    }
    graph.clear();
    graph.seekg(0, ios::beg);

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

        if (debug) {
            cout << "[DEBUG] target line: " << target_line << endl;
            cout << "[DEBUG] target line reverse: " << target_line_reverse << endl;
        }

        while (getline(graph, line)) {
            if (line.find(target_line) != string::npos || line.find(target_line_reverse) != string::npos){
                exists = true;
                break;
            }
        }
        graph.clear();
        graph.seekg(0, ios::beg);
    }

    // remove the link from the file
    tmp_graph.open(tmp_filename, fstream::out | fstream::app);
    if (! tmp_graph.is_open()){

        if (debug)
            cout << "[ERROR] Impossible to open the temp file" << endl;

        return;
    }

    while (getline(graph, line)) {
        if (line.find(target_line) == string::npos && line.find(target_line_reverse) == string::npos){
            tmp_graph << line << endl;
        }
    }
    graph.clear();
    graph.seekg(0, ios::beg);

    const char * c_filename = filename.c_str();
    const char * c_tmp_filename = tmp_filename.c_str();

    std::rename(c_filename, "../log.graph");
    std::rename(c_tmp_filename, c_filename);

    graph.close();
    tmp_graph.close();

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
void edgeMarkovian (long n, float p, float q, int steps, bool debug, string filename, string tmp_filename){
    int prob_creation;
    int prob_deletion;
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
            addRandomLink (n, debug, filename);
        }

        if (prob_deletion < q) {
            removeRandomLink (n, debug, filename, tmp_filename);
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

    filename = "../edgeMarkovian.graph";
    tmp_filename = "../tmp_edgeMarkovian.graph";

    nodes = 100;
    p = 0.8;
    q = 0.2;
    steps = 1000;

    debug = true;

    cout << "[STATUS] Running the program ..." << endl;

    edgeMarkovian(nodes, p, q, steps, debug, filename, tmp_filename);
    readFile(debug, filename);

    return 0;
}