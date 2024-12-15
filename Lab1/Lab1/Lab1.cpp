#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Component class
class Component {
private:
    string name;
    string manufacturer;
    string specifications;

public:
    Component(string name, string manufacturer, string specifications) {
        this->name = name;
        this->manufacturer = manufacturer;
        this->specifications = specifications;
    }

    void showDetails() {
        cout << "Component: " << name << endl;
        cout << "Manufacturer: " << manufacturer << endl;
        cout << "Specifications: " << specifications << endl;
    }
};

// ClusterNode class
class ClusterNode {
private:
    string nodeName;
    vector<Component> componentList;

public:
    ClusterNode(string nodeName) {
        this->nodeName = nodeName;
    }

    void insertComponent(Component c) {
        componentList.push_back(c);
    }

    void showNode() {
        cout << "Cluster Node: " << nodeName << endl;
        for (int i = 0; i < componentList.size(); i++) {
            componentList[i].showDetails();
            cout << endl;
        }
    }
};

// Cluster class
class Cluster {
private:
    string clusterName;
    vector<ClusterNode> nodeList;

public:
    Cluster(string clusterName) {
        this->clusterName = clusterName;
    }

    void addClusterNode(ClusterNode cn) {
        nodeList.push_back(cn);
    }

    void showCluster() {
        cout << "Cluster: " << clusterName << endl;
        for (int i = 0; i < nodeList.size(); i++) {
            nodeList[i].showNode();
            cout << "-------------------------" << endl;
        }
    }
};

int main() {
    // Create components
    Component cpu("CPU", "Intel", "3.5GHz, 6 cores");
    Component ram("RAM", "Corsair", "16GB DDR4");
    Component storage("Storage", "Samsung", "1TB SSD");

    // Create a node and add components
    ClusterNode node1("Node1");
    node1.insertComponent(cpu);
    node1.insertComponent(ram);
    node1.insertComponent(storage);

    // Create a cluster and add the node
    Cluster cluster("Cluster1");
    cluster.addClusterNode(node1);

    // Display the cluster
    cluster.showCluster();

    return 0;
}
