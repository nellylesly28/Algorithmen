/*#include <iostream>
#include <fstream>
#include <climits>
#include <csignal>
#include <cstdlib>
#include <string>

class PatriciaTree {
public:

    class Node {
public:
    Node(const std::string& key, int bitPos) : m_Key(key), m_BitPos(bitPos), m_Left(nullptr), m_Right(nullptr) {}

  ~Node() {
    // Löschen Sie die Kindknoten, ohne sie erneut zu löschen
    m_Left = nullptr;
    m_Right = nullptr;
}


    std::string m_Key;
    int m_BitPos;
    Node* m_Left;
    Node* m_Right;
};



    class NodeHandler {
     public:
        static const int NODE = 0;
        static const int DAD = 1;

        NodeHandler(PatriciaTree* tree, Node* n) : m_Tree(tree) {
            m_Nodes[NODE] = n;
        }

        NodeHandler(const NodeHandler&) = default;
        NodeHandler& operator=(const NodeHandler&) = default;

        NodeHandler(NodeHandler&&) noexcept = default;
        NodeHandler& operator=(NodeHandler&&) noexcept = default;

        ~NodeHandler() {
            // Der Destruktor sorgt dafür, dass der Speicher für den aktuellen NodeHandler freigegeben wird,
            // ohne die darunter liegenden Nodes zu beeinträchtigen.
            m_Nodes[NODE] = nullptr;
        }

        void down(bool left, int bitPos);

        bool isNull() const {
            return m_Nodes[NODE] == nullptr;
        }

        Node* node(int kind) const {
            return static_cast<Node*>(m_Nodes[kind]);
        }

        void set(Node* n, int kind);

        void search(const std::string& key, int maxPos);

        void searchExact(const std::string& key);

    private:
        PatriciaTree* m_Tree;
        Node* m_Nodes[2];
    };

public:
void visualizeTree(const Node* node,  std::ostream& out = std::cout, int depth = 0);
    PatriciaTree() : m_root(nullptr) {}

    ~PatriciaTree() {
        // Der Destruktor sorgt dafür, dass der Speicher für den gesamten Baum freigegeben wird.
        delete m_root;
    }

    static bool left(const std::string& key, int bitPos) {                                                                                   
      return static_cast<size_t>(bitPos) < key.size() && (key[bitPos] == '0');

    }

    bool insert(const std::string& key);

    bool remove(const std::string& key);

    Node* getRoot() const {
        return m_root; 
    }

private:
    Node* m_root;

    void inorderTraversal(const Node* node);
};

void PatriciaTree::NodeHandler::searchExact(const std::string& key) {
    size_t maxPos = key.size() - 1;
    size_t index = 0;

    while (!isNull() && index <= maxPos && node(NODE)->m_BitPos < key.size() &&
           left(key, static_cast<int>(index)) == left(node(NODE)->m_Key, static_cast<int>(index))) {
        if (node(NODE)->m_BitPos == static_cast<int>(index)) {
            down(left(key, static_cast<int>(index)), static_cast<int>(index));
        } else {
            break;
        }
        ++index;
    }

    // Überprüfen Sie, ob der aktuelle Knoten nicht nullptr ist und der Schlüssel übereinstimmt
    if (!isNull() && node(NODE)->m_BitPos <= static_cast<int>(maxPos) &&
        key.compare(0, node(NODE)->m_BitPos + 1, node(NODE)->m_Key) == 0) {
        std::cout << "Node found. Key: " << node(NODE)->m_Key << ", BitPos: " << node(NODE)->m_BitPos << std::endl;
    } else {
        std::cout << "Node not found. isNull: " << isNull() << ", lastBitPos: " << node(NODE)->m_BitPos << ", maxPos: " << maxPos << std::endl;
    }
}


void PatriciaTree::NodeHandler::down(bool left, int bitPos) {
    int lastBitPos = -1;

    while (!isNull() && lastBitPos < node(NODE)->m_BitPos && bitPos > node(NODE)->m_BitPos) {
        lastBitPos = node(NODE)->m_BitPos;

        std::cout << "Entering down(): isNull: " << isNull() << ", lastBitPos: " << lastBitPos << ", bitPos: " << bitPos << std::endl;

        // Überprüfen Sie, ob der Node nicht nullptr ist, bevor Sie die down-Methode aufrufen
        if (node(NODE) != nullptr) {
            // Aktualisieren Sie den bitPos-Wert basierend auf dem linken oder rechten Pfad
            bitPos = left ? (node(NODE)->m_Left ? node(NODE)->m_Left->m_BitPos : -1) : (node(NODE)->m_Right ? node(NODE)->m_Right->m_BitPos : -1);

            std::cout << "debug: left=" << left << ", bitPos=" << bitPos << std::endl;

            // Überprüfen Sie, ob der aktuelle Node nicht nullptr ist, bevor Sie die down-Methode erneut aufrufen
            if (node(NODE) != nullptr) {
                // Fügen Sie einen Basisfall hinzu, um die Rekursion zu beenden
                if (lastBitPos == node(NODE)->m_BitPos || bitPos == node(NODE)->m_BitPos) {
                    std::cout << "Debug: Basisfall erreicht. Exiting down()." << std::endl;
                    break;
                }

                down(left, bitPos);
            } else {
                std::cout << "Node is nullptr. Exiting down()." << std::endl;
                break;
            }
        }
    }

    // Überprüfen Sie, ob der Vater nicht nullptr ist, bevor Sie fortsetzen
    if (node(DAD) != nullptr) {
        // Zusätzliche Ausgabe, um den Status zu überprüfen
        std::cout << "End of down(): isNull: " << isNull() << ", lastBitPos: " << lastBitPos << ", bitPos: " << bitPos << std::endl;
    } else {
        std::cout << "Vaterknoten ist nullptr. Exiting down()." << std::endl;
    }
}


void PatriciaTree::NodeHandler::set(Node* n, int kind) {
    Node* dad = node(kind + 1);

    if (dad == nullptr) {
        // Der Vaterknoten ist nullptr, daher setzen Sie den neuen Knoten als Wurzel
        m_Tree->m_root = n;
    } else if (dad->m_Left == node(kind)) {
        dad->m_Left = n;
    } else {
        dad->m_Right = n;
    }

    // Entfernen Sie nicht den Vaterknoten aus dem Baum, setzen Sie ihn stattdessen auf nullptr
    m_Nodes[kind] = n;
    m_Nodes[kind + 1] = nullptr;  // Setzen Sie den Vaterknoten auf nullptr
}


void PatriciaTree::NodeHandler::search(const std::string& key, int maxPos) {
    int lastBitPos = -1;

    while (!isNull() && (lastBitPos < node(NODE)->m_BitPos || maxPos > node(NODE)->m_BitPos)) {
        lastBitPos = node(NODE)->m_BitPos;

        if (left(key, lastBitPos)) {
            if (node(NODE)->m_Left != nullptr) {
                down(true, lastBitPos);
            } else {
                break;  // Exit the loop if there is no left child
            }
        } else {
            if (node(NODE)->m_Right != nullptr) {
                down(false, lastBitPos);
            } else {
                break;  // Exit the loop if there is no right child
            }
        }
    }

    // Check if the current node is within the key range
    if (!isNull() && node(NODE)->m_BitPos <= maxPos) {
        // Compare the key with the node's key up to the current bit position
        if (key.compare(0, node(NODE)->m_BitPos + 1, node(NODE)->m_Key) != 0) {
            std::cout << "Node not found. isNull: " << isNull() << ", lastBitPos: " << lastBitPos << ", maxPos: " << maxPos << std::endl;
        }
    }
}

bool PatriciaTree::insert(const std::string& key) {
    if (m_root == nullptr) {
        m_root = new Node(key, key.size());
        return true;
    }

    NodeHandler h(this, m_root);
    h.searchExact(key);

    if (!h.isNull() && h.node(h.NODE) && h.node(h.NODE)->m_Key == key) {
        std::cout << "Key already exists: " << key << std::endl;
        return false;
    }

    int index = 0;
    while (h.node(h.NODE) && index < static_cast<int>(key.size()) && h.node(h.NODE)->m_BitPos < static_cast<int>(key.size()) &&
           left(key, index) == left(h.node(h.NODE)->m_Key, index)) {
        if (h.node(h.NODE)->m_BitPos == index) {
            h.down(left(key, index), index);
        } else {
            break;
        }
        ++index;
    }

    std::cout << "Inserting key: " << key << ", index: " << index << ", left: " << left(key, index) << std::endl;

    Node* newNode = new Node(key, index);

    if (h.node(h.NODE) && left(key, index)) {
        newNode->m_Left = h.node(h.NODE)->m_Left;
        newNode->m_Right = h.node(h.NODE);
    } else if (h.node(h.NODE)) {
        newNode->m_Left = h.node(h.NODE);
        newNode->m_Right = h.node(h.NODE)->m_Right;
    }

    // Setze die Bit-Position des neuen Knotens
    newNode->m_BitPos = index;

    h.set(newNode, h.NODE);

    std::cout << "After insertion:" << std::endl;
    visualizeTree(m_root);

    return true;
}


bool PatriciaTree::remove(const std::string& key) {
    NodeHandler h(this, m_root);
    h.searchExact(key);

    if (h.isNull() || h.node(h.NODE)->m_Key != key) {
        std::cout << "Cannot remove " << key << ": not in the tree." << std::endl;
        return false;
    } else {
        Node* nodeToDelete = h.node(h.NODE);
        NodeHandler hToDelete(this, h.node(h.DAD));

        // Finden Sie den Nachfolger des zu löschenden Knotens
        NodeHandler successorHandler(this, nodeToDelete);
        successorHandler.down(false, hToDelete.node(h.DAD)->m_BitPos);

        // Überprüfen Sie, ob der Nachfolger gefunden wurde
        if (!successorHandler.isNull()) {
            // Ersetzen Sie den zu löschenden Knoten durch seinen Nachfolger
            nodeToDelete->m_Key = successorHandler.node(successorHandler.NODE)->m_Key;

            // Aktualisieren Sie hToDelete auf den Nachfolger, um ihn zu löschen
            hToDelete = successorHandler;

            // Rufen Sie down() erneut auf, um den eigentlichen Knoten zum Löschen zu finden
            hToDelete.down(false, hToDelete.node(h.DAD)->m_BitPos);

            // Überprüfen Sie, ob der eigentliche Knoten zum Löschen gefunden wurde
            if (hToDelete.node(h.NODE) != nodeToDelete) {
                std::cout << "Node to delete not found." << std::endl;
                return false;
            }
        }

        // Entfernen Sie den zu löschenden Knoten und setzen Sie den entsprechenden Zeiger auf nullptr
        if (hToDelete.node(h.NODE)->m_Left == nodeToDelete) {
            Node* temp = hToDelete.node(h.NODE)->m_Left;
            hToDelete.node(h.NODE)->m_Left = nodeToDelete->m_Left;
            delete temp;
        } else {
            Node* temp = hToDelete.node(h.NODE)->m_Right;
            hToDelete.node(h.NODE)->m_Right = nodeToDelete->m_Right;
            delete temp;
        }

        std::cout << "Removed: " << key << std::endl;

        std::cout << "After removal:" << std::endl;
        visualizeTree(m_root);

        return true;
    }
}

void PatriciaTree::inorderTraversal(const Node* node) {
    if (node) {
        inorderTraversal(node->m_Left);
        std::cout << node->m_Key << " ";
        inorderTraversal(node->m_Right);
    }
}*/

