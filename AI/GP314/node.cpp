#include "node.h"

Node::Node() {

}

double Node::evaluate(const vector<double>& features) {
    if (left_child == NULL && right_child == NULL) { //  leaf node
        return output;
    } else {
        if (features[feature_index] < threshold) { // left child
            return left_child->evaluate(features);
        } else { //  right child
            return right_child->evaluate(features);
        }
    }
}

void Node::printTree(Node* node, string indent) {
    if(node == NULL)
        return;

    if(node->right_child) {
        printTree(node->right_child, indent + "   ");
        cout<<indent<<" / "<<endl;
        cout<<indent<<node->threshold<<endl;
    }

    if(node->left_child) {
        cout<<indent<<" \\ "<<endl;
        printTree(node->left_child, indent + "   ");
    }
}
