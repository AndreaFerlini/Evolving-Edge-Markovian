#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

long edges;

// ADDRANDOMLINK: add a random non-existing link to the graph
void addRandomLink (int n, int step, int steps, bool debug, string filename, bool **matrix) {
    int node_1 = 0;
    int node_2 = 0;

    fstream graph;

    if (debug)
        cout << endl << "[DEBUG] Creating a link ..." << endl;

    // pick for the first time two random nodes

    do {
        node_1 = rand() % (n);
        node_2 = rand() % (n);
    } while (node_1 == node_2);

    if (debug) {
        cout << "[DEBUG] node_1: " << node_1 << endl;
        cout << "[DEBUG] node_2: " << node_2 << endl;
    }

    // check the non-existence of the link:
    // pick two different random nodes if the link exists (true in the matrix)
    // ******************************************************************

    if (debug)
        cout << "matrix: " << matrix[node_1][node_2] << endl;

    while (matrix[node_1][node_2] || matrix[node_2][node_1]) {

        if (debug)
            cout << "[DEBUG] the link exists" << endl;

        do {
            node_1 = rand() % (n);
            node_2 = rand() % (n);
        } while (node_1 == node_2);

        if (debug) {
            cout << "[DEBUG] node_1: " << node_1 << endl;
            cout << "[DEBUG] node_2: " << node_2 << endl;
        }
    }

    // ******************************************************************

    // add the link to the file
    // ------------------------------------------------------------------

    graph.open(filename, fstream::app);
    if (! graph.is_open()){

        cout << "[ERROR] Impossible to open the file" << endl;

        return;
    }

    if (debug)
        cout << "[DEBUG] adding: " << node_1 << "-" << node_2 << endl;

    graph << steps-step << " " << node_1 << " " << node_2 << " C\n";

    // updating the matrix after adding the link
    matrix[node_1][node_2] = true;

    // incrementing the global variable
    edges ++;

    graph.close();

    // ------------------------------------------------------------------

    if (debug)
        cout << endl;
}


// REMOVERANDOMLINK: remove a random existing link from the graph
void removeRandomLink (int n, int step, int steps, bool debug, string filename, bool **matrix) {
    int node_1=0;
    int node_2=0;

    fstream graph;

    if (debug)
        cout << endl << "[DEBUG] Deleting a link ..." << endl;

    // pick for the first time two random nodes

    node_1 = int (rand() % (n));
    node_2 = int (rand() % (n));


    if (debug) {
        cout << "[DEBUG] node_1: " << node_1 << endl;
        cout << "[DEBUG] node_2: " << node_2 << endl;
    }

    // check the existence of the link:
    // pick two different random nodes if the link does not exist (false in the matrix)
    // ******************************************************************

    if (debug)
        cout << "matrix: " << matrix[node_1][node_2] << endl;

    while (!matrix[node_1][node_2] && !matrix[node_2][node_1]) {

        if (debug)
            cout << "[DEBUG] the link exists" << endl;

        do {
            node_1 = rand() % (n);
            node_2 = rand() % (n);
        } while (node_1 == node_2);

        if (debug) {
            cout << "[DEBUG] node_1: " << node_1 << endl;
            cout << "[DEBUG] node_2: " << node_2 << endl;
        }
    }

    // ******************************************************************

    // remove the link from the file
    // ------------------------------------------------------------------

    graph.open(filename, fstream::app);
    if (! graph.is_open()){

        cout << "[ERROR] Impossible to open the file" << endl;

        return;
    }

    if (matrix[node_1][node_2]){
        graph << steps-step+1 << " " << node_1 << " " << node_2 << " D\n";

        // updating the matrix after removing the link
        matrix[node_1][node_2] = false;
    } else {
        graph << steps-step+1 << " " << node_2 << " " << node_1 << " D\n";

        // updating the matrix after removing the link
        matrix[node_2][node_1] = false;
    }

    // decrementing the global variable
    edges --;

    graph.close();

    // ------------------------------------------------------------------

    if (debug)
        cout << endl;
}


// EDGEMARKOVIAN: the core of the evolving edge-markovian model
void edgeMarkovian (int n, double p, double q, int steps, bool debug, string filename){
    double prob_creation = 0;
    double prob_deletion = 0;

    int max_steps = steps;

    ofstream graph;

    bool **links_matrix = new bool *[n]; // rows

    long max_num_edges = ((n-1)*n)/2;
    long non_existing_edges = max_num_edges;

    int created=0;
    int deleted=0;

    edges = 0;

    // creation of the matrix (n*n)
    for (int i = 0; i < n; i ++) {
        links_matrix[i] = new bool [n]; // columns
    }

    // init the matrix to false
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            links_matrix[i][j] = false;
        }
    }

    graph.open (filename);
    if (! graph.is_open()){

        cout << "[ERROR] Impossible to open the file" << endl;

        return;
    }

    graph.close();

    srand(time(0));

    if (debug) {
        cout << "p = " << p << endl;
        cout << "q = " << q << endl;
    }

    while (steps > 0) {

        non_existing_edges = max_num_edges - edges;

        if (non_existing_edges == 0) {
            cout << "[STATUS] All the edges have been created" << endl;
        }

        for (int j = 0; j < non_existing_edges; j ++) {
            prob_creation = ((double) rand() / (RAND_MAX));
            if (prob_creation < p) {
                addRandomLink(n, steps, max_steps, debug, filename, &*links_matrix);
                created ++;
            }
        }

        long e = edges; // e is used to debug, can be replaced with edges

        for (int i = 0; i < e; i ++) {
            prob_deletion = ((double) rand() / (RAND_MAX));
            if (prob_deletion < q) {
                removeRandomLink (n, steps, max_steps, debug, filename, &*links_matrix);
                deleted ++;
            }
        }

        cout << "t = " << max_steps - steps << endl;
        cout << "created links = " << created << endl;
        cout << "deleted links = " << deleted << endl;
        cout << "links number = " << edges << endl;
        cout << endl;
        steps --;
    }

    // free the memory
    for (int i = 0; i < n; ++i){
        delete [] links_matrix[i];
    }
    delete [] links_matrix;
}


// MAIN: everything is tested in this function
int main() {
    int nodes;
    double p, q;
    int steps;
    bool debug;

    string filename;

    filename = "../edgeMarkovian.graph";

/*
 * ROLLER
 * */
/*
    nodes = 62;
    p = 0.00322543;
    q = 0.144295;
    steps = 9977;
*/

/*
 * INFOCOM
 * */

    nodes = 98;
    p = 9.54057e-05;
    q = 0.0166691;
    steps = 337419;

    debug = false;

    cout << "[STATUS] Running the program ..." << endl;

    edgeMarkovian(nodes, p, q, steps, debug, filename);

    return 0;
}