/*void PatriciaTree::visualizeTree(const Node* node, std::ostream& out, int depth) {
    if (node) {
        visualizeTree(node->m_Right, out, depth + 1);

        for (int i = 0; i < depth; ++i) {
            out << "  ";
        }

        out << node->m_Key << " (" << node->m_BitPos << ")" << std::endl;

        visualizeTree(node->m_Left, out, depth + 1);
    } else {
        out << "NULL Node" << std::endl;
    }
}*/

/*void signalHandler(int signal) {
    std::cerr << "Segmentation Fault! Signal: " << signal << std::endl;
    std::cerr << "Exiting..." << std::endl;
    exit(signal);
}*/
/*
int main() {
    // Installiere den Signalhandler
  //  std::signal(SIGSEGV, signalHandler);
    PatriciaTree patriciaTree;

    patriciaTree.insert("joyce");
    patriciaTree.insert("john");  // Hier wird "john" hinzugefügt
    patriciaTree.insert("Er");
    patriciaTree.insert("Er");
   // patriciaTree.remove("Er");
    patriciaTree.remove("joyce");
    patriciaTree.remove("les");
    
    std::cout << "digraph PatriciaTree {" << std::endl;
    patriciaTree.visualizeTree(patriciaTree.getRoot(), std::cout);
    std::cout << "}" << std::endl;

                                                                                                                                                         
   //  std::cout << "Patricia Tree Visualization after insertion:" << std::endl;
//patriciaTree.visualizeTree(patriciaTree.getRoot());



    return 0;
}
*/

