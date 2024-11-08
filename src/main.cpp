#include <bits/stdc++.h>


using namespace std;

// Function to calculate the Euclidean distance between two points
double euclidean_distance(const vector<double>& p1, const vector<double>& p2) {
    double sum = 0.0;
    for (size_t i = 0; i < p1.size(); i++) {
        sum += pow(p1[i] - p2[i], 2);
    }
    return sqrt(sum);
}

// Function to find the e-neighborhood of a point
vector<int> region_query(const vector<vector<double>>& D, int p, double epsilon) {
    vector<int> neighbors;
    for (int i = 0; i < D.size(); ++i) {
        if (euclidean_distance(D[p], D[i]) <= epsilon) {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

// DBSCAN algorithm implementation
void dbscan(const vector<vector<double>>& D, double epsilon, int MinPts) {
    int n = D.size(); // number of points
    vector<bool> visited(n, false);  // visited flag for each point
    vector<int> cluster_labels(n, -1); // -1 means noise, otherwise cluster ID
    int current_cluster = 0;  // Cluster counter

    // Main DBSCAN loop
    for (int p = 0; p < n; ++p) {
        if (visited[p]) continue;  // Skip already visited points
        visited[p] = true;
        
        // Find the e-neighborhood of point p
        vector<int> neighbors = region_query(D, p, epsilon);
        
        if (neighbors.size() < MinPts) {
            // Not enough neighbors -> mark as noise
            cluster_labels[p] = -1;
        } else {
            // Expand the cluster
            current_cluster++;
            cluster_labels[p] = current_cluster;

            // Process the neighbors of p
            queue<int> q;
            for (int neighbor : neighbors) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    vector<int> neighbor_neighbors = region_query(D, neighbor, epsilon);
                    if (neighbor_neighbors.size() >= MinPts) {
                        // Add new points to the neighborhood
                        neighbors.insert(neighbors.end(), neighbor_neighbors.begin(), neighbor_neighbors.end());
                    }
                }

                if (cluster_labels[neighbor] == -1) {
                    // If the neighbor was marked as noise, add it to the cluster
                    cluster_labels[neighbor] = current_cluster;
                }
            }
        }
    }

    // Output the results
    for (int i = 0; i < n; ++i) {
        cout << "Point " << i << ": Cluster " << cluster_labels[i] << endl;
    }
}

// Function to read data from a txt file into a vector of vectors
bool read_data_from_file(const string& filename, vector<vector<double>>& data) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<double> point;
        double value;
        while (ss >> value) {
            point.push_back(value);
            if (ss.peek() == ',' || ss.peek() == ' ') ss.ignore();  // skip commas and spaces
        }
        data.push_back(point);
    }
    file.close();
    return true;
}

int main() {
    vector<vector<double>> data;

    // Read data from the file
    string filename = "dbdata.txt";  // Change this to the actual file name
    if (!read_data_from_file(filename, data)) {
        return -1;
    }

    // DBSCAN parameters
    double epsilon = 0.5;  // ? (epsilon) radius
    int MinPts = 4;        // Minimum number of points for a neighborhood

    // Perform DBSCAN clustering
    dbscan(data, epsilon, MinPts);

    return 0;
}