/*#include <iostream>
#include <string>
#include <climits>
#include <fstream>

class PatriciaTree {
public:
    PatriciaTree() : m_Root(nullptr) {}

    ~PatriciaTree() {
        deleteTree(m_Root);
    }


    bool insert(const std::string& c);
    bool remove(const std::string& c);

private:
    static bool left(const std::string& key, int bitPos) {
        return (key[0] & (1 << bitPos)) == 0;
    }

    class Node {
    public:
        Node(const std::string& key, int bitPos, Node* succ)
            : m_Key(key), m_BitPos(bitPos), m_Left(left(key, bitPos) ? this : succ),
              m_Right(left(key, bitPos) ? succ : this) {}

        Node(const std::string& key, int bitPos) : Node(key, bitPos, nullptr) {}

        ~Node() {
            delete m_Left;
            delete m_Right;
        }

        std::string m_Key;
        int m_BitPos;
        Node* m_Left;
        Node* m_Right;
    };

    class NodeHandler {
    public:
        enum { NODE = 0, DAD = 1 };

        NodeHandler(Node* n, PatriciaTree* tree) : m_Root(tree) { m_Nodes[NODE] = n; }

        void down(bool left) {
            for (int i = NODES_SIZE - 1; i > 0; --i)
                m_Nodes[i] = m_Nodes[i - 1];
            m_Nodes[NODE] = left ? node(DAD)->m_Left : node(DAD)->m_Right;
        }

        bool isNull() const { return m_Nodes[NODE] == nullptr; }

        Node* node(int kind) const { return static_cast<Node*>(m_Nodes[kind]); }

        void set(Node* n, int kind) {
            if (node(kind + 1) == nullptr)
                m_Root->m_Root = n;
            else if (node(kind) != nullptr ? node(kind + 1)->m_Left == node(kind) :
                     left(n->m_Key, node(kind + 1)->m_BitPos))
                node(kind + 1)->m_Left = n;
            else
                node(kind + 1)->m_Right = n;
            m_Nodes[kind] = n;
        }

        void search(const std::string& c, int maxPos) {
            int lastBitPos = -1;
            while (!isNull() && lastBitPos < node(NODE)->m_BitPos &&
                   maxPos > node(NODE)->m_BitPos) {
                lastBitPos = node(NODE)->m_BitPos;
                down(left(c, lastBitPos));
            }
        }

        void search(const std::string& c) { search(c, INT_MAX); }

        Node* brother(int kind) const {
            Node* dad = node(kind + 1);
            Node* current = node(kind);
            return dad->m_Left == current ? dad->m_Right : dad->m_Left;
        }

    private:
        static const int NODES_SIZE = 3;
        PatriciaTree* m_Root;
        Node* m_Nodes[NODES_SIZE];
    };


    void deleteTree(Node* node) {
        if (node != nullptr) {
            deleteTree(node->m_Left);
            deleteTree(node->m_Right);
            delete node;
        }
    }
        

    Node* m_Root;
};

bool PatriciaTree::insert(const std::string& c) {
    NodeHandler h(m_Root, this);
    h.search(c);
    int index = 0;
    if (h.isNull()) {
        if (h.node(NodeHandler::DAD) != nullptr) {
            while (left(c, index) == left(h.node(NodeHandler::DAD)->m_Key, index) &&
                   index < h.node(NodeHandler::DAD)->m_BitPos)
                ++index;
            if (index == h.node(NodeHandler::DAD)->m_BitPos)
                ++index;
        }
    } else if (h.node(NodeHandler::NODE)->m_Key != c) {
        while (left(c, index) == left(h.node(NodeHandler::NODE)->m_Key, index))
            ++index;
    } else {
        // already inserted
        std::cout << "wurde nicht eingefügt, da schon im Baum ist: " << c << std::endl;
        return false;
    }
    h = NodeHandler(m_Root, this);
    h.search(c, index);
    h.set(new Node(c, index, h.node(NodeHandler::NODE)), NodeHandler::NODE);
    std::cout << "wurde eingefügt: " << c << std::endl;
    return true;
}

bool PatriciaTree::remove(const std::string& c) {
    NodeHandler h(m_Root, this);
    h.search(c);
    if (h.isNull() || h.node(NodeHandler::NODE)->m_Key != c) {
        std::cout << "befindet sich nicht im Baum, deshalb kann nicht gelöscht: " << c << std::endl;
        return false;
    } else {
        NodeHandler h2(h.node(NodeHandler::DAD), this);
        h2.search(h.node(NodeHandler::DAD)->m_Key);
        h.node(NodeHandler::NODE)->m_Key = h.node(NodeHandler::DAD)->m_Key;
        h2.set(h.node(NodeHandler::NODE), NodeHandler::NODE);

        Node* brother = h.brother(NodeHandler::NODE);
        h.set(brother, NodeHandler::DAD);

        // Lösche den übrig gebliebenen Knoten (ehemals DAD)
        if (brother != nullptr) {
            delete brother;
        } else {
            delete h.node(NodeHandler::DAD);
        }
    }
    std::cout << "wurde erfolgreich gelöscht: " << c << std::endl;
    return true;
}

void Graphviz() {

}


int main() {
    PatriciaTree patriciaTree;
    patriciaTree.insert("apple");
    patriciaTree.insert("banana");
    patriciaTree.insert("cherry");
    patriciaTree.insert("cherry");
    

    std::cout << "Tree after insertions:" << std::endl;
    //patriciaTree.printTree();

    patriciaTree.remove("banana");
    patriciaTree.remove("orange");

    /*std::cout << "Tree after removal of 'banana' and 'orange':" << std::endl;
    patriciaTree.printTree();*/

//    return 0;
//}*/


#include <iostream>
#include <string>
#include <climits>
#include <fstream>

class PatriciaTree {
public:
    PatriciaTree() : m_Root(nullptr) {}

    ~PatriciaTree() {
        deleteTree(m_Root);
    }

    bool insert(const std::string& c);
    bool remove(const std::string& c);

private:
    static bool left(const std::string& key, int bitPos) {
        return (key[0] & (1 << bitPos)) == 0;
    }

    class Node {
    public:
        Node(const std::string& key, int bitPos, Node* succ)
            : m_Key(key), m_BitPos(bitPos), m_Left(left(key, bitPos) ? this : succ),
              m_Right(left(key, bitPos) ? succ : this) {}

        Node(const std::string& key, int bitPos) : Node(key, bitPos, nullptr) {}

        ~Node() {
            delete m_Left;
            delete m_Right;
        }

        std::string m_Key;
        int m_BitPos;
        Node* m_Left;
        Node* m_Right;
    };

    class NodeHandler {
    public:
        enum { NODE = 0, DAD = 1 };

        NodeHandler(Node* n, PatriciaTree* tree) : m_Root(tree) { m_Nodes[NODE] = n; }

        void down(bool left) {
            for (int i = NODES_SIZE - 1; i > 0; --i)
                m_Nodes[i] = m_Nodes[i - 1];
            m_Nodes[NODE] = left ? node(DAD)->m_Left : node(DAD)->m_Right;
        }

        bool isNull() const { return m_Nodes[NODE] == nullptr; }

        Node* node(int kind) const { return static_cast<Node*>(m_Nodes[kind]); }

        void set(Node* n, int kind) {
            if (node(kind + 1) == nullptr)
                m_Root->m_Root = n;
            else if (node(kind) != nullptr ? node(kind + 1)->m_Left == node(kind) :
                     left(n->m_Key, node(kind + 1)->m_BitPos))
                node(kind + 1)->m_Left = n;
            else
                node(kind + 1)->m_Right = n;
            m_Nodes[kind] = n;
        }

        void search(const std::string& c, int maxPos) {
            int lastBitPos = -1;
            while (!isNull() && lastBitPos < node(NODE)->m_BitPos &&
                   maxPos > node(NODE)->m_BitPos) {
                lastBitPos = node(NODE)->m_BitPos;
                down(left(c, lastBitPos));
            }
        }

        void search(const std::string& c) { search(c, INT_MAX); }

        Node* brother(int kind) const {
            Node* dad = node(kind + 1);
            Node* current = node(kind);
            return dad->m_Left == current ? dad->m_Right : dad->m_Left;
        }

    private:
        static const int NODES_SIZE = 3;
        PatriciaTree* m_Root;
        Node* m_Nodes[NODES_SIZE];
    };

    void deleteTree(Node* node) {
        if (node != nullptr) {
            deleteTree(node->m_Left);
            deleteTree(node->m_Right);
            delete node;
        }
    }

    Node* m_Root;

    // New functions for visualization
    void visualizeNode(std::ofstream& file, Node* node) const {
        file << "  \"" << node->m_Key << "\" [label=\"" << node->m_Key << "\\n" << node->m_BitPos << "\"]" << std::endl;

        if (node->m_Left != nullptr) {
            file << "  \"" << node->m_Key << "\" -> \"" << node->m_Left->m_Key << "\" [label=\"0\"]" << std::endl;
            visualizeNode(file, node->m_Left);
        }

        if (node->m_Right != nullptr) {
            file << "  \"" << node->m_Key << "\" -> \"" << node->m_Right->m_Key << "\" [label=\"1\"]" << std::endl;
            visualizeNode(file, node->m_Right);
        }
    }

public:
    void visualizeTree(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file: " << filename << std::endl;
            return;
        }

        file << "digraph PatriciaTree {" << std::endl;
        visualizeNode(file, m_Root);
        file << "}" << std::endl;

        file.close();
        std::cout << "Tree visualization exported to: " << filename << std::endl;
    }
};

bool PatriciaTree::insert(const std::string& c) {
    NodeHandler h(m_Root, this);
    h.search(c);
    int index = 0;
    if (h.isNull()) {
        if (h.node(NodeHandler::DAD) != nullptr) {
            while (left(c, index) == left(h.node(NodeHandler::DAD)->m_Key, index) &&
                   index < h.node(NodeHandler::DAD)->m_BitPos)
                ++index;
            if (index == h.node(NodeHandler::DAD)->m_BitPos)
                ++index;
        }
    } else if (h.node(NodeHandler::NODE)->m_Key != c) {
        while (left(c, index) == left(h.node(NodeHandler::NODE)->m_Key, index))
            ++index;
    } else {
        // already inserted
        std::cout << "wurde nicht eingefügt, da schon im Baum ist: " << c << std::endl;
        return false;
    }
    h = NodeHandler(m_Root, this);
    h.search(c, index);
    h.set(new Node(c, index, h.node(NodeHandler::NODE)), NodeHandler::NODE);
    std::cout << "wurde eingefügt: " << c << std::endl;
    return true;
}

bool PatriciaTree::remove(const std::string& c) {
    NodeHandler h(m_Root, this);
    h.search(c);
    if (h.isNull() || h.node(NodeHandler::NODE)->m_Key != c) {
        std::cout << "befindet sich nicht im Baum, deshalb kann nicht gelöscht: " << c << std::endl;
        return false;
    } else {
        NodeHandler h2(h.node(NodeHandler::DAD), this);
        h2.search(h.node(NodeHandler::DAD)->m_Key);
        h.node(NodeHandler::NODE)->m_Key = h.node(NodeHandler::DAD)->m_Key;
        h2.set(h.node(NodeHandler::NODE), NodeHandler::NODE);

        Node* brother = h.brother(NodeHandler::NODE);
        h.set(brother, NodeHandler::DAD);

        // Lösche den übrig gebliebenen Knoten (ehemals DAD)
        if (brother != nullptr) {
            delete brother;
        } else {
            delete h.node(NodeHandler::DAD);
        }
    }
    std::cout << "wurde erfolgreich gelöscht: " << c << std::endl;
    return true;
}

void Graphviz(const PatriciaTree& tree, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return;
    }

    file << "digraph PatriciaTree {" << std::endl;
    tree.visualizeTree(filename);
    file << "}" << std::endl;

    file.close();
    std::cout << "Tree visualization exported to: " << filename << std::endl;
}

int main() {
    PatriciaTree patriciaTree;
    char operation;
    std::string input;

    while (std::cin >> operation) {
        if (operation == 'I' || operation == 'D' || operation == 'S') {
            if (!(std::cin >> input)) {
                std::cerr << "Error reading input. Aborted." << std::endl;
                break;
            }

            switch (operation) {
                case 'I':
                    patriciaTree.insert(input);
                    break;
                case 'D':
                    patriciaTree.remove(input);
                    break;
                case 'S':
                    // Implement the search operation if needed
                    break;
            }
        } else {
            std::cerr << "Unknown operation: " << operation << std::endl;
            std::cin.clear();  // clear the error flag
            std::cin.ignore(100, '\n');  // discard invalid input
        }
    }

    Graphviz(patriciaTree, "tree.dot");

    return 0;
}

